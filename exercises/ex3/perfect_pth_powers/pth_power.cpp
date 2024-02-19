#include <cmath>
#include <iostream>
#include <ios>

using namespace std;

int pth_perfect_power(long double x) {
	// Negative numbers are easier to handle as positive numbers but with the 
	// special restriction that p must be odd (or else x would be positive)
	long double pos_x;
	if (x < 0) {
		pos_x = abs(x);
	}
	else {
		pos_x = x;
	}


	// Calculate the p:th root of x and see if it is an integer
	// Start from the highest possible p and decrease until finding an integer root
	// The highest possible root is given from the smallest possible base which
	// is 2 => highest possible p = log2(x) cast to an int
	long double largest_possible_p = (int) floor(log2(pos_x));
	for (long long p = largest_possible_p; p >= 1; p--) {
		long double pth_root = pow(pos_x, 1.0/p);

		if (pow(round(pth_root), p) == pos_x) {
			if (x > 0 || (x < 0 && p % 2 != 0)) {
				return p;
			}
		}
	}
	return 1;
}

int main() {
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	long double x;
	while (true) {
		cin >> x;
		if (x == 0 || cin.eof()) {
			break;
		}
		cout << pth_perfect_power(x) << "\n";
	}
	cout.flush();
}
