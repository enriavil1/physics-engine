#pragma once

#include <cmath>

#include "./physicsObject.hpp"

#include "../../forces/gravity.hpp"

class CircleObject : public PhysicsObject {
private:
  float radius = 0.0f;
  float m_area = 0.0f;

public:
  CircleObject(float mass, float x, float y, float radius,
               ImU32 color = ObjectColors::WHITE) {
    this->mass = mass;
    this->radius = radius;
    this->m_area = pow(radius, 2) * 3.14;

    this->position = ImVec2(x, y);
    this->color = color;
  }

  ImVec2 getDistanceFromCenter() const;
  void draw();

  void update(const double &dt);
  void constraint(const ImVec2 &position);
};
