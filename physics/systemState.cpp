#include "./systemState.hpp"
#include "physicsObjects/circleObject.hpp"
#include "physicsObjects/physicsObject.hpp"

#include "../forces/gravity.hpp"

#include <iostream>
#include <valarray>
#include <vector>

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

void SystemState::Update() {
  Gravity gravity;

  auto &io = ImGui::GetIO();
  const auto fps_cap = io.Framerate > 60 ? 60.0f : io.Framerate;

  for (PhysicsObject *obj : SystemState::objects) {
    // TODO(enriavil1): apply force is currently hard coded
    obj->applyForce(gravity);
    // TODO(enriavil1): better gravity
    obj->update(1.0f / fps_cap);
    obj->constraint(obj->getPosition());
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
                                         float &distance) {
  const float pos_1_x = obj_1->getPosition().x;
  const float pos_1_y = obj_1->getPosition().y;

  const float pos_2_x = obj_2->getPosition().x;
  const float pos_2_y = obj_2->getPosition().y;

  distance = pow(pos_1_x - pos_2_x, 2) + pow(pos_1_y - pos_2_y, 2);
}

bool SystemState::CheckCircleCollision(CircleObject *circle_1,
                                       CircleObject *circle_2,
                                       float &distance) {
  constexpr float eps = 0.001f;

  // x or y are both the same thing
  // since the radius is constant
  const float distance_from_center_1 = circle_1->getDistanceFromCenter().x;
  const float distance_from_center_2 = circle_2->getDistanceFromCenter().x;

  const float min_distance =
      pow(distance_from_center_1 + distance_from_center_2, 2);

  SystemState::DistanceFromTwoObjects(circle_1, circle_2, distance);

  return min_distance >= fabs(distance) - eps && distance > eps;
}

void SystemState::ResolveCircleCollision(CircleObject *circle_1,
                                         CircleObject *circle_2,
                                         float &distance) {
  const float distance_from_center_1 = circle_1->getDistanceFromCenter().x;
  const float distance_from_center_2 = circle_2->getDistanceFromCenter().x;

  const float min_distance = distance_from_center_1 + distance_from_center_2;

  // distance will be our vector normalizer
  distance = sqrtf(distance);

  // we calculate the overlap of the circle
  const float overlap_dist = 0.5f * (distance - min_distance);

  const ImVec2 pos_1 = circle_1->getPosition();
  const ImVec2 pos_2 = circle_2->getPosition();

  float new_pos_1_x = pos_1.x - (overlap_dist * (pos_1.x - pos_2.x) / distance);
  float new_pos_1_y = pos_1.y - (overlap_dist * (pos_1.y - pos_2.y) / distance);

  float new_pos_2_x = pos_2.x + (overlap_dist * (pos_1.x - pos_2.x) / distance);
  float new_pos_2_y = pos_2.y + (overlap_dist * (pos_1.y - pos_2.y) / distance);

  circle_1->setPosition(ImVec2(new_pos_1_x, new_pos_1_y));
  circle_2->setPosition(ImVec2(new_pos_2_x, new_pos_2_y));
}
