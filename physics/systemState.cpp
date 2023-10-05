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
    }
  }
}
