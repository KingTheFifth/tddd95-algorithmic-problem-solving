#include <ios>
#include <iostream>
#include <unordered_map>
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

bool mapPlaceholderToWord(string p, string w, unordered_map<string, string> &p_to_w) {
	if (p_to_w.find(p) != p_to_w.end()) {
		if (p_to_w[p].compare(w) == 0) {
			return false;
		}
		return false;
	}
	p_to_w.insert({p, w});
	return true;
}

bool doEasyMappings(vector<string> &p1, vector<string> &p2, unordered_map<string, string> &p_to_w) {
	for (int i = 0; i < p1.size(); i++) {
		string w1 = p1[i];
		string w2 = p2[i];

		if (is_placeholder(w1) && !is_placeholder(w2)) {
			if (mapPlaceholderToWord(w1, w2, p_to_w)) {
				return true;
			}
		}
		if (is_placeholder(w2) && !is_placeholder(w1)) {
			if (mapPlaceholderToWord(w2, w1, p_to_w)) {
				return true;
			}
		}
		if (is_placeholder(w1) && is_placeholder(w2)) {
			if (p_to_w.find(w1) != p_to_w.end() && p_to_w.find(w2) == p_to_w.end()) {
				mapPlaceholderToWord(w2, p_to_w[w1], p_to_w);
				return true;
			}
			if (p_to_w.find(w1) == p_to_w.end() && p_to_w.find(w2) != p_to_w.end()) {
				mapPlaceholderToWord(w1, p_to_w[w2], p_to_w);
				return true;
			}
		}
	}
	return false;
}

bool doHardMappings(vector<string> &p1, vector<string> &p2, unordered_map<string, string> &p_to_w) {
	for (int i = 0; i < p1.size(); i++) {
		string w1 = p1[i];
		string w2 = p2[i];

		if (is_placeholder(w1) && is_placeholder(w2)) {
			if (p_to_w.find(w1) == p_to_w.end() && p_to_w.find(w2) == p_to_w.end()) {
				if (mapPlaceholderToWord(w1, "temp", p_to_w) && mapPlaceholderToWord(w2, "temp", p_to_w)) {
					return true;
				}
			}
		}
	}
	return false;
}

void solve() {
	vector<string> phrase_1 = vector<string>();
	vector<string> phrase_2 = vector<string>();
	unordered_map<string, string> p_to_w = unordered_map<string, string>();
	unordered_map<string, string> p_to_p = unordered_map<string, string>();
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

	while (doEasyMappings(phrase_1, phrase_2, p_to_w)) {}

	while (doHardMappings(phrase_1, phrase_2, p_to_w)) {}

	string result;
	for (int i = 0; i < phrase_1.size(); i++) {
		string w1 = phrase_1[i];
		string w2 = phrase_2[i];
		if (is_placeholder(w1)) {
			w1 = p_to_w[w1];
		}
		if (is_placeholder(w2)) {
			w2 = p_to_w[w2];
		}

		if (w1.compare(w2) == 0) {
			result.append(w1);
			if (i != phrase_1.size() - 1) {
				result.append(" ");	
			}
		}
		else {
			cout << "-\n";
			return;
		}
	}
	cout << result << "\n";
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
