#include "./systemState.hpp"

#define MINIMUM_CELL_WIDTH 2
#define MINIMUM_CELL_HEIGHT 2

#define SUB_STEP_DIVISION 8.0f

#define EPSILON 0.0001f

std::shared_ptr<PhysicsObject> SystemState::m_picked_object = nullptr;

ThreadPool SystemState::sm_thread_pool = ThreadPool(5);

Grid SystemState::sm_grid = Grid();

std::vector<std::shared_ptr<PhysicsObject>> SystemState::objects =
    std::vector<std::shared_ptr<PhysicsObject>>{};

uint32_t SystemState::GetObjectAmount() { return SystemState::objects.size(); }

void SystemState::AddObject(std::shared_ptr<PhysicsObject> object) {
  SystemState::objects.push_back(object);
}

void SystemState::Draw() {
  for (auto obj : SystemState::objects) {
    obj->draw();
  }
}

void SystemState::Update(float dt) {
  Gravity gravity;

  const float sub_step = dt / SUB_STEP_DIVISION;
  float max_width = MINIMUM_CELL_WIDTH;
  float max_height = MINIMUM_CELL_HEIGHT;

  // sub steps will make us low frame rate resistant
  for (float i = 0; i < dt; i += sub_step) {
    for (auto obj : SystemState::objects) {
      // we dont apply gravity onto the object we pick up
      if (obj != SystemState::m_picked_object) {
        obj->applyForce(&gravity);
      }
      obj->update(sub_step);
      obj->constraint(obj->getPosition());

      max_width = std::max(max_width, obj->getDistanceFromCenter().x);
      max_height = std::max(max_height, obj->getDistanceFromCenter().y);
    }

    SystemState::sm_grid.clear();
    SystemState::sm_grid.updateCellWidthAndHeight(max_width * 2,
                                                  max_height * 2);
    for (const auto obj : SystemState::objects) {
      SystemState::sm_grid.add(obj);
    }
    // every sub step we should resolve collisions of all balls and update draw
    SystemState::ResolveCollisions();
    SystemState::Draw();
  }
}

void SystemState::ResolveCellCollisions(std::shared_ptr<PhysicsObject> obj,
                                        GridCell& cell) {
  for (auto obj_2 : cell.getObjects()) {
    if (obj == obj_2) {
      continue;
    }

    float distance = 0.0f;

    const bool has_collision = SystemState::CheckCircleCollision(
        std::dynamic_pointer_cast<CircleObject>(obj),
        std::dynamic_pointer_cast<CircleObject>(obj_2), distance);

    if (has_collision) {
      SystemState::ResolveCircleCollision(
          std::dynamic_pointer_cast<CircleObject>(obj),
          std::dynamic_pointer_cast<CircleObject>(obj_2), distance);
    }
  }
}

void SystemState::ResolveNeighborCellCollisions(
    std::shared_ptr<PhysicsObject> obj, uint32_t& pos_x, uint32_t& pos_y) {
  uint32_t width = SystemState::sm_grid.getWidth();
  uint32_t height = SystemState::sm_grid.getHeight();

  SystemState::ResolveCellCollisions(obj, SystemState::sm_grid.getCell(obj));

  // check left
  SystemState::ResolveCellCollisions(
      obj, SystemState::sm_grid.getCell(pos_x - width, pos_y));

  // check above
  SystemState::ResolveCellCollisions(
      obj, SystemState::sm_grid.getCell(pos_x, pos_y - height));

  // check right
  SystemState::ResolveCellCollisions(
      obj, SystemState::sm_grid.getCell(pos_x + width, pos_y));

  // check below
  SystemState::ResolveCellCollisions(
      obj, SystemState::sm_grid.getCell(pos_x, pos_y + height));

  // check right and above
  SystemState::ResolveCellCollisions(
      obj, SystemState::sm_grid.getCell(pos_x + width, pos_y - height));

  // check left and above
  SystemState::ResolveCellCollisions(
      obj, SystemState::sm_grid.getCell(pos_x - width, pos_y - height));

  // check below and right
  SystemState::ResolveCellCollisions(
      obj, SystemState::sm_grid.getCell(pos_x + width, pos_y + height));

  // check below and left
  SystemState::ResolveCellCollisions(
      obj, SystemState::sm_grid.getCell(pos_x - width, pos_y + height));
}

void SystemState::ResolveMultiThreadedCollisions() {
  const uint32_t slice_size =
      std::max(static_cast<int>(SystemState::objects.size() /
                                SystemState::sm_thread_pool.getThreadCount()),
               1);

  for (uint32_t i = 0; i < SystemState::objects.size() / 2; i += slice_size) {
    SystemState::sm_thread_pool.addTask([i, slice_size]() {
      for (uint32_t pos = i;
           pos < i + slice_size && pos < SystemState::objects.size(); ++pos) {
        auto obj = SystemState::objects[pos];
        uint32_t pos_x = obj->getPosition().x;
        uint32_t pos_y = obj->getPosition().y;

        SystemState::ResolveNeighborCellCollisions(obj, pos_x, pos_y);
      }
    });
  }

  SystemState::sm_thread_pool.waitForCompletion();

  for (uint32_t i = SystemState::objects.size() / 2;
       i < SystemState::objects.size(); i += slice_size) {
    SystemState::sm_thread_pool.addTask([i, slice_size]() {
      for (uint32_t pos = i;
           pos < i + slice_size && pos < SystemState::objects.size(); ++pos) {
        auto obj = SystemState::objects[pos];
        uint32_t pos_x = obj->getPosition().x;
        uint32_t pos_y = obj->getPosition().y;

        SystemState::ResolveNeighborCellCollisions(obj, pos_x, pos_y);
      }
    });
  }

  SystemState::sm_thread_pool.waitForCompletion();
}

