#include "./drawView.hpp"

ImGuiWindowFlags DrawView::s_window_flags = ImGuiWindowFlags_NoCollapse;

void DrawView::handleLeftClick() {
  ImGuiIO& io = ImGui::GetIO();
  auto mouse_pos = io.MousePos;

  if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
    SystemState::UpdatePickedObject(mouse_pos.x, mouse_pos.y);
  } else if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
    SystemState::SetPickedObject(mouse_pos.x, mouse_pos.y);
  }

  if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
    SystemState::UnsetPickedObject();
  }
}

void DrawView::handleRightClick() {
  ImGuiIO& io = ImGui::GetIO();

  auto add_circles = [io]() {
    auto mouse_pos = io.MousePos;
    SystemState::AddObject(new CircleObject(
        ViewObjectsConfig::GetMass(), mouse_pos.x, mouse_pos.y,
        ViewObjectsConfig::GetRadius(), ViewObjectsConfig::GetColor()));
  };

  if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
    add_circles();
  } else if (ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
    add_circles();
  }
}

void DrawView::render(const float& dt) {
  ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(50, 50, 50, 255));

  const auto view_port_size = ImGui::GetMainViewport()->WorkSize;
  ImGui::Begin("Draw View", NULL, DrawView::s_window_flags);

  handleLeftClick();
  handleRightClick();

  SystemState::Update(dt);

  ImGui::End();
  ImGui::PopStyleColor();
}
