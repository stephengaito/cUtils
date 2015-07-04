#include <string.h>
#include <stdio.h>
#include <exception>

#include <cUtils/specs/specs.h>

#ifndef protected
#define protected public
#endif

#include <stdio.h>
#include <cUtils/indexedBlockAllocator.h>

/// \brief We test the correctness of the C-based
/// IndexedBlockAllocator structure.
///
describe(IndexedBlockAllocator) {

  specSize(IndexedBlockAllocator);

  it("should create an IndexedBlockAllocator") {
    IndexedBlockAllocator *iba = new IndexedBlockAllocator(11, 4);
    shouldNotBeNULL(iba);
    shouldBeEqual(iba->itemSize, 11);
    shouldBeEqual(iba->bitShift, 4);
    delete iba;
  } endIt();

  it("can allocate a new item") {
    IndexedBlockAllocator *iba = new IndexedBlockAllocator(11, 4);
    shouldNotBeNULL(iba);
    size_t iba0 = iba->allocateNewStructure();
    size_t iba1 = iba->allocateNewStructure();
    shouldBeEqual(iba0, 0);
    shouldBeEqual(iba1, 1);
    shouldBeEqual(iba->getItemPtr(iba0), iba->blocks.getTop());
    shouldBeEqual(iba->getItemPtr(iba1), iba->blocks.getTop()+11);
    delete iba;
  } endIt();

  it("can allocate lots of new items") {
    IndexedBlockAllocator *iba = new IndexedBlockAllocator(11, 4);
    shouldNotBeNULL(iba);
    shouldBeZero(iba->blocks.getNumItems());
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
    delete iba;
  } endIt();

  it("can allocate lots of new items, clear them and then allocate some more") {
    IndexedBlockAllocator *iba = new IndexedBlockAllocator(11, 4);
    shouldNotBeNULL(iba);
    shouldBeZero(iba->blocks.getNumItems());
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
    shouldBeZero(iba->blocks.getNumItems());
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
    delete iba;
  } endIt();

  it("should NOT allow the use of allocateNewStructure(size_t)") {
    IndexedBlockAllocator *iba = new IndexedBlockAllocator(11, 4);
    shouldNotBeNULL(iba);
    for (size_t i = 0; i < 10; i++) {
      size_t ibaItem0 = iba->allocateNewStructure(5);
      shouldBeZero(ibaItem0);
    }
    delete iba;
  } endIt();

} endDescribe(IndexedBlockAllocator);
