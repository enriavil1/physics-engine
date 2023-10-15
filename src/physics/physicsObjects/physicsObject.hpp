#pragma once

#include <vector>

#include "../../../imgui/imgui.h"

#include "../../forces/gravity.hpp"

namespace ObjectColors {
const ImU32 WHITE = IM_COL32(255, 255, 255, 255);
}

class PhysicsObject {
protected:
  float mass;

  std::vector<Force *> forces = std::vector<Force *>{};

  ImVec2 position;
  ImVec2 velocity = ImVec2(0.0f, 0.0f);
  ImVec2 acceleration = ImVec2(0.0f, 0.0f);

  ImU32 color = ObjectColors::WHITE;

public:
  void setPosition(ImVec2 new_pos) { this->position = new_pos; }
  ImVec2 getPosition() const { return position; }
  virtual ImVec2 getDistanceFromCenter() const = 0;

  void setVelocity(ImVec2 new_vel) { this->velocity = new_vel; }
  ImVec2 getVelocity() const { return this->velocity; }

  float getMass() const { return this->mass; }

  void applyForce(Force *force) { this->forces.push_back(force); }

  virtual void draw() = 0;
  virtual void update(const double &dt /* change in time*/) = 0;
  virtual void constraint(const ImVec2 &position) = 0;
};
