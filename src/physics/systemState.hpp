#pragma once

#include <cmath>
#include <vector>

#include <SDL3/SDL.h>

#include "grid/gridCell.hpp"
#include "physicsObjects/circleObject.hpp"
#include "physicsObjects/physicsObject.hpp"

#include "grid/grid.hpp"

class SystemState {
private:
  static PhysicsObject *m_picked_object;

  static Grid sm_grid;

  static std::vector<PhysicsObject *> objects;

  static void DistanceFromTwoObjects(PhysicsObject *obj_1, PhysicsObject *obj_2,
                                     float& distance);
  static bool CheckCircleCollision(CircleObject *circle_1,
                                   CircleObject *circle_2, float& distance);
  static void ResolveCircleCollision(CircleObject *circle_1,
                                     CircleObject *circle_2, float& distance);

  static void ResolveCellCollisions(PhysicsObject *obj, GridCell& cell);

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
