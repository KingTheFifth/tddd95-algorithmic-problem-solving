#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

int gcd(int a, int b) {
  return (b==0) ? abs(a) : gcd(b, a%b);
}

int lcm(int a, int b) {
  return a*b/gcd(a,b);
}

class RationalNumber {
  int numerator;
  int denominator;

public:
  RationalNumber(int numerator, int denominator) {
    int d = gcd(numerator, denominator);
    this->numerator = numerator / d;
    this->denominator = denominator / d;
  }

  bool operator<(const RationalNumber &other) const {
    int l = lcm(this->denominator, other.denominator);
    return this->numerator * l < other.numerator * l;
  }
  bool operator>(const RationalNumber &other) const {
    return other < *this;
  }

  bool operator<=(const RationalNumber &other) const {
    return !(*this > other);
  }

  bool operator>=(const RationalNumber &other) const {
    return !(*this < other);
  }
  bool operator==(const RationalNumber &other) const {
    return this->numerator == other.numerator && this->denominator == other.denominator;
  }

  RationalNumber operator+(const RationalNumber &other) const {
    int l = lcm(this->denominator, other.denominator);
    int numerator = this->numerator * l/this->denominator + other.numerator * l/other.denominator;
    int d = gcd(numerator, l);
    return {numerator / d, l / d};
  }

  RationalNumber operator-(const RationalNumber &other) const {
    int l = lcm(this->denominator, other.denominator);
    //int numerator = this->numerator * l - other.numerator * l;
    int numerator = this->numerator * l/this->denominator - other.numerator * l/other.denominator;
    int d = gcd(numerator, l);
    return {numerator / d, l / d};
  }

  RationalNumber operator*(const RationalNumber &other) const {
    int numerator = this->numerator*other.numerator;
    int denominator = this->denominator*other.denominator;
    int d = gcd(numerator, denominator);
    return {numerator / d, denominator / d};
  }

  RationalNumber operator/(const RationalNumber &other) const {
    return *this * RationalNumber(other.denominator, other.numerator);
  }

  friend ostream& operator<<(ostream& os, const RationalNumber &r_n) {
    os << r_n.numerator << " / " << r_n.denominator;
    return os;
  }

  RationalNumber(const RationalNumber &) = default;
  RationalNumber(RationalNumber &&) = default;
  RationalNumber &operator=(const RationalNumber &) = default;
  RationalNumber &operator=(RationalNumber &&) = default;
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  int cases, x1, x2, y1, y2;
  string op;
  cin >> cases;
  while (cases-- > 0) {
    cin >> x1 >> y1 >> op >> x2 >> y2;
    RationalNumber n1{x1, y1};
    RationalNumber n2{x2, y2};

    if (op.compare("+") == 0) {
      cout << n1 + n2 << "\n";
    }
    if (op.compare("-") == 0) {
      cout << n1 - n2 << "\n";
    }
    if (op.compare("*") == 0) {
      cout << n1 * n2 << "\n";
    }
    if (op.compare("/") == 0) {
      cout << n1 / n2 << "\n";
    }
  }
}
