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

    void manipulateBit(size_t bitNum, bool toggleBit, bool setBit) {
      ASSERT(invariant());
      size_t bitOffset = num2offset(bitNum);
      size_t bitMask   = getBitMask(bitNum);
      Segment *prevPrevSeg = NULL;
      Segment *prevSeg     = NULL;
      Segment *curSeg      = root;
//      fprintf(stdout, "> %p %p %p\n", prevPrevSeg, prevSeg, curSeg);
      for ( ; curSeg ;
           prevPrevSeg = prevSeg, prevSeg = curSeg, curSeg = curSeg->next ) {
//        fprintf(stdout, "+ %p %p %p\n", prevPrevSeg, prevSeg, curSeg);
        if (bitOffset < curSeg->offset) {
          addSegmentBetween(bitNum, root, prevPrevSeg, prevSeg, curSeg);
        }
        if ((curSeg->offset + curSeg->numItems) < bitOffset) continue;
        size_t itemNum = bitOffset - curSeg->offset;
        if (toggleBit)   curSeg->bits[itemNum] ^= bitMask;
        else if (setBit) curSeg->bits[itemNum] |= bitMask;
        else             curSeg->bits[itemNum] &= ~bitMask;
        return;
      }
      ASSERT(!curSeg); // prevSeg may or may not be NULL
      addSegmentBetween(bitNum, root, prevPrevSeg, prevSeg, curSeg);
      ASSERT(curSeg);
      size_t itemNum = bitOffset - curSeg->offset;
      ASSERT(itemNum < curSeg->numItems);
        if (toggleBit)   curSeg->bits[itemNum] ^= bitMask;
        else if (setBit) curSeg->bits[itemNum] |= bitMask;
        else             curSeg->bits[itemNum] &= ~bitMask;
    }

    void setBit(size_t bitNum)    { manipulateBit(bitNum, false, true);  }
    void clearBit(size_t bitNum)  { manipulateBit(bitNum, false, false); }
    void toggleBit(size_t bitNum) { manipulateBit(bitNum, true,  false); }

    bool isEmpty(void) const {
      Segment *curSeg = root;
      for ( ; curSeg ; curSeg = curSeg->next ) {
        for ( BIT_SET_UINT i = 0; i < curSeg->numItems; i++ ) {
          if (curSeg->bits[i]) return false;
        }
      }
      return true;
    }

    size_t numNonZero(void) {
      Segment *curSeg = root;
      size_t bitCount = 0;
      for ( ; curSeg ; curSeg = curSeg->next ) {
        for ( BIT_SET_UINT i = 0 ; i < curSeg->numItems ; i++ ) {
          size_t curItem = curSeg->bits[i];
          for ( int j = 0 ; curItem && (j < sizeof(size_t)*8) ;
                j++, curItem >>= 1 ) {
            if (curItem & 0x1) bitCount++;
          }
        }
      }
      return bitCount;
    }

    BitSet clone(void) {
      BitSet copyBitSet;
      if (root) {
        copyBitSet.root      = copySegment(root);
        Segment *curSeg      = root->next;
        Segment *prevCopySeg = copyBitSet.root;
        for ( ; curSeg ; curSeg = curSeg->next ) {
          Segment *copySeg = copySegment(curSeg);
          ASSERT(copySeg);
          prevCopySeg->next = copySeg;
          prevCopySeg       = copySeg;
        }
        prevCopySeg->next = NULL;
      }
      return copyBitSet;
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

    typedef struct Segment {
      Segment *next;
      BIT_SET_UINT offset;
      BIT_SET_UINT numItems;
      size_t bits[0];
    } Segment;

    static Segment *copySegment(Segment *curSegment) {
      ASSERT(curSegment);
      size_t numMembers = curSegment->numItems +
        sizeof(Segment)/BIT_SET_ITEM_SIZE;
      Segment *copySegment =
        (Segment*)calloc(numMembers, sizeof(size_t));
      ASSERT(copySegment);
      copySegment->next   = curSegment->next;
      copySegment->offset = curSegment->offset;
      copySegment->numItems = curSegment->numItems;
      memcpy(copySegment->bits, curSegment->bits,
        curSegment->numItems*sizeof(size_t));
      return copySegment;
    }

    static Segment *newSegment(size_t bitNum, size_t numBits,
                               Segment *nextSegment = NULL) {
      BIT_SET_UINT offset   = num2offset(bitNum);
      BIT_SET_UINT numItems = num2offset(numBits)+1;
      if (((size_t)BIT_SET_UINT_MAX) <= offset + numItems) {
        return NULL;
      }
      size_t numMembers = numItems + sizeof(Segment)/BIT_SET_ITEM_SIZE;
      Segment *segment =
        (Segment*)calloc(numMembers, sizeof(size_t));
      ASSERT(segment);
      segment->next     = nextSegment;
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

    // TODO: this currently is ONLY OK for manipulting SINGLE bits
    void addSegmentBetween(size_t bitNum,
                           Segment *&root,
                           Segment *&prevPrevSeg,
                           Segment *&prevSeg,
                           Segment *&curSeg) {
//      fprintf(stdout, "a %p %p %p\n", prevPrevSeg, prevSeg, curSeg);
      if (!prevPrevSeg && !prevSeg && !curSeg) {
        // no segment has ever been added
        ASSERT(!root);
        curSeg = newSegment(bitNum,63, NULL);
        ASSERT(curSeg);
        root = curSeg;
        return;
      }

      BIT_SET_UINT bitOffset = BitSet::num2offset(bitNum);

      if (!prevPrevSeg && !prevSeg && curSeg) {
        // only the curSeg has been added
        ASSERT(root == curSeg);
        ASSERT(bitOffset < curSeg->offset);
        // need to add a new segment BEFORE the current one...
        curSeg = newSegment(bitNum, 63, curSeg);
        ASSERT(curSeg);
        root = curSeg;
        return;
      }

      if (!prevPrevSeg && prevSeg && !curSeg) {
        // only the prevSeg has been added
        ASSERT(root == prevSeg);
        ASSERT((prevSeg->offset + prevSeg->numItems) < bitOffset);
        // need to add a new segment AFTER the current one...
        prevSeg->next = newSegment(bitNum, 63, prevSeg->next);
        curSeg = prevSeg->next;
        return;
      }

      if (prevSeg && curSeg) {
        // we need to add a new segment between prevSeg and curSeg
        ASSERT(prevSeg->offset + prevSeg->numItems < bitOffset);
        ASSERT(bitOffset < curSeg->offset);
        ASSERT(prevSeg->next == curSeg);
        prevSeg->next = newSegment(bitNum, 63, prevSeg->next);
        curSeg = prevSeg->next;
      }
    }

    Segment *root;
};

#endif

