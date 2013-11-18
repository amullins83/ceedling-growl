//**************************************************************************************************
//**************************************************************************************************
//*																		
//*	Filename:		Functions.c
//*	Date:			08/24/10
//*                                                                     
//*	Author:			James Comstock
//*	Company:		Ludlum Measurements                               	
//*	Address:		501 Oak Street										
//*					Sweetwater, TX 79556								
//*                                                                     
//**************************************************************************************************
//**************************************************************************************************
//*                                                                     
//*	Revision Notes:                                                  	
//*
//*	~~~~~~~~	08/24/10	James Comstock
//*	- Original Prototype
//*                                                                     
//**************************************************************************************************
//*                                                                     
//*	Function Screen Routines
//*
//**************************************************************************************************
//*
//*	Disclosure:
//*
//*	Some of the routines contained within this firmware may have been developed by Microchip and are
//*	provided to developers for use in Microchip devices. The Microchip software license agreement is
//*	listed below.
//*
//**************************************************************************************************
//*
//*	Software License Agreement
//*
//* The software supplied herewith by Microchip Technology Incorporated (the "Company") for its
//*	PICmicro® Microcontroller is intended and supplied to you, the Company’s customer, for use
//*	solely and exclusively on Microchip PICmicro Microcontroller products.
//*
//* The software is owned by the Company and/or its supplier, and is protected under applicable
//*	copyright laws. All rights are reserved. Any use in violation of the foregoing restrictions may
//*	subject the user to criminal sanctions under applicable laws, as well as to civil liability for
//*	the breach of the terms and conditions of this license.
//*
//* THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR
//*	STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//*	A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT, IN ANY CIRCUMSTANCES, BE
//*	LIABLE FOR SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
//*

#include "instrument.h"
#include "Functions.h"
#include "Definitions.h"
#include "System.h"
#include "Screens.h"

//**************************************************************************************************
//**************************************************************************************************
//*	DEFINES
//**************************************************************************************************
//**************************************************************************************************

#define X_FUNCTION_NAME								10												// left adjusted
#define X_FUNCTION_UNITS							210												// left adjusted
#define X_FUNCTION_VALUE							200												// right adjusted

#define DEFAULT_ACCUMDOSE_MULTIPLIER				MICRO
#define DEFAULT_PEAKRATE_MULTIPLIER					MICRO

//**************************************************************************************************
//**************************************************************************************************
//*	END OF DEFINES
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//*	EXTERNAL MEMORY DECLARATIONS
//**************************************************************************************************
//**************************************************************************************************
//*	External - Instrument

extern TYPE_INSTRUMENT lmi;

//**************************************************************************************************
//*	External - Fonts

extern const FONT_FLASH Symbol_SansSerif_12;
extern const FONT_FLASH Normal_Arial_Black_44;
extern const FONT_FLASH Secondary_Arial_Black_14;
extern const FONT_FLASH Arial_Rounded_12pt_32_90;
extern const FONT_FLASH Normal_Arial_Black_8;
extern const FONT_FLASH GOLFontDefault;

//**************************************************************************************************
//*	External - Display

extern WORD foreground;
extern WORD background;
extern BYTE Current_Detector;
extern BYTE Current_View;
extern BYTE Current_Function_Index;

//**************************************************************************************************
//*	External - Readings

extern float Rate_uR_hr;
extern float Peak_Rate_Reading;
extern float Rate_Accumulation;
extern float Signal_Voltage;
extern float Peak_Voltage;
extern float Temperature_Reading;
extern float High_Voltage_Reading;
extern float Reading_Average;																		// (9DP v1.03.02)
extern float Temperature_Average;																	// (9DP v1.03.02)

//**************************************************************************************************
//*	External - Alarms

extern float Integrate_Alert_Rate;																	// Rate Alert Level
extern float Integrate_Alarm_Rate;																	// Rate Alert Level
extern unsigned char New_Alarm;

//**************************************************************************************************
//*	External - Units

extern const XCHAR multipliers[16];
extern BYTE Default_Rate_Units;
extern BYTE Default_Dose_Units;

//**************************************************************************************************
//*	External - Variables

extern BYTE Correction_Factor_Mode;																	// (9DP v1.02.02)
extern char Range_Number;																			// current range
extern INT16 ADCReading_Peak;																		// (9DP v1.01.05)
extern DWORD tick;										 											// tick counter (1/2 second)
extern DWORD log_file_write_tick;																	// log file write tick timer (1/2 second)
extern DWORD timed_integrate_tick;
extern float Temperature_Base;
extern float Temperature_Hot;
extern float Temperature_Cold;
extern float Correction_Factor_Value;																// (9DP v1.02.02)
extern float Chamber_Pressure_PSI;																	// (9DP v1.02.03)

extern DWORD debugValue;																			// (9DP v1.01.05)

//**************************************************************************************************
//*	External - Flags

extern BOOL Reset_Level;
extern BOOL Update_Log_File;
extern BOOL Range_Lock;
extern BOOL Range_Lock_Valid;
extern BOOL Data_Logging_Header_Sent;
extern BOOL Integrate_Alert_Flag;
extern BOOL Integrate_Alert_Ack_Flag;
extern BOOL Integrate_Alarm_Flag;
extern BOOL Integrate_Alarm_Ack_Flag;
extern BOOL Start_Timed_Dose;
extern BOOL Stop_Timed_Dose;
extern BOOL Pulsed_Mode_Function;
extern BOOL Set_Temperature_Offset;
extern BOOL Set_High_Temperature_Offset;
extern BOOL Set_High_Temperature_Offset_10;															// (9DP v1.02.07)
extern BOOL Set_High_Temperature_Offset_15;															// (9DP v1.02.07)
extern BOOL Set_High_Temperature_Offset_20;															// (9DP v1.02.07)
extern BOOL Set_Low_Temperature_Offset;
extern BOOL Set_Base_Temperature_Offset;

//**************************************************************************************************
//*	External - Pointers

extern void	*_font;																					// (9DP v1.03.01)

//*
//**************************************************************************************************
//**************************************************************************************************
//*	END OF EXTERNAL MEMORY DECLARATIONS
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//*	LOCAL MEMORY DECLARATIONS
//**************************************************************************************************
//**************************************************************************************************

XCHAR DoseValue[7] 								= {0,0,0,0,0,0,0};
XCHAR DoseUnits[7]								= {0,0,0,0,0,0,0};
XCHAR TimedDoseValue[7]							= {0,0,0,0,0,0,0};
XCHAR TimedDoseUnits[7]							= {0,0,0,0,0,0,0};
XCHAR HighDoseValue[7]							= {0,0,0,0,0,0,0};
XCHAR HighDoseUnits[7]							= {0,0,0,0,0,0,0};
XCHAR PeakrateValue[7] 							= {0,0,0,0,0,0,0};
XCHAR PeakrateUnits[7]							= {0,0,0,0,0,0,0};
XCHAR ExposureValue[7] 							= {0,0,0,0,0,0,0};

const XCHAR OnTxt[]								= {'O','N',0};
const XCHAR OffTxt[]							= {'O','F','F',0};

//**************************************************************************************************
//**************************************************************************************************
//*	END OF LOCAL MEMORY DECLARATIONS
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//*	FUNCTION CONTROLS
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void New_Function_Window(void) 	
//*
//*	Description:
//* 	Clears the current function window and creates a new function window.
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	None
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

