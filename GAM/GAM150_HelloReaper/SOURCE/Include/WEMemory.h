#include <map>
#include <string>
#include <memory>
#include <limits>
#include <cstdlib>


//template <typename T>
//class CustomAllocator : public std::allocator<T>
//{
//public:
//  CustomAllocator(void){}
//  CustomAllocator(const CustomAllocator&){}
//  ~CustomAllocator(void){}
//
//  CustomAllocator& operator=(const CustomAllocator&){}
//  template <typename U>
//  CustomAllocator(const CustomAllocator<U>&){}
//
//  // Type Definitions
//  typedef T value_type;
//  typedef T* pointer;
//  typedef const T* const_pointer;
//  typedef T& reference;
//  typedef const T& const_reference;
//  typedef std::size_t size_type;
//  typedef std::ptrdiff_t difference_type;
//
//  //Rebind allocator to type U
//  //Used for getting an allocator of a different type
//  template <typename U>
//  struct rebind
//  {
//    typedef CustomAllocator<U>  other;
//  };
//  const_pointer address(const_reference value) const
//  {
//    return &value;
//  }
//  pointer address(reference value) const
//  {
//    return &value;
//  }
//
//  size_type max_size() const
//  {
//    unsigned maxUnsigned = 0xFFFFFFFF;
//    return maxUnsigned / sizeof(T);
//  }
//  pointer allocate(size_type num)
//  {
//    if (num == 0)
//      return NULL;
//
//    //if size if too big
//    if (num > max_size())
//      throw std::length_error("CustomAllocator Too Many Elements");
//
//    void* ptr = malloc(num * sizeof(T));
//    if (!ptr)
//      throw std::bad_alloc();
//
//    return static_cast<pointer>(ptr);
//
//  }
//  pointer allocate(const size_t n, const void*)
//  {
//    return allocate(n);
//  }
//
//  void deallocate(pointer ptr, size_type /*num*/)
//  {
//    free(ptr);
//  }
//
//  template<class U, class... Args>
//  void construct(U* ptr, Args&&... args)
//  {
//    ::new((void*)ptr) U(std::forward<Args>(args)...);
//  }
//  template <class U>
//  void destroy(U* ptr)
//  {
//    ptr->~U();
//    ptr = 0;
//  }
//
//private:
//};
//
//template <typename T1, typename T2>
//bool operator==(const CustomAllocator<T1>&, const CustomAllocator<T2>&)
//{
//  return true;
//}
//template <typename T1, typename T2>
//bool operator !=(const CustomAllocator<T1>&, const CustomAllocator<T2>&)
//{
//  return false;
//}
//
//
//
//class AllocManager
//{
//public:
//  AllocManager(void);
//  ~AllocManager(void);
// 
//  void Alloc(void* data, void* returnAddress, size_t size, bool isArray);
//  void DeAlloc(void* ptr, bool isArray);
//
//
//private:
//  struct AllocInfo
//  {
//    void*       data;
//    void*       returnAddress;
//    size_t      size;
//    bool        isArray;
//    bool        isFree;
//  };
//  struct ReturnInfo
//  {
//    std::string fileName;
//    std::string function;
//    unsigned    lineNumber;
//  };
//  typedef std::map< void*, AllocInfo, std::less<void*>, CustomAllocator<AllocInfo> > AllocMap;
//  typedef AllocMap::iterator                                                         AllocItor;
//  typedef std::pair< void*, AllocInfo >                                              AllocPair;
//
//  ReturnInfo GetReturnAddressInfo(void* address);
//
//  AllocMap m_allocMap;
//  AllocMap m_deallocMap;
//
//};
//
//static AllocManager g_allocMgr;

