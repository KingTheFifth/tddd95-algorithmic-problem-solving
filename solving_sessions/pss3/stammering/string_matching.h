#pragma once
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

vector<int> find_pattern(string pattern, string phrase) {
  vector<int> positions;
  vector<int> pi = prefix_function(pattern + "#" + phrase);
  for (int i = 0; i < pi.size(); i++) {
    if (pi[i] == pattern.size()) {
      positions.push_back(i - 2*pattern.size());
    }
  }
  return positions;
}
