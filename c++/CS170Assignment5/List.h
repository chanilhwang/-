#ifndef LIST_H
#define LIST_H
#include <iostream> // ostream

//! The namespace for the CS170 class
namespace CS170
{
  class List
  {
    public:
        // Three constructors
        // Destructor

        // Six methods:
        //   PushFront, adds the item to the front of the list
        //   PushBack, adds the item to the end of the list
        //   PopFront, removes the first item in the list
        //   size, returns the number of items in the list
        //   IsEmpty, returns true if IsEmpty, else false
        //   Clear, clears the list (removes all of the nodes)



        // Five operators: 
        //   operator=
        //   operator+=
        //   operator+
        //   operator[] (2 of these)

        // Output operator for printing lists (<<)
      friend std::ostream & operator<<(std::ostream & os, const List& list);

        // Returns the number of Lists that have been created
      static int ObjectCount(void);

    private:
        // used to build a linked list of integers
      struct Node
      {
        Node *pNext;  // pointer to the next Node
        int   data;   // the actual data in the node
      };

      Node* m_pHead; // pointer to the head of the list
      Node* m_pTail; // pointer to the last node
      int   m_size;   // number of items on the list

      static int s_m_ObjectCount;     // number of Lists created
      Node* MakeNode(int data) const; // allocate node, initialize data/next
  };

} // namespace CS170

#endif
////////////////////////////////////////////////////////////////////////////////
