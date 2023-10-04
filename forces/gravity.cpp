#include "./gravity.hpp"
#include <iostream>

ImVec2 Gravity::apply(float mass, float current_velocity) {
  float drag_force = 0.5 * Gravity::drag_coeffiecient *
                     (current_velocity *
                      current_velocity); // D = 0.5 * (rho * C * Area * vel^2)
  ImVec2 drag_acc = ImVec2(0, drag_force / mass);

  return ImVec2(0, Gravity::gravity_acc.y - drag_acc.y);
}