void New_Function_Window(void)
{
	Clear_Function_Window();
	Change_Function();
	Update_Function(NEW_WINDOW);
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Clear_Function(void) 	
//*
//*	Description:
//* 	Clears the function window.
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	None
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

void Clear_Function_Window(void)
{
	SetColor(background);
	Bar(0, FUNCTION_Y, 319, (FUNCTION_Y + FUNCTION_HEIGHT));
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Reset_Function(float value) 	
//*
//*	Description:
//* 	Reset the current function as required.
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	None
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

void Reset_Function(float value)
{
	switch(lmi.function[Current_Function_Index].info.header.type)
	{
		case INTEGRATE:
		{
			Rate_Accumulation = value;
			lmi.features.integrate.info.current_reading.value = value;													// (9DP v1.01.02)
			Integrate_Alert_Flag = FALSE;
			Integrate_Alert_Ack_Flag = FALSE;
			Integrate_Alarm_Flag = FALSE;
			Integrate_Alarm_Ack_Flag = FALSE;
			CheckIntegrate();
			if(Start_Timed_Dose == TRUE)
			{
				Start_Timed_Dose = FALSE;
				CheckTimedIntegrate();
			}
			break;
		}
		case PEAK_HOLD:
		{
			Peak_Rate_Reading = value;
			break;
		}
		case PEAK_RATE:
		{
			Peak_Rate_Reading = value;
			Range_Lock_Valid = FALSE;
			break;
		}
		case DATA_LOGGING:
		{
			if(lmi.features.data_logging.info.operating_mode == 0)
			{
				lmi.features.data_logging.info.operating_mode = 1;
			}
			else
			{
				lmi.features.data_logging.info.operating_mode = 0;
				Data_Logging_Header_Sent = FALSE;
			}
			break;
		}
		case TIMED_INTEGRATE:
		{
			timed_integrate_tick = tick + (lmi.function[Current_Function_Index].info.integrate.time * (DWORD) TICKS_PER_SEC);
			Start_Timed_Dose = !Start_Timed_Dose;
			CheckTimedIntegrate();
			break;
		}
		case RANGE_STEP:																								// (9DP v1.01.05)
		case SET_RANGE:
		{
			if(Correction_Factor_Mode == 0)																				// (9DP v1.02.02)
			{																											// (9DP v1.02.02)
				lmi.calibration.controls.info.fixed_range++;
				lmi.calibration.controls.info.fixed_range %= 6;
			}																											// (9DP v1.02.02)
			break;
		}
#ifdef INSTRUMENT_9D																									// moved to include pulsed mode and peak voltage (9DP v1.03.00)
		case PULSED_MODE:
		{
			if(Correction_Factor_Mode == 0)																				// (9DP v1.02.02)
			{																											// (9DP v1.02.02)
				Pulsed_Mode_Function = !Pulsed_Mode_Function;
			}																											// (9DP v1.02.02)
			break;
		}
		case PEAK_VOLTAGE:
		{
			Peak_Voltage = 0.0;
			ADCReading_Peak = 0;																						// (9DP v1.01.05)
			break;
		}
		case TEMPERATURE_OFFSET:
		{
			if(Set_Temperature_Offset == TRUE)																			// if test is currently running...
			{
				Reading_Average = 0.0;																					// ...clear reading average (9DP v1.03.02)
				Temperature_Average = 0.0;																				// ...clear temperature average (9DP v1.03.02)

				Temperature_Base = 0.0;																					// ...clear function base temp
				Temperature_Cold = 0.0;																					// ...clear function low temp
				Temperature_Hot = 0.0;																					// ...clear function high temp

				lmi.display.detector[Current_Detector].calibration.info.f_arg[0] = 0.0;									// ...clear structure base temp (9DP v1.01.03)
				lmi.display.detector[Current_Detector].calibration.info.f_arg[1] = 0.0;									// ...clear structure low offset
				lmi.display.detector[Current_Detector].calibration.info.f_arg[2] = 0.0;									// ...clear structure high offset
				lmi.display.detector[Current_Detector].calibration.info.f_arg[3] = 0.0;									// ...clear structure high offset (9DP v1.02.07)
				lmi.display.detector[Current_Detector].calibration.info.f_arg[4] = 0.0;									// ...clear structure high offset (9DP v1.02.07)
				lmi.display.detector[Current_Detector].calibration.info.f_arg[5] = 0.0;									// ...clear structure high offset (9DP v1.02.07)

				Set_Base_Temperature_Offset = FALSE;																	// ...stop base temp offset
				Set_Low_Temperature_Offset = FALSE;																		// ...stop low temp offset
				Set_High_Temperature_Offset = FALSE;																	// ...stop high temp offset
				Set_High_Temperature_Offset_10 = FALSE;																	// ...stop high temp offset (9DP v1.02.07)
				Set_High_Temperature_Offset_15 = FALSE;																	// ...stop high temp offset (9DP v1.02.07)
				Set_High_Temperature_Offset_20 = FALSE;																	// ...stop high temp offset (9DP v1.02.07)

				CalculateTemperatureOffset(TRUE);																		// ...stops test and performs structure update (9DP v1.01.03)
			}
			else
			{
				Set_Temperature_Offset = TRUE;
				CalculateTemperatureOffset(FALSE);
			}
			break;
		}
		case CORRECTION_FACTOR:																																		// (9DP v1.02.02)
		{																																							// (9DP v1.02.02)
			if(Pulsed_Mode_Function == 0)																															// (9DP v1.02.02)
			{																																						// (9DP v1.02.02)
				do																																					// (9DP v1.02.03)
				{																																					// (9DP v1.02.03)
					Correction_Factor_Mode++;																														// (9DP v1.02.02)
					Correction_Factor_Mode %= 4;																													// (9DP v1.02.02)
				} while(lmi.function[Current_Function_Index].info.correction_factor.factor[Correction_Factor_Mode] == 0.000);										// (9DP v1.02.03)
																																									// (9DP v1.02.03)
				switch(Correction_Factor_Mode)																														// (9DP v1.02.02)
				{																																					// (9DP v1.02.02)
					case 0:																																			// (9DP v1.02.02)
					{																																				// (9DP v1.02.02)
						lmi.calibration.controls.info.fixed_range = 0;																								// (9DP v1.02.02)
						Correction_Factor_Value = 1.000;																											// (9DP v1.02.02)
						break;																																		// (9DP v1.02.02)
					}																																				// (9DP v1.02.02)
					case 1:																																			// (9DP v1.02.02)
					case 2:																																			// (9DP v1.02.02)
					case 3:																																			// (9DP v1.02.02)
					{																																				// (9DP v1.02.02)
						lmi.calibration.controls.info.fixed_range = lmi.function[Current_Function_Index].info.correction_factor.range[Correction_Factor_Mode];		// (9DP v1.02.02)
						Correction_Factor_Value = lmi.function[Current_Function_Index].info.correction_factor.factor[Correction_Factor_Mode];						// (9DP v1.02.02)
						break;																																		// (9DP v1.02.02)
					}																																				// (9DP v1.02.02)
				}																																					// (9DP v1.02.02)
			}																																						// (9DP v1.02.02)
			break;																																					// (9DP v1.02.02)
		}																																							// (9DP v1.02.02)
		case CHAMBER_PRESSURE:																							// (9DP v1.02.03)
		{																												// (9DP v1.02.03)
			Chamber_Pressure_PSI = 0.0;																					// (9DP v1.02.03)
			break;																										// (9DP v1.02.03)
		}																												// (9DP v1.02.03)
#endif
		case DEBUG_FUNCTION:
		{																												// (9DP v1.01.21)
			debugValue = 0;																								// (9DP v1.01.21)
			break;																										// (9DP v1.01.21)
		}																												// (9DP v1.01.21)
		case CLOCK_TIME:
		case CALENDAR_DATE:
		case INPUT_VOLTAGE:
		case CHAMBER_TEMP:
		case INSTRUMENT_TEMP:
		case HIGH_VOLTAGE:
		case NO_FUNCTION:
		default:
		{
			break;
		}
	}
	Update_Function(RESET_WINDOW);
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Update_Function(UINT8 update_type) 	
//*
//*	Description:
//* 	Update the current function as required.
//*
//*	Precondition:
//*		None
//*
//*	Parameters:
//* 	update_type:
//* 		CLEAR_WINDOW
//*			RESET_WINDOW
//*			UPDATE_WINDOW
//*			NEW_WINDOW
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

void Update_Function(UINT8 update_type)
{
	CheckIntegrate();																				// moved before if statement (9DP v1.01.11)
	CheckTimedIntegrate();																			// moved before if statement (9DP v1.01.11)

	if(lmi.display.detector[Current_Detector].view[Current_View].functions.info.function[lmi.display.detector[Current_Detector].view[Current_View].functions.info.current_function].enable != 0)
	{
		switch(lmi.function[Current_Function_Index].info.header.type)
		{
			case INTEGRATE:
			{
				UpdateAccumDose(update_type);
				break;
			}
			case PEAK_RATE:
			{
				UpdatePeakRate(update_type);
				break;
			}
			case DATA_LOGGING:
			{
				UpdateDataLogging(update_type);
				break;
			}
			case TIMED_INTEGRATE:
			{
				UpdateTimedDose(update_type);
				break;
			}
			case CLOCK_TIME:
			{
				UpdateTimeDisplay(update_type);
				break;
			}
			case CALENDAR_DATE:
			{
				UpdateDateDisplay(update_type);
				break;
			}
			case HIGH_VOLTAGE:
			{
				UpdateHighVoltageDisplay(update_type);
				break;
			}
			case RANGE_STEP:																		// (9DP v1.01.05)
			case SET_RANGE:
			{
				UpdateRange(update_type);
				break;
			}
#ifndef INSTRUMENT_9D																				// (9DP v1.03.00)
			case INSTRUMENT_TEMP:
			{
				UpdateInstrumentTemperatureDisplay(update_type);
				break;
			}
#endif
#ifdef INSTRUMENT_9D																				// (9DP v1.03.00)
			case INPUT_VOLTAGE:
			{
				UpdateInputVoltage(update_type);
				break;
			}
			case CHAMBER_TEMP:
			{
				UpdateChamberTemperatureDisplay(update_type);
				break;
			}
			case PULSED_MODE:
			{
				UpdatePulsedMode(update_type);
				break;
			}
			case PEAK_VOLTAGE:
			{
				UpdatePeakVoltage(update_type);
				break;
			}
			case TEMPERATURE_OFFSET:
			{
				UpdateTemperatureOffsetDisplay(update_type);
				break;
			}
			case CORRECTION_FACTOR:																	// (9DP v1.02.02)
			{																						// (9DP v1.02.02)
				UpdateCorrectionFactorMode(update_type);											// (9DP v1.02.02)
				break;																				// (9DP v1.02.02)
			}																						// (9DP v1.02.02)
			case CHAMBER_PRESSURE:																	// (9DP v1.02.03)
			{																						// (9DP v1.02.03)
				UpdateChamberPressureDisplay(update_type);											// (9DP v1.02.03)
				break;																				// (9DP v1.02.03)
			}																						// (9DP v1.02.03)
#endif
			case DEBUG_FUNCTION:
			{
				UpdateDebugDisplay(update_type);
				break;
			}
			case NO_FUNCTION:
			default:
			{
				break;
			}
		}
	}
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Change_Function(void) 	
//*
//*	Description:
//* 	Advances current_function to the next available function.
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	None
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

void Change_Function(void)
{
	unsigned char old_function;

	old_function = lmi.display.detector[Current_Detector].view[Current_View].functions.info.current_function;

	do
	{
		lmi.display.detector[Current_Detector].view[Current_View].functions.info.current_function++;
		lmi.display.detector[Current_Detector].view[Current_View].functions.info.current_function %= DEFAULT_VIEW_NUMBER_OF_FUNCTIONS;
	} while((lmi.display.detector[Current_Detector].view[Current_View].functions.info.function[lmi.display.detector[Current_Detector].view[Current_View].functions.info.current_function].enable == 0) && 
			(lmi.display.detector[Current_Detector].view[Current_View].functions.info.current_function != old_function));

	Current_Function_Index = lmi.display.detector[Current_Detector].view[Current_View].functions.info.function[lmi.display.detector[Current_Detector].view[Current_View].functions.info.current_function].function_index;
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	END FUNCTION CONTROLS
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//*	FUNCTION UPDATES
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void UpdateAccumDose(UINT8 update_type) 	
//*
//*	Description:
//* 	Display accumulated dose value
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	update_type:
//* 		CLEAR_WINDOW
//*			RESET_WINDOW
//*			UPDATE_WINDOW
//*			NEW_WINDOW
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	Function Type: 1
//*

void UpdateAccumDose(UINT8 update_type) 	
{
	static float last_accumdose_value = 0.1;
	static short last_accumdose_multiplier = DEFAULT_DOSE_MULTIPLIER;									// changed value (9DP v1.01.12)
	static BYTE last_view = 0;
	static INT last_offset = 0;

	float relative_conversion;
	float dose_value;
	short dose_multiplier;
	short relative_multiplier;
	INT offset;
	BYTE dose_units;
	char char_buffer[30];
	char multiplier;
	char index;
	char length;
	BOOL conversion_complete;

	if(update_type == CLEAR_WINDOW)
	{
		SetColor(background);
		sprintf(char_buffer,"INTEGRATED DOSE\0");
		DisplayFunctionName(char_buffer);
		DisplayFunctionValue(DoseValue, last_offset);
		DisplayFunctionUnits(DoseUnits, last_offset);
		return;
	}
	else if(update_type == NEW_WINDOW)
	{
		SetColor(foreground);
		sprintf(char_buffer,"INTEGRATED DOSE\0");
		DisplayFunctionName(char_buffer);
	}
	
// Multiplier is now based on Integrate Feature multiplier (9DP v1.01.12)
	if(update_type == RESET_WINDOW)
	{
		dose_multiplier = lmi.features.integrate.info.current_reading.multiplier;
		last_accumdose_multiplier = lmi.features.integrate.info.current_reading.multiplier;
	}
	else
	{
		if(last_accumdose_multiplier != lmi.features.integrate.info.current_reading.multiplier)			// was < (9DP v1.01.13)
		{
			dose_multiplier = lmi.features.integrate.info.current_reading.multiplier;
		}
		else
		{
			if(last_view != Current_View)
			{
				last_view = Current_View;
				dose_multiplier = lmi.features.integrate.info.current_reading.multiplier;
				last_accumdose_multiplier = lmi.features.integrate.info.current_reading.multiplier;
			}
			else
			{
				dose_multiplier = last_accumdose_multiplier;
			}
		}
	}
// End changes (9DP v1.01.12)

	if((lmi.display.detector[Current_Detector].view[Current_View].element.info.arc.control.units >= R_HR) && (lmi.display.detector[Current_Detector].view[Current_View].element.info.arc.control.units <= GY_HR))
	{
		dose_units = RateToDoseConversion(lmi.display.detector[Current_Detector].view[Current_View].element.info.arc.control.units);
	}
	else
	{
		dose_units = lmi.features.integrate.info.current_reading.units;									// changed to use integrate feature units (9DP v1.01.12)
	}
	dose_value = UnitConversions(Rate_Accumulation, Default_Dose_Units, dose_units);

	conversion_complete = FALSE;

	do
	{
		relative_multiplier = RelativeMultiplier((short) DEFAULT_DOSE_MULTIPLIER, dose_multiplier);		// (9DP v1.01.12)
		relative_conversion = MultiplierConversion(relative_multiplier);
		dose_value = dose_value * relative_conversion;
		if(dose_value < 999.0)																			// changed from 1000.0 to 999.0 (9DP v1.01.12)
			conversion_complete = TRUE;
		else
			dose_multiplier++;
	} while(!conversion_complete);

	switch(lmi.function[Current_Function_Index].info.integrate.digits)
	{
		case 3:
		{
			sprintf(char_buffer , "%3d\0", (UINT16) dose_value);
			offset = -10;
			break;
		}
		case 4:
		{
			sprintf(char_buffer , "%4.1f\0", dose_value);
			offset = 0;
			break;
		}
		case 5:
		{
			sprintf(char_buffer , "%5.2f\0", dose_value);
			offset = 10;
			break;
		}
		case 6:																							// (9DP v1.01.21)
		{																								// (9DP v1.01.21)
//			sprintf(char_buffer , "%5.2f\0", dose_value);												// (9DP v1.01.21)
			sprintf(char_buffer , "%6.3f\0", dose_value);												// oops! (9DP v1.02.06)
			offset = 20;																				// (9DP v1.01.21)
			break;																						// (9DP v1.01.21)
		}																								// (9DP v1.01.21)
		default:
		{
			return;
		}
	}

	if((dose_value != last_accumdose_value) || (update_type == NEW_WINDOW) || (update_type == RESET_WINDOW))
	{
		SetColor(background);
		DisplayFunctionValue(DoseValue, last_offset);
		Convert_For_Display(DoseValue, NULL, char_buffer, 6);
		if((Integrate_Alarm_Flag == TRUE) || (Integrate_Alarm_Ack_Flag == TRUE))
		{
			SetColor(ALARM_ARC_COLOR);
			update_type = RESET_WINDOW;
		}
		else if((Integrate_Alert_Flag == TRUE) || (Integrate_Alert_Ack_Flag == TRUE))
		{
			SetColor(ALERT_ARC_COLOR);
			update_type = RESET_WINDOW;
		}
		else
		{
			SetColor(foreground);
		}
		DisplayFunctionValue(DoseValue, offset);
		last_accumdose_value = dose_value;
	}

	if((dose_multiplier != last_accumdose_multiplier) || (update_type == NEW_WINDOW) || (update_type == RESET_WINDOW))
	{
		SetColor(background);
		DisplayFunctionUnits(DoseUnits, last_offset);
		multiplier = multipliers[dose_multiplier];
		switch(dose_units)
		{
			case R:
			{
				if((dose_multiplier == UNITY) || (dose_multiplier == NO_MULTIPLIER))
					sprintf(char_buffer,"R\0");
				else
					sprintf(char_buffer,"%cR\0", multiplier);
				break;
			}
			case SV:
			{
				if((dose_multiplier == UNITY) || (dose_multiplier == NO_MULTIPLIER))
					sprintf(char_buffer,"Sv\0");
				else
					sprintf(char_buffer,"%cSv\0", multiplier);
				break;
			}
			case REM:																					// (9DP v1.01.12)
			{
				if((dose_multiplier == UNITY) || (dose_multiplier == NO_MULTIPLIER))
					sprintf(char_buffer,"REM\0");
				else
					sprintf(char_buffer,"%cREM\0", multiplier);
				break;
			}
			case GY:
			{
				if((dose_multiplier == UNITY) || (dose_multiplier == NO_MULTIPLIER))
					sprintf(char_buffer,"Gy\0");
				else
					sprintf(char_buffer,"%cGy\0", multiplier);
				break;
			}
			default:
			{
				sprintf(char_buffer,"ERROR\0");
				break;
			}
		}
		Convert_For_Display(DoseUnits, NULL, char_buffer, 6);
		if((Integrate_Alarm_Flag == TRUE) || (Integrate_Alarm_Ack_Flag == TRUE))
		{
			SetColor(ALARM_ARC_COLOR);
		}
		else if((Integrate_Alert_Flag == TRUE) || (Integrate_Alert_Ack_Flag == TRUE))
		{
			SetColor(ALERT_ARC_COLOR);
		}
		else
		{
			SetColor(foreground);
		}
		DisplayFunctionUnits(DoseUnits, offset);
		last_accumdose_multiplier = dose_multiplier;
	}

	last_offset = offset;
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void UpdatePeakRate(UINT8 update_type) 	
//*
//*	Description:
//* 	Display accumulated dose value
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	update_type:
//* 		CLEAR_WINDOW
//*			RESET_WINDOW
//*			UPDATE_WINDOW
//*			NEW_WINDOW
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	Function Type: 2
//*

void UpdatePeakRate(UINT8 update_type) 	
{
	static float last_peakrate_value = 0.1;
	static short last_peakrate_multiplier = 0;
	static BYTE last_view = 0;
	static INT last_offset = 0;
	float relative_conversion;
	float peakrate_value;
	short peakrate_multiplier;
	short relative_multiplier;
	INT offset;
	char char_buffer[30];
	char multiplier;
	char index;
	char length;
	BOOL conversion_complete;

	if(last_peakrate_multiplier == 0)
	{
		last_peakrate_multiplier = (short) DEFAULT_RATE_MULTIPLIER;											// renamed (9DP v1.01.12)
	}

	if(update_type == CLEAR_WINDOW)
	{
		sprintf(char_buffer,"PEAK VALUE\0");
		SetColor(background);
		DisplayFunctionName(char_buffer);
		DisplayFunctionValue(PeakrateValue, last_offset);
		DisplayFunctionUnits(PeakrateUnits, last_offset);
		return;
	}
	else if(update_type == NEW_WINDOW)
	{
		sprintf(char_buffer,"PEAK VALUE\0");
		SetColor(foreground);
		DisplayFunctionName(char_buffer);
	}

	if(update_type == RESET_WINDOW)
	{
		peakrate_multiplier = (short) DEFAULT_RATE_MULTIPLIER;												// renamed (9DP v1.01.12)
	}
	else
	{
		if(Current_View == last_view)
		{
			peakrate_multiplier = last_peakrate_multiplier;
		}
		else
		{
			last_view = Current_View;
			peakrate_multiplier = (short) DEFAULT_RATE_MULTIPLIER;											// renamed (9DP v1.01.12)
		}
	}

	peakrate_value = UnitConversions(Peak_Rate_Reading, Default_Rate_Units, lmi.display.detector[Current_Detector].view[Current_View].element.info.arc.control.units);
	conversion_complete = FALSE;

	do
	{
		relative_multiplier = RelativeMultiplier((short) DEFAULT_RATE_MULTIPLIER, peakrate_multiplier);		// (9DP v1.01.12)
		relative_conversion = MultiplierConversion(relative_multiplier);
		peakrate_value = peakrate_value * relative_conversion;
		if(peakrate_value < 999.0)																			// changed from 1000.0 (9DP v1.01.12)
			conversion_complete = TRUE;
		else
			peakrate_multiplier++;
	} while(!conversion_complete);

	switch(lmi.function[Current_Function_Index].info.peak_rate.digits)
	{
		case 3:
		{
			sprintf(char_buffer , "%3d\0", (INT16) peakrate_value);
			offset = -10;
			break;
		}
		case 4:
		{
			sprintf(char_buffer , "%4.1f\0", peakrate_value);
			offset = 0;
			break;
		}
		case 5:
		{
			sprintf(char_buffer , "%5.2f\0", peakrate_value);
			offset = 10;
			break;
		}
		default:
		{
			return;
		}
	}

	if((peakrate_value != last_peakrate_value) || (update_type == NEW_WINDOW) || (update_type == RESET_WINDOW))
	{
		SetColor(background);
		DisplayFunctionValue(PeakrateValue, last_offset);
		Convert_For_Display(PeakrateValue, NULL, char_buffer, 6);
		SetColor(foreground);
		DisplayFunctionValue(PeakrateValue, offset);
		last_peakrate_value = peakrate_value;
	}

	if((peakrate_multiplier != last_peakrate_multiplier) || (update_type == NEW_WINDOW) || (update_type == RESET_WINDOW))
	{
		SetColor(background);
		DisplayFunctionUnits(PeakrateUnits, last_offset);
		multiplier = multipliers[peakrate_multiplier];

		switch(lmi.display.detector[Current_Detector].view[Current_View].element.info.arc.control.units)
		{
			case R_HR:
			{
				if((peakrate_multiplier == UNITY) || (peakrate_multiplier == NO_MULTIPLIER))
					sprintf(char_buffer,"R/hr\0");
				else
					sprintf(char_buffer,"%cR/hr\0", multiplier);
				break;
			}
			case SV_HR:
			{
				if((peakrate_multiplier == UNITY) || (peakrate_multiplier == NO_MULTIPLIER))
					sprintf(char_buffer,"Sv/h\0");															// changed from Sv/hr to Sv/h (9DP v1.01.12)
				else
					sprintf(char_buffer,"%cSv/h\0", multiplier);											// changed from Sv/hr to Sv/h (9DP v1.01.12)
				break;
			}
			case GY_HR:
			{
				if((peakrate_multiplier == UNITY) || (peakrate_multiplier == NO_MULTIPLIER))
					sprintf(char_buffer,"Gy/h\0");															// changed from Gy/hr to Gy/h (9DP v1.01.12)
				else
					sprintf(char_buffer,"%cGy/h\0", multiplier);											// changed from Gy/hr to Gy/h (9DP v1.01.12)
				break;
			}
			case CPM:
			{
				if((peakrate_multiplier == UNITY) || (peakrate_multiplier == NO_MULTIPLIER))
					sprintf(char_buffer,"CPM\0");
				else
					sprintf(char_buffer,"%cCPM\0", multiplier);
				break;
			}
			default:
			{
				sprintf(char_buffer,"ERROR\0");
				break;
			}
		}

		Convert_For_Display(PeakrateUnits, NULL, char_buffer, 6);
		SetColor(foreground);
		DisplayFunctionUnits(PeakrateUnits, offset);
		last_peakrate_multiplier = peakrate_multiplier;
	}

	last_offset = offset;
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void UpdateDataLogging(UINT8 update_type) 	
//*
//*	Description:
//* 	Turns on/off data logging.
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	update_type:
//* 		CLEAR_WINDOW
//*			RESET_WINDOW
//*			UPDATE_WINDOW
//*			NEW_WINDOW
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	Function Type: 15
//*

void UpdateDataLogging(UINT8 update_type)
{
	static UINT8 DataLoggingState = 255;
	char char_buffer[30];

	if(update_type == CLEAR_WINDOW)
	{
		sprintf(char_buffer,"DATA LOG\0");
		SetColor(background);
		DisplayFunctionName(char_buffer);
		DisplayFunctionState(DataLoggingState);
	}
	else if(update_type == NEW_WINDOW)
	{
		sprintf(char_buffer,"DATA LOG\0");
		SetColor(foreground);
		DisplayFunctionName(char_buffer);
		DisplayFunctionState(lmi.features.data_logging.info.operating_mode);
	}
	else if((update_type == UPDATE_WINDOW) || (update_type == RESET_WINDOW))
	{
		if(DataLoggingState != lmi.features.data_logging.info.operating_mode)
		{
			SetColor(background);
			DisplayFunctionState(DataLoggingState);
			SetColor(foreground);
			DisplayFunctionState(lmi.features.data_logging.info.operating_mode);
		}
	}
	DataLoggingState = lmi.features.data_logging.info.operating_mode;
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void UpdateTimedDose(UINT8 update_type) 	
//*
//*	Description:
//* 	Display accumulated dose value
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	update_type:
//* 		CLEAR_WINDOW
//*			RESET_WINDOW
//*			UPDATE_WINDOW
//*			NEW_WINDOW
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	Function Type: 20
//*

void UpdateTimedDose(UINT8 update_type) 	
{
	static float first_timed_dose_value = 0.0;
	static float last_timed_dose_value = 0.0;
	static short last_timed_dose_multiplier = DEFAULT_DOSE_MULTIPLIER;										// changed value (9DP v1.01.12)
	static BYTE last_timed_dose_units = 0;
	static BOOL last_status = FALSE;
	static INT last_offset = 0;

	float timed_dose_value = 0.0;
	float relative_conversion;
	short timed_dose_multiplier;
	short relative_multiplier;
	INT offset;
	char char_buffer[30];
	char multiplier;
	BYTE timed_dose_units;
	BOOL conversion_complete;
	BOOL status_changed = FALSE;

	if(update_type == CLEAR_WINDOW)
	{
		sprintf(char_buffer,"TIMED DOSE\0");
		SetColor(background);
		DisplayFunctionName(char_buffer);
		DisplayFunctionValue(TimedDoseValue, last_offset);
		DisplayFunctionUnits(TimedDoseUnits, last_offset);
		return;
	}
	else if(update_type == NEW_WINDOW)
	{
		sprintf(char_buffer,"TIMED DOSE\0");
		SetColor(foreground);
		DisplayFunctionName(char_buffer);
	}
	else if(update_type == RESET_WINDOW)
	{
		first_timed_dose_value = Rate_Accumulation;
		last_timed_dose_value = 0.0;
		last_timed_dose_multiplier = lmi.features.integrate.info.current_reading.multiplier;				// changed to integrate feature units from function units (9DP v1.01.12)
	}

// check added (9DP v1.01.12)
	if((lmi.display.detector[Current_Detector].view[Current_View].element.info.arc.control.units >= R_HR) && (lmi.display.detector[Current_Detector].view[Current_View].element.info.arc.control.units <= GY_HR))
	{
		timed_dose_units = RateToDoseConversion(lmi.display.detector[Current_Detector].view[Current_View].element.info.arc.control.units);
	}
	else
	{
		timed_dose_units = lmi.features.integrate.info.current_reading.units;
	}
// end check

	if(last_timed_dose_units != timed_dose_units)
	{
		Start_Timed_Dose = FALSE;																			// stop test
		first_timed_dose_value = Rate_Accumulation;															// reinitialize first value (9DP v1.01.12)
		last_timed_dose_value = 0.0;																		// clear last value
		last_timed_dose_multiplier = lmi.features.integrate.info.current_reading.multiplier;				// reinitialize last multiplier (9DP v1.01.12)
	}

	if(Start_Timed_Dose == TRUE)
	{
		if(last_status == FALSE)
		{
			status_changed = TRUE;
			last_status = !last_status;
		}
		timed_dose_value = UnitConversions((Rate_Accumulation - first_timed_dose_value), Default_Dose_Units, timed_dose_units);
	}
	else
	{
		if(last_status == TRUE)
		{
			status_changed = TRUE;
			last_status = !last_status;
		}
		timed_dose_value = last_timed_dose_value;
	}

	timed_dose_multiplier = lmi.features.integrate.info.current_reading.multiplier;							// (9DP v1.01.12)
	conversion_complete = FALSE;

	do
	{
		relative_multiplier = RelativeMultiplier((short) DEFAULT_DOSE_MULTIPLIER, timed_dose_multiplier);	// (9DP v1.01.12)
		relative_conversion = MultiplierConversion(relative_multiplier);
		timed_dose_value = timed_dose_value * relative_conversion;
		if(timed_dose_value < 999.0)																		// changed from 1000.0 to 999.0 (9DP v1.01.12)
			conversion_complete = TRUE;
		else
			timed_dose_multiplier++;
	} while(!conversion_complete);

	switch(lmi.function[Current_Function_Index].info.timed_dose.digits)
	{
		case 3:
		{
			sprintf(char_buffer , "%3d\0", (UINT16) timed_dose_value);
			offset = -10;
			break;
		}
		case 4:
		{
			sprintf(char_buffer , "%4.1f\0", timed_dose_value);
			offset = 0;
			break;
		}
		case 5:
		{
			sprintf(char_buffer , "%5.2f\0", timed_dose_value);
			offset = 10;
			break;
		}
		case 6:																								// (9DP v1.01.21)
		{																									// (9DP v1.01.21)
			sprintf(char_buffer , "%6.3f\0", timed_dose_value);												// (9DP v1.01.21)
			offset = 20;																					// (9DP v1.01.21)
			break;																							// (9DP v1.01.21)
		}																									// (9DP v1.01.21)
		default:
		{
			return;
		}
	}

	if(offset != last_offset)
	{
		status_changed = TRUE;
	}

	if((timed_dose_value != last_timed_dose_value) || (update_type == NEW_WINDOW) || (update_type == RESET_WINDOW) || (status_changed))
	{
		SetColor(background);
		DisplayFunctionValue(TimedDoseValue, last_offset);
		Convert_For_Display(TimedDoseValue, NULL, char_buffer, 6);
		if(last_status)
			SetColor(BRIGHTGREEN);
		else
			SetColor(foreground);
		DisplayFunctionValue(TimedDoseValue, offset);
		last_timed_dose_value = timed_dose_value;
	}

	if((timed_dose_multiplier != last_timed_dose_multiplier) || (update_type == NEW_WINDOW) || (update_type == RESET_WINDOW) || (status_changed))
	{
		SetColor(background);
		DisplayFunctionUnits(TimedDoseUnits, last_offset);
		multiplier = multipliers[timed_dose_multiplier];
		switch(timed_dose_units)
		{
			case R:
			{
				if((timed_dose_multiplier == UNITY) || (timed_dose_multiplier == NO_MULTIPLIER))
					sprintf(char_buffer,"R\0");
				else
					sprintf(char_buffer,"%cR\0", multiplier);
				break;
			}
			case SV:
			{
				if((timed_dose_multiplier == UNITY) || (timed_dose_multiplier == NO_MULTIPLIER))
					sprintf(char_buffer,"Sv\0");
				else
					sprintf(char_buffer,"%cSv\0", multiplier);
				break;
			}
			case REM:																						// (9DP v1.01.12)
			{
				if((timed_dose_multiplier == UNITY) || (timed_dose_multiplier == NO_MULTIPLIER))
					sprintf(char_buffer,"REM\0");
				else
					sprintf(char_buffer,"%cREM\0", multiplier);
				break;
			}
			case GY:
			{
				if((timed_dose_multiplier == UNITY) || (timed_dose_multiplier == NO_MULTIPLIER))
					sprintf(char_buffer,"Gy\0");
				else
					sprintf(char_buffer,"%cGy\0", multiplier);
				break;
			}
			default:
			{
				sprintf(char_buffer,"ERROR\0");
				break;
			}
		}
		Convert_For_Display(TimedDoseUnits, NULL, char_buffer, 6);
		if(last_status == TRUE)
			SetColor(BRIGHTGREEN);
		else
			SetColor(foreground);
		DisplayFunctionUnits(TimedDoseUnits, offset);
		last_timed_dose_multiplier = timed_dose_multiplier;
		last_timed_dose_units = timed_dose_units;
	}

	last_offset = offset;
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void UpdatePulsedMode(UINT8 update_type) 	
//*
//*	Description:
//* 	Turns on/off pulsed mode.
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	update_type:
//* 		CLEAR_WINDOW
//*			RESET_WINDOW
//*			UPDATE_WINDOW
//*			NEW_WINDOW
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	Function Type: 21
//*

#ifdef INSTRUMENT_9D																				// (9DP v1.03.00)

void UpdatePulsedMode(UINT8 update_type)
{
	static UINT8 Temp_Pulsed_Mode_Function = 255;
	char char_buffer[30];

	if(update_type == CLEAR_WINDOW)
	{
		sprintf(char_buffer,"PULSED MODE\0");
		SetColor(background);
		DisplayFunctionName(char_buffer);
		DisplayFunctionState(Temp_Pulsed_Mode_Function);
	}
	else if(update_type == NEW_WINDOW)
	{
		sprintf(char_buffer,"PULSED MODE\0");
		SetColor(foreground);
		DisplayFunctionName(char_buffer);
		DisplayFunctionState((UINT8) Pulsed_Mode_Function);
	}
	else if((update_type == UPDATE_WINDOW) || (update_type == RESET_WINDOW))
	{
		if(Temp_Pulsed_Mode_Function != (UINT8) Pulsed_Mode_Function)
		{
			SetColor(background);
			DisplayFunctionState(Temp_Pulsed_Mode_Function);
			SetColor(foreground);
			DisplayFunctionState((UINT8) Pulsed_Mode_Function);
		}
	}
	Temp_Pulsed_Mode_Function = (UINT8) Pulsed_Mode_Function;
	return;
}

#endif

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void UpdateRange(UINT8 update_type) 	
//*
//*	Description:
//* 	Toggles through available ranges.
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	update_type:
//* 		CLEAR_WINDOW
//*			RESET_WINDOW
//*			UPDATE_WINDOW
//*			NEW_WINDOW
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	Function Type: 22			(9DP v1.01.05)
//* 	Function Type: 60
//*

void UpdateRange(UINT8 update_type)
{
	static UINT8 Temp_Range_Function = 255;
	char char_buffer[30];
	XCHAR tempValue[2];

	if(update_type == CLEAR_WINDOW)
	{
		sprintf(char_buffer,"SET RANGE\0");
		SetColor(background);
		DisplayFunctionName(char_buffer);
		sprintf(char_buffer,"%d\0",lmi.calibration.controls.info.fixed_range);
		Convert_For_Display(tempValue, NULL, char_buffer, 2);
		DisplayFunctionValue(tempValue, 0);
	}
	else if(update_type == NEW_WINDOW)
	{
		sprintf(char_buffer,"SET RANGE\0");
		SetColor(foreground);
		DisplayFunctionName(char_buffer);
		sprintf(char_buffer,"%d\0",lmi.calibration.controls.info.fixed_range);
		Convert_For_Display(tempValue, NULL, char_buffer, 2);
		DisplayFunctionValue(tempValue, 0);
	}
	else if((update_type == UPDATE_WINDOW) || (update_type == RESET_WINDOW))
	{
		if(Temp_Range_Function != lmi.calibration.controls.info.fixed_range)
		{
			SetColor(background);
			sprintf(char_buffer,"%d\0",Temp_Range_Function);
			Convert_For_Display(tempValue, NULL, char_buffer, 2);
			DisplayFunctionValue(tempValue, 0);
			SetColor(foreground);
			sprintf(char_buffer,"%d\0",lmi.calibration.controls.info.fixed_range);
			Convert_For_Display(tempValue, NULL, char_buffer, 2);
			DisplayFunctionValue(tempValue, 0);
		}
	}
	Temp_Range_Function = lmi.calibration.controls.info.fixed_range;
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void UpdateCorrectionFactorMode(UINT8 update_type)
//*
//*	Description:
//* 	Turns on/off correction factor mode.
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	update_type:
//* 		CLEAR_WINDOW
//*			RESET_WINDOW
//*			UPDATE_WINDOW
//*			NEW_WINDOW
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	Function Type: 23
//*
//*		Added (9DP v1.02.02)
//*

#ifdef INSTRUMENT_9D																				// (9DP v1.03.00)

void UpdateCorrectionFactorMode(UINT8 update_type)
{
	static BYTE Temp_Correction_Factor_Mode = 255;
	XCHAR tempValue[10];
	char char_buffer[30];

	if(update_type == CLEAR_WINDOW)
	{
		sprintf(char_buffer,"CORRECTION FACTOR\0");
		SetColor(background);
		DisplayFunctionName(char_buffer);
		sprintf(char_buffer,"%6.3f\0",lmi.function[Current_Function_Index].info.correction_factor.factor[Temp_Correction_Factor_Mode]);
		Convert_For_Display(tempValue, NULL, char_buffer, 8);
		DisplayFunctionUnits(tempValue, 0);
	}
	else if(update_type == NEW_WINDOW)
	{
		sprintf(char_buffer,"CORRECTION FACTOR\0");
		SetColor(foreground);
		DisplayFunctionName(char_buffer);
		sprintf(char_buffer,"%6.3f\0",lmi.function[Current_Function_Index].info.correction_factor.factor[Correction_Factor_Mode]);
		Convert_For_Display(tempValue, NULL, char_buffer, 8);
		DisplayFunctionUnits(tempValue, 0);
	}
	else if((update_type == UPDATE_WINDOW) || (update_type == RESET_WINDOW))
	{
		if(Temp_Correction_Factor_Mode != (UINT8) Correction_Factor_Mode)
		{
			SetColor(background);
			sprintf(char_buffer,"%6.3f\0",lmi.function[Current_Function_Index].info.correction_factor.factor[Temp_Correction_Factor_Mode]);
			Convert_For_Display(tempValue, NULL, char_buffer, 8);
			DisplayFunctionUnits(tempValue, 0);
			SetColor(foreground);
			sprintf(char_buffer,"%6.3f\0",lmi.function[Current_Function_Index].info.correction_factor.factor[Correction_Factor_Mode]);
			Convert_For_Display(tempValue, NULL, char_buffer, 8);
			DisplayFunctionUnits(tempValue, 0);
		}
	}
	Temp_Correction_Factor_Mode = Correction_Factor_Mode;
	return;
}

#endif

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void UpdateChamberPressureDisplay(UINT8 update_type)
//*
//*	Description:
//* 	Turns on/off correction factor mode.
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	update_type:
//* 		CLEAR_WINDOW
//*			RESET_WINDOW
//*			UPDATE_WINDOW
//*			NEW_WINDOW
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	Function Type: 24
//*
//*		Added (9DP v1.02.03)
//*

#ifdef INSTRUMENT_9D																				// (9DP v1.03.00)

void UpdateChamberPressureDisplay(UINT8 update_type)
{
	static float last_Chamber_Pressure_PSI = 0.0;
	XCHAR tempValue[10];
	XCHAR tempUnits[5] = {'P','S','I','G','\0'};
	char char_buffer[30];

	if(update_type == CLEAR_WINDOW)
	{
		sprintf(char_buffer,"CHAMBER PRESSURE\0");
		SetColor(background);
		DisplayFunctionName(char_buffer);
		sprintf(char_buffer,"%6.2f\0",last_Chamber_Pressure_PSI);
		Convert_For_Display(tempValue, NULL, char_buffer, 7);
		DisplayFunctionValue(tempValue, 0);
		DisplayFunctionUnits(tempUnits, 0);
		return;
	}
	else if(update_type == NEW_WINDOW)
	{
		sprintf(char_buffer,"CHAMBER PRESSURE\0");
		SetColor(foreground);
		DisplayFunctionName(char_buffer);
		sprintf(char_buffer,"%6.2f\0",Chamber_Pressure_PSI);
		Convert_For_Display(tempValue, NULL, char_buffer, 7);
		DisplayFunctionValue(tempValue, 0);
		DisplayFunctionUnits(tempUnits, 0);
	}
	else if((update_type == UPDATE_WINDOW) || (update_type == RESET_WINDOW))
	{
		if(Chamber_Pressure_PSI != last_Chamber_Pressure_PSI)
		{
			SetColor(background);
			sprintf(char_buffer,"%6.2f\0",last_Chamber_Pressure_PSI);
			Convert_For_Display(tempValue, NULL, char_buffer, 7);
			DisplayFunctionValue(tempValue, 0);
			DisplayFunctionUnits(tempUnits, 0);
			SetColor(foreground);
			sprintf(char_buffer,"%6.2f\0",Chamber_Pressure_PSI);
			Convert_For_Display(tempValue, NULL, char_buffer, 7);
			DisplayFunctionValue(tempValue, 0);
			DisplayFunctionUnits(tempUnits, 0);
		}
	}
	last_Chamber_Pressure_PSI = Chamber_Pressure_PSI;
	return;
}

#endif

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void UpdateInputVoltage(UINT8 update_type)
//*
//*	Description:
//* 	
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	update_type:
//* 		CLEAR_WINDOW
//*			RESET_WINDOW
//*			UPDATE_WINDOW
//*			NEW_WINDOW
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	Function Type: 51
//*

#ifdef INSTRUMENT_9D																				// (9DP v1.03.00)

void UpdateInputVoltage(UINT8 update_type)
{
	static float signal_voltage_average = 0.0;
	static BYTE oldDisplay[6] = {0};
	static char oldRangeNumber = 0;
	static BYTE updatecounter = 0;
	BYTE newDisplay[6] = {0};
	XCHAR tempValue[6];
	XCHAR tempUnits[2] = {'V','\0'};
	char char_buffer[30];

	if(Range_Number != oldRangeNumber)
	{
		oldRangeNumber = Range_Number;
		signal_voltage_average = 0.0;
		updatecounter = 0;
	}

	if(updatecounter == 0)
	{
		signal_voltage_average = (signal_voltage_average * 0.99) + (Signal_Voltage * 0.01);
		sprintf(newDisplay,"%05.3f\0",signal_voltage_average);
	}
	else
	{
		strncpy(newDisplay,oldDisplay,6);
	}

	if(update_type == CLEAR_WINDOW)
	{
		sprintf(char_buffer,"INPUT VOLTAGE\0");
		SetColor(background);
		DisplayFunctionName(char_buffer);
		Convert_For_Display(tempValue, NULL, oldDisplay, 6);
		DisplayFunctionValue(tempValue, 0);
		DisplayFunctionUnits(tempUnits, 0);
	}
	else if(update_type == NEW_WINDOW)
	{
		sprintf(char_buffer,"INPUT VOLTAGE\0");
		SetColor(foreground);
		DisplayFunctionName(char_buffer);
		Convert_For_Display(tempValue, NULL, newDisplay, 6);
		DisplayFunctionValue(tempValue, 0);
		DisplayFunctionUnits(tempUnits, 0);
	}
	else if((update_type == UPDATE_WINDOW) || (update_type == RESET_WINDOW))
	{
		if(strncmp((char *)oldDisplay, (char *)newDisplay, 6))
		{
			SetColor(background);
			Convert_For_Display(tempValue, NULL, oldDisplay, 6);
			DisplayFunctionValue(tempValue, 0);
			SetColor(foreground);
			Convert_For_Display(tempValue, NULL, newDisplay, 6);
			DisplayFunctionValue(tempValue, 0);
		}
	}
	strncpy(oldDisplay,newDisplay,6);
	updatecounter = (updatecounter + 1) % 8;
	return;
}

#endif

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void UpdatePeakVoltage(UINT8 update_type)
//*
//*	Description:
//* 	
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	update_type:
//* 		CLEAR_WINDOW
//*			RESET_WINDOW
//*			UPDATE_WINDOW
//*			NEW_WINDOW
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	Function Type: 52
//*

#ifdef INSTRUMENT_9D																				// (9DP v1.03.00)

void UpdatePeakVoltage(UINT8 update_type)
{
	static BYTE oldDisplay[8] = {0};
	BYTE newDisplay[8] = {0};
	char char_buffer[30];

	sprintf(newDisplay,"%05.3f\0",Peak_Voltage);

	if(update_type == CLEAR_WINDOW)
	{
		sprintf(char_buffer,"PEAK VOLTAGE\0");
		SetColor(background);
		DisplayFunctionName(char_buffer);
		SetFont((void*)&Secondary_Arial_Black_14);
		DisplayFunctionEvent(oldDisplay, 120);
	}
	else if(update_type == NEW_WINDOW)
	{
		sprintf(char_buffer,"PEAK VOLTAGE\0");
		SetColor(foreground);
		DisplayFunctionName(char_buffer);
		SetFont((void*)&Secondary_Arial_Black_14);
		DisplayFunctionEvent(newDisplay, 120);
	}
	else if((update_type == UPDATE_WINDOW) || (update_type == RESET_WINDOW))
	{
		if(strncmp((char *)oldDisplay, (char *)newDisplay, 8))
		{
			SetFont((void*)&Secondary_Arial_Black_14);
			SetColor(background);
			DisplayFunctionEvent(oldDisplay, 120);
			SetColor(foreground);
			DisplayFunctionEvent(newDisplay, 120);
		}
	}
	strncpy(oldDisplay,newDisplay,8);
	return;
}

#endif

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void UpdateTimeDisplay(UINT8 update_type)
//*
//*	Description:
//* 	
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	update_type:
//* 		CLEAR_WINDOW
//*			RESET_WINDOW
//*			UPDATE_WINDOW
//*			NEW_WINDOW
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	Function Type: 53
//*

void UpdateTimeDisplay(UINT8 update_type)
{
//	static BYTE oldTime[9] = {0};
	static BYTE oldTime[12] = {0};																						// (9DP v1.03.01)
//	BYTE newTime[9] = {0};																			
	BYTE newTime[12] = {0};																								// (9DP v1.03.01)
	char char_buffer[30];
//	char temp_hours;
	XCHAR tempValue[12];																								// (9DP v1.03.00)

	TimeStringConversion(&lmi.settings.clock.info.time, (BYTE *)&newTime);												// (9DP v1.03.01)
//	if(lmi.settings.clock.info.time.format == 12)
//	{
//		temp_hours = lmi.settings.clock.info.time.hours % 12;
//		if(temp_hours == 0) temp_hours = 12;
//		if(lmi.settings.clock.info.time.hours < 12)
//		{
//			sprintf(newTime,"%02d:%02d:%02d\0",temp_hours,lmi.settings.clock.info.time.minutes,lmi.settings.clock.info.time.seconds);
//		}
//		else
//		{
//			sprintf(newTime,"%02d:%02d:%02d\0",temp_hours,lmi.settings.clock.info.time.minutes,lmi.settings.clock.info.time.seconds);
//		}
//	}
//	else
//	{
//		sprintf(newTime,"%02d:%02d:%02d\0",lmi.settings.clock.info.time.hours,lmi.settings.clock.info.time.minutes,lmi.settings.clock.info.time.seconds);
//	}

	if(update_type == CLEAR_WINDOW)
	{
		SetColor(background);
		sprintf(char_buffer,"TIME\0");
		DisplayFunctionName(char_buffer);
		SetFont((void*)&Secondary_Arial_Black_14);
		Convert_For_Display(tempValue, NULL, oldTime, 12);																// (9DP v1.03.01)
		DisplayFunctionCenter(tempValue, 0);																			// (9DP v1.03.01)
//		DisplayFunctionEvent(oldTime, 120);																				// (hidden 9DP v1.03.01)
	}
	else if(update_type == NEW_WINDOW)
	{
		SetColor(foreground);
		sprintf(char_buffer,"TIME\0");
		DisplayFunctionName(char_buffer);
		SetFont((void*)&Secondary_Arial_Black_14);
		Convert_For_Display(tempValue, NULL, newTime, 12);																// (9DP v1.03.01)
		DisplayFunctionCenter(tempValue, 0);																			// (9DP v1.03.01)
//		DisplayFunctionEvent(newTime, 120);																				// (hidden 9DP v1.03.01)
	}
	else if((update_type == UPDATE_WINDOW) || (update_type == RESET_WINDOW))
	{
//		if(strncmp((char *)oldTime, (char *)newTime, 9))
		if(strncmp((char *)oldTime, (char *)newTime, 12))																// (9DP v1.03.01)
		{
			SetColor(background);
			SetFont((void*)&Secondary_Arial_Black_14);
			Convert_For_Display(tempValue, NULL, oldTime, 12);															// (9DP v1.03.01)
			DisplayFunctionCenter(tempValue, 0);																		// (9DP v1.03.01)
//			DisplayFunctionEvent(oldTime, 120);																			// (hidden 9DP v1.03.00)
			SetColor(foreground);
			Convert_For_Display(tempValue, NULL, newTime, 12);															// (9DP v1.03.01)
			DisplayFunctionCenter(tempValue, 0);																		// (9DP v1.03.01)
//			DisplayFunctionEvent(newTime, 120);																			// (9DP v1.03.01)
		}
	}
//	strncpy(oldTime,newTime,9);
	strncpy(oldTime,newTime,12);																						// (9DP v1.03.01)
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void UpdateDateDisplay(UINT8 update_type)
//*
//*	Description:
//* 	
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	update_type:
//* 		CLEAR_WINDOW
//*			RESET_WINDOW
//*			UPDATE_WINDOW
//*			NEW_WINDOW
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	Function Type: 54
//*

void UpdateDateDisplay(UINT8 update_type)
{
	static BYTE oldDate[11] = {0};
	BYTE newDate[11] = {0};
	char char_buffer[30];
	XCHAR tempValue[11];																			// (9DP v1.03.00)

//	sprintf(newDate,"%02d/%02d/20%02d\0",lmi.settings.calendar.info.date.month,lmi.settings.calendar.info.date.day,lmi.settings.calendar.info.date.year);
	DateStringConversion(&lmi.settings.calendar.info.date, (BYTE *)&newDate);						// (9DP v1.03.00)

	if(update_type == CLEAR_WINDOW)
	{
		SetColor(background);
		sprintf(char_buffer,"DATE\0");
		DisplayFunctionName(char_buffer);
		SetFont((void*)&Secondary_Arial_Black_14);
		Convert_For_Display(tempValue, NULL, oldDate, 11);											// (9DP v1.03.00)
		DisplayFunctionCenter(tempValue, 0);														// (changed from value to center 9DP v1.03.01)
//		DisplayFunctionEvent(oldDate, 120);															// (hidden 9DP v1.03.00)
	}
	else if(update_type == NEW_WINDOW)
	{
		SetColor(foreground);
		sprintf(char_buffer,"DATE\0");
		DisplayFunctionName(char_buffer);
		SetFont((void*)&Secondary_Arial_Black_14);
		Convert_For_Display(tempValue, NULL, newDate, 11);											// (9DP v1.03.00)
		DisplayFunctionCenter(tempValue, 0);														// (changed from value to center 9DP v1.03.01)
//		DisplayFunctionEvent(newDate, 120);															// (hidden 9DP v1.03.00)
	}
	else if((update_type == UPDATE_WINDOW) || (update_type == RESET_WINDOW))
	{
		if(strncmp((char *)oldDate, (char *)newDate, 11))
		{
			SetColor(background);
			SetFont((void*)&Secondary_Arial_Black_14);
			Convert_For_Display(tempValue, NULL, oldDate, 11);										// (9DP v1.03.00)
			DisplayFunctionCenter(tempValue, 0);													// (changed from value to center 9DP v1.03.01)
//			DisplayFunctionEvent(oldDate, 120);														// (hidden 9DP v1.03.00)
			SetColor(foreground);
			Convert_For_Display(tempValue, NULL, newDate, 11);										// (9DP v1.03.00)
			DisplayFunctionCenter(tempValue, 0);													// (changed from value to center 9DP v1.03.01)
//			DisplayFunctionEvent(newDate, 120);														// (hidden 9DP v1.03.00)
		}
	}
	strncpy(oldDate,newDate,11);
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void UpdateChamberTemperatureDisplay(UINT8 update_type)
//*
//*	Description:
//* 	
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	update_type:
//* 		CLEAR_WINDOW
//*			RESET_WINDOW
//*			UPDATE_WINDOW
//*			NEW_WINDOW
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	Function Type: 55
//*

#ifdef INSTRUMENT_9D																				// (9DP v1.03.00)

void UpdateChamberTemperatureDisplay(UINT8 update_type)
{
	static BYTE oldTemperature[6] = {0};
	BYTE newTemperature[6] = {0};
	XCHAR tempValue[6];
	XCHAR tempUnits[2] = {'C','\0'};
	char char_buffer[30];

	sprintf(newTemperature,"%5.1f\0",Temperature_Reading);

	if(update_type == CLEAR_WINDOW)
	{
		sprintf(char_buffer,"CHAMBER TEMP\0");
		SetColor(background);
		DisplayFunctionName(char_buffer);
		Convert_For_Display(tempValue, NULL, oldTemperature, 6);
		DisplayFunctionValue(tempValue, 0);
		DisplayFunctionUnits(tempUnits, 0);
	}
	else if(update_type == NEW_WINDOW)
	{
		sprintf(char_buffer,"CHAMBER TEMP\0");
		SetColor(foreground);
		DisplayFunctionName(char_buffer);
		Convert_For_Display(tempValue, NULL, newTemperature, 6);
		DisplayFunctionValue(tempValue, 0);
		DisplayFunctionUnits(tempUnits, 0);
	}
	else if((update_type == UPDATE_WINDOW) || (update_type == RESET_WINDOW))
	{
		if(strncmp((char *)oldTemperature, (char *)newTemperature, 6))
		{
			SetColor(background);
			Convert_For_Display(tempValue, NULL, oldTemperature, 6);
			DisplayFunctionValue(tempValue, 0);
			SetColor(foreground);
			Convert_For_Display(tempValue, NULL, newTemperature, 6);
			DisplayFunctionValue(tempValue, 0);
		}
	}
	strncpy(oldTemperature,newTemperature,6);
	return;
}

#endif

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void UpdateInstrumentTemperatureDisplay(UINT8 update_type)
//*
//*	Description:
//* 	
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	update_type:
//* 		CLEAR_WINDOW
//*			RESET_WINDOW
//*			UPDATE_WINDOW
//*			NEW_WINDOW
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	Function Type: 56
//*

#ifndef INSTRUMENT_9D																				// (9DP v1.03.00)

void UpdateInstrumentTemperatureDisplay(UINT8 update_type)
{
	static BYTE oldTemperature[6] = {0};
	BYTE newTemperature[6] = {0};
	XCHAR tempValue[6];
	XCHAR tempUnits[2] = {'C','\0'};
	char char_buffer[30];

	sprintf(newTemperature,"%5.1f\0",Temperature_Reading);

	if(update_type == CLEAR_WINDOW)
	{
		sprintf(char_buffer,"INSTRUMENT TEMP\0");
		SetColor(background);
		DisplayFunctionName(char_buffer);
		Convert_For_Display(tempValue, NULL, oldTemperature, 6);
		DisplayFunctionValue(tempValue, 0);
		DisplayFunctionUnits(tempUnits, 0);
	}
	else if(update_type == NEW_WINDOW)
	{
		sprintf(char_buffer,"INSTRUMENT TEMP\0");
		SetColor(foreground);
		DisplayFunctionName(char_buffer);
		Convert_For_Display(tempValue, NULL, newTemperature, 6);
		DisplayFunctionValue(tempValue, 0);
		DisplayFunctionUnits(tempUnits, 0);
	}
	else if((update_type == UPDATE_WINDOW) || (update_type == RESET_WINDOW))
	{
		if(strncmp((char *)oldTemperature, (char *)newTemperature, 6))
		{
			SetColor(background);
			Convert_For_Display(tempValue, NULL, oldTemperature, 6);
			DisplayFunctionValue(tempValue, 0);
			SetColor(foreground);
			Convert_For_Display(tempValue, NULL, newTemperature, 6);
			DisplayFunctionValue(tempValue, 0);
		}
	}
	strncpy(oldTemperature,newTemperature,6);
	return;
}

#endif

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void UpdateHighVoltageDisplay(UINT8 update_type)
//*
//*	Description:
//* 	
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	update_type:
//* 		CLEAR_WINDOW
//*			RESET_WINDOW
//*			UPDATE_WINDOW
//*			NEW_WINDOW
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	Function Type: 57
//*

void UpdateHighVoltageDisplay(UINT8 update_type)
{
	static BYTE oldVoltage[6] = {0};
	BYTE newVoltage[6] = {0};
	XCHAR tempValue[6];
	XCHAR tempUnits[2] = {'V','\0'};
	char char_buffer[30];

	sprintf(newVoltage,"%5.1f\0",High_Voltage_Reading);

	if(update_type == CLEAR_WINDOW)
	{
		sprintf(char_buffer,"HIGH VOLTAGE\0");
		SetColor(background);
		DisplayFunctionName(char_buffer);
		Convert_For_Display(tempValue, NULL, oldVoltage, 6);
		DisplayFunctionValue(tempValue, 0);
		DisplayFunctionUnits(tempUnits, 0);
	}
	else if(update_type == NEW_WINDOW)
	{
		sprintf(char_buffer,"HIGH VOLTAGE\0");
		SetColor(foreground);
		DisplayFunctionName(char_buffer);
		Convert_For_Display(tempValue, NULL, newVoltage, 6);
		DisplayFunctionValue(tempValue, 0);
		DisplayFunctionUnits(tempUnits, 0);
	}
	else if((update_type == UPDATE_WINDOW) || (update_type == RESET_WINDOW))
	{
		if(strncmp((char *)oldVoltage, (char *)newVoltage, 6))
		{
			SetColor(background);
			Convert_For_Display(tempValue, NULL, oldVoltage, 6);
			DisplayFunctionValue(tempValue, 0);
			SetColor(foreground);
			Convert_For_Display(tempValue, NULL, newVoltage, 6);
			DisplayFunctionValue(tempValue, 0);
		}
	}
	strncpy(oldVoltage,newVoltage,6);
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void UpdateTemperatureOffsetDisplay(UINT8 update_type)
//*
//*	Description:
//* 	
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	update_type:
//* 		CLEAR_WINDOW
//*			RESET_WINDOW
//*			UPDATE_WINDOW
//*			NEW_WINDOW
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	Function Type: 58
//*

#ifdef INSTRUMENT_9D																				// (9DP v1.03.00)

void UpdateTemperatureOffsetDisplay(UINT8 update_type)
{
	static BYTE oldOffset[30] = {0};
	static BOOL oldHigh = FALSE;																														// (9DP v1.01.04)
	static BOOL oldLow = FALSE;																															// (9DP v1.01.04)
	BYTE newOffset[30] = {0};
	char char_buffer[30];

	if(Set_Base_Temperature_Offset == TRUE)
	{
		sprintf(newOffset,"B:  -  C C:  -  C H:  -  C\0");
//		oldHigh = Set_High_Temperature_Offset;																											// (9DP v1.01.04)
		oldHigh = Set_High_Temperature_Offset_20;																										// (9DP v1.02.07)
		oldLow = Set_Low_Temperature_Offset;																											// (9DP v1.01.04)
	}
	else
	{
		sprintf(newOffset,"B:%5.1fC C:%5.1fC H:%5.1fC\0", Temperature_Base, Temperature_Cold, Temperature_Hot);
	}

	if(update_type == CLEAR_WINDOW)
	{
		sprintf(char_buffer,"TEMP OFFSET\0");
		SetColor(background);
		DisplayFunctionName(char_buffer);
		SetFont((void*)&Normal_Arial_Black_8);
		DisplayFunctionEvent(oldOffset, 100);
	}
	else if(update_type == NEW_WINDOW)
	{
		sprintf(char_buffer,"TEMP OFFSET\0");
		SetColor(foreground);
		DisplayFunctionName(char_buffer);
		if(Set_Temperature_Offset == TRUE)
			SetColor(BRIGHTGREEN);
		else
			SetColor(foreground);
		SetFont((void*)&Normal_Arial_Black_8);
		DisplayFunctionEvent(newOffset, 100);
	}
	else if((update_type == UPDATE_WINDOW) || (update_type == RESET_WINDOW))
	{
//		if((strncmp((char *)oldOffset, (char *)newOffset, 29)) || (oldHigh != Set_High_Temperature_Offset) || (oldLow != Set_Low_Temperature_Offset)) 	// (9DP v1.01.04)
		if((strncmp((char *)oldOffset, (char *)newOffset, 29)) || (oldHigh != Set_High_Temperature_Offset_20) || (oldLow != Set_Low_Temperature_Offset))// (9DP v1.02.07)
		{
			SetFont((void*)&Normal_Arial_Black_8);
			SetColor(background);
			DisplayFunctionEvent(oldOffset, 100);
			if(Set_Temperature_Offset == TRUE)
				SetColor(BRIGHTGREEN);
			else
				SetColor(foreground);
			DisplayFunctionEvent(newOffset, 100);
		}
	}
	strncpy(oldOffset,newOffset,29);
//	oldHigh = Set_High_Temperature_Offset;																												// (9DP v1.01.04)
	oldHigh = Set_High_Temperature_Offset_20;																											// (9DP v1.02.07)
	oldLow = Set_Low_Temperature_Offset;																												// (9DP v1.01.04)

	return;
}

#endif

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void UpdateDebugDisplay(UINT8 update_type)
//*
//*	Description:
//* 	
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	update_type:
//* 		CLEAR_WINDOW
//*			RESET_WINDOW
//*			UPDATE_WINDOW
//*			NEW_WINDOW
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	Function Type: 99
//*

#define DEBUG_DISPLAY_SIZE						11

void UpdateDebugDisplay(UINT8 update_type)
{
	static BYTE oldDebug[DEBUG_DISPLAY_SIZE] = {0};
	BYTE newDebug[DEBUG_DISPLAY_SIZE] = {0};
	XCHAR tempValue[DEBUG_DISPLAY_SIZE] = {0};;
	char char_buffer[30];

	sprintf(newDebug,"%08X\0",debugValue);

	if(update_type == CLEAR_WINDOW)
	{
		sprintf(char_buffer,"DEBUG VALUE\0");
		SetColor(background);
		DisplayFunctionName(char_buffer);
		Convert_For_Display(tempValue, NULL, oldDebug, DEBUG_DISPLAY_SIZE);
		DisplayFunctionValue(tempValue, 30);
	}
	else if(update_type == NEW_WINDOW)
	{
		sprintf(char_buffer,"DEBUG VALUE\0");
		SetColor(foreground);
		DisplayFunctionName(char_buffer);
		Convert_For_Display(tempValue, NULL, newDebug, DEBUG_DISPLAY_SIZE);
		DisplayFunctionValue(tempValue, 30);
	}
	else if((update_type == UPDATE_WINDOW) || (update_type == RESET_WINDOW))
	{
		if(strncmp((char *)oldDebug, (char *)newDebug, DEBUG_DISPLAY_SIZE))
		{
			SetColor(background);
			Convert_For_Display(tempValue, NULL, oldDebug, DEBUG_DISPLAY_SIZE);
			DisplayFunctionValue(tempValue, 30);
			SetColor(foreground);
			Convert_For_Display(tempValue, NULL, newDebug, DEBUG_DISPLAY_SIZE);
			DisplayFunctionValue(tempValue, 30);
		}
	}
	strncpy(oldDebug,newDebug,DEBUG_DISPLAY_SIZE);
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	END OF FUNCTION UPDATES
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//*	DISPLAY FUNCTIONS
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void DisplayFunctionName(char *name)
//*
//*	Description:
//* 	None
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	None
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

void DisplayFunctionName(char *name)
{
	XCHAR xchar_buffer[30] = {0};
	char char_buffer_1[30] = {0};
	char char_buffer_2[30] = {0};
	char index_1, index_2;
	char length;
	char space_index;

	SetFont((void*)&Normal_Arial_Black_8);
	sprintf(char_buffer_1,"%s\0", name);
	length = strlen(char_buffer_1);

	for(index_1 = 0, space_index = 0; (index_1 < length) && (space_index == 0); index_1++)
	{
		if(char_buffer_1[index_1] == 0x20)
		{
			space_index = index_1;
			char_buffer_1[index_1] = '\0';
		}
	}

	if(space_index != 0)
	{
		for(index_1 = space_index + 1, index_2 = 0; index_1 < length; index_2++, index_1++)
		{
			char_buffer_2[index_2] = char_buffer_1[index_1];
			char_buffer_1[index_1] = '\0';
		}
		char_buffer_2[index_2] = '\0';

		length = strlen(char_buffer_1);
		for(index_1 = 0; index_1< (length + 1); index_1++)
			xchar_buffer[index_1] = '\0';
		Convert_For_Display(xchar_buffer, NULL, char_buffer_1, length);
		MoveTo(X_FUNCTION_NAME, (FUNCTION_Y - FUNCTION_Y_OFFSET));
		while(!OutText((XCHAR*)xchar_buffer));

		length = strlen(char_buffer_2);
		for(index_1 = 0; index_1< (length + 1); index_1++)
			xchar_buffer[index_1] = '\0';
		Convert_For_Display(xchar_buffer, NULL, char_buffer_2, length);
		MoveTo(X_FUNCTION_NAME, (FUNCTION_Y - FUNCTION_Y_OFFSET + 12));
		while(!OutText((XCHAR*)xchar_buffer));
	}
	else
	{
		Convert_For_Display(xchar_buffer, NULL, char_buffer_1, length);
		MoveTo(X_FUNCTION_NAME, (FUNCTION_Y - FUNCTION_Y_OFFSET + 12));
		while(!OutText((XCHAR*)xchar_buffer));
	}

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void DisplayFunctionEvent(BYTE *displayEvent, UINT16 location)
//*
//*	Description:
//* 	
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	None
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

void DisplayFunctionEvent(BYTE *displayEvent, UINT16 location)
{
	XCHAR xchar_buffer[30];
	
	Convert_For_Display(xchar_buffer, NULL, displayEvent, 30);
	MoveTo(location ,(FUNCTION_Y - FUNCTION_Y_OFFSET));
	while(!OutText((XCHAR*)xchar_buffer));
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void DisplayFunctionCenter(void)
//*
//*	Description:
//* 	None
//*
//*	Precondition:
//* 	Font must be set before calling this routine.
//*
//*	Parameters:
//* 	None
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	(9DP v1.03.01)
//*

void DisplayFunctionCenter(XCHAR *value, INT offset)
{
	unsigned short value_width = 0;
	unsigned short value_location = 0;

	value_width = GetTextWidth((XCHAR*)value, (void*)_font);
	value_location = 160 - (value_width / 2);
	MoveTo(value_location, FUNCTION_Y - FUNCTION_Y_OFFSET + offset);
	while(!OutText((XCHAR*)value));
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void DisplayFunctionValue(void)
//*
//*	Description:
//* 	None
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	None
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

void DisplayFunctionValue(XCHAR *value, INT offset)
{
	unsigned short value_width = 0;
	unsigned short value_location = 0;

	SetFont((void*)&Secondary_Arial_Black_14);
	value_width = GetTextWidth((XCHAR*)value, (void*)&Secondary_Arial_Black_14);
	value_location = X_FUNCTION_VALUE + offset - value_width;										// right justify
	MoveTo(value_location, FUNCTION_Y - FUNCTION_Y_OFFSET);
	while(!OutText((XCHAR*)value));
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void DisplayFunctionUnits(XCHAR *units, INT offset)
//*
//*	Description:
//* 	None
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	None
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

void DisplayFunctionUnits(XCHAR *units, INT offset)
{
	SetFont((void*)&Secondary_Arial_Black_14);
	MoveTo(X_FUNCTION_UNITS + offset, FUNCTION_Y - FUNCTION_Y_OFFSET);
	while(!OutText((XCHAR*)units));
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void DisplayFunctionState(UINT8 value)
//*
//*	Description:
//* 	None
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	None
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

void DisplayFunctionState(UINT8 value)
{
	SetFont((void*)&Secondary_Arial_Black_14);
	MoveTo(250 ,(FUNCTION_Y - FUNCTION_Y_OFFSET));
	if(value == 0)
	{
		while(!OutText((XCHAR*)OffTxt));																				// moved to the end of routine (9DP v1.02.02)
		if(lmi.function[Current_Function_Index].info.header.type == PULSED_MODE)
		{
			if(Correction_Factor_Mode == 0)																				// (9DP v1.02.02)
				lmi.calibration.controls.info.fixed_range = 0;
		}
	}
	else if(value == 1)
	{
		while(!OutText((XCHAR*)OnTxt));																					// moved to the end of routine (9DP v1.02.02)
		if(lmi.function[Current_Function_Index].info.header.type == DATA_LOGGING)
		{
			log_file_write_tick = tick + (lmi.features.data_logging.info.sample_period * (DWORD) TICKS_PER_SEC);		// establish new tick timer (9DP v1.01.05)
			Update_Log_File = FALSE;																					// clear flag so first report will be one full sample later
		}
		else if(lmi.function[Current_Function_Index].info.header.type == PULSED_MODE)
		{
			if(Correction_Factor_Mode == 0)																				// (9DP v1.02.02)
				lmi.calibration.controls.info.fixed_range = 5;
		}
	}

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	END OF DISPLAY FUNCTIONS
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//*	UPDATE FUNCTION DISPLAY
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//*		void UpdateFunctionVariables(void)
//*
//*	Description:
//* 	None
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	None
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

void UpdateFunctionVariables(void)
{
	BYTE index;
	BYTE function_index;

	Range_Lock = FALSE;

	for(index = 0; index < DEFAULT_VIEW_NUMBER_OF_FUNCTIONS; index++)
	{
		if(lmi.display.detector[Current_Detector].view[Current_View].functions.info.function[index].enable)
		{
			function_index = lmi.display.detector[Current_Detector].view[Current_View].functions.info.function[index].function_index;

			switch(lmi.function[function_index].info.header.type)
			{
				case INTEGRATE:
				{
//					lmi.features.integrate.info.operating_mode = ON;								// hidden (v1.01.13)
					break;
				}
				case PEAK_HOLD:
				{
					Range_Lock = (BOOL) lmi.function[function_index].info.peak_rate.element_range_lock;
					break;
				}
			}
		}
	}
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	END OF UPDATE FUNCTION DISPLAY
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//* FUNCTION ALERTS AND ALARMS
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CheckIntegrate(void)
//*
//*	Description:
//* 	None
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	None
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

void CheckIntegrate(void)
{
	float dose_value;
	
	if(lmi.display.detector[Current_Detector].view[Current_View].alarms_integrate.info.feature.enable == TRUE)
	{
		dose_value = UnitConversions(Rate_Accumulation, Default_Dose_Units, lmi.display.detector[Current_Detector].view[Current_View].alarms_integrate.info.feature.reading.units);
		if((dose_value >= Integrate_Alarm_Rate) && (Integrate_Alarm_Ack_Flag == FALSE))
		{
			New_Alarm = ALARM;
			add_error_message_to_queue(ALARM_INTEGRATE);
			Integrate_Alarm_Flag = TRUE;
			Integrate_Alert_Flag = TRUE;																							// force alert as well (9DP v1.01.11)
			return;
		}
	}

	if(lmi.display.detector[Current_Detector].view[Current_View].alerts_integrate.info.feature.enable == TRUE)
	{
		dose_value = UnitConversions(Rate_Accumulation, Default_Dose_Units, lmi.display.detector[Current_Detector].view[Current_View].alerts_integrate.info.feature.reading.units);
		if((dose_value >= Integrate_Alert_Rate) && (Integrate_Alert_Ack_Flag == FALSE))
		{
			New_Alarm = ALERT;
			add_error_message_to_queue(ALERT_INTEGRATE);
			Integrate_Alert_Flag = TRUE;
			return;
		}
	}

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CheckTimedIntegrate(void)
//*
//*	Description:
//* 	None
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	None
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

void CheckTimedIntegrate(void)
{
	if((Start_Timed_Dose == TRUE) && (Stop_Timed_Dose == TRUE))
	{
		Stop_Timed_Dose = FALSE;
	}
	else if((Start_Timed_Dose == FALSE) && (Stop_Timed_Dose == FALSE))
	{
		New_Alarm = ALERT;
		add_error_message_to_queue(ALERT_SCALER);
	}
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	END OF FUNCTION ALERTS AND ALARMS
//**************************************************************************************************
//**************************************************************************************************

//*
//**************************************************************************************************
//**************************************************************************************************
//* LUDLUM MEASUREMENTS INC. COPYRIGHT (C) 2012
//**************************************************************************************************
//**************************************************************************************************

