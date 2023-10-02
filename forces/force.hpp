#pragma once

#include "../imgui/imgui.h"

namespace ForceConstants {
const unsigned int PIXELS_PER_METER = 50;
} // namespace ForceConstants

class Force {
public:
  static ImVec2 apply(float mass, float current_velocity);
};
