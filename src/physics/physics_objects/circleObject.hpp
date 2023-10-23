#pragma once

#include <cmath>

#include "./physicsObject.hpp"

#include "../../forces/gravity.hpp"

class CircleObject : public PhysicsObject {
private:
  float radius = 0.0f;

public:
  CircleObject(float mass, float x, float y, float radius,
               ImU32 color = IM_COL32_WHITE) {
    this->p_mass = mass;
    this->radius = radius;
    this->p_area = pow(radius, 2) * 3.14;

    this->p_position = ImVec2(x, y);
    this->p_color = color;
  }

  ImVec2 getDistanceFromCenter() const;
  void draw();

  void update(const double& dt);
  void constraint(const ImVec2& position);
};
