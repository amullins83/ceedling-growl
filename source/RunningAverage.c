#include "RunningAverage.h"
#include <stdlib.h>

RunningAverage *newDefaultRunningAverage(void) {
    RunningAverage *ra = (RunningAverage *)malloc(sizeof(RunningAverage));
    initDefaultRunningAverage(ra);
    return ra;
}

RunningAverage *newRunningAverage(float initialAverage, TimeConstants tc) {
    RunningAverage *ra = (RunningAverage *)malloc(sizeof(RunningAverage));
    initRunningAverage(ra, initialAverage, tc);
    return ra;
}

void initDefaultRunningAverage(RunningAverage *ra) {
    initRunningAverage(ra, 1.0, getDefaultTimeConstants()[0]);
}

void initRunningAverage(RunningAverage *ra, float initialAverage, TimeConstants tc) {
    ra->newSample = 0;
    ra->average = initialAverage;

    ra->tc = tc;

    runningAverageCalcNumSpeeds(ra);
    runningAverageCalcNewSampleWeight(ra);
}

int runningAverageGetNewSample(RunningAverage *ra) {
    return ra->newSample;
}

void runningAverageSetNewSample(RunningAverage *ra, int sample) {
    ra->newSample = sample;
    runningAverageCalcNewSampleWeight(ra);
    ra->average = (ra->newSampleWeight*sample + (ra->numSpeeds - ra->newSampleWeight)*ra->average/ra->tc.sampleRate)*ra->tc.sampleRate/ra->numSpeeds;
}

void runningAverageSetNewSampleFloat(RunningAverage *ra, float sample) {
    ra->newSample = (int)sample;
    runningAverageCalcNewSampleWeight(ra);
    ra->average = (ra->newSampleWeight*sample + (ra->numSpeeds - ra->newSampleWeight)*ra->average/ra->tc.sampleRate)*ra->tc.sampleRate/ra->numSpeeds;
}

float runningAverageGetAverage(RunningAverage *ra) {
    return ra->average;
}

float runningAverageCalcNewSampleWeight(RunningAverage *ra) {
    // If we're at background, minimize the influence of new samples
    if(ra->average <= (float)ra->tc.maxTimeThreshold)
        ra->newSampleWeight = 1.0;
    // If we're above minTimeThreshold, maximize the influence of new samples
    else if(ra->average > ra->tc.minTimeThreshold) {
        // If we are restricting ourselves to a minimum time constant, honor it
        if(ra->tc.minTime > 1.0 / ra->tc.sampleRate)
            ra->newSampleWeight = (ra->tc.maxTime / ra->tc.minTime);
        // Otherwise, the effective time constant is the sampling time (weight is nearly 100%)
        else
            ra->newSampleWeight = ra->numSpeeds - 1.0;
    }
    // If we're between min and max, calculate the best new weight
    else if(ra->tc.minTime > 1.0 / ra->tc.sampleRate)
        // If we are limiting ourselves to a minimum time constant, maxTime/minTime is the highest
        // rate allowed (as utilized above). The linear interpolation below multiplies this maximum
        // weight by the ratio of our current average count to the maximum count. The plus and minus
        // 1 seen ensures that our minimum is 1 and our maximum is maxTime/minTime as expected.
        ra->newSampleWeight = ((ra->tc.maxTime / ra->tc.minTime - 1.0)*ra->average / ra->tc.minTimeThreshold) + 1.0;
    else
        // Otherwise, the desired maximum weight is numSpeeds - 1 (as seen above). The linear
        // interpolation below multiplies this by the ratio of the average count to the max count.
        // The plus and minus 1 seen ensures that our minimum is 1 and maximum is numSpeeds - 1 as
        // expected.
        ra->newSampleWeight = ((ra->numSpeeds - 2)*ra->average/ra->tc.minTimeThreshold) + 1.0;

    return ra->newSampleWeight;
}

void runningAverageSetSampleRate(RunningAverage *ra, int rate) {
    ra->tc.sampleRate = rate;
    runningAverageCalcNumSpeeds(ra);
}

void runningAverageSetMaxTime(RunningAverage *ra, float max) {
    ra->tc.maxTime = max;
    runningAverageCalcNumSpeeds(ra);
}

void runningAverageSetMinTime(RunningAverage *ra, float min) {
    ra->tc.minTime = min;
}

void runningAverageSetMinTimeThreshold(RunningAverage *ra, int threshold) {
    ra->tc.minTimeThreshold = threshold;
}

int runningAverageCalcNumSpeeds(RunningAverage *ra) {
    ra->numSpeeds = (int)(ra->tc.maxTime*ra->tc.sampleRate);
    return ra->numSpeeds;
}

int runningAverageGetMaxTimeThreshold(RunningAverage *ra) {
    return ra->tc.maxTimeThreshold;
}

void runningAverageSetMaxTimeThreshold(RunningAverage *ra, int threshold) {
    ra->tc.maxTimeThreshold = threshold;
}

void runningAverageSetTimeConstants(RunningAverage *ra, TimeConstants tc) {
    ra->tc = tc;
}
