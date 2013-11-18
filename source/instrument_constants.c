#include "instrument_constants.h"
#include "models.h"
#include "type_instrument.h"
#include <stdlib.h>

extern TYPE_INSTRUMENT lmi;

void setDefaultRangeConstants(RangeConstants *rc, int correction_factor) {
    if(correction_factor == 1) {
        rc[0] = (RangeConstants){0, 45.47945, 1000.0, (RangeHysteresisTimeConstants){500000.0, 100.0}, (RangeHysteresisTimeConstants){500000.0, 450.0}, 0.1,  0.0, 60.0, 1, 10.0, 480};
        rc[1] = (RangeConstants){0, 4.547945, 1000.0, (RangeHysteresisTimeConstants){500000.0, 1000.0}, (RangeHysteresisTimeConstants){164231.1, 4500.0}, 0.14,  0.0, 60.0, 1, 7.14, 4800};
        rc[2] = (RangeConstants){0, 45.47945, 1000.0, (RangeHysteresisTimeConstants){500000.0, 10000.0}, (RangeHysteresisTimeConstants){54063.7, 45000.0}, 0.14,  0.0, 60.0, 1, 7.14, 48000};
        rc[3] = (RangeConstants){0.164, 4.547945, 1000.0, (RangeHysteresisTimeConstants){500000.0, 100000.0}, (RangeHysteresisTimeConstants){17797.4, 450000.0}, 0.14,  0.0, 60.0, 1, 7.14, 480000};
        rc[4] = (RangeConstants){2.201, 0.4547945, 1000.0, (RangeHysteresisTimeConstants){500000.0, 1000000.0}, (RangeHysteresisTimeConstants){5858.8, 4500000.0}, 0.14,  0.0, 60.0, 1, 7.14, 5000000};
    }
    else {
        rc[0] = (RangeConstants){0, 182.6484, 1000.0, (RangeHysteresisTimeConstants){500000.0, 1000.0}, (RangeHysteresisTimeConstants){500000.0, 4500.0}, 0.1,  0.0, 60.0, 1, 10.0, 480};
        rc[1] = (RangeConstants){0, 18.26484, 1000.0, (RangeHysteresisTimeConstants){500000.0, 10000.0}, (RangeHysteresisTimeConstants){164231.1, 45000.0}, 0.14,  0.0, 60.0, 1, 7.14, 4800};
        rc[2] = (RangeConstants){0, 182.6484, 1000.0, (RangeHysteresisTimeConstants){500000.0, 100000.0}, (RangeHysteresisTimeConstants){54063.7, 450000.0}, 0.14,  0.0, 60.0, 1, 7.14, 48000};
        rc[3] = (RangeConstants){0, 18.26484, 1000.0, (RangeHysteresisTimeConstants){500000.0, 1000000.0}, (RangeHysteresisTimeConstants){17797.4, 4500000.0}, 0.14,  0.0, 60.0, 1, 7.14, 480000};
        rc[4] = (RangeConstants){0.08, 0.910208, 1000.0, (RangeHysteresisTimeConstants){500000.0, 10000000.0}, (RangeHysteresisTimeConstants){5858.8, 45000000.0}, 0.14,  0.0, 60.0, 1, 7.14, 5000000};
    }
}

InstrumentConstants *newConstants(int id) {
    MODEL_NUMBER_TYPES model_type = Check_Model_Number(lmi.settings.product.info.model_number);                     // (v1.01.24)
    float chamber_output_low, chamber_output_high, over_range_voltage;
    int correction_factor;
    switch(model_type)                                                                                              // (v1.01.24)
    {                                                                                                               // (v1.01.24)
        case MODEL_9DP_LOW_PRESSURE:                                                                                // (v1.01.24)
        {                                                                                                           // (v1.01.24)
            chamber_output_low = 0.030;                                                                             // (v1.01.24)
            chamber_output_high = 0.000307;                                                                         // (v1.01.24)
            correction_factor = 1;                                                                                  // (v1.01.24)
            over_range_voltage = 2.0;                                                                               // (v1.01.24)
            break;                                                                                                  // (v1.01.24)
        }                                                                                                           // (v1.01.24)
        case MODEL_9DP_HIGH_PRESSURE:                                                                               // (v1.01.24)
        {                                                                                                           // (v1.01.24)
            chamber_output_low = 0.060;                                                                             // (v1.01.24)
            chamber_output_high = 0.000613;                                                                         // (v1.01.24)
            correction_factor = 1;                                                                                  // (v1.01.24)
            over_range_voltage = 2.0;                                                                               // (v1.01.24)
            break;                                                                                                  // (v1.01.24)
        }                                                                                                           // (v1.01.24)
        case MODEL_9DP_ULTRA_HIGH_PRESSURE:                                                                         // (v1.01.24)
        {                                                                                                           // (v1.01.24)
            chamber_output_low = 0.075;                                                                             // (v1.01.24)
            chamber_output_high = 0.000728;                                                                         // (v1.01.24)
            correction_factor = 1;                                                                                  // (v1.01.24)
            over_range_voltage = 2.2;                                                                               // (v1.01.24)
            break;                                                                                                  // (v1.01.24)
        }                                                                                                           // (v1.01.24)
        case MODEL_9DP_LOW_GAIN_LOW_PRESSURE:                                                                       // (v1.01.24)
        {                                                                                                           // (v1.01.24)
            chamber_output_low = 0.00154;                                                                           // (v1.01.24)
            chamber_output_high = 0.0000166;                                                                        // (v1.01.24)
            correction_factor = 2;                                                                                  // (v1.01.24)
            over_range_voltage = 2.0;                                                                               // (v1.01.24)
            break;                                                                                                  // (v1.01.24)
        }                                                                                                           // (v1.01.24)
        default:                                                                                                    // (v1.01.24)
        {                                                                                                           // (v1.01.24)
            break;                                                                                                  // (v1.01.24)
        }                                                                                                           // (v1.01.24)
    }                                                                                                               // (v1.01.24)

    InstrumentConstants *ic = (InstrumentConstants *)malloc(sizeof(InstrumentConstants));
    ic->correction_factor = correction_factor;
    setDefaultRangeConstants(ic->rangeConstants, correction_factor);
    ic->calConstant[0] = 1000;
    ic->calConstant[1] = 1000;
    ic->calConstant[2] = 1000;
    ic->calConstant[3] = 1000;
    ic->calConstant[4] = 1000;

    ic->chamber_output_low = chamber_output_low;
    ic->chamber_output_high = chamber_output_high;
    
    ic->over_range_voltage = over_range_voltage;
    ic->current_range = 1;
    ic->id = id;
    ic->sampleRate = 8;
    return ic;
}

