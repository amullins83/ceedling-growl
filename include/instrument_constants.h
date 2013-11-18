#ifndef INSTRUMENT_CONSTANTS_H
#define INSTRUMENT_CONSTANTS_H

typedef struct
{
    float coefficient;
    float rate;
} RangeHysteresisTimeConstants;

typedef struct 
{
    float squareFactor;
    float rangeFactor;
    float magnitude;
    RangeHysteresisTimeConstants fixedTimeConstants;
    RangeHysteresisTimeConstants autoTimeConstants;
    float newRateWeight;
    float minTime;
    float maxTime;
    int isFixedTime;
    float fixedTime;
    int max_uR;
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
    int sampleRate;
} InstrumentConstants;

InstrumentConstants *newConstants(int id);

InstrumentConstants *newM9DPconstants(int id);



void instrumentConstantsSetCalConstantForRange(InstrumentConstants *ic, int range, CalConstant c);
int  instrumentConstantsGetCurrentRange(InstrumentConstants *ic);
void instrumentConstantsSetCurrentRange(InstrumentConstants *ic, int range);
void instrumentConstantsSetMinTime(InstrumentConstants *ic, int minTime);
void instrumentConstantsSetMaxTime(InstrumentConstants *ic, int maxTime);

CalConstant instrumentConstantsGetCurrentCal(InstrumentConstants *ic);

float instrumentConstantsGetRangeFactor(InstrumentConstants *ic);
float instrumentConstantsGetSquareFactor(InstrumentConstants *ic);
float instrumentConstantsGetMagnitude(InstrumentConstants *ic);
float instrumentConstantsGetCoefficient(InstrumentConstants *ic);
float instrumentConstantsGetRate(InstrumentConstants *ic);
float instrumentConstantsGetWeight(InstrumentConstants *ic);
float   instrumentConstantsGetMinTime(InstrumentConstants *ic);
float   instrumentConstantsGetMaxTime(InstrumentConstants *ic);
int   instrumentConstantsGetSampleRate(InstrumentConstants *ic);
int   instrumentConstantsGetMaxUR(InstrumentConstants *ic);
float   instrumentConstantsGetFixedTime(InstrumentConstants *ic);

#endif
