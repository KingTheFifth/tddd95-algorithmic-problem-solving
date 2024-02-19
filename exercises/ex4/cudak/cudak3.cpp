#include <cmath>
#include <cstdint>
#include <iostream>
#include <ios>
#include <string>
#include <vector>

using namespace std;
using i64 = int64_t;

i64 nth_digit(i64 digit, i64 number) {
	return number/(i64)pow(10, digit-1) % 10;
}

i64 digit_sum(i64 number) {
	i64 res = 0;
	i64 digits = to_string(number).length();
	for (i64 d = 1; d <= digits; d++) {
		res += nth_digit(d, number);
	}
	return res;
}

i64 nums_with_digit_sum(i64 digits, i64 upper_bound, i64 sum, i64 limited, vector<vector<vector<i64>>> &dp) {
	// Look up for cached results, dp innit
	if (dp[digits][sum][limited] != -1) {
		return dp[digits][sum][limited];
	}

	// Cannot make a digit sum with 0 digits unless the sum should be 0 :P
	// Speed up
	if (digits == 0) {
		if (sum == 0) {
			dp[digits][sum][limited] = 1;
			return 1;
		}
		dp[digits][sum][limited] = 0;
		return 0;
	}

	// Try to different allowed n:th digits and then solve recursively
	// for the remaining n-1 digits
	i64 res = 0;	
	i64 limit = 9;
	// Make sure we do not try numbers that would be greater than the 
	// upper bound, by checking if the "limited" bool is set to 1
	if (limited == 1) {
		// Limited == 1 signifies that the previous tried digits in whatever
		// enclosing recursion calls have been set to the corresponding 
		// digits in the upper bound number
		// Thus, this digit can only be the numbers between 0 to the n:th digit 
		// of the upper bound, lest we shall accidentally consider numbers 
		// greater than the upper bound
		limit = nth_digit(digits, upper_bound);
	}
	for(i64 d = 0; d <= limit; d++) {
		if (sum - d >= 0) {
			if (limited == 1 && d == nth_digit(digits,upper_bound)) {
				// We have come to a poi64 where we have set the digits up 
				// to the n:th digit to the corresponding digits of the upper 
				// bound.
				// Therefore, the digits tried in the subproblem with n-1 digits 
				// must be limited as well
				res += nums_with_digit_sum(digits-1, upper_bound, sum-d, limited, dp);
			}
			else {
				res += nums_with_digit_sum(digits-1, upper_bound, sum-d, 0, dp);
			}
		}
		// Else, the digit cannot be used as the digit sum would be greater
		// than the sought digit sum
	}
	dp[digits][sum][limited] = res;
	return res;
}

i64 solve(i64 upper_bound, i64 sum) {
	i64 digits = to_string(upper_bound).length();
	vector<vector<vector<i64>>> dp;
	for (i64 d = 0; d <= digits; d++) {
		dp.push_back(vector<vector<i64>>(sum+1));
		for (i64 s = 0; s <= sum; s++) {
			dp[d][s].push_back(-1);
			dp[d][s].push_back(-1);
		}
	}
	return nums_with_digit_sum(digits, upper_bound, sum, 1, dp);
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
		cout << solve(B, S) - solve(A-1, S) << "\n";
		// Do binary search to try and find the lowest number with the 
		// sought digit sum, starting from A
		// This corresponds to an upper bound C with only 1 number 
		// lesser than or equal to it with the correct digit sum,
		// where C itself has the correct digit sum
		i64 left = A;
		i64 right = B;
		i64 C;
		while (left <= right) {
			C = (left+right)/2;
			i64 count = solve(C, S) - solve(A-1, S);
			if (count == 1 && digit_sum(C) == S) {
				cout << C << "\n";
				break;
			}
			else if (count >= 1) {
				right = C-1;
			}
			else {
				left = C+1;
			}
		}
	}

	cout.flush();
}
