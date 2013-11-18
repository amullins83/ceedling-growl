#include "RunningAverage.h"
#include <stdlib.h>

RunningAverage *newDefaultRunningAverage(void) {
    RunningAverage *ra = (RunningAverage *)malloc(sizeof(RunningAverage));
    initDefaultRunningAverage(ra);
    return ra;
}

RunningAverage *newRunningAverage(float initialAverage, int sampleRate, float minTime, float maxTime, int maxCountsPerSecond) {
    RunningAverage *ra = (RunningAverage *)malloc(sizeof(RunningAverage));
    initRunningAverage(ra, initialAverage, sampleRate, minTime, maxTime, maxCountsPerSecond);
    return ra;
}

void initDefaultRunningAverage(RunningAverage *ra) {
    initRunningAverage(ra, 1.0, 8, 0, 5, 200);
}

void initRunningAverage(RunningAverage *ra, float initialAverage, int sampleRate, float minTime, float maxTime, int maxCountsPerSecond) {
    ra->newSample = 0;
    ra->average = initialAverage;

    ra->minTime = minTime;
    ra->maxTime = maxTime;
    ra->sampleRate = sampleRate;
    ra->maxCountsPerSecond = maxCountsPerSecond;
    ra->backgroundCountsPerSecond = 10;

    runningAverageCalcNumSpeeds(ra);
    runningAverageCalcNewSampleWeight(ra);
}

int runningAverageGetNewSample(RunningAverage *ra) {
    return ra->newSample;
}

void runningAverageSetNewSample(RunningAverage *ra, int sample) {
    ra->newSample = sample;
    runningAverageCalcNewSampleWeight(ra);
    ra->average = (ra->newSampleWeight*sample + (ra->numSpeeds - ra->newSampleWeight)*ra->average/ra->sampleRate)*ra->sampleRate/ra->numSpeeds;
}

void runningAverageSetNewSampleFloat(RunningAverage *ra, float sample) {
    ra->newSample = (int)sample;
    runningAverageCalcNewSampleWeight(ra);
    ra->average = (ra->newSampleWeight*sample + (ra->numSpeeds - ra->newSampleWeight)*ra->average/ra->sampleRate)*ra->sampleRate/ra->numSpeeds;
}

float runningAverageGetAverage(RunningAverage *ra) {
    return ra->average;
}

float runningAverageCalcNewSampleWeight(RunningAverage *ra) {
    // If we're at background, minimize the influence of new samples
    if(ra->average <= (float)ra->backgroundCountsPerSecond)
        ra->newSampleWeight = 1.0;
    // If we're above maxCountsPerSecond, maximize the influence of new samples
    else if(ra->average > ra->maxCountsPerSecond) {
        // If we are restricting ourselves to a minimum time constant, honor it
        if(ra->minTime > 1.0 / ra->sampleRate)
            ra->newSampleWeight = (ra->maxTime / ra->minTime);
        // Otherwise, the effective time constant is the sampling time (weight is nearly 100%)
        else
            ra->newSampleWeight = ra->numSpeeds - 1.0;
    }
    // If we're between min and max, calculate the best new weight
    else if(ra->minTime > 1.0 / ra->sampleRate)
        // If we are limiting ourselves to a minimum time constant, maxTime/minTime is the highest
        // rate allowed (as utilized above). The linear interpolation below multiplies this maximum
        // weight by the ratio of our current average count to the maximum count. The plus and minus
        // 1 seen ensures that our minimum is 1 and our maximum is maxTime/minTime as expected.
        ra->newSampleWeight = ((ra->maxTime / ra->minTime - 1.0)*ra->average / ra->maxCountsPerSecond) + 1.0;
    else
        // Otherwise, the desired maximum weight is numSpeeds - 1 (as seen above). The linear
        // interpolation below multiplies this by the ratio of the average count to the max count.
        // The plus and minus 1 seen ensures that our minimum is 1 and maximum is numSpeeds - 1 as
        // expected.
        ra->newSampleWeight = ((ra->numSpeeds - 2)*ra->average/ra->maxCountsPerSecond) + 1.0;

    return ra->newSampleWeight;
}

void runningAverageSetSampleRate(RunningAverage *ra, int rate) {
    ra->sampleRate = rate;
    runningAverageCalcNumSpeeds(ra);
}

void runningAverageSetMaxTime(RunningAverage *ra, float max) {
    ra->maxTime = max;
    runningAverageCalcNumSpeeds(ra);
}

void runningAverageSetMinTime(RunningAverage *ra, float min) {
    ra->minTime = min;
}

void runningAverageSetMaxCountsPerSecond(RunningAverage *ra, int max) {
    ra->maxCountsPerSecond = max;
}

int runningAverageCalcNumSpeeds(RunningAverage *ra) {
    ra->numSpeeds = (int)(ra->maxTime*ra->sampleRate);
    return ra->numSpeeds;
}

int runningAverageGetBackgroundCountsPerSecond(RunningAverage *ra) {
    return ra->backgroundCountsPerSecond;
}

void runningAverageSetBackgroundCountsPerSecond(RunningAverage *ra, int background) {
    ra->backgroundCountsPerSecond = background;
}
