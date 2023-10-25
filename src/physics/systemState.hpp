#pragma once

#include <algorithm>
#include <cmath>
#include <memory>
#include <vector>

#include "physics_objects/circleObject.hpp"
#include "physics_objects/physicsObject.hpp"

#include "grid/grid.hpp"
#include "grid/gridCell.hpp"

#include "../thread_pool/threadPool.hpp"

class SystemState {
private:
  static std::shared_ptr<PhysicsObject> m_picked_object;

  static ThreadPool sm_thread_pool;

  static Grid sm_grid;

  static std::vector<std::shared_ptr<PhysicsObject>> objects;

  static void DistanceFromTwoObjects(std::shared_ptr<PhysicsObject> obj_1,
                                     std::shared_ptr<PhysicsObject> obj_2,
                                     float& distance);

  // Circle object specific
  static bool CheckCircleCollision(std::shared_ptr<CircleObject> circle_1,
                                   std::shared_ptr<CircleObject> circle_2,
                                   float& distance);
  static void ResolveCircleCollision(std::shared_ptr<CircleObject> circle_1,
                                     std::shared_ptr<CircleObject> circle_2,
                                     float& distance);

  static void ResolveCellCollisions(std::shared_ptr<PhysicsObject> obj,
                                    GridCell& cell);
  static void ResolveNeighborCellCollisions(std::shared_ptr<PhysicsObject> obj,
                                            uint32_t& pos_x, uint32_t& pos_y);

  static void ResolveMultiThreadedCollisions();
  static void ResolveSingleThreadedCollisions();

public:
  static uint32_t GetObjectAmount();
  static void AddObject(std::shared_ptr<PhysicsObject> object);

  static void Draw();
  static void Update(float dt);

  static void ResolveCollisions();

  static void UpdateWindowSpecs(uint32_t width, uint32_t height, ImVec2 pos) {
    SystemState::sm_grid.updateWindowSpecs(width, height, pos);
  }

  static void SetPickedObject(const float& mouse_pos_x,
                              const float& mouse_pos_y) {
    for (std::shared_ptr<PhysicsObject> obj : SystemState::objects) {
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
