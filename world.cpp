#include "world.h"

World::World(int width, int height)
: width(width), height(height) {

  food_growth_rate = Grid<float>(width, height, 1.f);
  food = Grid<float>(width, height, 100.f);
  precipitation = Grid<float>(width, height, 0.f);
}

void World::step() {

  for(int j=0; j<height; ++j)
    for(int i=0; i<width; ++i) {
      precipitation(i,j) = 0.f;
      food(i,j) += food_growth_rate(i,j); // TODO : precipitation
    }

}
