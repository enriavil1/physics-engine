#pragma once

#include <chrono>
#include <vector>

#include "./physicsObjects/physicsObject.hpp"

typedef uint32_t uint;

class SystemState {
private:
  static std::chrono::time_point<std::chrono::system_clock> last_update;
  static std::vector<PhysicsObject *> objects;

public:
  static uint GetObjectAmount();
  static void AddObject(PhysicsObject *object);

  static void Draw();
  static void Update(const float dt);

  static void ResolveCollisions();
};
