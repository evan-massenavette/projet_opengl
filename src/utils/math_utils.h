#ifndef MATH_UTILS_HPP
#define MATH_UTILS_HPP

#include <cmath>

namespace math_utils {

// PI constant
constexpr double PI = 3.1415926535897932384626433832795028841971693993751058209;

/**
 * Positive modulo
 */

inline int pos_mod(int i, int n) {
  return (i % n + n) % n;
}

inline size_t pos_mod(size_t i, size_t n) {
  return (i % n + n) % n;
}

inline float pos_fmod(float i, float n) {
  return std::fmod(std::fmod(i, n) + n, n);
}

}  // namespace math_utils

#endif