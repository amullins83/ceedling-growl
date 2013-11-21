#include "instrument_constants.h"
#include "models.h"
#include "model_constants.h"
#include "type_instrument.h"
#include <stdlib.h>

extern TYPE_INSTRUMENT lmi;

void rangeConstantsGetCalibration(RangeConstants *rc) {
    int i;
    for(i = 0; i < 5; i++)
        rc[i].magnitude = lmi.display.detector[0].calibration.info.u_arg[i] + lmi.display.detector[0].calibration.info.s_arg[i];
}

void setDefaultRangeConstants(RangeConstants *rc, int correction_factor) {
    int i;
    if(correction_factor == 1) {
        rc[0] = (RangeConstants){0, 45.47945, (UINT16)1000, (RangeHysteresisTimeConstants){500000.0, 100.0}, (RangeHysteresisTimeConstants){500000.0, 450.0}};
        rc[1] = (RangeConstants){0, 4.547945, (UINT16)1000, (RangeHysteresisTimeConstants){500000.0, 1000.0}, (RangeHysteresisTimeConstants){164231.1, 4500.0}};
        rc[2] = (RangeConstants){0, 45.47945, (UINT16)1000, (RangeHysteresisTimeConstants){500000.0, 10000.0}, (RangeHysteresisTimeConstants){54063.7, 45000.0}};
        rc[3] = (RangeConstants){0.164, 4.547945, (UINT16)1000, (RangeHysteresisTimeConstants){500000.0, 100000.0}, (RangeHysteresisTimeConstants){17797.4, 450000.0}};
        rc[4] = (RangeConstants){2.201, 0.4547945, (UINT16)1000, (RangeHysteresisTimeConstants){500000.0, 1000000.0}, (RangeHysteresisTimeConstants){5858.8, 4500000.0}};
    }
    else {
        rc[0] = (RangeConstants){0, 182.6484, (UINT16)1000, (RangeHysteresisTimeConstants){500000.0, 1000.0}, (RangeHysteresisTimeConstants){500000.0, 4500.0}};
        rc[1] = (RangeConstants){0, 18.26484, (UINT16)1000, (RangeHysteresisTimeConstants){500000.0, 10000.0}, (RangeHysteresisTimeConstants){164231.1, 45000.0}};
        rc[2] = (RangeConstants){0, 182.6484, (UINT16)1000, (RangeHysteresisTimeConstants){500000.0, 100000.0}, (RangeHysteresisTimeConstants){54063.7, 450000.0}};
        rc[3] = (RangeConstants){0, 18.26484, (UINT16)1000, (RangeHysteresisTimeConstants){500000.0, 1000000.0}, (RangeHysteresisTimeConstants){17797.4, 4500000.0}};
        rc[4] = (RangeConstants){0.08, 0.910208, (UINT16)1000, (RangeHysteresisTimeConstants){500000.0, 10000000.0}, (RangeHysteresisTimeConstants){5858.8, 45000000.0}};
    }

    rangeConstantsGetCalibration(rc);
}

InstrumentConstants *newConstants(int id) {
    InstrumentConstants *ic = (InstrumentConstants *)malloc(sizeof(InstrumentConstants));
    MODEL_NUMBER_TYPES model_type = Check_Model_Number(lmi.settings.product.info.model_number);                     // (v1.01.24)
    ModelConstants *constants = getModelConstants();
    ic->correction_factor = constants[model_type].correction_factor;
    setDefaultRangeConstants(ic->rangeConstants, constants[model_type].correction_factor);
    
    ic->chamber_output_low = constants[model_type].chamber_output_low;
    ic->chamber_output_high = constants[model_type].chamber_output_high;
    
    ic->over_range_voltage = constants[model_type].over_range_voltage;
    ic->current_range = 1;
    ic->id = id;
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

CalConstant instrumentConstantsGetCurrentCal(InstrumentConstants *ic) {
    return ic->calConstant[ic->current_range - 1];
}

float instrumentConstantsGetRangeFactor(InstrumentConstants *ic) {
    return ic->rangeConstants[ic->current_range - 1].rangeFactor;
}

float instrumentConstantsGetSquareFactor(InstrumentConstants *ic) {
    return ic->rangeConstants[ic->current_range - 1].squareFactor;
}

UINT16 instrumentConstantsGetMagnitude(InstrumentConstants *ic) {
    return ic->rangeConstants[ic->current_range - 1].magnitude;   
}

void instrumentConstantsSetCalibration(InstrumentConstants *ic) {
    rangeConstantsGetCalibration(ic->rangeConstants);
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
