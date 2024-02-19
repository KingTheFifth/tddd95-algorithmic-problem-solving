#pragma once
#include "tree_heap.h"
#include <functional>

using namespace std;

// TODO: fix all getters to return correct indices
template <class Compare> int TreeHeap<Compare>::getLeftChild(int index) {
	// 2n where n = index - 1
	return 2*index+1;
}

template <class Compare> int TreeHeap<Compare>::getRightChild(int index) {
	// 2(n+1) where n = index - 1
	return 2*index+2;
}

template <class Compare> int TreeHeap<Compare>::getParent(int index) {
	// n / 2 (integer division) where n = index - 1
	return (index-1)/2;
}

template <class Compare> void TreeHeap<Compare>::upHeap(int index) {
	// Base case: we have reached the top of the heap => stop
	if (index == 0) {
		return;
	}
	TreeNode *toMove = elems[index]; 
	int parentIndex = getParent(index);
	TreeNode *parent = elems[parentIndex];
	// Case: the current element should swap places with its parent
	if (Compare{}(*toMove, *parent)) {
		label_to_elem_index[toMove->get_label()] = parentIndex;
		label_to_elem_index[parent->get_label()] = index;
		swap(*toMove, *parent);
		upHeap(parentIndex);
	}
	// Else the heap property is already maintained
}

template <class Compare> void TreeHeap<Compare>::downHeap(int index) {
	// TODO: base case
	int leftChildIndex = getLeftChild(index);
	int rightChildIndex = getRightChild(index);

	if (leftChildIndex >= elems.size()) {
		return;
	}
	TreeNode *leftChild = elems[leftChildIndex];
	TreeNode *toMove = elems[index];
	if (rightChildIndex >= elems.size()) {
		if(Compare{}(*leftChild, *toMove)) {
			label_to_elem_index[toMove->get_label()] = leftChildIndex;
			label_to_elem_index[leftChild->get_label()] = index;
			swap(*leftChild, *toMove);
			downHeap(leftChildIndex);
		}
		return;
	}

	TreeNode *rightChild = elems[rightChildIndex];
	if (Compare{}(*leftChild, *rightChild)) {
		if(Compare{}(*leftChild, *toMove)) {
			label_to_elem_index[toMove->get_label()] = leftChildIndex;
			label_to_elem_index[leftChild->get_label()] = index;
			swap(*leftChild, *toMove);
			downHeap(leftChildIndex);
		}
	}
	else if (Compare{}(*rightChild, *toMove)) {
		label_to_elem_index[toMove->get_label()] = rightChildIndex;
		label_to_elem_index[rightChild->get_label()] = index;
		swap(*rightChild, *toMove);
		downHeap(rightChildIndex);
	}
}

template <class Compare> void TreeHeap<Compare>::insertNode(TreeNode *node) {
	label_to_elem_index.insert({node->get_label(), elems.size()});
	elems.push_back(node);
	upHeap(elems.size() - 1);
}

template <> void TreeHeap<greater<TreeNode>>::updateNode(TreeNode *node) {
	int index = label_to_elem_index[node->get_label()];	
	upHeap(index);
}

template <> void TreeHeap<less<TreeNode>>::updateNode(TreeNode *node) {
	int index = label_to_elem_index[node->get_label()];	
	downHeap(index);
}

template <class Compare> void TreeHeap<Compare>::removeNode(TreeNode &node) {
	// TODO: implement or evaluate if this is rly needed
	// TreeNode &last_node = elems.back();	
	// int last_node_index = elems.size()-1;
	// int node_index = label_to_elem_index[node.get_label()];
	// label_to_elem_index[last_node.get_label()]; 
}

template <class Compare> void TreeHeap<Compare>::popTop() {
	TreeNode *root = elems.front();
	TreeNode *last = elems.back();
	label_to_elem_index[last->get_label()] = 0;
	swap(*root, *last);
	elems.pop_back();
}

template <class Compare> TreeNode* TreeHeap<Compare>::getTop() {
	return elems[0];
}

template <class Compare> bool TreeHeap<Compare>::empty() const {
	return elems.empty();
}
template <class Compare> void TreeHeap<Compare>::updateNode(int label) {
	int index = label_to_elem_index[label];
	upHeap(index);
}

template <class Compare> TreeNode* TreeHeap<Compare>::getNode(int label) {
	int index = label_to_elem_index[label];
	return elems[index];
}

void changeLabel(int old_label, int new_label);
