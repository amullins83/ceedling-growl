#include "unity.h"
#include "RunningAverage.h"
#include "NewSampleWeightCalculator.h"
#include "newSampleWeightTests.h"
#include <stdlib.h>


RunningAverage *ra;

void setUp(void)
{
    ra = (RunningAverage *)malloc(sizeof(RunningAverage));
}

void tearDown(void)
{
    if(ra) free(ra);
}

/* newDefault */
void test_newDefault_returns_pointer(void)
{
    tearDown();
    ra = newDefaultRunningAverage();
    TEST_ASSERT_NOT_NULL(ra);
}

void test_newDefault_initialization(void)
{
    RunningAverage *newRA = (RunningAverage *)malloc(sizeof(RunningAverage));
    initDefaultRunningAverage(newRA);

    TEST_ASSERT_EQUAL(newRA->average, ra->average);
    TEST_ASSERT_EQUAL(newRA->sampleRate, ra->sampleRate);
    TEST_ASSERT_EQUAL(newRA->minTime, ra->minTime);
    TEST_ASSERT_EQUAL(newRA->maxTime, ra->maxTime);
    TEST_ASSERT_EQUAL(newRA->maxCountsPerSecond, ra->maxCountsPerSecond);
}

/* initDefault */
void test_initDefault_initialize(void)
{
    initDefaultRunningAverage(ra);

    TEST_ASSERT_EQUAL(1.0, ra->average);
    TEST_ASSERT_EQUAL(200, ra->maxCountsPerSecond);
    TEST_ASSERT_EQUAL(8, ra->sampleRate);
    TEST_ASSERT_EQUAL(0, ra->minTime);
    TEST_ASSERT_EQUAL(5, ra->maxTime);
}

/* new */
void test_new_returns_pointer(void)
{
    float testAverage = 0.0;
    int   testSampleRate = 16;
    int   testMinTime = 1;
    int   testMaxTime = 6;
    int   testMaxCountsPerSecond = 100;

    tearDown();

    ra = newRunningAverage(testAverage, testSampleRate, testMinTime, testMaxTime, testMaxCountsPerSecond);
    
    TEST_ASSERT_NOT_NULL(ra);
}

void test_new_initialization(void)
{
    float testAverage = 0.0;
    int   testSampleRate = 16;
    int   testMinTime = 1;
    int   testMaxTime = 6;
    int   testMaxCountsPerSecond = 100;

    tearDown();

    ra = newRunningAverage(testAverage, testSampleRate, testMinTime, testMaxTime, testMaxCountsPerSecond);
    
    TEST_ASSERT_EQUAL(testAverage, ra->average);
    TEST_ASSERT_EQUAL(testSampleRate, ra->sampleRate);
    TEST_ASSERT_EQUAL(testMinTime, ra->minTime);
    TEST_ASSERT_EQUAL(testMaxTime, ra->maxTime);
    TEST_ASSERT_EQUAL(testMaxCountsPerSecond, ra->maxCountsPerSecond);
}

/* init */
void test_init_sets_parameters(void)
{
    float testAverage = 0.0;
    int   testSampleRate = 16;
    int   testMinTime = 1;
    int   testMaxTime = 6;
    int   testMaxCountsPerSecond = 100;
    initRunningAverage(ra, testAverage, testSampleRate, testMinTime, testMaxTime, testMaxCountsPerSecond);
    TEST_ASSERT_EQUAL(testAverage, ra->average);
    TEST_ASSERT_EQUAL(testSampleRate, ra->sampleRate);
    TEST_ASSERT_EQUAL(testMinTime, ra->minTime);
    TEST_ASSERT_EQUAL(testMaxTime, ra->maxTime);
    TEST_ASSERT_EQUAL(testMaxCountsPerSecond, ra->maxCountsPerSecond);
}

// int   runningAverageGetNewSample(RunningAverage *ra);
void test_get_newSample(void)
{
    int testNewSample = 20;
    ra->newSample = testNewSample;
    TEST_ASSERT_EQUAL(testNewSample, runningAverageGetNewSample(ra));
}

// void  runningAverageSetNewSample(RunningAverage *ra, int sample);
void test_set_newSample_sets_newSample(void)
{
    int testNewSample = 20;
    runningAverageSetNewSample(ra, testNewSample);
    TEST_ASSERT_EQUAL(testNewSample, ra->newSample);
}

