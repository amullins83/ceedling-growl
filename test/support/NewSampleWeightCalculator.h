#ifndef NEW_SAMPLE_WEIGHT_H
#define NEW_SAMPLE_WEIGHT_H

#include "RunningAverage.h"

typedef struct {
    float *average;
    int *weight;
    int *expectedWeight;
    int length;
    RunningAverage *ra;
} NewSampleWeightCalculator;

void newSampleWeightCalcSetAverage(NewSampleWeightCalculator *nswc, float *average);
void newSampleWeightCalcSetExpected(NewSampleWeightCalculator *nswc, int *expected);

int  newSampleWeightsMatchExpected(NewSampleWeightCalculator *nswc);

int newSampleWeightsTest(RunningAverage *ra, int *correctWeights, float *averages);

#endif
