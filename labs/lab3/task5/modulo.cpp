/**
 * Johannes Kung johku144
 * Modular arithmetic
 *
 */
#include <cstdint>
#include <iostream>
#include <ios>

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
pair<i64, i64> extended_euclidean(i64 a, i64 b, i64 c) {
  if (b == 0) {
    return {c / a, 0};
  }

  pair<i64, i64> sub_res = extended_euclidean(b, mod(a, b), c);
  i64 x0 = sub_res.first;
  i64 y0 = sub_res.second;
  return {y0, x0 - y0 * (a/b)};
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
  if (y <= 2) {
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
  // There is an inverse of a modulo m iff a and m are relatively prime
  if (gcd(a, m) != 1) {
    return -1;
  }
  // Solve the linear diophantine equation a*a^(-1) + m*k = 1 where a^(-1)
  // and k are the variables
  return mod(extended_euclidean(a, m, 1).first, m);
}

/**
 * Calculates the division a / b (mod m), i.e. the product a * b^(-1) (mod m).
 */
i64 divide_mod(i64 a, i64 b, i64 m) {
  i64 b_inv = inverse_mod(b, m);
  return (b_inv == -1) ? -1 : multiply_mod(a, b_inv, m);
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);
  i64 cases, m, a, b;
  string op;
  while (true) {
    cin >> m >> cases;
    if (m == 0 & cases == 0) {
      break;
    }
    while (cases-- > 0) {
      cin >> a >> op >> b;
      if (op.compare("+") == 0) {
        cout << add_mod(a, b, m) << "\n";
      }
      else if (op.compare("-") == 0) {
        cout << subtract_mod(a, b, m) << "\n";
      }
      else if (op.compare("*") == 0) {
        cout << multiply_mod(a, b, m) << "\n";
      }
      else if (op.compare("/") == 0) {
        cout << divide_mod(a, b, m) << "\n";
      }
    }
  }

  cout.flush();
}
