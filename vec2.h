#ifndef VEC2_H
#define VEC2_H

#include <cmath>

struct Vec2 {
  float x, y;

  Vec2 operator+(const Vec2& rhs) const { return {x+rhs.x, y+rhs.y}; }
  Vec2 operator-(const Vec2& rhs) const { return {x-rhs.x, y-rhs.y}; }
  Vec2 operator*(const float rhs) const { return {x*rhs, y*rhs}; }
  Vec2 operator/(const float rhs) const { return {x/rhs, y/rhs}; }

  Vec2& operator+=(const Vec2& rhs) { return (*this = *this + rhs); }
  Vec2& operator-=(const Vec2& rhs) { return (*this = *this - rhs); }
  Vec2& operator*=(const float rhs) { return (*this = *this * rhs); }
  Vec2& operator/=(const float rhs) { return (*this = *this / rhs); }

  float length() const { return sqrtf(x*x + y*y); }
  float dot(const Vec2& rhs) const { return x*rhs.x + y*rhs.y; }

  Vec2 unit() const { return *this / length(); }
};

inline Vec2 operator*(const float lhs, const Vec2& rhs) { return rhs * lhs; }

#endif /* end of include guard: VEC2_H */
