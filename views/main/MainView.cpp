#include "./MainView.hpp"
#include "../ViewStats.cpp"

#include <iostream>

void MainView::createWindow() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("Error: SDL_Init(): %s\n", SDL_GetError());
    return;
  }

  // This is a Mac Setup
  // TODO: Add other setups
  SDL_GL_SetAttribute(
      SDL_GL_CONTEXT_FLAGS,
      SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

  // Setup window
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_Window *window = SDL_CreateWindow("Physics Engine", this->width,
                                        this->height, this->window_flags);
  if (window == nullptr) {
    printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
    return;
  }
  SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

  SDL_GLContext gl_context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, gl_context);
  // Enable vsync
  SDL_GL_SetSwapInterval(1);
  SDL_ShowWindow(window);

  this->window = window;
  this->gl_context = gl_context;

  this->setUpImGui();
  this->is_running = true;
}

void MainView::setUpImGui() {
  if (this->glsl_version == nullptr) {
    std::cerr << "[" << this->view_name
              << "] failed setUpImGui(): missing glsl_version\n";
    return;
  }

  if (this->window == nullptr) {
    std::cerr << "[" << this->view_name
              << "] failed setUpImGui(): missing window\n";
    return;
  }

  // Setting up ImGui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;

#if defined(LIGHT_MODE)
  ImGui::StyleColorsLight();
#else
  ImGui::StyleColorsDark();
#endif

  this->io = io;

  // Setup Platform/Renderer backends
  ImGui_ImplSDL3_InitForOpenGL(this->window, this->gl_context);
  ImGui_ImplOpenGL3_Init(this->glsl_version);
}

void MainView::processEvent() {
  while (SDL_PollEvent(&this->event)) {
    ImGui_ImplSDL3_ProcessEvent(&this->event);
    if (event.type == SDL_EVENT_QUIT)
      this->is_running = false;
    if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
        event.window.windowID == SDL_GetWindowID(this->window))
      this->is_running = false;
  }
}

void MainView::newFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
}

void MainView::render() {
  // Rendering
  ImGui::Render();
  glViewport(0, 0, static_cast<int>(this->io.DisplaySize.x),
             static_cast<int>(this->io.DisplaySize.y));
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  SDL_GL_SwapWindow(this->window);
}

void MainView::quitView() {
  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();

  SDL_GL_DeleteContext(this->gl_context);
  SDL_DestroyWindow(this->window);
  SDL_Quit();
}
