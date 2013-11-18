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

#ifndef _ICONS_INCLUDE_
#define _ICONS_INCLUDE_

#include "GenericTypeDefs.h"

//*
//**************************************************************************************************
// External Image Storage Locations

#define RANGE_1_ICON							0x00000
#define RANGE_2_ICON							0x00600
#define RANGE_3_ICON							0x00C00
#define RANGE_4_ICON							0x01200
#define RANGE_5_ICON							0x01800

#define THUMBDRIVE_ICON							0x01E00
#define KEYBOARD_ICON							0x02400
#define COMPUTER_CONNECT_ICON					0x02A00
#define COMPUTER_DISCONNECT_ICON				0x03000

#define BACKLIGHT_ON_ICON						0x03600
#define BACKLIGHT_OFF_ICON						0x03C00
#define BACKLIGHT_AUTO_ICON						0x04200

#define AUDIO_100_ICON							0x04800
#define AUDIO_80_ICON							0x04E00
#define AUDIO_60_ICON							0x05400
#define AUDIO_40_ICON							0x05A00
#define AUDIO_20_ICON							0x06000
#define AUDIO_0_ICON							0x06600

#define BATTERY_100_ICON						0x06C00
#define BATTERY_80_ICON							0x07200
#define BATTERY_60_ICON							0x07800
#define BATTERY_40_ICON							0x07E00
#define BATTERY_20_ICON							0x08400
#define BATTERY_0_ICON							0x08A00
#define BATTERY_CHARGE_ICON						0x09000

#define AUDIO_DISABLE_ICON						0x09600

#define CF_1_ICON								0x09C00												// creates overflow. needs correcting (9DP v1.02.02)
#define CF_2_ICON								0x09C00												// creates overflow. needs correcting (9DP v1.02.02)
#define CF_3_ICON								0x09C00												// creates overflow. needs correcting (9DP v1.02.02)

#define SPLASH_SCREEN							0x10000

//*
//**************************************************************************************************
// Function Declarations

void Clear_Icon_Window(void);
void New_Icon_Window(void);

void Update_Icon_Window(UINT8 update_type);
void Update_Audio_Icon(UINT8 update_type);
void Update_Backlight_Icon(UINT8 update_type);
void Update_Battery_Icon(UINT8 update_type);
void Update_No_Attached_Icon(UINT8 update_type);
void Update_PC_Icon(UINT8 update_type);
void Update_Keyboard_Icon(UINT8 update_type);
void Update_Thumbdrive_Icon(UINT8 update_type);
void Update_Range_Icon(UINT8 update_type);
void Update_Correction_Factor_Icon(UINT8 update_type);												// (9DP v1.02.02)

void Color_Battery_Edges(SHORT Xaxis, SHORT Yaxis);													// fill in the white space on the battery bitmap with background color
void Color_Framed_Corners(SHORT Xaxis, SHORT Yaxis);												// fill in the framed bitmap white edges with background color

#endif

//*
//**************************************************************************************************
//**************************************************************************************************
//* LUDLUM MEASUREMENTS INC. COPYRIGHT (C) 2010 
//**************************************************************************************************
//**************************************************************************************************
