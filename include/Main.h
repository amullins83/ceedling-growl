//**************************************************************************************************
//**************************************************************************************************
//*                                                                     
//* Filename:       main.h
//* Date:           02/26/11                                            
//*                                                                     
//* Author:         James Comstock
//* Company:        Ludlum Measurements                                 
//* Address:        501 Oak Street                                      
//*                 Sweetwater, TX 79556                                
//*                                                                     
//**************************************************************************************************
//**************************************************************************************************
//*                                                                     
//* Revision Notes:                                                     
//*
//*           11/13/13        Austin Mullins
//*     - bumped version to 1.4.0
//*
//* branch ceedling
//*    n1     11/05/13        Austin Mullins
//*     - Added references to reading_calculation and instrument_constants
//*
//* branch update_range
//*    n1     09/16/13        Austin Mullins
//*     - Changed electrometer default back to 1500 to test on existing devices.
//*
//* n3        09/13/13        Austin Mullins
//*     - Rearranged Main.h: NO_DETECTOR is the default detector and R_HR are the default units.
//*
//* n1        09/10/13        Austin Mullins
//*     - Changed default offset to 0V, per new electrometer design.
//*     - Bumped version number to 01.03.05.
//*
//* ~~~~~~~~    08/29/11    James Comstock
//* - redesign
//*                                                                     
//**************************************************************************************************
//*                                                                     
//* Microcontroller:    PIC32MX460F512L-80I/PT
//* Oscillator:         8Mhz, external
//* Clock:              60MHz, internal (nS instruction cycle)
//* Code Memory:        512K bytes (256K word instructions), flash
//* RAM:                32K bytes
//* Internal EEPROM:    none
//* External EEPROM:    64K bytes, U7
//*                     64K bytes, U8
//* External Flash:     1M words (16Mbit x16)
//*
//**************************************************************************************************
//*
//* Disclosure:
//*
//* Some of the routines contained within this firmware may have been developed by Microchip and are
//* provided to developers for use in Microchip devices. The Microchip software license agreement is
//* listed below.
//*
//**************************************************************************************************
//*
//* Software License Agreement
//*
//* The software supplied herewith by Microchip Technology Incorporated (the "Company") for its
//* PICmicro® Microcontroller is intended and supplied to you, the Company’s customer, for use
//* solely and exclusively on Microchip PICmicro Microcontroller products.
//*
//* The software is owned by the Company and/or its supplier, and is protected under applicable
//* copyright laws. All rights are reserved. Any use in violation of the foregoing restrictions may
//* subject the user to criminal sanctions under applicable laws, as well as to civil liability for
//* the breach of the terms and conditions of this license.
//*
//* THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR
//* STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//* A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT, IN ANY CIRCUMSTANCES, BE
//* LIABLE FOR SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
//*
//**************************************************************************************************

#ifndef _MAIN_INCLUDE_
#define _MAIN_INCLUDE_

#ifndef __PIC32MX__
#define __PIC32MX__
#endif

#ifndef __LANGUAGE_C__
#define __LANGUAGE_C__
#endif

#ifndef __MIPSEL__
#define __MIPSEL__
#define MIPSEL
#endif

#include <plib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Definitions.h"
#include "System.h"
#include "usb_config.h"
#include "usb.h"
#include "usb_host_hid_parser.h"
#include "usb_host_hid.h"
#include "usb_host_local.h"
#include "Graphics.h"
#include "SSD1906.h"
#include "p32mx460f512l.h"
#include "LCD-SST39VF1601v001.h"
#include "I2C.h"
#include "i2cConfig.h"
#include "UART.h"
#include "Message_Box.h"
#include "Elements.h"
#include "Functions.h"
#include "Conversions.h"
#include "GenericTypeDefs.h"
#include "Screens.h"
#include "Icons.h"
#include "ftdi.h"
#include "Commands.h"
#include <time.h>
#include "usb_host_generic.h"
#include "Keyboard_Menu.h"

#include "instrument_constants.h"
#include "reading_calculation.h"
#include "type_instrument.h"
#include "time_constants.h"
#include "model_constants.h"


//**************************************************************************************************
//**************************************************************************************************
//  Global Variables
//**************************************************************************************************
//**************************************************************************************************
//*

OBJ_HEADER    *pArcScreen;                                                                          // pointer to ARC Screen
OBJ_HEADER    *pNavList;                                                                            // pointer to navigation list
OBJ_HEADER    *pSettingsScrn;                                                                       // pointer to the settings screen
OBJ_HEADER    *pControlsScrn;                                                                       // pointer to the controls screen
OBJ_HEADER    *pDetectorScrn;                                                                       // pointer to the detector screen
OBJ_HEADER    *pAlertScrn;                                                                          // pointer to the alert screen
OBJ_HEADER    *pAlarmScrn;                                                                          // pointer to the alarm screen
OBJ_HEADER    *pFunctionScrn;                                                                       // pointer to the function screen
OBJ_HEADER    *pColorsScrn;                                                                         // pointer to the color screen
OBJ_HEADER    *PreviousScrn;                                                                        // pointer to the previous screen pointer

//*
//**************************************************************************************************
//**************************************************************************************************
//* End Global Variables
//**************************************************************************************************
//**************************************************************************************************

#include "M9DP_Config.h"

#endif

//*
//**************************************************************************************************
//**************************************************************************************************
//* LUDLUM MEASUREMENTS INC. COPYRIGHT (C) 2012
//**************************************************************************************************
//**************************************************************************************************
