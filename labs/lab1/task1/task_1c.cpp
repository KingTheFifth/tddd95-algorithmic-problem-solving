#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <ios>
#include <limits>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

struct Interval {
	double start;
	double end;
	int index;

	Interval(double start, double end, int index) {
		this.start = start;
		this.end = end;
		this.index = index;
	}
	Interval(double start, double end) {
		this.start = start;
		this.end = end;
		this.index = -1;
	}

	bool operator==(const Interval &other) {
		return start == other.start && end == other.end;
	}

	bool operator!=(const Interval &other) {
		return start != other.start || end != other.end;
	}
};

void sort_intervals(vector<Interval> &intervals) {
	auto lam = [](Interval &a, Interval &b) {
		return a.start < b.start;
	};
	sort(intervals.begin(), intervals.end(), lam);
}

void merge_intervals(Interval &i1, Interval &i2, Interval &result) {
	result.start = min(i1.start, i2.start);
	result.end = max(i1.end, i2.end);
}

bool covers(Interval &coverer, Interval &covered) {
	if (coverer.start <= covered.start && coverer.end >= covered.end) {
		return true;
	}
	return false;
}

bool solve(Interval &target, vector<Interval> &intervals, vector<int> &used) {
	// This should not happen per the problem description
	// (There is always at least 1 interval available)
	if (intervals.empty()) {
		return false;
	}

	// Sort the intervals based on their start values in increasing order
	sort_intervals(intervals);
	double neg_inf = -numeric_limits<double>::infinity();

	// Start building an interval that will we will extend until it covers 
	// the target interval 
	// This "cover" interval is built using the intervals at hand
	Interval cover = Interval(target.start, target.start);
	Interval best_choice = Interval(neg_inf, neg_inf);
	if(target.start < intervals.at(0).start) {
		return false;
	}
	for (Interval i : intervals) {
		// The current interval overlaps with the cover and could potentially
		// be the best to extend with
		if (i.start <= cover.end) {
			// If this interval would extend our current cover farther to 
			// the right than our current best candidate, then this interval 
			// is clearly the better candidate to extend with
			if (i.end >= best_choice.end) {
				best_choice = i;
			}
			// It is not better to extend our currect cover interval with 
			// this interval so this one can be safely skipped 
		}

		// We have looked through all the intervals that overlap with 
		// the current cover and thus know the best such interval to extend 
		// the cover with to the right
		// => extend the cover with the best choice and then start the 
		// process anew with the current interval as the best choice
		// (given that it overlaps with the cover after extending the cover)
		else {
			merge_intervals(cover, best_choice, cover);
			used.push_back(best_choice.index);
			// Stop trying to extend the cover as soon as it covers the target
			if (covers(cover, target)) {
				return true;
			}

			// If the current interval does not overlap with the cover,
			// then none of the following intervals will either as they 
			// start on the same number or a greater one
			// => It is not possible to cover the target
			if (i.start > cover.end) {
				break;
			}
			best_choice = i;
		}

	}
	if (best_choice != Interval(neg_inf, neg_inf)) {
		merge_intervals(cover, best_choice, cover);
		used.push_back(best_choice.index);
	}
	//if (best_choice == intervals.back()) {
	//	if (used.empty() || used.back() != best_choice.index) {
	//		merge_intervals(&cover, &best_choice, &cover);
	//	}
	//}
	if (used.empty()) {
		return false;
	}
	if (covers(cover, target)) {
		return true;
	}
	return false;
}

int main() {
	cin.tie(NULL);
	ios::sync_with_stdio(false);
	double A, B;
	int n;
	while(true) {
		cin >> A >> B >> n;
		if(cin.eof()) {
			break;
		}
		Interval target = Interval(A, B);
		vector<Interval> intervals = vector<Interval>();
		vector<int> used = vector<int>();
		int index = 0;
		while (index < n) {
			double a, b;
			cin >> a >> b;
			intervals.push_back(Interval(a, b, index));
			index++;
		}
		if (solve(target, intervals, used)) {
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
