//**************************************************************************************************
//**************************************************************************************************
//*																		
//*	Filename:		Message_Box.h
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
//*	~~~~~~~~	08/18/10	Clint Appling
//*	- Original Prototype
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

#ifndef	_MESSAGEBOX_H
#define _MESSAGEBOX_H

#include "Graphics.h"
#include "instrument.h"
#include "Definitions.h"
#include "System.h"

/*** Upper Left Start Values	***/
#define X_MSG									X_SPACING
#define LINE_SPACE								2
#define LEFT_SHADOW								1
#define RIGHT_SHADOW							6
#define TOP_SHADOW								1
#define BOTTOM_SHADOW							0

#define Y_HEADER_FULL_MSG						(Y_SPACING)																																		
#define Y_ELEMENT_NO_HEADING_MSG				(Y_SPACING+HEADER_HEIGHT)													
#define Y_FUNCTION_MSG							(Y_SPACING+HEADER_HEIGHT+ELEMENT_HEIGHT+DIVIDER_HEIGHT+LINE_SPACE)			
#define Y_ICON_MSG								(Y_SPACING+HEADER_HEIGHT+ELEMENT_HEIGHT+DIVIDER_HEIGHT+FUNCTION_HEIGHT)	
	
#define Y_ELEMENT_MSG_BOTTOM					(Y_ELEMENT_NO_HEADING_MSG+ELEMENT_HEIGHT-LINE_SPACE)											
#define Y_FUNCTION_MSG_BOTTOM					(Y_FUNCTION_MSG+FUNCTION_HEIGHT-LINE_SPACE-2)				
#define Y_ICON_MSG_BOTTOM						(Y_ICON_MSG+ICON_HEIGHT-LINE_SPACE-LINE_SPACE)													

volatile typedef struct _MESSAGE_BOX
{
	SHORT txt_total_lines;
	XCHAR txt_text[10][51];
	XCHAR *font[11];
	SHORT x_line[11];
	SHORT y_line[11];	
	SHORT txt_color_line[11];
	SHORT parent_menu;
	SHORT current_menu;
	SHORT sub_menu[11];
	void* line_item[11];
	BYTE line_item_type[11];
	BYTE current_line;						// DEBUG (9DP v1.01.01)
	signed short minimum_value;
	signed short maximum_value;
	SHORT box_background_color;
	SHORT box_x1;
	SHORT box_y1;
	SHORT box_x2;
	SHORT box_y2;
} TYPE_MESSAGE_BOX;

typedef enum _CUSTOM_MSG_PREF
{
	MSG_ADJ_NO_HEADING = 0,					// starting X,Y defined, but MSG box height depends on msg number of lines
	MSG_FULL_NO_HEADING,					// fixed X,Y locations are defined for full screen (header down to over icons)
	MSG_H_E_F_NO_HEADING,					// covers the Header, Element, Function regions
	MSG_H_E_NO_HEADING,						// covers the Header and Element regions
	MSG_H_NO_HEADING,						// covers the Header region

	MSG_E_F_I_NO_HEADING = 10,				// covers the Element, function and Icon regions.
	MSG_E_F_NO_HEADING,						// covers the Element and Function regions
	MSG_E_NO_HEADING,						// covers the Element region only

	MSG_F_I_NO_HEADING = 20,				// covers the Function and Icon regions
	MSG_F_NO_HEADING,						// covers the Function region only

	MSG_I_NO_HEADING = 30,					// covers the Icon region only

	MSG_ADJ_HEADING = 40,					// starting X,Y defined, but MSG box height depends on msg number of lines
	MSG_FULL_HEADING,						// fixed X,Y locations are defined for full screen (header down to over icons)
	MSG_H_E_F_HEADING,						// covers the Header, Element, Function regions
	MSG_H_E_HEADING,						// covers the Header and Element regions

} CUSTOM_MSG_PREF;

typedef enum _TEXT_ALIGNMENT
{
	LEFT_JUSTIFY = 0,
	RIGHT_JUSTIFY,
	CENTER_JUSTIFY
} TEXT_ALIGNMENT;

#endif //_MESSAGEBOX_H

