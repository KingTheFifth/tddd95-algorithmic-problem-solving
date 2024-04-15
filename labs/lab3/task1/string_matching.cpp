#include <iostream>
#include <ios>
#include <string>
#include <vector>

using namespace std;

vector<int> prefix_function(string s) {
  vector<int> pi(s.size());
  for (int i = 1; i < s.size(); i++) {
    int j = pi[i-1];
    while (j > 0 && s[i] != s[j]) {
      j = pi[j-1];
    }

    if (s[i] == s[j]) {
      j++;
    }

    pi[i] = j;
  }

  return pi;
}

vector<int> find(string pattern, string phrase) {
  vector<int> positions;
  vector<int> pi = prefix_function(pattern + "#" + phrase);
  for (int i = 0; i < pi.size(); i++) {
    if (pi[i] == pattern.size()) {
      positions.push_back(i - 2*pattern.size());
    }
  }
  return positions;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);
  string pattern, phrase;
  while (true) {
    getline(cin, pattern);
    getline(cin, phrase);
    if (cin.eof()) {
      break;
    }

    vector<int> positions = find(pattern, phrase);

    // Print out result with nice formatting
    for (int i = 0; i < positions.size(); i++) {
      cout << positions[i];
      if (i < positions.size() - 1) {
        cout << " ";
      }
    }
    cout << "\n";
  }

  cout.flush();
}
