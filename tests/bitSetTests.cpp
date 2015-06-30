#include <bandit/bandit.h>
using namespace bandit;

#include <string.h>
#include <stdio.h>
#include <exception>

#ifndef protected
#define protected public
#endif

#include <stdio.h>
#include <cUtils/bitSet.h>

go_bandit([](){

  printf("\n----------------------------------\n");
  printf(  "bitSetAllocator\n");
  printf(  "           BitSet = %zu bytes (%zu bits)\n", sizeof(BitSet), sizeof(BitSet)*8);
  printf(  "     BIT_SET_UINT = %zu bytes (%zu bits)\n", sizeof(BIT_SET_UINT), sizeof(BIT_SET_UINT)*8);
  printf(  " BIT_SET_UNIT_MAX = %zu\n", (size_t)(BIT_SET_UINT_MAX));
  printf(  "    BIT_SET_SHIFT = %zu\n", (size_t)(BIT_SET_SHIFT));
  printf(  "     BIT_SET_MASK = %zX\n", (size_t)(BIT_SET_MASK));
  printf(  "BIT_SET_ITEM_SIZE = %zu\n", (size_t)(BIT_SET_ITEM_SIZE));
  printf(  " BIT_SET_MIN_BITS = %zu\n", (size_t)(BIT_SET_MIN_BITS));
  printf(  " BIT_SET_MAX_BITS = %zu\n", (size_t)(BIT_SET_MAX_BITS));
  printf(  "----------------------------------\n");

  /// \brief We test the correctness of the C-based BitSet structure.
  ///
  describe("BitSet", [](){

    it("should be created with correct values", [](){
      BitSet *bitSet = BitSet::newBitSet(0, 10);
      AssertThat(bitSet, Is().Not().EqualTo((void*)0));
      bitSet->setBit(1000);
      AssertThat(bitSet->getBit(1),     Is().False());
      AssertThat(bitSet->getBit(1000),  Is().True());
      AssertThat(bitSet->getBit(10001), Is().False());
      bitSet->clearBit(1000);
      AssertThat(bitSet->getBit(1000),  Is().False());
    });

  }); // describe VarArray
});
