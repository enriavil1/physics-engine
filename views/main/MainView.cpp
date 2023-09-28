#include "./MainView.hpp"
#include "../ViewStats.cpp"

#include <iostream>

MainView::MainView(const char *glsl_version, ImVec4 clear_color) {
  this->glsl_version = glsl_version;
  this->clear_color = clear_color;
}

std::pair<SDL_Window *, SDL_GLContext *> MainView::sdl_init() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("Error: SDL_Init(): %s\n", SDL_GetError());
    return {nullptr, nullptr};
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
  SDL_WindowFlags window_flags = static_cast<SDL_WindowFlags>(
      SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN);
  SDL_Window *window =
      SDL_CreateWindow("Physics Engine", 1280, 720, window_flags);
  if (window == nullptr) {
    printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
    return {nullptr, nullptr};
  }
  SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

  SDL_GLContext gl_context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, gl_context);
  // Enable vsync
  SDL_GL_SetSwapInterval(1);
  SDL_ShowWindow(window);

  return {window, &gl_context};
}

ImGuiIO *MainView::setUpImGui() {
  if (this->glsl_version == nullptr) {
    std::cout << "[" << this->view_name
              << "] failed setUpImGui(): missing glsl_version\n";
    return nullptr;
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

  return &io;
}

void MainView::render(const ImGuiIO &io, SDL_Window *window) {
  // Rendering
  ImGui::Render();
  glViewport(0, 0, static_cast<int>(io.DisplaySize.x),
             static_cast<int>(io.DisplaySize.y));
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  SDL_GL_SwapWindow(window);
}

void MainView::stopMainLoop(SDL_Window *window, SDL_GLContext gl_context) {
  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();

  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

// TODO enriavil1: Fix runMainLoop so whenever the loop is ended we dont get a
// segmentation fault
void MainView::runMainLoop() {
  std::pair<SDL_Window *, SDL_GLContext *> pair = this->sdl_init();
  SDL_Window *window = pair.first;
  SDL_GLContext &gl_context = *pair.second;

  ImGuiIO *io = this->setUpImGui();
  auto &io_ref = *io;
  if (io == nullptr) {
    std::cout << "[" << this->view_name << "]"
              << "failed RunMainLoop: missing io\n";
    return;
  }

  // Setup Platform/Renderer backends
  ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
  ImGui_ImplOpenGL3_Init(this->glsl_version);

  bool quit = false;

  while (!quit) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL3_ProcessEvent(&event);
      if (event.type == SDL_EVENT_QUIT)
        quit = true;
      if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
          event.window.windowID == SDL_GetWindowID(window))
        quit = true;
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    auto statsModal = ViewPort::ViewStats(io_ref);
    statsModal.render();

    this->render(io_ref, window);
  }

  this->stopMainLoop(window, gl_context);
}
