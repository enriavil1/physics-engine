#include "./circleObject.hpp"
#include <iostream>

void CircleObject::draw() {
  ImGui::GetBackgroundDrawList()->AddCircle(this->position, this->radius * .05,
                                            ObjectColors::WHITE, 0, 1.5);
}

void CircleObject::update(double dt) {
  auto new_acc = Gravity::apply(this->mass, this->velocity.y);

  float new_position_x = this->position.x + this->velocity.x * dt +
                         (this->acceleration.x * dt) * 0.5f;
  float new_position_y = this->position.y + this->velocity.y * dt +
                         (this->acceleration.y * dt) * 0.5f;

  ImVec2 window_size = ImGui::GetWindowContentRegionMax();
  if (new_position_x > window_size.x) {
    new_position_x = window_size.x;
  } else if (new_position_x < 0) {
    new_position_x = 0;
  }

  if (new_position_y > window_size.y) {
    new_position_y = window_size.y;
  } else if (new_position_y < 0) {
    new_position_y = 0;
  }

  this->position = ImVec2(new_position_x, new_position_y);

  float new_velocity_x =
      this->velocity.x + (this->acceleration.x + new_acc.x * dt) / 2;
  float new_velocity_y =
      this->velocity.y + (this->acceleration.y + new_acc.y * dt) / 2;
  this->velocity = ImVec2(new_velocity_x, new_velocity_y);

  this->acceleration = ImVec2(this->acceleration.x + new_acc.x,
                              this->acceleration.y + this->acceleration.y);
}
