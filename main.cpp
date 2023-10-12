#include <algorithm>
#include <chrono>
#include <system_error>

#include "imgui/imgui.h"
#include "physics/physicsObjects/circleObject.hpp"
#include "physics/systemState.hpp"
#include "views/ViewObjectsConfig.hpp"
#include "views/ViewStats.hpp"
#include "views/main/MainView.hpp"

int main() {
  // this is mac versioning add other versions
  const char *glsl_version = "#version 150";
  ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
  uint width = 1280;
  uint height = 720;

  auto main_view = MainView(glsl_version, clear_color, width, height);
  main_view.createWindow();

  // auto start = std::chrono::system_clock::now();

  auto statsModal = ViewStats();
  auto objectConfigModal = ViewObjectsConfig();

  auto &io = ImGui::GetIO();

  const float dt = 1.0f / io.Framerate;

  auto start = std::chrono::system_clock::now();

  while (main_view.getIsRunning()) {
    main_view.processEvent();
    // Start the Dear ImGui frame
    main_view.newFrame();

    ImGui::NewFrame();

    statsModal.render();
    objectConfigModal.render();

    const auto current = std::chrono::system_clock::now();
    const std::chrono::duration<double> duration = current - start;

    start = current;
    float frame_time = duration.count();

    while (frame_time > 0.0f) {
      const float time_step = std::min(frame_time, dt);
      SystemState::ResolveCollisions();
      SystemState::Update(time_step);

      frame_time -= dt;
    }

    // auto current = std::chrono::system_clock::now();
    // std::chrono::duration<double> duration = current - start;
    // if (duration.count() > 0.5) {
    //   SystemState::AddObject(new CircleObject(1.0f, 0, 0, 5.0f));
    //   start = current;
    // }

    SystemState::Draw();
    main_view.render();
  }

  main_view.quitView();
}
