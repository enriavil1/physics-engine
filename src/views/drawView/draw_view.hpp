#pragma once

#include "../ViewPort.hpp"

class DrawView : ViewPort {
private:
  static ImGuiWindowFlags s_window_flags;

  bool can_create = false;

  void handleLeftClick();
  void handleRightClick();

public:
  void render() {}
  void render(const float &dt);
};
