#include <string.h>
#include <stdio.h>
#include <exception>

#include <cUtils/specs/specs.h>

#ifndef protected
#define protected public
#endif

#include <stdio.h>
#include <cUtils/bitSet.h>


/// \brief We test the correctness of the C-based BitSet structure.
///
describe(BitSet) {

  specSize(BitSet);
  specSize(BitSet::Segment);
  specSize(BIT_SET_UINT);
  specUValue(BIT_SET_UINT_MAX);
  specUValue(BIT_SET_SHIFT);
  specHValue(BIT_SET_MASK);
  specUValue(BIT_SET_ITEM_SIZE);
  specUValue(BIT_SET_ITEM_BITS);

  it("should be created with correct values") {
    BitSet *bitSet = new BitSet();
    shouldNotBeNULL(bitSet);
    shouldBeNULL(bitSet->root);
    shouldBeTrue(bitSet->isEmpty());
    shouldBeZero(bitSet->numNonZero());
    delete bitSet;
  } endIt();

  it("num2offset compose offset2num should be inverse") {
    shouldBeEqual(BitSet::offset2num(1), BIT_SET_ITEM_BITS);
    shouldBeEqual(BitSet::offset2num(1123), 1123*BIT_SET_ITEM_BITS);
    shouldBeEqual(BitSet::num2offset(BitSet::offset2num(12314)), 12314);
    shouldBeEqual(BitSet::num2offset(BitSet::offset2num(12316)), 12316);
  } endIt();

  it("offset2num compose num2offset should be inverse modulo rounding down") {
    shouldBeZero(BitSet::offset2num(BitSet::num2offset(12)));
#ifdef __x86_64__
    shouldBeZero(BitSet::offset2num(BitSet::num2offset(63)));
#else
    shouldBeZero(BitSet::offset2num(BitSet::num2offset(31)));
#endif
    shouldBeEqual(BitSet::offset2num(BitSet::num2offset(64)), 64);
    shouldBeEqual(BitSet::offset2num(BitSet::num2offset(65)), 64);
#ifdef __x86_64__
    shouldBeEqual(BitSet::offset2num(BitSet::num2offset(12287)), 12224);
#else
    shouldBeEqual(BitSet::offset2num(BitSet::num2offset(12287)), 12256);
#endif
    shouldBeEqual(BitSet::offset2num(BitSet::num2offset(12288)), 12288);
    shouldBeEqual(BitSet::offset2num(BitSet::num2offset(12289)), 12288);
    shouldBeEqual(BitSet::offset2num(BitSet::num2offset(12316)), 12288);
  } endIt();

  it("newSegment should return a correct bitSegment") {
    BitSet::Segment *segment = BitSet::newSegment(10, 10);
    shouldNotBeNULL(segment);
    shouldBeNULL(segment->next);
    shouldBeZero(segment->offset);
    shouldBeEqual(segment->numItems, 1);
    BitSet::deleteSegments(segment);
    segment = BitSet::newSegment(12314, 12314);
    shouldNotBeNULL(segment);
    shouldBeNULL(segment->next);
    shouldBeEqual(segment->offset, BitSet::num2offset(12314));
    shouldBeEqual(segment->numItems, BitSet::num2offset(12314)+1);
    BitSet::deleteSegments(segment);
  } endIt();

  it("copySegment should return a copy of a Segment") {
    BitSet::Segment *segment = BitSet::newSegment(12314, 12314);
    shouldNotBeNULL(segment);
    shouldBeNULL(segment->next);
    shouldBeEqual(segment->offset, BitSet::num2offset(12314));
    shouldBeEqual(segment->numItems, BitSet::num2offset(12314)+1);
    BitSet::Segment *copySeg = BitSet::copySegment(segment);
    shouldNotBeNULL(copySeg);
    shouldNotBeEqual(copySeg, segment);
    shouldBeNULL(copySeg->next);
    shouldBeEqual(copySeg->offset,   segment->offset);
    shouldBeEqual(copySeg->numItems, segment->numItems);
    for ( size_t i = 0; i < segment->numItems ; i++ ) {
      shouldBeEqual(copySeg->bits[i], segment->bits[i]);
    }
    BitSet::deleteSegments(copySeg);
    BitSet::deleteSegments(segment);
  } endIt();

  it("should be able to twiddle multiple close bits in an empty bitSet") {
    BitSet *bitSet = new BitSet();
    shouldNotBeNULL(bitSet);
    shouldBeNULL(bitSet->root);
    shouldBeTrue(bitSet->isEmpty());
    shouldBeZero(bitSet->numNonZero());
    bitSet->setBit(1000);
    shouldNotBeNULL(bitSet->root);
    shouldBeFalse(bitSet->getBit(1));
    shouldBeTrue(bitSet->getBit(1000));
    shouldBeFalse(bitSet->getBit(10001));
    shouldBeFalse(bitSet->isEmpty());
    shouldBeEqual(bitSet->numNonZero(), 1);
    bitSet->clearBit(1000);
    shouldBeFalse(bitSet->getBit(1000));
    shouldBeTrue(bitSet->isEmpty());
    shouldBeZero(bitSet->numNonZero());
    bitSet->toggleBit(1000);
    shouldBeFalse(bitSet->isEmpty());
    shouldBeTrue(bitSet->getBit(1000));
    shouldBeEqual(bitSet->numNonZero(), 1);
    bitSet->toggleBit(1000);
    shouldBeFalse(bitSet->getBit(1000));
    shouldBeTrue(bitSet->isEmpty());
    shouldBeZero(bitSet->numNonZero());
    delete bitSet;
  } endIt();

  it("should be able to twiddle multiple bits larger first") {
    BitSet *bitSet = new BitSet();
    shouldNotBeNULL(bitSet);
    shouldBeNULL(bitSet->root);
    shouldBeTrue(bitSet->isEmpty());
    shouldBeZero(bitSet->numNonZero());
    shouldBeFalse(bitSet->getBit(1000));
    bitSet->setBit(1000);
    shouldBeFalse(bitSet->isEmpty());
    shouldBeEqual(bitSet->numNonZero(), 1);
    shouldBeTrue(bitSet->getBit(1000));
    shouldNotBeNULL(bitSet->root);
    shouldBeFalse(bitSet->getBit(1));
    bitSet->setBit(1);
    shouldBeTrue(bitSet->getBit(1));
    shouldBeFalse(bitSet->isEmpty());
    shouldBeEqual(bitSet->numNonZero(), 2);
    delete bitSet;
  } endIt();

  it("should be able to twiddle multiple bits largest last") {
    BitSet *bitSet = new BitSet();
    shouldNotBeNULL(bitSet);
    shouldBeNULL(bitSet->root);
    shouldBeTrue(bitSet->isEmpty());
    shouldBeZero(bitSet->numNonZero());
    shouldBeFalse(bitSet->getBit(1));
    bitSet->setBit(1);
    shouldBeTrue(bitSet->getBit(1));
    shouldNotBeNULL(bitSet->root);
    shouldBeFalse(bitSet->isEmpty());
    shouldBeEqual(bitSet->numNonZero(), 1);
    shouldBeFalse(bitSet->getBit(1000));
    bitSet->setBit(1000);
    shouldBeTrue(bitSet->getBit(1000));
    shouldBeFalse(bitSet->isEmpty());
    shouldBeEqual(bitSet->numNonZero(), 2);
    delete bitSet;
  } endIt();

  it("should be able to twiddle multiple bits middle last") {
    BitSet *bitSet = new BitSet();
    shouldNotBeNULL(bitSet);
    shouldBeNULL(bitSet->root);
    shouldBeTrue(bitSet->isEmpty());
    shouldBeZero(bitSet->numNonZero());
    shouldBeFalse(bitSet->getBit(1));
    bitSet->setBit(1);
    shouldBeTrue(bitSet->getBit(1));
    shouldNotBeNULL(bitSet->root);
    shouldBeFalse(bitSet->isEmpty());
    shouldBeEqual(bitSet->numNonZero(), 1);

    shouldBeFalse(bitSet->getBit(1000));
    bitSet->setBit(1000);
    shouldBeTrue(bitSet->getBit(1000));
    shouldBeFalse(bitSet->isEmpty());
    shouldBeEqual(bitSet->numNonZero(), 2);

    shouldBeFalse(bitSet->getBit(500));
    bitSet->setBit(500);
    shouldBeTrue(bitSet->getBit(500));
    shouldBeFalse(bitSet->isEmpty());
    shouldBeEqual(bitSet->numNonZero(), 3);

    delete bitSet;
  } endIt();

  it("should be able to clone a complex bitSet") {
    BitSet *bitSet = new BitSet();
    shouldNotBeNULL(bitSet);
    shouldBeNULL(bitSet->root);
    shouldBeTrue(bitSet->isEmpty());
    shouldBeZero(bitSet->numNonZero());
    shouldBeFalse(bitSet->getBit(1));
    bitSet->setBit(1);
    shouldBeTrue(bitSet->getBit(1));
    shouldNotBeNULL(bitSet->root);
    shouldBeFalse(bitSet->isEmpty());
    shouldBeEqual(bitSet->numNonZero(), 1);

    shouldBeFalse(bitSet->getBit(1000));
    bitSet->setBit(1000);
    shouldBeTrue(bitSet->getBit(1000));
    shouldBeFalse(bitSet->isEmpty());
    shouldBeEqual(bitSet->numNonZero(), 2);

    shouldBeFalse(bitSet->getBit(500));
    bitSet->setBit(500);
    shouldBeTrue(bitSet->getBit(500));
    shouldBeFalse(bitSet->isEmpty());
    shouldBeEqual(bitSet->numNonZero(), 3);

    BitSet copyBitSet = bitSet->clone();
    BitSet::Segment *curSeg  = bitSet->root;
    BitSet::Segment *copySeg = copyBitSet.root;
    for ( ; curSeg ; curSeg = curSeg->next , copySeg = copySeg->next ) {
      shouldNotBeNULL(curSeg);
      shouldNotBeNULL(copySeg);
      shouldNotBeEqual(curSeg, copySeg);
      shouldBeEqual(curSeg->offset,   copySeg->offset);
      shouldBeEqual(curSeg->numItems, copySeg->numItems);
      for ( size_t i = 0 ; i < curSeg->numItems ; i++ ) {
        shouldBeEqual(curSeg->bits[i], copySeg->bits[i]);
      }
    }
    delete bitSet;
  } endIt();

} endDescribe(BitSet);
