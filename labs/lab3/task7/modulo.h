/**
 * Johannes Kung johku144
 * Modular arithmetic
 *
 */
#include <cstdint>
#include <iostream>
#include <tuple>

using namespace std;
using i64 = int64_t;

i64 mod(i64 a, i64 m) {
  // Make sure to not return a negative number
  return (a % m < 0) ? a % m + m : a % m;
}

/**
 * Greatest common divisor implemented using the euclidean algorithm.
 * Time complexity: 
 */
i64 gcd(i64 a, i64 b) {
  return (b==0) ? abs(a) : gcd(b, mod(a, b));
}

/**
 * Solves a linear diophantine equation on the form a*x + b*y = c, where x and y 
 * are the variables, using the extended euclidean algorithm.
 * Assumes gcd(a,b) divides c.
 * Time complexity: 
 */
tuple<i64, i64, i64> extended_euclidean(i64 a, i64 b) {
  if (b == 0) {
    return {abs(a), 1, 0};
  }

  tuple<i64, i64, i64> sub_res = extended_euclidean(b, mod(a, b));
  i64 d = get<0>(sub_res);
  i64 x0 = get<1>(sub_res);
  i64 y0 = get<2>(sub_res);
  return {d, y0, x0 - y0 * (a/b)};
}

i64 add_mod(i64 a, i64 b, i64 m) {
  return mod(mod(a, m) + mod(b, m), m);
}

i64 subtract_mod(i64 a, i64 b, i64 m) {
  return mod(mod(a, m) - mod(b, m), m);
}

/**
 * Calculates a*b (mod m) in a way that should avoid overflows.
 *
 * Time complexity: O(log(y))
 */
i64 multiply_mod_rec(i64 x, i64 y, i64 m) {
  // To avoid overflow, recursively partition b as b=(2k+d), where d is 0 or 1,
  // and calculate and sum the two products a*2k + a*d
  // The base case is b <= 2 which is a small enough b to most likely calculate 
  // the product a*b directly without overflowing
  // 
  // => the product is calculated for every time y can be partitioned by 
  // division by 2
  // => O(log(b))
  if (y <= 1) {
    return mod(x*y, m);
  }
  i64 k = y / 2;
  i64 d = y - 2*k;

  return add_mod(2*multiply_mod_rec(x, k, m), x*d, m);
}

i64 multiply_mod(i64 a, i64 b, i64 m) {
  i64 x = mod(a, m);
  i64 y = mod(b, m);
  return multiply_mod_rec(max(a,b), min(a,b), m);
}

/**
 * Calculates the inverse modulo m of a given integer a.
 * If there is no such inverse, -1 is returned.
 *
 * Time complexity:
 */
i64 inverse_mod(i64 a, i64 m) {
  // There is an inverse of a modulo m iff a and m are relatively prime i.e.
  // gcd(a, m) = 1
  //
  // Use the extended euclidean algorithm to calculate gcd(a,m) while also 
  // solving the linear diophantine equation 
  // a*a^(-1) + m*k = gcd(a, m) where a^-1 and k
  tuple<i64, i64, i64> ext_euclid = extended_euclidean(a, m);
  if (get<0>(ext_euclid) != 1) {
    return -1;
  }
  return mod(get<1>(ext_euclid), m);
}

/**
 * Calculates the division a / b (mod m), i.e. the product a * b^(-1) (mod m).
 */
i64 divide_mod(i64 a, i64 b, i64 m) {
  i64 b_inv = inverse_mod(b, m);
  return (b_inv == -1) ? -1 : multiply_mod(a, b_inv, m);
}
