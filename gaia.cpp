#include <cstdio>
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <functional>
#include "utilities.h"

Random random_generator;

// One discrete time step is a Metric Month (a tenth of a year).

struct Tile {
  float current_food;
};

struct World {
  int width, height;
  std::vector<Tile*> tiles;

  World(int width, int height, std::function<Tile*(u32, u32)> tileGenerator)
  : width(width), height(height) {

    tiles = std::vector<Tile*>(width * height);
    for(u32 y=0; y<height; ++y)
      for(u32 x=0; x<width; ++x)
        tiles[y*width + x] = tileGenerator(x,y);
  }

  Tile* get_random_tile() {
    return tiles[ random_generator.uniform_u32(0, tiles.size()-1) ];
  }
};

struct Person {
  std::string name;
  u32 age;
  u32 lifetime;
  Tile* location;

  static std::string random_name(){
    static u64 uid = 0;
    std::stringstream name;
    name << "Person_" << uid++;
    return name.str();
  }

  bool should_die() {
    return age >= lifetime;
  }
};

Person* newGenesisPerson(World& world) {
  auto* person = new Person {
    .name = Person::random_name(),
    .age = 0,
    .lifetime = random_generator.uniform_u32(50_years, 90_years),
    .location = world.get_random_tile()
  };

  return person;
}

struct Simulation {
  World world;
  std::vector<Person*> people;

  Simulation(int width, int height, int n_people, std::function<Tile*(u32,u32)> tile_generator)
  : world(World(width, height, tile_generator)) {

    // Construct people!
    people = std::vector<Person*>();
    people.reserve( n_people );
    for(size_t i=0; i<n_people; ++i) {
      people.push_back( newGenesisPerson(world) );
    }

    log("Create %lu people during Genesis", people.size());
  }

  u64 time_step = 0;
  void step() {
    log("Simulating time step %lu", time_step);

    // Age people
    for(auto* person : people) {
      person->age ++;
    }

    // Expire people
    people.erase(std::remove_if(people.begin(), people.end(), [](Person* person){
      const bool result = person->should_die();
      if(result) log("Killing %s at age %u", person->name.c_str(), person->age);
      return result;
    }), people.end());

    time_step ++;
  }

};

int main(int argc, char **argv) {
    printf("Gaia!\n");

    const auto n_starting_people = 1000u;
    auto simulation = Simulation(64, 64, n_starting_people, [](u32 x, u32 y) -> Tile* {
      log("Generating new tile at (%u, %u)", x, y)
      return new Tile{
        .current_food = 100
      };
    });

    for(int i=0; i<100_years; ++i) {
      simulation.step();
    }

    return 0;
}
