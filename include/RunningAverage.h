/*
 * File:   RunningAverage.h
 * Author: amullins
 *
 * Declarations for code to create a RunningAverage structure that automatically
 * adjusts the weight of new samples based on the current average. As the average
 * increases, new samples have more effect on the resulting average. This serves to
 * smooth out readings when the input is based on a Poisson distribution.
 *
 * Created on October 3, 2013, 9:55 AM
 */

#ifndef __RUNNING_AVERAGE_H
#define __RUNNING_AVERAGE_H

#include "time_constants.h"

typedef struct {
    int newSample;
    float average;
    TimeConstants tc;
    int numSpeeds;
    float newSampleWeight;
} RunningAverage;

RunningAverage *newDefaultRunningAverage(void);
RunningAverage *newRunningAverage(float initialAverage, TimeConstants tc);

void  initDefaultRunningAverage(RunningAverage *ra);
void  initRunningAverage(RunningAverage *ra, float initialAverage, TimeConstants tc);

int   runningAverageGetNewSample(RunningAverage *ra);
void  runningAverageSetNewSample(RunningAverage *ra, int sample);
void  runningAverageSetNewSampleFloat(RunningAverage *ra, float sample);
float runningAverageGetAverage(RunningAverage *ra);

float runningAverageCalcNewSampleWeight(RunningAverage *ra);

void  runningAverageSetSampleRate(RunningAverage *ra, int rate);
void  runningAverageSetMaxTime(RunningAverage *ra, float max);
void  runningAverageSetMinTime(RunningAverage *ra, float min);

int   runningAverageGetMaxTimeThreshold(RunningAverage *ra);
void  runningAverageSetMaxTimeThreshold(RunningAverage *ra, int threshold);

int   runningAverageGetMinTimeThreshold(RunningAverage *ra);
void  runningAverageSetMinTimeThreshold(RunningAverage *ra, int threshold);


int   runningAverageCalcNumSpeeds(RunningAverage *ra);

void  runningAverageSetTimeConstants(RunningAverage *ra, TimeConstants tc);

#endif
