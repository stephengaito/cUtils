#include <string.h>
#include <stdio.h>
#include <exception>

#include <cUtils/specs/specs.h>

#ifndef protected
#define protected public
#endif

#include <stdio.h>
#include <cUtils/indexedBlockAllocator.h>

#define deleteTest(variableName)	{		\
  delete variableName;					\
  char *someMemory = (char*)calloc(100, sizeof(char));	\
  shouldNotBeEqual(someMemory, NULL);	\
  free(someMemory);					\
}


/// \brief We test the correctness of the C-based
/// IndexedBlockAllocator structure.
///
describe(IndexedBlockAllocator) {

  specSize(IndexedBlockAllocator);

  it("should create an IndexedBlockAllocator") {
    IndexedBlockAllocator *iba = new IndexedBlockAllocator(11, 4);
    shouldNotBeEqual(iba, NULL);
    shouldBeEqual(iba->itemSize, 11);
    shouldBeEqual(iba->bitShift, 4);
    deleteTest(iba);
  } endIt();

  it("can allocate a new item") {
    IndexedBlockAllocator *iba = new IndexedBlockAllocator(11, 4);
    shouldNotBeEqual(iba, NULL);
    size_t iba0 = iba->allocateNewStructure();
    size_t iba1 = iba->allocateNewStructure();
    shouldBeEqual(iba0, 0);
    shouldBeEqual(iba1, 1);
    shouldBeEqual(iba->getItemPtr(iba0), iba->blocks.getTop());
    shouldBeEqual(iba->getItemPtr(iba1), iba->blocks.getTop()+11);
    deleteTest(iba);
  } endIt();

  it("can allocate lots of new items") {
    IndexedBlockAllocator *iba = new IndexedBlockAllocator(11, 4);
    shouldNotBeEqual(iba, NULL);
    shouldBeEqual(iba->blocks.getNumItems(), 0);
    for (size_t i = 0; i < 1<<5; i++) {
      size_t ibaItem = iba->allocateNewStructure();
      shouldBeEqual(ibaItem, i);
    }
    shouldBeEqual(iba->blocks.getNumItems(), 2);
    size_t ibaItem0 = iba->allocateNewStructure();
    shouldBeEqual(ibaItem0, (1<<5));
    shouldBeEqual(iba->blocks.getNumItems(), 3);
    shouldBeEqual(iba->getItemPtr(ibaItem0), iba->blocks.getTop());
    size_t ibaItem1 = iba->allocateNewStructure();
    shouldBeEqual(ibaItem1, ((1<<5)+1));
    shouldBeEqual(iba->blocks.getNumItems(), 3);
    shouldBeEqual(iba->getItemPtr(ibaItem1), iba->blocks.getTop()+11);
    deleteTest(iba);
  } endIt();

  it("can allocate lots of new items, clear them and then allocate some more") {
    IndexedBlockAllocator *iba = new IndexedBlockAllocator(11, 4);
    shouldNotBeEqual(iba, NULL);
    shouldBeEqual(iba->blocks.getNumItems(), 0);
    for (size_t i = 0; i < 1<<5; i++) {
      size_t ibaItem = iba->allocateNewStructure();
      shouldBeEqual(ibaItem, i);
    }
    shouldBeEqual(iba->blocks.getNumItems(), 2);
    size_t ibaItem0 = iba->allocateNewStructure();
    shouldBeEqual(ibaItem0, (1<<5));
    shouldBeEqual(iba->blocks.getNumItems(), 3);
    shouldBeEqual(iba->getItemPtr(ibaItem0), iba->blocks.getTop());
    size_t ibaItem1 = iba->allocateNewStructure();
    shouldBeEqual(ibaItem1, ((1<<5)+1));
    shouldBeEqual(iba->blocks.getNumItems(), 3);
    shouldBeEqual(iba->getItemPtr(ibaItem1), iba->blocks.getTop()+11);
    iba->clearBlocks();
    shouldBeEqual(iba->blocks.getNumItems(), 0);
    for (size_t i = 0; i < 1<<5; i++) {
      size_t ibaItem = iba->allocateNewStructure();
      shouldBeEqual(ibaItem, i);
    }
    shouldBeEqual(iba->blocks.getNumItems(), 2);
    ibaItem0 = iba->allocateNewStructure();
    shouldBeEqual(ibaItem0, (1<<5));
    shouldBeEqual(iba->blocks.getNumItems(), 3);
    shouldBeEqual(iba->getItemPtr(ibaItem0), iba->blocks.getTop());
    ibaItem1 = iba->allocateNewStructure();
    shouldBeEqual(ibaItem1, ((1<<5)+1));
    shouldBeEqual(iba->blocks.getNumItems(), 3);
    shouldBeEqual(iba->getItemPtr(ibaItem1), iba->blocks.getTop()+11);
    deleteTest(iba);
  } endIt();

  it("should NOT allow the use of allocateNewStructure(size_t)") {
    IndexedBlockAllocator *iba = new IndexedBlockAllocator(11, 4);
    shouldNotBeEqual(iba, NULL);
    for (size_t i = 0; i < 10; i++) {
      size_t ibaItem0 = iba->allocateNewStructure(5);
      shouldBeEqual(ibaItem0, 0);
    }
    deleteTest(iba);
  } endIt();

} endDescribe(IndexedBlockAllocator);
