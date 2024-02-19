#include <bits/stdc++.h>
#include <functional>
#include <queue>
#include <system_error>
#include <unordered_map>
#include "tree_node.h"
#include "tree_heap.h"
using namespace std;

vector<int> solve(int N, vector<int> &v_column) {
	vector<int> u_column;
	vector<TreeNode> tree_nodes = vector<TreeNode>(N+1);
	for (int i = N-1; i >= 0; i--) {
	}
	return u_column;
}

int main() {
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	TreeHeap<less<TreeNode>> t_h;
	TreeNode *b = new TreeNode(1, 1, -1);
	TreeNode *a = new TreeNode(2, 0, -1);
	t_h.insertNode(a);
	t_h.insertNode(new TreeNode(3, 0, -1));
	t_h.insertNode(b);
	cout << t_h.getTop()->get_label() << "\n";
	b = t_h.getNode(1);
	b->add_child(a->get_label());
	t_h.updateNode(b);
	cout << t_h.getTop()->get_label() << "\n";
	int N, in;
	while (true) {
		cin >> N;
		if (cin.eof()) {
			break;
		}

		vector<int> v_col, u_col;
		for (int i = 0; i < N; i++) {
			cin >> in;
			v_col.push_back(in);
		}
		u_col = solve(N, v_col);
		if (u_col.empty()) {
			cout << "Error\n";
		}
		else {
			for (int i = 0; i < u_col.size(); i++) {
				cout << u_col[i] << "\n";
			}
		}
	}
	cout.flush();
}
