#include <stdlib.h>
#include <stdio.h>

#include "cUtils/specs/verboseRunner.h"

VerboseRunner::VerboseRunner(FILE *aLogFile) {
  logFile = aLogFile;
};

VerboseRunner::~VerboseRunner(void) { };

void VerboseRunner::beginDescription(const char* message) {
  fprintf(logFile, "%s\n", message);
};

int VerboseRunner::endDescription(void) {
  fprintf(logFile, "\n");
  return 0;
};

void VerboseRunner::beginItSpec(const char* message) {
  fprintf(logFile, "  %s\n", message);
};

void VerboseRunner::endItSpec(void) {
  // do nothing
};

void VerboseRunner::assertionFailure(AssertionFailure af) {
  fprintf(logFile, "  --> assertion failure\n");
  fprintf(logFile, "  --> %s(%zu)\n", af.fileName, af.lineNumber);
  for (AssertionFailure::MessageHolder *curMessage = af.messages ;
       curMessage ; curMessage = curMessage->next) {
    fprintf(logFile, "  --> %s\n", curMessage->message);
  }
  if (af.messages) delete af.messages;
};

void VerboseRunner::assertShouldEqual(bool condition, bool sense,
         const char* actualStr, const char* expectedStr,
         const char* fileName, size_t lineNum) {
  if (condition != sense) {
    fprintf(logFile, "  --> should %s be equal\n", (sense ? "" : "not"));
    fprintf(logFile, "  -->   actual: [%s]\n", actualStr);
    fprintf(logFile, "  --> expected: [%s]\n", expectedStr);
    fprintf(logFile, "  --> file: %s(%zu)\n", fileName, lineNum);
  }
};

void VerboseRunner::logSize(const char* objTypeName, size_t objTypeSize) {
  fprintf(logFile, "  SIZE: %s = %zu bytes (%zu bits)\n",
          objTypeName, objTypeSize, objTypeSize*8);
};

void VerboseRunner::logValueUInt(const char* valueName, size_t value) {
  fprintf(logFile, "  VALUE: %s = %zu\n", valueName, value);
};

void VerboseRunner::logValueHInt(const char* valueName, size_t value) {
  fprintf(logFile, "  VALUE: %s = 0x%zX\n", valueName, value);
};

void VerboseRunner::logValueInt(const char* valueName, long value) {
  fprintf(logFile, "  VALUE: %s = %ld\n", valueName, value);
};

void VerboseRunner::logValueDbl(const char* valueName, double value) {
  fprintf(logFile, "  VALUE: %s = %f\n", valueName, value);
};
