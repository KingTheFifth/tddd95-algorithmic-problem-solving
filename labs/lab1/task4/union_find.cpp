#include <iostream>
#include <ios>
#include <string>
#include <vector>

using namespace std;

struct UnionSet {
	vector<int> parent;
	vector<int> depth;

	UnionSet(int size) {
		// O(N)
		for (int i = 0; i < size; i++) {
			// Initialise every element to belong to a set with itself only,
			// having depth 0
			parent.push_back(i);
			depth.push_back(0);
		}
	}

	int find_representative(int e) {
		// Recursively find the representative of the set for element e 
		// by following the chain of parents
		if (parent[e] == e) {
			return e;
		}

		// Path compression: set the parent of e to the representative of e
		int p = find_representative(parent[e]);
		parent[e] = p;
		return p;
	}

	void join(int a, int b) {
		// Join on the representatives
		a = find_representative(a);
		b = find_representative(b);

		// Only join if the representatives are different, i.e. they are 
		// not already the same set
		if (a != b) {
			// Choose the one with the lowest depth as the new representative 
			// and increase its depth with the depth of the one not chosen 
			// as representative
			if (depth[a] < depth[b]) {
				parent[b] = a;
				depth[a] += depth[b];
			}
			else {
				parent[a] = b;
				depth[b] += depth[a];
			}
		}
	}

	bool same(int a, int b) {
		return find_representative(a) == find_representative(b);
	}
};

struct Query {
	char operation;
	int operand1;
	int operand2;

	Query(char operation, int operand1, int operand2) {
		this->operation = operation;
		this->operand1 = operand1;
		this->operand2 = operand2;
	}
};

vector<string> solve(int N, vector<Query> &queries) {
	vector<string> answers;	
	UnionSet union_set = UnionSet(N);

	// O(Q) where Q is the number of queries (queries.size())
	for (Query q : queries) {
		if (q.operation == '=') {
			union_set.join(q.operand1, q.operand2);
		}
		else if (q.operation == '?') {
			if (union_set.same(q.operand1, q.operand2)) {
				answers.push_back("yes");
			}
			else {
				answers.push_back("no");
			}
		}
	}
	return answers;
}

int main() {
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int N, Q, a, b;
	char op;
	vector<Query> queries;
	while (true) {
		cin >> N >> Q;
		if (cin.eof()) {
			break;
		}
		while (Q > 0) {
			cin >> op >> a >> b;
			queries.push_back(Query(op, a, b));
			Q--;
		}
		vector<string> answers = solve(N, queries);
		for (string ans : answers) {
			cout << ans << "\n";
		}
	}
	cout.flush();
}
