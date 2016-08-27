#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include <functional>
#include "utilities.h"

#include "world.h"
#include "person.h"

class Simulation {
private:
  World world;
  std::vector<Person*> people;
  u64 time_step;

public:
  Simulation(int n_people, std::function<Person*()> person_generator, World& world);
  void step();
  u64 getTimeStep() const { return time_step; }
  const std::vector<Person*>& getPeople() const { return people; }

  const World& getWorld() const { return world; }
};

#endif /* end of include guard: SIMULATION_H */
