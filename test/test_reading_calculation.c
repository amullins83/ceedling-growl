#include "unity.h"
#include "mx_shims.h"
#include "Fake_SFRs.h"
#include "verbose_assert_float.h"
#include "Mocki2cConfig.h"
#include "MockConversions.h"
#include "Mockelectrometer.h"
#include "reading_calculation.h"
#include "RunningAverage.h"
#include "instrument_constants.h"
#include "expected_conversion.h"
#include "sample_lookup.h"
#include "sample_lookup_table.h"
#include "sample_lookup_all_corners.h"
#include "for_each.h"
#include "GenericTypeDefs.h"
#include "models.h"
#include "model_constants.h"
#include "type_Instrument.h"
#include "time_constants.h"

#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

TYPE_INSTRUMENT lmi;
InstrumentConstants *ic;

float Correction_Factor_Value = 1.0;
SampleLookup **instrument;
int numInstruments = 0;
int countedCorners = 0;
int totalCorners = 0;

int forEachCalls = 0;

BOOL   Reset_Level = FALSE;
float  Rate_uR_hr = 0.0;
float  new_rate_uR = 0.0;
UINT32 Period;
BOOL   Update_Audio_Flag = FALSE;
BOOL   Update_Arc_Flag = FALSE;
BOOL   Update_Display_Rate_Flag = FALSE;
UINT32 ADCReadingAverage = 0;
float  uR_filter = 10.0;

BOOL   Audio_Enable;
int    Pulse_Width;
BOOL   Thirty_Second_Startup;
float  Peak_Rate_Reading;
BOOL   Range_Lock;
BOOL   Range_Lock_Valid;
BYTE   Range_Number;

BYTE Current_View = 0;
BYTE Current_Detector = 0;
BYTE Default_Rate_Units = MICRO;
BYTE Electrometer_Switches = 0;

BYTE Range_Number = 1;
UINT16 Full_Scale_Arc_Range[][2] = {
    {
        0,
        0
    },
    {
        0,
        0
    },
    {
        0,
        0
    },
    {
        0,
        0
    },
    {
        0,
        0
    },
    {
        0,
        0
    },
    {
        0,
        0
    }
};

RunningAverage *runningAverage;
RunningAverage *GlobalReadingAverage;

#define addInstrument(a) instrument[numInstruments++] = newSampleLookup(newConstants(a))

void setUp(void)
{
    int i, current_range, current_pair, pairs_in_range;
    ic = newConstants(1001);
    Audio_Enable = FALSE;
    Pulse_Width = 0;
    Thirty_Second_Startup = TRUE;
    Peak_Rate_Reading = 10.0;
    Range_Lock = FALSE;
    Range_Lock_Valid = FALSE;
    TimeConstants *tc = getDefaultTimeConstants();

    runningAverage = newRunningAverage(0.0, tc[0]);

    GlobalReadingAverage = newRunningAverage(0.0, tc[0]);

        for(i = 0; i < 5; i++) {
        lmi.display.detector[0].calibration.info.u_arg[i] = (UINT16)1000;
        lmi.display.detector[0].calibration.info.s_arg[i] = (INT16)0;
    }
    lmi.settings.product.info.model_number = MODEL_9DP_HIGH_PRESSURE;
    lmi.calibration.controls.info.fixed_range = 0;
    instrument = (SampleLookup **)malloc(sizeof(SampleLookup *)*NUM_RANGES);
    addInstrument(3808);
    
    addInstrument(4003);

    addInstrument(6987);

    for(i = 0; i < numInstruments; i++) {
        for(current_range = 0; current_range < NUM_RANGES; current_range++) {
            if(current_range == 0)
                pairs_in_range = 6;
            else
                pairs_in_range = 5;

            for(current_pair = 0; current_pair < pairs_in_range; current_pair++) {
                sampleLookupAddPair(instrument[i], current_range, sampleLookupTable[i][current_range][current_pair].reading, sampleLookupTable[i][current_range][current_pair].volts);
                totalCorners++;
            }
        }
    }

    Update_Audio_Flag = FALSE;
    Update_Arc_Flag = FALSE;
    Update_Display_Rate_Flag = FALSE;
    for(i = 0; i < 5; i++)
        lmi.calibration.constants.info.range[i] = 1000;
}

void tearDown(void)
{
    int i = 0;
    for(; i < numInstruments; i++) 
        deleteSampleLookup(instrument[i]);
    free(instrument);
    free(runningAverage);
    free(GlobalReadingAverage);
    deleteInstrumentConstants(ic);
    numInstruments = 0;
}

