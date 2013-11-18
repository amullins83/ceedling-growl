//**************************************************************************************************
//**************************************************************************************************
//*																		
//*	Filename:		Elements.c
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
//* n4          09/18/13    Austin Mullins
//*  - Debugged Update_Arc / Update_Range to fix problem where the input maxes out constantly.
//*
//*	n1      	09/10/13	Austin Mullins
//*	 Update_Arc / Update_Range / GetRateResult
//*  - Extracted Update_Range from Update_Arc to facilitate range changes outside visual update
//*    cycle.
//*  - Created GetRateResult function to facilitate intermediate calculations needed for both
//*    Update_Arc and Update_Range
//*  - Changes correspond with 9DP software v01.03.05 in project 29307.
//*
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

#include "instrument.h"																					// include for the data structures
#include "Definitions.h"																			// include for the data structures
#include "Elements.h"																				// include for the obvious reason
#include "Conversions.h"																			// include for data conversions
#include "GenericTypeDefs.h"																		// include for the type definitions
#include "Screens.h"																				// include for arc colors
#include "electrometer.h"

//**************************************************************************************************
//**************************************************************************************************
//*	DEFINES
//**************************************************************************************************
//**************************************************************************************************

#define INITIAL_ANGLE							20.0
#define MIN_ARC_DIVISIONS						0
#define MAX_ARC_DIVISIONS 						280

//**************************************************************************************************
//**************************************************************************************************
//*	EXTERNAL MEMORY DECLARATIONS
//**************************************************************************************************
//**************************************************************************************************
//*	External - Instrument

extern TYPE_INSTRUMENT lmi;

//**************************************************************************************************
//*	External - Fonts

extern const FONT_FLASH Symbol_SansSerif_12;
extern const FONT_FLASH Normal_Arial_Black_36;
extern const FONT_FLASH Normal_Arial_Black_44;
extern const FONT_FLASH Secondary_Arial_Black_14;
extern const FONT_FLASH GOLFontDefault;
extern const FONT_FLASH Arial_Rounded_12pt_32_90;

//**************************************************************************************************
//*	External - Display

extern WORD foreground;
extern WORD background;
extern BYTE Current_Detector;
extern BYTE Current_View;
extern BYTE ExposureMultiplier;
extern BYTE ExposureUnits;
extern BYTE ExposureRange;
extern BYTE Default_Rate_Units;
extern BYTE Default_Dose_Units;

//**************************************************************************************************
//*	External - Readings

extern float Rate_uR_hr;
extern float Rate_xR_hr;
extern float Rate_Divide_By;
extern XCHAR ExposureValue[7];

//**************************************************************************************************
//*	External - Alarms

extern BYTE New_Alarm;
extern unsigned char Reset_Level;
extern float Radiation_Alert_Rate;
extern float Radiation_Alarm_Rate;

//**************************************************************************************************
//*	External - Units

extern const XCHAR multipliers[16];
extern const XCHAR units_R_hr[];
extern const XCHAR units_Sv_hr[];
extern const XCHAR units_Gy_hr[];
//extern SHORT Default_Rate_Multiplier;

//**************************************************************************************************
//*	External - Range Information

extern char Range_Number;
extern UINT16 Full_Scale_Arc_Range[7][2];

//**************************************************************************************************
//*	External - Arc Information (should become local)

extern const short x_orgin;
extern const short y_orgin;
extern const short edge_spacing;

//#define UNITS_TEXT_HEIGHT						36
//#define READING_TEXT_HEIGHT					82
//#define Y_SPACER								3
//#define Y_ORIGIN 								(ELEMENT_Y + ELEMENT_HEIGHT)
//#define Y_UNITS								(Y_ORIGIN - UNITS_TEXT_HEIGHT)
//#define Y_READING								(Y_ORIGIN - UNITS_TEXT_HEIGHT - READING_TEXT_HEIGHT)

//**************************************************************************************************
//*	External - Flags

extern BOOL Update_Arc_Flag;
extern BOOL Reset_Pressed;
extern BOOL Update_Display_Rate_Flag;
extern BOOL DisplayDetachOnce;
extern BOOL Range_Lock;
extern BOOL Range_Lock_Valid;

//extern BOOL Radiation_Alert_Flag;																	// (v1.01.11)
//extern BOOL Radiation_Alert_Ack_Flag;																// (v1.01.11)
//extern BOOL Radiation_Alarm_Flag;																	// (v1.01.11)
//extern BOOL Radiation_Alarm_Ack_Flag;																// (v1.01.11)

//**************************************************************************************************
//**************************************************************************************************
//*	LOCAL MEMORY DECLARATIONS
//**************************************************************************************************
//**************************************************************************************************

float arc_magnitude 							= 0.0;												// variable used to hold the radius value when drawing the arc and tick marks
float const draw_magnitude 						= 10.0;												// variable used with calculations when drawing the arc and tick marks
float const outline_magnitude 					= 2.0;												// variable used with calculations when drawing the arc and tick marks

