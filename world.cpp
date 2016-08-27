#include "world.h"
#include "person.h"

#include <cmath>

static constexpr unsigned TILE_WIDTH = 16;
static constexpr unsigned TILE_HEIGHT = 16;

constexpr unsigned roundup(unsigned a, unsigned b) {
  return (a + b - 1) / b;
}

World::World(int width, int height)
: width(width), height(height), buffer_idx(0) {

  tiles.resize(roundup(width, TILE_WIDTH) * roundup(height, TILE_HEIGHT));

  food_growth_rate = Grid<float>(width, height, [](u32 x, u32 y){
    float val = 10.f * cosf(x * 0.0025f);
    return std::max(0.1f, std::min(1000.f, val));
  });

  food_buffers[0] = Grid<float>(width, height, [](u32 x, u32 y){
    float val = 4000.f * cosf(x * 0.03f) * cosf(y * 0.02f);
    return std::max(0.f, std::min(10000.f, val));
  });

  food_buffers[1] = Grid<float>(width, height, [](u32 x, u32 y){
    float val = 4000.f * cosf(x * 0.03f) * cosf(y * 0.02f);
    return std::max(0.f, std::min(10000.f, val));
  });

  precipitation = Grid<float>(width, height, 0.f);
  population = Grid<int>(width, height, 0);
}

void World::step() {

  buffer_idx = 1 - buffer_idx;

  for (size_t i = 0; i < tiles.size(); ++i) {
    step_tile(i);
  }
}

void World::step_tile(unsigned tile_id)
{
  const unsigned tiles_x = roundup(width, TILE_WIDTH);
  const unsigned tx = tile_id % tiles_x, ty = tile_id / tiles_x;
  const unsigned start_x = tx * TILE_WIDTH, start_y = ty * TILE_HEIGHT;
  const unsigned end_x = std::min(start_x + TILE_WIDTH, width);
  const unsigned end_y = std::min(start_y + TILE_HEIGHT, height);

  for(int j=start_x; j<end_x; ++j)
    for(int i=start_y; i<end_y; ++i) {
      get_food_write()(i,j) = get_food_read()(i,j);
    }

  // TODO add to this list in simulation
  for (auto person : tiles[ty * tiles_x + tx]) {
    person->step(*this);
  }

  for(int j=start_x; j<end_x; ++j)
    for(int i=start_y; i<end_y; ++i) {
      precipitation(i,j) = 0.f;
      get_food_write()(i,j) += food_growth_rate(i,j); // TODO : precipitation
    }
}
