////////////////////////////////////////////////////////////////////////////////
#ifndef LARIAT_H
#define LARIAT_H
////////////////////////////////////////////////////////////////////////////////


class ListArrayException : public std::exception {
private:
	int m_ErrCode;
	std::string m_Description;

public:
	ListArrayException(int ErrCode, const std::string& Description) :
		m_ErrCode(ErrCode), m_Description(Description) {
	}

	virtual int code(void) const {
		return m_ErrCode;
	}

	virtual const char *what(void) const throw() {
		return m_Description.c_str( );
	}

	virtual ~ListArrayException( ) throw() {
	}

	enum LISTARRAY_EXCEPTION { E_NO_MEMORY, E_BAD_INDEX, E_DATA_ERROR };
};

// forward declaration for 1-1 operator<< 
template<typename T, int ArraySize>
class ListArray;

template<typename T, int ArraySize>
std::ostream& operator<< (std::ostream& os, ListArray<T, ArraySize> const & rhs);

template <typename T, int ArraySize>
class ListArray {
public:

	ListArray( );	// default constructor
	~ListArray( );	// destructor

	// add data
	void insert(const T& value); // this adds data sorted
	void push_back(const T& value);
	void push_front(const T& value);

	// and some more
	size_t size(void) const;   // total number of items (not nodes)
	unsigned int find(const T& value) const;       // returns index, size (one past last) if not found

	friend std::ostream& operator<< <T, ArraySize>(std::ostream &os, ListArray<T, ArraySize> const & list);
	void dumpStats( );
private:
	class Node {
	public:
		Node( ) : pNext(nullptr), pPrev(nullptr), count(0) {
			for(int i = 0; i < ArraySize + 1; i++) { values[i] = 0x7FFFFFFF; }
		}
		void shiftRight(int startIndex );
		void split( );
		bool hasSpace( ) { return count < ArraySize; }

		Node *pNext;
		Node *pPrev;
		int count;
		T values[ArraySize+1];
	};

	void deleteNodes( );
	std::pair<Node *, int> findInsertSpot(const T &value);
	int binaryFind(Node *curr, const T &value) const;

	Node *head;
	Node *tail;
	bool isSorted;
};

#include "ListArray.hpp"

#endif // LARIAT_H