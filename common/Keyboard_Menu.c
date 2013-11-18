//**************************************************************************************************
//**************************************************************************************************
//*																		
//*	Filename:		Keyboard_Menu.c
//*	Date:			09/07/10
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
//*	Revision 1	12/21/11	James Comstock
//*	- added Function routines
//*	- removed unused variables
//*	- created display function to reduce redundant code
//*
//*	Revision 0	09/07/10	James Comstock
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
//* THIS SOFTWARE IS PROVIDED IN AN "AS IS" CTRUEDITITRUE. NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR
//*	STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//*	A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT, IN ANY CIRCUMSTANCES, BE
//*	LIABLE FOR SPECIAL, INCIDENTAL OR CTRUESEQUENTIAL DAMAGES, FOR ANY REASTRUE WHATSOEVER.
//*

#include "Graphics.h"
#include "instrument.h"
#include "Definitions.h"
#include "System.h"
#include "Keyboard_Menu.h"

//**************************************************************************************************
//**************************************************************************************************
//*	External Variables
//**************************************************************************************************
//**************************************************************************************************

extern DWORD tick;

extern WORD foreground;
extern WORD background;

extern TYPE_MESSAGE_BOX pmsgScreen;
extern SCREEN_STATES screenState;
extern SCREEN_STATES nextscreenState;
extern BITMAP_FLASH LB_Down_Arrow;
extern BITMAP_FLASH LB_Up_Arrow;

extern BYTE Backlight_Mode;
extern BYTE Current_Detector;

//extern BYTE menu_line[];																			// (9DP v1.01.01)

extern char Total_Chars;
extern char Last_Total_Chars;
extern char DisplayMessage[29];

#ifdef TEST_MODE

extern BOOL Test_Update;																			// (Test v0.00.03)

#endif

//**************************************************************************************************
//**************************************************************************************************
//*	External - Structures
//**************************************************************************************************
//**************************************************************************************************

extern TYPE_INSTRUMENT lmi;

//**************************************************************************************************
//**************************************************************************************************
//*	Global Variables
//**************************************************************************************************
//**************************************************************************************************

volatile BOOL MenuSelect 						= FALSE;
volatile BOOL editScreen						= FALSE;
volatile BOOL editMode							= FALSE;

volatile unsigned char DetectorMenu				= 0;
volatile unsigned char ViewMenu					= 0;
volatile unsigned char ViewAlertsMenu			= 0;
volatile unsigned char ViewAlarmsMenu			= 0;
volatile unsigned char FunctionMenu				= 0;

volatile signed short MenuLine 					= 1;
volatile signed short SubMenuLine 				= 1;
volatile signed short LastMenuLine 				= 0;

volatile BYTE password_entry[17];

char MessageBack[] 	= "<- Back\0";
char MessageEnable[]= "Enable\0";

//**************************************************************************************************
//**************************************************************************************************
//*	Menu Routines
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateMenu(void)	
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

