#pragma once

class BSTException {
public:
		  // Possible exception codes
	enum BST_EXCEPTION {
		E_NO_MEMORY,
		E_DUPLICATE,
	};

	BSTException(BST_EXCEPTION ErrCode, const std::string& Message) : error_code_(ErrCode), message_(Message) { };

	virtual ~BSTException( ) {
	}

	BST_EXCEPTION code(void) const {
		return error_code_;
	}

	virtual const char *what(void) const {
		return message_.c_str( );
	}
private:
	BST_EXCEPTION error_code_;
	std::string message_;
};

template <typename T> 
class BSTree {
public:
	struct BinTreeNode {
	public:
		BinTreeNode(const T& value) : left(0), right(0), data(value), count(1) { };
		BinTreeNode(const BinTreeNode& rhs) = delete;
		BinTreeNode(BinTreeNode&& rhs) = delete;

		BinTreeNode & operator =(BinTreeNode &rhs) = delete;

		const BinTreeNode &IndexOf(int index) const;
		void Insert(const T &value);
		int Height( ) const;
		bool Find(const T &value);
		void FreeTree();

		BinTreeNode *left;
		BinTreeNode *right;
		T data;
		unsigned count;      
	};

	BSTree();
	BSTree(const BSTree& rhs);
	virtual ~BSTree( );
	BSTree& operator=(const BSTree& rhs);
	void DebugPrint(bool showCounts = false) const;
	virtual void Insert(const T& value);
	virtual void Remove(const T& value);

	BinTreeNode *CopyTree(BinTreeNode *source);
	void Clear(void);
	void ResetRoot( ) { root = nullptr; }
	bool Find(const T& value) const;
	bool Empty(void) const;
	unsigned int Size( ) const;
	int Height(void) const;
	const BinTreeNode &operator[](int index) const;
protected:     // Protected methods   
	BinTreeNode * root;
private:     // private members and methods 
	bool RemoveRecurs(BinTreeNode *& link, const T & value);
};

#include "BSTree.hpp"