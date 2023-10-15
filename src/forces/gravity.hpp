#pragma once

#include "./force.hpp"

class Gravity : public Force {
private:
  static constexpr ImVec2 gravity_acc = ImVec2(0, 9.81f);
  static constexpr float drag_coeffiecient = 0.5f;

public:
  ImVec2 apply(float mass, float current_velocity, float area) {
    float drag_force = 0.5f * Gravity::drag_coeffiecient *
                       (current_velocity *
                        current_velocity); // D = 0.5 * (rho * C * Area * vel^2)
    ImVec2 drag_acc = ImVec2(0, drag_force / mass);

    return ImVec2(0, (Gravity::gravity_acc.y - drag_acc.y));
  };
};
