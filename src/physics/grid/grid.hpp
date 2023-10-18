#pragma once

#include <iostream>
#include <utility>
#include <vector>

#include "./gridCell.hpp"

#include "../physicsObjects/physicsObject.hpp"

class Grid {
private:
  // TODO (enriavil1): add variable width and height cells by taking the biggest
  // size of width and height from objects
  const uint32_t m_width = 150;
  const uint32_t m_height = 150;

  std::unordered_map<uint32_t, GridCell> m_cells =
      std::unordered_map<uint32_t, GridCell>{};

public:
  void clear() { this->m_cells = std::unordered_map<uint32_t, GridCell>{}; }

  uint32_t getCellID(uint32_t pos_x, uint32_t pos_y) {
    const auto window_height = ImGui::GetWindowHeight();
    const auto window_pos = ImGui::GetWindowPos();

    const auto amount_of_columns = (window_height / m_height);

    return ((pos_y - window_pos.y) / this->m_height) * amount_of_columns +
           ((pos_x - window_pos.x) / this->m_width);
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

  void add(PhysicsObject *obj, uint32_t obj_index) {
    const uint32_t id = getCellID(obj->getPosition().x, obj->getPosition().y);

    if (this->m_cells.find(id) == this->m_cells.end()) {
      this->m_cells.insert(std::make_pair(id, GridCell()));
    }

    this->m_cells[id].add(obj);
  }
};
