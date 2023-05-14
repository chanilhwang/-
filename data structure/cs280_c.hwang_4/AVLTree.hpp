/*------------------------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: AVLTree.hpp
Purpose: to implement AVLTree
Project: assignment4
Author: c.hwang
Created: 2019/05/18
------------------------------------------------------------------------*/
#include <string>
#include <stack>

#include "BSTree.h"
#include "AVLTree.h"

template<typename T>
inline AVLTree<T>::AVLTree(void) : BSTree<T>() {
}

template<typename T>
AVLTree<T>::~AVLTree( ) {
}

template<typename T>
void AVLTree<T>::Insert(const T & value) {
	std::stack<typename BSTree<T>::BinTreeNode**> nodes;
	Insert(BSTree<T>::root, value, nodes);
	while (!nodes.empty()) {
		int l_height = -1,
			r_height = -1;
		if ((*nodes.top())->left)
			l_height += (*nodes.top())->left->Height() + 1;
		if ((*nodes.top())->right)
			r_height += (*nodes.top())->right->Height() + 1;
		int height_difference = l_height - r_height;
		if (height_difference > 1 || height_difference < -1) {
			if (l_height < r_height) {
				int rr_height = -1,
					rl_height = -1;
				if ((*nodes.top())->right->right)
					rr_height = (*nodes.top())->right->right->Height();
				if ((*nodes.top())->right->left)
					rl_height = (*nodes.top())->right->left->Height();
				if (rr_height > rl_height)
					Balance(nodes);
				else
					Balance(nodes, false);
			}
			else if (l_height > r_height) {
				int lr_height = -1,
					ll_height = -1;
				if ((*nodes.top())->left->right)
					lr_height = (*nodes.top())->left->right->Height();
				if ((*nodes.top())->left->left)
					ll_height = (*nodes.top())->left->left->Height();
				if (lr_height > ll_height)
					Balance(nodes, false);
				else
					Balance(nodes);
			}
		}
		nodes.pop();
	}
}

template<typename T>
void AVLTree<T>::Remove(const T & value) {
	std::stack<typename BSTree<T>::BinTreeNode**> nodes;
	Remove(BSTree<T>::root, value, nodes);
	if (!nodes.empty() && (*nodes.top()) == nullptr)
		nodes.pop();
	while (!nodes.empty()) {
		int l_height = -1,
			r_height = -1;
		if ((*nodes.top())->left)
			l_height = (*nodes.top())->left->Height();
		if ((*nodes.top())->right)
			r_height = (*nodes.top())->right->Height();
		int height_difference = l_height - r_height;
		if (height_difference > 1 || height_difference < -1) {
			if (l_height < r_height) {
				int rr_height = -1,
					rl_height = -1;
				if ((*nodes.top())->right->right)
					rr_height = (*nodes.top())->right->right->Height();
				if ((*nodes.top())->right->left)
					rl_height = (*nodes.top())->right->left->Height();
				if (rr_height > rl_height)
					Balance(nodes);
				else
					Balance(nodes, false);
			}
			else if (l_height > r_height) {
				int lr_height = -1,
					ll_height = -1;
				if ((*nodes.top())->left->right)
					lr_height = (*nodes.top())->left->right->Height();
				if ((*nodes.top())->left->left)
					ll_height = (*nodes.top())->left->left->Height();
				if (lr_height > ll_height)
					Balance(nodes, false);
				else
					Balance(nodes);
			}
		}
		nodes.pop();
	}
}

template <typename T>
void AVLTree<T>::Insert(typename BSTree<T>::BinTreeNode *&tree, const T& value, std::stack<typename BSTree<T>::BinTreeNode**>& nodes) {
	if (!tree) {
		tree = new typename BSTree<T>::BinTreeNode(value);
		return;
	}
	tree->count++;
	nodes.push(&tree);
	if (tree->data > value) {
		Insert(tree->left, value, nodes);
	}
	else if (tree->data < value) {
		Insert(tree->right, value, nodes);
	}
}

template <typename T>
void AVLTree<T>::Remove(typename BSTree<T>::BinTreeNode *&treePtr, const T& value, std::stack<typename BSTree<T>::BinTreeNode **>& nodes) {
	if (!treePtr) {
		nodes.push(&treePtr);
		return;
	}
	if (value < treePtr->data) {
		nodes.push(&treePtr);
		Remove(treePtr->left, value, nodes);
		if ((*nodes.top()))
			treePtr->count--;
		return;
	}
	else if (value > treePtr->data) {
		nodes.push(&treePtr);
		Remove(treePtr->right, value, nodes);
		if ((*nodes.top()))
			treePtr->count--;
		return;
	}
	else {
		if (treePtr->left && treePtr->right) {
			typename BSTree<T>::BinTreeNode* max_left = treePtr->left;
			while (max_left->right) {
				max_left = max_left->right;
			}
			treePtr->data = max_left->data;
			treePtr->count--;
			nodes.push(&treePtr);
			Remove(treePtr->left, max_left->data, nodes);
		}
		else if (treePtr->right) {
			typename BSTree<T>::BinTreeNode* temp = treePtr;
			treePtr = treePtr->right;
			delete temp;
		}
		else if (treePtr->left) {
			typename BSTree<T>::BinTreeNode* temp = treePtr;
			treePtr = treePtr->left;
			delete temp;
		}
		else {
			delete treePtr;
			treePtr = nullptr;
		}
	}
}

template <typename T>
void AVLTree<T>::Balance(std::stack<typename BSTree<T>::BinTreeNode **>& nodes, bool once) {
	int l_height = -1,
		r_height = -1;
	if ((*nodes.top())->left) {
		l_height = (*nodes.top())->left->Height();
	}
	if ((*nodes.top())->right) {
		r_height = (*nodes.top())->right->Height();
	}
	if (l_height > r_height) {
		if (once)
			RotateRight((*nodes.top()));
		else {
			RotateLeft((*nodes.top())->left);
			RotateRight((*nodes.top()));
		}
	}
	else {
		if (once)
			RotateLeft((*nodes.top()));
		else {
			RotateRight((*nodes.top())->right);
			RotateLeft((*nodes.top()));
		}
	}
}

template <typename T>
void AVLTree<T>::RotateLeft(typename BSTree<T>::BinTreeNode *&tree) {
	typename BSTree<T>::BinTreeNode* temp = tree;
	tree = tree->right;
	if (tree) {
		tree->count = temp->count;
		temp->right = tree->left;
		tree->left = temp;
	}
	temp->count = 1;
	if (temp->left)
		temp->count += temp->left->count;
	if (temp->right)
		temp->count += temp->right->count;
}

template <typename T>
void AVLTree<T>::RotateRight(typename BSTree<T>::BinTreeNode *&tree) {
	typename BSTree<T>::BinTreeNode* temp = tree;
	tree = tree->left;
	if (tree) {
		tree->count = temp->count;
		temp->left = tree->right;
		tree->right = temp;
	}

	temp->count = 1;
	if (temp->left)
		temp->count += temp->left->count;
	if (temp->right)
		temp->count += temp->right->count;
}
