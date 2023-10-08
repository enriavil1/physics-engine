#pragma once

#include "iostream"
#include <chrono>
#include <vector>

#include <SDL3/SDL.h>

#include "physicsObjects/circleObject.hpp"
#include "physicsObjects/physicsObject.hpp"

typedef uint32_t uint;

class SystemState {
private:
  static PhysicsObject *m_picked_object;

  static std::vector<PhysicsObject *> objects;

  static void DistanceFromTwoObjects(PhysicsObject *obj_1, PhysicsObject *obj_2,
                                     float &distance);
  static bool CheckCircleCollision(CircleObject *circle_1,
                                   CircleObject *circle_2, float &distance);
  static void ResolveCircleCollision(CircleObject *circle_1,
                                     CircleObject *circle_2, float &distance);

public:
  static uint GetObjectAmount();
  static void AddObject(PhysicsObject *object);

  static void Draw();
  static void Update();

  static void ResolveCollisions();

  static void SetPickedObject() {
    float m_pos_x;
    float m_pos_y;

    SDL_GetMouseState(&m_pos_x, &m_pos_y);

    for (PhysicsObject *obj : SystemState::objects) {
      const ImVec2 obj_pos = obj->getPosition();
      const ImVec2 distance_from_center = obj->getDistanceFromCenter();

      std::cout << "x: " << m_pos_x << " y: " << m_pos_y << std::endl;

      const float lower_x = obj_pos.x - distance_from_center.x;
      const float higher_x = obj_pos.x + distance_from_center.x;

      const float lower_y = obj_pos.y - distance_from_center.y;
      const float higher_y = obj_pos.y + distance_from_center.y;

      // find the object that the mouse is inside of
      if ((lower_x <= m_pos_x && m_pos_x <= higher_x) &&
          (lower_y <= m_pos_y && m_pos_y <= higher_y)) {
        SystemState::m_picked_object = obj;
        break;
      }
    }
  };

  static void UpdatePickedObject(const ImGuiIO &io) {
    float m_pos_x;
    float m_pos_y;

    SDL_GetMouseState(&m_pos_x, &m_pos_y);

    if (SystemState::m_picked_object != nullptr) {
      SystemState::m_picked_object->setPosition(ImVec2(m_pos_x, m_pos_y));
    }
  }

  static void UnsetPickedObject() { SystemState::m_picked_object = nullptr; }
};
