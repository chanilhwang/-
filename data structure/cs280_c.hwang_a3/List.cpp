/*------------------------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: list.cpp
Purpose: to make list using recurssion
Project: assignment 3
Author: c.hwang
Created: 2019/04/17
------------------------------------------------------------------------*/
#include <iostream>
#include "List.h"

void List::Reverse()
{
	if(pHead)
		Reverse(pHead);
}

void List::Print()
{
	pHead->Print();
}

void List::Reverse(Node * node)
{
	if (node->pNext) {
		Reverse(node->pNext);
		node->pNext->pNext = node;
		node->pNext = nullptr;
	}
	else
		pHead = node;
}

List::Node::~Node()
{
	if (pNext)
		pNext->~Node();
	delete this;
}

void List::Node::Print()
{
	std::cout << value << std::endl;
	if(pNext)
		pNext->Print();
}