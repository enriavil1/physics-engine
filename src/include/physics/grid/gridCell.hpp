#pragma once

#include <vector>

#include "../physics_objects/physicsObject.hpp"

class GridCell {
private:
  std::vector<PhysicsObject *> m_objects = std::vector<PhysicsObject *>{};

public:
  std::vector<PhysicsObject *> getObjects() { return this->m_objects; }
  void add(PhysicsObject *obj) { m_objects.push_back(obj); }
};
