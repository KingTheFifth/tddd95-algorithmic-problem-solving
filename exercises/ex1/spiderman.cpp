#include <algorithm>
#include <cmath>
#include <iostream>
#include <ios>
#include <limits>
#include <vector>

using namespace std;

struct State {
	int max_height;
	string direction;

	State(int m_h, string d) {
		max_height = m_h;
		direction = d;
	}

	State() {
		max_height = -1;
		direction = "";
	}
};

int solve_rec(vector<int> &distances, int dist_index, int height, vector<vector<State>> &dp) {
	// Impossible to go underground
	if (height < 0) {
		return numeric_limits<int>::max();		
	}
	// At the start of the exercise: we can only go up!
	if (dist_index == 0) {
		State &curr_state = dp[0][height];
		if (height - distances[0] == 0) {
			curr_state.max_height = distances[0];
			curr_state.direction.append("U");
		}
		else {
			curr_state.max_height = numeric_limits<int>::max();
			curr_state.direction = "IMPOSSIBLE";
		}
		return curr_state.max_height;
	}
	if (dp[dist_index][height].max_height != -1) {
		return dp[dist_index][height].max_height;
	}
	int curr_dist = distances[dist_index];
	int max_height;
	string direction = "";
	int down = solve_rec(distances, dist_index - 1, height+curr_dist, dp);
	int up = solve_rec(distances, dist_index-1, height-curr_dist, dp);
	int max_height_up = max(up, height);
	int max_height_down = down;
	if (max_height_up < max_height_down) {
		State &up_state = dp[dist_index-1][]
		max_height = max_height_up;
		direction.append(up.direction);
		direction.append("U");
	}
	else {
		max_height = max_height_down;
		direction.append(down.direction);
		direction.append("D");
	}
	dp[dist_index][height] = State(max_height, direction);	
	return dp[dist_index][height];
}

int main() {
	int test_cases;
	cin >> test_cases;
	while (test_cases > 0) {
		int num_dists, dist;
		cin >> num_dists;

		vector<int> distances;
		int dist_sum = 0;
		while (num_dists > 0) {
			cin >> dist;
			distances.push_back(dist);
			dist_sum += dist;
			num_dists--;
		}
	vector<vector<State>> dp;
	for (int i = 0; i < distances.size(); i++) {
		dp.push_back(vector<State>(dist_sum));
	}
	State solution = solve_rec(distances, distances.size()-1, 0, dp);
	cout << solution.direction << "\n";
	test_cases--;
	}
	cout.flush();
}
