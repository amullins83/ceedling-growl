#ifndef INSTRUMENT_CONSTANTS_H
#define INSTRUMENT_CONSTANTS_H

#include "GenericTypeDefs.h"

typedef struct
{
    float coefficient;
    float rate;
} RangeHysteresisTimeConstants;

typedef struct 
{
    float squareFactor;
    float rangeFactor;
    UINT16 magnitude;
    RangeHysteresisTimeConstants fixedTimeConstants;
    RangeHysteresisTimeConstants autoTimeConstants;
} RangeConstants;

typedef int CalConstant;

typedef struct
{
    RangeConstants rangeConstants[5];
    CalConstant    calConstant[5];
    float chamber_output_low;
    float chamber_output_high;
    int   correction_factor;
    float over_range_voltage;
    int current_range;
    int id;
} InstrumentConstants;

InstrumentConstants *newConstants(int id);



void InstrumentConstantsSetCalibration(InstrumentConstants *ic);
void instrumentConstantsSetCalConstantForRange(InstrumentConstants *ic, int range, CalConstant c);
int  instrumentConstantsGetCurrentRange(InstrumentConstants *ic);
void instrumentConstantsSetCurrentRange(InstrumentConstants *ic, int range);

CalConstant instrumentConstantsGetCurrentCal(InstrumentConstants *ic);

float instrumentConstantsGetRangeFactor(InstrumentConstants *ic);
float instrumentConstantsGetSquareFactor(InstrumentConstants *ic);
UINT16 instrumentConstantsGetMagnitude(InstrumentConstants *ic);
float instrumentConstantsGetCoefficient(InstrumentConstants *ic);
float instrumentConstantsGetRate(InstrumentConstants *ic);

void setDefaultRangeConstants(RangeConstants *rc, int correction_factor);

#endif
