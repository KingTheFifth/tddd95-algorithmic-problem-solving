#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>
#include <sys/types.h>
#include <vector>
#include <bits/stdc++.h>
#include <ios>

using namespace std;

uint64_t solve(long N, long M, long *wishes, vector<vector<uint64_t>> &dp) {
	if (dp[N][M] != -1) {
		return dp[N][M];
	}

	if (N == 0) {
		dp[N][0] = 0;
		return 0;
	}

	if (M == 0) {
		dp[N][M] = solve(N-1, M, wishes, dp) + wishes[N-1] * wishes[N-1];
		return dp[N][M];
	}

	uint64_t min_anger = numeric_limits<uint64_t>::max();
	// for (int m = 0; m <= min(M, wishes[N-1]); m++) {
	for (int m = min(M, wishes[N-1]); m >= 0; m--) {
		long remaining_candy = M-m;
		long remainder_of_wish = wishes[N-1] - m;
		uint64_t anger = solve(N-1, remaining_candy, wishes, dp)+pow(remainder_of_wish, 2);
		min_anger = min(min_anger, anger);
		if (min_anger == 0) {
			break;
		}
	}
	dp[N][M] = min_anger;
	return min_anger;
}

int main() {
	cin.tie(NULL);
	ios::sync_with_stdio(false);
	long N, M;
	while (true) {
		cin >> M >> N;
		if (cin.eof()) {
			break;
		}
		long wishes[N];
		vector<vector<uint64_t>> dp{};

		// Initialise the 2D vector of cached answers
		// We let -1 symbolise that the answer has not been computed yet
		for (int n = 0; n <= N; n++) {
			dp.push_back(vector<uint64_t>());
			for (int m = 0; m <= M; m++) {
				//if (n == 0) {
				//	dp[n].push_back(0);
				//}
				//else {
					dp[n].push_back(-1);
				//}
			}
		}
		sort(wishes, wishes+N);
		for (int i = 0; i < N; i++) {
			long long in;
			cin >> in;
			wishes[i] = in;
		}
		cout << solve(N, M, wishes, dp) << "\n";
	}
	cout.flush();
}
