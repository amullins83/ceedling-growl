//**************************************************************************************************
//**************************************************************************************************
//*																		
//*	Filename:		I2C.h
//*	Date:			06/04/10
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
//*	~~~~~~~~	08/09/10	James Comstock
//*	- Original Prototype
//*                                                                     
//**************************************************************************************************
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

#ifndef _I2C_H
#define _I2C_H

#define i2cWRITE	0
#define i2cREAD		1

//**************************************************************************************************
//* MCP4728

#define DAC_SingleWrite					0x58
#define DAC_MultiWrite					0x40
#define DAC_Channel_A					0x00
#define DAC_Channel_B					0x02
#define DAC_Channel_C					0x04
#define DAC_Channel_D					0x06

//**************************************************************************************************
//*

void i2c_wait(unsigned int cnt);
void i2c2Write(UINT8 DeviceAddress, UINT16 RegisterAddress, UINT8 RegisterSize, UINT16 RegisterData, UINT8 RegisterDataSize);
void i2c2MultiWrite(UINT8 DeviceAddress, UINT16 RegisterData1, UINT16 RegisterData2, UINT16 RegisterData3, UINT16 RegisterData4);
BYTE i2c2ReadByte(UINT8 DeviceAddress, UINT16 RegisterAddress, UINT8 RegisterSize);
WORD i2c2ReadWord(UINT8 DeviceAddress, UINT16 RegisterAddress, UINT8 RegisterSize);
UINT32_VAL i2c2ReadDWord(UINT8 DeviceAddress);																						// (9DP v1.02.03)
WORD i2c2ReadADC(UINT8 DeviceAddress, UINT8 URegisterAddress, UINT8 LRegisterAddress, UINT8 RegisterData);
void i2c2ClearPage(UINT8 DeviceAddress, UINT16 RegisterAddress, UINT8 RegisterSize, UINT8 PageSize);
void i2c2WritePage(UINT8 DeviceAddress, UINT16 RegisterAddress, UINT8 RegisterSize, BYTE *page, UINT8 PageSize);
BOOL i2c2ReadPage(UINT8 DeviceAddress, UINT16 RegisterAddress, UINT8 RegisterSize, BYTE *page, UINT8 PageSize);

#endif
