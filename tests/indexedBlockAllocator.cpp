#include <bandit/bandit.h>
using namespace bandit;

#include <string.h>
#include <stdio.h>
#include <exception>

#ifndef protected
#define protected public
#endif

#include <stdio.h>
#include <cUtils/indexedBlockAllocator.h>

go_bandit([](){

  printf("\n----------------------------------\n");
  printf(  "indexedBlockAllocator\n");
  printf(  "IndexedBlockAllocator = %zu bytes (%zu bits)\n", sizeof(IndexedBlockAllocator), sizeof(IndexedBlockAllocator)*8);
  printf(  "----------------------------------\n");

  /// \brief We test the correctness of the C-based
  /// IndexedBlockAllocator structure.
  ///
  describe("IndexedBlockAllocator", [](){

    it("should create an IndexedBlockAllocator", [](){
      IndexedBlockAllocator *iba = new IndexedBlockAllocator(11, 4);
      AssertThat(iba, Is().Not().EqualTo((void*)0));
      AssertThat(iba->itemSize, Equals(11));
      AssertThat(iba->bitShift, Equals(4));
      delete iba;
    });

    it("can allocate a new item", [](){
      IndexedBlockAllocator *iba = new IndexedBlockAllocator(11, 4);
      AssertThat(iba, Is().Not().EqualTo((void*)0));
      size_t iba0 = iba->allocateNewStructure();
      size_t iba1 = iba->allocateNewStructure();
      AssertThat(iba0, Equals(0));
      AssertThat(iba1, Equals(1));
      AssertThat(iba->getItemPtr(iba0), Equals(iba->blocks.getTop()));
      AssertThat(iba->getItemPtr(iba1), Equals(iba->blocks.getTop()+11));
      delete iba;
    });

    it("can allocate lots of new items", [](){
      IndexedBlockAllocator *iba = new IndexedBlockAllocator(11, 4);
      AssertThat(iba, Is().Not().EqualTo((void*)0));
      AssertThat(iba->blocks.getNumItems(), Equals(0));
      for (size_t i = 0; i < 1<<5; i++) {
        size_t ibaItem = iba->allocateNewStructure();
        AssertThat(ibaItem, Equals(i));
      }
      AssertThat(iba->blocks.getNumItems(), Equals(2));
      size_t ibaItem0 = iba->allocateNewStructure();
      AssertThat(ibaItem0, Equals(1<<5));
      AssertThat(iba->blocks.getNumItems(), Equals(3));
      AssertThat(iba->getItemPtr(ibaItem0), Equals(iba->blocks.getTop()));
      size_t ibaItem1 = iba->allocateNewStructure();
      AssertThat(ibaItem1, Equals((1<<5)+1));
      AssertThat(iba->blocks.getNumItems(), Equals(3));
      AssertThat(iba->getItemPtr(ibaItem1), Equals(iba->blocks.getTop()+11));
      delete iba;
    });

    it("can allocate lots of new items, clear them and then allocate some more", [](){
      IndexedBlockAllocator *iba = new IndexedBlockAllocator(11, 4);
      AssertThat(iba, Is().Not().EqualTo((void*)0));
      AssertThat(iba->blocks.getNumItems(), Equals(0));
      for (size_t i = 0; i < 1<<5; i++) {
        size_t ibaItem = iba->allocateNewStructure();
        AssertThat(ibaItem, Equals(i));
      }
      AssertThat(iba->blocks.getNumItems(), Equals(2));
      size_t ibaItem0 = iba->allocateNewStructure();
      AssertThat(ibaItem0, Equals(1<<5));
      AssertThat(iba->blocks.getNumItems(), Equals(3));
      AssertThat(iba->getItemPtr(ibaItem0), Equals(iba->blocks.getTop()));
      size_t ibaItem1 = iba->allocateNewStructure();
      AssertThat(ibaItem1, Equals((1<<5)+1));
      AssertThat(iba->blocks.getNumItems(), Equals(3));
      AssertThat(iba->getItemPtr(ibaItem1), Equals(iba->blocks.getTop()+11));
      iba->clearBlocks();
      AssertThat(iba->blocks.getNumItems(), Equals(0));
      for (size_t i = 0; i < 1<<5; i++) {
        size_t ibaItem = iba->allocateNewStructure();
        AssertThat(ibaItem, Equals(i));
      }
      AssertThat(iba->blocks.getNumItems(), Equals(2));
      ibaItem0 = iba->allocateNewStructure();
      AssertThat(ibaItem0, Equals(1<<5));
      AssertThat(iba->blocks.getNumItems(), Equals(3));
      AssertThat(iba->getItemPtr(ibaItem0), Equals(iba->blocks.getTop()));
      ibaItem1 = iba->allocateNewStructure();
      AssertThat(ibaItem1, Equals((1<<5)+1));
      AssertThat(iba->blocks.getNumItems(), Equals(3));
      AssertThat(iba->getItemPtr(ibaItem1), Equals(iba->blocks.getTop()+11));
      delete iba;
    });

    it("should NOT allow the use of allocateNewStructure(size_t)", [](){
      IndexedBlockAllocator *iba = new IndexedBlockAllocator(11, 4);
      AssertThat(iba, Is().Not().EqualTo((void*)0));
      for (size_t i = 0; i < 10; i++) {
        size_t ibaItem0 = iba->allocateNewStructure(5);
        AssertThat(ibaItem0, Equals(0));
      }
    });

  }); // indexedBlockAllocator
}); // go_bandit
