//**************************************************************************************************
//**************************************************************************************************
//*																		
//*	Filename:		Screens.h
//*	Date:			08/20/10                                          	
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

#ifndef _SCREENS_H
#define _SCREENS_H

//#define NUMBER_OF_ERRORS 10

#define SOFT_BTN1_XSTART		X_SPACING															// Shifted by 2 to leave 2 pixels at the left and the right side of the screen
#define SOFT_BTN1_YSTART		(239 - SOFTKEY_BTN_HEIGHT-Y_SPACING)								// 205

#define SOFT_BTN2_XSTART		(SOFT_BTN1_XSTART+SOFTKEY_BTN_WIDTH+BUTTON_SPACING)					// 82
#define SOFT_BTN2_YSTART		(239 - SOFTKEY_BTN_HEIGHT-Y_SPACING)								// 205

#define SOFT_BTN3_XSTART		(SOFT_BTN2_XSTART+SOFTKEY_BTN_WIDTH+BUTTON_SPACING)					// 162
#define SOFT_BTN3_YSTART		(239 - SOFTKEY_BTN_HEIGHT-Y_SPACING)								// 205

#define SOFT_BTN4_XSTART		(SOFT_BTN3_XSTART+SOFTKEY_BTN_WIDTH+BUTTON_SPACING)					// 242
#define SOFT_BTN4_YSTART		(239 - SOFTKEY_BTN_HEIGHT-Y_SPACING)								// 205

#define SOFTKEY_BTN_WIDTH		75
#define SOFTKEY_BTN_HEIGHT		34
#define BUTTON_SPACING			5

#define NOTE_REGION_XSTART		0
#define NOTE_REGION_YSTART		0
#define NOTE_REGION_XFINISH		79
#define NOTE_REGION_YFINISH		HEADER_HEIGHT	

#define STATUS_REGION_XSTART	240
#define STATUS_REGION_YSTART	0
#define STATUS_REGION_XFINISH	319
#define STATUS_REGION_YFINISH	HEADER_HEIGHT	

#define ALARM_ARC_COLOR			BRIGHTRED
//#define ALERT_ARC_COLOR			BRIGHTYELLOW
#define ALERT_ARC_COLOR			BURNTYELLOW
#define NORMAL_ARC_COLOR		BRIGHTGREEN

//**************************************************************************************************
//*	Colors

enum colors
{
	BLACK_EEPROM = 0,
	BRIGHTBLUE_EEPROM,
	BRIGHTGREEN_EEPROM,
	BRIGHTCYAN_EEPROM,
	BRIGHTRED_EEPROM,
	BRIGHTMAGENTA_EEPROM,
	BRIGHTYELLOW_EEPROM,
	BLUE_EEPROM,
	GREEN_EEPROM,
	CYAN_EEPROM,
	RED_EEPROM,
	MAGENTA_EEPROM,
	BROWN_EEPROM,
	LIGHTGRAY_EEPROM,
	DARKGRAY_EEPROM,
	LIGHTBLUE_EEPROM,
	LIGHTGREEN_EEPROM,
	LIGHTCYAN_EEPROM,
	LIGHTRED_EEPROM,
	LIGHTMAGENTA_EEPROM,
	YELLOW_EEPROM,
	WHITE_EEPROM,
	GRAY0_EEPROM,
	GRAY1_EEPROM,
	GRAY2_EEPROM,
	GRAY3_EEPROM,
	GRAY4_EEPROM,
	GRAY5_EEPROM,
	GRAY6_EEPROM
};

enum error_message
{
	// No Message
	NORMAL_MESSAGE = 0,																							
	
	// Alerts are 1 - 49
	ALERT_RADIATION = 1,																			
	ALERT_INTEGRATE,
	ALERT_FUNCTION,
	ALERT_LOW_BATTERY,
	ALERT_LOW_MEMORY,
	ALERT_CALIBRATION_DUE_DATE,
	ALERT_SOURCE_CHECK,
	ALERT_BACKGROUND_CHECK,
	ALERT_SCALER,

	// Alarms are 50 - 99
	ALARM_RADIATION = 50,			
	ALARM_INTEGRATE,
	ALARM_FUNCTION,
	ALARM_LOW_BATTERY,
	ALARM_LOW_MEMORY,
	ALARM_CALIBRATION_DUE_DATE,
	ALARM_SOURCE_CHECK,
	ALARM_BACKGROUND_CHECK,
	ALARM_SCALER,

	// System Warnings are 100 - 149
	WARNING_LOW_BATTERY = 100,		
	WARNING_SHUTDOWN,
	WARNING_LOW_MEMORY,
	WARNING_CALIBRATION_DUE,
	WARNING_SOURCE_CHECK,
	WARNING_HV_LOW,
	WARNING_HV_HIGH,
	WARNING_LOW_READING,

	// xx Errors are 150 - 199
	ERROR_OUT_OF_MEMORY = 150,
	ERROR_FILE_OPEN,
	ERROR_FILE_SYSTEM,

	// USB Errors are 200 - 249
	USB_ERROR_MSG = 200,
	USB_ERROR_HUB,
	USB_ERROR_NO_DEVICE_SUPPORT,
	USB_ERROR_ENUMERATION,
	USB_ERROR_CLIENT_INIT,
	USB_ERROR_NO_MEMORY,
	USB_ERROR_UNSPECIFIED,

	// User defined error message
	USER_DEFINED = 255
};

//**************************************************************************************************
//*	Function Declarations

void StartupScreen(void);
void LudlumScreen(void);
void ScrambleClearScreen(void);
void Use_Custom_Message(unsigned char* custom_message, UINT8 msg_location, UINT8 msg_condition, UINT8 text_alignment);
void draw_soft_buttons(XCHAR *pTextbtnA, XCHAR *pTextbtnB, XCHAR *pTextbtnC, XCHAR *pTextbtnD);
WORD UserDefinedMsgCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg);
void USB_Error_Message(UINT8 condition);
void File_Error_Message(UINT8 condition);
void ClearTopRightText(void);
void DisplayTopRightText(BYTE message);
void ClearTopLeftText(void);
void DisplayTopLeftText(BYTE message);
void DisplaySystemProcess(BYTE process);

void Thumbdrive_File_Menu(void);

#endif 

//*
//**************************************************************************************************
//**************************************************************************************************
//* LUDLUM MEASUREMENTS INC. COPYRIGHT (C) 2010 
//**************************************************************************************************
//**************************************************************************************************


