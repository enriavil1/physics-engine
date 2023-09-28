#include <iostream>

#include "./views/main/MainView.hpp"

int main() {
  // this is mac versioning add other versions
  const char *glsl_version = "#version 150";

  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  auto main_view = MainView(glsl_version, clear_color);
  main_view.runMainLoop();
}
