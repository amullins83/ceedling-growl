//**************************************************************************************************
//**************************************************************************************************
//*																		
//*	Filename:		Message_Box.c
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

#include "Graphics.h"
#include "instrument.h"
#include "Definitions.h"
#include "System.h"
#include "Message_Box.h"
#include "Icons.h"
#include "Functions.h"
#include "Elements.h"

//**************************************************************************************************
//*	External Variables

extern const FONT_FLASH Arial_Rounded_12pt_32_90;
extern WORD foreground;
extern WORD background;
extern char DisplayMessage[29];
extern BOOL MenuSelect;

//**************************************************************************************************
//*	Define Functions

void DrawBox(UINT16 loc_x1, UINT16 loc_y1, UINT16 loc_x2, UINT16 loc_y2, TYPE_MESSAGE_BOX *parm, UINT8 msg_location, UINT8 msg_condition);
void DrawSmallBox(UINT16 loc_x1, UINT16 loc_y1, UINT16 loc_x2, UINT16 loc_y2);
void DrawLine(UINT16 loc_x1, UINT16 loc_y1, UINT16 loc_x2, UINT16 loc_y2);
void Display_Message(TYPE_MESSAGE_BOX *parm, UINT8 msg_location, UINT8 msg_condition, UINT8 text_alignment);
void Display_Message_Box(TYPE_MESSAGE_BOX *parm, UINT8 msg_location, UINT8 msg_condition);
void Display_Message_Text(TYPE_MESSAGE_BOX *parm, UINT8 msg_location, UINT8 msg_condition, UINT8 text_alignment);

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Display_Message(TYPE_MESSAGE_BOX *parm, UINT8 msg_location, UINT8 msg_condition, UINT8 text_alignment)
//*
//*	Description:
//* 	
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//*		None
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//*
//*		Requires:	- total text lines to be specified for struct "*parm"
//*					- font style to be specified for each text line in struct "*parm"
//*					- color to be specified for each text line in struct "*parm"
//*

