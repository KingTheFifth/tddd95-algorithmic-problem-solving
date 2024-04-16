#include <iostream>
#include <ios>
#include "sieve.cpp"

int legendre(int n, int p) {
  int p_i = p;
  int res = 0;
  while (n/p_i > 0) {
    res += n / p_i;
    p_i *= p;
  }
  return res;
}

vector<pair<int, int>> factorize(int n, vector<int> &primes) {
  vector<pair<int, int>> result; 
  for (int i = 0; i < primes.size() && n > 1; ++i) {
    int p = primes[i];
    int exp = 0;
    while (n % p == 0) {
      exp++;
      n /= p;
    }
    if (exp > 0) {
      result.push_back({p, exp});
    }
  }
  return result;
}

bool solve(int n, int m, vector<int> &primes) {
  if (n == 0 || n == 1) {
    return m == 1;
  }

  if (m == 0) {
    return false;
  }

  if (m == 1) {
    return true;
  }
   if (m <= n) {
    return true;
   }

  vector<pair<int, int>> m_factors = factorize(m, primes);
  for (pair<int, int> f : m_factors) {
    int p = f.first;
    int e = f.second;
    if (legendre(n, p) < e) {
      return false;
    }
  }
  return true;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);
  PrimeSieve sieve(100000);
  vector<int> primes = sieve.get_primes();
  int n, m;
  while (cin >> n >> m) {
    if (solve(n, m, primes)) {
      cout << m << " divides " << n << "!\n"; 
    }
    else {
      cout << m << " does not divide " << n << "!\n";
    }
  }

  cout.flush();
}
