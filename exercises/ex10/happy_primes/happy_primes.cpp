#include <iostream>
#include <ios>
#include <unordered_set>
#include "sieve.cpp"

using namespace std;

int pdi_function(int number, int base) {
  int total = 0;
  while (number > 0) {
    total += (number % base) * (number % base);
    number = number / base;
  }
  return total;
}

bool is_happy(int n) {
 unordered_set<int> seen_numbers; 
 while (n > 1 && seen_numbers.find(n) == seen_numbers.end()) {
    seen_numbers.insert(n);
    n = pdi_function(n, 10);
 }
 return n == 1;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  PrimeSieve sieve(10000);
  int P, i, n;
  cin >> P;
  for (int i = 1; i <= P; ++i) {
    cin >> i >> n;
    if (sieve.is_prime(n) && is_happy(n)) {
      cout << i << " " << n << " YES\n";
    }
    else {
      cout << i << " " << n << " NO\n";
    }
  }
}
