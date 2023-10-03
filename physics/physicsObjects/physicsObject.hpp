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

  std::vector<Force> forces = std::vector<Force>{};

  ImVec2 position;
  ImVec2 velocity = ImVec2(0.0, 0.0);
  ImVec2 acceleration = ImVec2(0.0, 0.0);

public:
  void setPosition(ImVec2 new_pos) { this->position = new_pos; }
  ImVec2 getPosition() { return position; }
  virtual ImVec2 getDistanceFromCenter() = 0;

  void applyForce(Force force) { this->forces.push_back(force); }

  virtual void draw() = 0;
  virtual void update(double dt /* change in time*/) = 0;
};
