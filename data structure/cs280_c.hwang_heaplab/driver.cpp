#include <iostream>
#include <random>

#define MAX_SIZE 100

template <typename T>
struct HeapArray {
	T* data;
	int size;

	HeapArray(int size) 
		: size(size)
	{
		data = new T[MAX_SIZE];
	}

	friend std::ostream& operator<<(std::ostream& io, const HeapArray& myarr) {
		int count = 0;
		int two_complement = 1;
		for (int i = 0; i < myarr.size; i++) {
			if (two_complement <= count) {
				io << std::endl;
				two_complement *= 2;
				count = 0;
			}
			io << myarr.data[i];
			io << ' ';
			count++;
		}
		io << std::endl;
		return io;
	}
};

int GetParentIndex(int index) {
	if (index == 0)
		return -1;
	return (index + 1) / 2 - 1;
}

template<typename T>
bool IsInHeapArray(int index, HeapArray<T>& arr) {
	return arr.size > index && index >= 0;
}

template<typename T>
bool IsParent(int index, HeapArray<T>& arr) {
	if (arr.size <= (index + 1) * 2 - 1 &&
		arr.size <= (index + 1) * 2) {
		return false;
	}
	return true;
}

template<typename T>
int FindLastParentIndex(HeapArray<T>& arr) {
	return GetParentIndex(arr.size - 1);
}

template<typename T>
void Swap(int index1, int index2, HeapArray<T>& arr) {
	T temp = arr.data[index1];
	arr.data[index1] = arr.data[index2];
	arr.data[index2] = temp;
}

template<typename T>
void Compare(int index, HeapArray<T>& arr) {
	int leftchild = (index + 1) * 2 - 1;
	int rightchild = (index + 1) * 2;
	int biggerchild = 0;
	if (IsInHeapArray(rightchild, arr)) {
		if (static_cast<int>(arr.data[leftchild]) > static_cast<int>(arr.data[rightchild]))
			biggerchild = leftchild;
		else
			biggerchild = rightchild;
	}
	else if (IsInHeapArray(leftchild, arr)) {
		biggerchild = leftchild;
	}
	else
		return;
	//compare
	if (static_cast<int>(arr.data[biggerchild]) > static_cast<int>(arr.data[index])) {
		Swap(index, biggerchild, arr);

		//Check Swapped item is parent
		if (IsParent(biggerchild, arr))
			Compare(biggerchild, arr);
	}
}

template<typename T>
void ReverseCompare(int index, HeapArray<T>& arr) {
	int leftchild = (index + 1) * 2 - 1;
	int rightchild = (index + 1) * 2;
	int smallchild = 0;
	if (IsInHeapArray(rightchild, arr)) {
		if (static_cast<int>(arr.data[leftchild]) < static_cast<int>(arr.data[rightchild]))
			smallchild = leftchild;
		else
			smallchild = rightchild;
	}
	else if (IsInHeapArray(leftchild, arr)) {
		smallchild = leftchild;
	}
	else
		return;
	//compare
	if (static_cast<int>(arr.data[smallchild]) < static_cast<int>(arr.data[index])) {
		Swap(index, smallchild, arr);

		//Check Swapped item is parent
		if (IsParent(smallchild, arr))
			ReverseCompare(smallchild, arr);
	}
}

template<typename T, typename func>
void Heapify(func comparefunc, HeapArray<T>& arr) {
	int index = FindLastParentIndex(arr);
	for (int i = index; i >= 0; i--) {
		comparefunc(i, arr);
	}
}

template<typename T, typename func>
void HeapSort(T arr[], func comparefunc, HeapArray<T> heap) {
	int size = heap.size;
	for (int i = 0; i < size; i++) {
		arr[i] = RemoveFromHeap(0, comparefunc, heap);
	}
}

template<typename T>
void InsertToHeap(const T& item, HeapArray<T>& arr) {
	int index = arr.size;
	arr.size++;
	arr.data[index] = item;
	int childindex = index;
	index = GetParentIndex(index);
	while(index != -1) {
		if (arr.data[childindex] > arr.data[index])
			Swap(childindex, index, arr);
		else
			break;
		childindex = index;
		index = GetParentIndex(index);
	}
}

//if index is bigger than arr's size, return -1
template<typename T, typename func>
T RemoveFromHeap(int index, func comparefunc, HeapArray<T>& arr) {
	if (!IsInHeapArray(index, arr))
		return static_cast<T>(-1);
	T result = arr.data[index];
	arr.data[index] = arr.data[arr.size - 1];
	arr.size--;
	comparefunc(index, arr);
	return result;
}

void test1() {
	std::cout << "--------------------------------test1------------------------------------" << std::endl;
	HeapArray<int> arr(10);
	for (int i = 0; i < 10; i++) {
		arr.data[i] = rand() % 100;
	}
	std::cout << "------------Before Heapifying---------------" << std::endl;
	std::cout << arr;
	Heapify(&Compare<int>, arr);
	std::cout << "------------After Heapifying---------------" << std::endl;
	std::cout << arr;
	std::cout << "--------------------------------------------------------------------------" << std::endl;
	std::cout << std::endl;

}

void test2() {
	std::cout << "--------------------------------test2------------------------------------" << std::endl;
	HeapArray<int> arr(10);
	for (int i = 0; i < 10; i++) {
		arr.data[i] = rand() % 100;
	}
	std::cout << "------------Before Reverse Heapifying---------------" << std::endl;
	std::cout << arr;
	Heapify(&ReverseCompare<int>, arr);
	std::cout << "------------After Reverse Heapifying---------------" << std::endl;
	std::cout << arr;
	std::cout << "--------------------------------------------------------------------------" << std::endl;
	std::cout << std::endl;

}

