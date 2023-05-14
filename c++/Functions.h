//---------------------------------------------------------------------------
#ifndef FUNCTIONS_H
#define FUNCTIONS_H
//---------------------------------------------------------------------------

namespace CS170
{
  template <typename T1, typename T2>
  int count(T1 begin, T1 end, const T2& value);
  template <typename T1, typename T2>
  T2 copy(T1 begin1, T1 end1, T2 begin2);
  template <typename T> 
  void display(T begin, T end);
  template <typename T1, typename T2>
  bool equal(T1 begin1, T1 end2, T2 begin);
  template <typename T1, typename T2>
  void fill(T1 begin, T1 end, const T2& value);
  template <typename T1, typename T2>
  T1 find(T1 begin, T1 end, const T2& value);
  template <typename T>
  T max_element(T begin, T end);
  template <typename T>
  T min_element(T begin, T end);
  template <typename T1, typename T2>
  T1 remove(T1 begin, T1 end, const T2& value);
  template <typename T1, typename T2>
  void replace(T1 begin, T1 end, const T2& oldItem, const T2& newItem);
  template <typename T>
  int sum(T begin, T end);
  template <typename T> 
  void swap(T& left, T& right);
  template <typename T1, typename T2> 
  void swap_ranges(T1 begin1, T1 end1, T2 begin2);
  /* 
   *  Other template function declarations for count, remove, replace, etc.
   *  go here. Make sure all of your declarations are sorted in alphabetical
   *  order. This includes putting the swap function above in the proper
   *  position.
   *
   */ 
  

}
  #include "Functions.cpp"
#endif
//---------------------------------------------------------------------------
