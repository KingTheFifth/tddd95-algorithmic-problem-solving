#include <algorithm>
#include <bits/stdc++.h>
#include <cmath>
#include <cstdint>
#include <ios>

using namespace std;

uint64_t anger(uint64_t candies, uint64_t children, uint64_t* wishes) {
		uint64_t sum_wishes = 0;
		for (int c = 0; c < children; c++) {
			sum_wishes += wishes[c];	
		}

		uint64_t totalUnfulfillment = sum_wishes - candies;
		uint64_t totalAnger = 0;
		for (int i = 0; i < children; i++) {
			uint64_t remainingChildren = children - i;
			uint64_t distributedUnfulfillment = (uint64_t) floor((double) totalUnfulfillment / remainingChildren);
			// We try to give each child an equal amount of unfulfillment to 
			// minimise anger
			// However, if a child cannot be that unfulfilled (i.e. wants less than 
			// what we are trying to take away), we make the child completely
			// unfulfilled.
			// We also have to redistribute the total unfulfillment 
			// as the rest of the children must now become more unfulfilled 
			// as a result (we could not withhold as much as we expected)
			uint64_t candiesWithheld = min(wishes[i], distributedUnfulfillment);
			totalAnger += pow(candiesWithheld, 2);
			totalUnfulfillment -= candiesWithheld;

		}
		return totalAnger;
}

int main() {
	cin.tie(NULL);
	ios::sync_with_stdio(false);
	uint64_t N, M;
	while (true) {
		cin >> M >> N;
		if (cin.eof()) {
			break;
		}
		uint64_t wishes[N];

		for (int i = 0; i < N; i++) {
			uint64_t in;
			cin >> in;
			wishes[i] = in;
		}
		sort(wishes, wishes+N);
		cout << anger(M, N, wishes) << "\n";
	}
	cout.flush();
}
