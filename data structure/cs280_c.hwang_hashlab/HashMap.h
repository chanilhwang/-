#pragma once
#include <iostream>

constexpr unsigned int NUMSLOT = 7;

template<typename T1, typename T2>
class HashMap {
public:
	class Slot {
	public:
		class Node {
		public:
			Node() = delete;
			Node(T1 key, T2 value);
			Node* push(T1 key, T2 value);
			bool IsSameKey(T1 key) const;
			T2& Getdata();

			Node* pNext;
		private:
			T1 key;
			T2 data;
		};

		Slot();
		~Slot();
		void Clear();
		Node* push(T1 key, T2 data);
		Node* find(T1 key) const;
	private:
		Node* head;
	};

	HashMap();
	unsigned int HashFunction(T1 key);
	T2& operator[](T1 key);
	void Clear();

private:
	Slot slot[NUMSLOT];
};

#include "HashMap.hpp"