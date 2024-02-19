#include <iostream>
#include <ios>
#include <vector>

using namespace std;

struct State {
	int value;
	// Used to keep track of which state (subproblem) had the greatest value 
	// and was chosen as part of the solution for this state
	State* prev_state = nullptr;

	State(int value) {
		value = value;
	}

	State() {
		value = 0;
	}
};

vector<int> knapsack(int capacity, int *values, int *weights, int n) {
	vector<vector<State>> S;
	vector<int> included_items = vector<int>();

	// Build the table S of states in a bottom-up fashion, row for row
	for (int i = 0; i <= n; i++) {	
		// Initialize row i, e.i. S[i]
		S.push_back(vector<State>(capacity+1));
		for (int c = 0; c <= capacity; c++) {

			// OPT(0, c) = 0, OPT(j, 0) = 0
			// Having no capacity or no items means that no items are included 
			// in the knapsack, hence a total value of 0
			if (i == 0 || c == 0) {
				S[i][c].value = 0;
			}
			
			// Special case: the i:th item is too heavy to include in a knapsack 
			// of capacity c => the optimal such knapsack is one that 
			// does not include the item, i.e. subproblem OPT(i-1, c)
			else if (weights[i-1] > c) {
				S[i][c].value = S[i-1][c].value;
				S[i][c].prev_state = &(S[i-1][c]);
			}
			else {
				// Subproblem structure:
				// OPT(j, C) = min(OPT(j-1, C - w_j) + v_j, OPT(j-1, C))
				int value_include_i = S[i-1][c-weights[i-1]].value+values[i-1];
				int value_exclude_i = S[i-1][c].value;
				// Case: including the i:th item gives a better value than 
				// excluding it
				if (value_include_i > value_exclude_i) {
					S[i][c].value = value_include_i;
					S[i][c].prev_state = &(S[i-1][c-weights[i-1]]);

				}
				// Case: excluding the i:th item is better than including it
				else {
					S[i][c].value = value_exclude_i;
					S[i][c].prev_state = &(S[i-1][c]);
				}
			}
		}
	}

	// Follow the chain of states starting from S[n][capacity]
	// The i:th item was included in the solution if the value of its state 
	// is greater than the value of the state before it (for the subproblem i-1)
	// (the values for all items are strictly greater than 0)
	State* curr_state = &(S[n][capacity]);
	int i = n;
	while (i > 0 && curr_state->prev_state != nullptr) {
		State* prev_state = curr_state->prev_state;
		if (curr_state->value > prev_state->value) {
			// The i:th item has index i-1 
			included_items.push_back(i-1);	
		}
		curr_state = prev_state;
		i--;
	}

	return included_items;
}

int main() {
	int c, n, w, v;
	while(true) {
		cin >> c >> n;
		if(cin.eof()) {
			return 0;
		}

		int values[n];
		int weights[n];
		for (int i = 0; i < n; i++) {
			cin >> v >> w;	
			values[i] = v;
			weights[i] = w;
		}

		vector<int> solution = knapsack(c, values, weights, n);
		cout << solution.size() << "\n";
		for (int i = 0; i < solution.size(); i++) {
			cout << solution[i];
			if (i < solution.size()-1) {
				cout << " ";
			}
		}
		cout << "\n";
	}
	cout.flush();
}
