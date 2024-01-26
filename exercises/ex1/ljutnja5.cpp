#include <algorithm>
#include <bits/stdc++.h>
#include <cmath>
#include <cstdint>
#include <ios>

using namespace std;
typedef uint64_t u64;

struct WishRange {
	u64 wish_value;
	u64 start;
	u64 end;
};

u64 anger(u64 candy, u64 children, u64 *wishes) {
	vector<WishRange> wish_ranges;
	WishRange curr_wish_range = WishRange();	
	curr_wish_range.wish_value = wishes[0];
	curr_wish_range.start = 0;
	curr_wish_range.end = children - 1;
	for (int i = 0; i < children; i++) {
		if (wishes[i] > curr_wish_range.wish_value) {
			WishRange new_range = WishRange();
			new_range.start = i;
			new_range.end = children - 1;
			new_range.wish_value = wishes[i];

			curr_wish_range.end = i-1;
			wish_ranges.push_back(curr_wish_range);
			curr_wish_range = new_range;
		}	
	}
	wish_ranges.push_back(curr_wish_range);

	for (int i = wish_ranges.size()-1; i >= 0; i--) {
		WishRange &curr_wish_range = wish_ranges[i];
		u64 num_wishes = curr_wish_range.end - curr_wish_range.start + 1;
		u64 to_decrease_with;
		if (i > 0) {
			to_decrease_with = curr_wish_range.wish_value - wish_ranges[i-1].wish_value;
		}
		else {
			to_decrease_with = curr_wish_range.wish_value;
		}

		// It is possible to give every child in this wish range enough 
		// candy so that their remaining unfulfillment matches that of 
		// the next range of children
		if (candy >= to_decrease_with * num_wishes) {
			// Merge with next wish range
			if (i > 0) {
				wish_ranges[i-1].end = curr_wish_range.end;
				wish_ranges.pop_back();
				candy -= to_decrease_with * num_wishes;
			}
			else {
				// We have managed to satisfy all children!!
				return 0;
			}
		}
		else {
			// We cannot lower this range of wishes as much as we want
			// so instead we lower it as much as is possible with all 
			// of the remaining candy
			to_decrease_with = candy / num_wishes;
			u64 remaining = candy - num_wishes * to_decrease_with;
			curr_wish_range.wish_value -= to_decrease_with;

			WishRange new_range;
			new_range.wish_value = curr_wish_range.wish_value - 1;
			new_range.start = curr_wish_range.start;
			new_range.end = curr_wish_range.start+remaining-1;
			curr_wish_range.start += remaining;
			candy = 0;
			wish_ranges.insert(wish_ranges.begin()+i, new_range);
			break;
			//anger += (num_wishes-remaining)*pow(curr_wish_range.wish_value, 2) + remaining * pow(curr_wish_range.wish_value-1,2);
		}
	}
	u64 anger = 0;
	for (int i = 0; i < wish_ranges.size(); i++) {
		WishRange w = wish_ranges[i];
		u64 num_wishes = w.end - w.start + 1;
		anger += num_wishes * w.wish_value * w.wish_value;	
	}
	return anger;
}

int main() {
	cin.tie(NULL);
	ios::sync_with_stdio(false);
	u64 N, M;
	while (true) {
		cin >> M >> N;
		if (cin.eof()) {
			break;
		}
		u64 wishes[N];

		for (int i = 0; i < N; i++) {
			u64 in;
			cin >> in;
			wishes[i] = in;
		}
		sort(wishes, wishes+N);
		cout << anger(M, N, wishes) << "\n";
	}
	cout.flush();
}
