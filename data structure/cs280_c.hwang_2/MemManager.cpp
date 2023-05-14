/*------------------------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name:	MemManager.cpp
Purpose:	To manage the memory
Project:	Assignment2
Author:		c.hwang(Hwang Chan Il)
Created:	2019-04-03
------------------------------------------------------------------------*/
#include <iostream>
#include <algorithm>
#include "MemManager.h"

constexpr unsigned int PAGE_SIZE = 256;

#undef new

MemManager gMemManager;

MemManager::MemManager()
{
	pPageHead = reinterpret_cast<Page*>(new char[PAGE_SIZE]);
	new(pPageHead) Page(PAGE_SIZE);
	pPageHead->pNext = reinterpret_cast<Page*>(new char[PAGE_SIZE]);
	new(pPageHead->pNext) Page(PAGE_SIZE);
}
MemManager::~MemManager() 
{
	std::for_each(allocs.begin(), allocs.end(), [](std::pair<void*, std::string> alloc) {
		std::cout << "Unfreed memory from " << alloc.second << std::endl;
	});
	for (Page *currPage = pPageHead; currPage != nullptr;)
	{
		Page* temp = currPage;
		currPage = currPage->pNext;
		delete reinterpret_cast<char*>(temp);
	}
}

void* MemManager::Alloc(std::size_t size, const std::string tag) 
{
	std::cout << "Alloc (" << size << ") " << tag << std::endl;
	size_t IsSizeMultipleOf4 = size % 4;
	size = (IsSizeMultipleOf4 == 0) ? size : size + (4 - IsSizeMultipleOf4);
	for (Page* currPage = pPageHead; currPage != nullptr; currPage = currPage->pNext)
	{
		for (Node* currNode = currPage->GetHeadNode(); currNode != nullptr; currNode = currNode->pNext)
		{
			if (currNode->isAvailable == true)
			{
				size_t sizediffer = currNode->size - size;
				if (sizediffer < sizeof(Node) + 4 && currNode->size >= size)
				{
					currNode->isAvailable = false;
					allocs.insert(std::make_pair(currNode->GetUserData(), tag));
					return currNode->GetUserData();
				}
				else if (currNode->size > size)
				{
					currNode->Split(size);
					allocs.insert(std::make_pair(currNode->GetUserData(), tag));
					return currNode->GetUserData();
				}
			}
		}
	}
	std::cout << "Out of Memory.  This should probably throw an exception" << std::endl;
	return nullptr;
}

void MemManager::Free(void* ptr) 
{
	if (!allocs.empty() && allocs.find(ptr) != allocs.end()) 
	{
		std::cout << "Freeing " << allocs[ptr] << std::endl;

		char* nodeAddress = reinterpret_cast<char*>(ptr) - Node::GetHeaderSize();
		Node* node = reinterpret_cast<Node*>(nodeAddress);

		if (!node->BoundsCheck()) {
			std::cout << "Buffer was overflown from object allocated at " << allocs[ptr] << std::endl;
		}
		allocs.erase(ptr);
		node->Merge();
	}
	else {
		free(ptr);
	}
}

void MemManager::WriteMemoryInfo(bool memDump) 
{
	for (Page *currPage = pPageHead; currPage != nullptr; currPage = currPage->pNext)
	{
		currPage->WritePageInfo(memDump); 
	} 
	std::cout << std::endl; 
}

void MemManager::Page::WritePageInfo(bool memDump)
{
	std::cout.width(40);
	std::cout.fill('-');
	std::cout << "" << "Page (" << PAGE_SIZE << ")";
	std::cout.width(40);
	std::cout << "" << std::endl;

	for (MemManager::Node *currNode = GetHeadNode(); currNode != nullptr; currNode = currNode->pNext)
	{
		currNode->WriteNodeInfo(memDump);
	}
}

