#pragma once

#include <chrono>
#include <vector>

#include "physicsObjects/circleObject.hpp"
#include "physicsObjects/physicsObject.hpp"

typedef uint32_t uint;

class SystemState {
private:
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
  static void Update(const float dt);

  static void ResolveCollisions();
};
