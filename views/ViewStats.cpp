#include "./ViewPort.hpp"

namespace ViewPort {
class ViewStats : ViewPort {
private:
  const ImGuiIO &io;

public:
  ViewStats(const ImGuiIO &io) : io(io) {}

  void render() {
    ImGui::NewFrame();

    ImGui::Begin("Stats", __null, ImGuiWindowFlags_NoTitleBar);
    ImGui::Text("Application average: %.1f FPS", io.Framerate);
    ImGui::End();
  }
};
} // namespace ViewPort
