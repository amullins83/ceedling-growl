#include "unity.h"
#include "RunningAverage.h"
#include "time_constants.h"
#include "NewSampleWeightCalculator.h"
#include "newSampleWeightTests.h"
#include "verbose_assert_float.h"
#include "type_instrument.h"
#include "instrument_constants.h"
#include "model_constants.h"
#include "models.h"
#include <stdlib.h>


RunningAverage *ra;

TYPE_INSTRUMENT lmi;
InstrumentConstants *ic;

void setUp(void)
{
    ra = newDefaultRunningAverage();
}

void tearDown(void)
{
    free(ra);
}

/* newDefault */
void test_newDefault_returns_pointer(void)
{
    RunningAverage *newRa = newDefaultRunningAverage();
    TEST_ASSERT_NOT_NULL(newRa);
    free(newRa);
}

void test_newDefault_initialization(void)
{
    RunningAverage *newRa = (RunningAverage *)malloc(sizeof(RunningAverage));
    initDefaultRunningAverage(newRa);

    TEST_ASSERT_EQUAL(newRa->average, ra->average);
    TEST_ASSERT_EQUAL_MEMORY(&newRa->tc, &ra->tc, sizeof(TimeConstants));
}

/* initDefault */
void test_initDefault_initialize(void)
{
    TimeConstants *tc_array = getDefaultTimeConstants();
    initDefaultRunningAverage(ra);

    TEST_ASSERT_EQUAL(1.0, ra->average);
    TEST_ASSERT_EQUAL_MEMORY(&tc_array[0], &ra->tc, sizeof(TimeConstants));
}

/* new */
void test_new_returns_pointer(void)
{
    float testAverage = 0.0;
    TimeConstants testTc;
    testTc.sampleRate = 16;
    testTc.minTime = 1;
    testTc.maxTime = 6;
    testTc.minTimeThreshold = 100;
    testTc.maxTimeThreshold = 10;

    RunningAverage *newRa = newRunningAverage(testAverage, testTc);
    
    TEST_ASSERT_NOT_NULL(ra);
}

void test_new_initialization(void)
{
    float testAverage = 0.0;
    TimeConstants testTc;
    testTc.sampleRate = 16;
    testTc.minTime = 1;
    testTc.maxTime = 6;
    testTc.minTimeThreshold = 100;
    testTc.maxTimeThreshold = 10;

    ra = newRunningAverage(testAverage, testTc);
    
    TEST_ASSERT_EQUAL(testAverage, ra->average);
    TEST_ASSERT_EQUAL_MEMORY(&testTc, &ra->tc, sizeof(TimeConstants));
}

/* init */
void test_init_sets_parameters(void)
{
    float testAverage = 0.0;
    TimeConstants testTc;
    testTc.sampleRate = 16;
    testTc.minTime = 1;
    testTc.maxTime = 6;
    testTc.minTimeThreshold = 100;
    testTc.maxTimeThreshold = 10;

    initRunningAverage(ra, testAverage, testTc);
    TEST_ASSERT_EQUAL(testAverage, ra->average);
    TEST_ASSERT_EQUAL_MEMORY(&testTc, &ra->tc, sizeof(TimeConstants));
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
    float correctNewSampleWeight[] = {1.0, 1.0, 9.125, 66.0, 0.0};

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
    runningAverageSetMinTimeThreshold(ra, 150);

    calcNewSampleWeightTest(ra, testAverage, correctNewSampleWeight);
}

void test_calcNewSampleWeight_with_background(void)
{
    float testAverage[] = {1.0, 5.0, 50.0, 400.0, 800.0, 0.0};
    float correctNewSampleWeight[] = {1.0, 1.0, 27.0, 79.0, 79.0, 0.0};

    initDefaultRunningAverage(ra);

    runningAverageSetMaxTimeThreshold(ra, 5);
    runningAverageSetMaxTime(ra, 10);
    runningAverageSetMinTimeThreshold(ra, 150);

    calcNewSampleWeightTest(ra, testAverage, correctNewSampleWeight);
}


// void  runningAverageSetSampleRate(RunningAverage *ra, int rate);
void test_set_sampleRate(void)
{
    int testSampleRate = 24;

    runningAverageSetSampleRate(ra, testSampleRate);
    TEST_ASSERT_EQUAL(testSampleRate, ra->tc.sampleRate);
}

// void  runningAverageSetMaxTime(RunningAverage *ra, int max);
void test_setMaxTime(void)
{
    int testMaxTime = 10;

    runningAverageSetMaxTime(ra, testMaxTime);
    TEST_ASSERT_EQUAL(testMaxTime, ra->tc.maxTime);
}

// void  runningAverageSetMinTime(RunningAverage *ra, int min);
void test_setMinTime(void)
{
    int testMinTime = 2;

    runningAverageSetMinTime(ra, testMinTime);
    TEST_ASSERT_EQUAL(testMinTime, ra->tc.minTime);
}

// void  runningAverageSetMinTimeThreshold(RunningAverage *ra, int max);
void test_setMinTimeThreshold(void)
{
    int testMaxCounts = 300;

    runningAverageSetMinTimeThreshold(ra, testMaxCounts);
    TEST_ASSERT_EQUAL(testMaxCounts, ra->tc.minTimeThreshold);
}

// void  runningAverageCalcNumSpeeds(RunningAverage *ra);
void test_calcNumSpeeds(void)
{
    float testMaxTime = 1.25;
    int testSampleRate = 8;
    int correctNumSpeeds = (int)(testMaxTime*testSampleRate);
    TimeConstants *testTc = getDefaultTimeConstants();
    testTc[0].maxTime = testMaxTime;
    testTc[0].sampleRate = testSampleRate;

    initRunningAverage(ra, 1.0, testTc[0]);

    TEST_ASSERT_EQUAL(correctNumSpeeds, runningAverageCalcNumSpeeds(ra));
}

// int   runningAverageGetMaxTimeThreshold(RunningAverage *ra);
void test_get_background(void)
{
    int testThreshold = 4;

    ra->tc.maxTimeThreshold = testThreshold;

    TEST_ASSERT_EQUAL(testThreshold, runningAverageGetMaxTimeThreshold(ra));
}

// void  runningAverageSetMaxTimeThreshold(RunningAverage *ra, int background);
void test_set_background(void)
{
    int testThreshold = 5;

    initDefaultRunningAverage(ra);

    runningAverageSetMaxTimeThreshold(ra, testThreshold);

    TEST_ASSERT_EQUAL(testThreshold, ra->tc.maxTimeThreshold);
}