void test_set_newSample_updates_average(void)
{
    int testNewSample = 20;
    initDefaultRunningAverage(ra);

    runningAverageSetNewSample(ra, testNewSample);
    
    TEST_ASSERT_NOT_EQUAL(1.0, ra->average);
}

// float runningAverageGetAverage(RunningAverage *ra);
void test_get_average(void)
{
    float testAverage = 2.0;
    ra->average = testAverage;
    TEST_ASSERT_EQUAL(testAverage, runningAverageGetAverage(ra));
}

// int   runningAverageCalcNewSampleWeight(RunningAverage *ra);
void test_calcNewSampleWeight_defaults(void)
{
    float testAverage[] = {1.0, 5.0, 50.0, 400.0, 0.0};
    float correctNewSampleWeight[] = {1.0, 1.0, 10.0, 39.0, 0.0};

    initDefaultRunningAverage(ra);

    calcNewSampleWeightTest(ra, testAverage, correctNewSampleWeight);
}

void test_calcNewSampleWeight_with_minTime(void)
{
    float testAverage[] = {1.0, 5.0, 50.0, 400.0, 800.0, 0.0};
    float correctNewSampleWeight[] = {1.0, 1.0, 4.0, 10.0, 10.0, 0.0};

    initDefaultRunningAverage(ra);

    runningAverageSetMinTime(ra, 1);
    runningAverageSetMaxTime(ra, 10);
    runningAverageSetMaxCountsPerSecond(ra, 150);

    calcNewSampleWeightTest(ra, testAverage, correctNewSampleWeight);
}

void test_calcNewSampleWeight_with_background(void)
{
    float testAverage[] = {1.0, 5.0, 50.0, 400.0, 800.0, 0.0};
    float correctNewSampleWeight[] = {1.0, 1.0, 27.0, 79.0, 79.0, 0.0};

    initDefaultRunningAverage(ra);

    runningAverageSetBackgroundCountsPerSecond(ra, 5);
    runningAverageSetMaxTime(ra, 10);
    runningAverageSetMaxCountsPerSecond(ra, 150);

    calcNewSampleWeightTest(ra, testAverage, correctNewSampleWeight);
}


// void  runningAverageSetSampleRate(RunningAverage *ra, int rate);
void test_set_sampleRate(void)
{
    int testSampleRate = 24;

    runningAverageSetSampleRate(ra, testSampleRate);
    TEST_ASSERT_EQUAL(testSampleRate, ra->sampleRate);
}

// void  runningAverageSetMaxTime(RunningAverage *ra, int max);
void test_setMaxTime(void)
{
    int testMaxTime = 10;

    runningAverageSetMaxTime(ra, testMaxTime);
    TEST_ASSERT_EQUAL(testMaxTime, ra->maxTime);
}

// void  runningAverageSetMinTime(RunningAverage *ra, int min);
void test_setMinTime(void)
{
    int testMinTime = 2;

    runningAverageSetMinTime(ra, testMinTime);
    TEST_ASSERT_EQUAL(testMinTime, ra->minTime);
}

// void  runningAverageSetMaxCountsPerSecond(RunningAverage *ra, int max);
void test_setMaxCountsPerSecond(void)
{
    int testMaxCounts = 300;

    runningAverageSetMaxCountsPerSecond(ra, testMaxCounts);
    TEST_ASSERT_EQUAL(testMaxCounts, ra->maxCountsPerSecond);
}

// void  runningAverageCalcNumSpeeds(RunningAverage *ra);
void test_calcNumSpeeds(void)
{
    float testMaxTime = 1.25;
    int testSampleRate = 8;
    int correctNumSpeeds = (int)(testMaxTime*testSampleRate);

    initRunningAverage(ra, 1.0, testSampleRate, 0.0, testMaxTime, 200);

    TEST_ASSERT_EQUAL(correctNumSpeeds, runningAverageCalcNumSpeeds(ra));
}

// int   runningAverageGetBackgroundCountsPerSecond(RunningAverage *ra);
void test_get_background(void)
{
    int testBackground = 4;

    ra->backgroundCountsPerSecond = testBackground;

    TEST_ASSERT_EQUAL(testBackground, runningAverageGetBackgroundCountsPerSecond(ra));
}

// void  runningAverageSetBackgroundCountsPerSecond(RunningAverage *ra, int background);
void test_set_background(void)
{
    int testBackground = 5;

    initDefaultRunningAverage(ra);

    runningAverageSetBackgroundCountsPerSecond(ra, testBackground);

    TEST_ASSERT_EQUAL(testBackground, ra->backgroundCountsPerSecond);
}
