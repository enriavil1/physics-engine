#include "./force.hpp"

class Gravity : Force {
public:
  void apply(float *_, float *y) const { *y -= 1; }
};
