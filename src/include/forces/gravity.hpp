#pragma once

#include "./force.hpp"

class Gravity : public Force {
private:
  static constexpr ImVec2 gravity_acc = ImVec2(0, 9.81f);
  static constexpr float drag_coefficient = 0.005f;

public:
  ImVec2 apply(const float& mass, const float& current_velocity,
               const float& area) {

    ImVec2 drag_acc;
    if (current_velocity > 0) {
      float drag_force =
          0.5f * Gravity::drag_coefficient * area *
          (current_velocity *
           current_velocity); // D = 0.5 * (rho * C * Area * vel^2)
      drag_acc = ImVec2(0, drag_force / mass);
    }

    return ImVec2(0, (Gravity::gravity_acc.y - drag_acc.y));
  };
};
