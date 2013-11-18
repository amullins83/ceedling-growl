//**************************************************************************************************
//**************************************************************************************************
//*																		
//*	Filename:		Conversions.h
//*	Date:			08/25/10
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
//*	~~~~~~~~	05/23/12	James Comstock
//*	- updated the instrument to the latest Microchip applications library
//*	- eliminated all compiler warnings
//*
//*	~~~~~~~~	08/25/10	James Comstock
//*	- Original Prototype
//*                                                                     
//**************************************************************************************************
//*                                                                     
//*	Element Screen Routines
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

#include "Conversions.h"																			// include for the obvious reasons
#include "instrument.h"																					// include for the data structures
#include "Definitions.h"																			// include for the data structures
#include "GenericTypeDefs.h"																		// include for the type definitions

//*
//**************************************************************************************************
//*

extern TYPE_INSTRUMENT lmi;
extern TYPE_FLOAT_READING_FORMAT new_reading;
extern BYTE Current_Function_Index;

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	float FloatValueConversion(float value, UNIT_MULTIPLIER multiplier)
//*
//*	Description:
//* 	This routine multiplies float values by their given multiplier and returns the result.
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

float FloatValueConversion(float value, unsigned char multiplier)
{
	return(value * MultiplierConversion(multiplier));
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	float ShortValueConversion(short value, UNIT_MULTIPLIER multiplier)
//*
//*	Description:
//* 	This routine multiplies float values by their given multiplier and returns the result.
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

float ShortValueConversion(unsigned short value, unsigned char multiplier)
{
	return(value * MultiplierConversion(multiplier));
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	short RelativeMultiplier(short measurement_multiplier, short event_multiplier)
//*
//*	Description:
//* 	This routine determines the multiplier relative to the instrument storage multiplier.
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

short RelativeMultiplier(short measurement_multiplier, short event_multiplier)
{
	return ((short)UNITY + (measurement_multiplier - event_multiplier));
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	float MultiplierConversion(TYPE_MULTIPLIER multiplier)
//*
//*	Description:
//* 	This routine converts a given multiplier into its appropriate float value
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

float MultiplierConversion(unsigned char multiplier)
{
	switch(multiplier)
	{
		case NO_MULTIPLIER:
		{
			return 0e00;
		}
		case ZEPTO:
		{
			return 1e-21;
		}
		case ATTO:
		{
			return 1e-18;
		}
		case FEMTO:
		{
			return 1e-15;
		}
		case PICO:
		{
			return 1e-12;
		}
		case NANO:
		{
			return 1e-09;
		}
		case MICRO:
		{
			return 1e-06;
		}
		case MILLI:
		{
			return 1e-03;
		}
		case UNITY:
		{
			return 1e00;
		}
		case KILO:
		{
			return 1e03;
		}
		case MEGA:
		{
			return 1e06;
		}
		case GIGA:
		{
			return 1e09;
		}
		case TERA:
		{
			return 1e12;
		}
		case PETA:
		{
			return 1e15;
		}
		case EXA:
		{
			return 1e18;
		}
		case ZETTA:
		{
			return 1e21;
		}
		default:
		{
			return 1e00;
		}
	}
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void DateStringConversion(TYPE_DATE_FORMAT *datepointer, BYTE *datestringpointer)
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

//void DateStringConversion(BYTE *datepointer, BYTE *datestringpointer)
void DateStringConversion(TYPE_DATE_FORMAT *datepointer, BYTE *datestringpointer)																						// (9DP v1.01.21)
{
	char newMonth[4] = {0};																																				// (9DP v1.03.00)
																																										// (9DP v1.03.00)
	if(lmi.settings.clock.info.time.format == 12)																														// (9DP v1.03.00)
	{																																									// (9DP v1.03.00)
//		sprintf(datestringpointer,"%02d/%02d/20%02d\0",lmi.settings.calendar.info.date.month,lmi.settings.calendar.info.date.day,lmi.settings.calendar.info.date.year);	// (9DP v1.03.00)
		sprintf(datestringpointer,"%02d/%02d/20%02d\0",datepointer->month,datepointer->day,datepointer->year);															// (9DP v1.03.01)
	}																																									// (9DP v1.03.00)
	else																																								// (9DP v1.03.00)
	{																																									// (9DP v1.03.00)
//		CreateShortMonthList(lmi.settings.calendar.info.date.month, (char *)newMonth);																					// (9DP v1.03.00)
//		CreateShortMonthList(datepointer->month, (char *)newMonth);																										// (9DP v1.03.01)
//		sprintf(datestringpointer,"%02d %s %02d\0", lmi.settings.calendar.info.date.day, newMonth, lmi.settings.calendar.info.date.year);								// (9DP v1.03.00)
		sprintf(datestringpointer,"%02d %s %02d\0", datepointer->day, newMonth, datepointer->year);																		// (9DP v1.03.01)
	}																																									// (9DP v1.03.00)
//	sprintf(datestringpointer,"%02d/%02d/%04d\0",datepointer->month, datepointer->day,(2000 + datepointer->year));														// (9DP v1.01.21)
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void TimeStringConversion(TYPE_TIME_FORMAT *timepointer, BYTE *timestringpointer)
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

//void TimeStringConversion(BYTE *timepointer, BYTE *timestringpointer)
void TimeStringConversion(TYPE_TIME_FORMAT *timepointer, BYTE *timestringpointer)										// (9DP v1.01.21)
{
	char temp_hours;																									// (9DP v1.03.01)

	if(timepointer->format == 12)																						// (9DP v1.03.01)
	{																													// (9DP v1.03.01)
		temp_hours = timepointer->hours % 12;																			// (9DP v1.03.01)
		if(temp_hours == 0) temp_hours = 12;																			// (9DP v1.03.01)
		if(timepointer->hours < 12)																						// (9DP v1.03.01)
		{																												// (9DP v1.03.01)
			sprintf(timestringpointer,"%02d:%02d:%02d AM\0",temp_hours,timepointer->minutes,timepointer->seconds);		// (9DP v1.03.01)
		}																												// (9DP v1.03.01)
		else																											// (9DP v1.03.01)
		{																												// (9DP v1.03.01)
			sprintf(timestringpointer,"%02d:%02d:%02d PM\0",temp_hours,timepointer->minutes,timepointer->seconds);		// (9DP v1.03.01)
		}																												// (9DP v1.03.01)
	}																													// (9DP v1.03.01)
	else																												// (9DP v1.03.01)
	{																													// (9DP v1.03.01)
		sprintf(timestringpointer,"%02d:%02d:%02d\0",timepointer->hours,timepointer->minutes,timepointer->seconds);		// (9DP v1.03.01)
	}																													// (9DP v1.03.01)
//	sprintf(timestringpointer,"%02d:%02d:%02d\0",timepointer->hours, timepointer->minutes, timepointer->seconds);		// (9DP v1.01.21)
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void UnitConversions(TYPE_FLOAT_READING_FORMAT *old_value, BYTE new_units)
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
//*
//*		case NO_UNITS:
//*		case SV_HR:																					// sievert per hour
//*		case R_HR:																					// roentgen per hour
//*		case REM_HR:																				// REM per hour
//*		case GY_HR:																					// gray per hour
//*		case CPS:																					// counts per second
//*		case CPM:																					// counts per minute
//*		case DPS:																					// disintegrations per second
//*		case DPM:																					// disintegrations per minute
//*		case BQ:																					// becquerel
//*		case R:																						// roentgen
//*		case SV:																					// sievert
//*		case REM:																					// REM
//*		case GY:																					// gray
//*		case COUNTS:																				// counts
//*		case DISINTEGRATIONS:																		// disintegrations
//*

#define CPS_CONVERSION_44_2							2.9167											// cps / constant = (uR/hr)
//#define CPS_CONVERSION_44_7						0.035											// cps / constant = (uR/hr)
#define CPS_CONVERSION_44_7							35000.0											// cps / constant = (uR/hr)

#define CPS_RATE_CONVERSION_44_9					0.055											// cps / constant = (uR/hr)
#define CPS_DOSE_CONVERSION_44_9					198.0											// cps / constant = (uR/hr)

float UnitConversions(float old_value, BYTE old_units, BYTE new_units)
{
	switch(old_units)
	{
		case R_HR:
		{
			switch(new_units)
			{
				case R_HR:																			// roentgen per hour
				{
					return (old_value);
				}
				case SV_HR:																			// sievert per hour
				{
					return (old_value * 0.0106);
				}
				case GY_HR:																			// gray per hour
				{
					return (old_value * 0.00878);
				}
			}
			break;
		}
		case R:
		{
			switch(new_units)
			{
				case R:																				// roentgen
				case REM:																			// REM (9DP v1.01.10)
				{
					return (old_value);
				}
				case SV:																			// sievert
				{
					return (old_value * 0.0106);
				}
				case GY:																			// gray
				{
					return (old_value * 0.00878);
				}
			}
			break;
		}
		case CPS:
		{
			switch(new_units)
			{
				case R:																				// roentgen
				case REM:																			// REM (9DP v1.01.10)
				{
					return (old_value / CPS_DOSE_CONVERSION_44_9);
				}
				case R_HR:																			// roentgen per hour
				{
					return (old_value / CPS_RATE_CONVERSION_44_9);
				}
				case CPS:																			// counts per second
				{
					return (old_value);
				}
				case CPM:																			// counts per minute
				{
					return (old_value * 60.0);																
				}
			}
			break;
		}
		case SV_HR:																					// (9DP v1.01.10)
		{
			switch(new_units)
			{
				case R_HR:																			// roentgen per hour
				{
					return (old_value / 0.0106);
				}
			}
			break;
		}
		case SV:																					// (9DP v1.01.10)
		{
			switch(new_units)
			{
				case R:																				// roentgen
				{
					return (old_value / 0.0106);
				}
			}
			break;
		}
		case GY_HR:																					// (9DP v1.01.10)
		{
			switch(new_units)
			{
				case R_HR:																			// roentgen per hour
				{
					return (old_value / 0.00878);
				}
			}
			break;
		}
		case GY:																					// (9DP v1.01.10)
		{
			switch(new_units)
			{
				case R:																				// roentgen
				{
					return (old_value / 0.00878);
				}
			}
			break;
		}
	}
	return (0.000);
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BYTE RateToDoseConversion(BYTE rate_units)
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

BYTE RateToDoseConversion(BYTE old_units)
{
	switch(old_units)
	{
		case R_HR:
		{
			return R;
		}
		case SV_HR:
		{
			return SV;
		}
		case REM_HR:
		{
			return REM;
		}
		case GY_HR:
		{
			return GY;
		}
	}
//	return lmi.function[Current_Function_Index].info.integrate.units;
	return lmi.features.integrate.info.current_reading.units;										// (9DP v1.01.15)
}

//*
//**************************************************************************************************
//**************************************************************************************************
//* LUDLUM MEASUREMENTS INC. COPYRIGHT (C) 2010 
//**************************************************************************************************
//**************************************************************************************************