void CreateMenu(void)
{
	char stringconversion[29];

	pmsgScreen.txt_color_line[0]		=	'\0';
	pmsgScreen.txt_color_line[1]		=	LIGHTGRAY;
	pmsgScreen.txt_color_line[2]		=	LIGHTGRAY;
	pmsgScreen.txt_color_line[3]		=	LIGHTGRAY;
	pmsgScreen.txt_color_line[4]		=	LIGHTGRAY;
	pmsgScreen.txt_color_line[5]		=	LIGHTGRAY;
	pmsgScreen.txt_color_line[6]		=	LIGHTGRAY;
	pmsgScreen.txt_color_line[7]		=	LIGHTGRAY;
	pmsgScreen.txt_color_line[8]		=	LIGHTGRAY;
	pmsgScreen.txt_color_line[9]		=	LIGHTGRAY;

	pmsgScreen.font[0]					= 	'\0';
	pmsgScreen.font[1]					= 	(void*)&GOLFontDefault;
	pmsgScreen.font[2]					= 	(void*)&GOLFontDefault;
	pmsgScreen.font[3]					= 	(void*)&GOLFontDefault;
	pmsgScreen.font[4]					= 	(void*)&GOLFontDefault;
	pmsgScreen.font[5]					= 	(void*)&GOLFontDefault;
	pmsgScreen.font[6]					= 	(void*)&GOLFontDefault;
	pmsgScreen.font[7]					= 	(void*)&GOLFontDefault;
	pmsgScreen.font[8]					= 	(void*)&GOLFontDefault;
	pmsgScreen.font[9]					= 	(void*)&GOLFontDefault;

	pmsgScreen.box_background_color		=	WHITE;

	DetectorMenu = Current_Detector;

	switch(screenState)
	{
// INSTRUMENT
		case CREATE_MENU:
		{
			CreateMainMenu();
			nextscreenState = DISPLAY_MENU;
			break;
		}
// INSTRUMENT - SETTINGS
		case CREATE_SETTINGS:
		{
			CreateSettingsMenu();
			nextscreenState = DISPLAY_SETTINGS;
			break;
		}
// INSTRUMENT - SETTINGS - PRODUCTS
		case CREATE_PRODUCT:
		{
			CreateProductMenu();
			nextscreenState = DISPLAY_PRODUCT;
			break;
		}
// INSTRUMENT - SETTINGS - LANGUAGE
		case CREATE_LANGUAGE:
		{
			CreateLanguageMenu();
			nextscreenState = DISPLAY_LANGUAGE;
			break;
		}
// INSTRUMENT - SETTINGS - CALENDAR
		case CREATE_CALENDAR:
		{
			CreateCalendarMenu();
			nextscreenState = DISPLAY_CALENDAR;
			break;
		}
// INSTRUMENT - SETTINGS - CLOCK
		case CREATE_CLOCK:
		{
			CreateClockMenu();
			nextscreenState = DISPLAY_CLOCK;
			break;
		}
// INSTRUMENT - SETTINGS - PASSWORD
		case CREATE_PASSWORD:
		{
			CreatePasswordMenu();
			nextscreenState = DISPLAY_PASSWORD;
			break;
		}
// INSTRUMENT - SETTINGS - FIRMWARE
		case CREATE_SYSTEM:
		{
			CreateSystemMenu();
			nextscreenState = DISPLAY_SYSTEM;
			break;
		}
// INSTRUMENT - FEATURES
		case CREATE_FEATURES:
		{
			CreateFeaturesMenu();
			nextscreenState = DISPLAY_FEATURES;
			break;
		}
// INSTRUMENT - FEATURES - SOURCE CHECK
		case CREATE_SOURCE_CHECK:
		{
			CreateSourceCheckMenu();
			nextscreenState = DISPLAY_SOURCE_CHECK;
			break;
		}
// INSTRUMENT - FEATURES - BACKGROUND CHECK
		case CREATE_BACKGROUND_CHECK:
		{
			CreateBackgroundCheckMenu();
			nextscreenState = DISPLAY_BACKGROUND_CHECK;
			break;
		}
// INSTRUMENT - FEATURES - INTEGRATE
		case CREATE_INTEGRATE:
		{
			CreateIntegrateMenu();
			nextscreenState = DISPLAY_INTEGRATE;
			break;
		}
// INSTRUMENT - FEATURES - INTEGRATE - REPORT
		case CREATE_INTEGRATE_REPORT:
		{
			CreateIntegrateReportMenu();
			nextscreenState = DISPLAY_INTEGRATE_REPORT;
			break;
		}	
// INSTRUMENT - FEATURES - INTEGRATE - VALUE
		case CREATE_INTEGRATE_VALUE:
		{
			CreateIntegrateValueMenu();
			nextscreenState = DISPLAY_INTEGRATE_VALUE;
			break;
		}	
// INSTRUMENT - FEATURES - DATA LOGGING
		case CREATE_DATA_LOGGING:
		{
			CreateDataLoggingMenu();
			nextscreenState = DISPLAY_DATA_LOGGING;
			break;
		}
// INSTRUMENT - FEATURES - DATA LOGGING - REPORT
		case CREATE_DATA_LOGGING_REPORT:
		{
			CreateDataLoggingReportMenu();
			nextscreenState = DISPLAY_DATA_LOGGING_REPORT;
			break;
		}
// INSTRUMENT - FEATURES - DATA LOGGING - REPORT - SYSTEM
		case CREATE_DATA_LOGGING_REPORT_SYSTEM:
		{
			CreateDataLoggingReportSystemMenu();
			nextscreenState = DISPLAY_DATA_LOGGING_REPORT_SYSTEM;
			break;
		}
// INSTRUMENT - FEATURES - DATA LOGGING - REPORT - INSTRUMENT
		case CREATE_DATA_LOGGING_REPORT_INSTRUMENT:
		{
			CreateDataLoggingReportInstrumentMenu();
			nextscreenState = DISPLAY_DATA_LOGGING_REPORT_INSTRUMENT;
			break;
		}
// INSTRUMENT - FEATURES - DATA LOGGING - REPORT - READINGS
		case CREATE_DATA_LOGGING_REPORT_READINGS:
		{
			CreateDataLoggingReportReadingsMenu();
			nextscreenState = DISPLAY_DATA_LOGGING_REPORT_READINGS;
			break;
		}
// INSTRUMENT - CONTROLS
		case CREATE_CONTROLS:
		{
			CreateControlsMenu();
			nextscreenState = DISPLAY_CONTROLS;
			break;
		}
// INSTRUMENT - CONTROLS - BACKLIGHT
		case CREATE_BACKLIGHT:
		{
			CreateBacklightMenu();
			nextscreenState = DISPLAY_BACKLIGHT;
			break;
		}
// INSTRUMENT - CONTROLS - BACKLIGHT
		case CREATE_AUTO_BACKLIGHT:
		{
			CreateAutoBacklightMenu();
			nextscreenState = DISPLAY_AUTO_BACKLIGHT;
			break;
		}
// INSTRUMENT - CONTROLS - AUDIO
		case CREATE_AUDIO:
		{
			CreateAudioMenu();
			nextscreenState = DISPLAY_AUDIO;
			break;
		}
// INSTRUMENT - CONTROLS - GPS
		case CREATE_GPS:
		{
			CreateGPSMenu();
			nextscreenState = DISPLAY_GPS;
			break;
		}
// INSTRUMENT - CONTROLS - BLUETOOTH
		case CREATE_BLUETOOTH:
		{
			CreateBluetoothMenu();
			nextscreenState = DISPLAY_BLUETOOTH;
			break;
		}
#ifdef EEPROM_DETECTOR_PROPERTIES_LOCATITRUE
// INSTRUMENT - DETECTOR
		case CREATE_DETECTOR:
		{
			CreateDetectorMenu();
			nextscreenState = DISPLAY_DETECTOR;
			break;
		}
// INSTRUMENT - DETECTOR - SETUP
		case CREATE_DETECTOR_SETUP:
		{
			CreateDetectorSetupMenu();
			nextscreenState = DISPLAY_DETECTOR_SETUP;
			break;
		}
// INSTRUMENT - DETECTOR - DETECTOR 0
		case CREATE_DETECTOR_0:
		{
			DetectorMenu = 0;
			CreateDisplayMenu(CREATE_DETECTOR);
			nextscreenState = DISPLAY_DISPLAY;
			break;
		}
// INSTRUMENT - DETECTOR - DETECTOR 1
		case CREATE_DETECTOR_1:
		{
			DetectorMenu = 1;
			CreateDisplayMenu(CREATE_DETECTOR);
			nextscreenState = DISPLAY_DISPLAY;
			break;
		}
// INSTRUMENT - DETECTOR - DETECTOR 2
		case CREATE_DETECTOR_2:
		{
			DetectorMenu = 2;
			CreateDisplayMenu(CREATE_DETECTOR);
			nextscreenState = DISPLAY_DISPLAY;
			break;
		}
// INSTRUMENT - DETECTOR - DETECTOR 3
		case CREATE_DETECTOR_3:
		{
			DetectorMenu = 3;
			CreateDisplayMenu(CREATE_DETECTOR);
			nextscreenState = DISPLAY_DISPLAY;
			break;
		}
// INSTRUMENT - DETECTOR - DETECTOR 4
		case CREATE_DETECTOR_4:
		{
			DetectorMenu = 4;
			CreateDisplayMenu(CREATE_DETECTOR);
			nextscreenState = DISPLAY_DISPLAY;
			break;
		}
#endif
// INSTRUMENT - DISPLAY
		case CREATE_DISPLAY:
		{
			CreateDisplayMenu(CREATE_MENU);
			nextscreenState = DISPLAY_DISPLAY;
			break;
		}
// INSTRUMENT - DISPLAY - VIEWS SETUP
		case CREATE_DISPLAY_SETUP:
		{
			CreateDisplaySetupMenu();
			nextscreenState = DISPLAY_DISPLAY_SETUP;
			break;
		}
// INSTRUMENT - DISPLAY - VIEW 1
		case CREATE_VIEW_1:
		{
			ViewMenu = 1;
			CreateViewMenu();
			nextscreenState = DISPLAY_VIEW;
			break;
		}
// INSTRUMENT - DISPLAY - VIEW 2
		case CREATE_VIEW_2:
		{
			ViewMenu = 2;
			CreateViewMenu();
			nextscreenState = DISPLAY_VIEW;
			break;
		}
// INSTRUMENT - DISPLAY - VIEW 3
		case CREATE_VIEW_3:
		{
			ViewMenu = 3;
			CreateViewMenu();
			nextscreenState = DISPLAY_VIEW;
			break;
		}
// INSTRUMENT - DISPLAY - VIEW 4
		case CREATE_VIEW_4:
		{
			ViewMenu = 4;
			CreateViewMenu();
			nextscreenState = DISPLAY_VIEW;
			break;
		}
// INSTRUMENT - DISPLAY - VIEW x - APPEARANCE
		case CREATE_VIEW_APPEARANCE:
		{
			CreateViewAppearanceMenu();
			nextscreenState = DISPLAY_VIEW_APPEARANCE;
			break;
		}
// INSTRUMENT - DISPLAY - VIEW x - FUNCTIONS
		case CREATE_VIEW_FUNCTIONS:
		{
			CreateViewFunctionsMenu();
			nextscreenState = DISPLAY_VIEW_FUNCTIONS;
			break;
		}
// INSTRUMENT - DISPLAY - VIEW x - FUNCTION 1
		case CREATE_VIEW_FUNCTION_1:
		{
			FunctionMenu = 1;
			CreateViewFunctionMenu();
			nextscreenState = DISPLAY_VIEW_FUNCTION;
			break;
		}
// INSTRUMENT - DISPLAY - VIEW x - FUNCTION 2
		case CREATE_VIEW_FUNCTION_2:
		{
			FunctionMenu = 2;
			CreateViewFunctionMenu();
			nextscreenState = DISPLAY_VIEW_FUNCTION;
			break;
		}
// INSTRUMENT - DISPLAY - VIEW x - FUNCTION 3
		case CREATE_VIEW_FUNCTION_3:
		{
			FunctionMenu = 3;
			CreateViewFunctionMenu();
			nextscreenState = DISPLAY_VIEW_FUNCTION;
			break;
		}
// INSTRUMENT - DISPLAY - VIEW x - FUNCTION 4
		case CREATE_VIEW_FUNCTION_4:
		{
			FunctionMenu = 4;
			CreateViewFunctionMenu();
			nextscreenState = DISPLAY_VIEW_FUNCTION;
			break;
		}
// INSTRUMENT - DISPLAY - VIEW x - ALERTS
		case CREATE_VIEW_ALERTS:
		{
			CreateViewAlertsMenu();
			nextscreenState = DISPLAY_VIEW_ALERTS;
			break;
		}
// INSTRUMENT - DISPLAY - VIEW x - ALERTS - RADIATITRUE
// INSTRUMENT - DISPLAY - VIEW x - ALERTS - INTEGRATITRUE
// INSTRUMENT - DISPLAY - VIEW x - ALERTS - FUNCTION
		case CREATE_VIEW_ALERTS_RAD:
		case CREATE_VIEW_ALERTS_INT:
		case CREATE_VIEW_ALERTS_FUN:
		{
			CreateViewAlertsReadingMenu();
			nextscreenState = DISPLAY_VIEW_ALERTS_READING;
			break;
		}
// INSTRUMENT - DISPLAY - VIEW x - ALARMS
		case CREATE_VIEW_ALARMS:
		{
			CreateViewAlarmsMenu();
			nextscreenState = DISPLAY_VIEW_ALARMS;
			break;
		}
// INSTRUMENT - DISPLAY - VIEW x - ALARMS - RADIATITRUE
// INSTRUMENT - DISPLAY - VIEW x - ALARMS - INTEGRATITRUE
// INSTRUMENT - DISPLAY - VIEW x - ALARMS - FUNCTION
		case CREATE_VIEW_ALARMS_RAD:
		case CREATE_VIEW_ALARMS_INT:
		case CREATE_VIEW_ALARMS_FUN:
		{
			CreateViewAlarmsReadingMenu();
			nextscreenState = DISPLAY_VIEW_ALARMS_READING;
			break;
		}
// INSTRUMENT - ALERTS - ALARMS
		case CREATE_ALERTS_ALARMS:
		{
			CreateAlertsAlarmsMenu();
			nextscreenState = DISPLAY_ALERTS_ALARMS;
			break;
		}
// INSTRUMENT - ALERTS
		case CREATE_ALERTS:
		{
			CreateAlertsMenu();
			nextscreenState = DISPLAY_ALERTS;
			break;
		}
// INSTRUMENT - ALARMS
		case CREATE_ALARMS:
		{
			CreateAlarmsMenu();
			nextscreenState = DISPLAY_ALARMS;
			break;
		}
// INSTRUMENT - FUNCTIONS
		case CREATE_FUNCTIONS:
		{
			CreateFunctionsMenu();
			nextscreenState = DISPLAY_FUNCTIONS;
			break;
		}
// INSTRUMENT - CALIBRATION
		case CREATE_CALIBRATION:
		{
			CreateBlankMenu();
			nextscreenState = DISPLAY_BLANK_MENU;
			break;
		}
// INSTRUMENT - BLANK MENU (Under Construction)
		case CREATE_BLANK_MENU:
		{
			CreateBlankMenu();
			nextscreenState = DISPLAY_BLANK_MENU;
			break;
		}
// INSTRUMENT - LINE DISPLAY (NO EDIT)
		case CREATE_LINE:
		{
			CreateLine();
			nextscreenState = DISPLAY_LINE;
			break;
		}
// INSTRUMENT - HANDLES EDITABLE LINES
		case CREATE_EDIT_LINE:
		{
			CreateEditLine();
			nextscreenState = DISPLAY_EDIT_LINE;
			break;
		}
// INSTRUMENT - HANDLES EDITABLE STRINGS
		case CREATE_EDIT_STRING_LINE:
		{
			CreateEditStringLine();
			nextscreenState = DISPLAY_EDIT_STRING_LINE;
			break;
		}
// PASSWORD ENTRY
		case CREATE_PASSWORD_ENTRY:
		{
			CreatePasswordEntryMenu();
			nextscreenState = DISPLAY_PASSWORD_ENTRY;
			break;
		}
// PASSWORD RESPONSE
		case CREATE_PASSWORD_RESPONSE:
		{
			CreatePasswordResponseMenu();
			nextscreenState = DISPLAY_PASSWORD_RESPONSE;
			break;
		}
		default:
		{
			return;
		}
	}

	if((Backlight_Mode == BK_LIGHT_TEMP_ON) || (Backlight_Mode == BK_LIGHT_OFF))
	{
		Set_Backlight_Level(lmi.controls.backlight.info.on_level);
	}

	SetColor(WHITE);
	ClearDevice();
	Display_Message_Box(&pmsgScreen, MSG_FULL_HEADING, NORMAL_MESSAGE);
	Display_Message_Text(&pmsgScreen, MSG_FULL_HEADING, NORMAL_MESSAGE, LEFT_JUSTIFY);
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreatePasswordEntryMenu(void)
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

void CreatePasswordEntryMenu(void)
{
	char stringconversion[29];
	char current_line;
	char index;

	for(index = 0; index < LENGTH_PASSWORD; index++)
	{
		password_entry[index] = 0;
	}

	sprintf(DisplayMessage, "PASSWORD\0");
	pmsgScreen.parent_menu = CREATE_MENU;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 1;

	current_line = 1;

	sprintf(stringconversion, "");
	FormatLine(current_line++, stringconversion, 0, (void *)password_entry, TYPE_HIDDEN_STRING);

	pmsgScreen.minimum_value = 0;
	pmsgScreen.maximum_value = 16;
	Total_Chars = 0;
	Last_Total_Chars = -1;

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = TRUE;
	editMode = TRUE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreatePasswordResponseMenu(void)
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

void CreatePasswordResponseMenu(void)
{
	char stringconversion[29];
	char current_line;
	BYTE response;

	sprintf(DisplayMessage, "PASSWORD STATUS\0");
	pmsgScreen.parent_menu = CREATE_PASSWORD;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 2;

	current_line = 1;

//	sprintf(stringconversion,"%s\0",lmi.settings.passwords.info.password_3);																// (9DP v1.01.04)
	FormatString(stringconversion, lmi.settings.passwords.info.password_3, LENGTH_PASSWORD);												// (9DP v1.01.04)
	response = strcmp((char *)password_entry, stringconversion);
	if(response == 0)
	{
		sprintf(stringconversion, "Level 3 Approved\0");
		FormatLine(current_line++, stringconversion, CREATE_MENU, NULL, 0);
	}
	else
	{
//		sprintf(stringconversion,"%s\0",lmi.settings.passwords.info.password_2);															// (9DP v1.01.04)
		FormatString(stringconversion, lmi.settings.passwords.info.password_2, LENGTH_PASSWORD);											// (9DP v1.01.04)
		response = strcmp((char *)password_entry, stringconversion);
		if(response == 0)
		{
			sprintf(stringconversion, "Level 2 Approved\0");
			FormatLine(current_line++, stringconversion, CREATE_MENU, NULL, 0);
		}
		else
		{
//			sprintf(stringconversion,"%s\0",lmi.settings.passwords.info.password_1);														// (9DP v1.01.04)
			FormatString(stringconversion, lmi.settings.passwords.info.password_1, LENGTH_PASSWORD);										// (9DP v1.01.04)
			response = strcmp((char *)password_entry, stringconversion);
			if(response == 0)
			{
				sprintf(stringconversion, "Level 1 Approved\0");
				FormatLine(current_line++, stringconversion, CREATE_MENU, NULL, 0);
			}
			else
			{
				sprintf(stringconversion,"qwerty\0");
				response = strcmp((char *)password_entry, stringconversion);
				if(response == 0)
				{
					sprintf(stringconversion, "Service Approved\0");
					FormatLine(current_line++, stringconversion, CREATE_MENU, NULL, 0);
				}
				else
				{
					sprintf(stringconversion, "Invalid Password");
					FormatLine(current_line++, stringconversion, CREATE_ELEMENT, NULL, 0);
				}
			}
		}
	}

	sprintf(stringconversion, "Exit");
	FormatLine(current_line++, stringconversion, CREATE_ELEMENT, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateMainMenu(void)	
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

void CreateMainMenu(void)
{
	char stringconversion[29];
	char current_line;

	sprintf(DisplayMessage, "INSTRUMENT\0");
	pmsgScreen.parent_menu = CREATE_ELEMENT;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 6;

	current_line = 1;

	sprintf(stringconversion, "Settings");
	FormatLine(current_line++, stringconversion, CREATE_SETTINGS, NULL, 0);

	sprintf(stringconversion, "Controls");			  
	FormatLine(current_line++, stringconversion, CREATE_CONTROLS, NULL, 0);

	sprintf(stringconversion, "Features");
	FormatLine(current_line++, stringconversion, CREATE_FEATURES, NULL, 0);

#ifdef EEPROM_DETECTOR_PROPERTIES_LOCATITRUE
	sprintf(stringconversion, "Detectors");
	FormatLine(current_line++, stringconversion, CREATE_DETECTOR, NULL, 0);
#else
	sprintf(stringconversion, "Display");
	FormatLine(current_line++, stringconversion, CREATE_DISPLAY, NULL, 0);
#endif

	sprintf(stringconversion, "Functions");
	FormatLine(current_line++, stringconversion, CREATE_FUNCTIONS, NULL, 0);

//	sprintf(stringconversion, "Alerts/Alarms");
//	FormatLine(current_line++, stringconversion, CREATE_ALERTS_ALARMS, NULL, 0);

	sprintf(stringconversion, "Exit");
	FormatLine(current_line++, stringconversion, CREATE_ELEMENT, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
 	MenuLine = 1;
//	MenuLine = menu_line[pmsgScreen.current_line];													// (9DP v1.01.01)
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateSettingsMenu(void)	
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

void CreateSettingsMenu(void)
{
	char stringconversion[29];
	char current_line;

	sprintf(DisplayMessage, "SETTINGS\0");
	pmsgScreen.parent_menu = CREATE_MENU;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 7;

	current_line = 1;
	sprintf(stringconversion, "Product");
	FormatLine(current_line++, stringconversion, CREATE_PRODUCT, NULL, 0);

	sprintf(stringconversion, "Language");
	FormatLine(current_line++, stringconversion, CREATE_LINE, (void *)&lmi.settings.language.info.index, TYPE_LANGUAGE_SELECT);

	sprintf(stringconversion, "Calendar");			  
	FormatLine(current_line++, stringconversion, CREATE_CALENDAR, NULL, 0);

	sprintf(stringconversion, "Clock");
	FormatLine(current_line++, stringconversion, CREATE_CLOCK, NULL, 0);

	sprintf(stringconversion, "Password");
	FormatLine(current_line++, stringconversion, CREATE_PASSWORD, NULL, 0);

	sprintf(stringconversion, "System");
	FormatLine(current_line++, stringconversion, CREATE_SYSTEM, NULL, 0);

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateProductMenu(void)
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

void CreateProductMenu(void)
{
	char stringconversion[29];
	char current_line;

	sprintf(DisplayMessage, "PRODUCT\0");
	pmsgScreen.parent_menu = CREATE_SETTINGS;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 5;

	current_line = 1;
	sprintf(stringconversion, "Model");
	FormatLine(current_line++, stringconversion, CREATE_LINE, (void *)lmi.settings.product.info.model, TYPE_STRING);

	if(lmi.settings.product.info.model_number != MODEL_V1)																							// (9DP v1.01.21)
		sprintf(stringconversion, "Ludlum Serial Number");																							// (9DP v1.01.21)
	else																																			// (9DP v1.01.21)
		sprintf(stringconversion, "Serial Number");																									// (9DP v1.01.21)

	FormatLine(current_line++, stringconversion, CREATE_LINE, (void *)lmi.settings.product.info.serial_number, TYPE_STRING);

	sprintf(stringconversion, "User ID Number");			  
	FormatLine(current_line++, stringconversion, CREATE_EDIT_STRING_LINE, (void *)lmi.settings.product.info.user_id_number, TYPE_STRING);

	sprintf(stringconversion, "User Serial Number");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_STRING_LINE, (void *)lmi.settings.product.info.user_serial_number, TYPE_STRING);

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateLanguageMenu(void)
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

void CreateLanguageMenu(void)
{
	char stringconversion[29];
	char current_line;

	sprintf(DisplayMessage, "LANGUAGE\0");
	pmsgScreen.parent_menu = CREATE_SETTINGS;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 2;

	current_line = 1;
	CreateLanguageList(lmi.settings.language.info.index, stringconversion);
	FormatLine(current_line++, stringconversion, CREATE_LINE, (void *)&lmi.settings.language.info.index, TYPE_LANGUAGE_SELECT);

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateCalendarMenu(void)
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

void CreateCalendarMenu(void)
{
	char stringconversion[29];
	char current_line;

	sprintf(DisplayMessage, "CALENDAR\0");
	pmsgScreen.parent_menu = CREATE_SETTINGS;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 4;

	current_line = 1;
	sprintf(stringconversion, "Month");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.settings.calendar.info.date.month, TYPE_DATE_MONTH);

	sprintf(stringconversion, "Day");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.settings.calendar.info.date.day, TYPE_DATE_DAY);

	sprintf(stringconversion, "Year");			  
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.settings.calendar.info.date.year, TYPE_DATE_YEAR);

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateClockMenu(void)
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

void CreateClockMenu(void)
{
	char stringconversion[29];
	char current_line;

	sprintf(DisplayMessage, "CLOCK\0");
	pmsgScreen.parent_menu = CREATE_SETTINGS;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 5;

	current_line = 1;
	sprintf(stringconversion, "Hours");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.settings.clock.info.time.hours, TYPE_TIME_HOURS);

	sprintf(stringconversion, "Minutes");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.settings.clock.info.time.minutes, TYPE_TIME_MINUTES);

	sprintf(stringconversion, "Seconds");			  
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.settings.clock.info.time.seconds, TYPE_TIME_SECONDS);

	sprintf(stringconversion, "Format");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.settings.clock.info.time.format, TYPE_TIME_FORM);

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreatePasswordMenu(void)
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

void CreatePasswordMenu(void)
{
	char stringconversion[29];
	char current_line;

	sprintf(DisplayMessage, "PASSWORD\0");
	pmsgScreen.parent_menu = CREATE_SETTINGS;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 5;

	current_line = 1;
	strcpy(stringconversion, MessageEnable);
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.settings.passwords.info.enable, TYPE_ENABLE);

	sprintf(stringconversion, "Password 1");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_STRING_LINE, (void *)lmi.settings.passwords.info.password_1, TYPE_STRING);

	sprintf(stringconversion, "Password 2");			  
	FormatLine(current_line++, stringconversion, CREATE_EDIT_STRING_LINE, (void *)lmi.settings.passwords.info.password_2, TYPE_STRING);

	sprintf(stringconversion, "Password 3");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_STRING_LINE, (void *)lmi.settings.passwords.info.password_3, TYPE_STRING);

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateSystemMenu(void)
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

void CreateSystemMenu(void)
{
	char stringconversion[29];
	char current_line;

	sprintf(DisplayMessage, "SYSTEM\0");
	pmsgScreen.parent_menu = CREATE_SETTINGS;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 2;

	current_line = 1;
	sprintf(stringconversion, "Firmware Number");
	FormatLine(current_line++, stringconversion, CREATE_LINE, (void *)lmi.settings.misc.info.firmware, TYPE_STRING);

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateFeaturesMenu(void)	
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

void CreateFeaturesMenu(void)
{
	char stringconversion[29];
	char current_line;

	sprintf(DisplayMessage, "FEATURES\0");
	pmsgScreen.parent_menu = CREATE_MENU;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 3;																	// (9DP v1.01.02)

	current_line = 1;

//	sprintf(stringconversion, "Source Check");
//	FormatLine(current_line++, stringconversion, CREATE_BLANK_MENU, NULL, 0);

//	sprintf(stringconversion, "Background Check");
//	FormatLine(current_line++, stringconversion, CREATE_BLANK_MENU, NULL, 0);

	sprintf(stringconversion, "Integrate");			  												// (9DP v1.01.02)
	FormatLine(current_line++, stringconversion, CREATE_INTEGRATE, NULL, 0);						// (9DP v1.01.02)

	sprintf(stringconversion, "Data Logging");
	FormatLine(current_line++, stringconversion, CREATE_DATA_LOGGING, NULL, 0);

//	sprintf(stringconversion, "Scaler");
//	FormatLine(current_line++, stringconversion, CREATE_BLANK_MENU, NULL, 0);

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateSourceCheckMenu(void)
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

void CreateSourceCheckMenu(void)
{
	char stringconversion[29];
	char current_line;

	sprintf(DisplayMessage, "SOURCE_CHECK\0");
	pmsgScreen.parent_menu = CREATE_MENU;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 1;

	current_line = 1;
	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateBackgroundCheckMenu(void)
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

void CreateBackgroundCheckMenu(void)
{
	char stringconversion[29];
	char current_line;

	sprintf(DisplayMessage, "BACKGROUND CHECK\0");
	pmsgScreen.parent_menu = CREATE_MENU;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 1;

	current_line = 1;

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateIntegrateMenu(void)
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

void CreateIntegrateMenu(void)
{
	char stringconversion[29];
	char current_line;

	sprintf(DisplayMessage, "INTEGRATE\0");
	pmsgScreen.parent_menu = CREATE_FEATURES;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 7;

	current_line = 1;

	sprintf(stringconversion, "Power Up Mode");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.features.integrate.info.powerup_mode, TYPE_PMODE_LOGGING);

	sprintf(stringconversion, "Operating Mode");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.features.integrate.info.operating_mode, TYPE_ENABLE);

	sprintf(stringconversion, "Delay On Power Up");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.features.integrate.info.delay, TYPE_3_MINUTES);

	sprintf(stringconversion, "Save On Shutdown");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.features.integrate.info.save_on_shutdown, TYPE_ENABLE);

	sprintf(stringconversion, "Logging Report");
	FormatLine(current_line++, stringconversion, CREATE_INTEGRATE_REPORT, NULL, 0);

	sprintf(stringconversion, "Value");
	FormatLine(current_line++, stringconversion, CREATE_INTEGRATE_VALUE, NULL, 0);

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateIntegrateReportMenu(void)
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

void CreateIntegrateReportMenu(void)
{
	char stringconversion[29];
	char current_line;

	sprintf(DisplayMessage, "INTEGRATE REPORT\0");
	pmsgScreen.parent_menu = CREATE_INTEGRATE;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 3;

	current_line = 1;

	sprintf(stringconversion, "Log On Start Up");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.features.integrate.info.startup_log, TYPE_ENABLE);

	sprintf(stringconversion, "Log Before Shut Down");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.features.integrate.info.shutdown_log, TYPE_ENABLE);

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateIntegrateValueMenu(void)
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

void CreateIntegrateValueMenu(void)
{
	char stringconversion[29];
	char current_line;

	sprintf(DisplayMessage, "INTEGRATE VALUE\0");
	pmsgScreen.parent_menu = CREATE_INTEGRATE;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 4;

	current_line = 1;

	sprintf(stringconversion, "Value");
	FormatLine(current_line++, stringconversion, CREATE_LINE, (void *)&lmi.features.integrate.info.current_reading.value, TYPE_FLOAT);

	sprintf(stringconversion, "Multiplier");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.features.integrate.info.current_reading.multiplier, TYPE_INTEGRATE_MULTIPLIER);

	sprintf(stringconversion, "Units");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.features.integrate.info.current_reading.units, TYPE_INTEGRATE_UNITS);

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateDataLoggingMenu(void)
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

