#pragma once

#include <vector>

#include "../physics_objects/physicsObject.hpp"

class GridCell {
private:
  std::vector<std::shared_ptr<PhysicsObject>> m_objects =
      std::vector<std::shared_ptr<PhysicsObject>>{};

public:
  std::vector<std::shared_ptr<PhysicsObject>> getObjects() {
    return this->m_objects;
  }
  void add(std::shared_ptr<PhysicsObject> obj) { m_objects.push_back(obj); }
};
