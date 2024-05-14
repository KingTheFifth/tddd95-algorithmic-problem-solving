#include <cmath>
#include <cstdint>
#include <iostream>
#include <ios>
#include <vector>
#include "sieve.cpp"

using namespace std;
using i64 = int64_t;

int solve(vector<pair<i64, i64>> &flowers, i64 dist, PrimeSieve &sieve) {
  i64 dist_sqr = dist*dist;
  double total_dist = 0;
  i64 curr_x = 0;
  i64 curr_y = 0;
  i64 count = 0;
  i64 largest_count = 0;
  for (int i = 0; i < flowers.size(); ++i) {
    pair<i64, i64> flower = flowers[i];
    i64 x = flower.first;
    i64 y = flower.second;
    i64 dx = curr_x - x;
    i64 dy = curr_y - y;
    i64 d_sqr = dx*dx + dy*dy; 
    if (total_dist + sqrt(d_sqr) <= (double) dist) {
      count++;
      if (sieve.is_prime(count)) {
        largest_count = count;
      }
      curr_x = x;
      curr_y = y;
      total_dist += sqrt(d_sqr);
    }
    else {
      break;
    }
  }

  return largest_count;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);
  int cases, n;
  i64 d, x, y;
  PrimeSieve sieve(30000);
  cin >> cases;
  while (cases-- > 0) {
    cin >> n >> d;
    vector<pair<i64, i64>> flowers;
    for (int i = 0; i < n; ++i) {
      cin >> x >> y;
      flowers.push_back({x, y});
    }
    cout << solve(flowers, d, sieve) << "\n";
  }

  cout.flush();
}
