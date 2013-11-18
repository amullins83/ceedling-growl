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

#ifndef _CONVERSIONS_INCLUDE_
#define _CONVERSIONS_INCLUDE_

#include "GenericTypeDefs.h"
//#include "instrument.h"
#include "Definitions.h"
//#include "System.h"

float FloatValueConversion(float value, unsigned char multiplier);
float ShortValueConversion(unsigned short value, unsigned char multiplier);
short RelativeMultiplier(short measurement_multiplier, short event_multiplier);
float MultiplierConversion(unsigned char multiplier);
//void DateStringConversion(BYTE *datepointer, BYTE *datestringpointer);
void DateStringConversion(TYPE_DATE_FORMAT *datepointer, BYTE *datestringpointer);					// (v1.02.00)
//void TimeStringConversion(BYTE *timepointer, BYTE *timestringpointer);
void TimeStringConversion(TYPE_TIME_FORMAT *timepointer, BYTE *timestringpointer);					// (v1.02.00)
float UnitConversions(float old_value, BYTE old_units, BYTE new_units);
BYTE RateToDoseConversion(BYTE rate_units);

#endif

//*
//**************************************************************************************************
//**************************************************************************************************
//* LUDLUM MEASUREMENTS INC. COPYRIGHT (C) 2010 
//**************************************************************************************************
//**************************************************************************************************
