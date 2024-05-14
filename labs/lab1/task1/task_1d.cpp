/**
 * Johannes Kung johku144
 * Interval cover. This is essentially the basic interval scheduling problem,
 * a greedy problem, but where the intervals should be overlapping (but as little as possible) 
 * and as large as possible.
 * Link from the course page: https://medium.com/cracking-the-data-science-interview/greedy-algorithm-and-dynamic-programming-a8c019928405
 *
 * Time complexity: O(N*log(N)), see cover()
 */
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <ios>
#include <limits>
#include <vector>

using namespace std;

/**
 * Struct representing an interval as a start and end point. The struct also 
 * records the original index of the interval in the input, for convenience.
 */
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

/**
 * Given a list of N intervals and a target interval to cover using the intervals
 * in the list, returns true if the target can be wholly covered and a list of 
 * the indices of the intervals used to minimally cover the target.
 * Return false if it is not possible to cover the target interval with 
 * the given intervals. 
 *
 * Time complexity: O(N*log N) 
 * (Slowest part of algorithm is sorting which is O(N*log N) )
 */
bool cover(Interval &target, vector<Interval> &intervals, vector<int> &used) {
	// Sort the intervals in increasing order based on where they start
  // Time complexity O(N*log N)
	auto lam = [](Interval &a, Interval &b) {
		return a.start < b.start;
	};
	sort(intervals.begin(), intervals.end(), lam);

	// Special case: all intervals start after the start of the target interval 
	// => it is clearly not possible to cover the target interval 
	if (intervals[0].start > target.start) {
		return false;
	}

  // Loop through all intervals from left to right exactly once, time complexity 
  // O(N) as a constant amount of comparisons/operations are done in each iteration
  //
  // Try to construct an interval that covers the target interval, call it 
  // the cover, using the given intervals
  //
  // This is done by continuosly picking the interval that overlaps with the 
  // cover and that stretches farthest to the right, and then extending the 
  // cover with it (can be seen as taking the union of the cover and the chosen
  // interval for stretching)
  //
  // This continuously stretches the cover to the right, until either the cover
  // covers the target interval or we run out of intervals to consider
  //
  // Let the cover initially start and end on the left end of the target interval
  // Let the best choice initially be an interval that is on the left of all 
  // other intervals
	double neg_inf = -1 * numeric_limits<double>::infinity();
	double leftmost_start = target.start;
	double rightmost_end = target.start;
	Interval best_choice = Interval(neg_inf, neg_inf);
	for (Interval i : intervals) {
		if (i.start <= rightmost_end) {
			if (i.end > best_choice.end) {
				// the current interval overlaps with how much we've covered 
				// so far and it stretches our covering most to the right
        // among the considered intervals so far
				// => use this as the best candidate to stretch out covering
				best_choice = i;
			}
			// else, the current interval does not stretch the current covered span 
			// more to the right than the current best choice => skip it
		}
		else {
			// The current interval no longer overlaps with the cover span as constructed 
      // so far => none of the intervals left overlap with the cover span as 
      // they all start to the left of this one
      //
      // Thus, we've reached the end of intervals that overlap with the cover 
      // interval 
			// => the current best choice *is* the best choice to stretch the 
			// cover interval with
      // Expand the cover interval to the right end of the best choice and 
      // record the best choice as a used interval (record its index)
			rightmost_end = best_choice.end;	
			used.push_back(best_choice.index);

			// if the covering interval now covers the target interval then we're done
      // No need to stretch the cover interval more as we're looking for 
      // the minimal cover interval
			if (rightmost_end >= target.end) {
				return true;
			}

			// else: if the current interval does not overlap with the current span
      // after having stretched the cover with the best choice,
			// then so will none of the remaining intervals after this one
			// => there is an unbridgable gap and we cannot cover the target interval wholly
			if (i.start > rightmost_end) {
				return false;
			}

			// if the current interval *does* overlap with the current span after stretching,
			// then let it be the current best candidate with which we will 
			// extend the cover span to the right 
			best_choice = i;
		}
	}

	// we might have found an interval to extend the covering span with but 
	// not yet had the chance to do the extending (we only extend if we find 
  // an interval that does not overlap with the cover, but we might run out 
  // of intervals to use before that point!)
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

  // The covering span does not cover the target interval and there are no 
  // more intervals left to expand it with => impossible
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

    // Process input intervals
    // Make sure to save the index for each interval as this is used for the 
    // output and the solver function may reorder the intervals
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

    // Try to solve the problem
		if (cover(target, intervals, used)) {
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
