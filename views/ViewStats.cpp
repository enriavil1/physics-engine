#include "./ViewStats.hpp"
#include "../physics/systemState.hpp"

ImGuiWindowFlags ViewStats::window_flags =
    ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
    ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize |
    ImGuiWindowFlags_NoMove;

void ViewStats::render() {
  auto &io = ImGui::GetIO();
  (void)io;

  auto view_port_width = ImGui::GetMainViewport()->Size.x;

  ImGui::NewFrame();
  ImGui::SetNextWindowPos(ImVec2(view_port_width - 100, 10));
  ImGui::SetNextWindowSize(ImVec2(100, 100));

  ImGui::Begin("Stats", __null, ViewStats::window_flags);
  ImGui::Text("%d Objects", SystemState::GetObjectAmount());
  ImGui::Text("%.1f FPS", io.Framerate);
  ImGui::End();
}
