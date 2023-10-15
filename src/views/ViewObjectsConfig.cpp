#include "./ViewObjectsConfig.hpp"
#include "../physics/systemState.hpp"

#define RADIUS_MIN 2.0f
#define RADIUS_MAX 100.0f

#define WIDTH_MIN 5.0f
#define WIDTH_MAX 150.0f

#define HEIGHT_MIN 5.0f
#define HEIGHT_MAX 150.0f

#define MASS_MIN 1.0f
#define MASS_MAX 200.0f

float ViewObjectsConfig::s_radius = RADIUS_MIN;
float ViewObjectsConfig::s_height = HEIGHT_MIN;
float ViewObjectsConfig::s_width = WIDTH_MIN;
float ViewObjectsConfig::s_mass = MASS_MIN;

ImU32 ViewObjectsConfig::s_color = ObjectColors::WHITE;

std::mt19937_64 ViewObjectsConfig::s_rng = std::mt19937_64();

ImGuiWindowFlags ViewObjectsConfig::window_flags = 0;

void generateRamdonColor(ImU32 &col, std::mt19937_64 rng) {
  std::uniform_int_distribution<uint32_t> dist(0, 255);
  col = IM_COL32(dist(rng), rand() % 256, rand() % 256,
                 (rand() % (256 - 99)) + 100);
}

void ViewObjectsConfig::HelpMarker(const char *desc) {
  ImGui::TextDisabled("(?)");
  if (ImGui::BeginItemTooltip()) {
    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
    ImGui::TextUnformatted(desc);
    ImGui::PopTextWrapPos();
    ImGui::EndTooltip();
  }
}

void ViewObjectsConfig::addDisclaimer() {
  ImGui::SameLine();
  HelpMarker("Click and drag to edit value.\n"
             "Hold SHIFT/ALT for faster/slower edit.\n"
             "Double-click or CTRL+click to input value.");
}

void ViewObjectsConfig::loadCircleConfigs() {

  ImGui::DragFloat("Radius", &ViewObjectsConfig::s_radius, 0.005f, RADIUS_MIN,
                   RADIUS_MAX);
}

void ViewObjectsConfig::loadSquareConfigs() {
  ImGui::DragFloat("Width", &ViewObjectsConfig::s_width, 0.005f, WIDTH_MIN,
                   WIDTH_MAX);
  ImGui::DragFloat("Height", &ViewObjectsConfig::s_height, 0.005f, HEIGHT_MIN,
                   HEIGHT_MAX);
}

void ViewObjectsConfig::ColorPicker3U32(const char *label, ImU32 *color,
                                        ImGuiColorEditFlags flags) {
  float col[4];
  col[0] = (float)((*color) & 0xFF) / 255.0f;
  col[1] = (float)((*color >> 8) & 0xFF) / 255.0f;
  col[2] = (float)((*color >> 16) & 0xFF) / 255.0f;
  col[3] = (float)((*color >> 24) & 0xFF) / 255.0f;

  bool result = ImGui::ColorPicker4(label, col, flags);

  *color = ((ImU32)(col[0] * 255.0f)) | ((ImU32)(col[1] * 255.0f) << 8) |
           ((ImU32)(col[2] * 255.0f) << 16) | (ImU32)(col[3] * 255.0f) << 24;
}

void ViewObjectsConfig::render() {
  static int e = 0;
  ImGui::Begin("Object Config", NULL, ViewObjectsConfig::window_flags);
  ImGui::RadioButton("square", &e, 0);
  ImGui::SameLine();
  ImGui::RadioButton("circle", &e, 1);
  ImGui::SameLine();
  ViewObjectsConfig::addDisclaimer();

  ImGui::Spacing();

  if (e) {
    ViewObjectsConfig::loadCircleConfigs();
  } else {
    ViewObjectsConfig::loadSquareConfigs();
  }

  ImGui::DragFloat("Mass", &ViewObjectsConfig::s_mass, 0.005f, MASS_MIN,
                   MASS_MAX);

  ImGui::Spacing();
  ImGui::Spacing();

  static int pick_color = 0;
  ImGui::RadioButton("Generate Color Randomly", &pick_color, 0);
  ImGui::SameLine();
  ImGui::RadioButton("Pick Color", &pick_color, 1);

  if (pick_color) {
    const bool generate_random_color = ImGui::Button("Pick Random Color");
    if (generate_random_color) {
      generateRamdonColor(ViewObjectsConfig::s_color, ViewObjectsConfig::s_rng);
    }
    ViewObjectsConfig::ColorPicker3U32("Color Preview",
                                       &ViewObjectsConfig::s_color);
  } else {
    generateRamdonColor(ViewObjectsConfig::s_color, ViewObjectsConfig::s_rng);
  }

  ImGui::End();
}