void incrementCount(SampleLookup **sl, int i, int r, int p) {
    countedCorners++;
}

void test_sampleLookupAllCorners_looks_at_all_corners(void) {
    sampleLookupAllCorners(instrument, numInstruments, incrementCount);
    TEST_ASSERT_EQUAL(totalCorners, countedCorners);
}

void test_setUp_sets_ids(void) {
    int ids[] = {3808, 4003, 6987}, i;
    for(i = 0; i < numInstruments; i++) {
        TEST_ASSERT_EQUAL(ids[i], instrument[i]->instrument->id);
    }
}

void test_readingFromVolts_performs_the_expected_conversion(void) {
    sampleLookupAllCorners(instrument, numInstruments, expectedConversion);
}

void checkAverage(SampleLookup **sl, int i, int r, int p) {
    float Rate_uR_hr = 0.0, tau, rate, weight, reading, expected, actual, diff;
    InstrumentConstants *ic_average = sl[i]->instrument;
    TimeConstants *tc = getDefaultTimeConstants();

    instrumentConstantsSetCurrentRange(ic_average, r + 1);
    tau = tc[r].maxTime;
    
    rate = tc[r].sampleRate;
    
    weight = 1.0 / tau;
    
    reading = sl[i]->range[r][p].reading;
    
    expected = weight*reading;
    GlobalReadingAverage->average = 10.0;
    actual = readingAverage(reading, tc[r]);
    diff = actual - expected;
    if(fabs(diff) > deltaForRange(r)) {
        printf("Tau: %f\n", tau);
        printf("Rate: %f\n", rate);
        printf("Weight: %f\n", weight);
        printf("Reading: %f\n", reading);
        printf("Failed instrument %d, range %d, and pair %d, delta is %f\n", i, r + 1, p, diff);
    }
    verboseAssertFloatWithin(deltaForRange(r), expected, actual);
}

void test_readingAverage_computes_the_weighted_average(void) {
    sampleLookupAllCorners(instrument, numInstruments, checkAverage);
}

void test_readADC(void) {
    ADCReadingAverage = 600;
    TEST_ASSERT_EQUAL(ADCReadingAverage, readADC());
}

void test_setADC_sets_when_less_than_4096(void) {
    UINT32 testADC = 2048;
    setADC(testADC);
    TEST_ASSERT_EQUAL(testADC, ADCReadingAverage);
}

void test_setADC_fails_when_greater_than_4095(void) {
    UINT32 badADC = 50000, goodADC = 2048;
    ADCReadingAverage = goodADC;
    setADC(badADC);
    TEST_ASSERT_EQUAL(goodADC, ADCReadingAverage);
}

UINT32 getADCFromReading(float reading) {
    return (UINT32)((reading + 0.2)/2.5*4095 + 0.5);
}

void test_getADCFromReading(void) {
    UINT32 testValue = 500;
    TEST_ASSERT_EQUAL(testValue, getADCFromReading(voltsFromADC(testValue)));
}

void test_readingCalculation_resets_on_over_range(void) {
    
    float over_range_volts = ic->over_range_voltage + 0.1;
    float expected, actual, delta = 26.0;

    instrumentConstantsSetCurrentRange(ic, 1);

    expected = readingFromVolts(over_range_volts, ic);

    setADC(getADCFromReading(over_range_volts));

    Update_Range_Expect();

    ReadingCalculation();
    
    actual = Rate_uR_hr;
    
    verboseAssertFloatWithin(delta, expected, actual);
}

void test_readingCalculation_setsFlags_on_over_range(void) {
    
    float over_range_volts = ic->over_range_voltage + 0.1;
    float expected;

    instrumentConstantsSetCurrentRange(ic, 1);
    expected = readingFromVolts(over_range_volts, ic);
    setADC(getADCFromReading(expected));

    Update_Range_Expect();

    ReadingCalculation();

    TEST_ASSERT(Update_Arc_Flag);
    TEST_ASSERT(Update_Display_Rate_Flag);
}

void test_getAverage_gets_global_var(void) {
    float testAverage = 81.2;
    GlobalReadingAverage = runningAverage;
    GlobalReadingAverage->average = testAverage;

    verboseAssertFloatWithin(1.0, testAverage, getReadingAverage());
}

void test_readingAverage_access_global_var(void) {
    float testAverage = 8.0;
    float newReading = 81.2;
    TimeConstants *tc = getDefaultTimeConstants();
    GlobalReadingAverage->average = 0.0;

    readingAverage(newReading, tc[instrument[0]->instrument->current_range]);

    verboseAssertFloatWithin(1.0, testAverage, getReadingAverage());
}
