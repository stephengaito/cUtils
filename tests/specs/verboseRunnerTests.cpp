
#ifndef protected
#define protected public
#endif

#include <cUtils/specs/verboseRunner.h>

class SimpleTestClass {
public:

  bool invariant(void) {
    throw AssertionFailure("invariant failed");
    return false;
  }

  SimpleTestClass(void) {
    testInt = 1;
  };

  ~SimpleTestClass(void) {
    ASSERT_INSIDE_DELETE(invariant());
    ASSERT_INSIDE_DELETE(false);
    testInt = 0;
  };

  int testInt;
};

describeMM(VerboseRunner, "should have two failures") {

  specSize(VerboseRunner);

  it("simple AssertionFailure", "SHOULD FAIL") {
    throw AssertionFailure("simple assertion failure");
  } endIt();

  it("simple ASSERT failure", "SHOULD FAIL", "WITH Recent call stack") {
   ASSERT_MESSAGE(false, "this is an assert with message");
  } endIt();

  it("ASSERT failure inside delete", "SHOULD FAIL",
     "BUT throwing exceptions inside destructors is not supported",
     "AND results in immediate program termination") {
    SimpleTestClass *testClass = new SimpleTestClass();
    delete testClass;
  } endIt();

} endDescribe(VerboseRunner);
