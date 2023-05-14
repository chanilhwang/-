#include <iostream>
#include <iomanip>

template <typename T, int ArraySize>
std::ostream& operator<<(std::ostream &os, ListArray<T, ArraySize> const & list) {
	for(typename ListArray<T, ArraySize>::Node *curr = list.head; curr != nullptr; curr = curr->pNext) {
		os << '[';
		int i = 0;
		for(; i < curr->count - 1; i++) {
			os << curr->values[i] << ", ";
		}
		os << curr->values[i] << ']' << std::endl;
	}
	return os;
}

template<typename T, int ArraySize>
void ListArray<T, ArraySize>::dumpStats( ) {
	int numNodes = 0;
	size_t items = size( );

	for(Node *curr = head; curr != nullptr; curr = curr->pNext) {
		numNodes++;
	}

	double ipn;
	if(numNodes) {
		ipn = static_cast<double>(items) / static_cast<double>(numNodes);
	}
	else {
		ipn = 0;
	}
	double util = ipn / static_cast<double>(ArraySize) * 100;

	std::cout << "Array Size: " << ArraySize << std::endl;
	std::cout << "Items: " << items << std::endl;
	std::cout << "Nodes: " << numNodes << std::endl;
	std::cout << "Average items per node: " << ipn << std::endl;
	std::cout << "Node utilization: " << std::setprecision(3) << util << "%" << std::endl;
	std::cout << std::endl;
}

template<typename T, int ArraySize>
inline void ListArray<T, ArraySize>::deleteNodes()
{
	while (head != nullptr)
	{
		Node* temp = head;
		head = head->pNext;
		delete temp;
	}
}

template<typename T, int ArraySize>
inline std::pair<typename ListArray<T, ArraySize>::Node*, int> ListArray<T, ArraySize>::findInsertSpot(const T & value)
{
	Node* target_Node = head;
	int target_count = 0;
	for (Node* curr = head; curr != nullptr; curr = curr->pNext)
	{
		if (curr->values[curr->count - 1] > value)
		{
			target_Node = curr;
			int low = 0;
			int high = curr->count - 1;
			int mid;
			while (low < high)
			{
				mid = (low + high) / 2;
				if (curr->values[mid] > value)
				{
					high = mid - 1;
				}
				else if (curr->values[mid] < value)
				{
					low = mid + 1;
				}
				else
				{
					target_count = mid;
					break;
				}
			}
			mid = (low + high) / 2;
			if (curr->values[low] < value)
			{
				target_count = low + 1;
			}
			else
			{
				target_count = low;
			}
			break;
		}
		else if (curr->pNext != nullptr && curr->pNext->values[0] > value)
		{
			if (curr->hasSpace())
			{
				target_Node = curr;
				target_count = curr->count;
				break;
			}
		}
		else
		{
			target_Node = curr;
			target_count = curr->count;
		}
	}
	return std::make_pair(target_Node, target_count);
}

template<typename T, int ArraySize>
inline int ListArray<T, ArraySize>::binaryFind(Node * curr, const T & value) const
{
	int low = 0;
	int high = curr->count - 1;
	if (curr->values[low] == value)
	{
		return low;
	}
	else if (curr->values[high] == value)
	{
		return high;
	}
	while (low <= high)
	{
		int mid = (low + high) / 2;
		if (curr->values[mid] > value)
		{
			high = mid - 1;
		}
		else if(curr->values[mid] < value)
		{
			low = mid + 1;
		}
		else
		{
			return mid;
		}
	}
	return -1;
}

template<typename T, int ArraySize>
inline ListArray<T, ArraySize>::ListArray()
	: head(nullptr), tail(nullptr), isSorted(true)
{
}

template<typename T, int ArraySize>
inline ListArray<T, ArraySize>::~ListArray()
{
	deleteNodes();
}

