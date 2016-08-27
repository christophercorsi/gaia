#pragma once

#include <random>
#include <cstdint>

typedef uint64_t u64;
typedef uint32_t u32;
typedef int32_t i32;
typedef float f32;

class Random {
private:
  std::mt19937 generator;

public:
  Random() : generator(std::mt19937(std::random_device()())) {
  }

  u32 uniform_u32(const int lower_bound, const int upper_bound) {
    return std::uniform_int_distribution<>(lower_bound, upper_bound)(generator);
  }
};

constexpr u32 operator"" _years(unsigned long long int value) { return 10 * value; }

#define log(fmt, ...) { printf(fmt "\n", __VA_ARGS__ );  }
