#include <chrono>

#include "imgui/imgui.h"

#include "src/physics/physicsObjects/circleObject.hpp"
#include "src/physics/systemState.hpp"

#include "src/views/ViewObjectsConfig.hpp"
#include "src/views/ViewStats.hpp"
#include "src/views/drawView/drawView.hpp"
#include "src/views/main/MainView.hpp"

int main() {
  // this is mac versioning add other versions
  const char *glsl_version = "#version 150";
  ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
  uint32_t width = 1280;
  uint32_t height = 720;

  auto main_view = MainView(glsl_version, clear_color, width, height);
  main_view.createWindow();

  auto stats_modal = ViewStats();
  auto object_config_modal = ViewObjectsConfig();
  auto draw_view = DrawView();

  auto &io = ImGui::GetIO();

  const float dt = 1.0f / io.Framerate;

  auto start = std::chrono::system_clock::now();

  while (main_view.getIsRunning()) {
    main_view.processEvent();
    // Start the Dear ImGui frame
    main_view.newFrame();

    stats_modal.render();
    object_config_modal.render();

    const auto current = std::chrono::system_clock::now();
    const std::chrono::duration<double> duration = current - start;

    start = current;
    float frame_time = duration.count();

    while (frame_time > 0.0f) {
      const float time_step = std::min(frame_time, dt);
      draw_view.render(time_step);
      frame_time -= dt;
    }

    main_view.render();
  }

  main_view.quitView();
}
