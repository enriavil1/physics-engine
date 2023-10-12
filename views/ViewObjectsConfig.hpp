#pragma once

#include "./ViewPort.hpp"

#include <cstdlib>
#include <ctime>
#include <random>

class ViewObjectsConfig : ViewPort {
private:
  static float s_radius;

  static float s_height;
  static float s_width;

  static float s_mass;
  static ImU32 s_color;

  static ImGuiWindowFlags window_flags;

  static std::mt19937_64 s_rng;

  void loadCircleConfigs();
  void loadSquareConfigs();

  void addDisclaimer();
  void HelpMarker(const char *desc);

  void ColorPicker3U32(const char *label, ImU32 *color,
                       ImGuiColorEditFlags flags = 0);

public:
  static float GetRadius() { return ViewObjectsConfig::s_radius; }
  static ImVec2 GetWidthAndHeight() {
    return ImVec2(ViewObjectsConfig::s_width, ViewObjectsConfig::s_height);
  }

  static float GetMass() { return ViewObjectsConfig::s_mass; }
  static ImU32 GetColor() { return ViewObjectsConfig::s_color; }

  static void InitializeSeed() { ViewObjectsConfig::s_rng.seed(time(0)); }

  void render();
};
