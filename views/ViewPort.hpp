#pragma once

#include "../imgui/imgui.h"

class ViewPort {
public:
  virtual void render() = 0;
};
