#include "../../include/views/ViewStats.hpp"
#include "../../include/physics/systemState.hpp"

ImGuiWindowFlags ViewStats::window_flags = ImGuiWindowFlags_AlwaysAutoResize;

void ViewStats::render() {
  auto& io = ImGui::GetIO();
  (void)io;

  ImGui::Begin("Stats", NULL, ViewStats::window_flags);
  ImGui::Text("%d Objects", SystemState::GetObjectAmount());
  ImGui::Text("%.1f FPS", io.Framerate);
  ImGui::End();
}
