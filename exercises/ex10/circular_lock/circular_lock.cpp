#include <iostream>
#include <ios>
#include <numeric>

using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  int cases = 0;
  cin >> cases;
  while (cases-- > 0) {
    int s11, s12, s21, s22, p11, p12, p21, p22;
    cin >> s11 >> s12 >> p11 >> p12 >> s21 >> s22 >> p21 >> p22;
    int p = gcd(gcd(gcd(p11, p12), p21), p22);
    s11 = s11 % p;
    s12 = s12 % p;
    s21 = s21 % p;
    s22 = s22 % p;
    if ((s11+s22) %p == (s12 + s21) %p) {
      cout << "Yes\n";
    }
    else {
      cout << "No\n";
    }
  }

  cout.flush();
}
