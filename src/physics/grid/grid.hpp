#pragma once

#include <_types/_uint32_t.h>
#include <mutex>
#include <unordered_map>
#include <utility>
#include <vector>

#include "./gridCell.hpp"

#include "../physics_objects/physicsObject.hpp"

class Grid {
private:
  // size of width and height from objects
  uint32_t m_width = 20;
  uint32_t m_height = 20;

  // window specs
  uint32_t m_window_width;
  uint32_t m_window_height;
  ImVec2 m_window_pos;

  std::mutex m_lock;

  std::unordered_map<uint32_t, GridCell> m_cells;

public:
  void clear() {
    std::lock_guard<std::mutex> lock_guard(this->m_lock);
    this->m_cells = std::unordered_map<uint32_t, GridCell>{};
  }

  void updateCellWidthAndHeight(uint32_t width, uint32_t height) {
    this->m_width = width;
    this->m_height = height;
  }

  void updateWindowSpecs(uint32_t width, uint32_t height, ImVec2 pos) {
    std::lock_guard<std::mutex> lock_guard(this->m_lock);
    this->m_window_width = width;
    this->m_window_height = height;
    this->m_window_pos = pos;
  }

  uint32_t getWidth() const { return this->m_width; }
  uint32_t getHeight() const { return this->m_height; }

  uint32_t getCellID(uint32_t pos_x, uint32_t pos_y) const {

    const uint32_t amount_of_columns = (m_window_width / this->m_width) + 1;
    const uint32_t amount_of_rows = (m_window_height / this->m_height) + 1;

    uint32_t row = ((pos_y - m_window_pos.y) / this->m_height);
    uint32_t column = ((pos_x - m_window_pos.x) / this->m_width);

    // making sure the row and column dont leave the max range
    row = std::min(row, amount_of_rows - 1);
    column = std::min(column, amount_of_columns - 1);

    return row * amount_of_columns + column;
  }

  // have to lock getCell because unordered map will add a cell to the
  // corresponding key if the kv doesn't exist
  GridCell& getCell(uint32_t id) {
    std::lock_guard<std::mutex> lock_guard(this->m_lock);
    return this->m_cells[id];
  }

  GridCell& getCell(uint32_t pos_x, uint32_t pos_y) {
    std::lock_guard<std::mutex> lock_guard(this->m_lock);
    const uint32_t id = getCellID(pos_x, pos_y);
    return this->m_cells[id];
  }

  GridCell& getCell(std::shared_ptr<PhysicsObject> obj) {
    std::lock_guard<std::mutex> lock_guard(this->m_lock);
    const uint32_t id = getCellID(obj->getPosition().x, obj->getPosition().y);
    return this->m_cells[id];
  }

  void add(std::shared_ptr<PhysicsObject> obj) {
    std::lock_guard<std::mutex> lock_guard(this->m_lock);
    const uint32_t id = getCellID(obj->getPosition().x, obj->getPosition().y);
    this->m_cells[id].add(obj);
  }
};
