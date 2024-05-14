/**
 * Johannes Kung johku144
 * Rational arithmetics (+, -, *, /) on fractions a/b where a & b are integers.
 * Implemented using a class with relevant operators overloaded.
 */
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;
using i64 = int64_t;

/**
 * The euclidean algorithm for calculating gcd(a, b).
 * Time complexity: O(log(min(a, b)))
 */
i64 gcd(i64 a, i64 b) {
  return (b==0) ? abs(a) : gcd(b, a%b);
}

/**
 * lcm(a, b) using gcd(a, b).
 * Time complexity: O(log(min(a, b)))
 */
i64 lcm(i64 a, i64 b) {
  if (a >= b) return (a/gcd(a,b))*b;
  return a*(b/gcd(a,b));
}

/**
 * Class for representing a rational number on the form a/b where a & b are 
 * integers. Contains methods for basic arithmetics and printing.
 */
class RationalNumber {
  i64 numerator;
  i64 denominator;

public:
  RationalNumber(i64 numerator, i64 denominator) {
    // Make sure to simplify the fraction as much as possible, i.e. store
    // it in a normalized form, as this helps avoid overflows
    i64 d = gcd(numerator, denominator);
    this->numerator = numerator / d;
    this->denominator = denominator / d;

    // Keep the negative sign in the numerator instead of the denominator 
    // for convenience
    if (denominator < 0) {
      this->numerator *= -1;
      this->denominator *= -1;
    }
  }

  bool operator<(const RationalNumber &other) const {
    i64 l = lcm(this->denominator, other.denominator);
    return this->numerator * other.denominator < other.numerator * this->denominator;
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

  bool operator!=(const RationalNumber &other) const {
    return !(*this == other);
  }

  RationalNumber operator+(const RationalNumber &other) const {
    i64 l = lcm(this->denominator, other.denominator);
    i64 numerator = this->numerator * (l/this->denominator) + other.numerator * (l/other.denominator);
    // The constructor will normalize the resulting fraction
    return {numerator, l};
  }

  RationalNumber operator-(const RationalNumber &other) const {
    return *this + RationalNumber(-1, 1) * other;
  }

  RationalNumber operator*(const RationalNumber &other) const {
    i64 n1 = this->numerator;
    i64 n2 = other.numerator;
    i64 d1 = this->denominator;
    i64 d2 = other.denominator;

    i64 f1 = gcd(n1, d2);
    i64 f2 = gcd(n2, d1);

    i64 numerator = (n1/f1)*(n2/f2);
    i64 denominator = (d1/f2) * (d2/f1);
    return {numerator, denominator};
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

  i64 cases, x1, x2, y1, y2;
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