template<typename T, int ArraySize>
inline void ListArray<T, ArraySize>::insert(const T & value)
{
	if (head == nullptr)
	{
		head = new Node();
		tail = head;
		head->count++;
		head->values[0] = value;
	}
	else
	{
		std::pair<typename ListArray<T, ArraySize>::Node*, int> p = findInsertSpot(value);
		if (p.first->hasSpace())
		{
			p.first->shiftRight(p.second);
			p.first->values[p.second] = value;
			p.first->count++;
		}
		else
		{
			p.first->shiftRight(p.second);
			p.first->values[p.second] = value;
			p.first->count++;
			p.first->split();
			if (p.first == tail)
			{
				tail = tail->pNext;
			}
		}
	}
}

template<typename T, int ArraySize>
inline void ListArray<T, ArraySize>::push_back(const T & value)
{
	if (head == nullptr)
	{
		head = new Node();
		tail = head;
		tail->count++;
		tail->values[0] = value;
	}
	else
	{
		if (tail->hasSpace())
		{
			tail->values[tail->count] = value;
			tail->count++;
			if (tail->values[tail->count - 1] < tail->values[tail->count - 2])
			{
				isSorted = false;
			}
		}
		else
		{
			Node* back_node = new Node();
			back_node->count++;
			back_node->values[0] = value;
			back_node->pPrev = tail;
			tail->pNext = back_node;
			tail = back_node;
			if (tail->values[0] < tail->pPrev->values[ArraySize - 1])
			{
				isSorted = false;
			}
		}
	}
}

template<typename T, int ArraySize>
inline void ListArray<T, ArraySize>::push_front(const T & value)
{
	if (head == nullptr)
	{
		head = new Node();
		tail = head;
		head->count++;
		head->values[0] = value;
	}
	else
	{
		if (head->hasSpace())
		{
			head->shiftRight(0);
			head->values[0] = value;
			head->count++;
			if (head->values[0] > head->values[1])
			{
				isSorted = false;
			}
		}
		else
		{
			Node* front_node = new Node();
			front_node->count++;
			front_node->values[0] = value;
			front_node->pNext = head;
			head->pPrev = front_node;
			head = front_node;
			if (head->values[0] > head->pNext->values[0])
			{
				isSorted = false;
			}
		}
	}
}

template<typename T, int ArraySize>
inline size_t ListArray<T, ArraySize>::size(void) const
{
	size_t size = 0;
	for (Node* curr = head; curr != nullptr; curr = curr->pNext)
	{
		size += curr->count;
	}
	return size;
}

template<typename T, int ArraySize>
inline unsigned int ListArray<T, ArraySize>::find(const T & value) const
{
	if (head == nullptr)
	{
		return -1;
	}
	else if (isSorted)
	{
		int num_passed_items = 0;
		for (Node* curr = head; curr != nullptr; curr = curr->pNext)
		{
			int result = binaryFind(curr, value);
			if (result != -1)
			{
				return num_passed_items + result;
			}
			num_passed_items += curr->count;
		}
	}
	else
	{
		int num_passed_items = 0;
		for (Node* curr = head; curr != nullptr; curr = curr->pNext)
		{
			for (int i = 0; i < curr->count; i++)
			{
				if (curr->values[i] == value)
				{
					return num_passed_items;
				}
				num_passed_items++;
			}
		}
	}

	return static_cast<unsigned int>(size());
}

template<typename T, int ArraySize>
inline void ListArray<T, ArraySize>::Node::shiftRight(int startIndex)
{
	for (int i = 0; i < count - startIndex; i++)
	{
		values[count - i] = values[count - i - 1];
	}
}

template<typename T, int ArraySize>
inline void ListArray<T, ArraySize>::Node::split()
{
	Node* new_node = new Node();
	new_node->pNext = pNext;
	pNext = new_node;
	new_node->pPrev = this;
	
	int split_num = count / 2;

	count -= split_num;
	new_node->count += split_num;

	for (int i = 0; i < split_num; i++)
	{
		new_node->values[i] = values[count + i];
	}
}