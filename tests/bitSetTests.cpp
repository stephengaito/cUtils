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
  specSize(BitSet::BitSetSegment);
  specSize(BIT_SET_UINT);
  specUValue(BIT_SET_UINT_MAX);
  specUValue(BIT_SET_SHIFT);
  specHValue(BIT_SET_MASK);
  specUValue(BIT_SET_ITEM_SIZE);
  specUValue(BIT_SET_ITEM_BITS);
  shouldBeEqual(BitSet::offset2num(1), BIT_SET_ITEM_BITS);
  shouldBeEqual(BitSet::offset2num(1123), 1123*BIT_SET_ITEM_BITS);

  it("should be created with correct values") {
    BitSet *bitSet = new BitSet();
    shouldNotBeEqual(bitSet, NULL);
    bitSet->setBit(1000);
    shouldBeFalse(bitSet->getBit(1));
    shouldBeTrue(bitSet->getBit(1000));
    shouldBeFalse(bitSet->getBit(10001));
//      bitSet->clearBit(1000);
    shouldBeFalse(bitSet->getBit(1000));
  } endIt();

} endDescribe(BitSet);
