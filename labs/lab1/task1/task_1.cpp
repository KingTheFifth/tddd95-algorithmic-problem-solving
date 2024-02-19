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

void split(interval &target, interval& other, vector<interval> *result) {
	if (target.start >= other.start && target.end <= other.end) {
	}
	else if (target.start < other.start && other.end < target.end) {
		result->push_back(interval(target.start, other.start));
		result->push_back(interval(other.end, target.end));
	}
	else if (target.start < other.start) {
		result->push_back(interval(target.start, other.start));
	}
	else {
		result->push_back(interval(other.end, target.end));
	}
}

double overlap(interval &target, interval &interval) {
	// Edge case: the two intervals overlap completely
	// Treat this as an infinite overlap for practical reasons
	// (There is no better interval that overlaps more)
	if (interval.start == target.start && interval.end == target.end) {
		return INFINITY;
	}
	double start = max(target.start, interval.start);
	double end = min(target.end, interval.end);
	if (end- start < 0) {
		return 0;
	}
	return end - start;
}

void sort_intervals(interval &target, vector<interval> *intervals) {
	auto lam = [&target](interval &i1, interval &i2) {
		return overlap(target, i1) < overlap(target, i2);
	};
	sort(intervals->begin(), intervals->end(), lam);
}

bool solve(vector<interval> *targets, vector<interval> *intervals, vector<int> *used) {
	//cout << "solve(" << A << ", " << B << ", " << intervals->size() << ", " << *used << ")\n";

	// Base case: there is no interval to cover
	// => trivially possible
	if (targets->size() == 0) {
		return true;
	}

	// Base case: there are no intervals to cover the target interval(s) with
	// => impossible
	if (intervals->size() == 0) {
		return false;
	}

	// Recursive case: greedily try to cover a target interval [A, B] with 
	// the interval that covers the most of [A, B].
	interval target = targets->back();
	targets->pop_back();
	sort_intervals(target, intervals);
	interval greedy_choice = intervals->back();
	intervals->pop_back();
	used->push_back(greedy_choice.index);
	//used->push_back(greedy_choice);

	// If the best choice has no overlap then it is impossible to cover [A, B]
	if (overlap(target, greedy_choice) == 0) {
		return false;
	}

	// Try to solve the subproblem of covering the uncovered part of [A,B]
	// with the remainding unused intervals
	split(target, greedy_choice, targets);
	return solve(targets, intervals, used);
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
		vector<interval> targets = vector<interval>();
		vector<interval> intervals = vector<interval>();
		vector<int> used = vector<int>();
		targets.push_back(interval(A, B));
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
		if (solve(&targets, &intervals, &used)) {
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
