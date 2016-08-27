#include "world.h"

#include <cmath>

World::World(int width, int height)
: width(width), height(height) {

  food_growth_rate = Grid<float>(width, height, [](u32 x, u32 y){
    float val = 10.f * cosf(x * 0.0025f);
    return std::max(0.1f, std::min(1000.f, val));
  });

  food = Grid<float>(width, height, [](u32 x, u32 y){
    float val = 1700.f * cosf(x * 0.03f) * cosf(y * 0.02f);
    return std::max(0.f, std::min(10000.f, val));
  });

  precipitation = Grid<float>(width, height, 0.f);
}

void World::step() {

  for(int j=0; j<height; ++j)
    for(int i=0; i<width; ++i) {
      precipitation(i,j) = 0.f;
      food(i,j) += food_growth_rate(i,j); // TODO : precipitation
    }

}