void CreateDataLoggingMenu(void)
{
	char stringconversion[29];
	char current_line;

	sprintf(DisplayMessage, "DATA LOGGING\0");
	pmsgScreen.parent_menu = CREATE_FEATURES;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 6;

	current_line = 1;

	sprintf(stringconversion, "Power Up Mode");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.features.data_logging.info.powerup_mode, TYPE_PMODE_LOGGING);

	sprintf(stringconversion, "Operating Mode");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.features.data_logging.info.operating_mode, TYPE_ENABLE);

	sprintf(stringconversion, "Delay On Power Up");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.features.data_logging.info.delay, TYPE_3_MINUTES);

	sprintf(stringconversion, "Sample Period");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.features.data_logging.info.sample_period, TYPE_1_HOUR);

	sprintf(stringconversion, "Report Setup");
	FormatLine(current_line++, stringconversion, CREATE_DATA_LOGGING_REPORT, NULL, 0);

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateDataLoggingReportMenu(void)
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

void CreateDataLoggingReportMenu(void)
{
	char stringconversion[29];
	char current_line;

	sprintf(DisplayMessage, "REPORT SETUP\0");
	pmsgScreen.parent_menu = CREATE_DATA_LOGGING;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 4;

	current_line = 1;
	sprintf(stringconversion, "System");
	FormatLine(current_line++, stringconversion, CREATE_DATA_LOGGING_REPORT_SYSTEM, NULL, 0);

	sprintf(stringconversion, "Instrument");
	FormatLine(current_line++, stringconversion, CREATE_DATA_LOGGING_REPORT_INSTRUMENT, NULL, 0);

	sprintf(stringconversion, "Readings");
	FormatLine(current_line++, stringconversion, CREATE_DATA_LOGGING_REPORT_READINGS, NULL, 0);

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateDataLoggingReportSystemMenu(void)
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

void CreateDataLoggingReportSystemMenu(void)
{
	char stringconversion[29];
	char current_line;

	sprintf(DisplayMessage, "REPORT SYSTEM\0");
	pmsgScreen.parent_menu = CREATE_DATA_LOGGING_REPORT;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 6;

	current_line = 1;
	sprintf(stringconversion, "Date\0");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.features.data_logging.info.reported_data.date, TYPE_ENABLE);

	sprintf(stringconversion, "Time\0");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.features.data_logging.info.reported_data.time, TYPE_ENABLE);

	sprintf(stringconversion, "Chamber Temperature\0");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.features.data_logging.info.reported_data.temperature, TYPE_ENABLE);

	sprintf(stringconversion, "High Voltage\0");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.features.data_logging.info.reported_data.high_voltage_1, TYPE_ENABLE);

	sprintf(stringconversion, "Battery Voltage\0");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.features.data_logging.info.reported_data.battery_voltage, TYPE_ENABLE);

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateDataLoggingReportInstrumentMenu(void)
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

void CreateDataLoggingReportInstrumentMenu(void)
{
	char stringconversion[29];
	char current_line;

	sprintf(DisplayMessage, "REPORT INSTRUMENT\0");
	pmsgScreen.parent_menu = CREATE_DATA_LOGGING_REPORT;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 5;

	current_line = 1;
	sprintf(stringconversion, "Model\0");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.features.data_logging.info.reported_data.model_number, TYPE_ENABLE);

	if(lmi.settings.product.info.model_number != MODEL_V1)																							// (9DP v1.01.21)
		sprintf(stringconversion, "Ludlum Serial Number\0");																						// (9DP v1.01.21)
	else																																			// (9DP v1.01.21)
		sprintf(stringconversion, "Serial Number\0");																								// (9DP v1.01.21)
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.features.data_logging.info.reported_data.serial_number, TYPE_ENABLE);

	sprintf(stringconversion, "User ID Number\0");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.features.data_logging.info.reported_data.user_id_number, TYPE_ENABLE);

	sprintf(stringconversion, "User Serial Number\0");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.features.data_logging.info.reported_data.user_serial_number, TYPE_ENABLE);

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateDataLoggingReportReadingsMenu(void)
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

void CreateDataLoggingReportReadingsMenu(void)
{
	char stringconversion[29];
	char current_line;

	sprintf(DisplayMessage, "REPORT READINGS\0");
	pmsgScreen.parent_menu = CREATE_DATA_LOGGING_REPORT;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 7;

	current_line = 1;
	sprintf(stringconversion, "Reading\0");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.features.data_logging.info.reported_data.reading, TYPE_ENABLE);

	sprintf(stringconversion, "Reading Multiplier\0");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.features.data_logging.info.reported_data.reading_multiplier, TYPE_ENABLE);

	sprintf(stringconversion, "Reading Units\0");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.features.data_logging.info.reported_data.reading_units, TYPE_ENABLE);

	sprintf(stringconversion, "Integrated Reading\0");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.features.data_logging.info.reported_data.integrated_reading, TYPE_ENABLE);

	sprintf(stringconversion, "Integrated Multiplier\0");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.features.data_logging.info.reported_data.integrated_reading_multiplier, TYPE_ENABLE);

	sprintf(stringconversion, "Integrated Units\0");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.features.data_logging.info.reported_data.integrated_reading_units, TYPE_ENABLE);

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateControlsMenu(void)	
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

void CreateControlsMenu(void)
{
	char stringconversion[29];
	char current_line;

	sprintf(DisplayMessage, "CONTROLS\0");
	pmsgScreen.parent_menu = CREATE_MENU;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 3;

	current_line = 1;

	sprintf(stringconversion, "Backlight");
	FormatLine(current_line++, stringconversion, CREATE_BACKLIGHT, NULL, 0);

	sprintf(stringconversion, "Audio");
	FormatLine(current_line++, stringconversion, CREATE_AUDIO, NULL, 0);

//	sprintf(stringconversion, "GPS");			  
//	FormatLine(current_line++, stringconversion, CREATE_BLANK_MENU, NULL, 0);

//	sprintf(stringconversion, "Bluetooth");
//	FormatLine(current_line++, stringconversion, CREATE_BLANK_MENU, NULL, 0);

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateBacklightMenu(void)
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

void CreateBacklightMenu(void)
{
	char stringconversion[29];
	char current_line;

	sprintf(DisplayMessage, "BACKLIGHT\0");
	pmsgScreen.parent_menu = CREATE_CONTROLS;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 6;

	current_line = 1;
	sprintf(stringconversion, "Power Up Mode");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.controls.backlight.info.powerup_mode, TYPE_PMODE_BACKLIGHT);

	sprintf(stringconversion, "Operating Mode");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.controls.backlight.info.operating_mode, TYPE_OMODE_BACKLIGHT);

	sprintf(stringconversion, "Temporary Display Time");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.controls.backlight.info.temp_time, TYPE_3_MINUTES);

	sprintf(stringconversion, "Always On Level");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.controls.backlight.info.on_level, TYPE_100_PERCENT);

	sprintf(stringconversion, "Auto Mode Settings");
	FormatLine(current_line++, stringconversion, CREATE_AUTO_BACKLIGHT, NULL, 0);

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateAutoBacklightMenu(void)
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

void CreateAutoBacklightMenu(void)
{
	char stringconversion[29];
	char current_line;

	sprintf(DisplayMessage, "AUTO BACKLIGHT\0");
	pmsgScreen.parent_menu = CREATE_BACKLIGHT;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 4;

	current_line = 1;
	sprintf(stringconversion, "Low Level Backlight");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.controls.backlight.info.auto_low, TYPE_100_PERCENT);

	sprintf(stringconversion, "Medium Level Backlight");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.controls.backlight.info.auto_medium, TYPE_100_PERCENT);

	sprintf(stringconversion, "High Level Backlight");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.controls.backlight.info.auto_high, TYPE_100_PERCENT);

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateAudioMenu(void)
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

void CreateAudioMenu(void)
{
	char stringconversion[29];
	char current_line;

	sprintf(DisplayMessage, "AUDIO\0");
	pmsgScreen.parent_menu = CREATE_CONTROLS;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 5;

	current_line = 1;
	sprintf(stringconversion, "Power Up Mode");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.controls.audio.info.powerup_mode, TYPE_PMODE_AUDIO);

	sprintf(stringconversion, "Operating Mode");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.controls.audio.info.operating_mode, TYPE_OMODE_AUDIO);

	sprintf(stringconversion, "Alert Level");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.controls.audio.info.alert_level, TYPE_100_PERCENT);

	sprintf(stringconversion, "Alarm Level");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.controls.audio.info.alarm_level, TYPE_100_PERCENT);

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateGPSMenu(void)
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

void CreateGPSMenu(void)
{
	char stringconversion[29];
	char current_line;

	sprintf(DisplayMessage, "GPS\0");
	pmsgScreen.parent_menu = CREATE_CONTROLS;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 3;

	current_line = 1;
	sprintf(stringconversion, "Power Up Mode");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.controls.gps.info.powerup_mode, TYPE_PMODE_GPS);

	sprintf(stringconversion, "Operating Mode");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.controls.gps.info.operating_mode, TYPE_OMODE_GPS);

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateBluetoothMenu(void)
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

void CreateBluetoothMenu(void)
{
	char stringconversion[29];
	char current_line;

	sprintf(DisplayMessage, "BLUETOOTH\0");
	pmsgScreen.parent_menu = CREATE_CONTROLS;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 3;

	current_line = 1;
	sprintf(stringconversion, "Power Up Mode");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.controls.bluetooth.info.powerup_mode, TYPE_PMODE_BLUETOOTH);

	sprintf(stringconversion, "Operating Mode");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.controls.bluetooth.info.operating_mode, TYPE_OMODE_BLUETOOTH);

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateDetectorMenu(void)	
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

