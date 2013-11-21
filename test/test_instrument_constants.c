#include "unity.h"
#include "mx_shims.h"
#include "models.h"
#include "model_constants.h"
#include "instrument_constants.h"
#include "type_Instrument.h"
#include "GenericTypeDefs.h"
#include <stdlib.h>

InstrumentConstants *ic, *ic2;
#define defaultID 1000
TYPE_INSTRUMENT lmi;
BOOL Update_Audio_Flag = FALSE;
UINT32 Period;
float Rate_uR_hr = 0.0;
TYPE_INSTRUMENT lmi;
ModelConstants constants[];

void setUp(void)
{
    int i;
    lmi.settings.product.info.model_number = MODEL_9DP;
    lmi.calibration.controls.info.fixed_range = 0;
    for(i = 0; i < 5; i++) {
        lmi.display.detector[0].calibration.info.u_arg[i] = (UINT16)1000;
        lmi.display.detector[0].calibration.info.s_arg[i] = (INT16)0;
    }
    ic = newConstants(defaultID + 1);
}

void tearDown(void)
{
    deleteInstrumentConstants(ic);
}

void test_newConstants_returns_pointer(void) {
    TEST_ASSERT_NOT_NULL(ic);
}

void test_newConstants_sets_id(void) {
    TEST_ASSERT_EQUAL(defaultID + 1, ic->id);
}

void test_newConstants_sets_rangeConstants(void) {
    int testRange = 2;
    ic->current_range = testRange;
    RangeConstants testConstants[5];
    setDefaultRangeConstants(testConstants, 1);    

    TEST_ASSERT_EQUAL(testConstants[testRange].magnitude, ic->rangeConstants[testRange].magnitude);
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
