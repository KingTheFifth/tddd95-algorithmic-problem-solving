#include <iostream>
#include <ios>
#include <string>
#include <unordered_map>

using namespace std;

int solve_it(string word) {
  // Record the positions in the word of each letter present in the word
  unordered_map<char, int> letter_count;
  for (int i = 0; i < word.length(); ++i) {
    char c = word[i];
    if (letter_count.find(c) == letter_count.end()) {
      letter_count[c] = 0;
    }
    letter_count[c]++;
  }

  // Check that the word can be transformed into a palindrome
  // This means that for every letter of the alphabet that is present in the 
  // word, there must be an even amount of the letter in the word to form pairs
  // There may be at most one letter with an odd amount of instances in the word
  int odd_letters = 0;
  char odd_letter;
  for (auto c = letter_count.begin(); c != letter_count.end(); c++) {
    if (c->second % 2 == 1) {
      odd_letters++;
      odd_letter = c->first;

      if (odd_letters > 1) {
        return -1;
      }
    }
  }

  // At this point it is guaranteed to be possible to transform the word into 
  // a palindrome
  // Do this greedily and record the number of letter swaps performed
  int swaps = 0;
  while (word.size() > 1) {
    char to_move = word[0];
    int pos = word.find_last_of(to_move);
    if (to_move == odd_letter && pos == 0) {
      int pos = word.size() / 2;
      if (word[pos] != to_move) {
        swaps += word.size() - 1 - pos;
      }
      word.erase(0, 1);
    }
    else {
      if (word.back() != to_move) {
        swaps += word.size() - 1 - pos;
      }
      word.erase(pos, 1);
      word.erase(0, 1);
    }
  }
  return swaps;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);
  int cases;
  cin >> cases;

  string in;
  while (cases-- > 0) {
    cin >> in;
    int swaps = solve_it(in);
    if (swaps == -1) {
      cout << "Impossible\n";
    }
    else {
      cout << swaps << "\n";
    }
  }
  cout.flush();
}
