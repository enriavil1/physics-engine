#include "./systemState.hpp"
#include "physicsObjects/physicsObject.hpp"

#include "../forces/gravity.hpp"

#include <iostream>
#include <valarray>
#include <vector>

std::chrono::time_point<std::chrono::system_clock> SystemState::last_update =
    std::chrono::system_clock::now();

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

void SystemState::Update(const float dt) {

  Gravity gravity;
  for (PhysicsObject *obj : SystemState::objects) {
    obj->update(dt);
    obj->applyForce(gravity);
    obj->constraint(obj->getPosition());
  }
}

void SystemState::ResolveCollisions() {
  constexpr float response_coef = 1.0f;
  constexpr float eps = 0.0001f;

  for (PhysicsObject *obj : SystemState::objects) {
    for (PhysicsObject *obj_2 : SystemState::objects) {
      // dont want to check against itself (always collision)
      if (obj == obj_2) {
        continue;
      }

      const ImVec2 position_1 = obj->getPosition();
      float position_1_x = position_1.x;
      float position_1_y = position_1.y;

      const ImVec2 distance_from_center_1 = obj->getDistanceFromCenter();

      const ImVec2 position_2 = obj_2->getPosition();
      float position_2_x = position_2.x;
      float position_2_y = position_2.y;

      const bool is_obj_1_righty = position_1_x < position_2_x;
      const bool is_obj_1_under = position_1_y < position_2_y;

      const ImVec2 distance_from_center_2 = obj_2->getDistanceFromCenter();

      const auto dst_x = position_2_x - position_1_x;

      const auto dst_y = position_2_y - position_1_y;

      const float dst2 = (dst_x * dst_x) + (dst_y * dst_y);

      const float min_distance_x =
          (distance_from_center_1.x - distance_from_center_2.x);

      const float min_distance_y =
          (distance_from_center_1.y - distance_from_center_2.y);

      const float min_distance =
          (min_distance_x * min_distance_x) + (min_distance_y * min_distance_y);

      if (dst2 < min_distance * 0.5f && dst2 > eps) {
        const float dist = sqrt(dst2);
        const float delta = response_coef * 0.5f * (sqrt(min_distance) - dist);

        const auto collision_vec =
            ImVec2((dst_x / dist) * delta, (dst_y / dist) * delta);

        const auto x_collision =
            is_obj_1_righty ? collision_vec.x : collision_vec.x * -1.0f;
        position_1_x -= x_collision;
        position_2_x += x_collision;

        const auto y_collision =
            is_obj_1_under ? collision_vec.y : collision_vec.y * -1.0f;

        position_1_y -= collision_vec.y;
        position_2_y += collision_vec.y;

        const auto new_position_1 = ImVec2(position_1_x, position_1_y);
        obj->setPosition(new_position_1);

        const auto new_position_2 = ImVec2(position_2_x, position_2_y);
        obj_2->setPosition(new_position_2);
      }
    }
  }
}
