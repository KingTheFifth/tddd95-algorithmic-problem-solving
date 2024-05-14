#include <algorithm>
#include <iostream>
#include <ios>
#include <string>
#include <vector>
#include "string_matching.h"
#include "suffix_array.h"

using namespace std;

bool lcp_entry_comp(pair<int, int> e1, pair<int, int> e2) {
  return e1.second > e2.second;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);
  int m;
  string in;
  while (true) {
    cin >> m;
    if (m == 0 || cin.eof()) {
      break;
    }
    cin.ignore();
    getline(cin, in);

    SuffixArray sa(in);
    vector<int> lcp = sa.get_lcp();
    vector<pair<int, int>> lcp_sorted;
    for (int i = 0; i < lcp.size(); ++i) {
      lcp_sorted.push_back({i, lcp[i]});
    }
    stable_sort(lcp_sorted.begin(), lcp_sorted.end(), lcp_entry_comp);

    int best_pos = -1;
    int best_length = -1;
    for (pair<int, int> e : lcp_sorted) {
      if (e.second == 0) continue;
      string s = in.substr(sa.get_suffix(e.first), e.second);
      vector<int> occurences = find_pattern(s, in);
      if (occurences.size() >= m) {
        best_pos = occurences.back();
        best_length = s.size();
        break;
      }
    }

    if (best_pos != -1) {
      cout << best_length << " " << best_pos << "\n";
    }
    else {
      cout << "none\n";
    }

  }

  cout.flush();
}
