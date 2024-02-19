#pragma once
#include "tree_node.h"
using namespace std;

int TreeNode::get_label() const {
	return label;
}

bool TreeNode::is_child(const TreeNode &other) const {
	for (int l : children) {
		if (l == other.get_label()) {
			return true;
		}
	}
	return false;
}

bool TreeNode::has_free_child_spot() const {
	return children.size() < num_children;
}

void TreeNode::add_child(const int label) {
	children.push_back(label);
}

bool TreeNode::operator<(const TreeNode &other) const {
	return children.size() <= other.children.size() &&
		label < other.label;
}

bool TreeNode::operator>(const TreeNode &other) const {
	return false;	
}

void swap(TreeNode &a, TreeNode &b) {
	swap(a.label, b.label);
	swap(a.num_children, b.num_children);
	swap(a.children, b.children);
};
