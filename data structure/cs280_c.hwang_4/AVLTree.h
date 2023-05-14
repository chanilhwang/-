#pragma once
template <typename T> 
class AVLTree : public BSTree<T> {
public:     
	AVLTree(void);     
	virtual ~AVLTree( );     
	virtual void Insert(const T& value);     
	virtual void Remove(const T& value);
private:
	void Insert(typename BSTree<T>::BinTreeNode *&tree, const T& value, std::stack<typename BSTree<T>::BinTreeNode**>& nodes);
	void Remove(typename BSTree<T>::BinTreeNode *&tree, const T& value, std::stack<typename BSTree<T>::BinTreeNode**>& nodes);
	void Balance(std::stack<typename BSTree<T>::BinTreeNode **>& nodes, bool once = true);
	void RotateLeft(typename BSTree<T>::BinTreeNode *&tree);
	void RotateRight(typename BSTree<T>::BinTreeNode *&tree);
}; 

#include "AVLTree.hpp"