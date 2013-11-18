//**************************************************************************************************
//**************************************************************************************************
//*																		
//*	Filename:		LCD-SST39VF1601
//*	Date:			06/04/2009
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
//*	~~~~~~~~	05/27/11	James Comstock
//*	- 
//*
//*	~~~~~~~~	06/04/09	Clint Appling
//*	- Original Prototype
//*                                                                     
//**************************************************************************************************
//*                                                                     
//*	Command Routines
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

volatile DWORD Information;

// Definitions for Flash Chip Reset# - SST39VF1601
#define SST39_RST_TRIS_OUT		PORTSetPinsDigitalOut(IOPORT_F, BIT_3)
#define SST39_RST_LAT_BIT_SET	mPORTFSetBits(BIT_3)
#define SST39_RST_LAT_BIT_CLR	mPORTFClearBits(BIT_3)

// Definitions for Flash Chip Enable - SST39VF1601
#define	SST39_CE_TRIS_OUT		PORTSetPinsDigitalOut(IOPORT_D, BIT_3)
#define SST39_CE_LAT_BIT_SET	mPORTDSetBits(BIT_3)
#define SST39_CE_LAT_BIT_CLR	mPORTDClearBits(BIT_3)

// Definitions for Flash Chip Output Enable - SST39VF1601
#define	SST39_OE_TRIS_OUT		PORTSetPinsDigitalOut(IOPORT_D, BIT_5)
#define SST39_OE_LAT_BIT_SET	mPORTDSetBits(BIT_5)
#define SST39_OE_LAT_BIT_CLR	mPORTDClearBits(BIT_5)

// Definitions for Flash Chip Write Enable - SST39VF1601
#define	SST39_WE_TRIS_OUT		PORTSetPinsDigitalOut(IOPORT_D, BIT_4)
#define SST39_WE_LAT_BIT_SET	mPORTDSetBits(BIT_4)
#define SST39_WE_LAT_BIT_CLR	mPORTDClearBits(BIT_4)

void SST39ChipErase(void);
BYTE SST39BlockErase(DWORD address);
void SST39WaitProgram();

void SST39SetAddress(DWORD address);

BOOL SST39WriteWord(DWORD address, WORD data);
BYTE SST39WriteArray(DWORD address, WORD* pData, WORD nCount);
WORD SST39ReadWord(DWORD address);
WORD SST39ReadFormedWord(DWORD address);
void SST39ReadArray(DWORD address, WORD* pData, WORD nCount);
void SST39ReadWordArray(DWORD address, WORD* pData, WORD nCount);

char * Parse_Data(char *data_string);
void Image_Handler(void);
BYTE Terminate_Data(char *message_string);
BOOL ExtImage(SHORT left, SHORT top, DWORD address);
void ExtImage8BPP(SHORT left, SHORT top);
void ExtImage16BPP(SHORT left, SHORT top);
