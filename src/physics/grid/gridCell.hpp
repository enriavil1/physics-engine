#pragma once

#include <mutex>
#include <vector>

#include "../physics_objects/physicsObject.hpp"

class GridCell {
private:
  std::vector<std::shared_ptr<PhysicsObject>> m_objects =
      std::vector<std::shared_ptr<PhysicsObject>>{};

  std::mutex m_lock;

public:
  std::vector<std::shared_ptr<PhysicsObject>>& getObjects() {
    return this->m_objects;
  }
  void add(std::shared_ptr<PhysicsObject> obj) {
    std::lock_guard<std::mutex> lock_guard(this->m_lock);
    m_objects.push_back(obj);
  }
};
