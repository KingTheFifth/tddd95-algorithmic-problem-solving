#include <bits/stdc++.h>
#include <cmath>
#include <ios>

using namespace std;

struct FenwickTree {
	vector<int> bit;
	int length;

	FenwickTree(int length) {
		this->length = length;
		bit.assign(length, 0);
	}

	int sum(int i) {
		int res = 0;
		while (i >= 0) {
			res += bit[i];
			i = (i & (i+1)) - 1;
		}
		return res;
	}

	void update(int i, int delta) {
		while (i < length) {
			bit[i] += delta;
			i = i | (i+1);
		}
	}
};

vector<int> solve(int N, unordered_map<int, int> &num_indices) {
	vector<int> swaps;
	int smallest = 1;
	int greatest = N;
	FenwickTree l_adjustment = FenwickTree(N);
	FenwickTree r_adjustment = FenwickTree(N);
	bool to_the_right = false;
	while (smallest <= greatest) {
		if (to_the_right) {
			int num = greatest;
			int i = num_indices[greatest];
			int i_adjusted = i - l_adjustment.sum(i) + r_adjustment.sum(N-1-i);
			int s = abs(num-1-i_adjusted);
			swaps.push_back(s);
			if (s > 0) {
				l_adjustment.update(i, 1);
			}
			to_the_right = false;
			greatest--;
		}
		else {
			int num = smallest;
			int i = num_indices[num];
			int i_adjusted = i - l_adjustment.sum(i) + r_adjustment.sum(N-1-i);
			int s = abs(num-1-i_adjusted);
			swaps.push_back(s);
			if (s > 0) {
				r_adjustment.update(N-1-i, 1);
			}
			to_the_right = true;
			smallest++;
		}
	}
	return swaps;
}

int main() {
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int N, in;
	while(true) {
		cin >> N;
		if (cin.eof()) {
			break;
		}

		vector<int> numbers;
		unordered_map<int, int> num_to_index;
		int i = 0;
		while (i < N) {
			cin >> in;
			num_to_index.insert({in, i});
			i++;
		}
		vector<int> swaps = solve(N, num_to_index);
		for (int s : swaps) {
			cout << s << "\n";
		}
	}
	cout.flush();
}
