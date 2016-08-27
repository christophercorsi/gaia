#ifndef PERSON_H
#define PERSON_H

#include <list>

#include "world.h"
#include "utilities.h"

struct Person {
  std::list<Person>::iterator self_it;

  u64 id;
  u32 age;
  u32 lifetime;

  bool is_male;
  u64 father;
  u64 mother;

  // Location
  i32 x;
  i32 y;

  float food_stock;

  static constexpr float FOOD_HARVEST_PER_TURN = 10.f;
  static constexpr float CONSUMPTION_PER_TURN = 1.f;

  void move(int new_x, int new_y) {
    // It costs a little extra to move.
    food_stock -= CONSUMPTION_PER_TURN;
    x = new_x;
    y = new_y;
  }

  void step(World& world) {
    age ++;
    food_stock -= CONSUMPTION_PER_TURN;

    // Prowl for food if we're about to starve
    if(food_stock < CONSUMPTION_PER_TURN * 5) {
      int new_x, new_y;
      auto best_value = food_stock;
      world.get_food_read().gradient(x, y, &new_x, &new_y, &best_value);
      if(best_value >= 2 * food_stock){
        move(new_x, new_y);
      }
    }

    /* XXX */
    if(world.get_food_write()(x,y) > FOOD_HARVEST_PER_TURN){
      world.get_food_write()(x,y) -= FOOD_HARVEST_PER_TURN;
      food_stock += FOOD_HARVEST_PER_TURN;
    }
  }

  bool should_die() {
    if(food_stock == 0) return true;
    if(age >= lifetime) {
      //log("died of old age [%lu]", id);
      return true;
    }
    return false;
  }
};

#endif /* end of include guard: PERSON_H
 */