void MemManager::Node::WriteNodeInfo(bool memDump) {
	if (isAvailable == false && memDump == true) {
		std::cout.fill('0');
		int* dataPtr = reinterpret_cast<int *>(reinterpret_cast<char*>(this));
		unsigned int i = 0;
		std::cout.width(8);
		std::cout << std::hex << dataPtr[i++] << ' ';
		for (; i < (sizeof(Node) + size) / sizeof(int) - 1; i++)
		{
			if (i % 8 == 0) 
			{
				std::cout << std::endl;
			} 
			std::cout.width(8);
			std::cout << dataPtr[i] << ' ';
		} 
		std::cout.width(8);
		std::cout << dataPtr[i] << ' ' << std::endl << std::dec << std::endl;
		std::cout.fill(' ');
	}
	else 
	{
		std::cout << std::boolalpha;
		std::cout << "Node (" << size << ")" << " Available = " << isAvailable << std::endl << std::endl;
	}
}

MemManager::Node::Node(size_t size)
	:isAvailable(true), pNext(nullptr), pPrev(nullptr), size(size), preIndicator(INDICATOR)
{
	*GetPostIndicatorPtr() = INDICATOR;
}

MemManager::Page::Page(size_t size)
	:pNext(nullptr)
{
	new(GetHeadNode()) Node(size - sizeof(Page)+ sizeof(Node));
}

MemManager::Node* MemManager::Node::Split(std::size_t newsize)
{
	isAvailable = false;
	int* newPostIndicator = reinterpret_cast<int*>(reinterpret_cast<char*>(this) + GetHeaderSize() + newsize);
	*newPostIndicator = INDICATOR;
	Node* newNode = reinterpret_cast<Node*>(reinterpret_cast<char*>(newPostIndicator) + sizeof(postIndicator));
	new(newNode) Node(size - newsize - sizeof(Node));
	size = newsize;
	pNext = newNode;
	newNode->pPrev = this;

	return newNode;
}

void MemManager::Node::Merge()
{
	isAvailable = true;
	if (pPrev != nullptr && pPrev->isAvailable)
	{
		pPrev->size += size + static_cast<int>(sizeof(Node));
		if (pNext != nullptr)
		{
			if (pNext->isAvailable)
			{
				pPrev->size += pNext->size + static_cast<int>(sizeof(Node));
				pPrev->pNext = nullptr;
			}
			else
			{
				pPrev->pNext = pNext;
				pNext->pPrev = pPrev;
			}
		}
		else
		{
			pPrev->pNext = nullptr;
		}
	}
	else if (pNext != nullptr && pNext->isAvailable)
	{
		size += pNext->size + sizeof(Node);
		if (pNext->pNext != nullptr)
		{
			pNext = pNext->pNext;
			pNext->pPrev = this;
		}
		else
			pNext = nullptr;
	}
}

void *operator new[](std::size_t size, const std::string fileName, int lineNum) 
{ 
	unsigned int folderIndex = static_cast<unsigned int>(fileName.find_last_of('\\') + 1);
	std::string trackName = fileName.substr(folderIndex, fileName.length() - folderIndex) + ':' + std::to_string(lineNum); 
	return gMemManager.Alloc(size, trackName);
}

void *operator new(std::size_t size, const std::string fileName, int lineNum)
{ 
	unsigned int folderIndex = static_cast<unsigned int>(fileName.find_last_of('\\') + 1);
	std::string trackName = fileName.substr(folderIndex, fileName.length() - folderIndex) + ':' + std::to_string(lineNum);
	return gMemManager.Alloc(size, trackName);
}

void operator delete(void *ptr, const std::string, int) noexcept 
{
	gMemManager.Free(ptr);
}

void operator delete(void *ptr) noexcept 
{
	gMemManager.Free(ptr);
}

void operator delete(void *ptr, std::size_t) noexcept 
{
	gMemManager.Free(ptr);
}

void operator delete[](void *ptr) noexcept
{ 
	gMemManager.Free(ptr);
} 

void operator delete[](void *ptr, std::size_t) noexcept
{
	gMemManager.Free(ptr);
}