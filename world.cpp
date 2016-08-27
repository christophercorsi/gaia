#include "world.h"
#include "person.h"

#include <cassert>
#include <algorithm>
#include <cmath>

static constexpr unsigned TILE_WIDTH = 16;
static constexpr unsigned TILE_HEIGHT = 16;

constexpr unsigned roundup(unsigned a, unsigned b) {
  return (a + b - 1) / b;
}

World::World(int width, int height)
: width(width), height(height), buffer_idx(0) {

  const unsigned tiles_x = roundup(width, TILE_WIDTH);
  tiles.resize(roundup(width, TILE_WIDTH) * roundup(height, TILE_HEIGHT));
  for (size_t i = 0; i < tiles.size(); ++i) {
    tiles[i].tx = i % tiles_x;
    tiles[i].ty = i / tiles_x;
  }

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

  population.clear();
  total_population.store(0);

  for (size_t i = 0; i < tiles.size(); ++i) {
    step_tile(i);
  }

  // TODO optimize this
  for (size_t i = 0; i < tiles.size(); ++i) {
    move_tile(i);
  }
}

void World::add_person(Person &&person) {
  const unsigned tiles_x = roundup(width, TILE_WIDTH);
  const unsigned tx = person.x / TILE_WIDTH, ty = person.y / TILE_HEIGHT;

  Tile &tile = tiles[(ty * tiles_x) + tx];

  tile.people.emplace_front(person);
  tile.people.begin()->self_it = tile.people.begin();
}

void World::step_tile(unsigned tile_id)
{
  Tile &tile = tiles[tile_id];

  const unsigned start_x = tile.tx * TILE_WIDTH;
  const unsigned start_y = tile.ty * TILE_HEIGHT;
  const unsigned end_x = std::min(start_x + TILE_WIDTH, width);
  const unsigned end_y = std::min(start_y + TILE_HEIGHT, height);

  for(int j=start_y; j<end_y; ++j)
    for(int i=start_x; i<end_x; ++i) {
      get_food_write()(i,j) = get_food_read()(i,j);
    }

  // Accept people migration
  tile.people.splice(tile.people.begin(), tile.new_people,
                     tile.new_people.begin(), tile.new_people.end());

  // Let people advance (increase age, move, etc.)
  total_population.fetch_add(tile.people.size());
  for (auto &person : tile.people) {
    population(person.x, person.y) ++;
    person.step( *this );
  }

  // Expire people
  tile.people.erase(std::remove_if(tile.people.begin(), tile.people.end(), [](Person &person){
    return person.should_die();
  }), tile.people.end());


  for(int j=start_y; j<end_y; ++j)
    for(int i=start_x; i<end_x; ++i) {
      precipitation(i,j) = 0.f;
      get_food_write()(i,j) += food_growth_rate(i,j); // TODO : precipitation
    }
}

void World::move_tile(unsigned tile_id)
{
  const unsigned tiles_x = roundup(width, TILE_WIDTH);
  Tile &tile = tiles[tile_id];

  std::list<Person>::iterator it = tile.people.begin();
  while (it != tile.people.end()) {
    Person &person = *it;
    unsigned new_tx = person.x / TILE_WIDTH, new_ty = person.y / TILE_HEIGHT;
    unsigned new_id = (new_ty * tiles_x) + new_tx;

    if (new_id != tile_id) {
      Tile &new_tile = tiles[new_id];
      auto new_it = std::next(it, 1);
      new_tile.new_people.splice(new_tile.new_people.end(), tile.people, it);
      it = new_it;
   } else {
     ++it;
   }
  }
}
