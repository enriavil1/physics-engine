#include "./systemState.hpp"

#define SUB_STEP_DIVISION 4.0f

PhysicsObject *SystemState::m_picked_object = nullptr;

std::vector<PhysicsObject *> SystemState::objects =
    std::vector<PhysicsObject *>{};

uint SystemState::GetObjectAmount() { return SystemState::objects.size(); }

void SystemState::AddObject(PhysicsObject *object) {
  SystemState::objects.push_back(object);
}

void SystemState::Draw() {
  for (PhysicsObject *obj : SystemState::objects) {
    obj->draw();
  }
}

void SystemState::Update(float dt) {
  Gravity gravity;

  const float sub_steps = dt / SUB_STEP_DIVISION;

  // sub steps will make us low frame rate resistant
  for (float i = sub_steps; i > 0; --i) {
    for (PhysicsObject *obj : SystemState::objects) {
      // we dont apply gravity onto the object we pick up
      if (obj != SystemState::m_picked_object) {
        obj->applyForce(&gravity);
      }
      obj->update(dt);
      obj->constraint(obj->getPosition());
    }

    // every sub step we should resolve collisions of all balls and update draw
    SystemState::ResolveCollisions();
    SystemState::Draw();
  }
}

void SystemState::ResolveCollisions() {
  for (int i = 0; i < SystemState::objects.size(); ++i) {
    for (int j = 0; j < SystemState::objects.size(); ++j) {
      PhysicsObject *obj = SystemState::objects[i];
      PhysicsObject *obj_2 = SystemState::objects[j];

      if (obj == obj_2) {
        continue;
      }

      float distance = 0.0f;

      const bool has_collision = SystemState::CheckCircleCollision(
          reinterpret_cast<CircleObject *>(obj),
          reinterpret_cast<CircleObject *>(obj_2), distance);

      if (has_collision) {
        SystemState::ResolveCircleCollision(
            reinterpret_cast<CircleObject *>(obj),
            reinterpret_cast<CircleObject *>(obj_2), distance);
      }
    }
  }
}

void SystemState::DistanceFromTwoObjects(PhysicsObject *obj_1,
                                         PhysicsObject *obj_2,
                                         float& distance) {
  const float pos_1_x = obj_1->getPosition().x;
  const float pos_1_y = obj_1->getPosition().y;

  const float pos_2_x = obj_2->getPosition().x;
  const float pos_2_y = obj_2->getPosition().y;

  distance = pow(pos_1_x - pos_2_x, 2) + pow(pos_1_y - pos_2_y, 2);
}

bool SystemState::CheckCircleCollision(CircleObject *circle_1,
                                       CircleObject *circle_2,
                                       float& distance) {
  // x or y are both the same thing
  // since the radius is constant
  const float distance_from_center_1 = circle_1->getDistanceFromCenter().x;
  const float distance_from_center_2 = circle_2->getDistanceFromCenter().x;

  const float min_distance =
      pow(distance_from_center_1 + distance_from_center_2, 2);

  SystemState::DistanceFromTwoObjects(
      reinterpret_cast<PhysicsObject *>(circle_1),
      reinterpret_cast<PhysicsObject *>(circle_2), distance);

  return min_distance >= fabs(distance) - ForceConstants::EPSILON &&
         fabs(distance) > ForceConstants::EPSILON;
}

void SystemState::ResolveCircleCollision(CircleObject *circle_1,
                                         CircleObject *circle_2,
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
  if (SystemState::m_picked_object != (PhysicsObject *)circle_1 &&
      SystemState::m_picked_object != (PhysicsObject *)circle_2) {

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

    const float m1 = (dp_num_1 * (circle_1->getMass() - circle_2->getMass()) +
                      (2.0f * circle_2->getMass() * dp_num_2)) /
                     (circle_1->getMass() + circle_2->getMass());
    const float m2 = (dp_num_2 * (circle_2->getMass() - circle_1->getMass()) +
                      (2.0f * circle_1->getMass() * dp_num_1)) /
                     (circle_1->getMass() + circle_2->getMass());

    const float new_vel_1_x = tangent_x * dp_tan_1 + x_vector * m1;
    const float new_vel_1_y = tangent_y * dp_tan_1 + y_vector * m1;

    const float new_vel_2_x = tangent_x * dp_tan_2 + x_vector * m2;
    const float new_vel_2_y = tangent_y * dp_tan_2 + y_vector * m2;

    circle_1->setVelocity(ImVec2(new_vel_1_x, new_vel_1_y));
    circle_2->setVelocity(ImVec2(new_vel_2_x, new_vel_2_y));
  }
}
