//---------------------------------------------------------------------------
#ifndef OAHASHTABLEH
#define OAHASHTABLEH
//---------------------------------------------------------------------------

#include <string>
#include "ObjectAllocator.h"
#include "support.h"

// client-provided hash function: takes a key and table size,
// returns an index in the table.
typedef unsigned (*HASHFUNC)(const char *, unsigned);

// Max length of our "string" keys
const unsigned MAX_KEYLEN = 10;

class HashTableException
{
  private:  
    int errorCode;
    std::string message;

  public:
    HashTableException(int errCode, const std::string& message) :
        errorCode(errCode), message(message) {};

    virtual ~HashTableException() {
    }

    virtual int code() const { 
      return errorCode; 
    }

    virtual const char *what() const {
      return message.c_str();
    }
    enum HASHTABLE_EXCEPTION {E_ITEM_NOT_FOUND, E_DUPLICATE, E_NO_MEMORY};
};


// HashTable statistical info
struct HTStats
{
  HTStats() : count(0), tableSize(0), probes(0), expansions(0),
                    hashFunc(0), allocator(0) {};
  unsigned count;      // Number of elements in the table
  unsigned tableSize;  // Size of the table (total slots)
  mutable unsigned probes;     // Number of probes performed
  unsigned expansions; // Number of times the table grew
  HASHFUNC hashFunc;   // Pointer to primary hash function
  ObjectAllocator *allocator; // The allocator in use (may be 0)
};

template <typename T>
class ChHashTable
{
  public:

    typedef void (*FREEPROC)(T); // client-provided free proc (we own the data)

    struct HTConfig
    {
        HTConfig(unsigned initialTableSize, HASHFUNC hashFunc, double maxLoadFactor = 3.0,
                 double growthFactor = 2.0, FREEPROC freeProc = 0) : initialTableSize(initialTableSize), 
																		hashFunc(hashFunc), 
																		maxLoadFactor(maxLoadFactor), 
																		growthFactor(growthFactor),
																		freeProc(freeProc) {}

      unsigned initialTableSize;
      HASHFUNC hashFunc; 
      double maxLoadFactor;
      double growthFactor;
      FREEPROC freeProc;
    };
      
      // Nodes that will hold the key/data pairs
    struct ChHTNode
    {
      char key[MAX_KEYLEN]; // Key is a string
      T data;               // Client data
      ChHTNode *next;
      ChHTNode(const T& data) : data(data), next(0) {}; // constructor
    };

      // Each list has a special head pointer
    struct ChHTHeadNode
    {
      ChHTNode *nodes;
      ChHTHeadNode() : nodes(0), count(0) {};
      int count; // For testing
    };

    ChHashTable(const HTConfig& config, ObjectAllocator* allocator = 0);
    ~ChHashTable();

      // Insert a key/data pair into table. Throws an exception if the
      // insertion is unsuccessful.
    void insert(const char *key, const T& data);

      // Delete an item by key. Throws an exception if the key doesn't exist.
    void remove(const char *key);

      // Find and return data by key. Throws an exception if the
      // key doesn't exist.  
    const T& find(const char *key) const;

      // Removes all items from the table (Doesn't deallocate table)
    void clear();

      // Allow the client to peer into the data
    HTStats getStats() const;
    const ChHTHeadNode *getTable() const;

  private:
    //private functions
    ChHTNode* find_node(int index, const char* key) const;
    ChHTNode* find_node(int index, const char* key, ChHTNode*& prev_node) const;

    //variables
	  HTStats stats;
    HTConfig config;
	  ChHTHeadNode* slots;
};

#include "ChHashTable.hpp"

#endif