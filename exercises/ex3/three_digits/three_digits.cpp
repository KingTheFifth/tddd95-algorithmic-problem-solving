#include <cstdint>
#include <iostream>
#include <ios>
#include <string>

using u64 = uint64_t;
using namespace std;

u64 remove_trailing_zeros(u64 number) {
	while (number % 10 == 0) {
		number = number / 10;
	}
	return number;
}

u64 three_digits(u64 number) {
	u64 res = 1;
	for(u64 n = 1; n <= number; n++) {
		res *= n;
		res = remove_trailing_zeros(res);
		res = res % 1000000000000;
	}
	res = remove_trailing_zeros(res);
	return res;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);

	u64 in;
	while (true) {
		cin >> in;
		if (cin.eof()) {
			break;
		}
		u64 res = three_digits(in);
		string s = to_string(res);
		if (s.size() >= 3) {
			for (int i = 2; i >= 0; i--) {
				cout << s[s.size()-1-i];
			}
		}
		else {
			for (int i = s.size()-1; i >= 0; i--) {
				cout << s[s.size() - 1 - i];
			}
		}
		cout << "\n";
	}
	cout.flush();	
}
