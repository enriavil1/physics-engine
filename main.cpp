#include <iostream>

#include "imgui/imgui.h"
#include "objects/vertex.hpp"

int main() {
  std::cout << "Hello World!"
            << "\n";

  auto vertex = Vertex(0, 0);
  std::cout << "Created vertex with x-cordinate: " << vertex.GetX()
            << " and y-cordinate: " << vertex.GetY() << "\n";
}
