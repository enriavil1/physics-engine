#include "../forces/force.hpp"
#include "vector"

class Vertex {
private:
  float x;
  float y;

  std::vector<Vertex> connections;

public:
  Vertex(float x, float y);

  void applyForce(const Force &force);

  float GetX() const;
  void SetX(float newX);

  float GetY() const;
  void SetY(float newY);

  void addConnection(Vertex connection);
  void SetConnections(std::vector<Vertex> connections);
  std::vector<Vertex> GetConnections();
};
