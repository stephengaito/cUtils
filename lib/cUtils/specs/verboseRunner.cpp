#include <stdlib.h>
#include <stdio.h>

#include "cUtils/specs/verboseRunner.h"

VerboseRunner::VerboseRunner(FILE *aLogFile) {
  logFile          = aLogFile;
  inSideSizeValues = false;
};

VerboseRunner::~VerboseRunner(void) { };

void VerboseRunner::beginDescription(const char* message) {
  inSideSizeValues = false;
  fprintf(logFile, "%s\n", message);
};

int VerboseRunner::endDescription(void) {
  fprintf(logFile, "\n");
  inSideSizeValues = false;
  return 0;
};

void VerboseRunner::beginItSpec(const char* message) {
  clearInSideSizeValues();
  fprintf(logFile, "  %s\n", message);
};

void VerboseRunner::endItSpec(void) {
  clearInSideSizeValues();
};

void VerboseRunner::assertionFailure(AssertionFailure af) {
  clearInSideSizeValues();
  fprintf(logFile, "  --> assertion failure\n");
  fprintf(logFile, "  --> %s(%zu)\n", af.fileName, af.lineNumber);
  for (AssertionFailure::MessageHolder *curMessage = af.messages ;
       curMessage ; curMessage = curMessage->next) {
    fprintf(logFile, "  --> %s\n", curMessage->message);
  }
  if (af.messages) delete af.messages;
};

bool VerboseRunner::assertShouldEqual(bool sense,
         const char* actualStr,   int64_t actualValue,
         const char* expectedStr, int64_t expectedValue,
         const char* fileName, size_t lineNum) {
  clearInSideSizeValues();
  bool condition = (actualValue == expectedValue);
  if (condition != sense) {
    fprintf(logFile, "  >>> should %s be equal\n", (sense ? "" : "not"));
    fprintf(logFile, "  -->   actual: [%s] = %ld\n", actualStr, actualValue);
    fprintf(logFile, "  --> expected: [%s] = %ld\n", expectedStr, expectedValue);
    fprintf(logFile, "  --> file: %s(%zu)\n", fileName, lineNum);
  }
  return condition;
};

bool VerboseRunner::assertShouldEqual(bool sense,
         const char* actualStr,   void *actualValue,
         const char* expectedStr, void *expectedValue,
         const char* fileName, size_t lineNum) {
  clearInSideSizeValues();
  bool condition = (actualValue == expectedValue);
  if (condition != sense) {
    fprintf(logFile, "  >>> should %s be equal\n", (sense ? "" : "not"));
    fprintf(logFile, "  -->   actual: [%s] = %p\n", actualStr, actualValue);
    fprintf(logFile, "  --> expected: [%s] = %p\n", expectedStr, expectedValue);
    fprintf(logFile, "  --> file: %s(%zu)\n", fileName, lineNum);
  }
  return condition;
};

bool VerboseRunner::assertShouldEqual(bool sense,
         const char* actualStr,   const char *actualValue,
         const char* expectedStr, const char *expectedValue,
         const char* fileName, size_t lineNum) {
  clearInSideSizeValues();
  bool condition = (actualValue == expectedValue);
  if (condition != sense) {
    fprintf(logFile, "  >>> should %s be equal\n", (sense ? "" : "not"));
    fprintf(logFile, "  -->   actual: [%s] = [%s](%p)\n", actualStr, actualValue, actualValue);
    fprintf(logFile, "  --> expected: [%s] = [%s}(%p)\n", expectedStr, expectedValue, expectedValue);
    fprintf(logFile, "  --> file: %s(%zu)\n", fileName, lineNum);
  }
  return condition;
};

void VerboseRunner::logSize(const char* objTypeName, size_t objTypeSize) {
  setInSideSizeValues();
  fprintf(logFile, "  SIZE: %s = %zu bytes (%zu bits)\n",
          objTypeName, objTypeSize, objTypeSize*8);
};

void VerboseRunner::logValueUInt(const char* valueName, size_t value) {
  setInSideSizeValues();
  fprintf(logFile, "  VALUE: %s = %zu\n", valueName, value);
};

void VerboseRunner::logValueHInt(const char* valueName, size_t value) {
  setInSideSizeValues();
  fprintf(logFile, "  VALUE: %s = 0x%zX\n", valueName, value);
};

void VerboseRunner::logValueInt(const char* valueName, long value) {
  setInSideSizeValues();
  fprintf(logFile, "  VALUE: %s = %ld\n", valueName, value);
};

void VerboseRunner::logValueDbl(const char* valueName, double value) {
  setInSideSizeValues();
  fprintf(logFile, "  VALUE: %s = %f\n", valueName, value);
};

void VerboseRunner::setInSideSizeValues(void) {
  if (!inSideSizeValues) {
    inSideSizeValues = true;
    fprintf(logFile, "\n");
  }
}

void VerboseRunner::clearInSideSizeValues(void) {
  if (inSideSizeValues) {
    inSideSizeValues = false;
    fprintf(logFile, "\n");
  }
}
