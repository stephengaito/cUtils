#ifndef ASSERTIONS_H
#define ASSERTIONS_H

// This header file provides a simple Assertion framework which allows
// more information to be provided than simply the failure of a given
// condition.
//
// If the symbol DEBUG is not defined, then no ASSERTion is ever
// compiled.
//
// If the symbol DEBUG is defined, then ASSERTions are compiled and
// more usefully, the function names of recent calls on the stack are
// appended to the failure message.

#ifndef DEBUG

//////////////////////////////////////////////////////////////////////////////
// WE ARE NOT IN DEBUG/DEVELOPMENT
// SO REMOVE ALL ASSERTS
#define ASSERT(cond)
#define ASSERT_MESSAGE(cond, message)
#define FalseOrAssertionFailure(message) return false
//////////////////////////////////////////////////////////////////////////////

#else // DEBUG defined

//////////////////////////////////////////////////////////////////////////////
// WE want the full debug/development assertion system
//////////////////////////////////////////////////////////////////////////////

//#ifndef BANDIT_ASSERTION_EXCEPTION_H
//#include <cassert>
//#include <functional>
//#include <iostream>
//#include <list>
//#include <deque>
//#include <stdexcept>
//#include "bandit/assertion_exception.h"
//#endif

#include <string>
#include <string.h>
#include <stdlib.h>
#include <execinfo.h>
#include <errno.h>
#include <cxxabi.h>
#define MAX_FRAMES 10

/// \brief The AssertionFailure class provides a convenient way to
/// report more information than the simple failure of an assertion.
///
/// The AssertionFailure exception, when used inside of the provided
/// ASSERT or ASSERT_MESSAGE macros, adds recent call stack information
/// using ideas suggested by Rafael Baptista's excellent blog article:
/// [Generate Stack Traces on Crash Portably in
/// C++](http://oroboro.com/stack-trace-on-crash/)
///
/// The AsertionFailure class as well as the ASSERT/ASSERT_MESSAGE
/// macros are only defined if the C/C++ macro DEBUG has been defined.
///
/// To use this assertion framework the bandit/assertion_exception.h
/// file MUST be installed in either the system or local include
/// directories. A copy of this file can be [download
/// here](https://github.com/joakimkarlsson/bandit)
struct AssertionFailure {

  /// \brief Construct a new instance of AssertionFailure.
  AssertionFailure(const char *aMessage, bool shouldFreeMessage = false) {
    message     = aMessage;
    fileName    = NULL;
    lineNumber  = 0;
    freeMessage = shouldFreeMessage;
  };

  /// \brief Construct a new instance of AssertionFailure which
  /// explicitly contains file name and line number information.
  ///
  /// This can be used together with Bandit tests outside of the
  /// Assertion framework.
  AssertionFailure(const char   *aMessage,
                   const char   *aFileName,
                   const size_t  aLineNumber,
                   bool shouldFreeMessage = false) {
    message     = aMessage;
    fileName    = aFileName;
    lineNumber  = aLineNumber;
    freeMessage = shouldFreeMessage;
  };

  /// \brief Add the recent function call information from the call
  /// stack and then rethrow the exception.
  ///
  /// This method is typically, automatically, called by the ASSERT
  /// and/or ASSERT_MESSAGE macros.
  static void addStackTrace(const AssertionFailure& af,
                            const char   *aFileName,
                            const size_t  aLineNumber) {
    // now add the filename and line numbers provided....
    // and add recent call stack information...
    std::string messageBuf(af.message);
    void *stackReturnAddressList[MAX_FRAMES+1];
    size_t numFrames = backtrace(stackReturnAddressList, MAX_FRAMES);
    if (1 < numFrames) {
      messageBuf += "\nRecent call stack:";
      if (4 < numFrames) numFrames = 4;
      char **stackFrameSymbols =
        backtrace_symbols(stackReturnAddressList, numFrames);
      for (size_t i = 1; i < numFrames; i++) {
        messageBuf += "\n  ";
        char *callName   = NULL;
        /*char *callOffset = NULL;*/
        char *localSymbols = strdup(stackFrameSymbols[i]);
        for ( char *p = localSymbols; *p; p++) {
          if (*p == '(') callName = ++p;
          if (*p == '+') { *p = 0; /*callOffset = ++p;*/ }
          if (*p == ')') *p = 0;
        }
        size_t functionNameBufferSize = 1024;
        char functionNameBuffer[1024];
        int status = 0;
        char *functionNameResult =
          abi::__cxa_demangle(callName, functionNameBuffer,
                              &functionNameBufferSize, &status);
        char *functionName = callName;
        if (status == 0) functionName = functionNameResult;
        messageBuf += functionName;
        if (localSymbols) free(localSymbols);
      }
      if (stackFrameSymbols) free(stackFrameSymbols);
    }
    throw AssertionFailure(strdup(messageBuf.c_str()),
                           aFileName, aLineNumber, true);
  }

  const char *message;
  const char *fileName;
  size_t     lineNumber;
  bool       freeMessage;
};

#define ASSERT(condition)						 \
  try {									 \
    if (!(condition)) throw AssertionFailure("("#condition") is false"); \
  } catch (const AssertionFailure& af) {				 \
    AssertionFailure::addStackTrace(af, __FILE__, __LINE__);		 \
  }

#define ASSERT_MESSAGE(condition, message)				 \
  try {									 \
    if (!(condition)) throw AssertionFailure("("#condition") is false "#message); \
  } catch (const AssertionFailure& af) {				 \
    AssertionFailure::addStackTrace(af, __FILE__, __LINE__);		 \
  }

#define FalseOrAssertionFailure(message) throw AssertionFailure(message)

#endif // DEBUG defined

#endif // ASSERTIONS_H not defined
