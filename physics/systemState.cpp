#include "./systemState.hpp"
#include "physicsObjects/PhysicsObject.hpp"

#include "../forces/gravity.hpp"

#include <iostream>
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

  for (PhysicsObject *obj : SystemState::objects) {
    for (PhysicsObject *obj_2 : SystemState::objects) {
      // dont want to check against itself (always collision)
      if (obj == obj_2) {
        continue;
      }

      ImVec2 position_1 = obj->getPosition();
      ImVec2 distance_from_center_1 = obj->getDistanceFromCenter();

      // following left top right bottom
      ImVec4 all_edges_position =
          ImVec4(position_1.x - distance_from_center_1.x,
                 position_1.y + distance_from_center_1.y,
                 position_1.x + distance_from_center_1.x,
                 position_1.y - distance_from_center_1.y);

      ImVec2 position_2 = obj_2->getPosition();
      ImVec2 distance_from_center_2 = obj_2->getDistanceFromCenter();
      ImVec4 all_edges_position_2 =
          ImVec4(position_2.x - distance_from_center_2.x,
                 position_2.y + distance_from_center_2.y,
                 position_2.x + distance_from_center_2.x,
                 position_2.y - distance_from_center_2.y);

      // if object one is left side and its between object two
      if (all_edges_position_2.x <= all_edges_position.x &&
          all_edges_position.x <= all_edges_position_2.w) {
        position_1.x = position_1.x - distance_from_center_1.x / 2;
        position_2.x = position_2.x + distance_from_center_2.x / 2;
      }

      // second one checks if object two is in the left side and its inside of
      // object 1
      if (all_edges_position.x <= all_edges_position_2.x &&
          all_edges_position_2.x <= all_edges_position.w) {
        position_1.x = position_1.x - distance_from_center_1.x / 2;
        position_2.x = position_2.x + distance_from_center_2.x / 2;
      }

      if (all_edges_position_2.y <= all_edges_position.y &&
          all_edges_position.y <= all_edges_position_2.z) {
        position_1.y = position_1.y + distance_from_center_1.y / 2;
        position_2.y = position_2.y - distance_from_center_2.y / 2;
      }

      if (all_edges_position.y <= all_edges_position_2.y &&
          all_edges_position_2.y <= all_edges_position.z) {
        position_1.y = position_1.y - distance_from_center_1.y / 2;
        position_2.y = position_2.y + distance_from_center_2.y / 2;
      }

      // need to make sure new positions are not out ouf bound of the screen
      SystemState::makeSureObjectInBound(&position_1, distance_from_center_1);
      SystemState::makeSureObjectInBound(&position_2, distance_from_center_2);
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
