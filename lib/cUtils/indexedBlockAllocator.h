#ifndef INDEXED_BLOCK_ALLOCATOR_H
#define INDEXED_BLOCK_ALLOCATOR_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cUtils/blockAllocator.h"

/// \brief The IndexedBlockAllocator class holds the information
/// required to allocate multiple blocks of related (sub)structures
/// which are allocated at continquous indexes.
class IndexedBlockAllocator : public BlockAllocator {

  public:

    /// \brief An invariant which should ALWAYS be true for any
    /// instance of a IndexedBlockAllocator class.
    ///
    /// Throws an AssertionFailure with a brief description of any
    /// inconsistencies discovered.
    bool invariant(void) const {
      if (blockSize != (1<<bitShift)*itemSize) {
        throw AssertionFailure("incorrect block size");
      }
      return BlockAllocator::invariant();
    }

    /// \brief Create a new indexed block allocator which allocates items
    /// of a given anItemSize.
    ///
    /// ABitSize determines the balance between block size and the
    /// number of blocks.
    IndexedBlockAllocator(size_t anItemSize,
                          size_t aBitShift = 5)
                        : BlockAllocator( (1<<aBitShift)*anItemSize ){
      itemSize = anItemSize;
      bitShift = aBitShift;
      ASSERT(invariant());
    }

    /// \brief Allocate an indexed new (sub)structure.
    ///
    /// The indexes returns are contiquous.
    size_t allocateNewStructure(void) {
      ASSERT(invariant());
      char * itemPtr =  BlockAllocator::allocateNewStructure(itemSize);
      size_t blockNum = blocks.getNumItems() - 1;
      return (blockNum << bitShift) + (itemPtr - blocks.getTop())/itemSize;
    }

    size_t nextIndex(void) {
      char *itemPtr = curAllocationByte;
      size_t blockNum = blocks.getNumItems() - 1;
      return (blockNum << bitShift) + (itemPtr - blocks.getTop())/itemSize;
    }

    /// \brief Compute the char* pointer corresponding to this
    /// itemNumber.
    char *getItemPtr(size_t itemNum) {
      ASSERT(invariant());
      size_t blockNum = itemNum >> bitShift;
      if (blocks.getNumItems() <= blockNum) return NULL;
      char *blockPtr = blocks.getItem(blockNum, NULL);
      if (!blockPtr) return NULL;
      size_t itemMask = ~((~0L)<<bitShift);
      char *itemPtr = blockPtr + (itemNum&itemMask)*itemSize;
      if (curAllocationByte <= itemPtr) return NULL;
      return itemPtr;
    }

  protected:

    /// \brief Override the BlockAllocator::allocateNewStructure to
    /// prevent its use.
    size_t allocateNewStructure(size_t structureSize) {
      ASSERT(invariant());
      return 0;
    }

    /// \brief The size of each indexed item to be block allocated by
    /// this class.
    size_t itemSize;

    /// \brief The bitShift represents the power of two number of items
    /// in a given block.
    ///
    /// We use a power of two to keep the computation of index->pointer
    /// simple.
    size_t bitShift;
};

#endif
