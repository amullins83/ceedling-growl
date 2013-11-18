#include "NewSampleWeightCalculator.h"
#include "RunningAverage.h"
#include <stdlib.h>

NewSampleWeightCalculator *newSampleWeightCalculator(RunningAverage *ra)
{
    NewSampleWeightCalculator *nswc = (NewSampleWeightCalculator *)malloc(sizeof(NewSampleWeightCalculator));
    nswc->expectedWeight = NULL;
    nswc->average = NULL;
    nswc->length = 0;
    nswc->ra = ra;
    return nswc;
}

int *dynamicArrayMoveInt(int *oldArray, int oldLength, int newLength) {
    int i;
    int *newArray = (int *)malloc(sizeof(int)*newLength);

    for(i = 0; (i < oldLength) && (i < newLength); i++) {
        newArray[i] = oldArray[i];
    }

    free(oldArray);

    return newArray;
}

float *dynamicArrayMoveFloat(float *oldArray, int oldLength, int newLength) {
    int i;
    float *newArray = (float *)malloc(sizeof(float)*newLength);

    for(i = 0; (i < oldLength) && (i < newLength); i++) {
        newArray[i] = oldArray[i];
    }

    free(oldArray);

    return newArray;
}

void newSampleWeightCalcSetAverage(NewSampleWeightCalculator *nswc, float *average) {
    int i, oldLength = 0;

    if(!nswc) {
        nswc = newSampleWeightCalculator(NULL);
    }
    else
        oldLength = nswc->length;

    for(i = 0; average[i] > 0.001; i++);
    nswc->length = i;

    if(nswc->average) {
        free(nswc->average);
    }
    else
        nswc->average = (float *)malloc(sizeof(float)*nswc->length);

    if(nswc->expectedWeight) {
        if(nswc->length != oldLength) {
            nswc->expectedWeight = dynamicArrayMoveInt(nswc->expectedWeight, oldLength, nswc->length);
        }
    }
    else
        nswc->expectedWeight = (int *)malloc(sizeof(int)*nswc->length);

    for(i = 0; i < nswc->length; i++)
    {
        nswc->average[i] = average[i];
    }

}

void newSampleWeightCalcSetExpected(NewSampleWeightCalculator *nswc, int *expected) {
    int i, oldLength = 0;

    if(!nswc) {
        nswc = newSampleWeightCalculator(NULL);
    }
    else
        oldLength = nswc->length;

    for(i = 0; expected[i] > 0; i++);
    nswc->length = i;

    if(nswc->expectedWeight) {
        free(nswc->expectedWeight);
    }
    else
        nswc->expectedWeight = (int *)malloc(sizeof(int)*nswc->length);

    if(nswc->average) {
        if(nswc->length != oldLength) {
            nswc->average = dynamicArrayMoveFloat(nswc->average, oldLength, nswc->length);
        }
    }
    else
        nswc->average = (float *)malloc(sizeof(float)*nswc->length);

    for(i = 0; i < nswc->length; i++)
    {
        nswc->expectedWeight[i] = expected[i];
    }

}

int  newSampleWeightsMatchExpected(NewSampleWeightCalculator *nswc) {
    int i;

    if(!nswc) return 0;

    for(i = 0; i < nswc->length; i++) {
        nswc->ra->average = nswc->average[i];
        if(nswc->expectedWeight[i] != runningAverageCalcNewSampleWeight(nswc->ra))
            return 0;
    }

    return 1;
}

int newSampleWeightsTest(RunningAverage *ra, int *correctWeights, float *averages) {
    int pass = 0;
    NewSampleWeightCalculator *nswc = newSampleWeightCalculator(ra);
    newSampleWeightCalcSetExpected(nswc, correctWeights);
    
    newSampleWeightCalcSetAverage(nswc, averages);

    pass = newSampleWeightsMatchExpected(nswc);
    free(nswc);
    return pass;
}
