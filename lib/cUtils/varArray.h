#ifndef VAR_ARRAY_H
#define VAR_ARRAY_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cUtils/assertions.h"

#ifndef VarArrayIncrement
#define VarArrayIncrement 10
#endif

template<class ItemT>
class VarArrayIterator;

/// \brief The VarArray template class holds the information required
/// to manage a variable array of identical objects.
template<class ItemT>
class VarArray {
  public:

    /// \brief An invariant which should ALWAYS be true for any
    /// instance of a VarArray<ItemT> class.
    ///
    /// Throws an AssertionFailure with a brief description of any
    /// inconsistencies discovered.
    bool invariant(void) const {
      return (numItems <= arraySize) && ((itemArray) || (arraySize==0));
    }

    /// \brief Create a VarArray.
    VarArray(void) {
      numItems  = 0;
      arraySize = 0;
      itemArray = NULL;
      ASSERT(invariant());
    }

    /// \brief Explicitly destroy a VarArray.
    ///
    /// Note that this should be invoked implicitly when ever the
    /// containing object gets deleted.
    ~VarArray(void) {
      ASSERT_INSIDE_DELETE(invariant());
      if (itemArray) free(itemArray);
      itemArray = NULL;
      numItems  = 0;
      arraySize = 0;
    }

    /// \brief Return the current number of items in the array.
    size_t getNumItems(void) const {
      return numItems;
    }

    /// \brief Push a new item onto the "top" of the array.
    void pushItem(ItemT anItem) {
      ASSERT(invariant());
      if (arraySize <= numItems) {
        // we need to increase the size of the array
        ItemT *oldArray = itemArray;
        itemArray = (ItemT*)calloc(arraySize+VarArrayIncrement, sizeof(ItemT));
        if (oldArray) {
          memcpy(itemArray, oldArray, arraySize*sizeof(ItemT));
          free(oldArray);
        }
        arraySize += VarArrayIncrement;
      }
      itemArray[numItems] = anItem;
      numItems++;
      ASSERT(invariant());
    }

    /// \brief Get the requested item.
    ///
    /// Returns the default provided if the itemNumber is out of range.
    ItemT getItem(size_t itemNumber, ItemT defaultItem) const {
      ASSERT(invariant());
      if (numItems <= itemNumber) return defaultItem;
      return itemArray[itemNumber];
    }

    /// \brief Set the requested item to the value provided.
    void setItem(size_t itemNumber, ItemT anItem) {
      ASSERT(invariant());
      if (itemNumber < numItems) itemArray[itemNumber] = anItem;
    }

    /// \brief Get the top item
    ItemT getTop(void) const {
      ASSERT(invariant());
      ASSERT(numItems);
      return itemArray[numItems-1];
    }

    /// \brief Remove and return the "top" item on the array.
    ItemT popItem(void) {
      ASSERT(invariant());
      ASSERT(numItems); // incorrectly matched push/pops
      numItems--;
      return itemArray[numItems];
    }

    /// \brief Copy the items in this array into the buffer provided.
//    void copyItems(void*buffer, size_t bufferSize) {
//      ASSERT(invariant());
//      if (numItems*sizeof(ItemT) < bufferSize) {
//        bufferSize = numItems*sizeof(ItemT);
//      }
//      if (bufferSize < numItems*sizeof(ItemT)) {
//        bufferSize = sizeof(ItemT)*(bufferSize/sizeof(ItemT));
//      }
//      memcpy(buffer, itemArray, bufferSize);
//    }

    /// \brief Remove all items from this array.
    void clearItems(void) {
      numItems = 0;
      ASSERT(invariant());
    }

    VarArrayIterator<ItemT> getIterator(void) {
      VarArrayIterator<ItemT> iter(this);
      return iter;
    }

  protected:

    /// \brief Provide a *deep* copy of the other VarArray<ItemT>
    /// instance.
    void operator=(const VarArray &other) {
//        ASSERT_MESSAGE(false, "VarArgs<>::operator= must NOT be used");
      ASSERT(other.invariant());
      numItems  = 0;
      arraySize = 0;
      if (itemArray) free(itemArray);
      itemArray = NULL;
      for (size_t i = 0; i < other.numItems; i++) {
        pushItem(other.itemArray[i]);
      }
      ASSERT(invariant());
    }

    /// \brief The current number of items in the array.
    size_t numItems;

    /// \brief The current maximal possible number of items in the array.
    size_t arraySize;

    /// \brief The items in the array.
    ItemT *itemArray;

  friend class VarArrayIterator<ItemT>;


};

/// \brief The VarArrayIterator template class holds the information required
/// to iterate over a VarArray.

template<class ItemT>
class VarArrayIterator {
public:

  bool hasMoreItems(void) {
    ASSERT(baseArray);
    return curItem < baseArray->numItems;
  }

  ItemT nextItem(void) {
    ASSERT(baseArray);
    ASSERT(curItem < baseArray->numItems);
    return baseArray->itemArray[curItem++];
  }

  ~VarArrayIterator(void) {
    baseArray = NULL;
    curItem = 0;
  }

protected: // methods

  VarArrayIterator(VarArray<ItemT> *aVarArray) {
    baseArray = aVarArray;
    curItem = 0;
  }

protected: // variables

  VarArray<ItemT> *baseArray;

  size_t curItem;

  friend class VarArray<ItemT>;

};

#endif
