#include "imgui/imgui.h"

#include "src/physics/systemState.hpp"

#include "src/views/ViewObjectsConfig.hpp"
#include "src/views/ViewStats.hpp"
#include "src/views/drawView/drawView.hpp"
#include "src/views/main/MainView.hpp"

#define SUB_STEP 8.0f

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

  auto& io = ImGui::GetIO();

  const float fps_cap = 60.0f;

  while (main_view.getIsRunning()) {
    main_view.processEvent();
    // Start the Dear ImGui frame
    main_view.newFrame();

    stats_modal.render();
    object_config_modal.render();

    const double frame_rate = io.Framerate >= fps_cap ? fps_cap : io.Framerate;
    float dt = 1 / (frame_rate * SUB_STEP);

    draw_view.render(dt);

    main_view.render();
  }

  main_view.quitView();
}
