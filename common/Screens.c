//**************************************************************************************************
//**************************************************************************************************
//*																		
//*	Filename:		Screens.c
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
//*	~~~~~~~~	10/24/11	James Comstock
//*	- created system display process
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
//*	PICmicro� Microcontroller is intended and supplied to you, the Company�s customer, for use
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
#include "Screens.h" 
#include "Primitive.h"
#include "Elements.h"
#include "Functions.h"
#include "Icons.h"

//**************************************************************************************************
//* External Variable Definitions

extern SCREEN_STATES PreviousScreenState;															// (9DP v1.01.04)
extern const FONT_FLASH Arial_Rounded_12pt_32_90;													// Arial_Rounded_12pt_32_90
extern const FONT_FLASH Secondary_Arial_Black_14;
extern WORD foreground;
extern WORD background;
extern TYPE_MESSAGE_BOX pmsgScreen;
extern BITMAP_FLASH LMI_Icon;
extern TYPE_INSTRUMENT lmi;
extern GOL_SCHEME *btnScheme;
extern char DisplayMessage[29];
extern BOOL System_Error_Flag;
extern DWORD system_error_tick;
extern DWORD tick;
extern SCREEN_STATES screenState;
extern BYTE Present_Level;
extern BOOL Message_Event_Flag;
extern BYTE Current_Detector;
extern BYTE Current_View;
extern BYTE Backlight_Mode;

extern UINT8 Error_Recent;
extern UINT8 Error_Last;
extern UINT8 Last_Saved_Error_Number;
extern UINT8 Error_Buffer[];
extern BOOL Custom_Error_Pending;

//**************************************************************************************************
//* Global Variable Definitions

volatile BUTTON *pbtnSoftbutton1, *pbtnSoftbutton2, *pbtnSoftbutton3, *pbtnSoftbutton4 ;

volatile BOOL REFRESH_LEVEL = FALSE;

volatile BOOL Flag_Status_Text_Displayed = FALSE;
volatile BOOL Flag_Note_Text_Displayed = FALSE;
//volatile BOOL Flag_Custom_Msg_Enabled = FALSE;

//volatile UINT8 Error_Recent = 0;
//volatile UINT8 Error_Last = 0;
//volatile UINT8 Last_Saved_Error_Number = 0;
//volatile UINT8 Error_Buffer[NUMBER_OF_ERRORS];
//volatile BOOL Error_Pending = FALSE;

//**************************************************************************************************
//*	Constant Definitions

const XCHAR Ack[] 		= {'A','C','K',0};
const XCHAR Back[] 		= {'B','A','C','K',0};
const XCHAR Reset[]		= {'R','E','S','E','T',0};
const XCHAR Cancel[] 	= {'C','A','N','C','E','L',0};
const XCHAR Ok[]	  	= {'O','K',0};



//void clear_error_message_off_queue(void);
//void Display_Error_Message(void);
//void add_error_message_to_queue(unsigned char error);

//**************************************************************************************************
//**************************************************************************************************
//*	THUMBDRIVE SCREENS
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Thumbdrive_File_Menu(void)
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

