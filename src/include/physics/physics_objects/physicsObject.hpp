#pragma once

#include <imgui.h>
#include <vector>

#include "../../forces/force.hpp"

class PhysicsObject {
protected:
  float p_mass;
  float p_area = 0.0f;

  std::vector<Force *> p_forces = std::vector<Force *>{};

  ImVec2 p_position;
  ImVec2 p_vel;
  ImVec2 p_acc;

  ImU32 p_color = IM_COL32_WHITE;

public:
  void setPosition(ImVec2 new_pos) { this->p_position = new_pos; }
  ImVec2 getPosition() const { return p_position; }
  virtual ImVec2 getDistanceFromCenter() const = 0;

  void setVelocity(ImVec2 new_vel) { this->p_vel = new_vel; }
  ImVec2 getVelocity() const { return this->p_vel; }

  float getMass() const { return this->p_mass; }

  void applyForce(Force *force) { this->p_forces.push_back(force); }

  virtual void draw() = 0;
  virtual void update(const double& dt /* change in time*/) = 0;
  virtual void constraint(const ImVec2& position) = 0;
};
