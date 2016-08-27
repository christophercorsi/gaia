#ifndef GRID_H
#define GRID_H

#include <vector>
#include "utilities.h"

template<typename T>
class Grid {
private:
  int width;
  int height;
  std::vector<T> values;

public:
  Grid() : width(0), height(0) { }

  Grid(int width, int height)
  : width(width), height(height), values(std::vector<T>(width*height))
  { }

  Grid(int width, int height, T initial_value)
  : width(width), height(height), values(std::vector<T>(width*height, initial_value))
  { }

  T& operator()(u32 x, u32 y) { return values[y*width + x]; }
  const T& operator()(u32 x, u32 y) const { return values[y*width + x]; }
};

#endif /* end of include guard: GRID_H */
