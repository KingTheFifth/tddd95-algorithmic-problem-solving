#include <cstdint>
#include <iostream>
#include <ios>
#include <tuple>
#include "modulo.h"

using namespace std;
using i64 = int64_t;

pair<i64, i64> chinese_remainder(i64 a, i64 n, i64 b, i64 m) {
  // Solve the linear diophantine equation n*j - m*k = 1 for the variables 
  // j and k. This is equivalent to solving n*j + m*l = 1 where l = -k.
  // (the extended euclidean algorithm implementation assumes positive coefficients)
  // The multiply j and k with (b-a) to get the solution to the equation system:
  // x = a + n*j, x = b + m*k
  tuple<i64, i64, i64> ext_euclid = extended_euclidean(n, m);
  i64 d = get<0>(ext_euclid);

  if ((b-a) % d != 0) {
    return {-1, -1};
  }

  i64 j = get<1>(ext_euclid);
  i64 k = get<2>(ext_euclid);
  i64 K = n*(m/d);  // lcm(a,b) = a*b/gcd(a,b)
  i64 factor = (b-a)/d;
  i64 real_j = multiply_mod(factor, j, K);
  i64 x = add_mod(a, multiply_mod(real_j, n, K), K);
  return {x, K};
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);
  i64 cases, a, n, b, m;
  cin >> cases;
  while (cases-- > 0) {
    cin >> a >> n >> b >> m;
    pair<i64, i64> res = chinese_remainder(a, n, b, m);
    if (res.first == -1 && res.second == -1) {
      cout << "no solution\n";
    }
    else {
      cout << res.first << " " << res.second << "\n";
    }
  }

  cout.flush();

}
