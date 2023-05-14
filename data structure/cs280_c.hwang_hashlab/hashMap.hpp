#include "HashMap.h"

template<typename T1, typename T2>
HashMap<T1, T2>::HashMap()
{}

template<typename T1, typename T2>
unsigned int HashMap<T1, T2>::HashFunction(T1 key) {
	return key % NUMSLOT;
}

template<typename T1, typename T2>
inline T2& HashMap<T1, T2>::operator[](T1 key) {
	Slot& target_slot = slot[HashFunction(key)];
	typename Slot::Node* target_node = target_slot.find(key);
	if (target_node)
		return target_node->Getdata();
	else {
		target_node = target_slot.push(key, 0);
		return target_node->Getdata();
	}
}

template<typename T1, typename T2>
void HashMap<T1, T2>::Clear()
{
	for (int i = 0; i < NUMSLOT; i++) {
		slot[i].Clear();
	}
}

template<typename T1, typename T2>
HashMap<T1, T2>::Slot::Slot()
	:head(nullptr)
{}

template<typename T1, typename T2>
HashMap<T1, T2>::Slot::~Slot()
{
	Clear();
}

template<typename T1, typename T2>
void HashMap<T1, T2>::Slot::Clear()
{
	Node* curr = head;
	while (curr) {
		Node* temp = curr;
		curr = curr->pNext;
		delete temp;
	}
	head = nullptr;
}

template<typename T1, typename T2>
typename HashMap<T1, T2>::Slot::Node* HashMap<T1, T2>::Slot::push(T1 key, T2 data) {
	if (head)
		return head->push(key, data);
	else {
		head = new Node(key, data);
		return head;
	}
}

template<typename T1, typename T2>
HashMap<T1, T2>::Slot::Node::Node(T1 pkey, T2 value)
	: key(pkey), data(value), pNext(nullptr)
{}

template<typename T1, typename T2>
typename HashMap<T1, T2>::Slot::Node* HashMap<T1, T2>::Slot::Node::push(T1 pkey, T2 value) {
	if (pNext)
		return pNext->push(pkey, value);
	else {
		pNext = new Node(pkey, value);
		return pNext;
	}
}

template<typename T1, typename T2>
bool HashMap<T1, T2>::Slot::Node::IsSameKey(T1 pkey) const
{
	return key == pkey;
}

template<typename T1, typename T2>
T2& HashMap<T1, T2>::Slot::Node::Getdata()
{
	return data;
}

template<typename T1, typename T2>
inline typename HashMap<T1,T2>::Slot::Node* HashMap<T1, T2>::Slot::find(T1 key) const {
	Node* curr = head;
	while (curr) {
		if (curr->IsSameKey(key))
			return curr;
		curr = curr->pNext;
	}
	return nullptr;
}