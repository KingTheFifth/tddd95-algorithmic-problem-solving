/**
 * Johannes Kung johku144
 * Finding a pattern in a string using the Knuth-Morris-Pratt algorithm.
 *
 * Time complexity: O(text + pattern)
 */
#include <iostream>
#include <ios>
#include <string>
#include <vector>

using namespace std;

/**
 * Implementation of the string prefix function for Knuth-Morris-Pratt algorithm
 * as done by CP-Algorithms (https://cp-algorithms.com/string/prefix-function.html).
 *
 * Time complexity: O(n) where n is the length of the given string s.
 */
vector<int> prefix_function(string s) {
  vector<int> pi(s.size());
  // Calculate pi(i) for each i = 1,..., |s|-1
  // (pi(0) = 0)
  for (int i = 1; i < s.size(); i++) {
    // Save the length of the best prefix that is also a suffix of the substring 
    // s[0...i-1]
    int j = pi[i-1];

    // Reduce j to pi(j-1) until s[i] == s[j] or j == 0
    while (j > 0 && s[i] != s[j]) {
      j = pi[j-1];
    }

    // If s[i] == s[j], then the prefix of length j+1 is a suffix of the 
    // substring s[0...i] i.e. we have found the longest prefix
    // Else j == 0 and there is no prefix of s[0...i] that is also a suffix
    if (s[i] == s[j]) {
      j++;
    }

    pi[i] = j;
  }

  return pi;
}

/**
 * Implementation of the Knuth-Morris-Pratt algorithm to find occurences of 
 * a pattern in a string.
 *
 * Time complexity: O(pattern + phrase)
 */
vector<int> find(string pattern, string phrase) {
  // Add the pattern onto the beginning of the phrase, with a delimiting character
  // in between.
  //
  // The prefix function returns the length of the longest prefix of each substring
  // phrase[0...i] such that the prefix is also a suffix of the substring. If 
  // this length is the same as the length of the pattern, then this prefix 
  // is exactly the pattern since it was appended to the beginning of the phrase.
  //
  // This tells us that the pattern can be found at position i - the lenght of 
  // the pattern in the modified phrase, ending at position i.
  //
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
