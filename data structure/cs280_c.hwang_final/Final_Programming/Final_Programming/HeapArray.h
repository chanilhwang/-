#pragma once
#include "Cell.h"

class HeapArray {
public:
	HeapArray();
	~HeapArray();
	void insert(Cell* item);
	Cell* remove(int index);
	void heapify();
	bool IsEmpty() {
		return size == 0;
	}
private:
	void ExtendSize(int size2extend);
	int GetParentIndex(int index) const;
	int FindLastParentIndex() const {
		return GetParentIndex(size - 1);
	}
	void Compare(int index);
	void Swap(int index, int index2);
	bool IsParent(int index) const;
	bool IsInHeapArray(int index) const {
		return size > index && index >= 0;
	}

	Cell** data;
	int size;
	int extended_size;
};