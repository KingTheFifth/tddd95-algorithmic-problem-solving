#include <ios>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>
#include <string>

bool is_placeholder(std::string s) {
	if (s.find_first_of("<") == 0 && s.find_last_of(">") == s.length()-1) {
		return true;
	}
	return false;
}

void tokenize(std::vector<std::string> &vec, std::string s, int n) {
	int pos = s.find(" ");
	while (pos > 0) {
		std::string token = s.substr(0, pos);
		if (is_placeholder(token)) {
			token.insert(token.length()-1, std::to_string(n));
		}
		vec.push_back(token);

		s.erase(0, pos+1);
		pos = s.find(" ");
	}
	if (is_placeholder(s)) {
		s.insert(s.length()-1, std::to_string(n));
	}
	vec.push_back(s);
}

std::string substitute(std::unordered_map<std::string, std::string> *p_to_p, std::string w) {
	// TODO: Use memoization to speed up? Probably not needed
	
	// Base case:
	// Actual words are not substituted
	if (!is_placeholder(w)) {
		return w;
	}

	// Base case: 
	// The placeholder is not mapped to some other place holder
	// so don't substitute it
	if (p_to_p->find(w) == p_to_p->end()) {
		return w;
	}

	// Recursive case: substitute the placeholder with the substitute of 
	// the placeholder that it is mapped to
	return substitute(p_to_p, p_to_p->at(w));
}

bool map_placeholder(
		std::unordered_map<std::string, std::string> *p_to_w,
		std::string p,
		std::string w) {
	if (p_to_w->find(p) == p_to_w->end()) {
		p_to_w->insert(std::make_pair(p, w));
		return true;
	}
	if (p_to_w->at(p).compare(w) == 0) {
		return true;
	}
	return false;
}

bool map_placeholders(
		std::unordered_map<std::string, std::string> *p_to_w,
		std::unordered_map<std::string, std::string> *p_to_p,
		std::string p1,
		std::string p2) {

	if (p1.compare(p2) == 0) {
		return true;
	}

	// Both placeholders already mapped to words
	// Make sure they map to same word
	if (p_to_w->find(p1) != p_to_w->end() && p_to_w->find(p2) != p_to_w->end()) {
		if (p_to_w->at(p1).compare(p_to_w->at(p2)) != 0) {
			return false;
		}
	}

	// Only one of the placeholders not mapped to a word
	// Map it to the word that the other placeholder is mapped to
	if (p_to_w->find(p1) != p_to_w->end()) {
		p_to_w->insert(std::make_pair(p2, p_to_w->at(p1)));
		return true;
	}
	if (p_to_w->find(p2) != p_to_w->end()) {
		p_to_w->insert(std::make_pair(p1, p_to_w->at(p2)));
		return true;
	}
	// Neither placeholder mapped to a word
	// Map the first to the second
	p_to_p->insert(std::make_pair(p1, p2));
	return true;
}

void solve() {
	std::vector<std::string> phrase_1 = std::vector<std::string>();
	std::vector<std::string> phrase_2 = std::vector<std::string>();
	std::unordered_map<std::string, std::string> p_to_w = std::unordered_map<std::string, std::string>();
	std::unordered_map<std::string, std::string> p_to_p = std::unordered_map<std::string, std::string>();
	std::string in;

	// Split each phrase into a sequence of words
	getline(std::cin, in); 
	tokenize(phrase_1, in, 1);
	getline(std::cin, in);
	tokenize(phrase_2, in, 2);

	// Ensure both phrases have the same amount of words
	if (phrase_1.size() != phrase_2.size()) {
		std::cout << "-\n";
		return;
	}

	// Match up both words of each phrase at the same position in the phrases
	for(int i = 0; i < phrase_1.size(); i++) {
		std::string w1 = substitute(&p_to_p, phrase_1[i]);
		std::string w2 = substitute(&p_to_p, phrase_2[i]);

		if (!is_placeholder(w1) && !is_placeholder(w2)){
			if (w1.compare(w2) != 0) {
				std::cout << "-\n";
				return;
			}
			continue;
		}

		// If w1 is a placeholder and w2 a word, map w1 -> w2
		if (!is_placeholder(w2)) {
			if(!map_placeholder(&p_to_w, w1, w2)) {
				std::cout << "-\n";
				return;
			}
			continue;
		}

		// If w1 is a word and w2 a placeholder, map w2 -> w1
		if (!is_placeholder(w1)) {
			if(!map_placeholder(&p_to_w, w2, w1)) {
				std::cout << "-\n";
				return;
			}
			continue;
		}
		
		// If both w1 and w2 are placeholders, map w1 -> w2
		if(!map_placeholders(&p_to_w, &p_to_p, w1, w2)) {
			std::cout << "-\n";
			return;
		}

	}

	for (int i = phrase_2.size()-1; i >= 0; i--) {
		std::string w = phrase_2[i];
		// Actual words are not substituted
		if (!is_placeholder(w)) {
			phrase_1[i] = w;
		}

		// If the placeholder is mapped to some word,
		// replace it by its word
		else if (p_to_w.find(w) != p_to_w.end()) {
			phrase_1[i] = p_to_w[w];
		}

		// Else if it also does not map to some other placeholder,
		// then it is allowed to be any word
		// Map it to "temp" and substitute it with "temp"
		else if (p_to_p.find(w) == p_to_p.end()) {
			p_to_w.insert(std::make_pair(w, "temp"));
			phrase_1[i] = "temp";
		}

		// Lastly if it maps to some other placeholder,
		// substitue it with what the other placeholder maps to
		else if (p_to_p.find(w) != p_to_p.end()) {
			std::string p2 = p_to_p[w];
			std::string w2 = p_to_w[p2];
			p_to_w.insert(std::make_pair(w, w2));
			phrase_1[i] = w2;
		}
	}

	std::string result;
	for(auto iter = phrase_1.begin(); iter != phrase_1.end(); ++iter) {
		result.append(*iter);
		result.append(" ");
	}

	if (phrase_1.size() > 0) {
		result.pop_back();
	}

	std::cout << result << "\n";
}

int main() {
	std::cin.tie(NULL);
	std::ios::sync_with_stdio(false);
	std::string in;
	int num_cases;
	while (true) {
		getline(std::cin, in);
		if (std::cin.eof()) {
			break;
		}
		num_cases = std::stoi(in);
		int case_nr = 0;
		while (case_nr < num_cases) {
			solve();
			case_nr++;
		}
	}
	std::cout.flush();
}