#ifdef EEPROM_DETECTOR_PROPERTIES_LOCATITRUE

void CreateDetectorMenu(void)
{
	char stringconversion[29];
	char detectorconversion[29];
	char current_line;
	char detector_index;

	if((NUMBER_OF_INTERNAL_DETECTORS + NUMBER_OF_EXTERNAL_DETECTORS) > 1)
		sprintf(DisplayMessage, "DETECTORS\0");
	else
		sprintf(DisplayMessage, "DETECTOR\0");
	pmsgScreen.parent_menu = CREATE_MENU;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 2 + NUMBER_OF_DETECTORS;											// SETUP + BACK + DETECTORS

	current_line = 1;
	sprintf(stringconversion, "Setup");
	FormatLine(current_line++, stringconversion, CREATE_DETECTOR_SETUP, NULL, 0);

	if(NUMBER_OF_INTERNAL_DETECTORS)
	{
		detector_index = 0;
	}
	else
	{
		detector_index = 1;
		pmsgScreen.txt_total_lines--;
	}

	for(;detector_index < NUMBER_OF_DETECTORS; detector_index++)
	{
		if(lmi.display.detector[detector_index].properties.info.model == 0)
		{
			sprintf(stringconversion, "Detector %d:\0",detector_index);
		}
		else
		{
			CreateDetectorList(lmi.display.detector[detector_index].properties.info.model, (char *)detectorconversion);
			sprintf(stringconversion, "Detector %d: %s\0",detector_index, detectorconversion);
		}
		FormatLine(current_line++, stringconversion, CREATE_DETECTOR_0 + detector_index, NULL, 0);
	}

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

#endif

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateDetectorSetupMenu(void)	
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

#ifdef EEPROM_DETECTOR_PROPERTIES_LOCATITRUE

void CreateDetectorSetupMenu(void)
{
	char stringconversion[29];
	char current_line;

	sprintf(DisplayMessage, "DETECTOR - SETUP\0");
	pmsgScreen.parent_menu = CREATE_DETECTOR;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 3;

	current_line = 1;
	sprintf(stringconversion, "Power Up Mode");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.display.properties.info.powerup_mode, TYPE_PMODE_DETECTOR);

	sprintf(stringconversion, "Current Detector");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.display.properties.info.current_detector, TYPE_DETECTOR);

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

#endif

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateDisplayMenu(SHORT calling_menu)	
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

void CreateDisplayMenu(SHORT calling_menu)
{
	char stringconversion[29];
	char detectorconversion[29];
	char current_line;

#ifdef EEPROM_DETECTOR_PROPERTIES_LOCATITRUE
	CreateDetectorList(lmi.display.detector[DetectorMenu].properties.info.model, (char *)detectorconversion);
	sprintf(DisplayMessage, "DET %d: %s\0", DetectorMenu, detectorconversion);
	pmsgScreen.parent_menu = CREATE_DETECTOR;
#else
	sprintf(DisplayMessage, "DISPLAY\0");
	pmsgScreen.parent_menu = CREATE_MENU;
#endif
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 6;

	current_line = 1;
	sprintf(stringconversion, "Setup");
	FormatLine(current_line++, stringconversion, CREATE_DISPLAY_SETUP, NULL, 0);

	sprintf(stringconversion, "View 1: %s", lmi.display.detector[DetectorMenu].view[0].element_header.info.name);
	FormatLine(current_line++, stringconversion, CREATE_VIEW_1, NULL, 0);

	sprintf(stringconversion, "View 2: %s", lmi.display.detector[DetectorMenu].view[1].element_header.info.name);
	FormatLine(current_line++, stringconversion, CREATE_VIEW_2, NULL, 0);

	sprintf(stringconversion, "View 3: %s", lmi.display.detector[DetectorMenu].view[2].element_header.info.name);
	FormatLine(current_line++, stringconversion, CREATE_VIEW_3, NULL, 0);

	sprintf(stringconversion, "View 4: %s", lmi.display.detector[DetectorMenu].view[3].element_header.info.name);
	FormatLine(current_line++, stringconversion, CREATE_VIEW_4, NULL, 0);

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateDisplaySetupMenu(void)
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

void CreateDisplaySetupMenu(void)
{
	char stringconversion[29];
	char current_line;

	sprintf(DisplayMessage, "DISPLAY - SETUP\0");
#ifdef EEPROM_DETECTOR_PROPERTIES_LOCATITRUE
	pmsgScreen.parent_menu = CREATE_DETECTOR_0 + DetectorMenu;
#else
	pmsgScreen.parent_menu = CREATE_DISPLAY;
#endif
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 3;

	current_line = 1;
	sprintf(stringconversion, "Power Up Mode");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.display.detector[DetectorMenu].view_setup.info.powerup_mode, TYPE_PMODE_DISPLAY);

	sprintf(stringconversion, "Current View");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.display.detector[DetectorMenu].view_setup.info.current_view, TYPE_CURRENT_VIEW);

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateViewMenu(void)
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

void CreateViewMenu(void)
{
	char stringconversion[29];
	char detectorconversion[29];
	char current_line;

#ifdef EEPROM_DETECTOR_PROPERTIES_LOCATITRUE
	CreateDetectorList(lmi.display.detector[DetectorMenu].properties.info.model, (char *)detectorconversion);
	sprintf(DisplayMessage, "%s: VIEW %d: %s\0", detectorconversion, ViewMenu, lmi.display.detector[DetectorMenu].view[ViewMenu - 1].element_header.info.name);
	pmsgScreen.parent_menu = CREATE_DETECTOR_0 + DetectorMenu;
#else
	sprintf(DisplayMessage, "VIEW %d: %s\0", ViewMenu, lmi.display.detector[DetectorMenu].view[ViewMenu - 1].element_header.info.name);
	pmsgScreen.parent_menu = CREATE_DISPLAY;
#endif
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 5;

	current_line = 1;
	sprintf(stringconversion, "Appearance");
	FormatLine(current_line++, stringconversion, CREATE_VIEW_APPEARANCE, NULL, 0);

	sprintf(stringconversion, "Functions");
	FormatLine(current_line++, stringconversion, CREATE_VIEW_FUNCTIONS, NULL, 0);

	sprintf(stringconversion, "Alerts");
	FormatLine(current_line++, stringconversion, CREATE_VIEW_ALERTS, NULL, 0);

	sprintf(stringconversion, "Alarms");
	FormatLine(current_line++, stringconversion, CREATE_VIEW_ALARMS, NULL, 0);

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateViewAppearanceMenu(void)
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

void CreateViewAppearanceMenu(void)
{
	char stringconversion[29];
	char current_line;
	
	if(strlen(lmi.display.detector[DetectorMenu].view[ViewMenu - 1].element_header.info.name) != 0)
		sprintf(DisplayMessage, "%s - APPEARANCE\0", lmi.display.detector[DetectorMenu].view[ViewMenu - 1].element_header.info.name);
	else
		sprintf(DisplayMessage, "VIEW %d - APPEARANCE\0", ViewMenu);
	pmsgScreen.parent_menu =  CREATE_VIEW_1 + ViewMenu - 1;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 3;

	current_line = 1;
	sprintf(stringconversion, "Background Color");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.display.detector[DetectorMenu].view[ViewMenu - 1].appearance.info.background_color, TYPE_COLOR);

	sprintf(stringconversion, "Foreground Color");
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.display.detector[DetectorMenu].view[ViewMenu - 1].appearance.info.foreground_color, TYPE_COLOR);

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateViewFunctionsMenu(void)	
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

