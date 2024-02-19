#include <cmath>
#include <cstdint>
#include <iostream>
#include <ios>
#include <vector>

using namespace std;
using i64 = int64_t;

struct State {
	vector<int> viable_numbers;
};

bool digitSum(int digits, int lower_bound, int sum, vector<vector<State>> dp) {
	// Look up for cached results, memoization innit
	if (!dp[digits][sum].viable_numbers.empty()) {
		//return dp[digits][sum];
		return true;
	}

	// Cannot make a digit sum with 0 digits :P
	// Speed up
	if (digits == 0 || digits < lower_bound) {
		if (sum == 0) {
			//dp[digits][sum] = 0;
			// return 1;
			dp[digits][sum].viable_numbers.push_back(0);
			return true;
		}
		// dp[digits][sum] = 0;
		// return 0;
		return false;
	}

	// Try to different allowed n:th digits and then solve recursively
	// for the remaining n-1 digits
	//int res = 0;	
	State &res = dp[digits][sum];
	for(int d = 0; d <= 9; d++) {
		if (sum - d >= 0) {
			//res += digitSum(digits-1, lower_bound, sum - d, dp);	
			bool sub_res = digitSum(digits-1, lower_bound, sum-d, dp);
			if (sub_res) {
				for (int num : dp[digits-1][sum-d].viable_numbers) {
					res.viable_numbers.push_back(d * pow(10, digits-1) + num);	
				}	
			}
		}
		// Else, the digit cannot be used as the digit sum would be greater
		// than the sought digit sum
	}
	return res.viable_numbers.empty();
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);
	i64 A, B, S; 
	while (true) {
		cin >> A >> B >> S;
		if (cin.eof()) {
			break;
		}
		int lower_bound = to_string(A).length();
		int digits = to_string(B).length();
		vector<vector<int>> memoization;
		for (int d = 0; d <= digits; d++) {
			memoization.push_back(vector<int>(S+1));
			for (int s = 0; s <= S; s++) {
				memoization[d][s] = -1;
			}
		}
		cout << digitSum(digits, lower_bound, S, memoization) << "\n";
	}

	cout.flush();
}
