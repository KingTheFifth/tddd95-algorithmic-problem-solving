#include <bits/stdc++.h>
#include <cmath>
#include <cstdint>
#include <ios>

using namespace std;

struct WishRange {
	long wish_value;
	int start;
	int end;
	WishRange* next = nullptr;
};

uint64_t anger(long candy, long children, long *wishes) {
	WishRange* curr_wish_range = new WishRange();	
	WishRange* start_range = curr_wish_range;
	curr_wish_range->wish_value = wishes[0];
	curr_wish_range->start = 0;
	curr_wish_range->end = children - 1;
	for (int i = 0; i < children; i++) {
		if (wishes[i] < curr_wish_range->wish_value) {
			WishRange* new_range = new WishRange();
			new_range->start = i;
			new_range->end = children - 1;
			new_range->wish_value = wishes[i];

			curr_wish_range->end = i-1;
			curr_wish_range->next = new_range;
			curr_wish_range = new_range;
		}	
	}

	WishRange *curr = start_range;
	while (true) {
		if (candy == 0) {
			break;
		}
		int wishes = curr->end - curr->start + 1;	
		long reduce_amount;
		long to_distribute;
		// Calculate how much each wish in the wish range should be decreased
		// by in order to make it equal to the next wish range
		if (curr->next != nullptr) {
			reduce_amount = curr->wish_value - curr->next->wish_value;
		}
		else {
			// There is no next range so we want to make this one 
			// reduced to 0
			reduce_amount = curr->wish_value;
		}

		// If we can manage to reduce this range by the desired amount
		// (there is enough candy)
		if (reduce_amount * wishes <= candy) {
			curr->wish_value -= reduce_amount;
			candy -= reduce_amount * wishes;
			// Merge this range with the next one
			if (curr->next != nullptr) {
				WishRange* to_remove = curr->next;
				curr->end = curr->next->end;	
				curr->next = curr->next->next;
				delete to_remove;
			}
			else {
				// This was the only range of wishes and we managed to 
				// satisfy every wish => 0 total anger
				return 0;
			}
		}
		// There is not enough candy to reduce all wishes in the current range 
		// to the value of the wishes of the next range (or to 0 if no next range)
		else {
			reduce_amount = floor(candy/wishes);
			long remaining = candy - reduce_amount * wishes;
			curr->wish_value -= reduce_amount;
			WishRange* new_range =  new WishRange();
			new_range->start = curr->end - remaining + 1;
			new_range->end = curr->end;
			new_range->wish_value = curr->wish_value - 1;
			new_range->next = curr->next;
			curr->next = new_range;
			curr->end -= remaining;
			candy = 0;
		}
	}
	uint64_t anger = 0;
	while (true) {
		int wishes = curr->end - curr->start + 1;	
		anger += wishes * pow(curr->wish_value, 2);
		if (curr->next == nullptr) {
			return anger;
		}
		curr = curr->next;
	}
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

		for (int i = 0; i < N; i++) {
			long long in;
			cin >> in;
			wishes[i] = in;
		}
		sort(wishes, wishes+N, greater<>());
		cout << anger(M, N, wishes) << "\n";
	}
	cout.flush();
}