void SystemState::ResolveSingleThreadedCollisions() {
  for (auto obj : SystemState::objects) {

    uint32_t pos_x = obj->getPosition().x;
    uint32_t pos_y = obj->getPosition().y;

    SystemState::ResolveNeighborCellCollisions(obj, pos_x, pos_y);
  }
}

void SystemState::ResolveCollisions() {
  SystemState::ResolveMultiThreadedCollisions();
  // SystemState::ResolveSingleThreadedCollisions();
}

void SystemState::DistanceFromTwoObjects(std::shared_ptr<PhysicsObject> obj_1,
                                         std::shared_ptr<PhysicsObject> obj_2,
                                         float& distance) {
  const float pos_1_x = obj_1->getPosition().x;
  const float pos_1_y = obj_1->getPosition().y;

  const float pos_2_x = obj_2->getPosition().x;
  const float pos_2_y = obj_2->getPosition().y;

  distance = pow(pos_1_x - pos_2_x, 2) + pow(pos_1_y - pos_2_y, 2);
}

bool SystemState::CheckCircleCollision(std::shared_ptr<CircleObject> circle_1,
                                       std::shared_ptr<CircleObject> circle_2,
                                       float& distance) {
  if (circle_1 == nullptr || circle_2 == nullptr) {
    return false;
  }
  // x or y are both the same thing
  // since the radius is constant
  const float distance_from_center_1 = circle_1->getDistanceFromCenter().x;
  const float distance_from_center_2 = circle_2->getDistanceFromCenter().x;

  const float min_distance =
      pow(distance_from_center_1 + distance_from_center_2, 2);

  SystemState::DistanceFromTwoObjects(
      std::dynamic_pointer_cast<PhysicsObject>(circle_1),
      std::dynamic_pointer_cast<PhysicsObject>(circle_2), distance);

  return min_distance > distance - EPSILON && distance > EPSILON;
}

void SystemState::ResolveCircleCollision(std::shared_ptr<CircleObject> circle_1,
                                         std::shared_ptr<CircleObject> circle_2,
                                         float& distance) {
  const float distance_from_center_1 = circle_1->getDistanceFromCenter().x;
  const float distance_from_center_2 = circle_2->getDistanceFromCenter().x;

  const float min_distance = distance_from_center_1 + distance_from_center_2;

  // distance will be our vector normalizer
  distance = sqrtf(distance);

  // we calculate the overlap of the circle
  const float overlap_dist = 0.5f * (distance - min_distance);

  const ImVec2 pos_1 = circle_1->getPosition();
  const ImVec2 pos_2 = circle_2->getPosition();

  const float x_vector = (pos_1.x - pos_2.x) / distance;
  const float x_displacement = overlap_dist * x_vector;

  const float y_vector = (pos_1.y - pos_2.y) / distance;
  const float y_displacement = overlap_dist * y_vector;

  float new_pos_1_x = pos_1.x - x_displacement;
  float new_pos_1_y = pos_1.y - y_displacement;

  float new_pos_2_x = pos_2.x + x_displacement;
  float new_pos_2_y = pos_2.y + y_displacement;

  circle_1->setPosition(ImVec2(new_pos_1_x, new_pos_1_y));
  circle_2->setPosition(ImVec2(new_pos_2_x, new_pos_2_y));

  // do dynamic collisions when neither object is being held
  if (SystemState::m_picked_object !=
          std::dynamic_pointer_cast<PhysicsObject>(circle_1) &&
      SystemState::m_picked_object !=
          std::dynamic_pointer_cast<PhysicsObject>(circle_2)) {

    // resolve velocity
    const ImVec2 circle_1_vec = circle_1->getVelocity();
    const ImVec2 circle_2_vec = circle_2->getVelocity();

    // dot product normal
    const float dp_num_1 =
        circle_1_vec.x * x_vector + circle_1_vec.y * y_vector;
    const float dp_num_2 =
        circle_2_vec.x * x_vector + circle_2_vec.y * y_vector;

    const float tangent_x = -1.0f * y_vector;
    const float tangent_y = x_vector;

    // dot product tangent
    const float dp_tan_1 =
        circle_1_vec.x * tangent_x + circle_1_vec.y * tangent_y;
    const float dp_tan_2 =
        circle_2_vec.x * tangent_x + circle_2_vec.y * tangent_y;

    const float total_mass = circle_1->getMass() + circle_2->getMass();

    const float m1 = (dp_num_1 * (circle_1->getMass() - circle_2->getMass()) +
                      (2.0f * circle_2->getMass() * dp_num_2)) /
                     total_mass;
    const float m2 = (dp_num_2 * (circle_2->getMass() - circle_1->getMass()) +
                      (2.0f * circle_1->getMass() * dp_num_1)) /
                     total_mass;

    const float new_vel_1_x = tangent_x * dp_tan_1 + x_vector * m1;
    const float new_vel_1_y = tangent_y * dp_tan_1 + y_vector * m1;

    const float new_vel_2_x = tangent_x * dp_tan_2 + x_vector * m2;
    const float new_vel_2_y = tangent_y * dp_tan_2 + y_vector * m2;

    circle_1->setVelocity(ImVec2(new_vel_1_x, new_vel_1_y));
    circle_2->setVelocity(ImVec2(new_vel_2_x, new_vel_2_y));
  }
}
