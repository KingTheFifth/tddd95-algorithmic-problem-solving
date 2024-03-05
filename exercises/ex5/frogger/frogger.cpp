#include <deque>
#include <iostream>
#include <ios>
#include <set>
#include <tuple>
#include <vector>
#include <string>

using namespace std;

bool within_bounds(int width, int height, int x, int y) {
  return x >= 0 && x < width && y >= 0 && y < height;
}

bool car_at(vector<string> &lanes, int x, int y, int t) {
  // Cars in this lane move to the right, which means we have 
  // to look farther to the left in the string
  int x_adjusted;
  if ((y % 2) == (lanes.size() % 2)) {
    x_adjusted = (x-t) % lanes[0].size();
  }
  // Cars move to the left
  else {
    x_adjusted = (x+t) % lanes[0].size();
  }
  // Wrap negatives values around to a positive value
  if (x_adjusted < 0) {
    x_adjusted += lanes[0].size();
  }
  return lanes[y][x_adjusted] == 'X';
}

bool is_viable(vector<string> &lanes, int rounds, int x, int y, int t) {
  return within_bounds(lanes[0].size(), lanes.size(), x, y) &&
    t <= rounds &&
    !car_at(lanes, x, y, t);
}

int solve(vector<string> &lanes, int rounds, int s_x, int s_y, int g_x, int g_y) {
  // deque<tuple<int, int, int>> states;
  set<tuple<int, int, int>> states;
  tuple<int, int, int> curr_state;

  // Initialise with the start state to visit
  states.insert({0, s_x, s_y});
  while (!states.empty()) {
    curr_state = *states.begin();
    states.erase(states.begin());
    int t = get<0>(curr_state);
    int x = get<1>(curr_state);
    int y = get<2>(curr_state);

    // We have arrived at the goal state 
    // Since this is BFS, this will be the shortest path to the goal 
    // => stop and return
    if (x == g_x && y == g_y) {
      return t;
    }

    // Push viable child states of the current state onto the queue of states
    // to search
    if (is_viable(lanes, rounds, x-1, y, t+1)) {
      // states.push_back({x-1, y, t+1});
      states.insert({t+1, x-1, y});
    }
    if (is_viable(lanes, rounds, x, y, t+1)) {
      //states.push_back({x, y, t+1});
      states.insert({t+1, x, y});
    }
    if (is_viable(lanes, rounds, x+1, y, t+1)) {
      // states.push_back({x+1, y, t+1});
      states.insert({t+1, x+1, y});
    }
    if (is_viable(lanes, rounds, x, y-1, t+1)) {
      // states.push_back({x, y-1, t+1});
      states.insert({t+1, x, y-1});
    }
    if (is_viable(lanes, rounds, x, y+1, t+1)) {
      // states.push_back({x, y+1, t+1});
      states.insert({t+1, x, y+1});
    }
  }

  // We ran out of states to visit and did not reach the goal state in the 
  // search => impossible to reach the goal state
  return -1;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  int scenarios, rounds, lanes, width;
  string in;
  cin >> scenarios;
  while (scenarios > 0) {
    cin >> rounds >> lanes >> width;
    if (cin.eof()) {
      break;
    }

    vector<string> lane_vec;
    for (int l = 0; l < lanes+2; l++) {
      cin >> in;
      lane_vec.push_back(in);
    }

    int start_x = lane_vec[lanes+1].find_first_of('F');
    int goal_x = lane_vec[0].find_first_of('G');
    int solution = solve(lane_vec, rounds, start_x, lanes+1, goal_x, 0);
    if (solution == -1) {
      cout << "The problem has no solution.\n";
    }
    else {
      cout << "The minimum number of turns is " << solution << ".\n";
    }
    scenarios--;
  }

  cout.flush();
}
