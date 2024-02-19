#include <iostream>
#include <ios>
#include <queue>
#include <unordered_map>
#include <vector>

using namespace std;

vector<int> solve(int N, vector<int> &v_col) {
	vector<int> u_col;

	// Optimisation: if the last node in the v column is not the one of the 
	// highest label, then there is no tree that produced the v column
	if (v_col.back() != N+1) {
		return {};
	}

	unordered_map<int, int> degree;
	priority_queue<int, vector<int>, std::greater<int>> leaves;

	// Initialise everything to be leaves
	for (int n = 1; n <= N+1; n++) {
		degree.insert({n, 1});
	}

	// Increase degree of nodes appearing in the v column
	for (int v : v_col) {
		degree[v] += 1;	
	}
	degree[v_col.back()] -= 1;

	// Keep the leaves in a priority queue
	// First element is the leave with the smallest label
	for (int n = 1; n <= N+1; n++) {
		if (degree[n] == 1) {
			leaves.push(n);
		}
	}

	// Chop off leaves from the non-leaves in the order specified by the 
	// v column
	for (int v : v_col) {
		if (leaves.empty()) {
			// We have somehow run out of leaves to chop off but we still 
			// have nodes from which we're supposed to have chopped off leaves 
			// => Error
			return {};
		}

		degree[v] -= 1;
		u_col.push_back(leaves.top());
		leaves.pop();

		// If the node v turned into a leaf by chopping off a leaf from it,
		// keep track of it as a leaf
		if (degree[v] == 1) {
			leaves.push(v);
		}
	}

	return u_col;
}

int main() {
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int N, in;
	while (true) {
		cin >> N;
		if (cin.eof()) {
			break;
		}

		vector<int> v_col;
		for(int i = 0; i < N; i++) {
			cin >> in;
			v_col.push_back(in);
		}

		vector<int> u_col = solve(N, v_col);
		if (u_col.empty()) {
			cout << "Error\n";
		}
		else {
			for (int u : u_col) {
				cout << u << "\n";
			}
		}
	}
	cout.flush();
}
