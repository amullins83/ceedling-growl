#include "reading_calculation.h"
#include "RunningAverage.h"
#include "GenericTypeDefs.h"
#include "models.h"
#include "type_instrument.h"
//#include "proc/p32mx460f512l.h"
#include "peripheral/timer.h"
#include "peripheral/outcompare.h"
#include "electrometer.h"
#include "new.h"
#include "time_constants.h"
#include <math.h>
#include <stdlib.h>

extern float  Correction_Factor_Value; // instantiated in main instrument program and referenced in Functions.c and Commands.c
extern BOOL   Reset_Level;             // instantiated in main instrument program: flag to force average reset
extern float  Rate_uR_hr;              // instantiated in main instrument program: average reading
extern UINT32 Period;                  // instantiated in main instrument program: audio period
extern BOOL   Update_Audio_Flag;       // instantiated in main instrument program: flag to force audio update
extern UINT32 ADCReadingAverage;       // instantiated in main instrument program: raw integer value from ADC
extern float  uR_filter;               // instantiated in main instrument program: minimum difference between current and average readings
extern BOOL   Audio_Enable;
extern int    Pulse_Width;
extern BOOL   Thirty_Second_Startup;
extern float  Peak_Rate_Reading;
extern BOOL   Range_Lock;
extern BOOL   Range_Lock_Valid;

extern TYPE_INSTRUMENT lmi;
extern BOOL Update_Arc_Flag;
extern BOOL Update_Display_Rate_Flag;

extern InstrumentConstants *ic;
extern RunningAverage *GlobalReadingAverage;

// Definitions

float applySquare(float input, InstrumentConstants *instrument) {
    float coefficient = instrumentConstantsGetSquareFactor(instrument);
    return input*(1.0 + coefficient*input);
}

float applyRange(float input, InstrumentConstants *instrument) {
    float coefficient = instrumentConstantsGetRangeFactor(instrument);
    float magnitude = instrumentConstantsGetMagnitude(instrument);
    float chamber_output = instrument->current_range < 3 ? instrument->chamber_output_low : instrument->chamber_output_high;
    return input / coefficient / chamber_output * magnitude;
}

float applyCalibration(float input, InstrumentConstants *instrument) {
    float adjustedInput = input * instrumentConstantsGetCurrentCal(instrument) / 1000.0 * Correction_Factor_Value;
    return adjustedInput < 0 ? 0.0 : adjustedInput;
}

float readingFromVolts(float volts, InstrumentConstants *instrument) {
    float adjustedVolts, rawReading;
    adjustedVolts = instrument->current_range < 4 ? volts : applySquare(volts, instrument);
    rawReading = applyRange(adjustedVolts, instrument);
    return rawReading;
}

void readingReset(void) {
        Rate_uR_hr = 0.0;                                                                                               // reset uR reading
        Period = 500000;                                                                                                // reset audio period
        Update_Audio_Flag = TRUE;                                                                                       // update audio
        Reset_Level = FALSE;                                                                                            // clear reset flag
}


float readingAverage(float newReading, TimeConstants tc) {
    runningAverageSetTimeConstants(GlobalReadingAverage, tc);
    runningAverageSetNewSampleFloat(GlobalReadingAverage, newReading);
    return runningAverageGetAverage(GlobalReadingAverage);
}


float voltsFromADC(UINT32 reading) {
    return (reading / 4095.0) * 2.5 - 0.200;
}

UINT32 readADC(void) {
    return ADCReadingAverage;
}

void setADC(UINT32 newADC) {
    if(newADC < 4096)
        ADCReadingAverage = newADC;
}

float getReadingAverage() {
    return runningAverageGetAverage(GlobalReadingAverage);
}

void doReadingCalculation(InstrumentConstants *instrument) {
    static int lastRange = 0;

    if(lastRange != instrument->current_range) {
        runningAverageSetTimeConstants(GlobalReadingAverage, getCurrentTimeConstants());
        lastRange = instrument->current_range;
    }

    float signalvolts = voltsFromADC(ADCReadingAverage);
    BOOL Over_Range = FALSE;
    float new_rate_uR;
 
    if((signalvolts > instrument->over_range_voltage) && (lmi.calibration.controls.info.fixed_range == 0))                      // (v1.01.24 / v1.04.00 added instrument)
    {
        Over_Range = TRUE;                                                                                          // new reading is over range
        Update_Arc_Flag = TRUE;                                                                                     // update arc
        Update_Display_Rate_Flag = TRUE;                                                                            // update display rate
    }

  
    new_rate_uR = readingFromVolts(signalvolts, instrument);

    if(Over_Range)                                                                                          // (v1.02.04)
        Rate_uR_hr = new_rate_uR;                                                                           // (v1.02.04)
    else
    {
        runningAverageSetNewSampleFloat(GlobalReadingAverage, new_rate_uR);
        Rate_uR_hr = runningAverageGetAverage(GlobalReadingAverage);
    }                                                                                                 // (v1.02.04)

    if(Update_Audio_Flag)
        Period = (UINT32)(instrumentConstantsGetCoefficient(instrument) * exp(-Rate_uR_hr/instrumentConstantsGetRate(instrument)));

    if(Over_Range)
        Update_Range();
}


//**************************************************************************************************
//**************************************************************************************************
//* READING ROUTINES
//**************************************************************************************************
//**************************************************************************************************
//* Function:
//*     void ReadingCalculation(void)
//*
//* Description:
//*     None.
//*
//* Precondition:
//*     None
//*
//* Parameters:
//*     None
//*
//* Return Values:
//*     None
//*
//* Remarks:
//*     Required for all programs
//*

//void AdjustReading(void)
void ReadingCalculation()
{
    if(Reset_Level)
        readingReset();
    else
        doReadingCalculation(ic);
}
