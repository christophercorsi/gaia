#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <functional>

#include "gaia_random.h"
#include "grid.h"

class World {
private:
  int width, height;

public:
  World(int width, int height);

  int getWidth() const { return width; }
  int getHeight() const { return height; }

  Grid<float> food;
  Grid<float> food_growth_rate;
  Grid<float> precipitation;
  Grid<int>   population;

  void step();
};

#endif /* end of include guard: WORLD_H */
