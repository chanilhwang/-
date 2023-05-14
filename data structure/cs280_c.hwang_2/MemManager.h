/*------------------------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name:	MemManager.h
Purpose:	To manage the memory
Project:	Assignment2
Author:		c.hwang(Hwang Chan Il)
Created:	2019-04-03
------------------------------------------------------------------------*/
#pragma once
#include <map>
#include <string>

class MemManager
{
public:
	MemManager();
	~MemManager();

	void* Alloc(std::size_t size, const std::string tag);
	void Free(void* ptr);
	void WriteMemoryInfo(bool memDump = false);

private:
	struct Node 
	{
		static const int INDICATOR = 0xDEADBEEF;
		Node(size_t size);

		static int GetHeaderSize() {
			return sizeof(Node) - sizeof(postIndicator);
		}
		void *GetUserData() {
			return reinterpret_cast<char*>(this) + GetHeaderSize();
		}
		int *GetPostIndicatorPtr() {
			return reinterpret_cast<int*>(reinterpret_cast<char*>(this) + GetHeaderSize() + size);
		}
		bool BoundsCheck() {
			return (preIndicator == INDICATOR && *GetPostIndicatorPtr() == INDICATOR);
		}
		void WriteNodeInfo(bool memDump);
		Node* Split(std::size_t size);
		void Merge();

		bool isAvailable;
		Node* pNext;
		Node* pPrev;
		size_t size;
		int preIndicator;
		// User Data
		int postIndicator;
	};
	struct Page
	{
		Page(size_t size);
		void WritePageInfo(bool memDump);
		Node* GetHeadNode() {
			return reinterpret_cast<Node*>(reinterpret_cast<char*>(this)+ sizeof(Page));
		}
		Page* pNext;
	};
	std::map<void*, std::string> allocs;
	Page* pPageHead;
};

void *operator new[](std::size_t size, const std::string fileName, int lineNum);
void *operator new(std::size_t size, const std::string fileName, int lineNum);
void operator delete(void *ptr, const std::string, int) noexcept;
void operator delete(void *ptr) noexcept;
void operator delete(void *ptr, std::size_t) noexcept;
void operator delete[](void *ptr) noexcept;
void operator delete[](void *ptr, std::size_t) noexcept;

#define new new(__FILE__,__LINE__)