#include <chrono>
#include <iostream>

#include "imgui/imgui.h"
#include "physics/physicsObjects/circleObject.hpp"
#include "physics/systemState.hpp"
#include "views/ViewObjectsConfig.hpp"
#include "views/ViewStats.hpp"
#include "views/main/MainView.hpp"

int main() {
  // this is mac versioning add other versions
  const char *glsl_version = "#version 150";
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  uint width = 1280;
  uint height = 720;

  auto main_view = MainView(glsl_version, clear_color, width, height);
  main_view.createWindow();

  // auto start = std::chrono::system_clock::now();

  auto statsModal = ViewStats();
  auto objectConfigModal = ViewObjectsConfig();

  while (main_view.getIsRunning()) {
    main_view.processEvent();
    // Start the Dear ImGui frame
    main_view.newFrame();

    ImGui::NewFrame();

    statsModal.render();
    objectConfigModal.render();

    SystemState::ResolveCollisions();
    // dont know how to make it fall faster
    SystemState::Update();

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
