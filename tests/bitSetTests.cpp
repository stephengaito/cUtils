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
    delete bitSet;
  } endIt();

  it("num2offset compose offset2num should be inverse") {
    shouldBeEqual(BitSet::offset2num(1), BIT_SET_ITEM_BITS);
    shouldBeEqual(BitSet::offset2num(1123), 1123*BIT_SET_ITEM_BITS);
    shouldBeEqual(BitSet::num2offset(BitSet::offset2num(12314)), 12314);
    shouldBeEqual(BitSet::num2offset(BitSet::offset2num(12316)), 12316);
  } endIt();

  it("offset2num compose num2offset should be inverse modulo rounding down") {
    shouldBeEqual(BitSet::offset2num(BitSet::num2offset(12)), 0);
    shouldBeEqual(BitSet::offset2num(BitSet::num2offset(63)), 0);
    shouldBeEqual(BitSet::offset2num(BitSet::num2offset(64)), 64);
    shouldBeEqual(BitSet::offset2num(BitSet::num2offset(65)), 64);
    shouldBeEqual(BitSet::offset2num(BitSet::num2offset(12287)), 12224);
    shouldBeEqual(BitSet::offset2num(BitSet::num2offset(12288)), 12288);
    shouldBeEqual(BitSet::offset2num(BitSet::num2offset(12289)), 12288);
    shouldBeEqual(BitSet::offset2num(BitSet::num2offset(12316)), 12288);
  } endIt();

  it("newSegment should return a correct bitSegment") {
    BitSet::Segment *segment = BitSet::newSegment(10, 10);
    shouldNotBeNULL(segment);
    shouldBeNULL(segment->next);
    shouldBeEqual(segment->offset, 0);
    shouldBeEqual(segment->numItems, 1);
    BitSet::deleteSegments(segment);
    segment = BitSet::newSegment(12314, 12314);
    shouldNotBeNULL(segment);
    shouldBeNULL(segment->next);
    shouldBeEqual(segment->offset, BitSet::num2offset(12314));
    shouldBeEqual(segment->numItems, BitSet::num2offset(12314)+1);
    BitSet::deleteSegments(segment);
  } endIt();

  it("should be able to twiddle multiple close bits in an empty bitSet") {
    BitSet *bitSet = new BitSet();
    shouldNotBeNULL(bitSet);
    shouldBeNULL(bitSet->root);
    bitSet->setBit(1000);
    shouldNotBeNULL(bitSet->root);
    shouldBeFalse(bitSet->getBit(1));
    shouldBeTrue(bitSet->getBit(1000));
    shouldBeFalse(bitSet->getBit(10001));
    bitSet->clearBit(1000);
    shouldBeFalse(bitSet->getBit(1000));
    bitSet->toggleBit(1000);
    shouldBeTrue(bitSet->getBit(1000));
    bitSet->toggleBit(1000);
    shouldBeFalse(bitSet->getBit(1000));
    delete bitSet;
  } endIt();

  it("should be able to twiddle multiple bits larger first") {
    BitSet *bitSet = new BitSet();
    shouldNotBeNULL(bitSet);
    shouldBeNULL(bitSet->root);
    shouldBeFalse(bitSet->getBit(1000));
    bitSet->setBit(1000);
    shouldBeTrue(bitSet->getBit(1000));
    shouldNotBeNULL(bitSet->root);
    shouldBeFalse(bitSet->getBit(1));
    bitSet->setBit(1);
    shouldBeTrue(bitSet->getBit(1));
    delete bitSet;
  } endIt();

  it("should be able to twiddle multiple bits largest last") {
    BitSet *bitSet = new BitSet();
    shouldNotBeNULL(bitSet);
    shouldBeNULL(bitSet->root);
    shouldBeFalse(bitSet->getBit(1));
    bitSet->setBit(1);
    shouldBeTrue(bitSet->getBit(1));
    shouldNotBeNULL(bitSet->root);
    shouldBeFalse(bitSet->getBit(1000));
    bitSet->setBit(1000);
    shouldBeTrue(bitSet->getBit(1000));
    delete bitSet;
  } endIt();

  it("should be able to twiddle multiple bits middle last") {
    BitSet *bitSet = new BitSet();
    shouldNotBeNULL(bitSet);
    shouldBeNULL(bitSet->root);
    shouldBeFalse(bitSet->getBit(1));
    bitSet->setBit(1);
    shouldBeTrue(bitSet->getBit(1));
    shouldNotBeNULL(bitSet->root);

    shouldBeFalse(bitSet->getBit(1000));
    bitSet->setBit(1000);
    shouldBeTrue(bitSet->getBit(1000));

    shouldBeFalse(bitSet->getBit(500));
    bitSet->setBit(500);
    shouldBeTrue(bitSet->getBit(500));

    delete bitSet;
  } endIt();

} endDescribe(BitSet);
