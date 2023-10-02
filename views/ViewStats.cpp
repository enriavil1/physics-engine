#include "./ViewPort.hpp"
#include <iostream>

namespace ViewPort {
class ViewStats : ViewPort {
private:
public:
  ViewStats(){};

  void render() {
    auto &io = ImGui::GetIO();
    (void)io;

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoBackground;
    window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
    window_flags |= ImGuiWindowFlags_NoMove;

    auto view_port_width = ImGui::GetMainViewport()->Size.x;

    ImGui::NewFrame();
    ImGui::SetNextWindowPos(ImVec2(view_port_width - 100, 10));
    ImGui::SetNextWindowSize(ImVec2(100, 100));

    ImGui::Begin("Stats", __null, window_flags);
    ImGui::Text("%.1f FPS", io.Framerate);
    ImGui::End();
  }
};
} // namespace ViewPort
