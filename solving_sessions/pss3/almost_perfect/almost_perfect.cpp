#include <cmath>
#include <iostream>
#include <ios>

using namespace std;

void solve(int n) {
  int sum = 1;
  for (int i = 2; i <= sqrt(n); ++i) {
    if (n % i == 0) {

      if (i == n/i) {
        sum += i;
      }
      else {
        sum += i + n/i;
      }
    }
  }
  if (sum == n) {
    cout << n << " perfect\n";
  }
  else if (sum <= n+2 && sum >= n-2) {
    cout << n << " almost perfect\n";
  }
  else {
    cout << n << " not perfect\n";
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);
  int n;
  while (true) {
    cin >> n;
    if (cin.eof()) {
      break;
    }
    solve(n);
  }

  cout.flush();
}
