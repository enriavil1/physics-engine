#pragma once

#include <algorithm>
#include <cmath>
#include <vector>

#include <SDL3/SDL.h>

#include "grid/gridCell.hpp"
#include "physics_objects/circleObject.hpp"
#include "physics_objects/physicsObject.hpp"

#include "grid/grid.hpp"
#include "grid/gridCell.hpp"

#include "../thread_pool/threadPool.hpp"

class SystemState {
private:
  static PhysicsObject *m_picked_object;

  static Grid sm_grid;
  static ThreadPool sm_thread_pool;

  static std::vector<PhysicsObject *> objects;

  static void MultiThreadUpdate(float dt, float& max_width, float& max_height);
  static void SingleThreadUpdate(float dt, float& max_width, float& max_height);
  static void ApplyObjectUpdate(PhysicsObject *obj, float dt, float& max_width,
                                float& max_height);

  static void DistanceFromTwoObjects(PhysicsObject *obj_1, PhysicsObject *obj_2,
                                     float& distance);

  // dealing with different amount of threads and collisions
  static void ResolveSingleThreadCollisions();
  static void ResolveMultiThreadCollisions();

  static void ResolveNeighborCollisions(PhysicsObject *obj);
  static void ResolveCellCollisions(PhysicsObject *obj, GridCell& cell);

  // Circle object specific
  static bool CheckCircleCollision(CircleObject *circle_1,
                                   CircleObject *circle_2, float& distance);
  static void ResolveCircleCollision(CircleObject *circle_1,
                                     CircleObject *circle_2, float& distance);

public:
  static uint32_t GetObjectAmount();
  static void AddObject(PhysicsObject *object);

  static void Draw();
  static void Update(float dt);

  static void ResolveCollisions();

  static void SetPickedObject(const float& mouse_pos_x,
                              const float& mouse_pos_y) {
    for (PhysicsObject *obj : SystemState::objects) {
      const ImVec2 obj_pos = obj->getPosition();
      const ImVec2 distance_from_center = obj->getDistanceFromCenter();

      const float lower_x = obj_pos.x - distance_from_center.x;
      const float higher_x = obj_pos.x + distance_from_center.x;

      const float lower_y = obj_pos.y - distance_from_center.y;
      const float higher_y = obj_pos.y + distance_from_center.y;

      // find the object that the mouse is inside of
      if ((lower_x <= mouse_pos_x && mouse_pos_x <= higher_x) &&
          (lower_y <= mouse_pos_y && mouse_pos_y <= higher_y)) {
        SystemState::m_picked_object = obj;
        break;
      }
    }
  };

  static void UpdatePickedObject(const float& mouse_pos_x,
                                 const float& mouse_pos_y) {
    if (SystemState::m_picked_object != nullptr) {
      SystemState::m_picked_object->setPosition(
          ImVec2(mouse_pos_x, mouse_pos_y));
    }
  }

  static void UnsetPickedObject() { SystemState::m_picked_object = nullptr; }
};
