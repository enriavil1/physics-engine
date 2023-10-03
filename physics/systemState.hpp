#pragma once

#include <chrono>
#include <vector>

#include "./physicsObjects/PhysicsObject.hpp"

typedef unsigned int uint;

class SystemState {
private:
  static std::chrono::time_point<std::chrono::system_clock> last_update;
  static std::vector<PhysicsObject *> objects;

  static void makeSureObjectInBound(ImVec2 *pos,
                                    const ImVec2 &distance_from_center);

public:
  static uint GetObjectAmount();
  static void AddObject(PhysicsObject *object);

  static void Draw();
  static void Update();

  static void ResolveCollisions();
};
