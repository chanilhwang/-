#include <iostream>
#include <math.h>
#include "ChHashTable.h"

template<typename T>
ChHashTable<T>::ChHashTable(const HTConfig& _config, ObjectAllocator* _allocator)
	: config(_config), slots(new ChHTHeadNode[_config.initialTableSize])
{
	stats.allocator = _allocator;
	stats.tableSize = config.initialTableSize;
	stats.hashFunc = config.hashFunc;
}

template<typename T>
ChHashTable<T>::~ChHashTable() {
	clear();
	delete[] slots;
}

template<typename T>
void ChHashTable<T>::insert(const char* key, const T& data) {
	//check load factor
	double load_factor = static_cast<double>(stats.count + 1) / stats.tableSize;
	if(load_factor > config.maxLoadFactor) {
		//calculate new size
		double factor = std::ceil(stats.tableSize * config.growthFactor);
		unsigned new_size = GetClosestPrime(static_cast<unsigned>(factor));
		ChHTHeadNode* old_slots = slots;
		slots = new ChHTHeadNode[new_size];
		//re-inserting all nodes
		//I don't need to walk all nodes so it need to be changed
		for(unsigned int i = 0; i < stats.tableSize; i++) {
			ChHTNode* node = old_slots[i].nodes;
			while(node) {
				//find index using hashfunc
				int index = stats.hashFunc(node->key, new_size);
				//store node in temp
				ChHTNode* temp = node;
				node = node->next;
				//push the node into new slot
				temp->next = slots[index].nodes;
				slots[index].nodes = temp;
				slots[index].count++;
			}
			old_slots[i].nodes = 0;
		}
		delete[] old_slots;
		stats.tableSize = new_size;
		//update stats
		stats.expansions++;
	}
	//find index using hashfunc
	unsigned int index = stats.hashFunc(key, stats.tableSize);
	//check duplicate node
	ChHTNode* node = find_node(index, key);
	if(node) {
		throw HashTableException(HashTableException::E_DUPLICATE, 
		std::string("Key ") + key + std::string(" is duplicated"));
	}
	//allocate and initialize new node
	if (stats.allocator)
		node = static_cast<ChHTNode*>(stats.allocator->Allocate());
	else
		node = new ChHTNode(data);
	strcpy(node->key, key);
	//push the node into slot
	node->next = slots[index].nodes;
	slots[index].nodes = node;
	slots[index].count++;
	//update stats
	stats.count++;
}

template<typename T>
void ChHashTable<T>::remove(const char* key) {
	//find index using hashfunc
	unsigned int index = stats.hashFunc(key, stats.tableSize);
	//check item is in
	ChHTNode* prev_node = 0;
	ChHTNode* node = find_node(index, key, prev_node);
	if(!node) {
		throw HashTableException(HashTableException::E_ITEM_NOT_FOUND, 
		std::string("Key ") + key + std::string(" is not found"));
	}
	//remove item
	if(prev_node)
		prev_node->next = node->next;
	else
		slots[index].nodes = node->next;
	if(stats.allocator)
		stats.allocator->Free(node);
	else
		delete node;
	//update stats
	slots[index].count--;
	stats.count--;
}

template<typename T>
const T& ChHashTable<T>::find(const char* key) const {
	//find index using hashfunc
	unsigned int index = stats.hashFunc(key, stats.tableSize);
	//check item is in
	ChHTNode* node = find_node(index, key);
	if(!node) {
		throw HashTableException(HashTableException::E_ITEM_NOT_FOUND, 
		std::string("Key ") + key + std::string(" is not found"));
	}
	return node->data;
}

template<typename T>
void ChHashTable<T>::clear() {
	for(unsigned int i = 0; i < stats.tableSize; i++) {
		ChHTNode* node = slots[i].nodes;
		while(node) {
			//remove item
			ChHTNode* temp = node;
			node = node->next;
			if(stats.allocator)
				stats.allocator->Free(temp);
			else
				delete temp;
		}
		slots[i].count = 0;
		slots[i].nodes = 0;
	}
}

template<typename T>
HTStats ChHashTable<T>::getStats() const {
	return stats;
}

template<typename T>
const typename ChHashTable<T>::ChHTHeadNode* ChHashTable<T>::getTable() const {
	return slots;
}

template<typename T>
typename ChHashTable<T>::ChHTNode* ChHashTable<T>::find_node(int index, const char* key) const {
	ChHTNode* node = slots[index].nodes;
	while(node) {
		stats.probes++;
		if(strcmp(node->key, key) == 0) {
			return node;
		}
		node = node->next;
	}
	stats.probes++;
	return node;
}

template<typename T>
typename ChHashTable<T>::ChHTNode* ChHashTable<T>::find_node(int index, const char* key, ChHTNode*& prev_node) const {
	ChHTNode* node = slots[index].nodes;
	while(node) {
		stats.probes++;
		if(strcmp(node->key, key) == 0) {
			return node;
		}
		prev_node = node;
		node = node->next;
	}
	stats.probes++;
	return node;
}