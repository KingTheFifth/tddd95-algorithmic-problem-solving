#include <cstdint>
#include <iostream>
#include <ios>
#include <vector>

using namespace std;

int solve(int digits, int sum) {
	vector<vector<int>> dp;
	for (int d = 1; d <= digits; d++) {
		dp.push_back(vector<int>(sum+1));
		for (int s = 0; s <= sum; s++) {
			if (d == 1 && s < 10) {
				dp[d-1][s] = 1;
			}
			else if (d == 1 && s >= 10) {
				dp[d-1][s] = 0;
			}
			else {
				for (int n = 0; n <= s; n++) {
					if (s - n <= 9) {
						dp[d-1][s] = dp[d-1][s] + dp[d-2][n];		
					}
				}
			}
		}
	}
	return dp[digits-1][sum];
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);
	int64_t A, B, S;
	while (true) {
		cin >> A >> B >> S;
		if (cin.eof()) {
			break;
		}
		int digits = to_string(B).length(); 
		cout << solve(digits, S) << "\n";
	}
	cout.flush();
}
