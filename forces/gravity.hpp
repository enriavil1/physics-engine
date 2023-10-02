#pragma once

#include "../imgui/imgui.h"

namespace ForceConstants {
const unsigned int PIXELS_PER_METER = 50;
} // namespace ForceConstants

class Gravity {
public:
  static constexpr ImVec2 gravity_acc = ImVec2(0, 9.81f);
  static constexpr float drag_coeffiecient = 0.5f;

  static ImVec2 apply(float mass, float current_velocity);
};
