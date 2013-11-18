//**************************************************************************************************
//**************************************************************************************************
//*																		
//*	Filename:		Commands.h
//*	Date:			09/13/10
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
//*	~~~~~~~~	09/13/10	James Comstock
//*	- Original Prototype
//*                                                                     
//**************************************************************************************************
//*                                                                     
//*	Command Processing Routines
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

#ifndef _COMMANDS_INCLUDE_
#define _COMMANDS_INCLUDE_

#include "GenericTypeDefs.h"
#include "instrument.h"
#include "Definitions.h"
#include "System.h"

//**************************************************************************************************
//**************************************************************************************************
//* Defines

//#define MESSAGE_SIZE								196
#define MESSAGE_SIZE								250												// (9DP v1.01.06)	
#define COMMAND_SIZE								3
#define SEPARATOR_SIZE								1
#define INSTRUMENT_SIZE								MESSAGE_SIZE
#define TIER_1_SIZE									(INSTRUMENT_SIZE - COMMAND_SIZE - SEPARATOR_SIZE)
#define TIER_2_SIZE									(TIER_1_SIZE - COMMAND_SIZE - SEPARATOR_SIZE)
#define TIER_3_SIZE									(TIER_2_SIZE - COMMAND_SIZE - SEPARATOR_SIZE)
#define TIER_4_SIZE									(TIER_3_SIZE - COMMAND_SIZE - SEPARATOR_SIZE)
#define TIER_5_SIZE									(TIER_4_SIZE - COMMAND_SIZE - SEPARATOR_SIZE)
#define TIER_6_SIZE									(TIER_5_SIZE - COMMAND_SIZE - SEPARATOR_SIZE)
#define TIER_7_SIZE									(TIER_6_SIZE - COMMAND_SIZE - SEPARATOR_SIZE)
#define TIER_8_SIZE									(TIER_7_SIZE - COMMAND_SIZE - SEPARATOR_SIZE)
#define TIER_9_SIZE									(TIER_8_SIZE - COMMAND_SIZE - SEPARATOR_SIZE)

#define CARRIAGE_RETURN								0x0D
#define LINEFEED									0x0A

#define VALID_MESSAGE								0
#define INVALID_MESSAGE_LENGTH						1
#define INVALID_MESSAGE_TERMINATION					2

//**************************************************************************************************
//**************************************************************************************************
//* Structures

typedef struct _COMMAND
{
	unsigned char command[COMMAND_SIZE];
	unsigned char separator;
} TYPE_COMMAND;

typedef struct _TIER
{
	TYPE_COMMAND command;
	unsigned char message[TIER_1_SIZE];
} TYPE_INSTRUMENT_MESSAGE;

//**************************************************************************************************
//**************************************************************************************************
//* Function Definitions

BYTE Terminate_Message(char *message_string);
char * Parse_Message(char *message_string);

void Message_Handler(void);

//* Instrument Handler
BOOL Instrument_Handler(void);

//* System Handlers
BOOL System_Process(BYTE process);

//* Settings Handlers
BOOL Settings_Handler(void);
BOOL Settings_Product_Handler(void);
BOOL Settings_Product_Process(BYTE action);															// required because of mixed tier messages
BOOL Settings_Product_Model_Number(char *message);													// (3D v1.00.04)
//BOOL Settings_Product_Model_Number(void);
BOOL Settings_Language_Handler(void);
BOOL Settings_Calendar_Handler(void);
BOOL Settings_Clock_Handler(void);
BOOL Settings_Passwords_Handler(void);
BOOL Settings_Firmware_Handler(void);

//* Features Handlers
BOOL Features_Handler(void);
BOOL Features_Source_Handler(void);
BOOL Features_Source_Process(BYTE process);															// required because of mixed tier messages
BOOL Features_Background_Handler(void);
BOOL Features_Background_Process(BYTE process);														// required because of mixed tier messages
BOOL Features_Integrate_Handler(void);
BOOL Features_Integrate_Process(BYTE process);														// required because of mixed tier messages
BOOL Features_Data_Logging_Handler(void);
BOOL Features_Data_Logging_Process(BYTE process);													// required because of mixed tier messages
void Request_New_Header(BYTE lastvalue, BYTE value);

//* Controls Handlers
BOOL Controls_Handler(void);
BOOL Controls_Backlight_Handler(void);
BOOL Controls_Audio_Handler(void);
BOOL Controls_GPS_Handler(void);
BOOL Controls_Bluetooth_Handler(void);

