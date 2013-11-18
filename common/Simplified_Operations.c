//**************************************************************************************************
//**************************************************************************************************																	
//*	Filename:		Simplified_Operations.c
//*	Date:			08/18/10                                          	
//*                                                                     
//*	Author:			Clint Appling                                   
//*	Company:		Ludlum Measurements                               	
//*	Address:		501 Oak Street										
//*					Sweetwater, TX 79556								
//*                                                                     
//**************************************************************************************************
//**************************************************************************************************
//*                                                                     
//*	Revision Notes:                                                   	
//*																		
//*	- Original Release
//* 	Functions called numerous times 
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

#include "Graphics.h"
#include "instrument.h"
#include "Definitions.h"
#include "System.h"

void Convert_For_Display(XCHAR *FinalStr, char *AddToStr, char *ConvertStr, UINT8 stringlength);
void Unconvert_Display(char *FinalStr, XCHAR *ConvertStr, UINT8 stringlength);
void XCHAR_Copy(XCHAR *FinalStr, XCHAR *ConvertStr, UINT8 stringlength);

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Convert_For_Display(XCHAR *FinalStr, char *AddToStr, char *ConvertStr, UINT8 stringlength)
//*
//*	Description:
//* 	
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//*		stringlength 	- is the length of the string to be appended "ConvertStr"
//*		AddToStr 		- is the string with description text if used.
//*						- length of AddToStr must be long enough to include the ConvertStr contents
//*		FinalStr		- is the XCHAR string used when displaying info to LCD
//*						- length of FinalStr must be at least as long as AddToStr string
//*						- FinalStr contents arranged so "AddToStr" followed by "ConvertStr"
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//*		FinalStr ex.	- if AddToStr = "Test " and ConvertStr = "String 1"
//*						- Resulting FinalStr = "Test String 1"
//*

void Convert_For_Display(XCHAR *FinalStr, char *AddToStr, char *ConvertStr, UINT8 stringlength)
{
	SHORT i;
	char tempstr[61] = {'\0'};
	char tempstr2[61] = {'\0'};
	
	strncpy(tempstr, ConvertStr, stringlength);

	for(i = 0; i < stringlength; i++)
	{
		if(tempstr[i] == '\0')																		// find the end of the serial number
		{
			strncat(tempstr2, tempstr, i);															// shorten string to not include spaces (only works if checking for 'spaces' in if statement
			i = stringlength;																		// set variable to exit loop
			strncpy(tempstr, tempstr2, stringlength);	
		}							
	}

	if(AddToStr != NULL)
	{
		strncat(AddToStr, tempstr, stringlength);													// Added 2nd string values to the end of the 1st string
		stringlength = strlen(AddToStr);															// get overall string length

		for(i = 0; i < stringlength; i++)
		{
			FinalStr[i] = (XCHAR)AddToStr[i];														// convert char array to XCHAR info
		}
	}
	else
	{	
		for(i = 0; i < stringlength; i++)
		{
			FinalStr[i] = (XCHAR)tempstr[i];														// convert char array info to XCHAR info								
		}
	}
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Unconvert_Display(XCHAR *FinalStr, char *AddToStr, char *ConvertStr, UINT8 stringlength)
//*
//*	Description:
//* 	
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//*		stringlength 	- is the length of the string to be converted
//*		ConvertStr		- is the XCHAR string used when displaying info to LCD
//*		FinalStr		- is the char string extracted from ConvertStr
//*						- length of FinalStr must be at least as long as AddToStr string
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//*		None
//*

void Unconvert_Display(char *FinalStr, XCHAR *ConvertStr, UINT8 stringlength)
{
	SHORT index;
	SHORT value;
	
	for(index = 0; index < stringlength; index++)
	{
		value = ConvertStr[index];
		FinalStr[index] = (char) toupper(value);
	}

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Unconvert_Display(XCHAR *FinalStr, char *AddToStr, char *ConvertStr, UINT8 stringlength)
//*
//*	Description:
//* 	
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//*		stringlength 	- is the length of the string to be converted
//*		ConvertStr		- is the XCHAR string used when displaying info to LCD
//*		FinalStr		- is the char string extracted from ConvertStr
//*						- length of FinalStr must be at least as long as AddToStr string
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//*		None
//*

void XCHAR_Copy(XCHAR *FinalStr, XCHAR *ConvertStr, UINT8 stringlength)
{
	SHORT index;
	
	for(index = 0; index < stringlength; index++)
	{
		FinalStr[index] = ConvertStr[index];
	}

	return;
}


//*
//**************************************************************************************************
//**************************************************************************************************
//* LUDLUM MEASUREMENTS INC. COPYRIGHT (C) 2010 
//**************************************************************************************************
//**************************************************************************************************
