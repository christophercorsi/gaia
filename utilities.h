#pragma once

#include <random>
#include <cstdint>

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint8_t u8;
typedef int32_t i32;
typedef float f32;

constexpr u32 operator"" _years(unsigned long long int value) { return 10 * value; }

#define log(fmt, ...) { printf(fmt "\n", __VA_ARGS__ );  }
