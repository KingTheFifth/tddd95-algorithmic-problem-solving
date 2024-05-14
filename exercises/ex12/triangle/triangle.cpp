#include <cstdlib>
#include <iostream>
#include <ios>

using namespace std;
using vec = pair<int, int>;

vec operator*(const int& c, const vec& v) {
  return {v.first*c, v.second*c};
}

int dot(vec &v1, vec &v2) {
  return v1.first * v2.first + v1.second * v2.second;
}

int area(vec &p1, vec &p2, vec &p3) {
  return abs((p1.first - p3.first)*(p2.second - p1.second) - (p1.first-p2.first)*(p3.second-p1.second));
}

bool equal_lenght(vec &v1, vec &v2) {
  return v1.first*v1.first + v1.second*v1.second == v2.first*v2.first + v2.second*v2.second;
}

string solve(vec &p1, vec &p2, vec &p3) {
  if (area(p1, p2, p3) == 0) {
    return "not a triangle";
  }

  string result = "";
  vec v1 = {p2.first-p1.first, p2.second-p1.second};
  vec v2 = {p3.first-p1.first, p3.second-p1.second};
  vec v3 = {p3.first-p2.first, p3.second-p2.second};
  vec v4 = -1*v3;
  int angle_1 = dot(v1, v2);
  int angle_2 = dot(v1, v4);
  int angle_3 = dot(v2, v3);

  if (equal_lenght(v1, v2) || equal_lenght(v1, v3) || equal_lenght(v2, v3)) {
    result += "isosceles ";
  }
  else {
    result += "scalene ";
  }
  if (angle_1 == 0 || angle_2 == 0 || angle_3 == 0) {
    result += "right ";
  }
  else if (angle_1 < 0 || angle_2 < 0 || angle_3 < 0) {
    result += "obtuse ";
  }
  else {
    result += "acute ";
  }

  result += "triangle";
  return result;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);
  int cases, x, y;
  cin >> cases;
  for (int i = 1; i <= cases; ++i) {
    cin >> x >> y;
    vec p1 = {x, y};
    cin >> x >> y;
    vec p2 = {x, y};
    cin >> x >> y;
    vec p3 = {x, y};
    cout << "Case #" << i << ": " << solve(p1, p2, p3) << "\n";
  }
  cout.flush();
}
