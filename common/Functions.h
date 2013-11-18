//**************************************************************************************************
//**************************************************************************************************
//*																		
//*	Filename:		Functions.h
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

#ifndef _FUNCTIONS_INCLUDE_
#define _FUNCTIONS_INCLUDE_

#include "GenericTypeDefs.h"

void New_Function_Window(void);
void Clear_Function_Window(void);
void Reset_Function(float value);
void Change_Function(void);

void Update_Function(UINT8 update_type);
void UpdateAccumDose(UINT8 Update_Type);
void UpdateTimedDose(UINT8 update_type);
void UpdatePeakRate(UINT8 Update_Type);
void UpdateDataLogging(UINT8 update_type);
void UpdateTimeDisplay(UINT8 update_type);
void UpdateDateDisplay(UINT8 update_type);
void UpdateHighIntegrate(UINT8 update_type);
void UpdateHighVoltageDisplay(UINT8 update_type);
void UpdateRange(UINT8 update_type);
void UpdateDebugDisplay(UINT8 update_type);

//#ifndef INSTRUMENT_9D																				// (9DP v1.03.00)
void UpdateInputVoltage(UINT8 update_type);
void UpdatePulsedMode(UINT8 update_type);
void UpdatePeakVoltage(UINT8 update_type);
void UpdateChamberTemperatureDisplay(UINT8 update_type);
void UpdateTemperatureOffsetDisplay(UINT8 update_type);
void UpdateCorrectionFactorMode(UINT8 update_type);
void UpdateChamberPressureDisplay(UINT8 update_type);
//#endif

//#ifndef INSTRUMENT_9D																				// (9DP v1.03.00)
void UpdateInstrumentTemperatureDisplay(UINT8 update_type);
//#endif

void DisplayFunctionName(char *name);
void DisplayFunctionUnits(XCHAR *units, INT offset);
void DisplayFunctionValue(XCHAR *value, INT offset);
void DisplayFunctionCenter(XCHAR *value, INT offset);												// (9DP v1.03.01)
void DisplayFunctionState(UINT8 value);
void DisplayFunctionEvent(BYTE *displayEvent, UINT16 location);
void UpdateFunctionVariables(void);
void CheckIntegrate(void);
void CheckTimedIntegrate(void);

//**************************************************************************************************
//**************************************************************************************************
//* EEPROM Function Declarations
//**************************************************************************************************
//**************************************************************************************************

void EEPROM_Clear(UINT8 EEPROMchipAddress);
void EEPROM_Transfer(UINT8 FromEEPROMchipAddress, UINT8 ToEEPROMchipAddress);

void EEPROM_Initialize(BOOL wipe);
void EEPROM_Function_Defaults(BYTE function_index, FUNCTION_TYPE function);
void EEPROM_Assign_Global_Variables(void);
void EEPROM_Set_Modes(void);
unsigned char Check_Model_Number(unsigned char model_number);
BOOL Change_Model_Number(BYTE new_model_number, char *new_model);
short Designate_Color(short);

void EEPROM_Read_System_Data(void);
void EEPROM_Load_System_Defaults(void);

void EEPROM_Read_Calibration_Data(void);
void EEPROM_Load_Calibration_Defaults(void);

void EEPROM_Write_All(void);

#endif

//*
//**************************************************************************************************
//**************************************************************************************************
//* LUDLUM MEASUREMENTS INC. COPYRIGHT (C) 2010 
//**************************************************************************************************
//**************************************************************************************************
