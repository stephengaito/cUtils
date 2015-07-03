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
      for (Segment *next = root; next; next = next->next) {
        if (((size_t)BIT_SET_UINT_MAX) <= next->offset + next->numItems)
          throw AssertionFailure("BitSet too large");
      }
      return true;
    }

    BitSet(void) {
      root = NULL;
      ASSERT(invariant());
    }

    ~BitSet(void) {
      BitSet::deleteSegments(root);
    }

    bool getBit(size_t bitNum) const {
      ASSERT(invariant());
      size_t bitOffset = num2offset(bitNum);
      size_t bitMask   = getBitMask(bitNum);
      for (Segment *curSeg = root; curSeg; curSeg = curSeg->next) {
        if (bitOffset < curSeg->offset) return false;
        size_t itemNum = bitOffset - curSeg->offset;
        if (curSeg->numItems < itemNum) continue;
        return (curSeg->bits[itemNum] & bitMask) ? true : false;
      }
      return false;
    }

    void manipulateBit(size_t bitNum, bool setBit) {
      ASSERT(invariant());
      size_t bitOffset = num2offset(bitNum);
      size_t bitMask   = getBitMask(bitNum);
      Segment *prevPrevSeg = NULL;
      Segment *prevSeg     = NULL;
      Segment *curSeg      = root;
      for ( ; curSeg ;
           prevPrevSeg = prevSeg, prevSeg = curSeg, curSeg = curSeg->next ) {
        if (bitOffset < curSeg->offset) {
          addSegmentBetween(bitNum, prevPrevSeg, prevSeg, curSeg);
        }
        size_t itemNum = bitOffset - curSeg->offset;
        if (curSeg->numItems < itemNum) continue;
        if (setBit) curSeg->bits[itemNum] |= bitMask;
        else        curSeg->bits[itemNum] &= ~bitMask;
        return;
      }
      addSegmentBetween(bitNum, prevPrevSeg, prevSeg, curSeg);
      ASSERT(curSeg);
      size_t itemNum = bitOffset - curSeg->offset;
      ASSERT(itemNum < curSeg->numItems);
        if (setBit) curSeg->bits[itemNum] |= bitMask;
        else        curSeg->bits[itemNum] &= ~bitMask;
      if (!root) root = curSeg;
    }

    void setBit(size_t bitNum)   { manipulateBit(bitNum, true);  }
    void clearBit(size_t bitNum) { manipulateBit(bitNum, false); }

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

    typedef struct Segment {
      Segment *next;
      BIT_SET_UINT offset;
      BIT_SET_UINT numItems;
      size_t bits[0];
    } Segment;

    static Segment *newSegment(size_t bitNum, size_t numBits) {
      BIT_SET_UINT offset   = num2offset(bitNum);
      BIT_SET_UINT numItems = num2offset(numBits)+1;
      if (((size_t)BIT_SET_UINT_MAX) <= offset + numItems) {
        return NULL;
      }
      size_t numMembers = numItems + sizeof(Segment)/BIT_SET_ITEM_SIZE;
      Segment *segment =
        (Segment*)calloc(numMembers, sizeof(size_t));
      ASSERT(segment);
      segment->next     = NULL;
      segment->offset   = offset;
      segment->numItems = numItems;
      return segment;
    }

    static void deleteSegments(Segment *segment) {
      if (!segment) return;
      deleteSegments(segment->next);
      segment->next = NULL;
      segment->offset = 0;
      segment->numItems = 0;
      free(segment);
    }

    void addSegmentBetween(size_t bitNum,
                           Segment *&prevPrevSeg,
                           Segment *&prevSeg,
                           Segment *&curSeg) {
      if (!prevPrevSeg && !prevSeg && !curSeg) {
        // no segment has ever been assigned
        curSeg = newSegment(bitNum,63);
        ASSERT(curSeg);
        return;
      }
    }

    Segment *root;
};

#endif

