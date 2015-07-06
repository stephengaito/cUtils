#ifndef CUTILS_VERBOSE_RUNNER_H
#define CUTILS_VERBOSE_RUNNER_H

#include <stdio.h>
#include "cUtils/specs/specs.h"

/// \brief A (very) simple BDD/RSpec/CSpec inspired specification framework.
///
/// The VerboseRunner class (together with a collection
/// of C-Macros) manage the registration and specification testing of
/// C/C++ code using a rather verbose reporting style.
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
class VerboseRunner : public SpecRunner {
public:

  /// \brief Create a VerboseRunner.
  VerboseRunner(FILE *aLogFile = stdout);

  /// \brief Destroy a VerboseRunner.
  ~VerboseRunner(void);

  /// \brief Instrument the begining of a description.
  void beginDescription(bool runDescribe,
                        const char *firstMessage,
                        ...); // uses varargs

  /// \brief Instrument the end of a description.
  int endDescription(void);

  /// \brief Instrument the begining of a "it" specfication.
  void beginItSpec(bool runIt,
                   const char *firstMessage,
                   ...); // uses varargs

  /// \brief Instrument the end of a "it" specfication.
  void endItSpec(void);

  /// \brief Report an assertion failure.
  void assertionFailure(AssertionFailure af);

  /// \brief Report the failure of an equal/not-equal condition.
  ///
  /// Returns the value of the condition.
  bool assertShouldEqual(bool sense,
         const char* actualStr,   int64_t actualValue,
         const char* expectedStr, int64_t expectedValue,
         const char* fileName, size_t lineNum);

  /// \brief Report the failure of an equal/not-equal condition.
  ///
  /// Returns the value of the condition.
  bool assertShouldEqual(bool sense,
         const char* actualStr,   void *actualValue,
         const char* expectedStr, void *expectedValue,
         const char* fileName, size_t lineNum);

  /// \brief Report the failure of an equal/not-equal condition.
  ///
  /// Returns the value of the condition.
  bool assertShouldEqual(bool sense,
         const char* actualStr,   const char *actualValue,
         const char* expectedStr, const char *expectedValue,
         const char* fileName, size_t lineNum);

  /// \brief Report the size of a given object type.
  void logSize(const char* objTypeName, size_t objTypeSize);

  /// \brief Report the value of an unsigned integer.
  void logValueUInt(const char* valueName, size_t value);

  /// \brief Report the Hexadecimal value of an unsigned integer.
  void logValueHInt(const char* valueName, size_t value);

  /// \brief Report the value of an signed integer.
  void logValueInt(const char* valueName, long value);

  /// \brief Report the value of an floating point number.
  void logValueDbl(const char* valueName, double   value);

  void logReport(void);

  void setInSideSizeValues(void);
  void clearInSideSizeValues(void);

protected:

  /// \brief The io FILE pointer for the file/stream on which the
  /// reports should be made.
  ///
  /// By default this is stdout.
  FILE *logFile;

  bool inSideSizeValues;

  bool descriptionFailed;
  bool descriptionPending;
  bool specFailed;
  bool specPending;

  size_t numDescriptions;
  size_t numSpecs;
  size_t numShoulds;
  size_t numFailedDescriptions;
  size_t numFailedSpecs;
  size_t numFailedShoulds;
  size_t numSuccessfulDescriptions;
  size_t numSuccessfulSpecs;
  size_t numSuccessfulShoulds;
  size_t numPendingDescriptions;
  size_t numPendingSpecs;
};


#endif
