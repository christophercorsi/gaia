#ifndef PERSON_H
#define PERSON_H

#include "vec2.h"
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

  void step() {
    age ++;
  }

  bool should_die() { return age >= lifetime; }
};

#endif /* end of include guard: PERSON_H
 */
