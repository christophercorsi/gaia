#ifndef GRID_H
#define GRID_H

#include <vector>
#include <functional>
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

  Grid(u32 width, u32 height, std::function<T(u32,u32)> initial_value_generator)
  : width(width), height(height), values(std::vector<T>(width*height, 0))
  {
    for(int y=0; y<height; ++y)
      for(int x=0; x<width; ++x)
        get(x,y) = initial_value_generator(x,y);
  }

  T& operator()(u32 x, u32 y) { return get(x,y); }
  const T& operator()(u32 x, u32 y) const { return get(x,y); }

  void gradient(int x, int y, int* best_x, int* best_y, T* best_value) const {
    *best_value = operator()(x,y);
    T test = 0;
    *best_x = x, *best_y = y;
    if(x>0        && (test = get(x-1,y)) > *best_value) { *best_value = test; *best_x = x-1; }
    if(x<width-1  && (test = get(x+1,y)) > *best_value) { *best_value = test; *best_x = x+1; }
    if(y>0        && (test = get(x,y-1)) > *best_value) { *best_value = test; *best_y = y-1; }
    if(y<height-1 && (test = get(x,y+1)) > *best_value) { *best_value = test; *best_y = y+1; }
  }

  void clear() { size_t n = values.size(); values = std::vector<T>(n, 0); }
};

#endif /* end of include guard: GRID_H */
