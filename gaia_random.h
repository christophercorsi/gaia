#ifndef GAIA_RANDOM_H
#define GAIA_RANDOM_H

#include <random>
#include "utilities.h"

class Random {
private:
  std::mt19937 generator;

public:
  Random() : generator(std::mt19937(std::random_device()())) {
  }

  u32 uniform_u32(const int lower_bound, const int upper_bound) {
    return std::uniform_int_distribution<>(lower_bound, upper_bound)(generator);
  }

  f32 uniform_f32(const float lower_bound, const float upper_bound) {
    return std::uniform_real_distribution<float>(lower_bound, upper_bound)(generator);
  }

  i32 uniform_i32(const int lower_bound, const int upper_bound) {
    return std::uniform_int_distribution<int>(lower_bound, upper_bound)(generator);
  }
};

#endif /* end of include guard: GAIA_RANDOM_H */
