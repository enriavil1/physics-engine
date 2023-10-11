#pragma once

#include "../imgui/imgui.h"

namespace ForceConstants {
const uint32_t PIXELS_PER_METER = 40;
const float EPSILON = 0.0001f;
} // namespace ForceConstants

class Force {
public:
  virtual ImVec2 apply(float mass, float current_velocity) = 0;
};
