#include <stdlib.h>
#include <stdio.h>

#include "cUtils/specs/verboseRunner.h"

VerboseRunner::VerboseRunner(FILE *aLogFile) {
  logFile                   = aLogFile;

  inSideSizeValues          = false;
  descriptionFailed         = false;
  specFailed                = false;

  numDescriptions           = 0;
  numSpecs                  = 0;
  numShoulds                = 0;
  numFailedDescriptions     = 0;
  numFailedSpecs            = 0;
  numFailedShoulds          = 0;
  numSuccessfulDescriptions = 0;
  numSuccessfulSpecs        = 0;
  numSuccessfulShoulds      = 0;
  numPendingDescriptions    = 0;
  numPendingSpecs           = 0;
};

VerboseRunner::~VerboseRunner(void) { };

void VerboseRunner::beginDescription(const char* message, bool runDescribe) {
  inSideSizeValues = false;
  descriptionFailed = false;
  numDescriptions++;
  if (runDescribe) {
    fprintf(logFile, "%s\n", message);
    descriptionPending = false;
  } else {
    fprintf(logFile, "PENDING: %s\n", message);
    descriptionPending = true;
    numPendingDescriptions++;
  }
};

int VerboseRunner::endDescription(void) {
  fprintf(logFile, "\n");
  inSideSizeValues = false;
  if (descriptionFailed) {
    numFailedDescriptions++;
    return 1;
  }
  if (!descriptionPending) numSuccessfulDescriptions++;
  return 0;
};

void VerboseRunner::beginItSpec(const char* message, bool runIt) {
  clearInSideSizeValues();
  specFailed = false;
  numSpecs++;
  if (runIt) {
    fprintf(logFile, "  %s\n", message);
    specPending = false;
  } else {
    fprintf(logFile, "--PENDING: %s\n", message);
    specPending = true;
    numPendingSpecs++;
  }
};

void VerboseRunner::endItSpec(void) {
  clearInSideSizeValues();
  if (specFailed) {
    descriptionFailed = true;
    numFailedSpecs++;
  } else if (!specPending) numSuccessfulSpecs++;
};

#define SHOULD_FAILED		\
  specFailed        = true;	\
  descriptionFailed = true;	\
  numFailedShoulds++

void VerboseRunner::assertionFailure(AssertionFailure af) {
  clearInSideSizeValues();
  fprintf(logFile, "-->>> assertion failure\n");
  fprintf(logFile, "----> %s(%zu)\n", af.fileName, af.lineNumber);
  for (AssertionFailure::MessageHolder *curMessage = af.messages ;
       curMessage ; curMessage = curMessage->next) {
    fprintf(logFile, "----> %s\n", curMessage->message);
  }
  if (af.messages) delete af.messages;
  SHOULD_FAILED;
};

bool VerboseRunner::assertShouldEqual(bool sense,
         const char* actualStr,   int64_t actualValue,
         const char* expectedStr, int64_t expectedValue,
         const char* fileName, size_t lineNum) {
  clearInSideSizeValues();
  numShoulds++;
  bool condition = (actualValue == expectedValue);
  if (condition != sense) {
    fprintf(logFile, "-->>> should %s be equal\n", (sense ? "" : "not"));
    fprintf(logFile, "---->   actual: [%s] = %lld\n", actualStr, actualValue);
    fprintf(logFile, "----> expected: [%s] = %lld\n", expectedStr, expectedValue);
    fprintf(logFile, "----> file: %s(%zu)\n", fileName, lineNum);
    SHOULD_FAILED;
  } else numSuccessfulShoulds++;
  return condition;
};

bool VerboseRunner::assertShouldEqual(bool sense,
         const char* actualStr,   void *actualValue,
         const char* expectedStr, void *expectedValue,
         const char* fileName, size_t lineNum) {
  clearInSideSizeValues();
  numShoulds++;
  bool condition = (actualValue == expectedValue);
  if (condition != sense) {
    fprintf(logFile, "-->>> should %s be equal\n", (sense ? "" : "not"));
    fprintf(logFile, "---->   actual: [%s] = %p\n", actualStr, actualValue);
    fprintf(logFile, "----> expected: [%s] = %p\n", expectedStr, expectedValue);
    fprintf(logFile, "----> file: %s(%zu)\n", fileName, lineNum);
    SHOULD_FAILED;
  } else numSuccessfulShoulds++;
  return condition;
};

bool VerboseRunner::assertShouldEqual(bool sense,
         const char* actualStr,   const char *actualValue,
         const char* expectedStr, const char *expectedValue,
         const char* fileName, size_t lineNum) {
  clearInSideSizeValues();
  numShoulds++;
  bool condition = (strcmp(actualValue, expectedValue) == 0);
  if (condition != sense) {
    fprintf(logFile, "-->>> should %s be equal\n", (sense ? "" : "not"));
    fprintf(logFile, "---->   actual: [%s] = [%s](%p)\n", actualStr, actualValue, actualValue);
    fprintf(logFile, "----> expected: [%s] = [%s}(%p)\n", expectedStr, expectedValue, expectedValue);
    fprintf(logFile, "----> file: %s(%zu)\n", fileName, lineNum);
    SHOULD_FAILED;
  } else numSuccessfulShoulds++;
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

void VerboseRunner::logReport(void) {
  fprintf(logFile, "------------------------------------------------------\n");
  fprintf(logFile, "              Failed    Succeeded       Pending Total\n");
  fprintf(logFile, "Descriptions: %zu \t%zu\t\t%zu\t%zu\n",
          numFailedDescriptions,  numSuccessfulDescriptions,
          numPendingDescriptions, numDescriptions);
  fprintf(logFile, "       Specs: %zu \t%zu\t\t%zu\t%zu\n",
          numFailedSpecs, numSuccessfulSpecs, numPendingSpecs, numSpecs);
  fprintf(logFile, "     Shoulds: %zu \t%zu\t\t%zu\t%zu\n",
          numFailedShoulds, numSuccessfulShoulds, (size_t)0, numShoulds);
  fprintf(logFile, "------------------------------------------------------\n");
}
