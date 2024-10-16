#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>

#include "../ViewPort.hpp"

class MainView : ViewPort {
private:
  const char *view_name = "MainView";
  const char *glsl_version;

  bool is_running = false;

  uint32_t width;
  uint32_t height;

  SDL_Event event;

  SDL_WindowFlags window_flags = static_cast<SDL_WindowFlags>(
      SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN);

  SDL_Window *window;
  SDL_GLContext gl_context;

  ImGuiIO io;
  ImVec4 clear_color;

public:
  MainView(const char *glsl_version, ImVec4 clear_color, unsigned int width,
           unsigned int height) {
    this->width = width;
    this->height = height;

    this->glsl_version = glsl_version;
    this->clear_color = clear_color;
  };

  bool getIsRunning() { return this->is_running; }
  ImGuiIO& getImGuiIO() { return this->io; }

  void createWindow();
  void setUpImGui();
  void processEvent();
  void newFrame();
  void drawCircle();
  void render();
  void quitView();
};
