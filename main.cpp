#include <chrono>
#include <iostream>

#include "physics/physicsObjects/circleObject.hpp"
#include "physics/systemState.hpp"
#include "views/ViewStats.cpp"
#include "views/main/MainView.hpp"

int main() {
  // this is mac versioning add other versions
  const char *glsl_version = "#version 150";
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  uint width = 1280;
  uint height = 720;

  auto main_view = MainView(glsl_version, clear_color, width, height);
  main_view.createWindow();

  auto start = std::chrono::system_clock::now();

  auto &io = ImGui::GetIO();

  while (main_view.getIsRunning()) {
    main_view.processEvent();
    // Start the Dear ImGui frame
    main_view.newFrame();
    auto statsModal = ViewStats();
    statsModal.render();

    const auto fps_cap = io.Framerate > 60 ? 60.0f : io.Framerate;

    SystemState::ResolveCollisions();
    // dont know how to make it fall faster
    SystemState::Update((1.0f / fps_cap) * 6);
    SystemState::Draw();

    auto current = std::chrono::system_clock::now();
    std::chrono::duration<double> dt = current - start;
    if (dt.count() > 2) {
      SystemState::AddObject(new CircleObject(1.0, 0, 0, 10));
      start = current;
    }

    main_view.render();
  }

  main_view.quitView();
}
