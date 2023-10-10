#include "./circleObject.hpp"

#include <iostream>

ImVec2 CircleObject::getDistanceFromCenter() const {
  // radius + thickness of line
  return ImVec2(this->radius, this->radius);
}

void CircleObject::draw() {
  ImGui::GetBackgroundDrawList()->AddCircle(this->position, this->radius,
                                            ObjectColors::WHITE, 0, 1.0f);
}

void CircleObject::update(const double &dt) {
  // using pixels per meter since position is in pixels
  // we only need to multiple by pixels/meter to convert velocity
  while (!this->forces.empty()) {
    Force *force = this->forces[0];
    auto new_acc = force->apply(this->mass, this->velocity.y);
    this->acceleration = ImVec2(this->acceleration.x + new_acc.x,
                                this->acceleration.y + new_acc.y);

    this->forces.pop_back();
  }

  float new_position_x =
      this->position.x +
      ((this->velocity.x * dt) + (this->acceleration.x * (dt * dt))) *
          ForceConstants::PIXELS_PER_METER;
  float new_position_y =
      this->position.y +
      ((this->velocity.y * dt) + (this->acceleration.y * (dt * dt))) *
          ForceConstants::PIXELS_PER_METER;

  this->position = ImVec2(new_position_x, new_position_y);

  float new_velocity_x = this->acceleration.x * dt;
  float new_velocity_y = this->acceleration.y * dt;

  this->velocity = ImVec2(new_velocity_x, new_velocity_y);
  this->acceleration = ImVec2(0, 0);
}

void CircleObject::constraint(const ImVec2 &position) {
  ImVec2 window_size = ImGui::GetMainViewport()->Size;

  float new_position_x = position.x;
  float new_position_y = position.y;

  // the radius is the distance from the center of the circle to the edge
  // new_position is only considering the center of the circle
  if (new_position_x + this->radius >= window_size.x) {
    new_position_x = window_size.x - this->radius;
  } else if (new_position_x - this->radius <= 0) {
    new_position_x = this->radius;
  }

  if (new_position_y + this->radius >= window_size.y) {
    new_position_y = window_size.y - this->radius;
  } else if (new_position_y - this->radius <= 0) {
    new_position_y = this->radius;
  }

  this->setPosition(ImVec2(new_position_x, new_position_y));
}
