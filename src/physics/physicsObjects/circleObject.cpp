#define BOUNCE_PERCENTAGE -0.80f
#define MIN_BOUNCE 0.1f

#include "./circleObject.hpp"

ImVec2 CircleObject::getDistanceFromCenter() const {
  // radius + thickness of line
  return ImVec2(this->radius, this->radius);
}

void CircleObject::draw() {
  auto draw_list = ImGui::GetWindowDrawList();
  draw_list->AddCircleFilled(this->p_position, this->radius, this->p_color, 0);
}

void CircleObject::update(const double& dt) {
  // using pixels per meter since position is in pixels
  // we only need to multiple by pixels/meter to convert velocity
  while (!this->p_forces.empty()) {
    Force *force = this->p_forces[0];
    auto new_acc = force->apply(this->p_mass, this->p_vel.y, this->p_area);
    this->p_acc = ImVec2(this->p_acc.x + new_acc.x, this->p_acc.y + new_acc.y);

    this->p_forces.pop_back();
  }

  float new_position_x = this->p_position.x +
                         ((this->p_vel.x * dt) + (this->p_acc.x * (dt * dt))) *
                             ForceConstants::PIXELS_PER_METER;
  float new_position_y = this->p_position.y +
                         ((this->p_vel.y * dt) + (this->p_acc.y * (dt * dt))) *
                             ForceConstants::PIXELS_PER_METER;

  this->p_position = ImVec2(new_position_x, new_position_y);

  float new_velocity_x = this->p_vel.x + this->p_acc.x * dt;
  float new_velocity_y = this->p_vel.y + this->p_acc.y * dt;

  this->p_vel = ImVec2(new_velocity_x, new_velocity_y);
  this->p_acc = ImVec2(0, 0);
}

void CircleObject::constraint(const ImVec2& position) {
  ImVec2 window_max = ImGui::GetWindowContentRegionMax();
  ImVec2 window_min = ImGui::GetWindowContentRegionMin();

  ImVec2 window_pos = ImGui::GetWindowPos();

  const float max_x = window_max.x + window_pos.x;
  const float min_x = window_min.x + window_pos.x;

  const float max_y = window_max.y + window_pos.y;
  const float min_y = window_min.y + window_pos.y;

  float new_position_x = position.x;
  float new_position_y = position.y;

  // the radius is the distance from the center of the circle to the edge
  // new_position is only considering the center of the circle
  if (new_position_x + this->radius >= max_x ||
      new_position_x - this->radius <= min_x) {

    if (new_position_x + this->radius >= max_x) {
      new_position_x = max_x - this->radius;
    }

    if (new_position_x - this->radius <= min_x) {
      new_position_x = this->radius + min_x;
    }

    auto new_x_vel = fabs(this->p_vel.x * BOUNCE_PERCENTAGE) > MIN_BOUNCE
                         ? this->p_vel.x * BOUNCE_PERCENTAGE
                         : 0.0f;
    this->p_vel = ImVec2(new_x_vel, this->p_vel.y);
  }

  if (new_position_y + this->radius >= max_y ||
      new_position_y - this->radius <= min_y) {

    if (new_position_y + this->radius >= max_y) {
      new_position_y = max_y - this->radius;

      // we add the normal (inverse of gravity)
      Gravity g;
      auto acc = g.apply(this->p_mass, this->p_vel.y, this->p_area);
      this->p_acc = ImVec2(this->p_acc.x, acc.y * -1);
    }

    if (new_position_y - this->radius <= min_y) {
      new_position_y = this->radius + min_y;
    }

    auto new_y_vel = fabs(this->p_vel.y * BOUNCE_PERCENTAGE) > MIN_BOUNCE
                         ? this->p_vel.y * BOUNCE_PERCENTAGE
                         : 0.0f;
    this->p_vel = ImVec2(this->p_vel.x, new_y_vel);
  }

  this->setPosition(ImVec2(new_position_x, new_position_y));
}
