#include <string.h>
#include <stdio.h>
#include <exception>

#include <cUtils/specs/specs.h>

#ifndef protected
#define protected public
#endif

#include <stdio.h>
#include <cUtils/varArray.h>

/// \brief We test the correctness of the C-based VarArrayIterator 
/// structure.
describe(VarArrayIterator) {

  specSize(VarArrayIterator<int>);
  specSize(VarArrayIterator<const char*>);

  it("should iterate over an existing VarArray<int>") {
    VarArray<int> aVarArray;
    shouldBeZero(aVarArray.numItems);
    shouldBeZero(aVarArray.arraySize);
    shouldBeNULL(aVarArray.itemArray);
    for (size_t i = 0; i < 100; i++) {
      aVarArray.pushItem(i);
      shouldBeEqual(aVarArray.getNumItems(), (i+1));
      shouldNotBeZero(aVarArray.arraySize);
      shouldBeEqual(aVarArray.itemArray[i], (i));
    }
    shouldBeEqual(aVarArray.numItems,      (100));
    VarArrayIterator<int> iter = aVarArray.getIterator();
    size_t i = 0;
    for ( ; iter.hasMoreItems() ; i++ ) {
      int anItem = iter.nextItem();
      shouldBeEqual(anItem, i);
    }
    shouldBeEqual(i, 100);
  } endIt();

  it("should iterate over an existing VarArray<char *>") {
    VarArray<const char*> aVarArray;
    shouldBeZero(aVarArray.numItems);
    shouldBeZero(aVarArray.arraySize);
    shouldBeNULL(aVarArray.itemArray);
    for (size_t i = 0; i < 100; i++) {
      aVarArray.pushItem((char*)i);
      shouldBeEqual(aVarArray.getNumItems(), (i+1));
      shouldNotBeZero(aVarArray.arraySize);
      shouldBeEqual((void*)aVarArray.itemArray[i], ((void*)i));
    }
    shouldBeEqual(aVarArray.numItems,      (100));
    VarArrayIterator<const char*> iter = aVarArray.getIterator();
    size_t i = 0;
    for ( ; iter.hasMoreItems() ; i++ ) {
      const char *anItem = iter.nextItem();
      shouldBeEqual((void*)anItem, (void*)i);
    }
    shouldBeEqual(i, 100);
  } endIt();

} endDescribe(VarArrayIterator);