InstrumentConstants *newM9DPconstants(int id) {
    InstrumentConstants *ic = (InstrumentConstants *)malloc(sizeof(InstrumentConstants));
    setDefaultRangeConstants(ic->rangeConstants, 1);

    ic->calConstant[0] = 1000;
    ic->calConstant[1] = 1000;
    ic->calConstant[2] = 1000;
    ic->calConstant[3] = 1000;
    ic->calConstant[4] = 1000;

    ic->chamber_output_low = 0.060;
    ic->chamber_output_high = 0.000613;
    ic->correction_factor = 1;
    ic->over_range_voltage = 2.0;
    ic->current_range = 5;
    ic->id = id;
    ic->sampleRate = 8;
    return ic;
} 

void deleteInstrumentConstants(InstrumentConstants *ic) {
    free(ic);
}

void instrumentConstantsSetCalConstantForRange(InstrumentConstants *ic, int range, CalConstant c) {
    ic->calConstant[range - 1] = c;
}

int instrumentConstantsGetCurrentRange(InstrumentConstants *ic) {
    return ic->current_range;
}

void instrumentConstantsSetCurrentRange(InstrumentConstants *ic, int range) {
    ic->current_range = range;
}

void instrumentConstantsSetMinTime(InstrumentConstants *ic, int minTime) {
    ic->rangeConstants[ic->current_range - 1].minTime = minTime;
}

void instrumentConstantsSetMaxTime(InstrumentConstants *ic, int maxTime) {
    ic->rangeConstants[ic->current_range - 1].maxTime = maxTime;
}

CalConstant instrumentConstantsGetCurrentCal(InstrumentConstants *ic) {
    return ic->calConstant[ic->current_range - 1];
}

float instrumentConstantsGetRangeFactor(InstrumentConstants *ic) {
    return ic->rangeConstants[ic->current_range - 1].rangeFactor;
}

float instrumentConstantsGetSquareFactor(InstrumentConstants *ic) {
    return ic->rangeConstants[ic->current_range - 1].squareFactor;
}

float instrumentConstantsGetMagnitude(InstrumentConstants *ic) {
    return ic->rangeConstants[ic->current_range - 1].magnitude;   
}

float instrumentConstantsGetCoefficient(InstrumentConstants *ic) {
    if(lmi.calibration.controls.info.fixed_range == 0)
        return ic->rangeConstants[ic->current_range - 1].autoTimeConstants.coefficient;
    else
        return ic->rangeConstants[ic->current_range - 1].fixedTimeConstants.coefficient;
}

float instrumentConstantsGetRate(InstrumentConstants *ic) {
    if(lmi.calibration.controls.info.fixed_range == 0)
        return ic->rangeConstants[ic->current_range - 1].autoTimeConstants.rate;
    else
        return ic->rangeConstants[ic->current_range - 1].fixedTimeConstants.rate;
}

float instrumentConstantsGetWeight(InstrumentConstants *ic) {
    return ic->rangeConstants[ic->current_range - 1].newRateWeight;
}

float instrumentConstantsGetMinTime(InstrumentConstants *ic) {
    if(ic->rangeConstants[ic->current_range - 1].isFixedTime)
        return ic->rangeConstants[ic->current_range - 1].fixedTime;
    else
        return ic->rangeConstants[ic->current_range - 1].minTime;
}

float instrumentConstantsGetMaxTime(InstrumentConstants *ic) {
    if(ic->rangeConstants[ic->current_range - 1].isFixedTime)
        return ic->rangeConstants[ic->current_range - 1].fixedTime;
    else
        return ic->rangeConstants[ic->current_range - 1].maxTime;
}

int instrumentConstantsGetSampleRate(InstrumentConstants *ic) {
    return ic->sampleRate;
}

int instrumentConstantsGetMaxUR(InstrumentConstants *ic) {
    return ic->rangeConstants[ic->current_range - 1].max_uR;
}

float instrumentConstantsGetFixedTime(InstrumentConstants *ic) {
    return ic->rangeConstants[ic->current_range - 1].fixedTime;
}
