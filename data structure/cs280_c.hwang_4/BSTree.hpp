/*------------------------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: BSTree.hpp
Purpose: to implement BSTree
Project: assignment4
Author: c.hwang
Created: 2019/05/18
------------------------------------------------------------------------*/
#include <string>

#include "BSTree.h"

const char *ReadableType(const char *name) {
	if(std::strstr(name, "AVL"))
		return "AVLTree";
	else
		return "BSTree";
}

unsigned int two_complement(unsigned int index) {
	unsigned int result = 1;
	for (unsigned int i = 0; i < index; i++) {
		result *= 2;
	}
	return result;
}

template<typename T>
void BSTree<T>::DebugPrint(bool showCounts) const {
	std::vector<BinTreeNode *> printQueue;

	if(root != nullptr) {
		std::cout << "type: " << ReadableType(typeid(T).name( )) << ", height: " << root->Height( );
		std::cout << ", nodes: " << root->count << std::endl;

		// Populate printQueue
		printQueue.push_back(root);
		unsigned int maximum_size = two_complement(Height()) - 1;
		for(unsigned int i = 0; i < maximum_size; i++) {
			BinTreeNode* curr = printQueue[i];
			if (curr) {
				printQueue.push_back(curr->left);
				printQueue.push_back(curr->right);
			}
			else {
				printQueue.push_back(nullptr);
				printQueue.push_back(nullptr);
			}
		}
		int nodeRowCount = 0;
		int nodeRowTarget = 1;
		int coutWidth = 2 << root->Height( );
		std::cout.width(coutWidth / 2);
		std::cout << ' ';
		// first node of each row should have a 1/2 width offset
		for(unsigned int queueIndex = 0; queueIndex < printQueue.size( ); queueIndex++) {
			if(printQueue[queueIndex] != nullptr) {
				std::cout << printQueue[queueIndex]->data;
				std::cout.width(coutWidth - 1);
				std::cout << ' ';
			} else {
				std::cout << "X";
				std::cout.width(coutWidth - 1);
				std::cout << ' ';
			}

			if(++nodeRowCount == nodeRowTarget) {
				std::cout << std::endl;
				nodeRowTarget *= 2;
				coutWidth /= 2;
				nodeRowCount = 0;
				std::cout.width(coutWidth / 2);
				std::cout << ' ';
				// first node of each row should have a 1/2 width offset
			}
		}
		if(showCounts) {
			for(unsigned int queueIndex = 0; queueIndex < printQueue.size( ); queueIndex++) {
				if(printQueue[queueIndex] != nullptr) {
					std::cout << printQueue[queueIndex]->data << "(" << printQueue[queueIndex]->count << ")  ";
				}
			}
		}
		std::cout << std::endl;
		std::cout << std::endl;
	} else {
		std::cout << "Tree is Empty" << std::endl;
	}
}

template<typename T>
inline BSTree<T>::BSTree() : root(nullptr) {
}

template<typename T>
BSTree<T>::BSTree(const BSTree & rhs) : root(CopyTree(rhs.root)) {
}

template<typename T>
BSTree<T>::~BSTree( ) {
	Clear();
}

//need to arrange with memory problem
template<typename T>
BSTree<T> & BSTree<T>::operator=(const BSTree & rhs) {
	if (root == nullptr)
		root = rhs.root;
	else {
		Clear();
		root = rhs.root;
	}
	return *this;
}

template<typename T>
void BSTree<T>::Insert(const T & value) {
	if (root)
		root->Insert(value);
	else
		root = new BinTreeNode(value);
}

template<typename T>
void BSTree<T>::Remove(const T & value) {
	if(Find(value))
		RemoveRecurs(root, value);
}

template<typename T>
void BSTree<T>::Clear( ) {
	if (root) {
		root->FreeTree();
		ResetRoot();
	}
}

template<typename T>
bool BSTree<T>::Empty(void) const {
	return !root;
}

