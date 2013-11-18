#include "unity.h"
#include "mx_shims.h"
#include "models.h"
#include "instrument_constants.h"
#include "type_Instrument.h"
#include "GenericTypeDefs.h"
#include <stdlib.h>

InstrumentConstants *ic, *ic2;
int defaultID = 1000;
TYPE_INSTRUMENT lmi;
BOOL Update_Audio_Flag = FALSE;
UINT32 Period;
float Rate_uR_hr = 0.0;
TYPE_INSTRUMENT lmi;

void setUp(void)
{
    lmi.settings.product.info.model_number = MODEL_9DP;
    lmi.calibration.controls.info.fixed_range = 0;
    ic2 = newConstants(defaultID + 1);
    ic = newM9DPconstants(defaultID);
}

void tearDown(void)
{
    deleteInstrumentConstants(ic);
    deleteInstrumentConstants(ic2);
}

void test_newM9DPconstants_returns_pointer(void) {
    TEST_ASSERT_NOT_NULL(ic);
}

void test_newM9DPconstants_sets_id(void) {
    TEST_ASSERT_EQUAL(defaultID, ic->id);
}

void test_newM9DPconstants_sets_rangeConstants(void) {
    int testRange = 1;
    ic->current_range = testRange;
    RangeConstants testConstants;
    testConstants.squareFactor = 0;
    testConstants.rangeFactor = 45.47945;
    testConstants.magnitude = 1000.0;
    testConstants.fixedTimeConstants = (RangeHysteresisTimeConstants){500000.0, 100.0};
    testConstants.autoTimeConstants = (RangeHysteresisTimeConstants){500000.0, 450.0};
    testConstants.newRateWeight = 0.10;
    testConstants.minTime = 0.0;
    testConstants.maxTime = 60.0;
    testConstants.isFixedTime = 1;
    testConstants.fixedTime = 10.0;
    testConstants.max_uR = 480;

    TEST_ASSERT_EQUAL_MEMORY(&testConstants, &ic->rangeConstants[testRange - 1], sizeof(RangeConstants));
}

void test_newConstants_returns_pointer(void) {
    TEST_ASSERT_NOT_NULL(ic2);
}

void test_newConstants_sets_id(void) {
    TEST_ASSERT_EQUAL(defaultID + 1, ic2->id);
}

void test_newConstants_sets_rangeConstants(void) {
    int testRange = 2;
    ic->current_range = testRange;
    RangeConstants testConstants;
    testConstants.squareFactor = 0;
    testConstants.rangeFactor = 4.547945;
    testConstants.magnitude = 1000.0;
    testConstants.fixedTimeConstants = (RangeHysteresisTimeConstants){500000.0, 1000.0};
    testConstants.autoTimeConstants = (RangeHysteresisTimeConstants){164231.1, 4500.0};
    testConstants.newRateWeight = 0.14;
    testConstants.minTime = 0.0;
    testConstants.maxTime = 60.0;
    testConstants.isFixedTime = 1;
    testConstants.fixedTime = 7.14;
    testConstants.max_uR = 4800;

    TEST_ASSERT_EQUAL_MEMORY(&testConstants, &ic2->rangeConstants[testRange - 1], sizeof(RangeConstants));
}


void test_instrumentConstantsSetCalConstantForRange_sets_constant(void) {
    int testConstant = 1000;
    int testRange = 5;
    instrumentConstantsSetCalConstantForRange(ic, testRange, testConstant);
    TEST_ASSERT_EQUAL(testConstant, ic->calConstant[testRange - 1]);
}

void test_instrumentConstantsGetCurrentRange_returns_current_range(void) {
    int testRange = 5;
    ic->current_range = testRange;

    TEST_ASSERT_EQUAL(testRange, instrumentConstantsGetCurrentRange(ic));
}

void test_instrumentConstantsSetCurrentRange(void) {
    int testRange = 5;
    instrumentConstantsSetCurrentRange(ic, testRange);
    TEST_ASSERT_EQUAL(testRange, ic->current_range);
}

void test_instrumentConstantsGetCurrentCal(void) {
    int testRange = 5;
    instrumentConstantsSetCurrentRange(ic, testRange);
    TEST_ASSERT_EQUAL(ic->calConstant[testRange - 1], instrumentConstantsGetCurrentCal(ic));
}

void test_instrumentConstantsGetRangeFactor(void) {
    int testRange = 5;
    instrumentConstantsSetCurrentRange(ic, testRange);
    TEST_ASSERT_EQUAL_FLOAT(ic->rangeConstants[testRange - 1].rangeFactor, instrumentConstantsGetRangeFactor(ic));
}

void test_instrumentConstantsGetSquareFactor(void) {
    int testRange = 5;
    instrumentConstantsSetCurrentRange(ic, testRange);
    TEST_ASSERT_EQUAL_FLOAT(ic->rangeConstants[testRange - 1].squareFactor, instrumentConstantsGetSquareFactor(ic));
}

void test_instrumentConstantsGetMagnitude(void) {
    int testRange = 5;
    instrumentConstantsSetCurrentRange(ic, testRange);
    TEST_ASSERT_EQUAL_FLOAT(ic->rangeConstants[testRange - 1].magnitude, instrumentConstantsGetMagnitude(ic));   
}

void test_instrumentConstantsGetCoefficient(void) {
    int testRange = 5;
    lmi.calibration.controls.info.fixed_range = 0;
    instrumentConstantsSetCurrentRange(ic, testRange);
    TEST_ASSERT_EQUAL_FLOAT(ic->rangeConstants[testRange - 1].autoTimeConstants.coefficient, instrumentConstantsGetCoefficient(ic));   
}

void test_instrumentConstantsGetRate(void) {
    int testRange = 5;
    lmi.calibration.controls.info.fixed_range = 0;
    instrumentConstantsSetCurrentRange(ic, testRange);
    TEST_ASSERT_EQUAL_FLOAT(ic->rangeConstants[testRange - 1].autoTimeConstants.rate, instrumentConstantsGetRate(ic));
}

void test_instrumentConstantsGetWeight(void) {
    int testRange = 5;
    instrumentConstantsSetCurrentRange(ic, testRange);
    TEST_ASSERT_EQUAL_FLOAT(ic->rangeConstants[testRange - 1].newRateWeight, instrumentConstantsGetWeight(ic));   
}
