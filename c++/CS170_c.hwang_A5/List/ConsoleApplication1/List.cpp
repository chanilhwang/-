#include "List.h"  // List class
#include <iomanip> // setw


namespace CS170
{

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// public methods in this order: 
// Constructors (default, copy, non-defaults), destructor, operators, others
List::List() : m_pHead(NULL), m_pTail(NULL), m_size(0)
{
	s_m_ObjectCount += 1;
}
List::List(const int* array, int size) : m_pHead(NULL), m_pTail(NULL), m_size(0)
{
	for (int Index = 0; Index < size; Index++)
		PushBack(array[Index]);
	s_m_ObjectCount += 1;
}
List::List(const List& _List) : m_pHead(NULL), m_pTail(NULL), m_size(0)
{
	*this = _List;
	s_m_ObjectCount += 1;
}
List::~List()
{
  Clear();
}

List& List::operator=(const List& _List)
{
  Clear();
  for(int Index = 0; Index < _List.size(); Index++)
  {
    PushBack(_List[Index]);
  }
  return *this;
}

List& List::operator+=(const List& _List)
{
	for (int Index = 0; Index < _List.size(); Index++)
	{
		PushBack(_List[Index]);
	}
	return *this;
}

List List::operator+(const List& _List) const
{
	List* List = new CS170::List;
	*List += *this;
	*List += _List;
	return *List;
}

int List::operator[](int num) const
{
	Node* temp = m_pHead;
	for (int Index = 0; Index < num; Index++)
		temp = temp->pNext;
	return temp->data;
}

int& List::operator[](int num)
{
	Node* temp = m_pHead;
	for (int Index = 0; Index < num; Index++)
		temp = temp->pNext;
	return temp->data;
}

void List::PushFront(int _val)
{
	Node* temp = MakeNode(_val);
	if (m_pHead == NULL)
	{
		m_pHead = m_pTail = temp;
	}
	else
	{
		temp->pNext = m_pHead;
		m_pHead = temp;
	}
	m_size += 1;
}

void List::PushBack(int _val)
{
	Node* temp = MakeNode(_val);
	if (!m_pTail)
		m_pHead = m_pTail = temp;
	else
	{
		m_pTail->pNext = temp;
		m_pTail = temp;
	}
	m_size += 1;
}

int List::PopFront()
{
	if (IsEmpty())
		return -1;

	Node* temp = m_pHead;
	m_pHead = m_pHead->pNext;
	delete temp;

	int sizetemp = m_size;
	m_size -= 1;

	return sizetemp;
}

int List::size() const
{
	return m_size;
}

bool List::IsEmpty() const
{
	if (!m_size)
		return true;
	return false;
}

void List::Clear()
{
	for(int Index = 0; Index < m_size; Index++)
	{
		PopFront();
	}
	m_size = 0;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// private methods
int List::s_m_ObjectCount = 0;
int List::ObjectCount(void)
{
	return s_m_ObjectCount;
}

CS170::List::Node* List::MakeNode(int data) const
{
	Node* temp = new Node;
	temp->data = data;
	temp->pNext = NULL;
	return temp;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// non-member methods

std::ostream& operator<<(std::ostream& os, const List& list)
{
    // Start at the first node
  List::Node *pNode = list.m_pHead;
    // Until we reach the end of the list
  while (pNode != 0)
  {
    os << std::setw(4) << pNode->data; // print the data in this node
    pNode = pNode->pNext;               // move to the next node
  }
  os << std::endl; // extra newline for readability
  return os;
}

} //namespace CS170
