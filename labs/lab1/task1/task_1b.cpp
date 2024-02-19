#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <ios>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

struct interval {
	double start;
	double end;
	int index;

	interval(double start, double end, int index) {
		this->start = start;
		this->end = end;
		this->index = index;
	}
	interval(double start, double end) {
		this->start = start;
		this->end = end;
		this->index = -1;
	}
};

bool covers(interval &coverer, interval &covered) {
	if (coverer.start <= covered.start && coverer.end >= covered.end) {
		return true;
	}
	return false;
}

double overlap(interval &target, interval &interval) {
	// Edge case: the two intervals overlap completely
	// Treat this as an infinite overlap for practical reasons
	// (There is no better interval that overlaps more)
	if (interval.start == target.start && interval.end == target.end) {
		return interval.start - interval.end;
		//return INFINITY;
	}
	double start = max(target.start, interval.start);
	double end = min(target.end, interval.end);
	return end - start;
}

void sort_intervals(interval &target, vector<interval> *intervals) {
	auto lam = [&target](interval &i1, interval &i2) {
		return overlap(target, i1) > overlap(target, i2);
	};
	sort(intervals->begin(), intervals->end(), lam);
}

/***
 * Assumes i1 and i2 are overlapping
 */
void merge_intervals(interval *i1, interval *i2, interval *result) {
	result->start = min(i1->start, i2->start);
	result->end = max(i1->end, i2->end);
}

bool solve(interval *target, vector<interval> *intervals, vector<int> *used) {
	sort_intervals(*target, intervals);
	interval cover = intervals->front();
	used->push_back(cover.index);
	for (interval i : *intervals) {
		if (covers(cover, *target)) {
			return true;
		}
		// If the most overlapping interval does not overlap with the target,
		// then it is impossible to solve
		if (overlap(*target, i) < 0) {
			return false;	
		}

		// If the blob already covers this interval, then it should not merge 
		// with the blob
		if (covers(cover, i)) {
			continue;
		}

		if (overlap(cover, i) < 0) {
			return false;
		}

		// If this interval would extend the blob, merge it with the blob
		merge_intervals(&cover, &i, &cover);
		used->push_back(i.index);
	}
	if (covers(cover, *target)) {
		return true;
	}
	// The blob does not cover the target interval but there are no more 
	// intervals to extend the blob with
	return false;
}

int main() {
	//cin.tie(NULL);
	//ios::sync_with_stdio(false);
	double A, B;
	int n;
	while(true) {
		cin >> A >> B >> n;
		if(cin.eof()) {
			break;
		}
		interval target = interval(A, B);
		vector<interval> intervals = vector<interval>();
		vector<int> used = vector<int>();
		//unordered_map<pair<double, double>, int> interval_index =  unordered_map<pair<double, double>, int>();
		//vector<int> used = vector<int>();
		//cout << "A: " << A << " B: " << B << " n: " << n << "\n";
		int index = 0;
		while (index < n) {
			double a, b;
			cin >> a >> b;
			intervals.push_back(interval(a, b, index));
			index++;
			//cout << "[" << a << ", " << b <<"]\n";
		}
		//cout << "time to solve\n";
		if (solve(&target, &intervals, &used)) {
			cout << used.size() << "\n";
			for (int i = 0; i < used.size(); i++) {
				cout << used[i];
				if (i < used.size()-1) {
					cout << " ";
				}
			}
			cout << "\n";
		}
		else {
			cout << "impossible\n";
		}
	}
	cout.flush();
}
