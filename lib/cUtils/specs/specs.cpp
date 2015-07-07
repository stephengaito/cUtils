#include <stdlib.h>

#include "cUtils/specs/specs.h"
#include "cUtils/specs/verboseRunner.h"

SpecRunner *SpecRunner::runner = NULL;

typedef struct RunnerInfo {
  RunnerInfo *next;
  RunnerFunc func;
} RunnerInfo;

static RunnerInfo *runnerList = NULL;

int SpecRunner::registerRunner(RunnerFunc aRunner) {
  RunnerInfo *newInfo =
    (RunnerInfo*)calloc(1, sizeof(RunnerInfo));
    newInfo->next = NULL;
    newInfo->func = aRunner;

  if (runnerList) {
    RunnerInfo *curRunner = runnerList;
    for ( ; curRunner->next ; curRunner = curRunner->next);
    curRunner->next = newInfo;
  } else {
    runnerList = newInfo;
  }
  return 1;
}

int SpecRunner::runAllUsing(SpecRunner *aRunner) {
  runner = aRunner;
  int numFailures = 0;
  RunnerInfo *curRunner = runnerList;
  for ( ; curRunner ; curRunner = curRunner->next) {
    numFailures += (*(curRunner->func))();
  };
  aRunner->logReport();
  return numFailures;
}

SpecRunner *SpecRunner::get(void) {
  if (!runner) runner = new VerboseRunner();
  return runner;
}

//SpecRunner::SpecRunner(void) { };

SpecRunner::~SpecRunner(void) { };

void SpecRunner::beginDescription(bool runDescribe,
                                  const char *firstMessage,
                                  ...) { };

int SpecRunner::endDescription(void) { return 0; };

void SpecRunner::beginItSpec(bool runIt,
                             const char *firstMessage,
                             ...) { };

void SpecRunner::endItSpec(void) { };

void SpecRunner::assertionFailure(AssertionFailure af) {  };

void SpecRunner::assertShouldReachThisPoint(bool sense,
                                            const char *fileName,
                                            size_t lineNum,
                                            ...) { };

bool SpecRunner::assertShouldEqual(bool sense,
         const char* actualStr,   int64_t actualValue,
         const char* expectedStr, int64_t expectedValue,
         const char* fileName, size_t lineNum) {
  return (actualValue == expectedValue);
};

bool SpecRunner::assertShouldEqual(bool sense,
         const char* actualStr,   void *actualValue,
         const char* expectedStr, void *expectedValue,
         const char* fileName, size_t lineNum) {
  return (actualValue == expectedValue);
};

bool SpecRunner::assertShouldEqual(bool sense,
         const char* actualStr,   const char *actualValue,
         const char* expectedStr, const char *expectedValue,
         const char* fileName, size_t lineNum) {
  return (actualValue == expectedValue);
};

void SpecRunner::logSize(const char* objTypeName, size_t objTypeSize) { };
void SpecRunner::logValueUInt(const char* valueName, size_t value) { };
void SpecRunner::logValueHInt(const char* valueName, size_t value) { };
void SpecRunner::logValueInt(const char* valueName, long value) { };
void SpecRunner::logValueDbl(const char* valueName, double   value) { };

void SpecRunner::logReport(void) { };
