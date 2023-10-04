#include "./circleObject.hpp"

#include <iostream>

ImVec2 CircleObject::getDistanceFromCenter() {
  return ImVec2(this->radius, this->radius);
}

void CircleObject::draw() {
  ImGui::GetBackgroundDrawList()->AddCircle(this->position, this->radius,
                                            ObjectColors::WHITE, 0, 1.0);
}

void CircleObject::update(const double &dt) {
  // using pixels per meter since position is in pixels
  // // we only need to multiple by pixels/meter to convert velocity
  auto new_acc = Gravity::apply(this->mass, this->velocity.y);

  float new_position_x =
      ((2.0f * this->position.x) - this->last_position.x +
       (new_acc.x * (dt * dt)) * ForceConstants::PIXELS_PER_METER);
  float new_position_y =
      ((2.0f * this->position.y) - this->last_position.y +
       (new_acc.y * (dt * dt)) * ForceConstants::PIXELS_PER_METER);

  this->last_position = this->position;
  this->position = ImVec2(new_position_x, new_position_y);

  float new_velocity_x = ((this->position.x - this->last_position.x) /
                          ForceConstants::PIXELS_PER_METER) /
                         dt;
  float new_velocity_y = ((this->position.y - this->last_position.y) /
                          ForceConstants::PIXELS_PER_METER) /
                         dt;
  this->velocity = ImVec2(new_velocity_x, new_velocity_y);
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
