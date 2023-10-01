#include <iostream>

#include "./views/ViewStats.cpp"
#include "./views/main/MainView.hpp"
#include "imgui/imgui.h"

int main() {
  // this is mac versioning add other versions
  const char *glsl_version = "#version 150";
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  unsigned int width = 1280;
  unsigned int height = 720;

  auto main_view = MainView(glsl_version, clear_color, width, height);

  main_view.createWindow();
  main_view.setUpImGui();

  while (main_view.getIsRunning()) {
    main_view.processEvent();
    // Start the Dear ImGui frame
    main_view.newFrame();
    auto statsModal = ViewPort::ViewStats();
    statsModal.render();

    main_view.render();
  }

  main_view.quitView();
}
