#include <algorithm>
#include <iostream>
#include <ios>
#include <string>
#include <vector>

using namespace std;

char smallest_letter(string &s) {
  return *min_element(s.begin(), s.end());
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);
  string in;
  while (cin >> in) {
    string result = "";
    //char c = *min_element(in.begin(), in.end());
    //int pos = in.find_first_of(c);
    char c;
    int pos = -1;
    for (int i = 0; i < in.size()-2; ++i) {
      if (pos == -1) {
        c = in[i];
        pos = i;
      }
      if (in[i] < c) {
        c = in[i];
        pos = i;
      }
    }
    string s = in.substr(0, pos+1);
    reverse(s.begin(), s.end());
    result += s;

    in = in.substr(pos+1, in.size()-pos);
    pos = -1;
    for (int i = 0; i < in.size()-1; ++i) {
      if (pos == -1) {
        c = in[i];
        pos = i;
      }
      if (in[i] < c) {
        c = in[i];
        pos = i;
      }
    }
    s = in.substr(0, pos+1);
    reverse(s.begin(), s.end());
    result += s;
    in = in.substr(pos+1, in.size()-pos);

    reverse(in.begin(), in.end());
    result += in;
   
    cout << result << "\n";
  }

  cout.flush();
}
