#pragma once

#include <cmath>

#include "./physicsObject.hpp"

#include "../../forces/gravity.hpp"

class CircleObject : public PhysicsObject {
private:
  float radius = 0.0;

public:
  CircleObject(float mass, float x, float y, float radius,
               ImU32 color = ObjectColors::WHITE) {
    this->mass = mass;
    this->radius = radius;

    this->position = ImVec2(x, y);
    this->color = color;
  }

  ImVec2 getDistanceFromCenter() const;
  void draw();

  void update(const double &dt);
  void constraint(const ImVec2 &position);
};