template<typename T>
unsigned int BSTree<T>::Size( ) const {
	if (root)
		return root->count;
	else
		return 0;
}

template<typename T>
int BSTree<T>::Height(void) const {
	if (root)
		return root->Height();
	else
		return 0;
}

template<typename T>
bool BSTree<T>::Find(const T & value) const {
	if (root)
		return root->Find(value);
	else
		return false;
}

//need to change
template<typename T>
bool BSTree<T>::RemoveRecurs(BSTree<T>::BinTreeNode *&treePtr, const T & value) {
	if (!treePtr)
		return false;
	if (value < treePtr->data) {
		if (RemoveRecurs(treePtr->left, value)) {
			treePtr->count--;
			return true;
		}
	}
	else if (value > treePtr->data) {
		if (RemoveRecurs(treePtr->right, value)) {
			treePtr->count--;
			return true;
		}
	}
	else {
		if (treePtr->left && treePtr->right) {
			BinTreeNode* max_left = treePtr->left;
			while (max_left->right) {
				max_left = max_left->right;
			}
			treePtr->data = max_left->data;
			treePtr->count--;
			RemoveRecurs(treePtr->left, max_left->data);
		}
		else if(treePtr->right) {
			BinTreeNode* temp = treePtr;
			treePtr = treePtr->right;
			delete temp;
		}
		else if (treePtr->left) {
			BinTreeNode* temp = treePtr;
			treePtr = treePtr->left;
			delete temp;
		}
		else {
			delete treePtr;
			treePtr = nullptr;
		}
		return true;
	}
	return false;
}

template<typename T>
void BSTree<T>::BinTreeNode::FreeTree() {	
	if (left)
		left->FreeTree();
	if (right)
		right->FreeTree();
	delete this;
}

template<typename T>
typename BSTree<T>::BinTreeNode * BSTree<T>::CopyTree(BSTree<T>::BinTreeNode * source) {
	if (!source)
		return nullptr;
	BinTreeNode* copied_tree = new BinTreeNode(source->data);
	copied_tree->count = source->count;
	if (source->left)
		copied_tree->left = CopyTree(source->left);
	if (source->right)
		copied_tree->right = CopyTree(source->right);
	return copied_tree;
}

//need to implement
template<typename T>
const typename BSTree<T>::BinTreeNode &BSTree<T>::BinTreeNode::IndexOf(int index) const {
	int decreased_index = 1;
	if (left) {
		decreased_index += static_cast<int>(left->count);
		if (left->count > static_cast<unsigned int>(index)) {
			return left->IndexOf(index);
		}
		else if (left->count == static_cast<unsigned int>(index)) {
			return *this;
		}
	}
	if (index == 0) {
		return *this;
	}
	if(right) {
		return right->IndexOf(index - decreased_index);
	}
	return *this;
}

template <typename T>
void BSTree<T>::BinTreeNode::Insert(const T &value) {
	count++;
	if (data > value) {
		if (left)
			left->Insert(value);
		else {
			left = new BinTreeNode(value);
		}
	}
	else if (data < value) {
		if (right)
			right->Insert(value);
		else {
			right = new BinTreeNode(value);
		}
	}
}

template <typename T>
int BSTree<T>::BinTreeNode::Height() const {
	int l_height = -1,
		r_height = -1;
	if (left)
		l_height = left->Height();
	if (right)
		r_height = right->Height();
	return l_height > r_height ? l_height + 1 : r_height + 1;
}

template<typename T>
inline bool BSTree<T>::BinTreeNode::Find(const T & value) {
	if (value == data)
		return true;
	else if (value > data) {
		if (right)
			return right->Find(value);
		else
			return false;
	}
	else if (value < data) {
		if (left)
			return left->Find(value);
		else
			return false;
	}
	return false;
}

template<typename T>
const typename BSTree<T>::BinTreeNode &BSTree<T>::operator[](int index) const {
	if (index < 0 || index > static_cast<int>(Size()))
		return *root;
	if (root)
		return root->IndexOf(index);
	return *root;
}