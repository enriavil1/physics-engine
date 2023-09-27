#include "./vertices.hpp"
#include <vector>

Vertex::Vertex(float x, float y) {
  this->x = x;
  this->y = y;
}

void Vertex::applyForce(const Force &force) {
  force.apply(&this->x, &this->y);
  // we go through all connnections applying the force affecting the current
  // vertex making sure the whole structure is followed
  // TODO: Add visted vertices (AKA Apply full DFS)
  for (Vertex connection : this->connections) {
    connection.applyForce(force);
  }
}

float Vertex::GetX() const { return this->x; }
void Vertex::SetX(float newX) { this->x = newX; }

float Vertex::GetY() const { return this->y; }
void Vertex::SetY(float newY) { this->y = newY; }

void Vertex::addConnection(Vertex connection) {
  this->connections.push_back(connection);
}

std::vector<Vertex> Vertex::GetConnections() { return this->connections; }
void Vertex::SetConnections(std::vector<Vertex> connections) {
  this->connections = connections;
}