float pos_xx[MAX_ARC_DIVISIONS + 1];
float pos_yy[MAX_ARC_DIVISIONS + 1];
float cursor_x[MAX_ARC_DIVISIONS + 1];
float cursor_y[MAX_ARC_DIVISIONS + 1];

const XCHAR units_CPM[] 						= {'C','P','M',0};
const XCHAR units_R_hr[] 						= {'R','/','h','r',0};
//const XCHAR units_Sv_hr[] 						= {'S','v','/','h','r',0};
//const XCHAR units_Gy_hr[] 						= {'G','y','/','h','r',0};
const XCHAR units_Sv_hr[] 						= {'S','v','/','h',0};
const XCHAR units_Gy_hr[] 						= {'G','y','/','h',0};

//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Clear_Divider_Window(void) 	
//*
//*	Description:
//* 	Update the current function as required.
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	None
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

void Clear_Header_Window(void)
{
	SetColor(background);
	Bar(0, HEADER_Y, 319, (HEADER_Y + HEADER_HEIGHT));
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void New_Divider_Window(void) 	
//*
//*	Description:
//* 	Update the current function as required.
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	None
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

void New_Divider_Window(void)
{
	Clear_Divider_Window();
	Update_Divider(NEW_WINDOW);
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Clear_Divider_Window(void) 	
//*
//*	Description:
//* 	Update the current function as required.
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	None
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

void Clear_Divider_Window(void)
{
	SetColor(background);
	Bar(0, DIVIDER_Y, 319, (DIVIDER_Y + DIVIDER_HEIGHT));
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Update_Divider(UINT8 update_type) 	
//*
//*	Description:
//* 	Update the current divider as required,
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	None
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

void Update_Divider(UINT8 update_type)
{
	SetLineType(SOLID_LINE);
	SetLineThickness(NORMAL_LINE);
	SetColor(LIGHTGRAY);																			// was DARKGRAY Set color for line divider
//	Line(22, DIVIDER_Y + 2, 297, DIVIDER_Y + 2);													// Draw color sections
	Line((SHORT) 22, (SHORT) (DIVIDER_Y + 2), (SHORT) 297, (SHORT) (DIVIDER_Y + 2));				// Draw color sections

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void New_Element_Window(void) 	
//*
//*	Description:
//* 	Update the current function as required.
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	None
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

void New_Element_Window(void)
{
	Clear_Element_Window();
	Update_Element(NEW_WINDOW);
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Clear_Element_Window(void) 	
//*
//*	Description:
//* 	Update the current function as required.
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	None
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

void Clear_Element_Window(void)
{
	SetColor(background);
	Bar(0, ELEMENT_Y, 319, (ELEMENT_Y + ELEMENT_HEIGHT));
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Update_Element(UINT8 update_type) 
//*
//*	Description:
//* 	
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	update_type:
//* 		CLEAR_WINDOW
//*			RESET_WINDOW
//*			UPDATE_WINDOW
//*			NEW_WINDOW
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

void Update_Element(UINT8 update_type)
{
	switch(lmi.display.detector[Current_Detector].view[Current_View].element_header.info.style)
	{
		case NO_ELEMENT:
		{
			Clear_Element_Window();
			break;
		}
		case ARC:
		{
			switch(update_type)
			{
				case CLEAR_WINDOW:
				{
					Clear_Element_Window();
					break;
				}
				case RESET_WINDOW:
				{
					INTEnable(INT_T1, 0);															// disable timer 1 interrupt (9DP v1.01.02)
					Reset_Level = TRUE;																// force audio and reading reset
//					AdjustReading();																// renamed (9DP v1.02.02)
					ReadingCalculation();															// renamed (9DP v1.02.02)
					Update_Arc_Flag = TRUE;															// force arc update
					Update_Display_Rate_Flag = TRUE;												// force rate update
					INTEnable(INT_T1, 1);															// enable timer 1 interrupt (9DP v1.01.02)
				}
				case UPDATE_WINDOW:
				{
					Update_Arc();																	// update arc
					break;
				}
				case NEW_WINDOW:
				{
					Create_New_Arc();																// draw arc outline and tickmarks
					break;
				}
			}
			break;
		}
		default:
		{
			break;
		}
	}

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Create_New_Arc(void) 
//*
//*	Description:
//* 	
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	None
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

void Create_New_Arc(void) 
{
	short step;
	float draw_x, draw_y;
	float move_x, move_y;
	float draw_angle, draw_increment;
	float inner_arc_magnitude, outer_arc_magnitude;
	float angle_increment;

	angle_increment = (180.0 - (2.0 * INITIAL_ANGLE)) / (float) MAX_ARC_DIVISIONS;

	arc_magnitude = (x_orgin - outline_magnitude - draw_magnitude - edge_spacing);					// -2

	draw_increment = 0.25;
	inner_arc_magnitude = (arc_magnitude - outline_magnitude -1);									// add spacing for fill color to not overlap
	outer_arc_magnitude = (arc_magnitude + draw_magnitude +1);										// add spacing for fill color to not overlap

	SetLineType(SOLID_LINE);
	SetLineThickness(THICK_LINE);

	SetColor(background);																			// for background
	ClearDevice();		
	SetColor(foreground);																			// for arc outline
		
	draw_angle = INITIAL_ANGLE;

	while(draw_angle < (180.0 - INITIAL_ANGLE))
	{
		move_x = x_orgin - (inner_arc_magnitude * (cos(draw_angle*3.14159/180)));
		move_y = (float) Y_ORIGIN - (inner_arc_magnitude * (sin(draw_angle*3.14159/180)));

		draw_x = move_x - (outline_magnitude * (cos(draw_angle*3.14159/180)));
		draw_y = move_y - (outline_magnitude * (sin(draw_angle*3.14159/180)));

//		Line(move_x, move_y, draw_x, draw_y);														// Draw color sections
		Line((SHORT) move_x, (SHORT) move_y, (SHORT) draw_x, (SHORT) draw_y);						// Draw color sections (9DP v1.01.16)
		draw_angle += draw_increment;
	}

	draw_angle = INITIAL_ANGLE;

	while(draw_angle < (180.0 - INITIAL_ANGLE))
	{
		move_x = x_orgin - (outer_arc_magnitude * (cos(draw_angle*3.14159/180)));
		move_y = (float) Y_ORIGIN - (outer_arc_magnitude * (sin(draw_angle*3.14159/180)));

		draw_x = move_x - (outline_magnitude * (cos(draw_angle*3.14159/180)));
		draw_y = move_y - (outline_magnitude * (sin(draw_angle*3.14159/180)));

//		Line(move_x, move_y, draw_x, draw_y);														// Draw color sections
		Line((SHORT) move_x, (SHORT) move_y, (SHORT) draw_x, (SHORT) draw_y);						// Draw color sections (9DP v1.01.16)
		draw_angle += draw_increment;
	}

	draw_angle = INITIAL_ANGLE;

	for(step = 0; step < (unsigned short) MAX_ARC_DIVISIONS; step++)
	{
		cursor_x[step] = x_orgin - (arc_magnitude * (cos(draw_angle*3.14159/180)));
		cursor_y[step] = (float) Y_ORIGIN - (arc_magnitude * (sin(draw_angle*3.14159/180)));

		pos_xx[step] = cursor_x[step] - (draw_magnitude * (cos(draw_angle*3.14159/180)));
		pos_yy[step] = cursor_y[step] - (draw_magnitude * (sin(draw_angle*3.14159/180)));
		
		draw_angle += angle_increment;
	}

	Draw_Arc_Tick_Marks((BYTE)lmi.display.detector[Current_Detector].view[Current_View].element.info.arc.control.scale.number_of_divisions);
	Setup_Arc_Units();
	Draw_Arc_Units(NEW_WINDOW);
	New_Divider_Window();
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Draw_Arc_Tick_Marks(BYTE scale)
//*
//*	Description:
//* 	
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	None
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

void Draw_Arc_Tick_Marks(BYTE scale)
{
	float InnerX, InnerY, OutterX, OutterY, LowerOffset;
	float draw_angle, MajorTickLength, MinorTickLength;
	float inner_arc_magnitude, outer_arc_magnitude;
	float TickAngle, MajorAngle, MinorAngle;
	char index;

	LowerOffset = 3;																														// drop line below Rainbow Arc lines so no overlap
	MajorTickLength = 10;
	MinorTickLength = 5;
	arc_magnitude = (x_orgin - outline_magnitude - draw_magnitude - edge_spacing);
	inner_arc_magnitude = (arc_magnitude - outline_magnitude - 1);																			// -1
	outer_arc_magnitude = (arc_magnitude + draw_magnitude + 1);																				// +1

	draw_angle = INITIAL_ANGLE;
	InnerX = (x_orgin -(inner_arc_magnitude * (cos(draw_angle*3.14159/180))));
	InnerY = ((float) Y_ORIGIN -(inner_arc_magnitude * (sin(draw_angle*3.14159/180))))+ LowerOffset;

	OutterX = (x_orgin -((outer_arc_magnitude+outline_magnitude) * (cos(draw_angle*3.14159/180))));
	OutterY = ((float) Y_ORIGIN -((outer_arc_magnitude+outline_magnitude) * (sin(draw_angle*3.14159/180))))+ LowerOffset;

//	Line(InnerX, InnerY-1, OutterX, OutterY);																								// Draw line closing Zero side of Arc
	Line((SHORT) InnerX, (SHORT) InnerY - 1, (SHORT) OutterX, (SHORT) OutterY);																// Draw line closing Zero side of Arc (9DP v1.01.16)

	draw_angle = 180.0 - INITIAL_ANGLE;
	InnerX = (x_orgin -(inner_arc_magnitude * (cos(draw_angle*3.14159/180))));
	InnerY = ((float) Y_ORIGIN -(inner_arc_magnitude * (sin(draw_angle*3.14159/180))))+ LowerOffset;

	OutterX = (x_orgin -((outer_arc_magnitude+outline_magnitude) * (cos(draw_angle*3.14159/180))));
	OutterY = ((float) Y_ORIGIN -((outer_arc_magnitude+outline_magnitude) * (sin(draw_angle*3.14159/180))))+ LowerOffset;

//	Line(InnerX-1, InnerY-1, OutterX, OutterY-1);																							// Draw line closing Full Scale side of Arc
	Line((SHORT) InnerX - 1, (SHORT) InnerY - 1, (SHORT) OutterX, (SHORT) OutterY - 1);														// Draw line closing Zero side of Arc (9DP v1.01.16)

	TickAngle = (180.0 - (2.0 * INITIAL_ANGLE))/scale;

	for(index=0; index<=scale; index++)
	{
		MajorAngle = (TickAngle * index) + INITIAL_ANGLE;
		draw_angle = MajorAngle;
		InnerX = (x_orgin -((outer_arc_magnitude+outline_magnitude) * (cos(draw_angle*3.14159/180))));
		InnerY = ((float) Y_ORIGIN -((outer_arc_magnitude+outline_magnitude) * (sin(draw_angle*3.14159/180))));

		OutterX = (x_orgin -((outer_arc_magnitude+MajorTickLength+outline_magnitude) * (cos(draw_angle*3.14159/180))));
		OutterY = ((float) Y_ORIGIN -((outer_arc_magnitude+MajorTickLength+outline_magnitude) * (sin(draw_angle*3.14159/180))));

		if((index == 0) | (index == scale))
//			Line(InnerX, (InnerY + LowerOffset), OutterX, (OutterY + LowerOffset));															// Draw Major tick marks
			Line((SHORT) InnerX, (SHORT) (InnerY + LowerOffset), (SHORT) OutterX, (SHORT) (OutterY + LowerOffset));							// Draw Major tick marks (9DP v1.01.16)
		else
//			Line(InnerX, InnerY, OutterX, OutterY);																							// Draw Major tick marks
			Line((SHORT) InnerX, (SHORT) InnerY, (SHORT) OutterX, (SHORT) OutterY);															// Draw Major tick marks (9DP v1.01.16)
	}

	for(index=0; index<scale; index++)
	{
		MinorAngle = ((TickAngle * index) + (TickAngle/2)) + INITIAL_ANGLE;
		draw_angle = MinorAngle;
		InnerX = (x_orgin -((outer_arc_magnitude+outline_magnitude) * (cos(draw_angle*3.14159/180))));
		InnerY = ((float) Y_ORIGIN -((outer_arc_magnitude+outline_magnitude) * (sin(draw_angle*3.14159/180))));

		OutterX = (x_orgin -((outer_arc_magnitude+MinorTickLength+outline_magnitude) * (cos(draw_angle*3.14159/180))));
		OutterY = ((float) Y_ORIGIN -((outer_arc_magnitude+MinorTickLength+outline_magnitude) * (sin(draw_angle*3.14159/180))));

//		Line(InnerX, InnerY, OutterX, OutterY);																								// Draw Minor tick marks
		Line((SHORT) InnerX, (SHORT) InnerY, (SHORT) OutterX, (SHORT) OutterY);																// Draw Minor tick marks
	}

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Setup_Arc_Units(void)
//*
//*	Description:
//* 	
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	None
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

void Setup_Arc_Units(void)
{
	unsigned char index;
	unsigned char multiplier;
	unsigned short end_point;

	multiplier = lmi.display.detector[Current_Detector].view[Current_View].element.info.arc.control.multiplier;
	end_point = lmi.display.detector[Current_Detector].view[Current_View].element.info.arc.control.end_point;

	if(lmi.display.detector[Current_Detector].view[Current_View].element.info.arc.control.type_of_scale == LINEAR)
	{
		Full_Scale_Arc_Range[0][0] = 0;
		Full_Scale_Arc_Range[0][1] = 0;
		for(index = 1; index <= NUMBER_OF_RANGES; index++)
		{
			Full_Scale_Arc_Range[index][0] = end_point;
			Full_Scale_Arc_Range[index][1] = multiplier;
			end_point = end_point * 10;
			if(end_point > 999)
			{
				end_point = end_point / 1000;
				multiplier++;
			}
		}
	}
	else if(lmi.display.detector[Current_Detector].view[Current_View].element.info.arc.control.type_of_scale == LOG)
	{
		Full_Scale_Arc_Range[0][0] = end_point;
		Full_Scale_Arc_Range[0][1] = multiplier;
		for(index = 1; index <= NUMBER_OF_RANGES; index++)
		{
			Full_Scale_Arc_Range[index][0]	= 0;
			Full_Scale_Arc_Range[index][1]	= 0;
		}
	}

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Draw_Arc_Units(UINT8 update_type)
//*
//*	Description:
//* 	
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	None
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

void Draw_Arc_Units(UINT8 update_type)
{
	static unsigned char First_Draw = 0;
	static unsigned char previous_range = 1;
	unsigned char current_range;
	unsigned char index;
	unsigned char Erase, Drawn;
	unsigned char counter;
	unsigned int pos_x1;
	char char_buffer[10];
	XCHAR xchar_buffer[10];
	SHORT TextWidth, pri_unit_center;

	if((Range_Number != previous_range) || (update_type == NEW_WINDOW))
	{
		Erase = 0;
		Drawn = 0;	
		SetFont((void*)&Secondary_Arial_Black_14);
		while(!Drawn)
		{
			if(Erase == 0)
			{
				SetColor(background);																// for erasing range values
				current_range = previous_range;														// for previous range
				Erase = 1;																			// will indicate finished first cycle to erase
			}
			else
			{
				SetColor(foreground);																// for drawing range values
				current_range = Range_Number;														// for new range
				Drawn = 1;																			// will indicate finished drawing range units and values
			}

			sprintf(char_buffer , "%3d\0", Full_Scale_Arc_Range[current_range][0]);
			for(index = 0; index < 4; index++)
				xchar_buffer[index] = (XCHAR)char_buffer[index];							
			TextWidth = GetTextWidth((XCHAR*)xchar_buffer,(void*)&Secondary_Arial_Black_14);
			pos_x1 = 319 - (edge_spacing / 2) - TextWidth;
			MoveTo( pos_x1, Y_UNITS);
			while(!OutText((XCHAR*)xchar_buffer));
			
			index = 0;

			if(multipliers[Full_Scale_Arc_Range[current_range][1]] != 0)
				xchar_buffer[index++] = multipliers[Full_Scale_Arc_Range[current_range][1]];

			if(lmi.display.detector[Current_Detector].view[Current_View].element.info.arc.control.units == R_HR)
			{
				for(counter = 0; units_R_hr[counter] != 0; index++, counter++)
					xchar_buffer[index] = units_R_hr[counter];
			}
			else if(lmi.display.detector[Current_Detector].view[Current_View].element.info.arc.control.units == SV_HR)
			{
				for(counter = 0; units_Sv_hr[counter] != 0; index++, counter++)
					xchar_buffer[index] = units_Sv_hr[counter];
			}					
			else if(lmi.display.detector[Current_Detector].view[Current_View].element.info.arc.control.units == GY_HR)
			{
				for(counter = 0; units_Gy_hr[counter] != 0; index++, counter++)
					xchar_buffer[index] = units_Gy_hr[counter];
			}					
			else if(lmi.display.detector[Current_Detector].view[Current_View].element.info.arc.control.units == CPM)
			{
				for(counter = 0; units_CPM[counter] != 0; index++, counter++)
					xchar_buffer[index] = units_CPM[counter];
			}					

			xchar_buffer[index] = 0;

			pri_unit_center = GetTextWidth((XCHAR*)xchar_buffer, (void*)&Secondary_Arial_Black_14);
			pri_unit_center = (160-(pri_unit_center/2));
			MoveTo( pri_unit_center, Y_UNITS);
			while(!OutText((XCHAR*)xchar_buffer));
		}

		if((First_Draw == 0) || (update_type == NEW_WINDOW))
		{
			xchar_buffer[0] = '0';
			for(index = 1; index < 4; index++)
				xchar_buffer[index] = 0;
			pos_x1 = edge_spacing / 2;
			MoveTo( pos_x1, Y_UNITS);
			while(!OutText((XCHAR*)xchar_buffer));
			First_Draw = 1;
		}

		previous_range = Range_Number;
	}
	
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	WORD Update_Arc(void)
//*
//*	Description:
//* 	
//*
//*	Precondition:
//* 	Rate_uR_hr	- current exposure rate
//*		Range_Number - current range number
//*
//*	Parameters:
//* 	None
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

WORD Update_Arc(void)
{
	static float Last_Written_Rate = 1000.0;
	static int level = 0;
	static int scale_value;
	
	static unsigned char Last_Range_Drawn = 255;
	static unsigned char Last_Range_Number_Written = 255;
	static unsigned char last_written_digits = 3;

    float rate_result = GetRateResult();                                                                                                    // n1 Moved rate result calculation to new GetRateResult function
    RelativeAdjustment rels;
	float exposure;
	float temp_calc;

	int Alert_Level = 1000;
	int Alarm_Level = 1000;
	short rate_width;
	unsigned char index, draw_new_range, erase_finished;
	char char_buffer[7];
	BOOL exit_loop;

	if(DisplayDetachOnce == TRUE)
	{
		level = 0;																															// reinitialize to defaults
		Rate_Divide_By = 1.0;																												// reinitialize to defaults
		Last_Written_Rate = 1000.0;																											// reinitialize to defaults
		Last_Range_Drawn = 255;																												// reinitialize to defaults
		Last_Range_Number_Written = 255;																									// reinitialize to defaults
		last_written_digits = 3;																											// reinitialize to defaults
		DisplayDetachOnce = FALSE;
	}

	if(Update_Arc_Flag)																														// check if any flags have been set
	{
        Update_Range();                                                                                                     // n4 Added call to Update_Range to ensure that the electrometer range and Range_Number are correct before updating arc
        rels = GetRelativeAdjustment(Range_Number);
		SetLineType(SOLID_LINE);
		SetLineThickness(THICK_LINE);

		if(Last_Range_Drawn != Range_Number)                                                                                // n1 Left in these lines from Update_Range. Let's the drawing function know the
		{                                                                                                                   //    scaling marks need to be redrawn
			Draw_Arc_Units(UPDATE_WINDOW);
		}

		Rate_xR_hr = rate_result / Rate_Divide_By;																							// calculate rate with respect to the current range

		if(lmi.display.detector[Current_Detector].view[Current_View].alerts_radiation.info.feature.enable)
		{
			if(Rate_xR_hr >= (Radiation_Alert_Rate / rels.conversion))																	// 2nd check if Alert is activated
			{
				New_Alarm = ALERT;
				Alert_Level = (int)((Radiation_Alert_Rate/rels.result) * (float) MAX_ARC_DIVISIONS);
			}
		}
 		if(lmi.display.detector[Current_Detector].view[Current_View].alarms_radiation.info.feature.enable)
		{
			if(Rate_xR_hr >= (Radiation_Alarm_Rate / rels.conversion))																	// 1st check if Alarm is activated
			{
				New_Alarm = ALARM;
				Alarm_Level = (int)((Radiation_Alarm_Rate/rels.result) * (float) MAX_ARC_DIVISIONS);
			}
		}

		if(New_Alarm == ALERT)
		{
			add_error_message_to_queue(ALERT_RADIATION);
		}
		else if(New_Alarm == ALARM)
		{
			add_error_message_to_queue(ALARM_RADIATION);
		}

		temp_calc = Rate_xR_hr;																												// set temp variable to equal new Rate
		temp_calc = temp_calc / (float) Full_Scale_Arc_Range[Range_Number][0];																// divide temp reading by max current range
		temp_calc = (float) MAX_ARC_DIVISIONS * temp_calc;																					// calculate ratio of full Arc picture
		scale_value = (int)temp_calc;																										// set scale value for new arc level

		erase_finished = 0;																													// clear erase finished, default not finished
		draw_new_range = 0;																													// clear flag which makes while loop continue
		exit_loop = 0;																														// clear exit loop flag

		while(((level >= (int) MIN_ARC_DIVISIONS) && (level <= (int) MAX_ARC_DIVISIONS) && (level != scale_value) && (!exit_loop)) | (draw_new_range))
		{		
			if(Last_Range_Drawn != Range_Number)
			{
				if((level > 0) && (!erase_finished))
				{
					scale_value = 0;																										// clear scale_value to clear arc picture down to 0
					draw_new_range = 1;																										// set variable to make loop continue
				}
				else
				{
					erase_finished = 1;																										// indicate finished erasing arc
					scale_value = (int)temp_calc;																							// set scale value for new arc level
					draw_new_range = 0;																										// clear variable so loop stops when level = scale_value
				}
			}

			if(level >= scale_value)  																										// need to decrease arc level 
			{
				if((level >= 0) && (level < MAX_ARC_DIVISIONS))																				// (9DP v1.01.16)
				{				
					SetColor(background);																									// for background color	
					Line((SHORT) cursor_x[level], (SHORT) cursor_y[level], (SHORT) pos_xx[level], (SHORT) pos_yy[level]);					// draw 'cleared' line (9DP v1.01.16)
				}
						
				if(level > 0)
				{
					level = level - 1;
				}
			}
			else
			{	
				if(scale_value > level)																										// Need to Increase Arc Level
				{
					// decide line color
					if((level >= Alarm_Level) && (lmi.display.detector[Current_Detector].view[Current_View].alarms_radiation.info.feature.enable))
					{
						SetColor(ALARM_ARC_COLOR);
					}
					else
					{
						if((level >= Alert_Level) && (lmi.display.detector[Current_Detector].view[Current_View].alerts_radiation.info.feature.enable))
						{
							SetColor(ALERT_ARC_COLOR);	
						}
						else
						{
							SetColor(NORMAL_ARC_COLOR);
						}
					}
	
					Line((SHORT) cursor_x[level], (SHORT) cursor_y[level], (SHORT) pos_xx[level], (SHORT) pos_yy[level]);					// draw colored lines	
					
					if(level < (int) MAX_ARC_DIVISIONS)
					{
						level = (level + 1);
					}
					
					if((level >= (int) MAX_ARC_DIVISIONS) && (scale_value > (int) MAX_ARC_DIVISIONS))
					{
						exit_loop = 1;
					}
				}
			}
		}

		Update_Arc_Flag = 0;																												// clear update flags
		Last_Range_Drawn = Range_Number;
	}

	if((Update_Display_Rate_Flag) || (Last_Range_Number_Written != Range_Number))
	{
		exposure = Rate_xR_hr;
		if((exposure != Last_Written_Rate) || (Last_Range_Number_Written != Range_Number))													// if new value (peak) is different from previously written value 
		{	
			SetColor(background);
			if(last_written_digits == 4)
			{
				SetFont((void*)&Normal_Arial_Black_36);
				rate_width = GetTextWidth((XCHAR*)ExposureValue, (void*)&Normal_Arial_Black_36);
				rate_width = 160 - (rate_width / 2);
				MoveTo(rate_width, Y_READING + 15);
			}
			else
			{
				SetFont((void*)&Normal_Arial_Black_44);
				rate_width = GetTextWidth((XCHAR*)ExposureValue, (void*)&Normal_Arial_Black_44);
				rate_width = 160 - (rate_width / 2);
				MoveTo(rate_width, Y_READING);
			}

			while(!OutText((XCHAR*)ExposureValue));

			SetColor(foreground);

			if(lmi.display.detector[Current_Detector].view[Current_View].element.info.arc.control.digits == 4)
			{
				SetFont((void*)&Normal_Arial_Black_36);

				if(Range_Number == 1)
				{
					switch(lmi.display.detector[Current_Detector].view[Current_View].element.info.arc.control.units)
					{
						case R_HR:
						{
//							if((exposure < 9.999) && (lmi.settings.product.info.model_number != MODEL_9DP) && (lmi.settings.product.info.model_number != MODEL_9DP_PLUS) && (lmi.settings.product.info.model_number != MODEL_V1))		// (9DP v1.01.21)
							if((exposure < 9.999) && (lmi.display.detector[Current_Detector].view[Current_View].element.info.arc.control.multiplier > MICRO))					// (9DP v1.01.24)
							{																																					// (9DP v1.01.17)
								sprintf(char_buffer , "%5.3f\0", exposure);																										// (9DP v1.01.17)
							}																																					// (9DP v1.01.17)
//							else if((exposure < 99.99) && (lmi.settings.product.info.model_number != MODEL_9DP) && (lmi.settings.product.info.model_number != MODEL_9DP_PLUS) && (lmi.settings.product.info.model_number != MODEL_V1))	// (9DP v1.01.21)
							else if((exposure < 99.99) && (lmi.display.detector[Current_Detector].view[Current_View].element.info.arc.control.multiplier > MICRO))				// (9DP v1.01.24)
							{																	
								sprintf(char_buffer , "%5.2f\0\0", exposure);																									// (9DP v1.01.17)
							}																																					// (9DP v1.01.17)
							else if(exposure < 999.9)																															// (9DP v1.01.17)
							{
								sprintf(char_buffer , "%5.1f\0", exposure);
							}
							else																																				// (9DP v1.01.16)
							{																																					// (9DP v1.01.16)
								exposure = 999.9;																																// (9DP v1.01.16)
								sprintf(char_buffer , "%5.1f\0", exposure);																										// (9DP v1.01.16)
							}																																					// (9DP v1.01.16)
							break;
						}
						case SV_HR:
						case GY_HR:
						{
//							if((exposure < 9.999) && (lmi.settings.product.info.model_number != MODEL_9DP) && (lmi.settings.product.info.model_number != MODEL_9DP_PLUS) && (lmi.settings.product.info.model_number != MODEL_V1))		// (9DP v1.01.21)
							if((exposure < 9.999) && (lmi.display.detector[Current_Detector].view[Current_View].element.info.arc.control.multiplier > MICRO))					// (9DP v1.01.24)
							{																																					// (9DP v1.01.17)
								sprintf(char_buffer , "%5.3f\0", exposure);																										// (9DP v1.01.17)
							}																																					// (9DP v1.01.17)
							else if(exposure < 99.99)																															// (9DP v1.01.17)
							{
								sprintf(char_buffer , "%5.2f\0", exposure);
							}
							else if(exposure < 999.9)																															// (9DP v1.01.17)	
							{																																					// (9DP v1.01.16)
								sprintf(char_buffer , "%5.1f\0", exposure);																										// (9DP v1.01.16)
							}																																					// (9DP v1.01.16)
							else
							{
								exposure = 999.9;																																// (9DP v1.01.17)
								sprintf(char_buffer , "%5.1f\0", exposure);
							}
							break;
						}
						case CPM:																																				// (9DP v1.01.17)
						{																																						// (9DP v1.01.17)
							if(exposure < 9.999)																																// (9DP v1.01.17)
							{																																					// (9DP v1.01.17)
								sprintf(char_buffer , "%5.3f\0", exposure);																										// (9DP v1.01.17)
							}																																					// (9DP v1.01.17)
							else if(exposure < 99.99)																															// (9DP v1.01.17)
							{																																					// (9DP v1.01.17)
								sprintf(char_buffer , "%5.2f\0", exposure);																										// (9DP v1.01.17)
							}																																					// (9DP v1.01.17)
							else if(exposure < 999.9)																															// (9DP v1.01.17)
							{																																					// (9DP v1.01.17)
								sprintf(char_buffer , "%5.1f\0", exposure);																										// (9DP v1.01.17)
							}																																					// (9DP v1.01.17)
							else																																				// (9DP v1.01.17)
							{																																					// (9DP v1.01.17)
								exposure = 999.9;																																// (9DP v1.01.17)
								sprintf(char_buffer , "%5.1f\0", exposure);																										// (9DP v1.01.17)
							}																																					// (9DP v1.01.17)
						}																																						// (9DP v1.01.17)
					}
				}
				else
				{
					if(exposure < 9.999)
					{
						sprintf(char_buffer , "%5.3f\0", exposure);
					}
					else if(exposure < 99.99)
					{
						sprintf(char_buffer , "%5.2f\0", exposure);
					}
					else if(exposure < 999.9)
					{
						sprintf(char_buffer , "%5.1f\0", exposure);
					}
					else
					{
						exposure = 999.9;																																		// (9DP v1.01.16)
						sprintf(char_buffer , "%5.1f\0", exposure);																												// (9DP v1.01.16)
					}
				}

				for (index = 0; index < 6; index++)
					ExposureValue[index] = (XCHAR)char_buffer[index];

				ExposureMultiplier = Full_Scale_Arc_Range[Range_Number][1];
				ExposureUnits = lmi.display.detector[Current_Detector].view[Current_View].element.info.arc.control.units;
				ExposureRange = Range_Number;

				rate_width = GetTextWidth((XCHAR*)ExposureValue, (void*)&Normal_Arial_Black_36);
				rate_width = 160 - (rate_width / 2);
				MoveTo(rate_width, Y_READING + 15);
				last_written_digits = 4;
			}
			else
			{
				SetFont((void*)&Normal_Arial_Black_44);

				if(Range_Number == 1)
				{
					switch(lmi.display.detector[Current_Detector].view[Current_View].element.info.arc.control.units)
					{
						case R_HR:
						{
//							if((exposure < 9.99) && (lmi.settings.product.info.model_number != MODEL_9DP) && (lmi.settings.product.info.model_number != MODEL_9DP_PLUS)	&& (lmi.settings.product.info.model_number != MODEL_V1))	// (9DP v1.01.21)
							if((exposure < 9.99) && (lmi.display.detector[Current_Detector].view[Current_View].element.info.arc.control.multiplier > MICRO))					// (9DP v1.01.24)
							{																																					// (9DP v1.01.17)
								sprintf(char_buffer , "%4.2f\0\0", exposure);																									// (9DP v1.01.17)
							}																																					// (9DP v1.01.17)
							else if(exposure < 99.9)																															// (9DP v1.01.17)
							{
								sprintf(char_buffer , "%4.1f\0\0", exposure);
							}
							else if(exposure < 999.0)
							{
								sprintf(char_buffer , "%3d\0\0\0", (unsigned short) exposure);
							}
							else
							{
								exposure = 999.0;
								sprintf(char_buffer , "%3d\0\0\0", (unsigned short) exposure);
							}
							break;
						}
						case CPM:
						case SV_HR:
						case GY_HR:
						{
							if(exposure < 9.99)
							{
								sprintf(char_buffer , "%4.2f\0\0", exposure);
							}
							else if(exposure < 99.9)
							{
								sprintf(char_buffer , "%4.1f\0\0", exposure);																									// (9DP v1.01.16)
							}
							else if(exposure < 999.0)																															// (9DP v1.01.17)
							{																																					// (9DP v1.01.17)
								sprintf(char_buffer , "%3d\0\0\0", (unsigned short) exposure);																					// (9DP v1.01.17)
							}																																					// (9DP v1.01.17)
							else																																				// (9DP v1.01.17)
							{																																					// (9DP v1.01.17)
								exposure = 999.0;																																// (9DP v1.01.17)
								sprintf(char_buffer , "%3d\0\0\0", (unsigned short) exposure);																					// (9DP v1.01.17)
							}																																					// (9DP v1.01.17)
							break;
						}
					}
				}
				else
				{
					if(exposure < 9.99)
					{
						sprintf(char_buffer , "%4.2f\0\0", exposure);
					}
					else if(exposure < 99.9)
					{
						sprintf(char_buffer , "%4.1f\0\0", exposure);
					}
					else if(exposure < 999.0)
					{
						sprintf(char_buffer , "%3d\0\0\0", (unsigned short) exposure);
					}
					else
					{
						exposure = 999.0;
						sprintf(char_buffer , "%3d\0\0\0", (unsigned short) exposure);
					}
				}
				for (index = 0; index < 6; index++)
					ExposureValue[index] = (XCHAR)char_buffer[index];

				ExposureMultiplier = Full_Scale_Arc_Range[Range_Number][1];
				ExposureUnits = lmi.display.detector[Current_Detector].view[Current_View].element.info.arc.control.units;
				ExposureRange = Range_Number;

				rate_width = GetTextWidth((XCHAR*)ExposureValue, (void*)&Normal_Arial_Black_44);
				rate_width = 160 - (rate_width / 2);
				MoveTo(rate_width, Y_READING);
				last_written_digits = 3;
			}

			while(!OutText((XCHAR*)ExposureValue));
			Last_Written_Rate = exposure;
		}
		Last_Range_Number_Written = Range_Number;
		Update_Display_Rate_Flag = 0;																									
	}

	return 1;
}


//*
//**************************************************************************************************
//**************************************************************************************************
//* LUDLUM MEASUREMENTS INC. COPYRIGHT (C) 2010 
//**************************************************************************************************
//**************************************************************************************************

