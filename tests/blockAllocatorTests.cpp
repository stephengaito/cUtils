#include <string.h>
#include <stdio.h>
#include <exception>

#include <cUtils/specs/specs.h>

#ifndef protected
#define protected public
#endif

#include <stdio.h>
#include <cUtils/blockAllocator.h>

/// \brief We test the correctness of the C-based BlockAllocator structure.
///
describe(BlockAllocator) {

  specSize(BlockAllocator);

  it("CreateBlockAllocator should create a working block allocator") {
    BlockAllocator *blockAllocator = new BlockAllocator(10);
    shouldNotBeEqual(blockAllocator, NULL);
    shouldBeEqual(blockAllocator->blockSize, 10);
//    for (size_t i = 0; i < blockAllocator->numBlocks; i++) {
//      shouldBeEqual(blockAllocator->blocks[i], NULL);
//    }
    delete blockAllocator;
  } endIt();

  it("AddNewBlock should add a new block") {
    char* prevPtrs[25];
    BlockAllocator *blockAllocator = new BlockAllocator(10);
    for (size_t j = 0; j < 25; j++) {
      blockAllocator->addNewBlock();
      prevPtrs[j] = blockAllocator->curAllocationByte;
      shouldBeEqual(blockAllocator->blocks.getNumItems(), j+1);
      for (size_t i = 0; i <= j; i++) {
       shouldNotBeEqual(blockAllocator->blocks.getItem(i, NULL), NULL);
       shouldBeEqual(blockAllocator->blocks.getItem(i, NULL), prevPtrs[i]);
      }
    }
    delete blockAllocator;
  } endIt();

  it("ClearBlocks and AddNewBlock should work together") {
    char* prevPtrs[25];
    BlockAllocator *blockAllocator = new BlockAllocator(10);
    //
    // allocate lots of blocks
    //
    for (size_t j = 0; j < 25; j++) {
      blockAllocator->addNewBlock();
      prevPtrs[j] = blockAllocator->curAllocationByte;
      shouldBeEqual(blockAllocator->blocks.getNumItems(), j+1);
      for (size_t i = 0; i <= j; i++) {
       shouldNotBeEqual(blockAllocator->blocks.getItem(i, NULL), NULL);
       shouldBeEqual(blockAllocator->blocks.getItem(i, NULL), prevPtrs[i]);
      }
    }
    //
    // clear them all
    //
    blockAllocator->clearBlocks();
    //
    // now allocate some more
    //
    for (size_t j = 0; j < 25; j++) {
      blockAllocator->addNewBlock();
      prevPtrs[j] = blockAllocator->curAllocationByte;
      shouldBeEqual(blockAllocator->blocks.getNumItems(), j+1);
      for (size_t i = 0; i <= j; i++) {
       shouldNotBeEqual(blockAllocator->blocks.getItem(i, NULL), NULL);
       shouldBeEqual(blockAllocator->blocks.getItem(i, NULL), prevPtrs[i]);
      }
    }
    delete blockAllocator;
  } endIt();

  it("AllocateNewStructure should allocate some new structures") {
    BlockAllocator *blockAllocator = new BlockAllocator(10);
    for (size_t j = 0; j < 25; j++) {
      char *aStructure = blockAllocator->allocateNewStructure(2);
      shouldNotBeEqual(aStructure, NULL);
    }
    shouldBeEqual(blockAllocator->blocks.getNumItems(), 5);
    size_t i = 0;
    for ( ; i < 5; i++) {
      shouldNotBeEqual(blockAllocator->blocks.getItem(i, NULL), NULL);
    }
    for ( ; i < blockAllocator->blocks.getNumItems(); i++) {
      shouldBeEqual(blockAllocator->blocks.getItem(i, NULL), NULL);
    }
    delete blockAllocator;
  } endIt();

} endDescribe(BlockAllocator);

//static int somethingSilly = SpecRunner::registerRunner(runBlockAllocator);

