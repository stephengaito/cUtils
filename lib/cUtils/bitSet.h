#ifndef BIT_SET_H
#define BIT_SET_H

#include <cUtils/assertions.h>

// Determine the architecture (64bit vs 32bit) to determine a number of
// constants
//
// Both clang and gcc define the __x86_64__ and __i386__ macros
#ifdef __x86_64__
#define BIT_SET_UINT      uint32_t
#define BIT_SET_UINT_MAX  UINT32_MAX
#define BIT_SET_SHIFT     6
#define BIT_SET_MASK      0x3F
#else
#define BIT_SET_UINT      uint16_t
#define BIT_SET_UINT_MAX  UINT16_MAX
#define BIT_SET_SHIFT     5
#define BIT_SET_MASK      0x1F
#endif
#define BIT_SET_ITEM_SIZE __SIZEOF_SIZE_T__
#define BIT_SET_ITEM_BITS ((BIT_SET_ITEM_SIZE)*8)

class BitSet {

  public:
    bool invariant(void) const {
      for (BitSetSegment *next = root; next; next = next->next) {
        if (((size_t)BIT_SET_UINT_MAX) <= next->offset + next->numItems)
          throw AssertionFailure("BitSet too large");
      }
      return true;
    }

    BitSet(void) {
      root = NULL;
      ASSERT(invariant());
    }

    bool getBit(size_t bitNum) const {
      ASSERT(invariant());
      size_t bitOffset = num2offset(bitNum);
      size_t bitMask   = getBitMask(bitNum);
      for (BitSetSegment *curSeg = root; curSeg; curSeg = curSeg->next) {
        if (bitOffset < curSeg->offset) return false;
        size_t itemNum = bitOffset - curSeg->offset;
        if (curSeg->numItems < itemNum) continue;
        return (curSeg->bits[itemNum] & bitMask) ? true : false;
      }
      return false;
    }

    void setBit(size_t bitNum) {
      ASSERT(invariant());
      size_t bitOffset = num2offset(bitNum);
      size_t bitMask   = getBitMask(bitNum);
      BitSetSegment *prevPrevSeg = NULL;
      BitSetSegment *prevSeg     = NULL;
      BitSetSegment *curSeg      = root;
      for ( ; curSeg ;
           prevPrevSeg = prevSeg, prevSeg = curSeg, curSeg = curSeg->next ) {
        if (bitOffset < curSeg->offset) {
          addSegmentBetween(prevPrevSeg, prevSeg, curSeg);
        }
        size_t itemNum = bitOffset - curSeg->offset;
        if (curSeg->numItems < itemNum) continue;
        curSeg->bits[itemNum] |= bitMask;
        return;
      }
      addSegmentBetween(prevPrevSeg, prevSeg, curSeg);
      ASSERT(curSeg);
      size_t itemNum = bitOffset - curSeg->offset;
      ASSERT(curSeg->numItems < itemNum);
      curSeg->bits[itemNum] |= bitMask;
    }

    void clearBit(size_t bitNum) {
      ASSERT(invariant());
      size_t bitOffset = num2offset(bitNum);
      size_t bitMask   = getBitMask(bitNum);
      for (BitSetSegment *curSeg = root; curSeg; curSeg = curSeg->next) {
        if (bitOffset < curSeg->offset) return;
        size_t itemNum = bitOffset - curSeg->offset;
        if (curSeg->numItems < itemNum) continue;
        curSeg->bits[itemNum] &= ~bitMask;
      }
    }

  protected:

    static size_t num2offset(size_t aNumber) {
      return aNumber>>BIT_SET_SHIFT;
    }
    static size_t offset2num(size_t anOffset) {
      return anOffset<<BIT_SET_SHIFT;
    }
    static size_t getBitMask(size_t bitNum) {
      return 1 << ((bitNum & BIT_SET_MASK)-1);
    }

    typedef struct BitSetSegment {
      BitSetSegment *next;
      BIT_SET_UINT offset;
      BIT_SET_UINT numItems;
      size_t bits[0];
    } BitSetSegment;

    static BitSetSegment *newBitSetSegment(size_t offset, size_t numBits) {
      offset = num2offset(offset);
      size_t numItems = num2offset(numBits)+1;
      if (((size_t)BIT_SET_UINT_MAX) <= offset + numItems) {
        return NULL;
      }
      size_t numMembers = numItems + sizeof(BitSetSegment)/BIT_SET_ITEM_SIZE;
      fprintf(stdout, " %zu %zu\n", numMembers, sizeof(size_t));
      fflush(stdout);
      BitSetSegment *segment =
        (BitSetSegment*)calloc(1, 5*sizeof(size_t));
//        (BitSetSegment*)calloc(numMembers, sizeof(size_t));
      ASSERT(segment);
//      segment->next = NULL;
//      segment->offset = offset;
//      segment->numItems = numItems;
      return segment;
    }

    void addSegmentBetween(BitSetSegment *&prevPrevSeg,
                           BitSetSegment *&prevSeg,
                           BitSetSegment *&curSeg) {
      curSeg = newBitSetSegment(10,10);
      ASSERT(curSeg);
    }

    BitSetSegment *root;
};

#endif

