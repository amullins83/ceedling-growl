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

typedef struct {
    int newSample;
    float average;
    float minTime;
    float maxTime;
    int sampleRate;
    int backgroundCountsPerSecond;
    int maxCountsPerSecond;
    int numSpeeds;
    float newSampleWeight;
} RunningAverage;

RunningAverage *newDefaultRunningAverage(void);
RunningAverage *newRunningAverage(float initialAverage, int sampleRate, float minTime, float maxTime, int maxCountsPerSecond);

void  initDefaultRunningAverage(RunningAverage *ra);
void  initRunningAverage(RunningAverage *ra, float initialAverage, int sampleRate, float minTime, float maxTime, int maxCountsPerSecond);

int   runningAverageGetNewSample(RunningAverage *ra);
void  runningAverageSetNewSample(RunningAverage *ra, int sample);
void  runningAverageSetNewSampleFloat(RunningAverage *ra, float sample);
float runningAverageGetAverage(RunningAverage *ra);

float runningAverageCalcNewSampleWeight(RunningAverage *ra);

void  runningAverageSetSampleRate(RunningAverage *ra, int rate);
void  runningAverageSetMaxTime(RunningAverage *ra, float max);
void  runningAverageSetMinTime(RunningAverage *ra, float min);

int   runningAverageGetBackgroundCountsPerSecond(RunningAverage *ra);
void  runningAverageSetBackgroundCountsPerSecond(RunningAverage *ra, int background);

int   runningAverageGetMaxCountsPerSecond(RunningAverage *ra);
void  runningAverageSetMaxCountsPerSecond(RunningAverage *ra, int max);


int   runningAverageCalcNumSpeeds(RunningAverage *ra);

#endif
