#ifndef GRID_H
#define GRID_H

#include <vector>
#include "utilities.h"

template<typename T>
class Grid {
private:
  u32 width;
  u32 height;
  std::vector<T> values;

  T& get(u32 x, u32 y) { return values[y*width + x]; }
  const T& get(u32 x, u32 y) const { return values[y*width + x]; }

public:
  Grid() : width(0), height(0) { }

  Grid(u32 width, u32 height)
  : width(width), height(height), values(std::vector<T>(width*height))
  { }

  Grid(u32 width, u32 height, T initial_value)
  : width(width), height(height), values(std::vector<T>(width*height, initial_value))
  { }

  T& operator()(u32 x, u32 y) { return get(x,y); }
  const T& operator()(u32 x, u32 y) const { return get(x,y); }

  void gradient(int x, int y, int* best_x, int* best_y) const {
    T best_value = operator()(x,y), test = 0;
    *best_x = x, *best_y = y;
    if(x>0        && (test = get(x-1,y)) > best_value) { best_value = test; *best_x = x-1; }
    if(x<width-1  && (test = get(x+1,y)) > best_value) { best_value = test; *best_x = x+1; }
    if(y>0        && (test = get(x,y-1)) > best_value) { best_value = test; *best_y = y-1; }
    if(y<height-1 && (test = get(x,y+1)) > best_value) { best_value = test; *best_y = y+1; }
  }
};

#endif /* end of include guard: GRID_H */
