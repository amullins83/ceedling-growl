#include "newSampleWeightTests.h"
#include "RunningAverage.h"
#include "unity.h"
#include "verbose_assert_float.h"

void calcNewSampleWeightTest(RunningAverage *ra, float *testAverages, float *correctWeights) {
    int i;
    for(i = 0; testAverages[i] > 0.001; i++) {
        ra->average = testAverages[i];
        verboseAssertFloatWithin(0.5, correctWeights[i], runningAverageCalcNewSampleWeight(ra));
    }
}
