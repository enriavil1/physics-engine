#pragma once

#include <unordered_map>
#include <utility>
#include <vector>

#include "./gridCell.hpp"

#include "../physicsObjects/physicsObject.hpp"

class Grid {
private:
  // TODO (enriavil1): add variable width and height cells by taking the biggest
  // size of width and height from objects
  const uint32_t m_width = 20;
  const uint32_t m_height = 20;

  std::unordered_map<uint32_t, GridCell> m_cells =
      std::unordered_map<uint32_t, GridCell>{};

public:
  void clear() { this->m_cells = std::unordered_map<uint32_t, GridCell>{}; }

  uint32_t getWidth() { return this->m_width; }
  uint32_t getHeight() { return this->m_height; }

  uint32_t getCellID(uint32_t pos_x, uint32_t pos_y) {
    const auto window_width = ImGui::GetWindowWidth();
    const auto window_height = ImGui::GetWindowHeight();
    const auto window_pos = ImGui::GetWindowPos();

    const uint32_t amount_of_columns = (window_width / this->m_width) + 1;
    const uint32_t amount_of_rows = (window_height / this->m_height) + 1;

    uint32_t row = ((pos_y - window_pos.y) / this->m_height);
    uint32_t column = ((pos_x - window_pos.x) / this->m_width);

    // making sure the row and column dont leave the max range
    row = std::min(row, amount_of_rows - 1);
    column = std::min(column, amount_of_columns - 1);

    return row * amount_of_columns + column;
  }

  GridCell& getCell(uint32_t id) { return this->m_cells[id]; }

  GridCell& getCell(uint32_t pos_x, uint32_t pos_y) {
    const uint32_t id = getCellID(pos_x, pos_y);
    return this->m_cells[id];
  }

  GridCell& getCell(PhysicsObject *obj) {
    const uint32_t id = getCellID(obj->getPosition().x, obj->getPosition().y);
    return this->m_cells[id];
  }

  void add(PhysicsObject *obj) {
    const uint32_t id = getCellID(obj->getPosition().x, obj->getPosition().y);

    this->m_cells[id].add(obj);
  }
};
