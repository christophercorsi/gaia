#include "simulation.h"

Simulation::Simulation(int n_people, std::function<Person(const World &)> person_generator,
                       int width, int height)
: world(width, height), time_step(0) {

  // Construct people!
  for(size_t i=0; i<n_people; ++i) {
    world.add_person( person_generator(world) );
  }

  log("Create %lu people during Genesis", n_people);
}

void Simulation::step() {
  log("Simulating time step %lu", time_step);

  // TODO : multi-thread this.

  // Advance the world state
  world.step();

  time_step ++;
}
