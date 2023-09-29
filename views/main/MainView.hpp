#pragma once

#include <string>
#include <utility>

#include "../../imgui/backends/imgui_impl_opengl3.h"
#include "../../imgui/backends/imgui_impl_sdl3.h"
#include "../../imgui/imgui.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

class MainView {
private:
  const char *view_name = "MainView";
  const char *glsl_version;

  ImVec4 clear_color;

  SDL_Window *sdl_init();
  ImGuiIO *setUpImGui();
  void render(const ImGuiIO &io, SDL_Window *window);
  void stopMainLoop(SDL_Window *window, SDL_GLContext gl_context);

public:
  MainView(const char *glsl_version, ImVec4 clear_color);
  void runMainLoop();
};
