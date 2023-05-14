#include <iostream> // cout, endl

/******************************************************************************/
/*!
    Swaps two objects. There is no return value but the two objects are
    swapped in place.
  
  \param left
    The first object to swap.
  
  \param right
    The second object to swap.
*/
/******************************************************************************/
template <typename T> 
void swap(T& left, T& right)
{
  T temp(right); // need a temporary copy
  right = left;
  left = temp;
}


