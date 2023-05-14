#include "HeapArray.h"

namespace {
	constexpr int INITIAL_SIZE = 5;
	constexpr int SIZE_TO_INCREASE_AT_ATIME = 3;
}

HeapArray::HeapArray()
	: size(0), extended_size(INITIAL_SIZE)
{
	data = new Cell*[INITIAL_SIZE];
}

HeapArray::~HeapArray() {
	delete[] data;
}

void HeapArray::insert(Cell* item) {
	int index = size++;
	if (extended_size <= size)
		ExtendSize(SIZE_TO_INCREASE_AT_ATIME);
	data[index] = item;
	heapify();
}

Cell* HeapArray::remove(int index) {
	if (!IsInHeapArray(index))
		return nullptr;
	Cell* result = data[index];
	data[index] = data[size-- - 1];
	Compare(index);
	return result;
}

void HeapArray::heapify() {
	int index = FindLastParentIndex();
	for (int i = index; i >= 0; i--) {
		Compare(i);
	}
}

void HeapArray::ExtendSize(int size2extend) {
	Cell** new_data = new Cell*[size + size2extend];
	for (int i = 0; i < size; i++) {
		new_data[i] = data[i];
	}
	delete[] data;
	data = new_data;
}

int HeapArray::GetParentIndex(int index) const {
	return (index + 1) / 2 - 1;
}

void HeapArray::Compare(int index) {
	//find bigger child
	int leftchild = index * 2 + 1,
		rightchild = index * 2 + 2,
		smallerchild = 0;
	if (IsInHeapArray(rightchild)) {
		if (data[leftchild]->f_cost < data[rightchild]->f_cost)
			smallerchild = leftchild;
		else
			smallerchild = rightchild;
	}
	else if (IsInHeapArray(leftchild))
		smallerchild = leftchild;
	else
		return;
	//compare
	if (data[smallerchild]->f_cost < data[index]->f_cost) {
		Swap(index, smallerchild);
		//check swapped item is parent
		if (IsParent(smallerchild))
			Compare(smallerchild);
	}
}

void HeapArray::Swap(int index1, int index2) {
	Cell* temp = data[index1];
	data[index1] = data[index2];
	data[index2] = temp;
}

bool HeapArray::IsParent(int index) const {
	if (IsInHeapArray(index * 2 + 1)) {
		return true;
	}
	return false;
}