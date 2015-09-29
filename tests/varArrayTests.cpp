#include <string.h>
#include <stdio.h>
#include <exception>

#include <cUtils/specs/specs.h>

#ifndef protected
#define protected public
#endif

#include <stdio.h>
#include <cUtils/varArray.h>


/// \brief We test the correctness of the C-based VarArray structure.
describe(VarArray) {

  specSize(VarArray<int>);
  specSize(VarArray<const char*>);

  it("should be created with correct values when instantiated with int") {
    VarArray<int> aVarArray;
    shouldBeZero(aVarArray.numItems);
    shouldBeZero(aVarArray.arraySize);
    shouldBeNULL(aVarArray.itemArray);
    aVarArray.~VarArray<int>();
    shouldBeNULL(aVarArray.itemArray);
    shouldBeZero(aVarArray.numItems);
    shouldBeZero(aVarArray.arraySize);
  } endIt();

  it("should be able to push and pop lots of items when instantiated with int") {
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
    aVarArray.~VarArray<int>();
    shouldBeNULL(aVarArray.itemArray);
    shouldBeZero(aVarArray.numItems);
    shouldBeZero(aVarArray.arraySize);
  } endIt();

  it("should be created with correct values when instantiated with const char*") {
    VarArray<const char*> aVarArray;
    shouldBeZero(aVarArray.numItems);
    shouldBeZero(aVarArray.arraySize);
    shouldBeNULL(aVarArray.itemArray);
    aVarArray.~VarArray<const char*>();
    shouldBeNULL(aVarArray.itemArray);
    shouldBeZero(aVarArray.numItems);
    shouldBeZero(aVarArray.arraySize);
  } endIt();

  it("should be able to push/pop lots of items when instantiated with const char*") {
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
    size_t arraySize = aVarArray.arraySize;
    for(size_t i = 100; 0 < i; i--) {
      shouldBeEqual(aVarArray.popItem(), (i-1));
      shouldBeEqual(aVarArray.getNumItems(), (i-1));
      shouldBeEqual(aVarArray.arraySize, (arraySize));
    }
    shouldBeZero(aVarArray.getNumItems());
    shouldBeEqual(aVarArray.arraySize, (arraySize));
    aVarArray.~VarArray<int>();
    shouldBeNULL(aVarArray.itemArray);
    shouldBeZero(aVarArray.numItems);
    shouldBeZero(aVarArray.arraySize);
  } endIt();

  it("should be able to push/pop lots of items when instantiated with const char*") {
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
    size_t arraySize = aVarArray.arraySize;
    for(size_t i = 100; 0 < i; i--) {
      shouldBeEqual((void*)aVarArray.popItem(), ((void*)(i-1)));
      shouldBeEqual(aVarArray.getNumItems(), (i-1));
      shouldBeEqual(aVarArray.arraySize, (arraySize));
    }
    shouldBeZero(aVarArray.getNumItems());
    shouldBeEqual(aVarArray.arraySize, (arraySize));
    aVarArray.~VarArray<const char*>();
    shouldBeNULL(aVarArray.itemArray);
    shouldBeZero(aVarArray.numItems);
    shouldBeZero(aVarArray.arraySize);
  } endIt();

} endDescribe(VarArray);

