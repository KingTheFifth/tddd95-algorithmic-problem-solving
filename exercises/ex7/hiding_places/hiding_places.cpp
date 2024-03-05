#include <algorithm>
#include <iostream>
#include <ios>
#include <vector>
#include <string>
#include <deque>

using namespace std;
using board_vec = vector<vector<pair<int, bool>>>;

string COLUMN_LETTERS = "abcdefgh";

vector<pair<int, int>> OFFSETS = {{-2, -1}, {-1, -2}, {2, -1}, {1, -2}, 
  {-2, 1}, {-1, 2}, {2, 1}, {1, 2}};

string chess_position_from(int row, int col) {
  return COLUMN_LETTERS[col] + to_string(row+1);
}

pair<int, int> chess_position_to_int(string pos) {
  return {stoi(pos.substr(1,1))-1, COLUMN_LETTERS.find_first_of(pos[0])};
}

bool within_board(int row, int col) {
  return 0 <= row && row < 8 && 0 <= col && col < 8;
}

pair<int, vector<string>> solve(string start_pos) {

  // Initialise board as 2D vector with pairs containing distance & bool visited
  board_vec board = board_vec(8);
  for (int r = 0; r < 8; r++) {
    board[r].resize(8);
  }

  int longest_dist = 0;
  pair<int, int> start = chess_position_to_int(start_pos);
  deque<tuple<int, int, int>> to_visit = {{start.first, start.second, 0}};
  board[start.first][start.second].second = true;
  while (!to_visit.empty()) {
    tuple<int, int, int> curr = to_visit.front();
    to_visit.pop_front();

    int row = get<0>(curr);
    int col = get<1>(curr);
    int dist = get<2>(curr);

    for (pair<int, int> &offset : OFFSETS) {
      int new_r = row + offset.first;
      int new_c = col + offset.second;
      int new_d = dist + 1;
      if (within_board(new_r, new_c) && !board[new_r][new_c].second) {
        board[new_r][new_c].first = new_d;
        board[new_r][new_c].second = true;
        to_visit.push_back({new_r, new_c, new_d});
        longest_dist = max(longest_dist, new_d);
      }
    }
  }

  vector<string> result;
  for (int r = 7; r >= 0; r--) {
    for (int c = 0; c < 8; c++) {
      if (board[r][c].first == longest_dist) {
        result.push_back(chess_position_from(r, c));
      }
    }
  }
  return {longest_dist, result};
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);
  int cases;
  string in_pos;
  cin >> cases;
  while (cases > 0) {
    cin >> in_pos;
    pair<int, vector<string>> result = solve(in_pos);
    cout << result.first << " ";
    for (int i = 0; i < result.second.size(); i++) {
     cout << result.second[i];
     if (i < result.second.size()-1) {
      cout << " ";
     }
    }
    cout << "\n";
   cases--; 
  }
  cout.flush();
}
