#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <list>
#include <functional>

#include "gaia_random.h"
#include "grid.h"

class Person;

class World {
private:
  unsigned width, height;

public:
  World(int width, int height);

  int getWidth() const { return width; }
  int getHeight() const { return height; }

  const Grid<float> &get_food_read() const
  {
    return food_buffers[buffer_idx];
  }

  Grid<float> &get_food_write()
  {
    return food_buffers[1 - buffer_idx];
  }

  Grid<float> &get_food_growth_rate()
  {
    return food_growth_rate;
  }

  Grid<float> &get_precipitation()
  {
    return precipitation;
  }

  const Grid<int> &get_population() const
  {
    return population;
  }

  void step();

  void reset_population()
  {
    population.clear();
  }

  void add_population(unsigned i, unsigned j)
  {
    population(i,j) ++;
  }

private:
  unsigned buffer_idx;

  // These are double buffered (dependent updates)
  Grid<float> food_buffers[2];

  // These are not (non-dependent updates)
  Grid<float> food_growth_rate;
  Grid<float> precipitation;
  Grid<int>   population;

  // These are people stored per-tile for parallelism
  std::vector<std::list<Person *>> tiles;

  void step_tile(unsigned id);
};

#endif /* end of include guard: WORLD_H */
