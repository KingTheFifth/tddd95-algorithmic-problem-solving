#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <ios>
#include <limits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

struct Interval {
	double start;
	double end;
	int index;

	Interval(double start, double end, int index) {
		this->start = start;
		this->end = end;
		this->index = index;
	}
	Interval(double start, double end) {
		this->start = start;
		this->end = end;
		this->index = -1;
	}

	bool operator==(const Interval &other) {
		return start == other.start && end == other.end;
	}

	bool operator!=(const Interval &other) {
		return start != other.start || end != other.end;
	}
};

bool solve(Interval &target, vector<Interval> &intervals, vector<int> &used) {
	// Sort the intervals in increasing order based on where they start
	auto lam = [](Interval &a, Interval &b) {
		return a.start < b.start;
	};
	sort(intervals.begin(), intervals.end(), lam);

	// Special case: all intervals start after the start of the target interval 
	// => it is clearly not possible to cover the target interval 
	if (intervals[0].start > target.start) {
		return false;
	}

	double neg_inf = -1 * numeric_limits<double>::infinity();
	double leftmost_start = target.start;
	double rightmost_end = target.start;
	Interval best_choice = Interval(neg_inf, neg_inf);
	for (Interval i : intervals) {
		if (i.start <= rightmost_end) {
			if (i.end > best_choice.end) {
				// the current interval overlaps with how much we've covered 
				// so far and it stretches our covering most to the right
				// => use this as the best candidate to stretch out covering
				best_choice = i;
			}
			// the current interval does not stretch the current covered span 
			// more to the right => skip it
		}
		else {
			// the current interval no longer overlaps with the span that covers
			// the target interval 
			// => the current best choice *is* the best choice to stretch the 
			// span with
			rightmost_end = best_choice.end;	
			used.push_back(best_choice.index);

			// if the covering span now covers the target interval then we're done
			if (rightmost_end >= target.end) {
				return true;
			}

			// else: if the current does not overlap with the current span,
			// then so will not all intervals after this one
			// => we cannot cover the target interval wholly
			if (i.start > rightmost_end) {
				return false;
			}

			// if the current interval *does* overlap with the current span,
			// then let it be the current best candidate with which we will 
			// extend the span to the right
			best_choice = i;
		}
	}

	// we might have found an interval to extend the covering span with but 
	// not yet had the chance to do the extending
	// => do the extending
	if (best_choice.start != neg_inf) {
		rightmost_end = best_choice.end;
		used.push_back(best_choice.index);
	}

	// now we have extended the span as far as possible to the right 
	// with the given intervals and hopefully cover the target interval
	if (rightmost_end >= target.end) {
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
		vector<Interval> intervals;
		vector<int> used;
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
