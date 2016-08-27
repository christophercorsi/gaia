#include <algorithm>
#include "simulation.h"

Simulation::Simulation(int n_people, std::function<Person*()> person_generator, World& world)
: world(world), time_step(0) {

  // Construct people!
  people = std::vector<Person*>();
  people.reserve( n_people );
  for(size_t i=0; i<n_people; ++i) {
    people.push_back( person_generator() );
  }

  log("Create %lu people during Genesis", people.size());
}

void Simulation::step() {
  log("Simulating time step %lu", time_step);

  // TODO : multi-thread this.

  // Advance the world state
  world.step();

  // Let people advance (increase age, move, etc.)
  for(auto* person : people)
    person->step(world);

  world.reset_population();
  for(auto* person : people)
    world.add_population(person->x, person->y);

  // Expire people
  people.erase(std::remove_if(people.begin(), people.end(), [](Person* person){
    return person->should_die();
  }), people.end());

  time_step ++;
}
