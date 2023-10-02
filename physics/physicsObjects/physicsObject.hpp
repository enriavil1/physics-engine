#pragma once

#include <vector>

#include "../../forces/Force.hpp"
#include "../../imgui/imgui.h"

namespace ObjectColors {
const ImU32 WHITE = IM_COL32(255, 255, 255, 255);
}

class PhysicsObject {
protected:
  float mass;

  std::vector<Force> forces;

  ImVec2 position;
  ImVec2 velocity = ImVec2(0.0, 0.0);
  ImVec2 acceleration = ImVec2(0.0, 0.0);

public:
  void draw();
  void update(double dt /* change in time*/);
  void applyForces();
};
