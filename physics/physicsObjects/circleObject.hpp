#pragma once

#include "../../forces/gravity.hpp"
#include "./PhysicsObject.hpp"

#include <chrono>

class CircleObject : public PhysicsObject {
private:
  float radius = 0.0;

public:
  CircleObject(float mass, float x, float y, float radius) {
    this->mass = mass;
    this->radius = radius;
    this->position = ImVec2(x, y);
  }

  ImVec2 getDistanceFromCenter();
  void draw();

  void update(double dt);
};
