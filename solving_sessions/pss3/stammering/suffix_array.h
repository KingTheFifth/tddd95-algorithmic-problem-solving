#pragma once
#include <algorithm>
#include <string>
#include <vector>

using namespace std;
int ALPHABET = 256;

class SuffixArray {
  vector<int> p;
  vector<int> lcp;

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
      // sort, 
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

  void construct_suffix_array(string &s) {
    s += '\0';
    this->sort_cyclic_shifts(s);
    this->p.erase(this->p.begin());
  }

  /**
   * Assumes that construct_suffix_array() has been called on the given string s 
   * before this method is called.
   */
  void construct_lcp_array(string &s) {
    int n = s.size();
    vector<int> rank(n, 0);
    for (int i = 0; i < n; ++i) {
      rank[this->p[i]] = i;
    }

    int k = 0;
    this->lcp = vector<int>(n-1, 0);
    for (int i = 0; i < n; ++i) {
      if(rank[i] == n-1) {
        k=0;
        continue;
      }
      int j = this->p[rank[i] + 1];
      while (i+k<n && j+k<n && s[i+k] == s[j+k]) {
        k++;
      }
      this->lcp[rank[i]] = k;
      k = max(0, k-1);
    }
  }

public:
  SuffixArray(string &s) {
    construct_suffix_array(s);
    construct_lcp_array(s);
  }

  int get_suffix(int i) {
    return this->p[i];
  }

  vector<int> get_lcp() {
    return this->lcp;
  }
};
