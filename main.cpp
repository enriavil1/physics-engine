#include <chrono>
#include <iostream>

#include "./physics/physicsObjects/circleObject.hpp"
#include "./physics/systemState.hpp"
#include "./views/ViewStats.cpp"
#include "./views/main/MainView.hpp"

int main() {
  // this is mac versioning add other versions
  const char *glsl_version = "#version 150";
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  uint width = 1280;
  uint height = 720;

  auto main_view = MainView(glsl_version, clear_color, width, height);
  main_view.createWindow();

  int count = 0;

  auto last = std::chrono::system_clock::now();

  while (main_view.getIsRunning()) {
    main_view.processEvent();
    // Start the Dear ImGui frame
    main_view.newFrame();
    auto statsModal = ViewStats();
    statsModal.render();

    std::chrono::duration<double> elapsed_seconds =
        std::chrono::system_clock::now() - last;
    if (elapsed_seconds.count() > 1.5) {
      SystemState::AddObject(new CircleObject(1.0, 0, 0, 10));
      last = std::chrono::system_clock::now();
    }

    SystemState::ResolveCollisions();
    SystemState::Draw();
    SystemState::Update();

    main_view.render();
  }

  main_view.quitView();
}
