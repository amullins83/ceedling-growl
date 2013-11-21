#include "electrometer.h"
#include "GenericTypeDefs.h"
#include "M9DP_Config.h"
#include "type_instrument.h"
#include "i2cConfig.h"
#include "instrument_constants.h"
#include "Conversions.h"

extern BYTE Range_Number;
extern TYPE_INSTRUMENT lmi;
extern InstrumentConstants *ic;
extern BOOL Range_Lock_Valid;
extern UINT16 Full_Scale_Arc_Range[6][2];
extern float Rate_uR_hr;
extern BYTE Electrometer_Switches;
extern BYTE Current_Detector;
extern BYTE Current_View;
extern BYTE Default_Rate_Units;

float Rate_Divide_By = 1.0;


//*
//**************************************************************************************************
//**************************************************************************************************
//* Function:
//*     void Update_Range(void)
//*
//* Description:
//*     Extracted from Update_Arc() to facilitate electrometer range changes outside redraw cycle.
//*     This function looks at the last measurement taken and determines a new range based on that
//*     value. If the device is in a fixed range mode, this function has no effect.
//*
//* Precondition:
//*     Rate_uR_hr  - current exposure rate
//*     Range_Number - current range number
//*
//* Parameters:
//*     None
//*
//* Return Values:
//*     None
//*
//* Remarks:
//*     n1 Created
//*

void Update_Range(void)
{
        BYTE Last_Range = Range_Number;                                                                                                 // n1 Save initial value of Range_Number
        BYTE start_range = 1;                                                                                                           // n1 initial range to check in loop
        BYTE i;
        RelativeAdjustment rels;

        float rate_result;

        if(lmi.calibration.controls.info.fixed_range == 0)  // n1 Skip processing if we're in fixed-range mode.
        {
            if(Range_Lock_Valid == TRUE)      // n1 start from current range if we're in range-lock mode (prevents excessive range jumping near the limits.
                start_range = Last_Range;
            else
                start_range = 1;             // n1 otherwise start from lowest range every time.

            rate_result = GetRateResult();   // n1 Moved rate_result calculation to GetRateResult function
            
            for(i = start_range; i <= NUMBER_OF_RANGES; i++)  // n1 main process loop. Scan ranges to find the best match.
            {
                rels = GetRelativeAdjustment(i);
                if(rate_result < rels.result)   // n1  Might be a match: current rate is within limits of this range.
                {
                    if(rate_result < (rels.result * 0.80))   // n1 make sure we're not too close to the max already
                    {
                        Range_Number = i;                        // n1 set Range
                        Rate_Divide_By = rels.conversion;    // n1 set rate unit conversion factor.
                        break;                    // force early exit from loop. n1 Moved inside if statement because the alternative made no sense.
                    }
                        
                }
            }

            if(i == NUMBER_OF_RANGES + 1)
                Range_Number = NUMBER_OF_RANGES;

            if(Last_Range != Range_Number)
            {
                Change_Range(Range_Number);
            }
        }
        else
        {
            Range_Number = lmi.calibration.controls.info.fixed_range;  // n1 If we're in fixed range, set Range to the user setting.
            rels = GetRelativeAdjustment(Range_Number);
            Rate_Divide_By = rels.conversion;  // n1 set rate unit conversion factor.
            if(Last_Range != Range_Number)  // n1 If the range changed
            {
                Rate_uR_hr = 0.0;                                                                                                           // new fixed range, start fresh
                Change_Range(Range_Number);                                                                                                 // change range
            }
        }
}

RelativeAdjustment GetRelativeAdjustment(BYTE range) {
    RelativeAdjustment ra;
    ra.multiplier = RelativeMultiplier(Full_Scale_Arc_Range[range][1], (short) DEFAULT_RATE_MULTIPLIER);                  // multiplier relative to the default storage value (9DP v1.01.12)
    ra.conversion = MultiplierConversion(ra.multiplier);                                                            // return the engineering notation equivalent
    ra.result = (float)Full_Scale_Arc_Range[range][0] * ra.conversion;                                              // calculate the actual value in uR/hr
    return ra;
}
//*
//**************************************************************************************************
//**************************************************************************************************
//* Function:
//*     void Change_Range(BYTE new_range)
//*
//* Description:
//*     This routine will write the new electrometer range settings.
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

void Change_Range(BYTE new_range)
{
    // switch(new_range)
    // {
    //     case 1:
    //     {
    //         Electrometer_Switches = Range_1;                                                                                // set Range
    //         break;
    //     }
    //     case 2:
    //     {
    //         Electrometer_Switches = Range_10;                                                                               // set Range
    //         break;
    //     }
    //     case 3:
    //     {
    //         Electrometer_Switches = Range_100;                                                                              // set Range
    //         break;
    //     }
    //     case 4:
    //     {
    //         Electrometer_Switches = Range_1K;                                                                               // set Range
    //         break;
    //     }
    //     case 5:
    //     {
    //         Electrometer_Switches = Range_10K;                                                                              // set Range
    //         break;
    //     }
    //     default:
    //     {
    //         return;
    //     }
    // }
    if(new_range < 1 || new_range > 5)
        return;
    else
        Electrometer_Switches = (BYTE[]){Range_1, Range_10, Range_100, Range_1K, Range_10K}[new_range - 1];

    instrumentConstantsSetCurrentRange(ic, new_range);
    i2c2Write(i2cADDR_HV_IO_0x48, 0x0A, 1, Electrometer_Switches, 1);                                                       // write to the OLAT Register
    DelayMs(10);                                                                                                            // allow for some settling time
    HVBUpdateOffset(FALSE);                                                                                                 // update meteroffset
}

//*
//**************************************************************************************************
//**************************************************************************************************
//* Function:
//*     void GetRateResult(void)
//*
//* Description:
//*     Extracted from Update_Arc() to facilitate conversion from default units (nominally uR/hr)
//*     into user-selected units.
//*
//* Precondition:
//*     Rate_uR_hr  - current exposure rate
//*     Basic device data setup.
//*
//* Parameters:
//*     None
//*
//* Return Values:
//*     None
//*
//* Remarks:
//*     n1 Created
//*
float GetRateResult(void)
{
    return UnitConversions(Rate_uR_hr, Default_Rate_Units, lmi.display.detector[Current_Detector].view[Current_View].element.info.arc.control.units);  // n1 Use previously defined UnitConversion function to get Rate in user units.
}
