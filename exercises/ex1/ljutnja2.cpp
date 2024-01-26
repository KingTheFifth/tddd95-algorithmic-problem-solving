#include <bits/stdc++.h>
#include <cmath>
#include <cstdint>
#include <ios>

using namespace std;


uint64_t anger(long candy, long children, long* wishes) {
		long to_distribute = floor(candy/children);
		long remaining = candy - children*to_distribute;

		long sum_wishes = 0;
		for (int c = 0; c < children; c++) {
			sum_wishes += wishes[c];	
		}

		long distributed_wishes[children];
		long distributed_wish = floor(sum_wishes/children);
		long remaining_wish = sum_wishes - children*distributed_wish;
		for (int c = 0; c < children; c++) {
			if (remaining_wish > 0) {
				distributed_wishes[c] = distributed_wish+1;
				remaining_wish--;
			}
			else {
				distributed_wishes[c] = distributed_wish; 
			}
		}
		
		if (to_distribute > 0) {
			for(int c = 0; c < children; c++) {
				distributed_wishes[c] -= to_distribute;
			}
		}	
		for (int c = 0; c < remaining; c++) {
			distributed_wishes[c] -= 1;
		}

		uint64_t sum_anger = 0;
		for (int c = 0; c < children; c++) {
			sum_anger += pow(distributed_wishes[c], 2);
		}
		return sum_anger;
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
		//sort(wishes, wishes+N);

		for (int i = 0; i < N; i++) {
			long long in;
			cin >> in;
			wishes[i] = in;
		}
		cout << anger(M, N, wishes) << "\n";
	}
	cout.flush();
}