//* Display Handlers
BOOL Display_Handler(void);
BOOL Display_Properties_Handler(void);
BOOL Display_Detector_Handler(void);

//* Detector Properties Handlers
BOOL Detector_Handler(void);
BOOL Detector_Properties_Handler(void);
BOOL Detector_Calibration_Handler(void);
BOOL Detector_Views_Handler(void);

//* Views Handlers
BOOL Views_Settings_Handler(void);
BOOL Views_Appearance_Handler(BYTE view);
BOOL Views_Properties_Handler(void);
BOOL Views_Element_Header_Handler(BYTE view);
BOOL Views_Element_Handler(BYTE view);
BOOL Views_Functions_Handler(BYTE view);
BOOL Views_Alerts_Handler(BYTE view);
BOOL Views_Alerts_Radiation_Handler(BYTE view);
BOOL Views_Alerts_Integrate_Handler(BYTE view);
BOOL Views_Alerts_Function_Handler(BYTE view);
BOOL Views_Alarms_Handler(BYTE view);
BOOL Views_Alarms_Radiation_Handler(BYTE view);
BOOL Views_Alarms_Integrate_Handler(BYTE view);
BOOL Views_Alarms_Function_Handler(BYTE view);
BOOL Views_Element_Style_Handler(BYTE view);
BOOL Views_Element_Style_Arc_Handler(BYTE view);
BOOL Views_Calibration_Handler(BYTE view);

//* Functions Handlers
BOOL Functions_Handler(void);
BOOL Functions_Header_Handler(BYTE function);
BOOL Functions_Integrate_Handler(BYTE function, BYTE type);
BOOL Functions_Peakrate_Handler(BYTE function, BYTE type);
BOOL Functions_Correction_Factor_Handler(BYTE function_index, BYTE type);							// (9DP v1.02.02)
BOOL Functions_Time_Constants_Handler(BYTE function_index, BYTE type);							        // (9DP v1.04.00)
//* Alerts Handlers
BOOL Alerts_Handler(void);
BOOL Alerts_Low_Battery_Handler(void);
BOOL Alerts_Low_Memory_Handler(void);
BOOL Alerts_Calibration_Due_Handler(void);
BOOL Alerts_Source_Check_Handler(void);
BOOL Alerts_Background_Check_Handler(void);

//* Alarms Handlers
BOOL Alarms_Handler(void);
BOOL Alarms_Low_Battery_Handler(void);
BOOL Alarms_Low_Memory_Handler(void);
BOOL Alarms_Calibration_Due_Handler(void);
BOOL Alarms_Source_Check_Handler(void);
BOOL Alarms_Background_Check_Handler(void);
BOOL Alarms_HV_Low_Handler(void);
BOOL Alarms_HV_High_Handler(void);
BOOL Alarms_Low_Reading_Handler(void);

//* Calibration Handlers
BOOL Calibration_Handler(void);
BOOL Calibration_Header_Handler(void);
BOOL Calibration_Checkout_Handler(void);
BOOL Calibration_Constants_Handler(void);
BOOL Calibration_Offsets_Handler(void);
BOOL Calibration_HV_Board_Handler(void);
BOOL Calibration_Controls_Handler(void);
BOOL Calibration_Config_Handler(void);

//* Generic Handlers
BYTE Process_Handler(void);
BYTE Post_Process_Handler(char *message);
//BOOL Date_Handler(BYTE process, char *date_ptr);
BOOL Date_Handler(BYTE process, TYPE_DATE_FORMAT *date_ptr);										// (9DP v1.02.00)
//BOOL Time_Handler(BYTE process, char *time_ptr);
BOOL Time_Handler(BYTE process, TYPE_TIME_FORMAT *time_ptr);										// (9DP v1.02.00)
BOOL Reading_Handler(BYTE process, float *value_ptr, char *units_ptr, char *multiplier_ptr);
BOOL Event_Handler(BYTE value, BYTE maxvalue, BYTE *event);
BOOL Validate_Detector(char value);
void Append_Message(char *tier_command, char *separator);
void Format_String(char *generic_string, char *message, UINT8 message_length);
void Force_Screen_Redraw(void);																		// (9DP v1.01.20)

#endif

//*
//**************************************************************************************************
//**************************************************************************************************
//* LUDLUM MEASUREMENTS INC. COPYRIGHT (C) 2010 
//**************************************************************************************************
//**************************************************************************************************