void Thumbdrive_File_Menu(void)
{
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	END THUMBDRIVE SCREENS
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void LudlumScreen(void)
//*
//*	Description:
//* 	Displays Ludlum Measurements company contact information
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

void LudlumScreen(void)
{
	SHORT counter;
	char stringconversion[28];
	
	pmsgScreen.txt_total_lines			= 	6;														// set number of lines to write
	pmsgScreen.txt_color_line[0]		=	'\0';													// set array [0] location to NULL
	pmsgScreen.txt_color_line[1]		=	BRIGHTRED;												// set line colors
	pmsgScreen.txt_color_line[2]		=	BRIGHTRED;
	pmsgScreen.txt_color_line[3]		=	BRIGHTRED;
	pmsgScreen.txt_color_line[4]		=	BRIGHTRED;
	pmsgScreen.txt_color_line[5]		=	BRIGHTRED;
	pmsgScreen.txt_color_line[6]		=	BRIGHTRED;
	pmsgScreen.font[0]					= 	'\0';													// set array [0] location to NULL
	pmsgScreen.font[1]					= 	(void*)&Arial_Rounded_12pt_32_90;						// Set Fonts
	pmsgScreen.font[2]					= 	(void*)&GOLFontDefault;
	pmsgScreen.font[3]					= 	(void*)&GOLFontDefault;
	pmsgScreen.font[4]					= 	(void*)&GOLFontDefault;
	pmsgScreen.font[5]					= 	(void*)&GOLFontDefault;
	pmsgScreen.font[6]					= 	(void*)&GOLFontDefault;
	pmsgScreen.box_background_color		=	WHITE;


	sprintf(stringconversion, "LUDLUM MEASUREMENTS\0");
	Convert_For_Display(pmsgScreen.txt_text[1], NULL, stringconversion, 28);

	sprintf(stringconversion, "501 Oak Street\0");
	Convert_For_Display(pmsgScreen.txt_text[2], NULL, stringconversion, 28);

	sprintf(stringconversion, "P.O. Box 810\0");
	Convert_For_Display(pmsgScreen.txt_text[3], NULL, stringconversion, 28);

	sprintf(stringconversion, "Sweetwater, TX 79556\0");
	Convert_For_Display(pmsgScreen.txt_text[4], NULL, stringconversion, 28);

	sprintf(stringconversion, "Tel: 325-235-5494\0");
	Convert_For_Display(pmsgScreen.txt_text[5], NULL, stringconversion, 28);

	sprintf(stringconversion, "Fax: 325-235-4672\0");
	Convert_For_Display(pmsgScreen.txt_text[6], NULL, stringconversion, 28);

	Display_Message(&pmsgScreen, MSG_ADJ_NO_HEADING, NORMAL_MESSAGE, CENTER_JUSTIFY);				// Display message box with Ludlum information

	for(counter = 0; counter < (320-59); counter++)
	{																								// set X range from image to scroll within message box
        PutImage(counter,(190),(void*)&LMI_Icon,IMAGE_NORMAL);   									// Put image (move up screen 3 pixels due to box border shading width
		//Check_Battery();
    }

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void StartupScreen(void)
//*
//*	Description:
//* 	Displays instrument information:
//*			- Model Number, Serial Number, Firmware Number, User ID Number, User Serial Number, 
//*			  Calibration Date, and Calibration Due Date.
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

void StartupScreen(void)
{
	char stringconversion[29];
	char message_string[64];																		// (9DP v1.01.04)

	pmsgScreen.txt_total_lines			= 	7;
	pmsgScreen.txt_color_line[0]		=	'\0';
	pmsgScreen.txt_color_line[1]		=	BLACK;
	pmsgScreen.txt_color_line[2]		=	BLACK;
	pmsgScreen.txt_color_line[3]		=	BLACK;
	pmsgScreen.txt_color_line[4]		=	BLACK;
	pmsgScreen.txt_color_line[5]		=	BLACK;
	pmsgScreen.txt_color_line[6]		=	BLACK;
	pmsgScreen.txt_color_line[7]		=	BLACK;

	pmsgScreen.font[0]					= 	(void*)&GOLFontDefault;
	pmsgScreen.font[1]					= 	(void*)&GOLFontDefault;
	pmsgScreen.font[2]					= 	(void*)&GOLFontDefault;
	pmsgScreen.font[3]					= 	(void*)&GOLFontDefault;
	pmsgScreen.font[4]					= 	(void*)&GOLFontDefault;
	pmsgScreen.font[5]					= 	(void*)&GOLFontDefault;
	pmsgScreen.font[6]					= 	(void*)&GOLFontDefault;
	pmsgScreen.font[7]					= 	(void*)&GOLFontDefault;

	pmsgScreen.box_background_color		=	WHITE;

//	sprintf(stringconversion, "Model: %2s", lmi.settings.product.info.model);							// (9DP v1.01.04)
	strncpy(message_string, lmi.settings.product.info.model, LENGTH_MODEL);								// (9DP v1.01.04)
	message_string[LENGTH_MODEL] = '\0';																// (9DP v1.01.04)
	sprintf(stringconversion, "Model: %2s\0", message_string);											// (9DP v1.01.04)
	Convert_For_Display(pmsgScreen.txt_text[1], NULL, stringconversion, 28);

//	sprintf(stringconversion, "Serial #: %2s", lmi.settings.product.info.serial_number);				// (9DP v1.01.04)
	strncpy(message_string, lmi.settings.product.info.serial_number, LENGTH_SERIAL_NUMBER);				// (9DP v1.01.04)
	message_string[LENGTH_SERIAL_NUMBER] = '\0';														// (9DP v1.01.04)
	sprintf(stringconversion, "Serial #: %2s\0", message_string);										// (9DP v1.01.04)
	Convert_For_Display(pmsgScreen.txt_text[2], NULL, stringconversion, 28);

	sprintf(stringconversion, "Firmware: %2s\0", FIRMWARE_NUMBER_VALUE);			  
	Convert_For_Display(pmsgScreen.txt_text[3], NULL, stringconversion, 29);

//	sprintf(stringconversion, "User ID: %2s", lmi.settings.product.info.user_id_number);				// (9DP v1.01.04)
	strncpy(message_string, lmi.settings.product.info.user_id_number, LENGTH_USER_ID_NUMBER);			// (9DP v1.01.04)
	message_string[LENGTH_USER_ID_NUMBER] = '\0';														// (9DP v1.01.04)
	sprintf(stringconversion, "User ID: %2s\0", message_string);										// (9DP v1.01.04)
	Convert_For_Display(pmsgScreen.txt_text[4], NULL, stringconversion, 28);

//	sprintf(stringconversion, "User SN: %2s", lmi.settings.product.info.user_serial_number);			// (9DP v1.01.04)
	strncpy(message_string, lmi.settings.product.info.user_serial_number, LENGTH_USER_SERIAL_NUMBER);	// (9DP v1.01.04)
	message_string[LENGTH_USER_SERIAL_NUMBER] = '\0';													// (9DP v1.01.04)
	sprintf(stringconversion, "User SN: %2s\0", message_string);										// (9DP v1.01.04)
	Convert_For_Display(pmsgScreen.txt_text[5], NULL, stringconversion, 28);

	sprintf(stringconversion , "Cal Date: %2d/%02d/20%02d\0", lmi.settings.product.info.calibration_date.month, lmi.settings.product.info.calibration_date.day, lmi.settings.product.info.calibration_date.year);
	Convert_For_Display(pmsgScreen.txt_text[6], NULL, stringconversion, 25);

	if(lmi.settings.product.info.model_number != MODEL_V1)												// (9DP v1.01.21)
	{
		sprintf(stringconversion , "Cal Due Date: %2d/%02d/20%02d\0", lmi.settings.product.info.calibration_due_date.month, lmi.settings.product.info.calibration_due_date.day, lmi.settings.product.info.calibration_due_date.year);
		Convert_For_Display(pmsgScreen.txt_text[7], NULL, stringconversion, 25);						// set structure text line 7 to calibration due date value						
	}																									// (9DP v1.01.21)

	sprintf(DisplayMessage, "PRODUCT\0");
	Display_Message(&pmsgScreen, MSG_FULL_HEADING, NORMAL_MESSAGE, LEFT_JUSTIFY);						// Display the message box with the instrument information

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void ScrambleClearScreen(void)
//*
//*	Description:
//* 	Scrambles the pixel information on the screen by shifting values then clears the screen
//*		to WHITE
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

void ScrambleClearScreen(void)
{
	WORD ytemp, xtemp, srRes = 0x0001;
	WORD z, j, k;

	SetColor(WHITE);																				// set color to white
	
	// random fade effect using a Linear Feedback Shift Register (LFSR)
    for (z = 1800; z > 0 ; z--)																		// run loop to scramble pixel information on screen
	{
		// for a 16 bit LFSR variable the taps are at bits: 1, 2, 4, and 15
   		srRes = (srRes >> 1) ^ (-(int)(srRes & 1) & 0x8006);  
    	xtemp = (srRes & 0x00FF)%40;
    	ytemp = (srRes >> 8)%30;
    
    	for (j = 0; j < 8; j++)																		// by replicating the white (1x1) bars into 8 areas fading is faster
	   		for (k = 0; k < 8; k++)
	    		PutPixel(xtemp+(j*40), ytemp+(k*30)); 
	}  
	ClearDevice();																					// clear screen to start background color

	return;
}

////*
////**************************************************************************************************
////**************************************************************************************************
////*	Function:
////* 	void add_error_message_to_queue(unsigned char error)
////*
////*	Description:
////* 	
////*
////*	Precondition:
////* 	None
////*
////*	Parameters:
////*		None
////*
////*	Return Values:
////* 	None
////*
////*	Remarks:
////* 	None
////*
//
//void add_error_message_to_queue(unsigned char error)
//{
//    if(error != Last_Saved_Error_Number)
//    {
//        Error_Buffer[Error_Recent++] = error;
//        Error_Recent %= NUMBER_OF_ERRORS;
//        if(Error_Recent == Error_Last)
//        {
//            Error_Last++;
//            Error_Last %= NUMBER_OF_ERRORS;
//        }
//        Last_Saved_Error_Number = error;
//    }
//
//	return;
//}
//
////*
////**************************************************************************************************
////**************************************************************************************************
////*	Function:
////* 	void clear_error_message_off_queue(void)
////*
////*	Description:
////* 	
////*
////*	Precondition:
////* 	None
////*
////*	Parameters:
////*		None
////*
////*	Return Values:
////* 	None
////*
////*	Remarks:
////* 	None
////*
//
//void clear_error_message_off_queue(void)
//{
//    Error_Last++;
//    Error_Last %= NUMBER_OF_ERRORS;
//    Error_Pending = FALSE;
//
//	return;
//}
//
////*
////**************************************************************************************************
////**************************************************************************************************
////*	Function:
////* 	void Display_Error_Message(void)
////*
////*	Description:
////* 	
////*
////*	Precondition:
////* 	None
////*
////*	Parameters:
////*		None
////*
////*	Return Values:
////* 	None
////*
////*	Remarks:
////* 	None
////*
//
//void Display_Error_Message(void)
//{
//	unsigned char ErrorMsg[51] = {'\0'};																											// declare error message char array
//	XCHAR tempXmsg[51] = {'\0'};
//	SHORT msgwidth;
//	BYTE message_event;
//
//	Custom_Error_Pending = FALSE;																													// clear error pending flag
//	message_event = NORMAL;																															// clear event type
//
//	switch(Error_Buffer[Error_Last])
//	{
//		case NORMAL_MESSAGE:
//		{
//		    clear_error_message_off_queue();
//			return;
//		}
//		case ALERT_RADIATION:
//		{
//			if(lmi.display.detector[Current_Detector].view[Current_View].alerts_radiation.info.feature.enable)										// check if radiation alert enabled (9DP v1.01.04)
//	        {
//				Present_Level = ALERT;																												// enable alert audio
//				if(lmi.display.detector[Current_Detector].view[Current_View].alerts_radiation.info.feature.custom)									// check if custom message enabled (9DP v1.01.04)
//            	{
//            		Custom_Error_Pending = TRUE;																									// waiting on user acknowledgement
//					strncpy(ErrorMsg, lmi.display.detector[Current_Detector].view[Current_View].alerts_radiation.info.feature.custom_message, 50);	// (9DP v1.01.04)
//					ErrorMsg[50] = '\0';																											// (9DP v1.01.04)
//					Use_Custom_Message(ErrorMsg, MSG_H_E_F_HEADING, ALERT_RADIATION, CENTER_JUSTIFY);												// arrange custom message
//				}
//				else 																																// custom is disabled -- use default Message (no screen box used)
//				{
//					message_event = ALERT;
//				}
//			}
//			else																																	// if alerts are disabled
//			{		
//				Present_Level = NORMAL;
//			}
//			break;
//		}
//		case ALERT_INTEGRATE:
//		{
//			if(lmi.display.detector[Current_Detector].view[Current_View].alerts_integrate.info.feature.enable)										// check if integrate alert enabled (9DP v1.01.04)
//			{
//				Present_Level = ALERT;																												// enable alert audio
//				if(lmi.display.detector[Current_Detector].view[Current_View].alerts_integrate.info.feature.custom)									// check if custom message enabled (9DP v1.01.04)
//				{
//					Custom_Error_Pending = TRUE;																									// waiting on user acknowledgement
//					strncpy(ErrorMsg, lmi.display.detector[Current_Detector].view[Current_View].alerts_integrate.info.feature.custom_message, 50);	// (9DP v1.01.04)
//					ErrorMsg[50] = '\0';																											// (9DP v1.01.04)
//					Use_Custom_Message(ErrorMsg, MSG_H_E_F_HEADING, ALERT_INTEGRATE, CENTER_JUSTIFY);												// arrange custom message
//				}
//				else 																																// custom is disabled -- use default message (no screen box used)
//				{
//					message_event = ALERT;
//				}
//			}
//			else																																	// if alerts are disabled
//			{	
//				Present_Level = NORMAL;
//			}
//			break;
//		}
//		case ALERT_FUNCTION:
//		{
//			if(lmi.display.detector[Current_Detector].view[Current_View].alerts_function.info.feature.enable)										// check if function alert enabled (9DP v1.01.04)
//			{
//				Present_Level = ALERT;																												// enable alert audio
//				if(lmi.display.detector[Current_Detector].view[Current_View].alerts_function.info.feature.custom)									// check if custom message enabled (9DP v1.01.04)
//				{
//					Custom_Error_Pending = TRUE;																									// waiting on user acknowledgement
//					strncpy(ErrorMsg, lmi.display.detector[Current_Detector].view[Current_View].alerts_function.info.feature.custom_message, 50);	// (9DP v1.01.04)
//					ErrorMsg[50] = '\0';																											// (9DP v1.01.04)
//					Use_Custom_Message(ErrorMsg, MSG_H_E_F_HEADING, ALERT_INTEGRATE, CENTER_JUSTIFY);												// arrange custom message
//				}
//				else 																																// custom is disabled -- use default message (no screen box used)
//				{
//					message_event = ALERT;
//				}
//			}
//			else																																	// if alerts are disabled
//			{	
//				Present_Level = NORMAL;
//			}
//			break;
//		}
//		case ALERT_LOW_BATTERY:
//		{
//			if(lmi.alerts.low_battery.info.feature.enable)
//			{
//				Present_Level = ALERT;																												// enable alert audio
//				if(lmi.alerts.low_battery.info.feature.custom)
//				{
//					Custom_Error_Pending = TRUE;																									// waiting on user acknowledgement
//					strncpy(ErrorMsg, lmi.alerts.low_battery.info.feature.custom_message, 50);
//					ErrorMsg[50] = '\0';																											// (9DP v1.01.04)
//					Use_Custom_Message(ErrorMsg, MSG_H_E_F_HEADING, ALERT_INTEGRATE, CENTER_JUSTIFY);												// arrange custom message
//				}
//				else
//				{
//					message_event = ALERT;
//				}	
//			}
//			else
//			{
//				Present_Level = NORMAL;
//			}	
//			break;
//		}
//		case ALERT_LOW_MEMORY:
//		{
//			if(lmi.alerts.low_memory.info.feature.enable)
//			{
//				Present_Level = ALERT;																												// enable alert audio
//				if(lmi.alerts.low_memory.info.feature.custom)
//				{
//					Custom_Error_Pending = TRUE;																									// waiting on user acknowledgement
//					strncpy(ErrorMsg, lmi.alerts.low_memory.info.feature.custom_message, 50);
//					ErrorMsg[50] = '\0';																											// (9DP v1.01.04)
//					Use_Custom_Message(ErrorMsg, MSG_H_E_F_HEADING, ALERT_INTEGRATE, CENTER_JUSTIFY);												// arrange custom message
//				}
//				else
//				{
//					message_event = ALERT;
//				}	
//			}
//			else
//			{
//				Present_Level = NORMAL;
//			}	
//			break;
//		}
//		case ALERT_CALIBRATION_DUE_DATE:
//		{
//			if(lmi.alerts.calibration_due.info.feature.enable)
//			{
//				Present_Level = ALERT;																												// enable alert audio
//				if(lmi.alerts.calibration_due.info.feature.custom)
//				{
//					Custom_Error_Pending = TRUE;																									// waiting on user acknowledgement
//					strncpy(ErrorMsg, lmi.alerts.calibration_due.info.feature.custom_message, 50);
//					ErrorMsg[50] = '\0';																											// (9DP v1.01.04)
//					Use_Custom_Message(ErrorMsg, MSG_H_E_F_HEADING, ALERT_INTEGRATE, CENTER_JUSTIFY);												// arrange custom message
//				}
//				else
//				{
//					message_event = ALERT;
//				}	
//			}
//			else
//			{
//				Present_Level = NORMAL;
//			}	
//			break;
//		}
//		case ALERT_SOURCE_CHECK:
//		{
//			if(lmi.alerts.source_check.info.feature.enable)
//			{
//				Present_Level = ALERT;																												// enable alert audio
//				if(lmi.alerts.source_check.info.feature.custom)
//				{
//					Custom_Error_Pending = TRUE;																									// waiting on user acknowledgement
//					strncpy(ErrorMsg, lmi.alerts.source_check.info.feature.custom_message, 50);
//					ErrorMsg[50] = '\0';																											// (9DP v1.01.04)
//					Use_Custom_Message(ErrorMsg, MSG_H_E_F_HEADING, ALERT_INTEGRATE, CENTER_JUSTIFY);												// arrange custom message
//				}
//				else
//				{
//					message_event = ALERT;
//				}	
//			}
//			else
//			{
//				Present_Level = NORMAL;
//			}	
//			break;
//		}
//		case ALERT_BACKGROUND_CHECK:
//		{
//			if(lmi.alerts.background_check.info.feature.enable)
//			{
//				Present_Level = ALERT;																												// enable alert audio
//				if(lmi.alerts.background_check.info.feature.custom)
//				{
//					Custom_Error_Pending = TRUE;																									// waiting on user acknowledgement
//					strncpy(ErrorMsg, lmi.alerts.background_check.info.feature.custom_message, 50);
//					ErrorMsg[50] = '\0';																											// (9DP v1.01.04)
//					Use_Custom_Message(ErrorMsg, MSG_H_E_F_HEADING, ALERT_INTEGRATE, CENTER_JUSTIFY);												// arrange custom message
//				}
//				else
//				{
//					message_event = ALERT;
//				}	
//			}
//			else
//			{
//				Present_Level = NORMAL;
//			}	
//			break;
//		}
//		case ALERT_SCALER:
//		{
//			Present_Level = ALERT;																													// enable alert audio
//			message_event = ALERT;
//			break;
//		}
//		case ALARM_RADIATION:
//		{
//			if(lmi.display.detector[Current_Detector].view[Current_View].alarms_radiation.info.feature.enable)										// check if radiation alarm enabled (9DP v1.01.04)
//			{
//				Present_Level = ALARM;																												// enable alarm audio
//				if(lmi.display.detector[Current_Detector].view[Current_View].alarms_radiation.info.feature.custom)									// check if custom message enabled (9DP v1.01.04)
//				{
//					Custom_Error_Pending = TRUE;																									// waiting on user acknowledgement
//					strncpy(ErrorMsg, lmi.display.detector[Current_Detector].view[Current_View].alarms_radiation.info.feature.custom_message, 50);	// (9DP v1.01.04)
//					ErrorMsg[50] = '\0';																											// (9DP v1.01.04)
//					Use_Custom_Message(ErrorMsg, MSG_H_E_F_HEADING, ALARM_RADIATION, CENTER_JUSTIFY);												// arrange custom message
//				}
//				else 																																// custom is disabled -- use default message (no screen box used)
//				{
//					message_event = ALARM;
//				}
//			}
//			else																																	// if alerts are disabled
//			{		
//				Present_Level = NORMAL;
//			}
//			break;
//		}
//		case ALARM_INTEGRATE:
//		{
//			if(lmi.display.detector[Current_Detector].view[Current_View].alarms_integrate.info.feature.enable)										// check if integrate alarm enabled (9DP v1.01.04)
//			{
//				Present_Level = ALARM;																												// enable alarm audio
//				if(lmi.display.detector[Current_Detector].view[Current_View].alarms_integrate.info.feature.custom)									// check if custom message enabled (9DP v1.01.04)
//				{
//					Custom_Error_Pending = TRUE;																									// waiting on user acknowledgement
//					strncpy(ErrorMsg, lmi.display.detector[Current_Detector].view[Current_View].alarms_integrate.info.feature.custom_message, 50);	// (9DP v1.01.04)
//					ErrorMsg[50] = '\0';																											// (9DP v1.01.04)
//					Use_Custom_Message(ErrorMsg, MSG_H_E_HEADING, ALARM_INTEGRATE, CENTER_JUSTIFY);													// arrange custom message
//				}
//				else 																																// custom is disabled -- use default message (no screen box used)
//				{
//					message_event = ALARM;
//				}
//			}
//			else																																	// if alerts are disabled
//			{		
//				Present_Level = NORMAL;
//			}
//			break;
//		}
//		case ALARM_FUNCTION:
//		{
//			if(lmi.display.detector[Current_Detector].view[Current_View].alarms_function.info.feature.enable)										// (9DP v1.01.04)
//			{
//				Present_Level = ALARM;																												// enable alarm audio
//				if(lmi.display.detector[Current_Detector].view[Current_View].alarms_function.info.feature.custom)									// (9DP v1.01.04)
//				{
//					Custom_Error_Pending = TRUE;																									// waiting on user acknowledgement
//					strncpy(ErrorMsg, lmi.display.detector[Current_Detector].view[Current_View].alarms_function.info.feature.custom_message, 50);	// (9DP v1.01.04)
//					ErrorMsg[50] = '\0';																											// (9DP v1.01.04)
//					Use_Custom_Message(ErrorMsg, MSG_H_E_HEADING, ALARM_FUNCTION, CENTER_JUSTIFY);													// arrange custom message
//				}
//				else 																																// custom is disabled -- use default message (no screen box used)
//				{
//					message_event = ALARM;
//				}
//			}
//			else																																	// if alerts are disabled
//			{	
//				Present_Level = NORMAL;
//			}
//			break;
//		}		
//		case ALARM_LOW_BATTERY:
//		{
//			break;
//		}
//		case WARNING_SHUTDOWN:
//		{
//			Custom_Error_Pending = TRUE;	
//			sprintf(ErrorMsg, "Low Battery - Shutting down\0");																						// format string and save to array
//			Use_Custom_Message(ErrorMsg, MSG_H_E_F_HEADING, WARNING_SHUTDOWN, CENTER_JUSTIFY);														// arrange custom message
//			break;
//		}
//		case ERROR_FILE_SYSTEM:
//		case ERROR_FILE_OPEN:
//		{
//			system_error_tick = tick + ((DWORD) TICKS_PER_SEC * 5);																					// calculate error message display time (9DP v1.01.05)
//			System_Error_Flag = TRUE;																												// set system error flag
//			File_Error_Message(Error_Buffer[Error_Last]);																							// call USB file error processing function
//			break;
//		}
//		default:
//		{
//			if(Error_Buffer[Error_Last] >= USB_ERROR_MSG)																							// determine if the error is USB related
//			{
//				system_error_tick = tick + ((DWORD) TICKS_PER_SEC * 5);																				// calculate error message display time (9DP v1.01.05)
//				System_Error_Flag = TRUE;																											// set system error flag
//				USB_Error_Message(Error_Buffer[Error_Last]);																						// call USB error processing function
//			}
//			break;
//		}	
//	}
//
//	if(Custom_Error_Pending == TRUE)
//	{
//		Flag_Custom_Msg_Enabled = TRUE;																												// set flag to indicate custom message placed
//		Flag_Status_Text_Displayed = FALSE;																											// clear flag to indicate text
//	}
//	else
//	{
//		Flag_Custom_Msg_Enabled = FALSE;																											// clear flag to indicate custom message not placed
//		if(PreviousScreenState == MONITOR_RADIATION)																								// (9DP v1.01.04)
//		{
//			if(Present_Level != NORMAL)																												// (9DP v1.01.04)
//			{
//				DisplayTopRightText(message_event);																									// (9DP v1.01.04)
//				Flag_Status_Text_Displayed = TRUE;																									// set flag to indicate status text placed
//			}
//		}	
//		clear_error_message_off_queue();						
//	}
//
//	Set_Audio_Tone();																																// update audio tone settings
//
//	return;
//}
//
//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Use_Custom_Message(unsigned char* custom_message, UINT8 msg_location, UINT8 msg_condition, UINT8 text_alignment)
//*
//*	Description:
//*		The primary use of this function is when a user defined string is used.
//*		- 	This function checks the passed string information and determines the amount of text that
//*			must be moved to the next line to insure the text fits on the screen.
//*		- 	Once the info is assigned to lines the Display_Message() function is called
//*		-	If the text to display in the message box never changes and the designated line number is
//*			known, use the Display_Message() function directly instead of this function
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

void Use_Custom_Message(unsigned char* custom_message, UINT8 msg_location, UINT8 msg_condition, UINT8 text_alignment)
{
	SHORT i, strWidth, str2Width, str3Width, org_str_chars, first_str_chars, second_str_chars, third_str_chars;
	BOOL invalid_message = FALSE;
	unsigned char temp_line1_str[51] = {'\0'};
	unsigned char temp2_line1_str[51] = {'\0'};
	unsigned char temp3_line1_str[51] = {'\0'};
	unsigned char temp_line2_str[51] = {'\0'};
	unsigned char temp2_line2_str[51] = {'\0'};
	unsigned char temp_line3_str[51] = {'\0'};
	unsigned char *ptrline2;
	unsigned char *ptrline3;
	unsigned char invalid_user_msg[] = {"Invalid User Message Entered!"};

	XCHAR tempXmsg[51] = {'\0'};
	const SHORT max_screen_x = 291;


	pmsgScreen.txt_color_line[1]		=	BLACK;
	pmsgScreen.font[1]					= 	(void*)&GOLFontDefault;

	Convert_For_Display(tempXmsg, NULL, custom_message, 50);

	strWidth = GetTextWidth((XCHAR*)tempXmsg, (void*)&GOLFontDefault);	
	
	if(strWidth <= max_screen_x)																	// check if string is too long for 1 line
	{
		pmsgScreen.txt_total_lines = 1;																// set message for only 1 text line
		Convert_For_Display(pmsgScreen.txt_text[1], NULL, custom_message, 50);
	}
	else																							// message is too long for 1 line -- break apart onto several lines
	{
		pmsgScreen.txt_color_line[2]		=	BLACK;
		pmsgScreen.font[2]					= 	(void*)&GOLFontDefault;

		strcpy(temp_line1_str, custom_message);														// copy character string to variable
		org_str_chars = strlen(temp_line1_str);														// get the number of characters in the original string

		while((strWidth > max_screen_x) && (invalid_message == FALSE))
		{
			ptrline2 = strrchr(temp_line1_str, ' ');												// get message remaining after last space betweeen words 
			strcpy(temp2_line2_str, ptrline2);														// copy string from pointer to character array
			second_str_chars = strlen(temp_line2_str);												// get the character length in the second string
			strncat(temp2_line2_str, temp_line2_str, second_str_chars);								// add removed words together for line 2 text

			strcpy(temp_line2_str, temp2_line2_str);												// copy new combined line 2 message to variable
			second_str_chars = strlen(temp_line2_str);												// get new length of second string
			first_str_chars = (org_str_chars - second_str_chars);									// recalculate line 1 character number
			strncpy(temp2_line1_str, temp_line1_str, first_str_chars);								// readjust the character array to remove the character now on line 2

			for(i=(first_str_chars); i<org_str_chars; i++)											// cycle through array appending NULL to remaining characters
			{
				temp2_line1_str[i] = '\0';															// add NULL
			}

			strcpy(temp_line1_str, temp2_line1_str);												// copy string info back into original variable
			Convert_For_Display(tempXmsg, NULL, temp_line1_str, 50);								// convert char string to XCHAR string so width can be checked
			strWidth = GetTextWidth((XCHAR*)tempXmsg, (void*)&GOLFontDefault);						// check message line 1 

			if(first_str_chars < 10)
			{
				invalid_message == TRUE;
			}
		}

		for(i=0; i<second_str_chars; i++)															// cycle through array shifting characters over 1 space
		{
			temp_line2_str[i] = temp_line2_str[i+1];								
		}
		
		Convert_For_Display(tempXmsg, NULL, temp_line2_str, 50);									// convert char string to XCHAR string so width can be checked
		str2Width = GetTextWidth((XCHAR*)tempXmsg, (void*)&GOLFontDefault);							// check message line 2 width

		if((strWidth <= max_screen_x) && (str2Width <= max_screen_x))								// if line 1 and line 2 not too long, then save line 1 and line 2 message info
		{
			pmsgScreen.txt_total_lines = 2;															// set for 2 lines
			Convert_For_Display(pmsgScreen.txt_text[1], NULL, temp_line1_str, 50);					// save info for line 1 string (converts to XCHAR)
			Convert_For_Display(pmsgScreen.txt_text[2], NULL, temp_line2_str, 50);					// save info for line 2 string (converts to XCHAR)
		}
		else																						// Line 2 is too long, repeat break down process on line 2 and move to line 3
		{
			while((str2Width > max_screen_x) && (invalid_message == FALSE))
			{
				invalid_message == TRUE;
			}
		}
		
		if(invalid_message == TRUE)
		{
			pmsgScreen.txt_total_lines = 1;															// set message for only 1 text line
			Convert_For_Display(pmsgScreen.txt_text[1], NULL, invalid_user_msg, 50);				// save info for line 1 string (converts to XCHAR)
		}
	}
	
	draw_soft_buttons((XCHAR *)NULL, (XCHAR *)NULL, (XCHAR *)NULL, (XCHAR *)Ok);
	Display_Message(&pmsgScreen, msg_location, msg_condition, text_alignment);						// Display the message box with the instrument information

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void USB_Error_Message(UINT8 condition)
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

void USB_Error_Message(UINT8 condition)
{
	unsigned char stringconversion[51] = {'\0'};

	pmsgScreen.txt_color_line[1]		=	BLACK;													// set line colors
	pmsgScreen.txt_color_line[2]		=	BLACK;							
	pmsgScreen.font[1]					= 	(void*)&GOLFontDefault;									// Set Fonts
	pmsgScreen.font[2]					= 	(void*)&GOLFontDefault;
	pmsgScreen.box_background_color		=	WHITE;

	switch(condition)
	{
		case USB_ERROR_MSG:
		{
			sprintf(stringconversion, "USB ERROR\0");
			Convert_For_Display(pmsgScreen.txt_text[1], NULL, stringconversion, 50);
			pmsgScreen.txt_total_lines	 =	1;														// set number of lines to write
			Display_Message(&pmsgScreen, MSG_I_NO_HEADING, condition, CENTER_JUSTIFY);				// Display the message box with the USB ERROR info
			break;
		}
		case USB_ERROR_HUB:
		{
			sprintf(stringconversion, "Hubs Not Supported\0");
			Convert_For_Display(pmsgScreen.txt_text[1], NULL, stringconversion, 50);
			pmsgScreen.txt_total_lines	 =	1;														// set number of lines to write
			Display_Message(&pmsgScreen, MSG_I_NO_HEADING, condition, CENTER_JUSTIFY);				// Display the message box with the USB ERROR info
			break;
		}
		case USB_ERROR_NO_DEVICE_SUPPORT:
		{
			sprintf(stringconversion, "Device Not Supported\0");
			Convert_For_Display(pmsgScreen.txt_text[1], NULL, stringconversion, 50);
			pmsgScreen.txt_total_lines	 =	1;														// set number of lines to write
			Display_Message(&pmsgScreen, MSG_I_NO_HEADING, condition, CENTER_JUSTIFY);				// Display the message box with the USB ERROR info
			break;
		}
		case USB_ERROR_ENUMERATION:
		{
			sprintf(stringconversion, "Cannot Enumerate Device\0");
			Convert_For_Display(pmsgScreen.txt_text[1], NULL, stringconversion, 50);
			pmsgScreen.txt_total_lines	 =	1;														// set number of lines to write
			Display_Message(&pmsgScreen, MSG_I_NO_HEADING, condition, CENTER_JUSTIFY);				// Display the message box with the USB ERROR info
			break;
		}
		case USB_ERROR_CLIENT_INIT:
		{
			sprintf(stringconversion, "Client Driver Initialization Error\0");
			Convert_For_Display(pmsgScreen.txt_text[1], NULL, stringconversion, 50);
			pmsgScreen.txt_total_lines	 =	1;														// set number of lines to write
			Display_Message(&pmsgScreen, MSG_I_NO_HEADING, condition, CENTER_JUSTIFY);				// Display the message box with the USB ERROR info
			break;
		}
		case USB_ERROR_NO_MEMORY:
		{
			sprintf(stringconversion, "Out of Memory\0");
			Convert_For_Display(pmsgScreen.txt_text[1], NULL, stringconversion, 50);
			pmsgScreen.txt_total_lines	 =	1;														// set number of lines to write
			Display_Message(&pmsgScreen, MSG_I_NO_HEADING, condition, CENTER_JUSTIFY);				// Display the message box with the USB ERROR info
			break;
		}
		case USB_ERROR_UNSPECIFIED:
		{
			sprintf(stringconversion, "Unspecified USB Error\0");
			Convert_For_Display(pmsgScreen.txt_text[1], NULL, stringconversion, 50);
			pmsgScreen.txt_total_lines	 =	1;														// set number of lines to write
			Display_Message(&pmsgScreen, MSG_I_NO_HEADING, condition, CENTER_JUSTIFY);				// Display the message box with the USB ERROR info
			break;
		}
		case USER_DEFINED:
		{
			Convert_For_Display(pmsgScreen.txt_text[1], NULL, DisplayMessage, 50);
			pmsgScreen.txt_total_lines	 =	1;														// set number of lines to write
			Display_Message(&pmsgScreen, MSG_I_NO_HEADING, condition, CENTER_JUSTIFY);				// Display the message box with the USB ERROR info
			break;
		}
		default:
		{
			System_Error_Flag = FALSE;																// display nothing
			break;
		}
	}

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void File_Error_Message(UINT8 condition)
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

void File_Error_Message(UINT8 condition)
{
	unsigned char stringconversion[51] = {'\0'};

	pmsgScreen.txt_color_line[1]		=	BLACK;													// set line colors
	pmsgScreen.txt_color_line[2]		=	BLACK;							
	pmsgScreen.font[1]					= 	(void*)&GOLFontDefault;									// Set Fonts
	pmsgScreen.font[2]					= 	(void*)&GOLFontDefault;
	pmsgScreen.box_background_color		=	WHITE;

	switch(condition)
	{
		case ERROR_FILE_SYSTEM:
		{
			sprintf(stringconversion, "Thumbdrive FS Error");
			Convert_For_Display(pmsgScreen.txt_text[1], NULL, stringconversion, 50);
			pmsgScreen.txt_total_lines	 =	1;														// set number of lines to write
			Display_Message(&pmsgScreen, MSG_I_NO_HEADING, condition, CENTER_JUSTIFY);				// Display the message box with the USB ERROR info
			break;
		}
		case ERROR_FILE_OPEN:
		{
			sprintf(stringconversion, "Unable To Open File");
			Convert_For_Display(pmsgScreen.txt_text[1], NULL, stringconversion, 50);
			pmsgScreen.txt_total_lines	 =	1;														// set number of lines to write
			Display_Message(&pmsgScreen, MSG_I_NO_HEADING, condition, CENTER_JUSTIFY);				// Display the message box with the USB ERROR info
			break;
		}
		default:
		{
			System_Error_Flag = FALSE;																// display nothing
			break;
		}
	}

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void draw_soft_buttons(XCHAR *pTextbtnA, XCHAR *pTextbtnB, XCHAR *pTextbtnC, XCHAR *pTextbtnD)
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

void draw_soft_buttons(XCHAR *pTextbtnA, XCHAR *pTextbtnB, XCHAR *pTextbtnC, XCHAR *pTextbtnD)
{
	WORD state;
	
	XCHAR btn_1_str[7] = {'\0'};
	XCHAR btn_2_str[7] = {'\0'};
	XCHAR btn_3_str[7] = {'\0'};
	XCHAR btn_4_str[7] = {'\0'};

	Clear_Icon_Window();

    // set state to monitor buttons
    // set state to let monitor buttons know that the buttons have different meanings instead of their normal functions.

    if(pTextbtnA == NULL)
        state = BTN_DISABLED|BTN_HIDE;
	else
		state = BTN_DRAW;

	pbtnSoftbutton1 =	BtnCreate(ID_BUTTON_A,
              			SOFT_BTN1_XSTART,
              			SOFT_BTN1_YSTART,
              			SOFT_BTN1_XSTART+SOFTKEY_BTN_WIDTH,
              			SOFT_BTN1_YSTART+SOFTKEY_BTN_HEIGHT,
              			10,
              			state,
              			NULL,
              			pTextbtnA,
              			btnScheme);

    if(pTextbtnB == NULL)
        state = BTN_DISABLED|BTN_HIDE;
	else
	    state = BTN_DRAW;

    pbtnSoftbutton2 = 	BtnCreate(ID_BUTTON_B,
              			SOFT_BTN2_XSTART,
              			SOFT_BTN2_YSTART,
              			SOFT_BTN2_XSTART+SOFTKEY_BTN_WIDTH,
              			SOFT_BTN2_YSTART+SOFTKEY_BTN_HEIGHT,
              			10,
              			state,
              			NULL,
              			pTextbtnB,
              			btnScheme);

    if(pTextbtnC == NULL)
        state = BTN_DISABLED|BTN_HIDE;
	else
	    state = BTN_DRAW;

    pbtnSoftbutton3 = 	BtnCreate(ID_BUTTON_C,
              			SOFT_BTN3_XSTART,
              			SOFT_BTN3_YSTART,
              			SOFT_BTN3_XSTART+SOFTKEY_BTN_WIDTH,
		              	SOFT_BTN3_YSTART+SOFTKEY_BTN_HEIGHT,
		              	10,
		              	state,
		              	NULL,															
		              	pTextbtnC,
		              	btnScheme);

    if(pTextbtnD == NULL)
        state = BTN_DISABLED|BTN_HIDE;
	else
    	state = BTN_DRAW;

    pbtnSoftbutton4 = 	BtnCreate(ID_BUTTON_D,
              			SOFT_BTN4_XSTART,
              			SOFT_BTN4_YSTART,
              			SOFT_BTN4_XSTART+SOFTKEY_BTN_WIDTH,
              			SOFT_BTN4_YSTART+SOFTKEY_BTN_HEIGHT,
              			10,
              			state,
              			NULL,															
              			pTextbtnD,
              			btnScheme);  

     return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	WORD UserDefinedMsgCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
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

WORD UserDefinedMsgCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
{
	switch(GetObjID(pObj))
	{
		case ID_BUTTON_A:
		{
			if (objMsg == BTN_MSG_RELEASED) 														// check if button is pressed
			{ 				
				// do not process if user moved the touch away from the button
           		// returning 1 wil update the button
           		if (pMsg->uiEvent == EVENT_MOVE)
           			return 1;	        	
    
				return 1;
		    } 
		}
		case ID_BUTTON_B:   
		{
			if (objMsg == BTN_MSG_RELEASED) 
			{
	        	if (pMsg->uiEvent == EVENT_MOVE) 													// EVENT_MOVE
				{			
					return 1;
				}
		       	if (pMsg->uiEvent == BTN_PRESSED) 													// was EVENT_PRESS
				{   

			    }
		       	else if (pMsg->uiEvent == EVENT_RELEASE) 
				{
	
				}
		
	        return 1;
			}
		}
		case ID_BUTTON_C:
		{
			if (objMsg == BTN_MSG_RELEASED) 														// check if button is pressed
			{ 				
				// do not process if user moved the touch away from the button
           		// returning 1 wil update the button
           		if (pMsg->uiEvent == EVENT_MOVE)
           			return 1;	        				
    	    
			return 1;
			} 
		}
		case ID_BUTTON_D:
		{
			if (objMsg == BTN_MSG_RELEASED) 														// check if button is pressed
			{ 				
				// do not process if user moved the touch away from the button
           		// returning 1 wil update the button

			    clear_error_message_off_queue();
				SetColor(WHITE);	

				SetState(pbtnSoftbutton1, BTN_REMOVE);												// BTN_REMOVE
				GOLDraw(); 

				SetState(pbtnSoftbutton2, BTN_REMOVE);
				GOLDraw();

				SetState(pbtnSoftbutton3, BTN_REMOVE);
				GOLDraw();

				SetState(pbtnSoftbutton4, BTN_REMOVE);
				GOLDraw();

				screenState = CREATE_ELEMENT;
				REFRESH_LEVEL = TRUE;
	
				return 1;
    	    }  	
		}
	}

	return 1;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void ClearHeaderRegion(void)
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

void ClearHeaderRegion(void)
{
	SetColor(background);
    while(Bar(0, 0, GetMaxX(), HEADER_HEIGHT) == 0);
    MoveTo(0, 0);
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void ClearTopRightText(void)
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

void ClearTopRightText(void)
{
	SetColor(background);
	while(Bar(STATUS_REGION_XSTART, STATUS_REGION_YSTART, STATUS_REGION_XFINISH, STATUS_REGION_YFINISH) == 0);
	MoveTo(0, 0);
	Flag_Status_Text_Displayed = FALSE;
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void DisplayTopRightText(void)
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

void DisplayTopRightText(BYTE message)
{
	BYTE ErrorMsg[10] = {'\0'};																		// declare error message char array
	XCHAR tempXmsg[10] = {'\0'};
	SHORT msgwidth;

	switch(message)
	{
		case ALERT:
		{
			ClearTopRightText();																	// clear previous event if it still exists
			SetColor(ALERT_ARC_COLOR);																// set color to match element color
			sprintf(ErrorMsg, "ALERT");																// format string and save to array
			break;
		}
		case ALARM:
		{
			ClearTopRightText();																	// clear previous event if it still exists
			SetColor(ALARM_ARC_COLOR);																// set color to match element color
			sprintf(ErrorMsg, "ALARM");																// format string and save to array
			break;
		}
		case NORMAL:
		default:
		{
			return;
		}
	}

	SetFont((void*)&GOLFontDefault);																// set font
	Convert_For_Display(tempXmsg, NULL, ErrorMsg, 10);												// convert string to type XCHAR
	msgwidth = GetTextWidth((XCHAR*)tempXmsg, (void*)&GOLFontDefault);								// get text width
	msgwidth = (STATUS_REGION_XFINISH - msgwidth);													// adjust position for top right of screen
	MoveTo(msgwidth,STATUS_REGION_YSTART);															// move location to display top right
	while(!OutText((XCHAR*)tempXmsg));																// place text on LCD
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void ClearTopLeftText(void)
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

void ClearTopLeftText(void)
{
	SetColor(background);
	while(Bar(NOTE_REGION_XSTART, NOTE_REGION_YSTART, NOTE_REGION_XFINISH, NOTE_REGION_YFINISH) == 0);
	MoveTo(0, 0);
	Flag_Note_Text_Displayed = FALSE;
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void DisplayTopLeftText(void)
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

void DisplayTopLeftText(BYTE message)
{
	BYTE ErrorMsg[10] = {'\0'};																			// declare error message char array
	XCHAR tempXmsg[10] = {'\0'};

	switch(message)
	{
		case 1:
		{
			ClearTopLeftText();																			// clear previous event if it still exists
			SetColor(ALERT_ARC_COLOR);																	// set color to match element color
			sprintf(ErrorMsg, "ALERT");																	// format string and save to array
			break;
		}
		case 2:
		{
			ClearTopLeftText();																			// clear previous event if it still exists
			SetColor(ALARM_ARC_COLOR);																	// set color to match element color
			sprintf(ErrorMsg, "ALARM");																	// format string and save to array
			break;
		}
		case NORMAL:
		default:
		{
			return;
		}
	}

	SetFont((void*)&GOLFontDefault);																	// set font
	Convert_For_Display(tempXmsg, NULL, ErrorMsg, 10);													// convert string to type XCHAR
	MoveTo(NOTE_REGION_XSTART, NOTE_REGION_YSTART);														// move location to display top left
	while(!OutText((XCHAR*)tempXmsg));																	// place text on LCD
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void DisplaySystemProcess(BYTE process)
//*
//*	Description:
//* 	This routine will clear the LCD and display a system process message.
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//*		process - process that should be performed
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	Valid values for process:
//*
//*			0	- black screen
//*			1	- erasing EEPROM - primary
//*			2	- erasing EEPROM - secondary
//*			3	- creating backup
//*			4	- restoring backup
//*

void DisplaySystemProcess(BYTE process)
{
	BYTE char_buffer[25] = {0};
	XCHAR xchar_buffer[25] = {0};
	SHORT msgwidth;

	if(process == 0)
	{
		SetColor(BLACK);
		ClearDevice();
	}
	else
	{
		SetColor(WHITE);
		ClearDevice();
		SetColor(BLACK);
		SetFont((void*)&Secondary_Arial_Black_14);
		lmi.controls.audio.info.operating_mode = AUDIO_0;											// (9DP v1.01.12)
		Set_Audio_Tone();																			// (9DP v1.01.12)
		Check_Audio_Status();																		// (9DP v1.01.12)
		switch(process)
		{
			case 1:
			{
				sprintf(char_buffer, "Erasing EEPROM\0");
				Convert_For_Display(xchar_buffer, NULL, char_buffer, 25);
				msgwidth = GetTextWidth((XCHAR*) xchar_buffer, (void*)&Secondary_Arial_Black_14);
				msgwidth = (320 - msgwidth) / 2;
				MoveTo(msgwidth, 80);
				while(!OutText((XCHAR*)xchar_buffer));
				sprintf(char_buffer, "(primary)\0");
				Convert_For_Display(xchar_buffer, NULL, char_buffer, 25);
				msgwidth = GetTextWidth((XCHAR*) xchar_buffer, (void*)&Secondary_Arial_Black_14);
				msgwidth = (320 - msgwidth) / 2;
				MoveTo(msgwidth, 120);
				while(!OutText((XCHAR*)xchar_buffer));
				break;
			}
			case 2:
			{
				sprintf(char_buffer, "Erasing EEPROM\0");
				Convert_For_Display(xchar_buffer, NULL, char_buffer, 25);
				msgwidth = GetTextWidth((XCHAR*) xchar_buffer, (void*)&Secondary_Arial_Black_14);
				msgwidth = (320 - msgwidth) / 2;
				MoveTo(msgwidth, 80);
				while(!OutText((XCHAR*)xchar_buffer));
				sprintf(char_buffer, "(secondary)\0");
				Convert_For_Display(xchar_buffer, NULL, char_buffer, 25);
				msgwidth = GetTextWidth((XCHAR*) xchar_buffer, (void*)&Secondary_Arial_Black_14);
				msgwidth = (320 - msgwidth) / 2;
				MoveTo(msgwidth, 120);
				while(!OutText((XCHAR*)xchar_buffer));
				break;
			}
			case 3:
			{
				sprintf(char_buffer, "Creating Backup\0");
				Convert_For_Display(xchar_buffer, NULL, char_buffer, 25);
				msgwidth = GetTextWidth((XCHAR*) xchar_buffer, (void*)&Secondary_Arial_Black_14);
				msgwidth = (320 - msgwidth) / 2;
				MoveTo(msgwidth, 80);
				while(!OutText((XCHAR*)xchar_buffer));
				break;
			}
			case 4:
			{
				sprintf(char_buffer, "Restoring Backup\0");
				Convert_For_Display(xchar_buffer, NULL, char_buffer, 25);
				msgwidth = GetTextWidth((XCHAR*) xchar_buffer, (void*)&Secondary_Arial_Black_14);
				msgwidth = (320 - msgwidth) / 2;
				MoveTo(msgwidth, 80);
				while(!OutText((XCHAR*)xchar_buffer));
				break;
			}
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
