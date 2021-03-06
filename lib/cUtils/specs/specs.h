#ifndef CUTILS_SPECS_H
#define CUTILS_SPECS_H

#include <stdarg.h>

#include "cUtils/assertions.h"

/// \def pending_describe(className)
/// \brief Describes a collection of specifications.
///
/// The className provided MUST be able to be used a the name of a C
/// function.
///
/// There MUST be a corresponding endDescribe macro call.
#define pending_describe(className)				\
int run ## className(void) {					\
  SpecRunner::get()->beginDescription(false, #className, NULL);	\
  if (false) try

/// \def pending_describeMM(className, ...)
/// \brief Describes a collection of specifications using multiple messages.
///
/// The className provided MUST be able to be used a the name of a C
/// function.
///
/// There MUST be a corresponding endDescribe macro call.
#define pending_describeMM(className, ...)				     \
int run ## className(void) {						     \
  SpecRunner::get()->beginDescription(false, #className, __VA_ARGS__, NULL); \
  if (false) try

/// \def describe(className)
/// \brief Describes a collection of specifications.
///
/// The className provided MUST be able to be used a the name of a C
/// function.
///
/// There MUST be a corresponding endDescribe macro call.
#define describe(className)					\
int run ## className(void) {					\
  SpecRunner::get()->beginDescription(true, #className, NULL);	\
  try

/// \def describeMM(className, ...)
/// \brief Describes a collection of specifications using multiple messages.
///
/// The className provided MUST be able to be used a the name of a C
/// function.
///
/// There MUST be a corresponding endDescribe macro call.
#define describeMM(className, ...)					    \
int run ## className(void) {						    \
  SpecRunner::get()->beginDescription(true, #className, __VA_ARGS__, NULL); \
  try

/// \def endDescribe(className)
/// \brief Ends the description of a collection of specifications.
///
/// The className MUST be able to be used as the name of a C function
/// AND it MUST correspond to the same name used in the corresponding
/// describe(className).
#define endDescribe(className)				\
  catch (AssertionFailure af) {				\
    SpecRunner::get()->assertionFailure(af);		\
  } catch (SpecificationFailed sf) {			\
    /* do nothing */					\
  }							\
  return SpecRunner::get()->endDescription();		\
}							\
static int localRunner =				\
  SpecRunner::registerRunner(run ## className)

/// \def pending_it(message)
/// \brief Opens an "it" specification which will not be run.
///
/// There MUST be a corresponding endIt();
#define pending_it(...)						\
  SpecRunner::get()->beginItSpec(false, __VA_ARGS__, NULL);	\
  if (false) try 

/// \def it(message)
/// \brief Opens an "it" specification.
///
/// There MUST be a corresponding endIt();
#define it(...)							\
  SpecRunner::get()->beginItSpec(true, __VA_ARGS__, NULL);	\
  try

/// \def endIt()
/// \brief Closes an "it" specification.
#define endIt()						\
  catch (AssertionFailure af) {				\
    SpecRunner::get()->assertionFailure(af);		\
  } catch (SpecificationFailed sf) {			\
    /* do nothing */					\
  }							\
  SpecRunner::get()->endItSpec()

/// \def shouldReachThisPoint()
/// \brief Asserts that we have succeeded if we reach this point.
#define shouldReachThisPoint()				\
  SpecRunner::get()->assertShouldReachThisPoint(true,	\
    __FILE__, __LINE__, NULL)

/// \def shouldNotReachThisPoint(message, ...)
/// \brief Asserts that we have failed by reaching this point,
/// so we should report the message provided.
#define shouldNotReachThisPoint(...)			\
  SpecRunner::get()->assertShouldReachThisPoint(false,	\
    __FILE__, __LINE__, __VA_ARGS__, NULL)

/// \def shouldBeZero(actual)
/// \brief Asserts that the actual value should be 0.
#define shouldBeZero(actual)				\
  SpecRunner::get()->assertShouldEqual(			\
    true, #actual, (actual), "0", 0, __FILE__, __LINE__)

/// \def shouldNotBeZero(actual)
/// \brief Asserts that the actual value should not be 0.
#define shouldNotBeZero(actual)				\
  SpecRunner::get()->assertShouldEqual(			\
    false, #actual, (actual), "0", 0, __FILE__, __LINE__)

/// \def shouldBeNULL(actual)
/// \brief Asserts that the actual value should be NULL.
#define shouldBeNULL(actual)				\
  SpecRunner::get()->assertShouldEqual(			\
    true, #actual, (actual), "NULL", ((void*)NULL), __FILE__, __LINE__)

/// \def shouldNotBeNULL(actual)
/// \brief Asserts that the actual value should not be NULL.
#define shouldNotBeNULL(actual)						\
  if (SpecRunner::get()->assertShouldEqual(				\
    false, #actual, (actual), "NULL", ((void*)NULL), __FILE__, __LINE__)) \
    throw SpecificationFailed();

/// \def shouldBeTrue(actual)
/// \brief Asserts that the actual value should be true.
#define shouldBeTrue(actual)				\
  SpecRunner::get()->assertShouldEqual(			\
    true, #actual, (actual), "true", true, __FILE__, __LINE__)

/// \def shouldBeFalse(actual)
/// \brief Asserts that the actual value should be false.
#define shouldBeFalse(actual)				\
  SpecRunner::get()->assertShouldEqual(			\
    true, #actual, (actual), "false", false, __FILE__, __LINE__)

/// \def shouldBeEqual(actual, expected)
/// \brief Asserts that actual and expected should be equal.
#define shouldBeEqual(actual, expected)				\
  SpecRunner::get()->assertShouldEqual(				\
    true, #actual, (actual), #expected, (expected), __FILE__, __LINE__)

/// \def shouldNotBeEqual(actual, expected)
/// \brief Asserts that actual and expected should NOT be equal.
#define shouldNotBeEqual(actual, expected)			\
  SpecRunner::get()->assertShouldEqual(				\
    false, #actual, (actual), #expected, (expected), __FILE__, __LINE__)

/// \def specSize(objectType)
/// \brief Reports the size in bytes and bits of a given object type.
#define specSize(objType) SpecRunner::get()->logSize(#objType, sizeof(objType))

/// \def specUValue(integerValue)
/// \brief Reports the value of a given unsigned integer.
#define specUValue(objValue) SpecRunner::get()->logValueUInt(#objValue, ((size_t)(objValue)));

/// \def specHValue(integerValue)
/// \brief Reports the hexadecimal value of a given unsigned integer
#define specHValue(objValue) SpecRunner::get()->logValueHInt(#objValue, ((size_t)(objValue)));

/// \brief SpecificationFailed is thrown if a specification failed
/// (such as shouldNotBeNULL) for which it is unlikely the rest of the
/// specification should continue.
class SpecificationFailed {
  // At the moment we only care about the existence of this object
  // but do not care about any internal details.
};

/// \brief (Internal) The signature of a describe function to be run by
/// a SpecRunner.
typedef int (*RunnerFunc)(void);

/// \brief A (very) simple BDD/RSpec/CSpec inspired specification framework.
///
/// The SpecRunner class and its subclasses (together with a collection
/// of C-Macros) manage the registration and specification testing of
/// C/C++ code.
///
/// A typical usage might be:
///
/// \code{.cpp}
///
///  #include <cUnit/specs/specs.h>
///
///  describe(VarArray) {
///
///    specSize(VarArray<int>);
///
///    it("should be created with the correct values") {
///      VarArray<int> aVarArray;
///      shouldBeEqual(aVarArray.numItems, 0);
///    } endIt();
///
///  } endDescribe(VarArray);
///
///  int main(int argc, char* argv[]) {
///    return SpecRunner::runAllUsing(new VerboseRunner());
///  }
///
/// \endcode
class SpecRunner {
public:

  /// \brief Destroy a SpecRunner.
  virtual ~SpecRunner(void);

  /// \brief Instrument the begining of a description.
  virtual void beginDescription(bool runDescribe,
                                const char *firstMessage,
                                ...); // uses varargs!

  /// \brief Instrument the end of a description.
  virtual int endDescription(void);

  /// \brief Instrument the begining of a "it" specfication.
  virtual void beginItSpec(bool runIt,
                           const char *firstMessage,
                           ...); // uses varargs!

  /// \brief Instrument the end of a "it" specfication.
  virtual void endItSpec(void);

  /// \brief Report an assertion failure.
  virtual void assertionFailure(AssertionFailure af);

  /// \brief Report the lack of success in reaching a given point.
  virtual void assertShouldReachThisPoint(bool sense,
                                          const char *fileName, size_t lineNum,
                                          ...); // uses varargs!

  /// \brief Report the failure of an equal/not-equal condition.
  ///
  /// Returns the value of the condition.
  virtual bool assertShouldEqual(bool sense,
                 const char* actualStr,   int64_t actualValue,
                 const char* expectedStr, int64_t expectedValue,
                 const char* fileName, size_t lineNum);

  /// \brief Report the failure of an equal/not-equal condition.
  ///
  /// Returns the value of the condition.
  virtual bool assertShouldEqual(bool sense,
                 const char* actualStr,   void *actualValue,
                 const char* expectedStr, void *expectedValue,
                 const char* fileName, size_t lineNum);

  /// \brief Report the failure of an equal/not-equal condition.
  ///
  /// Returns the value of the condition.
  virtual bool assertShouldEqual(bool sense,
                 const char* actualStr,   const char *actualValue,
                 const char* expectedStr, const char *expectedValue,
                 const char* fileName, size_t lineNum);

  /// \brief Report the size of a given object type.
  virtual void logSize(const char* objTypeName, size_t objTypeSize);

  /// \brief Report the value of an unsigned integer.
  virtual void logValueUInt(const char* valueName, size_t value);

  /// \brief Report the Hexadecimal value of an unsigned integer.
  virtual void logValueHInt(const char* valueName, size_t value);

  /// \brief Report the value of an signed integer.
  virtual void logValueInt(const char* valueName, long value);

  /// \brief Report the value of an floating point number.
  virtual void logValueDbl(const char* valueName, double   value);

  /// \brief Report the success/failure of these specs.
  virtual void logReport(void);

  /// \brief Register the SpecRunner (or subclass) which will be used
  /// to manage the specifications.
  static int registerRunner(RunnerFunc aRunner);

  /// \brief Run all registered specifications using the SpecRunner
  /// provided.
  static int runAllUsing(SpecRunner *aSpecRunner);

  /// \brief (Internal) get the currently registered SpecRunner
  /// instance.
  static SpecRunner *get(void);

protected:

  /// \brief The currently registered SpecRunner instance.
  static SpecRunner *runner;

};

#endif
