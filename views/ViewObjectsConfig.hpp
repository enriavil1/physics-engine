#pragma once

#include "./ViewPort.hpp"

class ViewObjectsConfig : ViewPort {
private:
  static float radius;

  static float height;
  static float width;

  static ImGuiWindowFlags window_flags;

  void loadCircleConfigs();
  void loadSquareConfigs();

  void addDisclaimer();
  void HelpMarker(const char *desc);

public:
  void render();
};