//Let size 10
void test3() {
	std::cout << "--------------------------------test3------------------------------------" << std::endl;
	int integer_arr[10];
	HeapArray<int> arr(10);
	for (int i = 0; i < 10; i++) {
		arr.data[i] = rand() % 100;
	}
	std::cout << "------------Before Heapifying---------------" << std::endl;
	std::cout << arr;
	Heapify(&Compare<int>, arr);
	std::cout << "------------Before HeapSort---------------" << std::endl;
	std::cout << arr;
	HeapSort(integer_arr, &(Compare<int>), arr);
	std::cout << "------------After HeapSort---------------" << std::endl;
	for (int i = 0; i < 10; i++) {
		std::cout << integer_arr[i] << ' ';
	}
	std::cout << std::endl;
	std::cout << "-------------------------------------------------------------------" << std::endl;
	std::cout << std::endl;

}

void test4() {
	std::cout << "--------------------------------test4------------------------------------" << std::endl;
	int integer_arr[10];
	HeapArray<int> arr(10);
	for (int i = 0; i < 10; i++) {
		arr.data[i] = rand() % 100;
	}
	std::cout << "------------Before Reverse Heapifying---------------" << std::endl;
	std::cout << arr;
	Heapify(&ReverseCompare<int>, arr);
	std::cout << "------------Before Reverse HeapSort---------------" << std::endl;
	std::cout << arr;
	HeapSort(integer_arr, &ReverseCompare<int>, arr);
	std::cout << "------------After Reverse HeapSort---------------" << std::endl;
	for (int i = 0; i < 10; i++) {
		std::cout << integer_arr[i] << ' ';
	}
	std::cout << std::endl;
	std::cout << "-------------------------------------------------------------------" << std::endl;
	std::cout << std::endl;

}

void test5() {
	std::cout << "--------------------------------test5------------------------------------" << std::endl;

	HeapArray<int> arr(10);
	for (int i = 0; i < 10; i++) {
		arr.data[i] = rand() % 100;
	}
	std::cout << "------------Before Heapifying---------------" << std::endl;
	std::cout << arr;
	Heapify(&ReverseCompare<int>, arr);
	std::cout << "------------After Reverse Heapifying---------------" << std::endl;
	std::cout << arr;
	Heapify(&Compare<int>, arr);
	std::cout << "------------After Heapifying---------------" << std::endl;
	std::cout << arr;
	std::cout << "-------------------------------------------------------------------" << std::endl;
	std::cout << std::endl;

}

void test6() {
	std::cout << "--------------------------------test6------------------------------------" << std::endl;

	HeapArray<int> arr(10);
	for (int i = 0; i < 10; i++) {
		arr.data[i] = rand() % 100;
	}
	int insertion_num = 88;
	Heapify(&Compare<int>, arr);
	std::cout << "------------Before Inserting " << insertion_num << "---------------" << std::endl;
	std::cout << arr;
	InsertToHeap(insertion_num, arr);
	std::cout << "------------After Inserting " << insertion_num << "---------------" << std::endl;
	std::cout << arr;
	std::cout << "------------------------------------------------------------------" << std::endl;
	std::cout << std::endl;

}

void test7() {
	std::cout << "--------------------------------test7------------------------------------" << std::endl;

	HeapArray<int> arr(10);
	for (int i = 0; i < 10; i++) {
		arr.data[i] = rand() % 100;
	}
	Heapify(&Compare<int>, arr);
	std::cout << "------------Before Removing---------------" << std::endl;
	std::cout << arr;
	int removal_num = 1;
	if (RemoveFromHeap(removal_num, &Compare<int>, arr) == -1) {
		std::cout << "Failed to remove " << removal_num << std::endl;
		return;
	}
	std::cout << "------------After Removing "<< removal_num << "---------------" << std::endl;
	std::cout << arr;
	std::cout << "--------------------------------------------------------------------" << std::endl;
	std::cout << std::endl;

}

//check my heap with char instead of integer
void test8() {
	std::cout << "--------------------------------test8------------------------------------" << std::endl;
	HeapArray<char> arr(10);
	for (int i = 0; i < 10; i++) {
		arr.data[i] = rand() % 26 + 'a';
	}
	std::cout << "------------Before Heapifying---------------" << std::endl;
	std::cout << arr;
	Heapify(&ReverseCompare<char>, arr);
	std::cout << "------------After Reverse Heapifying---------------" << std::endl;
	std::cout << arr;
	Heapify(&Compare<char>, arr);
	std::cout << "------------After Heapifying---------------" << std::endl;
	std::cout << arr;
	RemoveFromHeap(0, &Compare<char>, arr);
	std::cout << "------------After Remove 0---------------" << std::endl;
	std::cout << arr;
	RemoveFromHeap(3, &Compare<char>, arr);
	std::cout << "------------After Remove 3---------------" << std::endl;
	std::cout << arr;
	RemoveFromHeap(5, &Compare<char>, arr);
	std::cout << "------------After Remove 5 again---------------" << std::endl;
	std::cout << arr;
	char character_arr[7];
	HeapSort(character_arr, &Compare<char>, arr);
	std::cout << "------------After HeapSort---------------" << std::endl;
	for (int i = 0; i < 7; i++) {
		std::cout << character_arr[i] << ' ';
	}
	std::cout << std::endl;
	std::cout << "--------------------------------------------------------------------" << std::endl;
	std::cout << std::endl;

}

int main(int argc, char* argv[]) {
	test1();
	test2();
	test3();
	test4();
	test5();
	test6();
	test7();
	test8();
	return 0;
}