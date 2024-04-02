#include <iostream>
#include <ios>
#include <string>
#include <vector>

using namespace std;

bool lock_it_in(string &result, string suffix, int pos) {
  // Used to correctly calculate the position in the reconstructed string of a 
  // character in the suffix given that a wildcard * present in the suffix has 
  // been skipped while iterating

  // Merge in forwards direction until finding a '*' in the suffix
  int res_str_pos = pos;
  for (char s_c : suffix) {
    if (s_c == '*') {
      break;
    }
    // There is a * in the reconstructed string => replace it with the suffix 
    // character
    else if (result[res_str_pos] == '*') {
      result[res_str_pos] = s_c;
    }

    // The character of the substring differs from the character of the 
    // reconstructed string, and neither is a wildcard * =>
    // Conflict! Cannot reconstruct the string!
    else if (s_c != result[res_str_pos]) {
      return false;
    }
    res_str_pos++;
  }

  // Merge in backwards direction until finding a '*' in the suffix
  // This could be sped up by only being done if a '*' was found in the forward 
  // merge
  res_str_pos = result.size() - 1;
  for (auto s_iter = suffix.rbegin(); s_iter != suffix.rend(); s_iter++) {

    // There is a * in the suffix => Skip it and hope that some other suffix 
    // has a proper character for this position in the reconstructed string
    if (*s_iter == '*') {
      break;
    }
    // There is a * in the reconstructed string => replace it with the suffix 
    // character
    else if (result[res_str_pos] == '*') {
      result[res_str_pos] = *s_iter;
    }

    // The character of the substring differs from the character of the 
    // reconstructed string, and neither is a wildcard * =>
    // Conflict! Cannot reconstruct the string!
    else if (*s_iter != result[res_str_pos]) {
      return false;
    }
    res_str_pos--;
  }


  // The suffix was succesfully merged with the reconstructed string
  return true;
}

string solve(vector<pair<int, string>> &suffixes, int string_length) {
  // Initialize the reconstructed string as asterisks *
  string result (string_length, '*');

  // Go through every suffix and "merge" it with the reconstructed string
  // I.e. reconstruct the string
  for (pair<int, string> s : suffixes) {
    int pos = s.first;
    string suffix = s.second;
    // Merge the suffix and the reconstructed string if possible
    // If this is not possible, i.e. they have differing non-* characters 
    // at some position, then the reconstruction is impossible
    if (!lock_it_in(result, suffix, pos)) {
      return "IMPOSSIBLE";
    }
  }

  // There is at least one * left in the reconstructed string after having 
  // gone through all suffixes => no solution
  if (result.find_first_of('*') != result.npos) {
    return "IMPOSSIBLE";
  }

  // There is a unique solution
  return result;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  int cases, length, suffixes, pos;
  string in;

  cin >> cases;
  while (cases-- > 0) {
    cin >> length >> suffixes;
    vector<pair<int, string>> suffix_vec;

    for (int i = 0; i < suffixes; ++i) {
     cin >> pos >> in; 
     suffix_vec.push_back({pos-1, in});
    }

    cout << solve(suffix_vec, length) << "\n";
  }

  cout.flush();
}
