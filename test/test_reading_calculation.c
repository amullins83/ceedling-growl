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
#include "type_Instrument.h"

#include <stdlib.h>
#include <stdarg.h>

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


#define addInstrument(a) instrument[numInstruments++] = newSampleLookup(newM9DPconstants(a))

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

    runningAverage = newRunningAverage(0.0, 8, 1.25, 1.25, 4000);
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
}

void tearDown(void)
{
    int i = 0;
    for(; i < numInstruments; i++) 
        deleteSampleLookup(instrument[i]);
    free(instrument);
    free(runningAverage);
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

void test_readingFromVolts_performs_the_expected_conversion(void) {
    sampleLookupAllCorners(instrument, numInstruments, expectedConversion);
}

void checkAverage(SampleLookup **sl, int i, int r, int p) {
    float Rate_uR_hr = 0.0, tau, rate, weight, reading;
    InstrumentConstants *ic_average = sl[i]->instrument;
    instrumentConstantsSetCurrentRange(ic_average, r + 1);
    tau = instrumentConstantsGetMaxTime(ic_average);
    //printf("Tau: %f\n", tau);
    rate = instrumentConstantsGetSampleRate(ic_average);
    //printf("Rate: %f\n", rate);
    weight = 1.0 / tau;
    //printf("Weight: %f\n", weight);
    reading = sl[i]->range[r][p].reading;
    //printf("Reading: %f\n", reading);
    float expected = weight*reading;
    float actual = readingAverage(reading, Rate_uR_hr, ic_average);
    verboseAssertFloatWithin(deltaForRange(r), expected, actual);
}

// void test_readingAverage_is_insane(void) {
//     char *printedAverage = (char *)malloc(32);
//     int rate, maxUR;
//     float minTime, maxTime;
//     InstrumentConstants *ic = instrument[0]->instrument;
//     instrumentConstantsSetCurrentRange(ic, 3);
//     rate = instrumentConstantsGetSampleRate(ic);
//     minTime = instrumentConstantsGetMinTime(ic);
//     maxTime = instrumentConstantsGetMaxTime(ic);
//     maxUR = instrumentConstantsGetMaxUR(ic);
//     //printf("%s: %d, %s: %f, %s: %f, %s: %d\n", "Rate", rate, "Min", minTime, "Max", maxTime, "MaxUR", maxUR);
//     s//printf(printedAverage, "%f", readingAverage(25000.0, 30000.0, ic));
//     TEST_ASSERT_EQUAL_STRING("1.#INF00", printedAverage);
// }


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
    InstrumentConstants *ic_calc = instrument[0]->instrument;
    float over_range_volts = ic_calc->over_range_voltage + 0.1;
    float expected, actual, delta = 26.0;

    instrumentConstantsSetCurrentRange(ic_calc, 1);

    expected = readingFromVolts(over_range_volts, ic_calc);

    setADC(getADCFromReading(over_range_volts));

    Update_Range_Expect();

    ReadingCalculation();
    actual = Rate_uR_hr;
    verboseAssertFloatWithin(delta, expected, actual);

}

void test_readingCalculation_setsFlags_on_over_range(void) {
    InstrumentConstants *ic_calc = instrument[0]->instrument;
    float over_range_volts = ic_calc->over_range_voltage + 0.1;
    float expected;

    instrumentConstantsSetCurrentRange(ic_calc, 1);

    expected = readingFromVolts(over_range_volts, ic_calc);
    

    setADC(getADCFromReading(expected));

    Update_Range_Expect();

    ReadingCalculation();

    TEST_ASSERT(Update_Arc_Flag);
    TEST_ASSERT(Update_Display_Rate_Flag);
}
