#include "./ViewObjectsConfig.hpp"
#include "../physics/systemState.hpp"
#include <iostream>

constexpr float radius_min = 2.0f;
constexpr float radius_max = 100.0f;

constexpr float width_min = 5.0f;
constexpr float width_max = 150.0f;

const float height_min = 5.0f;
const float height_max = 150.0f;

float ViewObjectsConfig::radius = radius_min;
float ViewObjectsConfig::height = height_min;
float ViewObjectsConfig::width = width_min;

ImGuiWindowFlags ViewObjectsConfig::window_flags = 0;

void ViewObjectsConfig::HelpMarker(const char *desc) {
  ImGui::TextDisabled("(?)");
  if (ImGui::BeginItemTooltip()) {
    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
    ImGui::TextUnformatted(desc);
    ImGui::PopTextWrapPos();
    ImGui::EndTooltip();
  }
}

void ViewObjectsConfig::addDisclaimer() {
  ImGui::SameLine();
  HelpMarker("Click and drag to edit value.\n"
             "Hold SHIFT/ALT for faster/slower edit.\n"
             "Double-click or CTRL+click to input value.");
}

void ViewObjectsConfig::loadCircleConfigs() {

  ImGui::DragFloat("Radius", &ViewObjectsConfig::radius, 0.005f, radius_min,
                   radius_max);
  addDisclaimer();
}

void ViewObjectsConfig::loadSquareConfigs() {
  ImGui::DragFloat("Width", &ViewObjectsConfig::width, 0.005f, width_min,
                   width_max);
  addDisclaimer();
  ImGui::DragFloat("Height", &ViewObjectsConfig::height, 0.005, height_min,
                   height_max);
  addDisclaimer();
}

void ViewObjectsConfig::render() {
  static int e = 0;
  ImGui::Begin("Object Config", __null, ViewObjectsConfig::window_flags);
  ImGui::RadioButton("square", &e, 0);
  ImGui::SameLine();
  ImGui::RadioButton("circle", &e, 1);

  if (e) {
    ViewObjectsConfig::loadCircleConfigs();
  } else {
    ViewObjectsConfig::loadSquareConfigs();
  }

  ImGui::End();
}
