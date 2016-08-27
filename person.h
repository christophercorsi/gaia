#ifndef PERSON_H
#define PERSON_H

#include "world.h"
#include "utilities.h"

struct Person {
  u64 id;
  u32 age;
  u32 lifetime;

  bool is_male;
  u64 father;
  u64 mother;

  // Location
  i32 x;
  i32 y;

  u32 food_stock;

  void step(World& world) {
    age ++;
    food_stock --;

    if(world.food(x,y) > 0){
      world.food(x,y) -= 1.f;
      food_stock += 1;
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
