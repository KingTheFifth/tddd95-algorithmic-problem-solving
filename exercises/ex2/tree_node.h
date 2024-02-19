#pragma once
#include <vector>
using namespace std;
class TreeNode {
private:
	int label;
	int num_children;
	vector<int> children;
	int parent;

public:
	TreeNode() {
		label = -1;
		num_children = 0;
		parent = -1;
	}

	TreeNode(int label, int num_children, int parent) {
		this->label = label;
		this->num_children = num_children;
		this->parent = parent;
	}

	int get_label() const;

	bool is_child(const TreeNode &other) const;

	bool has_free_child_spot() const;

	void add_child(const int label);

	bool operator<(const TreeNode &other) const;
	bool operator>(const TreeNode &other) const;

	friend void swap(TreeNode &a, TreeNode &b);
};

#include "tree_node_impl.h"
