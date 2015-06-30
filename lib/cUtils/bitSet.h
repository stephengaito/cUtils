#ifndef BIT_SET_H
#define BIT_SET_H

#include <cUtils/blockAllocator.h>

// Determine the architecture (64bit vs 32bit) to determine a number of
// constants
//
// Both clang and gcc define the __x86_64__ and __i386__ macros
#ifdef __x86_64__
#define BIT_SET_UINT      uint64_t
#define BIT_SET_UINT_MAX  UINT64_MAX
#define BIT_SET_SHIFT     6
#define BIT_SET_MASK      0x3F
#else
#define BIT_SET_UNIT      uint32_t
#define BIT_SET_UINT_MAX  UINT32_MAX
#define BIT_SET_SHIFT     5
#define BIT_SET_MASK      0x1F
#endif
#define BIT_SET_ITEM_SIZE __SIZEOF_SIZE_T__
#define BIT_SET_MIN_BITS  ((BIT_SET_ITEM_SIZE)*8)
#define BIT_SET_MAX_BITS  ((BIT_SET_UINT_MAX<<BIT_SET_SHIFT)|BIT_SET_MASK)

class BitSet {

  public:
    bool invariant(void) const {
      if (BIT_SET_MAX_BITS <
          ((offset<<BIT_SET_SHIFT) + numItems*BIT_SET_MIN_BITS))
        throw AssertionFailure("BitSet too large");
      return true;
    }

    static BitSet *newBitSet(size_t offset, size_t numBits) {
      offset >>= BIT_SET_SHIFT;
      size_t numItems = numBits >> BIT_SET_SHIFT;
      if (BIT_SET_MAX_BITS <
          ((offset<<BIT_SET_SHIFT) + numItems*BIT_SET_MIN_BITS)) {
        return NULL;
      }
      BitSet *bitSet =
        (BitSet*)calloc(numItems + sizeof(BitSet)/BIT_SET_ITEM_SIZE,
                        sizeof(size_t));
      bitSet->next = NULL;
      bitSet->offset = offset;
      bitSet->numItems = numItems;
      return bitSet;
    }

    bool getBit(size_t bitNum) {
      return true;
    }

    void setBit(size_t bitNum) {

    }

    void clearBit(size_t bitNum) {

    }

  protected:

    BitSet *next;
    BIT_SET_UINT offset;
    BIT_SET_UINT numItems;

};

#endif

