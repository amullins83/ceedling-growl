//**************************************************************************************************
//**************************************************************************************************
//*																		
//*	Filename:		Keyboard_Menu.h
//*	Date:			01/09/12
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
//*	~~~~~~~~	01/09/12	James Comstock
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

#ifndef _KEYBOARD_MENU_INCLUDE_
#define _KEYBOARD_MENU_INCLUDE_

#include "GenericTypeDefs.h"

void CreateMenu(void);
void CreateMainMenu(void);
void CreatePasswordEntryMenu(void);

void CreateSettingsMenu(void);
void CreateProductMenu(void);
void CreateLanguageMenu(void);
void CreateCalendarMenu(void);
void CreateClockMenu(void);
void CreatePasswordMenu(void);
void CreatePasswordResponseMenu(void);
void CreateSystemMenu(void);

void CreateFeaturesMenu(void);
void CreateSourceCheckMenu(void);
void CreateBackgroundCheckMenu(void);
void CreateIntegrateMenu(void);
void CreateIntegrateReportMenu(void);
void CreateIntegrateValueMenu(void);
void CreateDataLoggingMenu(void);
void CreateDataLoggingReportMenu(void);
void CreateDataLoggingReportSystemMenu(void);
void CreateDataLoggingReportInstrumentMenu(void);
void CreateDataLoggingReportReadingsMenu(void);

void CreateControlsMenu(void);
void CreateBacklightMenu(void);
void CreateAutoBacklightMenu(void);
void CreateAudioMenu(void);
void CreateGPSMenu(void);
void CreateBluetoothMenu(void);

void CreateDetectorMenu(void);
void CreateDetectorSetupMenu(void);
void CreateDisplayMenu(SHORT calling_menu);
void CreateDisplaySetupMenu(void);
void CreateViewMenu(void);
void CreateViewAppearanceMenu(void);
void CreateViewFunctionsMenu(void);
void CreateViewFunctionMenu(void);
void CreateViewAlertsMenu(void);
void CreateViewAlertsReadingMenu(void);
void CreateViewAlarmsMenu(void);
void CreateViewAlarmsReadingMenu(void);

void CreateAlertsAlarmsMenu(void);
void CreateAlertsMenu(void);
void CreateAlarmsMenu(void);

void CreateFunctionsMenu(void);

void CreateBlankMenu(void);

void FormatString(char *generic_string, char *message, UINT8 message_length);
void FormatLine(UINT8 current_line, char *generic_string, UINT8 sub_menu, void *line_item, UINT8 line_item_type);

void CreateLine(void);
void CreateEditLine(void);
void CreateEditStringLine(void);

void ChangeMenuLine(void);
void ChangeEditLine(void);
void ChangeEditStringLine(void);

void CreateMonthList(char generic_value, char *generic_string);
void CreateShortMonthList(char generic_value, char *generic_string);								// (9DP v1.03.00)
void CreateLanguageList(char generic_value, char *generic_string);
void CreatePowerUpModeList(char generic_value, char *generic_string);
void CreateDetectorPowerUpModeList(char generic_value, char *generic_string);
void CreateOperatingModeList(char generic_value, char *generic_string);
void CreateAudioOperatingModeList(char generic_value, char *generic_string);
void CreateColorList(char generic_value, char *generic_string);
void CreateCurrentViewList(char generic_value, char *generic_string);
void CreateMultiplierList(char generic_value, char *generic_string);
void CreateUnitsList(char generic_value, char *generic_string);
void CreateModelList(char generic_value, char *generic_string);
void CreateViewFunctionList(char generic_value, char *generic_string);
void CreateDetectorList(unsigned short generic_value, char *generic_string);
void CreateInstrumentFunctionsList(UINT8 generic_value, char *generic_string);
void CreatePercentageList(char generic_value, char *generic_string);
void CreateCurrentDetectorList(char generic_value, char *generic_string);
void CreateRangeList(char generic_value, char *generic_string);

#endif

//*
//**************************************************************************************************
//**************************************************************************************************
//* LUDLUM MEASUREMENTS INC. COPYRIGHT (C) 2012
//**************************************************************************************************
//**************************************************************************************************
