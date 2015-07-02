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
  return numFailures;
}

SpecRunner *SpecRunner::get(void) {
  if (!runner) runner = new VerboseRunner();
  return runner;
}

//SpecRunner::SpecRunner(void) { };

SpecRunner::~SpecRunner(void) { };

void SpecRunner::beginDescription(const char* message) { };

int SpecRunner::endDescription(void) { return 0; };

void SpecRunner::beginItSpec(const char* message) { };

void SpecRunner::endItSpec(void) { };

void SpecRunner::assertionFailure(AssertionFailure af) {  };

void SpecRunner::assertShouldEqual(bool condition, bool sense,
         const char* actualStr, const char* expectedStr,
         const char* fileName, size_t lineNum) { };

void SpecRunner::logSize(const char* objTypeName, size_t objTypeSize) { };
void SpecRunner::logValueUInt(const char* valueName, size_t value) { };
void SpecRunner::logValueHInt(const char* valueName, size_t value) { };
void SpecRunner::logValueInt(const char* valueName, long value) { };
void SpecRunner::logValueDbl(const char* valueName, double   value) { };
