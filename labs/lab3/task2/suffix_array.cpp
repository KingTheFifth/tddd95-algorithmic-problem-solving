/**
 * Johannes Kung johku144
 * Implementation of suffix array based on the implementation by CP-Algorithms.
 * (https://cp-algorithms.com/string/suffix-array.html#finding-a-substring-in-a-string)
 * A suffix array contains the order of a lexicographic sort on all suffixes 
 * of a given string. 
 *
 * Time complexities:
 *  - Suffix array construction: O(n*log(n)) 
 *  - Querying the array: O(1)
 */
#include <algorithm>
#include <iostream>
#include <ios>
#include <string>
#include <vector>

using namespace std;
int ALPHABET = 256;

/**
 * Class for constructing the suffix array for a given string.
 * Constructing an instance is O(n*log(n)) while querying is O(1). n is the 
 * length of the string for which the suffix array was constructed.
 */
class SuffixArray {
  vector<int> p;

  /**
   * Given a string s, sorts all cyclic shifts of s. The resulting sorted ordering
   * is saved in the private vector p.
   *
   * Time complexity: O(n*log(n)) where n is the length of s.
   */
  void sort_cyclic_shifts(string &s) {
    int n = s.size();
    this->p = vector<int>(n);
    vector<int> c(n), cnt(max(ALPHABET, n), 0);
  
    // Iteration 0:
    // Perform a counting sort on cyclic substrings of length 1
    for (int i = 0; i < n; ++i) {
      cnt[s[i]]++;
    }
    for (int i = 1; i < ALPHABET; ++i) {
      cnt[i] += cnt[i-1];
    }
    for (int i = 0; i < n; ++i) {
      this->p[--cnt[s[i]]] = i;
    }

    // Construct equivalent classes: all substrings that are the same 
    // end up in the same class. Class indices also preserve the sorted order 
    // of the substrings
    c[this->p[0]] = 0;
    int classes = 1;
    for (int i = 1; i < n; ++i) {
      if (s[this->p[i]] != s[this->p[i-1]]) {
        classes++;
      }
      c[this->p[i]] = classes - 1;
    }

    // Iteration 1 to log(n):
    vector<int> p_next(n), c_next(n);
    for (int h = 0; (1 << h) < n; ++h) {
      // working on cyclic substrings of length 2^h
      // Such a substring can be split into two halves of length 2^(h-1) each

      // First sort the substrings on their second half
      // From the previous iteration over h, we have the equivalence classes of
      // the substrings of length 2^(h-1) from which we can compare substrings 
      // and also check which one preceeds the other directly
      // => This sorting is therefore O(n)
      for (int i = 0; i < n; ++i) {
        p_next[i] = this->p[i] - (1 << h);
        if (p_next[i] < 0) {
          p_next[i] += n;
        }
      }

      // Secondly, sort the substrings based on the first half using counting 
      // sort 
      // O(n)
      fill(cnt.begin(), cnt.begin() + classes, 0);
      for (int i = 0; i < n; ++i) {
        cnt[c[p_next[i]]]++;
      }
      for (int i = 1; i < classes; ++i) {
        cnt[i] += cnt[i-1];
      }
      for (int i = n-1; i >= 0; --i) {
        this->p[--cnt[c[p_next[i]]]] = p_next[i];
      }

      // Construct equivalence classes
      // O(n)
      c_next[p_next[0]] = 0;
      classes = 1;
      for (int i = 1; i < n; ++i) {
        pair<int, int> cur = {c[this->p[i]], c[(this->p[i] + (1<<h)) % n]};
        pair<int, int> prev = {c[this->p[i-1]], c[(this->p[i-1] + (1<<h)) % n]};
        if(cur != prev) {
          classes++;
        }
        c_next[this->p[i]] = classes - 1;
      }
      c.swap(c_next);
      // Done sorting substrings of length 2^h and constructing equivalence 
      // classes
    }
  }

public:
  SuffixArray(string &s) {
    // Add a character to the end of s that is guaranteed to come first 
    // lexicographically.
    //
    // This character is used as a separator in the cyclic shifts of s.
    // All characters to the right of the separator in a cyclic shift correspond
    // to a suffix of s
    //
    // Sorting the cyclic shifts of s with the separator character is then 
    // equivalent to sorting the suffixes of s. Note that the suffix of length 1,
    // corresponding to the separator character, will be first in this ordering 
    // and should be removed to get the ordering of the suffixes of s.
    s += '\0';
    this->sort_cyclic_shifts(s);
    this->p.erase(this->p.begin());
  }

  // Given an index i for the substring 0...i, returns the index of the 
  // substring in the lexicographic order of the substrings.
  int get_suffix(int i) {
    return this->p[i];
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);
  string in;
  int n, q;
  while (true) {
    getline(cin, in);

    if (cin.eof()) {
      break;
    }

    SuffixArray sa = SuffixArray(in);

    cin >> n;
    string out;
    for (int i = 0; i < n - 1; ++i) {
      cin >> q;
      cout << sa.get_suffix(q) << " ";
    }
    cin >> q;
    cout << sa.get_suffix(q) << "\n";

    // Get rid of a trailing "\n" after reading the line of queries
    //cin.ignore();
    getline(cin, in);
  }

  cout.flush();
  
}
