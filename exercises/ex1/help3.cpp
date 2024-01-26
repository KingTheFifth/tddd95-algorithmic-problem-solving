#include <ios>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <string>

using namespace std;

bool is_placeholder(string s) {
	if (s.find_first_of("<") == 0 && s.find_last_of(">") == s.length()-1) {
		return true;
	}
	return false;
}

void tokenize(vector<string> &vec, string s, int n) {
	int pos = s.find(" ");
	while (pos > 0) {
		string token = s.substr(0, pos);
		if (is_placeholder(token)) {
			token.insert(token.length()-1, to_string(n));
		}
		vec.push_back(token);

		s.erase(0, pos+1);
		pos = s.find(" ");
	}
	if (is_placeholder(s)) {
		s.insert(s.length()-1, to_string(n));
	}
	vec.push_back(s);
}

void add_to_equalities(string w1, string w2, vector<unordered_set<string>> &eqs) {
	int first_match = -1;
	int second_match = -1;
	for (int i = 0; i < eqs.size(); i++) {
		unordered_set<string> &eq_set = eqs[i];
		if (eq_set.find(w1) != eq_set.end() || eq_set.find(w2) != eq_set.end()) {
			eq_set.insert(w1);
			eq_set.insert(w2);
			if (first_match == -1) {
				first_match = i;
			}
			else if (second_match == -1) {
				second_match = i;
			}
		}
	}
	if (first_match == -1) {
		unordered_set<string> new_eq_set = {w1, w2};
		eqs.push_back(new_eq_set);
	}
	else if (second_match != -1) {
		unordered_set<string> &first = eqs[first_match];
		for (string s : eqs[second_match]) {
			first.insert(s);	
		}
		eqs.erase(eqs.begin() + second_match);
	}
}

bool is_solvable_eq_set(unordered_set<string> &eq_set) {
	int num_non_placeholders = 0;
	for (string s : eq_set) {
		if (!is_placeholder(s)) {
			num_non_placeholders++;
		}
	}
	return num_non_placeholders < 2;
}

string replace_placeholder(string p, vector<unordered_set<string>> &eqs) {
	for (int i = 0; i < eqs.size(); i++) {
		unordered_set<string> &eq_set = eqs[i];
		if (eq_set.find(p) != eq_set.end()) {
			for (string s : eq_set) {
				if (!is_placeholder(s)) {
					return s;
				}
			}
		}
	}
	return "temp";
}

void solve() {
	vector<string> phrase_1 = vector<string>();
	vector<string> phrase_2 = vector<string>();
	vector<unordered_set<string>> eqs;
	string in;

	// Split each phrase into a sequence of words
	getline(cin, in); 
	tokenize(phrase_1, in, 1);
	getline(cin, in);
	tokenize(phrase_2, in, 2);

	// Ensure both phrases have the same amount of words
	if (phrase_1.size() != phrase_2.size()) {
		cout << "-\n";
		return;
	}
	vector<unordered_set<string>> equalities;
	for (int i = 0; i < phrase_1.size(); i++) {
		string w1 = phrase_1[i];
		string w2 = phrase_2[i];
		add_to_equalities(w1, w2, eqs);
	}
	for (int i = 0; i < eqs.size(); i++) {
		if(!is_solvable_eq_set(eqs[i])) {
			cout << "-\n";
			return;
		}	
	}
	for (int i = 0; i < phrase_1.size(); i++) {
		if (is_placeholder(phrase_1[i])) {
			cout << replace_placeholder(phrase_1[i], eqs);
		}
		else {
			cout << phrase_1[i];
		}
		if (i != phrase_1.size()-1) {
			cout << " ";
		}
	}
	cout << "\n";
}

int main() {
	cin.tie(NULL);
	ios::sync_with_stdio(false);
	string in;
	int num_cases;
	while (true) {
		getline(cin, in);
		if (cin.eof()) {
			break;
		}
		num_cases = stoi(in);
		int case_nr = 0;
		while (case_nr < num_cases) {
			solve();
			case_nr++;
		}
	}
	cout.flush();
}
