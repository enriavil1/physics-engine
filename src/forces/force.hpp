#pragma once

#include <cstdint>

#include "../../imgui/imgui.h"

namespace ForceConstants {
const uint32_t PIXELS_PER_METER = 20;
const float EPSILON = 0.000001f;
} // namespace ForceConstants

class Force {
public:
  virtual ImVec2 apply(const float& mass, const float& current_velocity,
                       const float& area) = 0;
};
