#include "./ViewPort.hpp"

namespace ViewPort {
class ViewStats : ViewPort {
private:
public:
  ViewStats(){};

  void render() {
    auto &io = ImGui::GetIO();
    (void)io;

    ImGui::NewFrame();
    ImGui::Begin("Stats", __null, ImGuiWindowFlags_NoTitleBar);
    ImGui::Text("Application average: %.1f FPS", io.Framerate);
    ImGui::End();
  }
};
} // namespace ViewPort
