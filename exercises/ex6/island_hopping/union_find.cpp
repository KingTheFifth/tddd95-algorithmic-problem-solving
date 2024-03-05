#include <vector>

using namespace std;

struct UnionSet {
	vector<int> parent;
	vector<int> depth;

	UnionSet(int size) {
		// O(N)
		for (int i = 0; i < size; i++) {
			// Initialise every element to belong to a set with itself only,
			// having depth 1
			parent.push_back(i);
			depth.push_back(1);
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
