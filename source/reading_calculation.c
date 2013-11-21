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
extern float  new_rate_uR;
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
extern BYTE   Range_Number;
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

void ReadingCorrection(void);

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
void ReadingCalculation(void)
{
    static TimeConstants tc;
    static float Signal_Voltage = 0.0;
    
    static lastRange = 0;
    float signalvolts;
    float chamber_output_low;
    float chamber_output_high;
    float uR_offset;
    float uR_offset_1;
    float uR_offset_2;
    float fixed_range_period;
    float over_range_voltage;                                                                                           // (v1.01.24)
    float temp_rate_uR;                                                                                                 // (v1.02.04)
    float high_percentage;                                                                                              // (v1.02.04)
    float low_percentage;                                                                                               // (v1.02.04)
    UINT32 timerReading;
    UINT8 correction_factor = 0;
    unsigned char model_type;                                                                                           // (v1.01.24)
    BOOL Over_Range = FALSE;

    if(Reset_Level)
    {
        Rate_uR_hr = 0.0;                                                                                               // reset uR reading
        Period = 500000;                                                                                                // reset audio period
        Update_Audio_Flag = TRUE;                                                                                       // update audio
        Reset_Level = FALSE;                                                                                            // clear reset flag
    }
    else
    {
        if(lastRange != Range_Number) {
            lastRange = Range_Number;
            tc = getCurrentTimeConstants();
            runningAverageSetTimeConstants(GlobalReadingAverage, tc);
        }

        // printf("ADCReadingAverage = %d\n", readADC());
        Signal_Voltage = (readADC() / 4095.0) * 2.5;
        signalvolts = Signal_Voltage - 0.200;
        // printf("signalvolts = %f\n", signalvolts);
        model_type = Check_Model_Number(lmi.settings.product.info.model_number);                                        // (v1.01.24)
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
                return;                                                                                                 // (v1.01.24)
            }                                                                                                           // (v1.01.24)
        }                                                                                                               // (v1.01.24)

        if((signalvolts > over_range_voltage) && (lmi.calibration.controls.info.fixed_range == 0))                      // (v1.01.24)
        {
            // printf("I went over range\n");
            Over_Range = TRUE;                                                                                          // new reading is over range
            Update_Arc_Flag = TRUE;                                                                                     // update arc
            Update_Display_Rate_Flag = TRUE;                                                                            // update display rate
        }



        switch(Range_Number)
        {
            case 1:
            {
                if(correction_factor == 1)
                    new_rate_uR = ((signalvolts / 45.47945) / chamber_output_low) * 1000.0;
                else
                    new_rate_uR = ((signalvolts / 182.6484) / chamber_output_low) * 1000.0;

                // printf("new_rate_uR before correction = %f\n", new_rate_uR);
                ReadingCorrection();                                                                                    // (v1.02.02)
                // printf("new_rate_uR after correction = %f\n", new_rate_uR);
                if(Over_Range)                                                                                          // (v1.02.04)
                    Rate_uR_hr = new_rate_uR;                                                                           // (v1.02.04)
                else if(new_rate_uR > (Rate_uR_hr + uR_filter) || Rate_uR_hr > (new_rate_uR + uR_filter)) {             // (v1.02.04)
                    runningAverageSetNewSampleFloat(GlobalReadingAverage, new_rate_uR);
                    Rate_uR_hr = GlobalReadingAverage->average;                                                         // (v1.02.05)
                }
                else if(uR_filter != 0.0)                                                                               // (v1.02.04)
                {                                                                                                       // (v1.02.04)
                    if(new_rate_uR > Rate_uR_hr)                                                                        // (v1.02.04)
                        temp_rate_uR = new_rate_uR - Rate_uR_hr;                                                        // (v1.02.04)
                    else                                                                                                // (v1.02.04)
                        temp_rate_uR = Rate_uR_hr - new_rate_uR;                                                        // (v1.02.04)
                    high_percentage = 0.90 + (0.10 * (1.0 - exp((temp_rate_uR - uR_filter) / (uR_filter * 0.20))));     // (v1.02.05)
                    low_percentage = 1.0 - high_percentage;                                                             // (v1.02.04)
                    Rate_uR_hr = (high_percentage * Rate_uR_hr) + (low_percentage * new_rate_uR);                       // (v1.02.04)
                }                                                                                                       // (v1.02.04)

                if(Update_Audio_Flag)
                {
                    if(lmi.calibration.controls.info.fixed_range == 0)
                    {
                        if(correction_factor == 1)
                            fixed_range_period = 500000.0 * exp(-Rate_uR_hr/450.0);
                        else
                            fixed_range_period = 500000.0 * exp(-Rate_uR_hr/4500.0);
                        Period = (UINT32)fixed_range_period;
                    }
                    else
                    {
                        if(correction_factor == 1)
                            fixed_range_period = 500000.0 * exp(-Rate_uR_hr/100.0);
                        else
                            fixed_range_period = 500000.0 * exp(-Rate_uR_hr/1000.0);
                        Period = (UINT32)fixed_range_period;
                    }
                }
                break;
            }
            case 2:
            {
                if(correction_factor == 1)
                    new_rate_uR = ((signalvolts / 4.547945) / chamber_output_low) * 1000.0;
                else
                    new_rate_uR = ((signalvolts / 18.26484) / chamber_output_low) * 1000.0;

                ReadingCorrection();                                                                                    // (v1.02.02)

                if(Over_Range)
                    Rate_uR_hr = GlobalReadingAverage->average = new_rate_uR;
                else {
                    runningAverageSetNewSampleFloat(GlobalReadingAverage, new_rate_uR);
                    Rate_uR_hr = GlobalReadingAverage->average;
                }

                if(Update_Audio_Flag)
                {
                    if(lmi.calibration.controls.info.fixed_range == 0)
                    {
                        if(correction_factor == 1)
                            fixed_range_period = 164231.1 * exp(-Rate_uR_hr/4500.0);
                        else
                            fixed_range_period = 164231.1 * exp(-Rate_uR_hr/45000.0);
                        Period = (UINT32)fixed_range_period;
                    }
                    else
                    {
                        if(correction_factor == 1)
                            fixed_range_period = 500000.0 * exp(-Rate_uR_hr/1000.0);
                        else
                            fixed_range_period = 500000.0 * exp(-Rate_uR_hr/10000.0);
                        Period = (UINT32)fixed_range_period;
                    }
                }
                break;
            }
            case 3:
            {
                if(correction_factor == 1)
                    new_rate_uR = ((signalvolts / 45.47945) / chamber_output_high) * 1000.0;
                else
                    new_rate_uR = ((signalvolts / 182.6484) / chamber_output_high) * 1000.0;

                ReadingCorrection();                                                                                    // (v1.02.02)
    
                if(Over_Range)
                    Rate_uR_hr = GlobalReadingAverage->average = new_rate_uR;
                else {
                    runningAverageSetNewSampleFloat(GlobalReadingAverage, new_rate_uR);
                    Rate_uR_hr = GlobalReadingAverage->average;
                }

                if(Update_Audio_Flag)
                {
                    if(lmi.calibration.controls.info.fixed_range == 0)
                    {
                        if(correction_factor == 1)
                            fixed_range_period = 54063.7 * exp(-Rate_uR_hr/45000.0);
                        else
                            fixed_range_period = 54063.7 * exp(-Rate_uR_hr/450000.0);
                        Period = (UINT32)fixed_range_period;
                    }
                    else
                    {
                        if(correction_factor == 1)
                            fixed_range_period = 500000.0 * exp(-Rate_uR_hr/10000.0);
                        else
                            fixed_range_period = 500000.0 * exp(-Rate_uR_hr/100000.0);
                        Period = (UINT32)fixed_range_period;
                    }
                }
                break;
            }
            case 4:
            {
                if(correction_factor == 1)
                {
                    signalvolts = signalvolts * (1.0 + (0.164 * signalvolts));
                    new_rate_uR = ((signalvolts / 4.547945) / chamber_output_high) * 1000.0;
                }
                else
                {
                    new_rate_uR = ((signalvolts / 18.26484) / chamber_output_high) * 1000.0;
                }

                ReadingCorrection();                                                                                    // (v1.02.02)
    
                if(Over_Range)
                    Rate_uR_hr = GlobalReadingAverage->average = new_rate_uR;
                else {
                    runningAverageSetNewSampleFloat(GlobalReadingAverage, new_rate_uR);
                    Rate_uR_hr = GlobalReadingAverage->average;
                }

                if(Update_Audio_Flag)
                {
                    if(lmi.calibration.controls.info.fixed_range == 0)
                    {
                        if(correction_factor == 1)
                            fixed_range_period = 17797.4 * exp(-Rate_uR_hr/450000.0);
                        else
                            fixed_range_period = 17797.4 * exp(-Rate_uR_hr/4500000.0);
                        Period = (UINT32)fixed_range_period;
                    }
                    else
                    {
                        if(correction_factor == 1)
                            fixed_range_period = 500000.0 * exp(-Rate_uR_hr/100000.0);
                        else
                            fixed_range_period = 500000.0 * exp(-Rate_uR_hr/1000000.0);
                        Period = (UINT32)fixed_range_period;
                    }
                }
                break;
            }
            case 5:
            {
                if(correction_factor == 1)
                {
                    signalvolts = signalvolts * (1.0 + (2.201 * signalvolts));
                    new_rate_uR = ((signalvolts / 0.4547945) / chamber_output_high) * 1000.0;
                }
                else
                {
                    signalvolts = signalvolts * (1.0 + (0.08 * signalvolts));                                           // (v1.01.07)
                    new_rate_uR = ((signalvolts / 0.910208) / chamber_output_high) * 1000.0;
                }   

                ReadingCorrection();                                                                                    // (v1.02.02)
    
                
                runningAverageSetNewSampleFloat(GlobalReadingAverage, new_rate_uR);
                Rate_uR_hr = GlobalReadingAverage->average;
                

                if(Update_Audio_Flag)
                {
                    if(lmi.calibration.controls.info.fixed_range == 0)
                    {
                        if(correction_factor == 1)
                            fixed_range_period = 5858.8 * exp(-Rate_uR_hr/4500000.0);
                        else
                            fixed_range_period = 5858.8 * exp(-Rate_uR_hr/45000000.0);
                        Period = (UINT32)fixed_range_period;
                    }
                    else
                    {
                        if(correction_factor == 1)
                            fixed_range_period = 500000.0 * exp(-Rate_uR_hr/1000000.0);
                        else
                            fixed_range_period = 500000.0 * exp(-Rate_uR_hr/10000000.0);
                        Period = (UINT32)fixed_range_period;
                    }
                }
                break;
            }
        }

        if(Over_Range) {
            Update_Arc_Flag = TRUE;
            Update_Range();
        }
    }

    // Adjust Audio
    if(Update_Audio_Flag)
    {
        if((Period < 4000) || (Period > 500E3))
            Period = 4000;

        if(Period > 483608)
            Audio_Enable = FALSE;
        else
            Audio_Enable = TRUE;

        WritePeriod23(Period + Pulse_Width);
        SetPulseOC1(Period, (Period + Pulse_Width));
        timerReading = ReadTimer23();
        if(timerReading >= (Period + Pulse_Width))
            WriteTimer23(Period + Pulse_Width - 1000);
        Update_Audio_Flag = FALSE;
    }

    // Adjust Peak Value
    if((Thirty_Second_Startup == TRUE) && (Rate_uR_hr > Peak_Rate_Reading))
    {
        Peak_Rate_Reading = Rate_uR_hr;
        if(Range_Lock == TRUE)
            Range_Lock_Valid = TRUE;
    }

    return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//* Function:
//*     void ReadingCorrection(void)
//*
//* Description:
//*     Adjust the electrometer reading by the calibration constant and the correction factor.
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
//*     Required for 9DP
//*

void ReadingCorrection(void)
{
    // printf("new_rate_uR at beginning of correction = %f\n", new_rate_uR);
    new_rate_uR *= ((float) lmi.calibration.constants.info.range[Range_Number - 1] / 1000.0);       // apply calibration correction to the new reading
    // printf("new_rate_uR adjusted for calibration = %f\n", new_rate_uR);
    new_rate_uR *= Correction_Factor_Value;                                                         // apply correction factor to the new reading
    // printf("new_rate_uR adjusted for correction factor = %f\n", new_rate_uR);

    if(new_rate_uR < 0.0)                                                                           // if the new reading is less than 0...
        new_rate_uR = 0.0;                                                                          // ...fix the new reading to 0

    // printf("new_rate_uR clamped to 0 = %f\n", new_rate_uR);

    return;
}
