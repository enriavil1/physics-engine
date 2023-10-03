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

void SystemState::Update() {
  auto current_time = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds = current_time - last_update;

  auto elapsed_seconds_count = elapsed_seconds.count();

  Gravity gravity;
  for (PhysicsObject *obj : SystemState::objects) {
    obj->update(elapsed_seconds_count);
    obj->applyForce(gravity);
  }
  SystemState::last_update = current_time;
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

      ImVec2 position_1 = obj->getPosition();
      ImVec2 distance_from_center_1 = obj->getDistanceFromCenter();

      ImVec2 position_2 = obj_2->getPosition();
      ImVec2 distance_from_center_2 = obj_2->getDistanceFromCenter();

      auto dst_x = position_1.x - position_2.x;
      auto dst_y = position_1.y - position_2.y;

      ImVec2 p2_p1 = ImVec2(dst_x, dst_y);

      const float dst2 = (dst_x * dst_x) + (dst_y * dst_y);

      const float min_distance =
          (distance_from_center_1.x + distance_from_center_2.x) *
          (distance_from_center_1.y + distance_from_center_2.y);

      if (dst2 < min_distance) {
        const float dist = sqrt(dst2);
        std::cout << "x: " << dst_x << " y: " << dst_y << " dst2 " << dst2
                  << " min-dst " << min_distance << "dist " << dist
                  << std::endl;
        const float delta = response_coef * 0.5f * (min_distance - dist);

        const auto collision_vec = ImVec2(p2_p1.x / dist, p2_p1.y / dist);
        position_1.x = position_1.x + collision_vec.x;
        position_2.x = position_2.x - collision_vec.x;

        position_1.y = position_1.y + collision_vec.y;
        position_2.y = position_2.y - collision_vec.y;
        std::cout << "here\n";
      }

      // need to make sure new positions are not out ouf bound of the screen
      SystemState::makeSureObjectInBound(&position_1, distance_from_center_1);
      SystemState::makeSureObjectInBound(&position_2, distance_from_center_2);

      obj->setPosition(position_1);
      obj_2->setPosition(position_2);
    }
  }
}

void SystemState::makeSureObjectInBound(ImVec2 *pos,
                                        const ImVec2 &distance_from_center) {
  ImVec2 window_size = ImGui::GetMainViewport()->Size;

  if (pos->x + distance_from_center.x >= window_size.x) {
    pos->x = window_size.x - distance_from_center.x;
  }

  if (pos->x - distance_from_center.x <= 0) {
    pos->x = distance_from_center.x;
  }

  if (pos->y + distance_from_center.y >= window_size.y) {
    pos->y = window_size.y - distance_from_center.y;
  }

  if (pos->y - distance_from_center.y <= 0) {
    pos->y = distance_from_center.y;
  }
}