void Display_Message(TYPE_MESSAGE_BOX *parm, UINT8 msg_location, UINT8 msg_condition, UINT8 text_alignment)
{
	SHORT textwidth;
	SHORT textheight;
	SHORT TextMaxWidth;
	SHORT TextTotalHeight;
	SHORT BoxHeight;
	SHORT Right_X;
	SHORT Right_X2;
	SHORT Vertical_Y;
	SHORT i;
	SHORT y;
	SHORT lines;
	SHORT index; 
	UINT16 inset, offset, loc_x1, loc_y1, loc_x2, txtspace;

	textwidth	= 0;
	textheight	= 0;
	TextMaxWidth = 0;
	TextTotalHeight = 0;
	inset = 6;
	offset = 3;
	txtspace = 5;

	loc_x1 = X_MSG+1;																				// set start x-location same for all cases
	loc_x2 = (GetMaxX() - X_MSG - inset +2);														// set the finish x-location same for all cases

	if(msg_location < MSG_E_F_I_NO_HEADING)															// check if will start over the Header
	{
		loc_y1 = Y_HEADER_FULL_MSG;																	// set start y-location
	}	
	else if(msg_location < MSG_F_I_NO_HEADING)														// check if will start over Element
	{
		loc_y1 = Y_ELEMENT_NO_HEADING_MSG;															// set start y-location
	}
	else if(msg_location < MSG_I_NO_HEADING)														// check if will start over Function
	{
		loc_y1 = Y_FUNCTION_MSG;																	// set start y-location
	}
	else if(msg_location < MSG_ADJ_HEADING)															// check if will start over the Icons
	{
		loc_y1 = Y_ICON_MSG;																		// set start y-location
	}
	else																							// msg will start with a heading and either be fixed or adj length
	{
		loc_y1 = Y_HEADER_FULL_MSG;																	// set start y-location
	}

	Vertical_Y = loc_y1 + inset + 2;
	Right_X = loc_x1 + inset + txtspace;
	Right_X2 = loc_x2 - inset - txtspace;

	parm->y_line[0] = Vertical_Y;

	for(i=1;i<parm->txt_total_lines+1;i++)															// calculate values based on text line vertical y
	{
		parm->y_line[i] = ((parm->y_line[i-1]) + textheight);
		textheight = GetTextHeight(parm->font[i]);	
		TextTotalHeight = TextTotalHeight + textheight;	
	}
	BoxHeight = TextTotalHeight + loc_y1 + inset + 9;												// increased from 6 to 9 to allow space for letters like gj that go lower on the screen

	for(i=1;i<parm->txt_total_lines+1;i++)															// determine the widest text string needed
	{
		textwidth = (GetTextWidth((XCHAR *)parm->txt_text[i], parm->font[i]));						// get text width of string

		parm->x_line[i] = Right_X;																	// set line x-coordinate position (DEFAULT LEFT JUSTIFY) for text

		if(textwidth > TextMaxWidth)																// verify text width does not exceed maximum screen size
		{
			TextMaxWidth = textwidth;
		}
	}

	switch(msg_location)
	{
		case MSG_ADJ_HEADING:																		// heading - use calc box height from # of lines
		case MSG_ADJ_NO_HEADING:																	// use calc box height from # of lines
			BoxHeight = BoxHeight;																	// use calc box height # based on # of lines
			break;

		case MSG_FULL_HEADING:																		// heading from top of header - icons
		case MSG_FULL_NO_HEADING:																	// from top of header - icons
			Clear_Header_Window();
		case MSG_E_F_I_NO_HEADING:																	// from top of element - icons
			Clear_Element_Window();
			Clear_Divider_Window();
		case MSG_F_I_NO_HEADING:																	// from top of function - icons
			Clear_Function_Window();
		case MSG_I_NO_HEADING:																		// only over icons
			Clear_Icon_Window();
			BoxHeight = Y_ICON_MSG_BOTTOM;															// set bottom Y to bottom of icon region
			break;

		case MSG_H_E_F_HEADING:																		// heading from top of header - function
		case MSG_H_E_F_NO_HEADING:																	// from top of header - function
			Clear_Header_Window();
		case MSG_E_F_NO_HEADING:																	// from top of element - function
			Clear_Element_Window();
			Clear_Divider_Window();
		case MSG_F_NO_HEADING:																		// only over function
			Clear_Function_Window();
			BoxHeight = Y_FUNCTION_MSG_BOTTOM;														// set bottom Y to bottom of function region
			break;

		case MSG_H_E_HEADING:																		// heading from top of header - element
		case MSG_H_E_NO_HEADING:																	// from top of header - element
			Clear_Header_Window();
		case MSG_E_NO_HEADING:																		// only over element
			Clear_Element_Window();
			BoxHeight = Y_ELEMENT_MSG_BOTTOM;														// set bottom Y to bottom of element region
			break;
		default:
			return;
	}


	if(text_alignment == LEFT_JUSTIFY)																// check text for LEFT JUSTIFIED specified
	{
		// Nothing changes since this is the default operation
	}
	else if(text_alignment == RIGHT_JUSTIFY)														// check text for RIGHT JUSTIFIED specified
	{
		for(i=1;i<parm->txt_total_lines+1;i++)														// cycle through struct adjusting each line
		{
			textwidth = (GetTextWidth((XCHAR *)parm->txt_text[i], parm->font[i]));					// get text width of string
			parm->x_line[i] = (Right_X2 - textwidth);												// adjust font position from the right side of LCD 
		}
	}
	else if(text_alignment == CENTER_JUSTIFY)														// check text for CENTER JUSTIFIED specified
	{
		for(i=1;i<parm->txt_total_lines+1;i++)														// cycle through struct adjusting each line
		{
			textwidth = (GetTextWidth((XCHAR *)parm->txt_text[i], parm->font[i]));					// get text width of string
			parm->x_line[i] = (160 - (textwidth / 2));												// adjust font position from the right side of LCD 
		}
	}

	DrawBox(loc_x1, loc_y1, loc_x2, BoxHeight, parm, msg_location, msg_condition);					// 
	
	if(msg_location >= MSG_ADJ_HEADING)																// check if a heading is needed
		index = 0;																					// set to 0 to include the heading
	else
		index = 1;																					// set to 1 since no heading

	for(;index < (parm->txt_total_lines + 1); index++)												// loop through to display each line
	{
		MoveTo(parm->x_line[index], parm->y_line[index]);											// move to starting location
		SetColor(parm->txt_color_line[index]);														// set corresponding line color
		SetFont(parm->font[index]);																	// set corresponding font style
		while(!OutText((XCHAR *)parm->txt_text[index]));											// output text from struct
	}

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Display_Message_Box(TYPE_MESSAGE_BOX *parm, UINT8 msg_location, UINT8 msg_condition)
//*
//*	Description:
//* 	
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//*		None
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//*
//*		Requires:	- total text lines to be specified for struct "*parm"
//*					- font style to be specified for each text line in struct "*parm"
//*					- color to be specified for each text line in struct "*parm"
//*

void Display_Message_Box(TYPE_MESSAGE_BOX *parm, UINT8 msg_location, UINT8 msg_condition)
{
	char StringConversion[28] = {'\0'};
	UINT16 loc_x1;
	UINT16 loc_y1;
	UINT16 loc_x2;
	UINT16 loc_y2;
	UINT16 offset;
	UINT16 inset;
	UINT16 BOX_HEADER_FILL = BLACK;
	UINT16 BOX_SHADOW = BLACK;
	UINT16 BOX_COLOR = DARKGRAY;
	UINT16 BOX_INSET = LIGHTGRAY;
	UINT16 BOX_BACKGROUND = WHITE;
	SHORT templines = 0;
	SHORT i;
	SHORT textwidth;
	SHORT textheight;
	SHORT header_size;
	SHORT fill_size;

	// Define colors
	if(msg_condition == NORMAL_MESSAGE)																// check if standard message
	{
		BOX_INSET = LIGHTGRAY;																		// fill with above assigned color
	}
	else																							// msg_condition must be either and alert, alarm, or system condition
	{
		if(msg_condition < ALARM_RADIATION)	    													// check if value is less than 1st alarm defined (checking if Alert)
		{
			BOX_INSET = BRIGHTYELLOW;																// Fill with BRIGHTYELLOW for Alert
		}
		else if(msg_condition <	WARNING_LOW_BATTERY)												// check if value is less than 1st system condition defined
		{
			BOX_INSET = BRICKRED;																	// Fill with Dark red for Alarm
		}
		else if(msg_condition < ERROR_OUT_OF_MEMORY)												// less than 1st error value, so must be a warning
		{
			BOX_INSET = BRIGHTYELLOW;																// Fill with BRIGHTYELLOW for Warning
		}
		else if(msg_condition < USB_ERROR_MSG) 														// is < 1st USB Error, so must be system error
		{
			BOX_INSET = BRICKRED;																	// Fill with Dark red for Error
		}	
		else if(msg_condition >= USB_ERROR_MSG)														// >= 1st USB Error, so must be USB error
		{
			BOX_INSET = foreground;																	// Fill with BRIGHTRED for USB ERROR
		}
	}
 
	// Define box coordinates
	loc_x1 = X_MSG + LEFT_SHADOW;																	// set start x-location same for all cases
	loc_x2 = GetMaxX() - X_MSG - RIGHT_SHADOW;														// set the finish x-location same for all cases

	if(msg_location < MSG_E_F_I_NO_HEADING)															// check if will start over the Header
	{
		loc_y1 = Y_HEADER_FULL_MSG;																	// set start y-location
	}	
	else if(msg_location < MSG_F_I_NO_HEADING)														// check if will start over Element
	{
		loc_y1 = Y_ELEMENT_NO_HEADING_MSG;															// set start y-location
	}
	else if(msg_location < MSG_I_NO_HEADING)														// check if will start over Function
	{
		loc_y1 = Y_FUNCTION_MSG;																	// set start y-location
	}
	else if(msg_location < MSG_ADJ_HEADING)															// check if will start over the Icons
	{
		loc_y1 = Y_ICON_MSG;																		// set start y-location
	}
	else																							// msg will start with a heading and either be fixed or adj length
	{
		loc_y1 = Y_HEADER_FULL_MSG;																	// set start y-location
	}

	switch(msg_location)
	{
		case MSG_FULL_HEADING:																		// heading from top of header - icons
		case MSG_FULL_NO_HEADING:																	// from top of header - icons
			Clear_Header_Window();
		case MSG_E_F_I_NO_HEADING:																	// from top of element - icons
			Clear_Element_Window();
			Clear_Divider_Window();
		case MSG_F_I_NO_HEADING:																	// from top of function - icons
			Clear_Function_Window();
		case MSG_I_NO_HEADING:																		// only over icons
			Clear_Icon_Window();
			loc_y2 = Y_ICON_MSG_BOTTOM;																// set bottom Y to bottom of icon region
			break;

		case MSG_H_E_F_HEADING:																		// heading from top of header - function
		case MSG_H_E_F_NO_HEADING:																	// from top of header - function
			Clear_Header_Window();
		case MSG_E_F_NO_HEADING:																	// from top of element - function
			Clear_Element_Window();
			Clear_Divider_Window();
		case MSG_F_NO_HEADING:																		// only over function
			Clear_Function_Window();
			loc_y2 = Y_FUNCTION_MSG_BOTTOM;															// set bottom Y to bottom of function region
			break;

		case MSG_H_E_HEADING:																		// heading from top of header - element
		case MSG_H_E_NO_HEADING:																	// from top of header - element
			Clear_Header_Window();
		case MSG_E_NO_HEADING:																		// only over element
			Clear_Element_Window();
			loc_y2 = Y_ELEMENT_MSG_BOTTOM;															// set bottom Y to bottom of element region
			break;

		default:
			return;
	}

	// Draw box
	inset = 6;
	offset = 3;

	// Define coordinates for interior of box
	parm->box_x1 = loc_x1 + inset + 2;
	parm->box_y1 = loc_y1 + inset + 2;
	parm->box_x2 = loc_x2 - inset - 2;
	parm->box_y2 = loc_y2 - inset - 2;

	if((msg_location !=	MSG_I_NO_HEADING) && (msg_location != MSG_F_NO_HEADING) && (msg_location != MSG_H_NO_HEADING))
	{
		// draw shadow
		SetColor(BOX_SHADOW);
		DrawSmallBox(loc_x1 + offset, loc_y1 + offset, loc_x2 + offset, loc_y2 + offset);

		// draw exterior of box
		SetColor(BOX_COLOR);
		DrawSmallBox(loc_x1, loc_y1, loc_x2, loc_y2);

		// draw interior of box
		DrawSmallBox(loc_x1 + inset, loc_y1 + inset, loc_x2 - inset, loc_y2 - inset);

		// fill inset between interior and exterior boxes
		SetColor(BOX_INSET);
		inset = inset / 2;
		DrawSmallBox(loc_x1 + inset, loc_y1 + inset, loc_x2 - inset, loc_y2 - inset);
	}

	// Fill interior of box
	SetColor(BOX_BACKGROUND);
	Bar(parm->box_x1, parm->box_y1, parm->box_x2, parm->box_y2);

	// Draw heading
	if(msg_location >= MSG_ADJ_HEADING)																// check if a heading is needed
	{
//		parm->font[0] = (void*)&Arial_Rounded_12pt_32_90;
		parm->font[0] = (void*)&GOLFontDefault;														//
		if(msg_condition == 0)																		// check if standard message
		{
			Convert_For_Display(parm->txt_text[0], NULL, DisplayMessage, 28);						// convert header text to XCHAR
			BOX_HEADER_FILL = GREEN;																// set header fill color
			parm->txt_color_line[0] = WHITE;														// set header text color
		}
		else																						// msg_condition must be either and alert, alarm, or system condition
		{
			if(msg_condition < ALARM_RADIATION)			   											// check if value is less than 1st alarm defined (checking if Alert)
			{
				sprintf(StringConversion, "ALERT");													// format header text
				Convert_For_Display(parm->txt_text[0], NULL, StringConversion, 28);					// convert header text to XCHAR
				BOX_HEADER_FILL = BRIGHTYELLOW;														// set header fill color
				parm->txt_color_line[0] = BLACK;													// set header text color
			}
			else if(msg_condition <	WARNING_LOW_BATTERY)											// check if value is less than 1st system condition defined
			{
				sprintf(StringConversion, "ALARM");													// format header text
				Convert_For_Display(parm->txt_text[0], NULL, StringConversion, 28);					// convert header text to XCHAR
				BOX_HEADER_FILL = BRICKRED;															// set header fill color
				parm->txt_color_line[0] = WHITE;													// set header text color
			}
			else if(msg_condition < ERROR_OUT_OF_MEMORY)											// < 1st error value, so must be a Warning
			{
				sprintf(StringConversion, "WARNING");												// format header text
				Convert_For_Display(parm->txt_text[0], NULL, StringConversion, 28);					// convert header text to XCHAR
				BOX_HEADER_FILL = BRIGHTYELLOW;														// set header fill color
				parm->txt_color_line[0] = BLACK;													// set header text color
			}
			else if(msg_condition < USB_ERROR_MSG)													// < 1st USB error value so must be an Error
			{
				sprintf(StringConversion, "ERROR");													// format header text
				Convert_For_Display(parm->txt_text[0], NULL, StringConversion, 28);					// convert header text to XCHAR
				BOX_HEADER_FILL = BRICKRED;															// set header fill color
				parm->txt_color_line[0] = WHITE;													// set header text color
			}
			else if(msg_condition >= USB_ERROR_MSG)													// >= 1st USB Error value so must be USB error
			{
				sprintf(StringConversion, "USB ERROR");												// format header text
				Convert_For_Display(parm->txt_text[0], NULL, StringConversion, 28);					// convert header text to XCHAR
				BOX_HEADER_FILL = BLACK;															// set header fill color
				parm->txt_color_line[0] = BRIGHTRED;												// set header text color
			}
		}

		textwidth = (GetTextWidth((XCHAR *)parm->txt_text[0], parm->font[0]));						// get header text width
		textheight = GetTextHeight(parm->font[0]);													// get header text font height
		header_size = (textheight + inset + 6);														// determine the height of the header region to fill in a new color

		parm->x_line[0] = (160 - (textwidth / 2));													// set header line X value so the text is centered on the screen
		parm->y_line[0] = parm->y_line[1];															// set the header line Y value to the first line coordinates from line 1 - shift line 1 down

		if(loc_y2 == Y_ICON_MSG_BOTTOM)																// msg box extends to bottom of icon region
		{
			textheight = (GetTextHeight(parm->font[0]) * 2);										// get header text font height	
			
			for(i=1; i<parm->txt_total_lines+1; i++)												// cycle through struct shifting the line y coordinates to make room for the Header
			{
				parm->y_line[i] = (textheight + parm->y_line[i-1]);									// shift text y_coordinate for each line to make room for the header
				textheight = GetTextHeight(parm->font[i]);
			}
		}
		else if(loc_y2 == Y_FUNCTION_MSG_BOTTOM)													// msg box extends to bottom of function region
		{
			textheight = (GetTextHeight(parm->font[0]) * 2);										// get header text font height

			for(i=1; i<parm->txt_total_lines+1; i++)												// cycle through struct shifting the line y coordinates to make room for the Header
			{
				parm->y_line[i] = (textheight + parm->y_line[i-1]);									// shift text y_coordinate for each line to make room for the header
				textheight = GetTextHeight(parm->font[i]);
			}
		}
		else if(loc_y2 == Y_ELEMENT_MSG_BOTTOM)														// msg box extends to bottom of element region
		{		
			textheight = (GetTextHeight(parm->font[0]) * 2);										// get header text font height

			for(i=1; i<parm->txt_total_lines+1; i++)												// cycle through struct shifting the line y coordinates to make room for the Header
			{
				parm->y_line[i] = (textheight + parm->y_line[i-1]);									// shift text y_coordinate for each line to make room for the header
				textheight = GetTextHeight(parm->font[i]);
			}
		}

		SetColor(BOX_HEADER_FILL);																	// set fill color
		Bar(parm->box_x1, parm->box_y1, parm->box_x2, header_size);									// fill header section
		SetColor(BOX_COLOR);
		DrawLine(((UINT16)parm->box_x1 - 3),(UINT16)header_size,((UINT16)parm->box_x2 + 3),(UINT16)header_size);
		SetColor(BOX_INSET);
		DrawLine(((UINT16)parm->box_x1 - 3),(UINT16)(header_size + 3),((UINT16)parm->box_x2 + 3),(UINT16)(header_size + 3));
		SetColor(BOX_COLOR);
		DrawLine(((UINT16)parm->box_x1 - 3),(UINT16)(header_size + 6),((UINT16)parm->box_x2 + 3),(UINT16)(header_size + 6));
	}
	else if(msg_location ==	MSG_I_NO_HEADING)														// check if message is for the icon region
	{
		SetColor(RED);					
		Bar(X_MSG, loc_y1+inset, 319-X_MSG, loc_y2 + 3);											// Add 3 so the bar extends into the msg box shadow region
		parm->txt_color_line[1] = background;
		parm->y_line[1] = loc_y1+inset;
	}
	else if(msg_location == MSG_H_NO_HEADING)
	{
		SetColor(BRIGHTGREEN);
		Bar(X_MSG, loc_y1, 319-X_MSG, loc_y2 + 3);													// Add 3 so the bar extends into the msg box shadow region	
		parm->txt_color_line[1] = background;
		parm->y_line[1] = loc_y1;
	}
	else if(msg_location == MSG_F_NO_HEADING)
	{
		SetColor(BRIGHTGREEN);
		Bar(X_MSG, loc_y1, 319-X_MSG, loc_y2 + 3);													// Add 3 so the bar extends into the msg box shadow region
		parm->txt_color_line[1] = background;
		textheight = GetTextHeight(parm->font[1]);
		parm->y_line[1] = loc_y1;	
	}
	
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Display_Message_Text(TYPE_MESSAGE_BOX *parm, UINT8 msg_location, UINT8 msg_condition, UINT8 text_alignment)
//*
//*	Description:
//* 	
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//*		None
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//*
//*		Requires:	- total text lines to be specified for struct "*parm"
//*					- font style to be specified for each text line in struct "*parm"
//*					- color to be specified for each text line in struct "*parm"
//*

void Display_Message_Text(TYPE_MESSAGE_BOX *parm, UINT8 msg_location, UINT8 msg_condition, UINT8 text_alignment)
{
	SHORT textwidth;
	SHORT textheight;
	SHORT TextMaxWidth;
	SHORT TextTotalHeight;
	SHORT i;
	SHORT y;
	SHORT lines;
	SHORT index; 

	textwidth	= 0;
	TextMaxWidth = 0;
	TextTotalHeight = 0;
	textheight = 30;

	parm->y_line[0] = parm->box_y1;

	for(i = 1; i < parm->txt_total_lines + 1; i++)													// calculate values based on text line vertical y
	{
		parm->y_line[i] = ((parm->y_line[i-1]) + textheight);
		textheight = GetTextHeight(parm->font[i]);	
		TextTotalHeight = TextTotalHeight + textheight;	
	}

	for(i = 1; i < parm->txt_total_lines + 1; i++)													// determine the widest text string needed
	{
		textwidth = (GetTextWidth((XCHAR *)parm->txt_text[i], parm->font[i]));						// get text width of string
		parm->x_line[i] = parm->box_x1;																// set line x-coordinate position (DEFAULT LEFT JUSTIFY) for text
		if(textwidth > TextMaxWidth)																// verify text width does not exceed maximum screen size
		{
			TextMaxWidth = textwidth;
		}
	}

	if(text_alignment == LEFT_JUSTIFY)																// check text for LEFT JUSTIFIED specified
	{
		// Nothing changes since this is the default operation
	}
	else if(text_alignment == RIGHT_JUSTIFY)														// check text for RIGHT JUSTIFIED specified
	{
		for(i = 1; i < parm->txt_total_lines + 1; i++)												// cycle through struct adjusting each line
		{
			textwidth = (GetTextWidth((XCHAR *)parm->txt_text[i], parm->font[i]));					// get text width of string
			parm->x_line[i] = (parm->box_x2 - textwidth);											// adjust font position from the right side of LCD 
		}
	}
	else if(text_alignment == CENTER_JUSTIFY)														// check text for CENTER JUSTIFIED specified
	{
		for(i = 1; i < parm->txt_total_lines + 1; i++)												// cycle through struct adjusting each line
		{
			textwidth = (GetTextWidth((XCHAR *)parm->txt_text[i], parm->font[i]));					// get text width of string
			parm->x_line[i] = (160 - (textwidth / 2));												// adjust font position from the right side of LCD 
		}
	}

	if(msg_location >= MSG_ADJ_HEADING)																// check if a heading is needed
		index = 0;																					// set to 0 to include the heading
	else
		index = 1;																					// set to 1 since no heading

	for(;index < (parm->txt_total_lines + 1); index++)												// loop through to display each line
	{
		MoveTo(parm->x_line[index], parm->y_line[index]);											// move to starting location
		SetColor(parm->txt_color_line[index]);														// set corresponding line color
		SetFont(parm->font[index]);																	// set corresponding font style
		while(!OutText((XCHAR *)parm->txt_text[index]));											// output text from struct
	}

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void DrawBox(UINT16 loc_x1, UINT16 loc_y1, UINT16 loc_x2, UINT16 loc_y2, TYPE_MESSAGE_BOX *parm, UINT8 msg_location, UINT8 msg_condition)
//*
//*	Description:
//* 	
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//*		None
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

void DrawBox(UINT16 loc_x1, UINT16 loc_y1, UINT16 loc_x2, UINT16 loc_y2, TYPE_MESSAGE_BOX *parm, UINT8 msg_location, UINT8 msg_condition)
{
	UINT16 BOX_HEADER_FILL = BLACK;
	UINT16 BOX_SHADOW = BLACK;
	UINT16 BOX_COLOR = DARKGRAY;
	UINT16 BOX_INSET = LIGHTGRAY;
	UINT16 BOX_BACKGROUND = WHITE;
	UINT16 BOX_X1 = 0;
	UINT16 BOX_Y1 = 0;
	UINT16 BOX_X2 = 0;
	UINT16 BOX_Y2 = 0;	
	SHORT templines = 0;
	SHORT i, textwidth, textheight, header_size, fill_size;

	UINT16 offset;
	UINT16 inset;

	char StringConversion[28] = {'\0'};
	offset = 3;
	inset = 6;

	BOX_X1 = loc_x1 + inset + 2;
	BOX_Y1 = loc_y1 + inset + 2;
	BOX_X2 = loc_x2 - inset - 2;
	BOX_Y2 = loc_y2 - inset - 2;

	if(msg_condition == NORMAL_MESSAGE)																// check if standard message
	{
		BOX_INSET = LIGHTGRAY;																		// fill with above assigned color
	}
	else																							// msg_condition must be either and alert, alarm, or system condition
	{
		if(msg_condition < ALARM_RADIATION)	    													// check if value is less than 1st alarm defined (checking if Alert)
		{
			BOX_INSET = BRIGHTYELLOW;																// Fill with BRIGHTYELLOW for Alert
		}
		else if(msg_condition <	WARNING_LOW_BATTERY)												// check if value is less than 1st system condition defined
		{
			BOX_INSET = BRICKRED;																	// Fill with Dark red for Alarm
		}
		else if(msg_condition < ERROR_OUT_OF_MEMORY)												// less than 1st error value, so must be a warning
		{
			BOX_INSET = BRIGHTYELLOW;																// Fill with BRIGHTYELLOW for Warning
		}
		else if(msg_condition < USB_ERROR_MSG) 														// is < 1st USB Error, so must be system error
		{
			BOX_INSET = BRICKRED;																	// Fill with Dark red for Error
		}	
		else if(msg_condition >= USB_ERROR_MSG)														// >= 1st USB Error, so must be USB error
		{
			BOX_INSET = foreground;																	// Fill with BRIGHTRED for USB ERROR
		}
	}
 
	if((msg_location !=	MSG_I_NO_HEADING) && (msg_location != MSG_F_NO_HEADING) && (msg_location != MSG_H_NO_HEADING))
	{
		// draw shadow
		SetColor(BOX_SHADOW);
		DrawSmallBox(loc_x1 + offset, loc_y1 + offset, loc_x2 + offset, loc_y2 + offset);

		// draw exterior box and interior box
		SetColor(BOX_COLOR);
		DrawSmallBox(loc_x1, loc_y1, loc_x2, loc_y2);
		DrawSmallBox(loc_x1 + inset, loc_y1 + inset, loc_x2 - inset, loc_y2 - inset);

		// fill inset
		inset = inset / 2;																			// divide the inset by 2
		SetColor(BOX_INSET);
		DrawSmallBox(loc_x1 + inset, loc_y1 + inset, loc_x2 - inset, loc_y2 - inset);
	
		// fill box
		SetColor(BOX_BACKGROUND);
		Bar(BOX_X1, BOX_Y1, BOX_X2, BOX_Y2);
	}

	if(msg_location >= MSG_ADJ_HEADING)																// check if a heading is needed
	{
		parm->font[0] = (void*)&Arial_Rounded_12pt_32_90;

		if(msg_condition == 0)																		// check if standard message
		{
			Convert_For_Display(parm->txt_text[0], NULL, DisplayMessage, 28);						// convert header text to XCHAR
			BOX_HEADER_FILL = GREEN;																// set header fill color
			parm->txt_color_line[0] = WHITE;														// set header text color
		}
		else																						// msg_condition must be either and alert, alarm, or system condition
		{
			if(msg_condition < ALARM_RADIATION)	// 50   											// check if value is less than 1st alarm defined (checking if Alert)
			{
				sprintf(StringConversion, "ALERT");													// format header text
				Convert_For_Display(parm->txt_text[0], NULL, StringConversion, 28);					// convert header text to XCHAR
				BOX_HEADER_FILL = BRIGHTYELLOW;														// set header fill color
				parm->txt_color_line[0] = BLACK;													// set header text color
			}
			else if(msg_condition <	WARNING_LOW_BATTERY)	// 100									// check if value is less than 1st system condition defined
			{
				sprintf(StringConversion, "ALARM");													// format header text
				Convert_For_Display(parm->txt_text[0], NULL, StringConversion, 28);					// convert header text to XCHAR
				BOX_HEADER_FILL = BRICKRED;															// set header fill color
				parm->txt_color_line[0] = WHITE;													// set header text color
			}
			else if(msg_condition < ERROR_OUT_OF_MEMORY)											// < 1st error value, so must be a Warning
			{
				sprintf(StringConversion, "WARNING");												// format header text
				Convert_For_Display(parm->txt_text[0], NULL, StringConversion, 28);					// convert header text to XCHAR
				BOX_HEADER_FILL = BRIGHTYELLOW;														// set header fill color
				parm->txt_color_line[0] = BLACK;													// set header text color
			}
			else if(msg_condition < USB_ERROR_MSG)													// < 1st USB error value so must be an Error
			{
				sprintf(StringConversion, "ERROR");													// format header text
				Convert_For_Display(parm->txt_text[0], NULL, StringConversion, 28);					// convert header text to XCHAR
				BOX_HEADER_FILL = BRICKRED;															// set header fill color
				parm->txt_color_line[0] = WHITE;													// set header text color
			}
			else if(msg_condition >= USB_ERROR_MSG)													// >= 1st USB Error value so must be USB error
			{
				sprintf(StringConversion, "USB ERROR");												// format header text
				Convert_For_Display(parm->txt_text[0], NULL, StringConversion, 28);					// convert header text to XCHAR
				BOX_HEADER_FILL = BLACK;															// set header fill color
				parm->txt_color_line[0] = BRIGHTRED;												// set header text color
			}
		}

		textwidth = (GetTextWidth((XCHAR *)parm->txt_text[0], parm->font[0]));						// get header text width
		textheight = GetTextHeight(parm->font[0]);													// get header text font height
		header_size = (textheight + inset + 6);														// determine the height of the header region to fill in a new color

		parm->x_line[0] = (160 - (textwidth / 2));													// set header line X value so the text is centered on the screen
		parm->y_line[0] = parm->y_line[1];															// set the header line Y value to the first line coordinates from line 1 - shift line 1 down

		if(loc_y2 == Y_ICON_MSG_BOTTOM)																// msg box extends to bottom of icon region
		{
			textheight = (GetTextHeight(parm->font[0]) * 2);										// get header text font height	
			
			for(i=1; i<parm->txt_total_lines+1; i++)												// cycle through struct shifting the line y coordinates to make room for the Header
			{
				parm->y_line[i] = (textheight + parm->y_line[i-1]);									// shift text y_coordinate for each line to make room for the header
				textheight = GetTextHeight(parm->font[i]);
			}
		}
		else if(loc_y2 == Y_FUNCTION_MSG_BOTTOM)													// msg box extends to bottom of function region
		{
			textheight = (GetTextHeight(parm->font[0]) * 2);										// get header text font height

			for(i=1; i<parm->txt_total_lines+1; i++)												// cycle through struct shifting the line y coordinates to make room for the Header
			{
				parm->y_line[i] = (textheight + parm->y_line[i-1]);									// shift text y_coordinate for each line to make room for the header
				textheight = GetTextHeight(parm->font[i]);
			}
		}
		else if(loc_y2 == Y_ELEMENT_MSG_BOTTOM)														// msg box extends to bottom of element region
		{		
			textheight = (GetTextHeight(parm->font[0]) * 2);										// get header text font height

			for(i=1; i<parm->txt_total_lines+1; i++)												// cycle through struct shifting the line y coordinates to make room for the Header
			{
				parm->y_line[i] = (textheight + parm->y_line[i-1]);									// shift text y_coordinate for each line to make room for the header
				textheight = GetTextHeight(parm->font[i]);
			}
		}

		SetColor(BOX_HEADER_FILL);																	// set fill color
		Bar(BOX_X1, BOX_Y1, BOX_X2, header_size);		/// was  header_height						// fill header section
		SetColor(BOX_COLOR);
		DrawLine((BOX_X1 - 3),header_size,(BOX_X2 + 3),header_size);
		SetColor(BOX_INSET);
		DrawLine((BOX_X1 - 3),(header_size + 3),(BOX_X2 + 3),(header_size + 3));
		SetColor(BOX_COLOR);
		DrawLine((BOX_X1 - 3),(header_size + 6),(BOX_X2 + 3),(header_size + 6));
	}
	else if(msg_location ==	MSG_I_NO_HEADING)														// check if message is for the icon region
	{
		SetColor(RED);					
		Bar(X_MSG, loc_y1+inset, 319-X_MSG, loc_y2 + 3);	// was (loc_y1 + inset <<<- since micro tail)	// Add 3 so the bar extends into the msg box shadow region
		parm->txt_color_line[1] = background;
		parm->y_line[1] = loc_y1+inset;
	}
	else if(msg_location == MSG_H_NO_HEADING)
	{
		SetColor(BRIGHTGREEN);
		Bar(X_MSG, loc_y1, 319-X_MSG, loc_y2 + 3);													// Add 3 so the bar extends into the msg box shadow region	
		parm->txt_color_line[1] = background;
		parm->y_line[1] = loc_y1;
	}
	else if(msg_location == MSG_F_NO_HEADING)
	{
		SetColor(BRIGHTGREEN);
		Bar(X_MSG, loc_y1, 319-X_MSG, loc_y2 + 3);													// Add 3 so the bar extends into the msg box shadow region

		parm->txt_color_line[1] = background;
		textheight = GetTextHeight(parm->font[1]);
		parm->y_line[1] = loc_y1;	
	}
	
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	
//*
//*	Description:
//* 	
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//*		None
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

void DrawSmallBox(UINT16 loc_x1, UINT16 loc_y1, UINT16 loc_x2, UINT16 loc_y2)
{
	DrawLine(loc_x1, loc_y1, loc_x2, loc_y1);
	DrawLine(loc_x1, loc_y1, loc_x1, loc_y2);
	DrawLine(loc_x1, loc_y2, loc_x2, loc_y2);
	DrawLine(loc_x2, loc_y1, loc_x2, loc_y2);
	PutPixel(loc_x1 - 1, loc_y1 - 1);
	PutPixel(loc_x2 + 1, loc_y1 - 1);
	PutPixel(loc_x1 - 1, loc_y2 + 1);
	PutPixel(loc_x2 + 1, loc_y2 + 1);

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	
//*
//*	Description:
//* 	
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//*		None
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

void DrawLine(UINT16 loc_x1, UINT16 loc_y1, UINT16 loc_x2, UINT16 loc_y2)
{
	unsigned int x, y;

	if(loc_x1 == loc_x2)
	{
		for(y = loc_y1; y <= loc_y2; y++)
		{
			PutPixel(loc_x1 - 1, y);
			PutPixel(loc_x1, y);
			PutPixel(loc_x1 + 1, y);
		}
	}
	else if(loc_y1 == loc_y2)
	{
		for(x = loc_x1; x <= loc_x2; x++)
		{
			PutPixel(x, loc_y1 - 1);
			PutPixel(x, loc_y1);
			PutPixel(x, loc_y1 + 1);
		}
	}
	
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//* LUDLUM MEASUREMENTS INC. COPYRIGHT (C) 2010 
//**************************************************************************************************
//**************************************************************************************************

