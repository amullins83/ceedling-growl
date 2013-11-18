//**************************************************************************************************
//**************************************************************************************************
//*																		
//*	Filename:		Elements.h
//*	Date:			08/23/10
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
//*     n1              09/10/13        Austin Mullins
//*     - Added declarations for Update_Range and GetRateResult.
//*     - Corresponds with update n1 of Elements.c and 01.03.05 of 9DP firmware, project #29307
//*
//*	~~~~~~~~	08/23/10	James Comstock
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

#ifndef _ELEMENTS_INCLUDE_
#define _ELEMENTS_INCLUDE_

#include "GenericTypeDefs.h"

//**************************************************************************************************
//* Defines

#define UNITS_TEXT_HEIGHT						36
#define READING_TEXT_HEIGHT						82
#define Y_SPACER								3
#define Y_ORIGIN 								(ELEMENT_Y + ELEMENT_HEIGHT)
#define Y_UNITS									(Y_ORIGIN - UNITS_TEXT_HEIGHT)
#define Y_READING								(Y_ORIGIN - UNITS_TEXT_HEIGHT - READING_TEXT_HEIGHT)

//**************************************************************************************************
//* Header

void Clear_Header_Window(void);

//**************************************************************************************************
//* Divider

void Clear_Divider_Window(void);
void New_Divider_Window(void);
void Update_Divider(UINT8 update_type);

//**************************************************************************************************
//* Element

void Clear_Element_Window(void);
void New_Element_Window(void);
void Update_Element(UINT8 update_type);

//**************************************************************************************************
//* Element - ARC

void Create_New_Arc(void);
void Draw_Arc_Tick_Marks(unsigned char);
void Setup_Arc_Units(void);
void Draw_Arc_Units(UINT8 update_type);
unsigned short int Update_Arc(void);

#endif

//*
//**************************************************************************************************
//**************************************************************************************************
//* LUDLUM MEASUREMENTS INC. COPYRIGHT (C) 2010 
//**************************************************************************************************
//**************************************************************************************************