void CreateViewFunctionsMenu(void)
{
	char stringconversion[29];
	char functionconversion[29];
	char current_line;

	if(strlen(lmi.display.detector[DetectorMenu].view[ViewMenu - 1].element_header.info.name) != 0)
		sprintf(DisplayMessage, "%s - FUNCTIONS\0", lmi.display.detector[DetectorMenu].view[ViewMenu - 1].element_header.info.name);
	else
		sprintf(DisplayMessage, "VIEW %d - FUNCTIONS\0", ViewMenu);
	pmsgScreen.parent_menu = CREATE_VIEW_1 + ViewMenu - 1;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 5;

	current_line = 1;

	if(lmi.display.detector[DetectorMenu].view[ViewMenu - 1].functions.info.function[current_line - 1].function_index == 255)
	{
		sprintf(stringconversion, "FCN %d: - empty -\0", current_line);
	}
	else
	{
		CreateInstrumentFunctionsList(lmi.function[lmi.display.detector[DetectorMenu].view[ViewMenu - 1].functions.info.function[current_line - 1].function_index].info.header.type, (char *)functionconversion);
		sprintf(stringconversion, "FCN %d: %s\0", current_line, functionconversion);
	}
	FormatLine(current_line++, stringconversion, CREATE_VIEW_FUNCTION_1, NULL, 0);

	if(lmi.display.detector[DetectorMenu].view[ViewMenu - 1].functions.info.function[current_line - 1].function_index == 255)
	{
		sprintf(stringconversion, "FCN %d: - empty -\0", current_line);
	}
	else
	{
		CreateInstrumentFunctionsList(lmi.function[lmi.display.detector[DetectorMenu].view[ViewMenu - 1].functions.info.function[current_line - 1].function_index].info.header.type, (char *)functionconversion);
		sprintf(stringconversion, "FCN %d: %s\0", current_line, functionconversion);
	}
	FormatLine(current_line++, stringconversion, CREATE_VIEW_FUNCTION_2, NULL, 0);

	if(lmi.display.detector[DetectorMenu].view[ViewMenu - 1].functions.info.function[current_line - 1].function_index == 255)
	{
		sprintf(stringconversion, "FCN %d: - empty -\0", current_line);
	}
	else
	{
		CreateInstrumentFunctionsList(lmi.function[lmi.display.detector[DetectorMenu].view[ViewMenu - 1].functions.info.function[current_line - 1].function_index].info.header.type, (char *)functionconversion);
		sprintf(stringconversion, "FCN %d: %s\0", current_line, functionconversion);
	}
	FormatLine(current_line++, stringconversion, CREATE_VIEW_FUNCTION_3, NULL, 0);

	if(lmi.display.detector[DetectorMenu].view[ViewMenu - 1].functions.info.function[current_line - 1].function_index == 255)
	{
		sprintf(stringconversion, "FCN %d: - empty -\0", current_line);
	}
	else
	{
		CreateInstrumentFunctionsList(lmi.function[lmi.display.detector[DetectorMenu].view[ViewMenu - 1].functions.info.function[current_line - 1].function_index].info.header.type, (char *)functionconversion);
		sprintf(stringconversion, "FCN %d: %s\0", current_line, functionconversion);
	}
	FormatLine(current_line++, stringconversion, CREATE_VIEW_FUNCTION_4, NULL, 0);

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateViewFunctionMenu(void)	
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

void CreateViewFunctionMenu(void)
{
	char stringconversion[29];
	char functionconversion[29];
	char current_line;

	if(strlen(lmi.display.detector[DetectorMenu].view[ViewMenu - 1].element_header.info.name) != 0)
		sprintf(DisplayMessage, "%s - FCN %d\0", lmi.display.detector[DetectorMenu].view[ViewMenu - 1].element_header.info.name, FunctionMenu);
	else
		sprintf(DisplayMessage, "VIEW %d - FCN %d\0", ViewMenu, FunctionMenu);
	pmsgScreen.parent_menu = CREATE_VIEW_FUNCTIONS;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 3;

	current_line = 1;

	if(lmi.display.detector[DetectorMenu].view[ViewMenu - 1].functions.info.function[FunctionMenu - 1].function_index == 255)
	{
		sprintf(stringconversion, "FCN %d: - empty -\0", FunctionMenu);
	}
	else
	{
		CreateInstrumentFunctionsList(lmi.function[lmi.display.detector[DetectorMenu].view[ViewMenu - 1].functions.info.function[FunctionMenu - 1].function_index].info.header.type, (char *)functionconversion);
		sprintf(stringconversion, "FCN %d: %s\0", FunctionMenu, functionconversion);
	}
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.display.detector[DetectorMenu].view[ViewMenu - 1].functions.info.function[FunctionMenu - 1].function_index, TYPE_VIEW_FUNCTION);

	sprintf(stringconversion, "FCN %d: Enable\0", FunctionMenu);
	FormatLine(current_line++, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.display.detector[DetectorMenu].view[ViewMenu - 1].functions.info.function[FunctionMenu - 1].enable, TYPE_ENABLE);

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateViewAlertsMenu(void)	
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

void CreateViewAlertsMenu(void)
{
	char stringconversion[29];
	char current_line;

	if(strlen(lmi.display.detector[DetectorMenu].view[ViewMenu - 1].element_header.info.name) != 0)
		sprintf(DisplayMessage, "%s - ALERTS\0", lmi.display.detector[DetectorMenu].view[ViewMenu - 1].element_header.info.name);
	else
		sprintf(DisplayMessage, "VIEW %d - ALERTS\0", ViewMenu);
	pmsgScreen.parent_menu = CREATE_VIEW_1 + ViewMenu - 1;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 3;

	current_line = 1;
	sprintf(stringconversion, "Radiation");
	FormatLine(current_line++, stringconversion, CREATE_VIEW_ALERTS_RAD, NULL, 0);

	sprintf(stringconversion, "Integration");
	FormatLine(current_line++, stringconversion, CREATE_VIEW_ALERTS_INT, NULL, 0);

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateViewAlertsReadingMenu(void)
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

void CreateViewAlertsReadingMenu(void)
{
	char stringconversion[29];
	char current_line;
	
	pmsgScreen.parent_menu = CREATE_VIEW_ALERTS;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 5;

	if(pmsgScreen.current_menu == CREATE_VIEW_ALERTS_RAD)
		sprintf(DisplayMessage, "VIEW %d - ALERTS - RAD\0", ViewMenu);
	else if(pmsgScreen.current_menu == CREATE_VIEW_ALERTS_INT)
		sprintf(DisplayMessage, "VIEW %d - ALERTS - INT\0", ViewMenu);

	current_line = 1;
	strcpy(stringconversion, MessageEnable);
	Convert_For_Display(pmsgScreen.txt_text[current_line], NULL, stringconversion, 28);
	pmsgScreen.sub_menu[current_line] = CREATE_EDIT_LINE;
	if(pmsgScreen.current_menu == CREATE_VIEW_ALERTS_RAD)
		pmsgScreen.line_item[current_line] = (void *)&lmi.display.detector[DetectorMenu].view[ViewMenu - 1].alerts_radiation.info.feature.enable;
	else if(pmsgScreen.current_menu == CREATE_VIEW_ALERTS_INT)
		pmsgScreen.line_item[current_line] = (void *)&lmi.display.detector[DetectorMenu].view[ViewMenu - 1].alerts_integrate.info.feature.enable;
	pmsgScreen.line_item_type[current_line++] = TYPE_ENABLE;

	sprintf(stringconversion, "Value");
	Convert_For_Display(pmsgScreen.txt_text[current_line], NULL, stringconversion, 28);
	pmsgScreen.sub_menu[current_line] = CREATE_EDIT_LINE;
	if(pmsgScreen.current_menu == CREATE_VIEW_ALERTS_RAD)
		pmsgScreen.line_item[current_line] = (void *)&lmi.display.detector[DetectorMenu].view[ViewMenu - 1].alerts_radiation.info.feature.reading.value;
	else if(pmsgScreen.current_menu == CREATE_VIEW_ALERTS_INT)
		pmsgScreen.line_item[current_line] = (void *)&lmi.display.detector[DetectorMenu].view[ViewMenu - 1].alerts_integrate.info.feature.reading.value;
	pmsgScreen.line_item_type[current_line++] = TYPE_VALUE;

	sprintf(stringconversion, "Multiplier");
	Convert_For_Display(pmsgScreen.txt_text[current_line], NULL, stringconversion, 28);
	pmsgScreen.sub_menu[current_line] = CREATE_EDIT_LINE;
	if(pmsgScreen.current_menu == CREATE_VIEW_ALERTS_RAD)
		pmsgScreen.line_item[current_line] = (void *)&lmi.display.detector[DetectorMenu].view[ViewMenu - 1].alerts_radiation.info.feature.reading.multiplier;
	else if(pmsgScreen.current_menu == CREATE_VIEW_ALERTS_INT)
		pmsgScreen.line_item[current_line] = (void *)&lmi.display.detector[DetectorMenu].view[ViewMenu - 1].alerts_integrate.info.feature.reading.multiplier;
	pmsgScreen.line_item_type[current_line++] = TYPE_MULTIPLIER;

	sprintf(stringconversion, "Units");
	Convert_For_Display(pmsgScreen.txt_text[current_line], NULL, stringconversion, 28);
	pmsgScreen.sub_menu[current_line] = CREATE_LINE;
	if(pmsgScreen.current_menu == CREATE_VIEW_ALERTS_RAD)
		pmsgScreen.line_item[current_line] = (void *)&lmi.display.detector[DetectorMenu].view[ViewMenu - 1].alerts_radiation.info.feature.reading.units;
	else if(pmsgScreen.current_menu == CREATE_VIEW_ALERTS_INT)
		pmsgScreen.line_item[current_line] = (void *)&lmi.display.detector[DetectorMenu].view[ViewMenu - 1].alerts_integrate.info.feature.reading.units;
	pmsgScreen.line_item_type[current_line++] = TYPE_UNITS;

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateViewAlarmsMenu(void)	
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

void CreateViewAlarmsMenu(void)
{
	char stringconversion[29];
	char current_line;

	if(strlen(lmi.display.detector[DetectorMenu].view[ViewMenu - 1].element_header.info.name) != 0)
		sprintf(DisplayMessage, "%s - ALARMS\0", lmi.display.detector[DetectorMenu].view[ViewMenu - 1].element_header.info.name);
	else
		sprintf(DisplayMessage, "VIEW %d - ALARMS\0", ViewMenu);
	pmsgScreen.parent_menu = CREATE_VIEW_1 + ViewMenu - 1;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 3;

	current_line = 1;
	sprintf(stringconversion, "Radiation");
	FormatLine(current_line++, stringconversion, CREATE_VIEW_ALARMS_RAD, NULL, 0);

	sprintf(stringconversion, "Integration");
	FormatLine(current_line++, stringconversion, CREATE_VIEW_ALARMS_INT, NULL, 0);

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateViewAlarmsReadingMenu(void)
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

void CreateViewAlarmsReadingMenu(void)
{
	char stringconversion[29];
	char current_line;
	
	pmsgScreen.parent_menu = CREATE_VIEW_ALARMS;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 5;

	if(pmsgScreen.current_menu == CREATE_VIEW_ALARMS_RAD)
		sprintf(DisplayMessage, "VIEW %d - ALARMS - RAD\0", ViewMenu);
	else if(pmsgScreen.current_menu == CREATE_VIEW_ALARMS_INT)
		sprintf(DisplayMessage, "VIEW %d - ALARMS - INT\0", ViewMenu);

	current_line = 1;
	strcpy(stringconversion, MessageEnable);
	Convert_For_Display(pmsgScreen.txt_text[current_line], NULL, stringconversion, 28);
	pmsgScreen.sub_menu[current_line] = CREATE_EDIT_LINE;
	if(pmsgScreen.current_menu == CREATE_VIEW_ALARMS_RAD)
		pmsgScreen.line_item[current_line] = (void *)&lmi.display.detector[DetectorMenu].view[ViewMenu - 1].alarms_radiation.info.feature.enable;
	else if(pmsgScreen.current_menu == CREATE_VIEW_ALARMS_INT)
		pmsgScreen.line_item[current_line] = (void *)&lmi.display.detector[DetectorMenu].view[ViewMenu - 1].alarms_integrate.info.feature.enable;
	pmsgScreen.line_item_type[current_line++] = TYPE_ENABLE;

	sprintf(stringconversion, "Value");
	Convert_For_Display(pmsgScreen.txt_text[current_line], NULL, stringconversion, 28);
	pmsgScreen.sub_menu[current_line] = CREATE_EDIT_LINE;
	if(pmsgScreen.current_menu == CREATE_VIEW_ALARMS_RAD)
		pmsgScreen.line_item[current_line] = (void *)&lmi.display.detector[DetectorMenu].view[ViewMenu - 1].alarms_radiation.info.feature.reading.value;
	else if(pmsgScreen.current_menu == CREATE_VIEW_ALARMS_INT)
		pmsgScreen.line_item[current_line] = (void *)&lmi.display.detector[DetectorMenu].view[ViewMenu - 1].alarms_integrate.info.feature.reading.value;
	pmsgScreen.line_item_type[current_line++] = TYPE_VALUE;

	sprintf(stringconversion, "Multiplier");
	Convert_For_Display(pmsgScreen.txt_text[current_line], NULL, stringconversion, 28);
	pmsgScreen.sub_menu[current_line] = CREATE_EDIT_LINE;
	if(pmsgScreen.current_menu == CREATE_VIEW_ALARMS_RAD)
		pmsgScreen.line_item[current_line] = (void *)&lmi.display.detector[DetectorMenu].view[ViewMenu - 1].alarms_radiation.info.feature.reading.multiplier;
	else if(pmsgScreen.current_menu == CREATE_VIEW_ALARMS_INT)
		pmsgScreen.line_item[current_line] = (void *)&lmi.display.detector[DetectorMenu].view[ViewMenu - 1].alarms_integrate.info.feature.reading.multiplier;
	pmsgScreen.line_item_type[current_line++] = TYPE_MULTIPLIER;

	sprintf(stringconversion, "Units");
	Convert_For_Display(pmsgScreen.txt_text[current_line], NULL, stringconversion, 28);
	pmsgScreen.sub_menu[current_line] = CREATE_LINE;
	if(pmsgScreen.current_menu == CREATE_VIEW_ALARMS_RAD)
		pmsgScreen.line_item[current_line] = (void *)&lmi.display.detector[DetectorMenu].view[ViewMenu - 1].alarms_radiation.info.feature.reading.units;
	else if(pmsgScreen.current_menu == CREATE_VIEW_ALARMS_INT)
		pmsgScreen.line_item[current_line] = (void *)&lmi.display.detector[DetectorMenu].view[ViewMenu - 1].alarms_integrate.info.feature.reading.units;
	pmsgScreen.line_item_type[current_line++] = TYPE_UNITS;

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateAlertsAlarmsMenu(void)	
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

void CreateAlertsAlarmsMenu(void)
{
	char stringconversion[29];
	char current_line;

	sprintf(DisplayMessage, "ALERTS/ALARMS\0");
	pmsgScreen.parent_menu = CREATE_MENU;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 3;

	current_line = 1;

	sprintf(stringconversion, "Alerts");
	FormatLine(current_line++, stringconversion, CREATE_ALERTS, NULL, 0);

	sprintf(stringconversion, "Alarms");
	FormatLine(current_line++, stringconversion, CREATE_ALARMS, NULL, 0);

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateAlertsMenu(void)	
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

void CreateAlertsMenu(void)
{
	char stringconversion[29];
	char current_line;

	sprintf(DisplayMessage, "ALERTS\0");
	pmsgScreen.parent_menu = CREATE_ALERTS_ALARMS;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 4;

	current_line = 1;

	sprintf(stringconversion, "Low Battery");
	FormatLine(current_line++, stringconversion, CREATE_BLANK_MENU, NULL, 0);

	sprintf(stringconversion, "Low Memory");
	FormatLine(current_line++, stringconversion, CREATE_BLANK_MENU, NULL, 0);

	sprintf(stringconversion, "Calibration Due");
	FormatLine(current_line++, stringconversion, CREATE_BLANK_MENU, NULL, 0);

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateAlarmsMenu(void)	
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

void CreateAlarmsMenu(void)
{
	char stringconversion[29];
	char current_line;

	sprintf(DisplayMessage, "ALARMS\0");
	pmsgScreen.parent_menu = CREATE_ALERTS_ALARMS;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 4;

	current_line = 1;

	sprintf(stringconversion, "Low Battery");
	FormatLine(current_line++, stringconversion, CREATE_BLANK_MENU, NULL, 0);

	sprintf(stringconversion, "Low Memory");
	FormatLine(current_line++, stringconversion, CREATE_BLANK_MENU, NULL, 0);

	sprintf(stringconversion, "Calibration Due");
	FormatLine(current_line++, stringconversion, CREATE_BLANK_MENU, NULL, 0);

//	sprintf(stringconversion, "Source Check");
//	FormatLine(current_line++, stringconversion, CREATE_BLANK_MENU, NULL, 0);

//	sprintf(stringconversion, "Background Check");
//	FormatLine(current_line++, stringconversion, CREATE_BLANK_MENU, NULL, 0);

//	sprintf(stringconversion, "High Voltage (Low)");
//	FormatLine(current_line++, stringconversion, CREATE_BLANK_MENU, NULL, 0);

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateFunctionsMenu(void)	
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

void CreateFunctionsMenu(void)
{
	char stringconversion[29];
	char functionconversion[29];
	char current_line;

	sprintf(DisplayMessage, "FUNCTIONS\0");
	pmsgScreen.parent_menu = CREATE_MENU;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 5;

	for(current_line = 1; current_line < pmsgScreen.txt_total_lines; current_line++)
	{
		CreateInstrumentFunctionsList(lmi.function[current_line - 1].info.header.type, (char *)functionconversion);
		sprintf(stringconversion, "FCN %d: %s\0", current_line, functionconversion);
//		FormatLine(current_line, stringconversion, CREATE_EDIT_LINE, (void *)&lmi.function[current_line - 1].info.header.type, TYPE_INSTRUMENT_FUNCTION);
		FormatLine(current_line, stringconversion, CREATE_LINE, (void *)&lmi.function[current_line - 1].info.header.type, TYPE_INSTRUMENT_FUNCTION);
	}

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateBlankMenu(void)
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

void CreateBlankMenu(void)
{
	char stringconversion[29];
	char current_line;

	sprintf(DisplayMessage, "UNDER CTRUESTRUCTITRUE\0");
	pmsgScreen.parent_menu = pmsgScreen.current_menu;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 1;

	current_line = 1;
	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	END OF MENU ROUTINES
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//*	FORMAT LINES
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void FormatLine(UINT8 current_line, char *generic_string, UINT8 sub_menu, void *line_item, UINT8 line_item_type)
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

void FormatLine(UINT8 current_line, char *generic_string, UINT8 sub_menu, void *line_item, UINT8 line_item_type)
{
	Convert_For_Display(pmsgScreen.txt_text[current_line], NULL, generic_string, 28);
	pmsgScreen.sub_menu[current_line] = sub_menu;
	pmsgScreen.line_item[current_line] = line_item;
	pmsgScreen.line_item_type[current_line] = line_item_type;
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void FormatString(char *generic_string, char *message, UINT8 message_length)
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
//* 	(9DP v1.01.04)
//*

void FormatString(char *generic_string, char *message, UINT8 message_length)
{
	char message_string[51];

	strncpy(message_string, message, message_length);
	message_string[message_length] = '\0';
	sprintf(generic_string,"%s\0",message_string);
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	END OF FORMAT LINES
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//*	CREATE LINES
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateLine(void)	
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

void CreateLine(void)
{
	char stringconversion[29];
	char current_line;
	INT8 *INT8_value;
	INT16 *INT16_value;
	INT32 *INT32_value;
	UINT8 *UINT8_value;
	UINT16 *UINT16_value;
	UINT32 *UINT32_value;
	float *float_value;

	Unconvert_Display(DisplayMessage, pmsgScreen.txt_text[MenuLine], 28);
	pmsgScreen.parent_menu = pmsgScreen.current_menu;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 2;

	switch(pmsgScreen.line_item_type[MenuLine])
	{
		case TYPE_STRING:
		{
			FormatString(stringconversion, (unsigned char *)pmsgScreen.line_item[MenuLine], 16);						// (9DP v1.01.04)
			break;
		}
		case TYPE_INT8:
		{
			INT8_value = (INT8 *)pmsgScreen.line_item[MenuLine];
			sprintf(stringconversion, "%d\0", *INT8_value);
			break;
		}
		case TYPE_INT16:
		{
			INT16_value = (INT16 *)pmsgScreen.line_item[MenuLine];
			sprintf(stringconversion, "%d\0", *INT16_value);
			break;
		}
		case TYPE_INT32:
		{
			INT32_value = (INT32 *)pmsgScreen.line_item[MenuLine];
			sprintf(stringconversion, "%ld\0", *INT32_value);
			break;
		}
		case TYPE_UINT8:
		{
			UINT8_value = (UINT8 *)pmsgScreen.line_item[MenuLine];
			sprintf(stringconversion, "%d\0", *UINT8_value);
			break;
		}
		case TYPE_UINT16:
		{
			UINT16_value = (UINT16 *)pmsgScreen.line_item[MenuLine];
			sprintf(stringconversion, "%d\0", *UINT16_value);
			break;
		}
		case TYPE_UINT32:
		{
			UINT32_value = (UINT32 *)pmsgScreen.line_item[MenuLine];
			sprintf(stringconversion, "%ld\0", *UINT32_value);
			break;
		}
		case TYPE_FLOAT:
		{
			float_value = (float *)pmsgScreen.line_item[MenuLine];
			sprintf(stringconversion, "%.3f\0", *float_value);
			break;
		}
		case TYPE_UNITS:
		{
			UINT8_value = (UINT8 *)pmsgScreen.line_item[MenuLine];
			CreateUnitsList(*UINT8_value, (char *)stringconversion);
			break;
		}
		case TYPE_LANGUAGE_SELECT:
		{
			UINT8_value = (UINT8 *)pmsgScreen.line_item[MenuLine];
			CreateLanguageList(*UINT8_value, (char *)stringconversion);
			break;
		}
		case TYPE_INSTRUMENT_FUNCTION:
		{
			UINT8_value = (UINT8 *)pmsgScreen.line_item[MenuLine];
			CreateInstrumentFunctionsList(*UINT8_value, (char *)stringconversion);
			break;
		}
		default:
		{
			sprintf(stringconversion, "ERROR\0");
			break;
		}
	}

	current_line = 1;
	FormatLine(current_line++, stringconversion, 0, NULL, 0);

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = FALSE;																				// was TRUE (Test v0.00.01)
	LastMenuLine = 0;
	MenuLine = 2;																					// was 1 (Test v0.00.01)
	editScreen = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateEditLine(void)
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

void CreateEditLine(void)
{
	char tempstring[20];
	char stringconversion[29];
	char current_line;
	char response;
	INT8 *INT8_value;
	UINT8 *UINT8_value;
	UINT16 *UINT16_value;

	Unconvert_Display(DisplayMessage, pmsgScreen.txt_text[MenuLine], 28);
	pmsgScreen.parent_menu = pmsgScreen.current_menu;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 2;

	switch(pmsgScreen.line_item_type[MenuLine])
	{
		case TYPE_DATE_MONTH:
		{
			pmsgScreen.minimum_value = 1;
			pmsgScreen.maximum_value = 12;
			UINT8_value = (unsigned char *)pmsgScreen.line_item[MenuLine];
			CreateMonthList(*UINT8_value, (char *)stringconversion);
			SubMenuLine = *UINT8_value;
			break;
		}
		case TYPE_DATE_DAY:
		{
			pmsgScreen.minimum_value = 1;
			pmsgScreen.maximum_value = 31;
			UINT8_value = (unsigned char *)pmsgScreen.line_item[MenuLine];
			sprintf(stringconversion, "%02d\0", *UINT8_value);
			SubMenuLine = *UINT8_value;
			break;
		}
		case TYPE_DATE_YEAR:
		{
			pmsgScreen.minimum_value = 0;
			pmsgScreen.maximum_value = 99;
			UINT8_value = (unsigned char *)pmsgScreen.line_item[MenuLine];
			sprintf(stringconversion, "20%02d\0", *UINT8_value);
			SubMenuLine = *UINT8_value;
			break;
		}
		case TYPE_TIME_HOURS:
		{
			pmsgScreen.minimum_value = 0;
			pmsgScreen.maximum_value = 23;
			UINT8_value = (unsigned char *)pmsgScreen.line_item[MenuLine];
			sprintf(stringconversion, "%02d\0", *UINT8_value);
			SubMenuLine = *UINT8_value;
			break;
		}
		case TYPE_TIME_MINUTES:
		case TYPE_TIME_SECONDS:
		{
			pmsgScreen.minimum_value = 0;
			pmsgScreen.maximum_value = 59;
			UINT8_value = (unsigned char *)pmsgScreen.line_item[MenuLine];
			sprintf(stringconversion, "%02d\0", *UINT8_value);
			SubMenuLine = *UINT8_value;
			break;
		}
		case TYPE_TIME_FORM:
		{
			pmsgScreen.minimum_value = 12;
			pmsgScreen.maximum_value = 24;
			UINT8_value = (unsigned char *)pmsgScreen.line_item[MenuLine];
			sprintf(stringconversion, "%02d\0", *UINT8_value);
			SubMenuLine = *UINT8_value;
			break;
		}
		case TYPE_PMODE_BACKLIGHT:
		{
			pmsgScreen.minimum_value = 0;
			pmsgScreen.maximum_value = 5;
			UINT8_value = (unsigned char *)pmsgScreen.line_item[MenuLine];
			CreateOperatingModeList(*UINT8_value, (char *)stringconversion);
			SubMenuLine = *UINT8_value;
			break;
		}
		case TYPE_PMODE_AUDIO:
		case TYPE_PMODE_GPS:
		case TYPE_PMODE_BLUETOOTH:
		case TYPE_PMODE_LOGGING:
		{
			pmsgScreen.minimum_value = 0;
			pmsgScreen.maximum_value = 3;
			UINT8_value = (unsigned char *)pmsgScreen.line_item[MenuLine];
			CreatePowerUpModeList(*UINT8_value, (char *)stringconversion);
			SubMenuLine = *UINT8_value;
			break;
		}
#ifdef EEPROM_DETECTOR_PROPERTIES_LOCATITRUE
		case TYPE_PMODE_DETECTOR:
		{
			if(NUMBER_OF_INTERNAL_DETECTORS)
				pmsgScreen.minimum_value = 0;
			else
				pmsgScreen.minimum_value = 1;
			if(NUMBER_OF_EXTERNAL_DETECTORS > 1)
				pmsgScreen.maximum_value = 6;
			else
				pmsgScreen.maximum_value = 1;
			UINT8_value = (unsigned char *)pmsgScreen.line_item[MenuLine];
			CreateDetectorPowerUpModeList(*UINT8_value, (char *)stringconversion);
			SubMenuLine = *UINT8_value;
			break;
		}
#endif
		case TYPE_PMODE_DISPLAY:
		{
			pmsgScreen.minimum_value = 0;
			pmsgScreen.maximum_value = 5;
			UINT8_value = (unsigned char *)pmsgScreen.line_item[MenuLine];
			CreateCurrentViewList(*UINT8_value, (char *)stringconversion);
			SubMenuLine = *UINT8_value;
			break;
		}
		case TYPE_CURRENT_VIEW:
		{
			pmsgScreen.minimum_value = 0;
			pmsgScreen.maximum_value = 3;
			UINT8_value = (unsigned char *)pmsgScreen.line_item[MenuLine];
			CreateCurrentViewList(*UINT8_value, (char *)stringconversion);
			SubMenuLine = *UINT8_value;
			break;
		}
		case TYPE_OMODE_BACKLIGHT:
		{
			pmsgScreen.minimum_value = 0;
			pmsgScreen.maximum_value = 3;
			UINT8_value = (unsigned char *)pmsgScreen.line_item[MenuLine];
			CreateOperatingModeList(*UINT8_value, (char *)stringconversion);
			SubMenuLine = *UINT8_value;
			break;
		}
		case TYPE_OMODE_AUDIO:
		{
			pmsgScreen.minimum_value = 0;
			pmsgScreen.maximum_value = 5;
			UINT8_value = (unsigned char *)pmsgScreen.line_item[MenuLine];
			CreateAudioOperatingModeList(*UINT8_value, (char *)stringconversion);
			SubMenuLine = *UINT8_value;
			break;
		}
		case TYPE_ENABLE:
		case TYPE_OMODE_GPS:
		case TYPE_OMODE_BLUETOOTH:
		{
			pmsgScreen.minimum_value = 0;
			pmsgScreen.maximum_value = 1;
			UINT8_value = (unsigned char *)pmsgScreen.line_item[MenuLine];
			CreateOperatingModeList(*UINT8_value, (char *)stringconversion);
			SubMenuLine = *UINT8_value;
			break;
		}
		case TYPE_100_PERCENT:
		{
			pmsgScreen.minimum_value = 0;
			pmsgScreen.maximum_value = 100;
			UINT8_value = (unsigned char *)pmsgScreen.line_item[MenuLine];
			CreatePercentageList(*UINT8_value, (char *)stringconversion);
			SubMenuLine = *UINT8_value;
			break;
		}
		case TYPE_COLOR:
		{
			pmsgScreen.minimum_value = 0;
			pmsgScreen.maximum_value = 28;
			UINT8_value = (unsigned char *)pmsgScreen.line_item[MenuLine];
			CreateColorList(*UINT8_value, (char *)stringconversion);
			SubMenuLine = *UINT8_value;
			break;
		}
		case TYPE_3_MINUTES:
		{
			pmsgScreen.minimum_value = 1;
			pmsgScreen.maximum_value = 180;
			UINT8_value = (unsigned char *)pmsgScreen.line_item[MenuLine];
			sprintf(stringconversion, "%d\0", *UINT8_value);
			SubMenuLine = *UINT8_value;
			break;
		}
		case TYPE_MULTIPLIER:
		case TYPE_INTEGRATE_MULTIPLIER:
		{
			pmsgScreen.minimum_value = MICRO;
			pmsgScreen.maximum_value = MEGA;
			UINT8_value = (unsigned char *)pmsgScreen.line_item[MenuLine];
			CreateMultiplierList(*UINT8_value, (char *)stringconversion);
			SubMenuLine = *UINT8_value;
			break;
		}
		case TYPE_UNITS:
		{
			pmsgScreen.minimum_value = 0;
			pmsgScreen.maximum_value = END_UNIT_TYPE - 1;
			UINT8_value = (unsigned char *)pmsgScreen.line_item[MenuLine];
			CreateUnitsList(*UINT8_value, (char *)stringconversion);
			SubMenuLine = *UINT8_value;
			break;
		}
		case TYPE_INTEGRATE_UNITS:
		{
			pmsgScreen.minimum_value = R;
			pmsgScreen.maximum_value = GY;
			UINT8_value = (unsigned char *)pmsgScreen.line_item[MenuLine];
			CreateUnitsList(*UINT8_value, (char *)stringconversion);
			SubMenuLine = *UINT8_value;
			break;
		}
		case TYPE_1_HOUR:
		{
			pmsgScreen.minimum_value = 1;
			pmsgScreen.maximum_value = 3600;
			UINT16_value = (UINT16 *)pmsgScreen.line_item[MenuLine];
			sprintf(stringconversion, "%d\0", *UINT16_value);
			SubMenuLine = *UINT16_value;
			break;
		}
		case TYPE_VALUE:
		{
			pmsgScreen.minimum_value = 0;
			pmsgScreen.maximum_value = 999;
			UINT16_value = (UINT16 *)pmsgScreen.line_item[MenuLine];
			sprintf(stringconversion, "%u\0", *UINT16_value);
			SubMenuLine = *UINT16_value;
			break;
		}
		case TYPE_MODEL_NUMBER:
		{
			pmsgScreen.minimum_value = 1;
			pmsgScreen.maximum_value = 3;
			CreateModelList(lmi.settings.product.info.model_number, (char *)stringconversion);
			SubMenuLine = lmi.settings.product.info.model_number;
			break;
		}
		case TYPE_LANGUAGE_SELECT:
		{
			pmsgScreen.minimum_value = 1;
			pmsgScreen.maximum_value = 1;
			CreateLanguageList(lmi.settings.language.info.index, (char *)stringconversion);
			SubMenuLine = lmi.settings.language.info.index;
			break;
		}
		case TYPE_VIEW_FUNCTION:
		{
			pmsgScreen.minimum_value = -1;
			pmsgScreen.maximum_value = 3;
			INT8_value = (char *)pmsgScreen.line_item[MenuLine];
			CreateViewFunctionList(*INT8_value, (char *)stringconversion);
			SubMenuLine = *INT8_value;
			break;
		}
		case TYPE_INSTRUMENT_FUNCTION:
		{
			pmsgScreen.minimum_value = 50;
			pmsgScreen.maximum_value = 60;
			UINT8_value = (UINT8 *)pmsgScreen.line_item[MenuLine];
			CreateInstrumentFunctionsList(*UINT8_value, (char *)stringconversion);
			SubMenuLine = *UINT8_value;
			break;
		}
#ifdef EEPROM_DETECTOR_PROPERTIES_LOCATITRUE
		case TYPE_DETECTOR:
		{
			if(NUMBER_OF_INTERNAL_DETECTORS)
				pmsgScreen.minimum_value = 0;
			else
				pmsgScreen.minimum_value = 1;
			if(NUMBER_OF_EXTERNAL_DETECTORS > 1)
				pmsgScreen.maximum_value = 4;
			else
				pmsgScreen.maximum_value = 1;
			UINT8_value = (unsigned char *)pmsgScreen.line_item[MenuLine];
			CreateDetectorPowerUpModeList(*UINT8_value, (char *)stringconversion);
			SubMenuLine = *UINT8_value;
			break;
		}
#endif
		case TYPE_OFFSET:
		{
			pmsgScreen.minimum_value = 1000;
			pmsgScreen.maximum_value = 2000;
			UINT16_value = (UINT16 *)pmsgScreen.line_item[MenuLine];
			sprintf(stringconversion, "%d\0", *UINT16_value);
			SubMenuLine = *UINT16_value;
			break;
		}
		case TYPE_RANGE:
		{
			pmsgScreen.minimum_value = 1;
			pmsgScreen.maximum_value = 5;
			UINT8_value = (UINT8 *)pmsgScreen.line_item[MenuLine];
			CreateRangeList(*UINT8_value, (char *)stringconversion);
			SubMenuLine = *UINT8_value;
			break;
		}
		default:
		{
			sprintf(stringconversion, "ERROR\0");
			SubMenuLine = 1;
			break;
		}
	}

	current_line = 1;
	Convert_For_Display(pmsgScreen.txt_text[current_line], NULL, stringconversion, 28);
	pmsgScreen.sub_menu[current_line] = 0;
	pmsgScreen.line_item[current_line] = pmsgScreen.line_item[MenuLine];
	pmsgScreen.line_item_type[current_line++] = pmsgScreen.line_item_type[MenuLine];

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = TRUE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateEditStringLine(void)
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

void CreateEditStringLine(void)
{
	char stringconversion[29];
	char current_line;

	Unconvert_Display(DisplayMessage, pmsgScreen.txt_text[MenuLine], 28);
	pmsgScreen.parent_menu = pmsgScreen.current_menu;
	pmsgScreen.current_menu = screenState;
	pmsgScreen.txt_total_lines = 2;

	switch(pmsgScreen.line_item_type[MenuLine])
	{
		case TYPE_MODEL_NUMBER:
		case TYPE_STRING:
		{
			pmsgScreen.minimum_value = 0;
			pmsgScreen.maximum_value = 16;
//			sprintf(stringconversion, "%s\0",(unsigned char *)pmsgScreen.line_item[MenuLine]);
			FormatString(stringconversion, (unsigned char *)pmsgScreen.line_item[MenuLine], 16);						// (9DP v1.01.04)
			Total_Chars = strlen(stringconversion);
			Last_Total_Chars = -1;
			break;
		}
		default:
		{
			sprintf(stringconversion, "ERROR\0");
			SubMenuLine = 1;
			break;
		}
	}

	current_line = 1;
	Convert_For_Display(pmsgScreen.txt_text[current_line], NULL, stringconversion, 28);
	pmsgScreen.sub_menu[current_line] = 0;
	pmsgScreen.line_item[current_line] = pmsgScreen.line_item[MenuLine];
	pmsgScreen.line_item_type[current_line++] = pmsgScreen.line_item_type[MenuLine];

	strcpy(stringconversion, MessageBack);
	FormatLine(current_line++, stringconversion, pmsgScreen.parent_menu, NULL, 0);

	MenuSelect = TRUE;
	LastMenuLine = 0;
	MenuLine = 1;
	editScreen = TRUE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	END OF CREATE LINES
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//*	EDIT LINES
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void ChangeMenuLine(void)	
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

void ChangeMenuLine(void)
{
	SHORT temp_color;

	if(MenuLine != LastMenuLine)
	{
		if(LastMenuLine != 0)
		{
			temp_color = pmsgScreen.txt_color_line[LastMenuLine];
			pmsgScreen.txt_color_line[LastMenuLine] = pmsgScreen.txt_color_line[MenuLine];
			pmsgScreen.txt_color_line[MenuLine] = temp_color;
			SetColor(WHITE);
			Bar(pmsgScreen.x_line[LastMenuLine], pmsgScreen.y_line[LastMenuLine], pmsgScreen.box_x2, (pmsgScreen.y_line[LastMenuLine] + 24));
			MoveTo(pmsgScreen.x_line[LastMenuLine], pmsgScreen.y_line[LastMenuLine]);																// move to starting location
			SetFont(pmsgScreen.font[LastMenuLine]);																									// set corresponding font style
			SetColor(pmsgScreen.txt_color_line[LastMenuLine]);																						// set corresponding line color
			while(!OutText((XCHAR *)pmsgScreen.txt_text[LastMenuLine]));																			// output text from struct
	
			pmsgScreen.box_y1 = pmsgScreen.box_y1 + ((MenuLine - 1) * 12);
		}	
		else
		{
			pmsgScreen.txt_color_line[MenuLine] = BLACK;
		}

		SetColor(LIGHTGREEN);
		Bar(pmsgScreen.x_line[MenuLine], pmsgScreen.y_line[MenuLine], pmsgScreen.box_x2, (pmsgScreen.y_line[MenuLine] + 24));

		MoveTo(pmsgScreen.x_line[MenuLine], pmsgScreen.y_line[MenuLine]);																			// move to starting location
		SetFont(pmsgScreen.font[MenuLine]);																											// set corresponding font style
		SetColor(pmsgScreen.txt_color_line[MenuLine]);																								// set corresponding line color
		while(!OutText((XCHAR *)pmsgScreen.txt_text[MenuLine]));																					// output text from struct

		LastMenuLine = MenuLine;
	}
#ifdef TEST_MODE
	else																																			// (Test v0.00.01)
	{																																				// (Test v0.00.01)
		if((MenuSelect == FALSE) && (Test_Update == TRUE) && (screenState != DISPLAY_TEST_KEYPAD))													// (Test v0.00.03)
		{																																			// (Test v0.00.01)
			SetColor(WHITE);																														// (Test v0.00.01)
			Bar(pmsgScreen.x_line[1], pmsgScreen.y_line[1], pmsgScreen.box_x2, (pmsgScreen.y_line[1] + 24));										// (Test v0.00.01)
			MoveTo(pmsgScreen.x_line[1], pmsgScreen.y_line[1]);																						// (Test v0.00.01)
			SetFont(pmsgScreen.font[1]);																											// (Test v0.00.01)
			SetColor(pmsgScreen.txt_color_line[1]);																									// (Test v0.00.01)
			while(!OutText((XCHAR *)pmsgScreen.txt_text[1]));																						// (Test v0.00.01)
			Test_Update = FALSE;																													// (Test v0.00.04)
		}																																			// (Test v0.00.01)
	}																																				// (Test v0.00.01)
#endif

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void ChangeEditLine(void)	
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

void ChangeEditLine(void)
{
	char stringconversion[29];
	SHORT temp_foreground_color;
	SHORT temp_background_color;

	if(editMode == TRUE)
	{
		temp_background_color = BLACK;
		temp_foreground_color = WHITE;
	}
	else
	{
		temp_background_color = LIGHTGREEN;
		temp_foreground_color = BLACK;
	}
	SetColor(temp_background_color);
	Bar(pmsgScreen.x_line[1], pmsgScreen.y_line[1], pmsgScreen.box_x2, (pmsgScreen.y_line[1] + 24));
	MoveTo(pmsgScreen.x_line[1], pmsgScreen.y_line[1]);												// move to starting location
	SetFont(pmsgScreen.font[1]);																	// set corresponding font style
	SetColor(temp_foreground_color);
	while(!OutText((XCHAR *)pmsgScreen.txt_text[1]));												// output text from struct

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void ChangeEditStringLine(void)	
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

void ChangeEditStringLine(void)
{
	char stringconversion[29];
	SHORT temp_foreground_color;
	SHORT temp_background_color;

	if(editMode == TRUE)
	{
		temp_background_color = BLACK;
		temp_foreground_color = WHITE;
	}
	else
	{
		temp_background_color = LIGHTGREEN;
		temp_foreground_color = BLACK;
	}
	SetColor(temp_background_color);
	Bar(pmsgScreen.x_line[1], pmsgScreen.y_line[1], pmsgScreen.box_x2, (pmsgScreen.y_line[1] + 24));
	MoveTo(pmsgScreen.x_line[1], pmsgScreen.y_line[1]);												// move to starting location
	SetFont(pmsgScreen.font[1]);																	// set corresponding font style
	SetColor(temp_foreground_color);
	while(!OutText((XCHAR *)pmsgScreen.txt_text[1]));												// output text from struct

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	END OF EDIT LINES
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//*	LISTS
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateMonthList(char generic_value, char *generic_string)
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

void CreateMonthList(char generic_value, char *generic_string)
{
	switch(generic_value)
	{
		case JANUARY:
		{
			strcpy(generic_string,"January\0");
			break;
		}
		case FEBRUARY:
		{
			strcpy(generic_string,"February\0");
			break;
		}
		case MARCH:
		{
			strcpy(generic_string,"March\0");
			break;
		}
		case APRIL:
		{
			strcpy(generic_string,"April\0");
			break;
		}
		case MAY:
		{
			strcpy(generic_string,"May\0");
			break;
		}
		case JUNE:
		{
			strcpy(generic_string,"June\0");
			break;
		}
		case JULY:
		{
			strcpy(generic_string,"July\0");
			break;
		}
		case AUGUST:
		{
			strcpy(generic_string,"August\0");
			break;
		}
		case SEPTEMBER:
		{
			strcpy(generic_string,"September\0");
			break;
		}
		case OCTOBER:
		{
			strcpy(generic_string,"October\0");
			break;
		}
		case NOVEMBER:
		{
			strcpy(generic_string,"November\0");
			break;
		}
		case DECEMBER:
		{
			strcpy(generic_string,"December\0");
			break;
		}
		default:
		{
			strcpy(generic_string,"Error\0");
			break;
		}
	}
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateShortMonthList(char generic_value, char *generic_string)
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
//* 	(9DP v1.03.00)
//*

void CreateShortMonthList(char generic_value, char *generic_string)
{
	switch(generic_value)
	{
		case JANUARY:
		{
			strcpy(generic_string,"JAN\0");
			break;
		}
		case FEBRUARY:
		{
			strcpy(generic_string,"FEB\0");
			break;
		}
		case MARCH:
		{
			strcpy(generic_string,"MAR\0");
			break;
		}
		case APRIL:
		{
			strcpy(generic_string,"APR\0");
			break;
		}
		case MAY:
		{
			strcpy(generic_string,"MAY\0");
			break;
		}
		case JUNE:
		{
			strcpy(generic_string,"JUN\0");
			break;
		}
		case JULY:
		{
			strcpy(generic_string,"JUL\0");
			break;
		}
		case AUGUST:
		{
			strcpy(generic_string,"AUG\0");
			break;
		}
		case SEPTEMBER:
		{
			strcpy(generic_string,"SEP\0");
			break;
		}
		case OCTOBER:
		{
			strcpy(generic_string,"OCT\0");
			break;
		}
		case NOVEMBER:
		{
			strcpy(generic_string,"NOV\0");
			break;
		}
		case DECEMBER:
		{
			strcpy(generic_string,"DEC\0");
			break;
		}
		default:
		{
			strcpy(generic_string,"ERR\0");
			break;
		}
	}
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateLanguageList(char generic_value, char *generic_string)
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

void CreateLanguageList(char generic_value, char *generic_string)
{
	switch(generic_value)
	{
		case ENGLISH:
		{
			strcpy(generic_string,"English\0");
			break;
		}
		default:
		{
			strcpy(generic_string,"Error\0");
			break;
		}
	}
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreatePowerUpModeList(char power_up_mode, char *generic_string)
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

void CreatePowerUpModeList(char generic_value, char *generic_string)
{
	switch(generic_value)
	{
		case POWERUP_OFF:																			// powerup in the off state
		{
			strcpy(generic_string,"Off\0");
			break;
		}
		case POWERUP_ON:																			// powerup in the on state
		{
			strcpy(generic_string,"On\0");
			break;
		}
		case POWERUP_DEFAULT:																		// powerup in the default state
		{
			strcpy(generic_string,"Default\0");
			break;
		}
		case POWERUP_LAST:																			// powerup in the last known state
		{
			strcpy(generic_string,"Last\0");
			break;
		}
		default:
		{
			strcpy(generic_string,"ERROR\0");
			break;
		}
	}
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateDetectorPowerUpModeList(char generic_value, char *generic_string)
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

#ifdef EEPROM_DETECTOR_PROPERTIES_LOCATITRUE

void CreateDetectorPowerUpModeList(char generic_value, char *generic_string)
{
	char detectorconversion[29];

	switch(generic_value)
	{
		case INTERNAL:																				// powerup using internal detector
		{
			strcpy(generic_string,"Internal\0");
			break;
		}
		case DET_1:																					// powerup using detector 1
		case DET_2:																					// powerup using detector 2
		case DET_3:																					// powerup using detector 3
		case DET_4:																					// powerup using detector 4
		{
			CreateDetectorList(lmi.display.detector[generic_value].properties.info.model, (char *)detectorconversion);
			sprintf(generic_string,"Det %d: %s\0", generic_value, detectorconversion);
			break;
		}
		case DEFAULT_DET:																			// powerup using the default detector
		{
			strcpy(generic_string,"Default\0");
			break;
		}
		case LAST_DET:																				// powerup using the last known detector
		{
			strcpy(generic_string,"Last\0");
			break;
		}
		default:
		{
			strcpy(generic_string,"ERROR\0");
			break;
		}
	}
	return;
}

#endif

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateOperatingModeList(char generic_value, char *generic_string)
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

void CreateOperatingModeList(char generic_value, char *generic_string)
{
	switch(generic_value)
	{
		case FALSE:
		{
			strcpy(generic_string,"Off\0");
			break;
		}
		case TRUE:
		{
			strcpy(generic_string,"On\0");
			break;
		}
		case BK_LIGHT_AUTO:
		{
			strcpy(generic_string,"Auto\0");
			break;
		}
		case BK_LIGHT_TEMP_ON:
		{
			strcpy(generic_string,"Temporary\0");
			break;
		}
		case BK_LIGHT_DEFAULT:																		// powerup in the default state
		{
			strcpy(generic_string,"Default\0");
			break;
		}
		case BK_LIGHT_LAST:																			// powerup in the last known state
		{
			strcpy(generic_string,"Last\0");
			break;
		}
		default:
		{
			strcpy(generic_string,"ERROR\0");
			break;
		}
	}
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateAudioOperatingModeList(char generic_value, char *generic_string)
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

void CreateAudioOperatingModeList(char generic_value, char *generic_string)
{
	switch(generic_value)
	{
		case AUDIO_0:
		{
			strcpy(generic_string,"Off\0");
			break;
		}
		case AUDIO_20:
		{
			strcpy(generic_string,"20%\0");
			break;
		}
		case AUDIO_40:
		{
			strcpy(generic_string,"40%\0");
			break;
		}
		case AUDIO_60:
		{
			strcpy(generic_string,"60%\0");
			break;
		}
		case AUDIO_80:
		{
			strcpy(generic_string,"80%\0");
			break;
		}
		case AUDIO_100:
		{
			strcpy(generic_string,"100%\0");
			break;
		}
		default:
		{
			strcpy(generic_string,"ERROR\0");
			break;
		}
	}
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreatePercentageList(char generic_value, char *generic_string)
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

void CreatePercentageList(char generic_value, char *generic_string)
{
	if((UINT8)generic_value <= 100)
	{
		sprintf(generic_string,"%d%%\0",generic_value);
	}
	else
	{
		strcpy(generic_string,"ERROR\0");
	}
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateColorList(char generic_value, char *generic_string)
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

void CreateColorList(char generic_value, char *generic_string)
{
	switch(generic_value)
	{
		case BLACK_EEPROM:																			// 0
		{
			strcpy(generic_string,"Black\0");
			break;
		}
		case BRIGHTBLUE_EEPROM:																		// 1
		{
			strcpy(generic_string,"Bright Blue\0");
			break;
		}
		case BRIGHTGREEN_EEPROM:																	// 2
		{
			strcpy(generic_string,"Bright Green\0");
			break;
		}
		case BRIGHTCYAN_EEPROM:																		// 3
		{
			strcpy(generic_string,"Bright Cyan\0");
			break;
		}
		case BRIGHTRED_EEPROM:																		// 4
		{
			strcpy(generic_string,"Bright Red\0");
			break;
		}
		case BRIGHTMAGENTA_EEPROM: 																	// 5
		{
			strcpy(generic_string,"Bright Magenta\0");
			break;
		}
		case BRIGHTYELLOW_EEPROM:																	// 6
		{
			strcpy(generic_string,"Bright Yellow\0");
			break;
		}
		case BLUE_EEPROM:																			// 7
		{
			strcpy(generic_string,"Blue\0");
			break;
		}
		case GREEN_EEPROM:																			// 8 
		{
			strcpy(generic_string,"Green\0");
			break;
		}
		case CYAN_EEPROM:																			// 9
		{
			strcpy(generic_string,"Cyan\0");
			break;
		}
		case RED_EEPROM:																			// 10
		{
			strcpy(generic_string,"Red\0");
			break;
		}
		case MAGENTA_EEPROM:																		// 11
		{
			strcpy(generic_string,"Magenta\0");
			break;
		}
		case BROWN_EEPROM:																			// 12
		{
			strcpy(generic_string,"Brown\0");
			break;
		}
		case LIGHTGRAY_EEPROM:																		// 13
		{
			strcpy(generic_string,"Light Gray\0");
			break;
		}
		case DARKGRAY_EEPROM:																		// 14
		{
			strcpy(generic_string,"Dark Gray\0");
			break;
		}
		case LIGHTBLUE_EEPROM:																		// 15
		{
			strcpy(generic_string,"Light Blue\0");
			break;
		}
		case LIGHTGREEN_EEPROM:																		// 16
		{
			strcpy(generic_string,"Light Green\0");
			break;
		}
		case LIGHTCYAN_EEPROM:																		// 17
		{
			strcpy(generic_string,"Light Cyan\0");
			break;
		}
		case LIGHTRED_EEPROM:																		// 18
		{
			strcpy(generic_string,"Light Red\0");
			break;
		}
		case LIGHTMAGENTA_EEPROM:																	// 19	
		{
			strcpy(generic_string,"Light Magenta\0");
			break;
		}
		case YELLOW_EEPROM:																			// 20
		{
			strcpy(generic_string,"Yellow\0");
			break;
		}
		case WHITE_EEPROM:																			// 21
		{
			strcpy(generic_string,"White\0");
			break;
		}
		case GRAY0_EEPROM:																			// 22
		{
			strcpy(generic_string,"Gray 0\0");
			break;
		}
		case GRAY1_EEPROM:																			// 23
		{
			strcpy(generic_string,"Gray 1\0");
			break;
		}
		case GRAY2_EEPROM:																			// 24
		{
			strcpy(generic_string,"Gray 2\0");
			break;
		}
		case GRAY3_EEPROM:																			// 25
		{
			strcpy(generic_string,"Gray 3\0");
			break;
		}
		case GRAY4_EEPROM:																			// 26
		{
			strcpy(generic_string,"Gray 4\0");
			break;
		}
		case GRAY5_EEPROM:																			// 27
		{
			strcpy(generic_string,"Gray 5\0");
			break;
		}
		case GRAY6_EEPROM:																			// 28
		{
			strcpy(generic_string,"Gray 6\0");
			break;
		}
		default:																					// > 28
		{
			strcpy(generic_string,"ERROR\0");
			break;
		}
	}
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateCurrentViewList(char generic_value, char *generic_string)
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

void CreateCurrentViewList(char generic_value, char *generic_string)
{
	char stringconversion[29];

	switch(generic_value)
	{
		case VIEW_1:
		case VIEW_2:
		case VIEW_3:
		case VIEW_4:
		{
			sprintf(stringconversion,"View %d: %s\0",generic_value + 1, lmi.display.detector[DetectorMenu].view[generic_value].element_header.info.name);
			strcpy(generic_string,stringconversion);
			break;
		}
		case VIEW_DEFAULT:
		{
			strcpy(generic_string,"Default\0");
			break;
		}
		case VIEW_LAST:
		{
			strcpy(generic_string,"Last\0");
			break;
		}
		default:
		{
			strcpy(generic_string,"ERROR\0");
			break;
		}
	}
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateMultiplierList(char generic_value, char *generic_string)
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

void CreateMultiplierList(char generic_value, char *generic_string)
{
	switch(generic_value)
	{
		case NO_MULTIPLIER:
		{
			strcpy(generic_string,"<NTRUEE>\0");
			break;
		}
		case ZEPTO:
		{
			strcpy(generic_string,"ZEPTO (1E-21)\0");
			break;
		}
		case ATTO:
		{
			strcpy(generic_string,"ATTO (1E-18)\0");
			break;
		}
		case FEMTO:
		{
			strcpy(generic_string,"FEMPTO (1E-15)\0");
			break;
		}
		case PICO:
		{
			strcpy(generic_string,"PICO (1E-12)\0");
			break;
		}
		case NANO:
		{
			strcpy(generic_string,"NANO (1E-09)\0");
			break;
		}
		case MICRO:
		{
			strcpy(generic_string,"MICRO (1E-06)\0");
			break;
		}
		case MILLI:
		{
			strcpy(generic_string,"MILLI (1E-03)\0");
			break;
		}
		case UNITY:
		{
			strcpy(generic_string,"UNITY (1E+00)\0");
			break;
		}
		case KILO:
		{
			strcpy(generic_string,"KILO (1E+03)\0");
			break;
		}
		case MEGA:
		{
			strcpy(generic_string,"MEGA (1E+06)\0");
			break;
		}
		case GIGA:
		{
			strcpy(generic_string,"GIGA (1E+09)\0");
			break;
		}
		case TERA:
		{
			strcpy(generic_string,"TERA (1E+12)\0");
			break;
		}
		case PETA:
		{
			strcpy(generic_string,"PETA (1E+15)\0");
			break;
		}
		case EXA:
		{
			strcpy(generic_string,"EXA (1E+18)\0");
			break;
		}
		case ZETTA:
		{
			strcpy(generic_string,"ZETTA (1E+21)\0");
			break;
		}
		default:
		{
			strcpy(generic_string,"ERROR\0");
			break;
		}
	}
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateUnitsList(char generic_value, char *generic_string)
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

void CreateUnitsList(char generic_value, char *generic_string)
{
	switch(generic_value)
	{
		case NO_UNITS:
		{
			strcpy(generic_string,"<NTRUEE>\0");
			break;
		}
		case R_HR:
		{
			strcpy(generic_string,"R/hr\0");
			break;
		}
		case SV_HR:
		{
//			strcpy(generic_string,"Sv/hr\0");
			strcpy(generic_string,"Sv/h\0");														// (9DP v1.01.12)
			break;
		}
		case REM_HR:
		{
			strcpy(generic_string,"REM/hr\0");
			break;
		}
		case GY_HR:
		{
//			strcpy(generic_string,"Gy/hr\0");
			strcpy(generic_string,"Gy/h\0");														// (9DP v1.01.12)
			break;
		}
		case CPS:
		{
			strcpy(generic_string,"CPS\0");
			break;
		}
		case CPM:
		{
			strcpy(generic_string,"CPM\0");
			break;
		}
		case DPS:
		{
			strcpy(generic_string,"DPS\0");
			break;
		}
		case DPM:
		{
			strcpy(generic_string,"DPM\0");
			break;
		}
		case BQ:
		{
			strcpy(generic_string,"Bq\0");
			break;
		}
		case R:
		{
			strcpy(generic_string,"R\0");
			break;
		}
		case SV:
		{
			strcpy(generic_string,"Sv\0");
			break;
		}
		case REM:
		{
			strcpy(generic_string,"REM\0");
			break;
		}
		case GY:
		{
			strcpy(generic_string,"Gy\0");
			break;
		}
		case COUNTS:
		{
			strcpy(generic_string,"Counts\0");
			break;
		}
		case DISINTEGRATIONS:
		{
			strcpy(generic_string,"Disintegrations\0");
			break;
		}
		default:
		{
			strcpy(generic_string,"ERROR\0");
			break;
		}
	}

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateModelList(char generic_value, char *generic_string)
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

void CreateModelList(char generic_value, char *generic_string)
{
	switch(generic_value)
	{
		case MODEL_9DP:
		{
			strcpy(generic_string,"9DP\0");
			break;
		}
		case MODEL_9DP_PLUS:
		{
			strcpy(generic_string,"9DP+\0");
			break;
		}
		case MODEL_9DP_1:
		{
			strcpy(generic_string,"9DP-1\0");
			break;
		}
		case MODEL_9DP_1_PLUS:
		{
			strcpy(generic_string,"9DP-1+\0");
			break;
		}
		case MODEL_9DLP:
		{
			strcpy(generic_string,"9DLP\0");
			break;
		}
		case MODEL_9DLP_PLUS:
		{
			strcpy(generic_string,"9DLP+\0");
			break;
		}
		case MODEL_V1:																				// (9DP v1.01.21)
		{																							// (9DP v1.01.21)
			strcpy(generic_string,"v1\0");															// (9DP v1.01.21)
			break;																					// (9DP v1.01.21)
		}																							// (9DP v1.01.21)
		case MODEL_9DP_STAR:																		// (9DP v1.01.24)
		{																							// (9DP v1.01.24)
			strcpy(generic_string,"9DP*\0");														// (9DP v1.01.24)
			break;																					// (9DP v1.01.24)
		}																							// (9DP v1.01.24)
//		case MODEL_9DP_STAR_PLUS:																	// (9DP v1.01.24)
//		{																							// (9DP v1.01.24)
//			strcpy(generic_string,"9DP*+\0");														// (9DP v1.01.24)
//			break;																					// (9DP v1.01.24)
//		}																							// (9DP v1.01.24)
		case MODEL_9DP_2:																			// (9DP v1.01.24)
		{																							// (9DP v1.01.24)
			strcpy(generic_string,"9DP-2\0");														// (9DP v1.01.24)
			break;																					// (9DP v1.01.24)
		}																							// (9DP v1.01.24)
//		case MODEL_9DP_2_PLUS:																		// (9DP v1.01.24)
//		{																							// (9DP v1.01.24)
//			strcpy(generic_string,"9DP-2\0");														// (9DP v1.01.24)
//			break;																					// (9DP v1.01.24)
//		}																							// (9DP v1.01.24)
		case MODEL_3D:
		{
			strcpy(generic_string,"3D\0");
			break;
		}
		case MODEL_3D_PLUS:
		{
			strcpy(generic_string,"3D+\0");
			break;
		}
		case MODEL_276D:
		{
			strcpy(generic_string,"276D\0");
			break;
		}
		default:
		{
			strcpy(generic_string,"ERROR\0");
			break;
		}
	}
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateInstrumentFunctionsList(UINT8 generic_value, char *generic_string)
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

void CreateInstrumentFunctionsList(UINT8 generic_value, char *generic_string)
{
	switch(generic_value)
	{
		case NO_FUNCTION:																			//  0 - empty function
		case NO_FUNCTION_X:																			// 50 - empty function
		{
			strcpy(generic_string,"No Function\0");
			break;
		}
		case INTEGRATE:																				//  1 - integrate
		{
			strcpy(generic_string,"Integrate\0");
			break;
		}
		case PEAK_RATE:																				//  2 - peak rate
		{
			strcpy(generic_string,"Peak Rate\0");
			break;
		}
		case TIMED_INTEGRATE:																		// 20 - timed integrate (dose)
		{
			strcpy(generic_string,"Timed Dose\0");
			break;
		}
		case PULSED_MODE:																			// 21 - pulsed mode
		{
			strcpy(generic_string,"Pulsed Mode\0");
			break;
		}
		case INPUT_VOLTAGE:																			// 51 - input voltage
		{
			strcpy(generic_string,"Input Voltage\0");
			break;
		}
		case PEAK_VOLTAGE:																			// 52 - peak voltage
		{
			strcpy(generic_string,"Peak Voltage\0");
			break;
		}
		case CLOCK_TIME:																			// 53 - time
		{
			strcpy(generic_string,"Time\0");
			break;
		}
		case CALENDAR_DATE:																			// 54 - date
		{
			strcpy(generic_string,"Date\0");
			break;
		}
		case CHAMBER_TEMP:																			// 55 - chamber temperature
		{
			strcpy(generic_string,"Chamber Temp.\0");
			break;
		}
		case INSTRUMENT_TEMP:																		// 56 - instrument temperature
		{
			strcpy(generic_string,"Instrument Temp.\0");
			break;
		}
		case HIGH_VOLTAGE:																			// 57 - high voltage
		{
			strcpy(generic_string,"High Voltage\0");
			break;
		}
		case TEMPERATURE_OFFSET:																	// 58 - temperature offset routine
		{
			strcpy(generic_string,"Temp. Offset\0");
			break;
		}
		case SET_RANGE:																				// 60 - force particular range
		{
			strcpy(generic_string,"Set Range\0");
			break;
		}
		case DEBUG_FUNCTION:																		// 99 - debug function
		{
			strcpy(generic_string,"Debug\0");
			break;
		}
		default:
		{
			strcpy(generic_string,"ERROR\0");
			break;
		}
	}
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateDetectorList(unsigned short generic_value, char *generic_string)
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

//#ifdef EEPROM_DETECTOR_PROPERTIES_LOCATITRUE

void CreateDetectorList(unsigned short generic_value, char *generic_string)
{
	switch(generic_value)
	{
		case NO_DETECTOR:
		{
			strcpy(generic_string,"No Detector\0");
			break;
		}
		case DETECTOR_9DP:
		{
			strcpy(generic_string,"9DP\0");
			break;
		}
		case DETECTOR_9DP_1:
		{
			strcpy(generic_string,"9DP-1\0");
			break;
		}
		case DETECTOR_9DLP:
		{
			strcpy(generic_string,"9DLP\0");
			break;
		}
		case DETECTOR_9D:
		{
			strcpy(generic_string,"9D\0");
			break;
		}
		case DETECTOR_44_9:
		{
			strcpy(generic_string,"44-9\0");
			break;
		}
		default:
		{
			strcpy(generic_string,"ERROR\0");
			break;
		}
	}
	return;
}

//#endif

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateViewFunctionList(char generic_value, char *generic_string)
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

void CreateViewFunctionList(char generic_value, char *generic_string)
{
	char stringconversion[29];

	switch(generic_value)
	{
		case 0:
		case 1:
		case 2:
		case 3:
		{
			CreateInstrumentFunctionsList(lmi.function[generic_value].info.header.type, (char *)stringconversion);
			strcpy(generic_string,stringconversion);
			break;
		}
		case -1:
		{
			strcpy(generic_string,"- empty -\0");
			break;
		}
		default:
		{
			strcpy(generic_string,"ERROR\0");
			break;
		}
	}
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateRangeList(char generic_value, char *generic_string)
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

void CreateRangeList(char generic_value, char *generic_string)
{
	sprintf(generic_string,"Range %d\0", generic_value);
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateList(char generic_value, char *generic_string)
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

//void CreateList(char generic_value, char *generic_string)
//{
//	switch(generic_value)
//	{
//		case :
//		{
//			strcpy(generic_string,"\0");
//			break;
//		}
//		case :
//		{
//			strcpy(generic_string,"\0");
//			break;
//		}
//		case :
//		{
//			strcpy(generic_string,"\0");
//			break;
//		}
//		default:
//		{
//			strcpy(generic_string,"ERROR\0");
//			break;
//		}
//	}
//	return;
//}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	END OF LISTS
//**************************************************************************************************
//**************************************************************************************************

//*
//**************************************************************************************************
//**************************************************************************************************
//* LUDLUM MEASUREMENTS INC. COPYRIGHT (C) 2010 
//**************************************************************************************************
//**************************************************************************************************
