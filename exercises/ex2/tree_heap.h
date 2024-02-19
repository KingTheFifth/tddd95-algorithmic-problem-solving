#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include "tree_node.h"

using namespace std;

template <class Compare> class TreeHeap {
	private:
		unordered_map<int, int>	label_to_elem_index;
		vector<TreeNode*> elems;

		//void swap(TreeNode &a, TreeNode &b);
		void upHeap(int index);
		void downHeap(int index);
		int getParent(int index);
		int getLeftChild(int index);
		int getRightChild(int index);
	public:
		void insertNode(TreeNode *node);
		void removeNode(TreeNode &node);
		void updateNode(TreeNode *node);
		void updateNode(int label);
		void changeLabel(int old_label, int new_label);
		TreeNode* getTop();
		void popTop();
		bool empty() const;
		TreeNode* getNode(int label);
};

#include "tree_heap_impl.h"
