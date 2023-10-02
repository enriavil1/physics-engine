#pragma once

#include "./force.hpp"

class Gravity : Force {
public:
  static constexpr ImVec2 gravity_acc = ImVec2(0, 9.81f);
  static constexpr float drag_coeffiecient = 0.5f;

  static ImVec2 apply(float mass, float current_velocity);
};
