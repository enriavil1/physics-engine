#include "./ViewPort.hpp"

class ViewStats : ViewPort {
private:
  static ImGuiWindowFlags window_flags;

public:
  void render();
};
