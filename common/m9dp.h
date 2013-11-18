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
//*     ~~~~~~~~        08/27/13        Austin Mullins
//*     - Changed default offset to 0V, per new electrometer design.
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

//**************************************************************************************************
//**************************************************************************************************
//  Instrument Configuration
//**************************************************************************************************
//**************************************************************************************************
//* 

#define NUMBER_OF_RANGES                                    5
#define NUMBER_OF_VIEWS                                     4
#define NUMBER_OF_FUNCTIONS                                 4
#define NUMBER_OF_INTERNAL_DETECTORS                        1
#define NUMBER_OF_EXTERNAL_DETECTORS                        0
#define NUMBER_OF_DETECTORS                                 (1 + NUMBER_OF_EXTERNAL_DETECTORS)

#define INSTRUMENT_9D
//#define INSTRUMENT_DETECTOR
//#define INSTRUMENT_MULTIPLE_DETECTOR

//*
//**************************************************************************************************
//**************************************************************************************************
//* End Instrument Configuration
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//  Reading Configuration
//**************************************************************************************************
//**************************************************************************************************
//* 

#define DEFAULT_DOSE_MULTIPLIER                             MICRO
#define DEFAULT_RATE_MULTIPLIER                             MICRO

//*
//**************************************************************************************************
//**************************************************************************************************
//* End Reading Configuration
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//* SETTINGS TIER
//**************************************************************************************************
//**************************************************************************************************
//* SETTINGS - PRODUCT

//* space holder                                    "1234567890123456"
#define DEFAULT_MODEL                               "9DP"
#define DEFAULT_MODEL_NUMBER                        MODEL_9DP
#define DEFAULT_SERIAL_NUMBER                       ""
#define DEFAULT_USER_ID_NUMBER                      ""
#define DEFAULT_USER_SERIAL_NUMBER                  ""

//**************************************************************************************************
//* SETTINGS - LANGUAGE

//* space holder                                    "1234567890123456"
#define DEFAULT_LANGUAGE_NAME                       "ENGLISH         "
#define DEFAULT_LANGUAGE_INDEX                      0

//**************************************************************************************************
//* SETTINGS - DATE

//* space holder                                            "1234567890123456"
#define DEFAULT_DATE_STRING                                 "01 JAN 2014     "
#define DEFAULT_DATE_MONTH                                  1
#define DEFAULT_DATE_DAY                                    1
#define DEFAULT_DATE_YEAR                                   14

//**************************************************************************************************
//* SETTINGS - TIME

//* space holder                                            "1234567890123456"
#define DEFAULT_TIME_STRING                                 "12:00           "
#define DEFAULT_TIME_HOURS                                  12
#define DEFAULT_TIME_MINUTES                                0
#define DEFAULT_TIME_SECONDS                                0
#define DEFAULT_TIME_FORMAT                                 12

//**************************************************************************************************
//* SETTINGS - PASSWORDS

//* space holder                                            "1234567890123456"
#define DEFAULT_PASSWORD_ENABLE                             NO
#define DEFAULT_PASSWORD_1                                  ""
#define DEFAULT_PASSWORD_2                                  ""
#define DEFAULT_PASSWORD_3                                  ""
#define DEFAULT_PASSWORD_MASTER                             ""

//**************************************************************************************************
//* SETTINGS - FIRMWARE

//* space holder                                            "1234567890123456"
#define FIRMWARE_NUMBER_VALUE                               "29307.01.03.04\0"

//*
//**************************************************************************************************
//**************************************************************************************************
//* END SETTINGS TIER
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//* FEATURES TIER
//**************************************************************************************************
//**************************************************************************************************
//* FEATURES - SOURCE CHECK

#define DEFAULT_SOURCE_POWERUP_MODE                         POWERUP_OFF                             // disabled
#define DEFAULT_SOURCE_OPERATING_MODE                       OFF                                     // disabled
#define DEFAULT_SOURCE_DELAY                                60                                      // 60 seconds
#define DEFAULT_SOURCE_LOG_RESULT                           OFF                                     // disabled
#define DEFAULT_SOURCE_SAMPLE_TIME                          15                                      // 15 seconds
#define DEFAULT_SOURCE_UNITS                                0                                       // TBD
#define DEFAULT_SOURCE_MULTIPLIER                           0                                       // TBD

//**************************************************************************************************
//* FEATURES - BACKGROUND CHECK

#define DEFAULT_BACKGROUND_POWERUP_MODE                     POWERUP_OFF                             // disabled
#define DEFAULT_BACKGROUND_OPERATING_MODE                   OFF                                     // disabled
#define DEFAULT_BACKGROUND_DELAY                            30                                      // 30 seconds
#define DEFAULT_BACKGROUND_LOG_RESULT                       OFF                                     // disabled
#define DEFAULT_BACKGROUND_SAMPLE_TIME                      15                                      // 15 seconds
#define DEFAULT_BACKGROUND_UNITS                            0                                       // TBD
#define DEFAULT_BACKGROUND_MULTIPLIER                       0                                       // TBD

//**************************************************************************************************
//* FEATURES - INTEGRATE

#define DEFAULT_INTEGRATE_POWERUP_MODE                      POWERUP_ON                              // enabled
#define DEFAULT_INTEGRATE_OPERATING_MODE                    ON                                      // on
#define DEFAULT_INTEGRATE_DELAY                             60                                      // 60 seconds
#define DEFAULT_INTEGRATE_LOG_RESULT                        OFF                                     // disabled
#define DEFAULT_INTEGRATE_LOG_SHUTDOWN                      OFF                                     // disabled
#define DEFAULT_INTEGRATE_SAVE_ON_SHUTDOWN                  OFF                                     // disabled
#define DEFAULT_INTEGRATE_VALUE                             0.0                                     // 0.0 (v1.01.02)
#define DEFAULT_INTEGRATE_UNITS                             R                                       // R (v1.01.02)
#define DEFAULT_INTEGRATE_MULTIPLIER                        MICRO                                   // micro (v1.01.02)

//**************************************************************************************************
//* FEATURES - DATA LOGGING

#define DEFAULT_DATA_LOGGING_POWERUP_MODE                   POWERUP_OFF                             // disabled
#define DEFAULT_DATA_LOGGING_OPERATING_MODE                 OFF                                     // disabled
#define DEFAULT_DATA_LOGGING_DELAY                          60                                      // 60 seconds
#define DEFAULT_DATA_LOGGING_SAMPLE_PERIOD                  60                                      // 60 seconds
#define DEFAULT_DATA_LOGGING_UNITS                          0                                       // TBD
#define DEFAULT_DATA_LOGGING_MULTIPLIER                     0                                       // TBD
#define DEFAULT_DATA_LOGGING_EVENT                          OFF                                     // disabled

//*
//**************************************************************************************************
//**************************************************************************************************
//* END FEATURES TIER
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//* CONTROLS TIER
//**************************************************************************************************
//**************************************************************************************************
//* CONTROLS - BACKLIGHT

//#define DEFAULT_BACKLIGHT_POWERUP_MODE                    POWERUP_LAST                            // 
#define DEFAULT_BACKLIGHT_POWERUP_MODE                      BK_LIGHT_DEFAULT                        // (v1.01.04)
#define DEFAULT_BACKLIGHT_OPERATING_MODE                    BK_LIGHT_AUTO                           // 
#define DEFAULT_BACKLIGHT_AUTO_LOW                          20                                      // percentage level: 0 - 100
#define DEFAULT_BACKLIGHT_AUTO_MEDIUM                       60                                      // percentage level: 0 - 100
#define DEFAULT_BACKLIGHT_AUTO_HIGH                         100                                     // percentage level: 0 - 100
#define DEFAULT_BACKLIGHT_CURRENT_LEVEL                     DEFAULT_BACKLIGHT_AUTO_MEDIUM           // percentage level: 0 - 100
#define DEFAULT_BACKLIGHT_ON_LEVEL                          DEFAULT_BACKLIGHT_AUTO_HIGH             // percentage level: 0 - 100
#define DEFAULT_BACKLIGHT_TEMP_TIME                         10                                      // time in seconds

//**************************************************************************************************
//* CONTROLS - AUDIO

#define DEFAULT_AUDIO_POWERUP_MODE                          POWERUP_ON                              // enabled
#define DEFAULT_AUDIO_OPERATING_MODE                        AUDIO_40                                // 40% audio
#define DEFAULT_AUDIO_CURRENT_LEVEL                         (DEFAULT_AUDIO_OPERATING_MODE * 20)     // 
#define DEFAULT_AUDIO_ALERT_LEVEL                           (AUDIO_80 * 20)                         // 80% audio
#define DEFAULT_AUDIO_ALARM_LEVEL                           (AUDIO_80 * 20)                         // 80% audio

//**************************************************************************************************
//* CONTROLS - GPS

#define DEFAULT_GPS_POWERUP_MODE                            POWERUP_OFF                             // disabled
#define DEFAULT_GPS_OPERATING_MODE                          OFF                                     // disabled

//**************************************************************************************************
//* CONTROLS - BLUETOOTH

#define DEFAULT_BLUETOOTH_POWERUP_MODE                      POWERUP_OFF                             // disabled
#define DEFAULT_BLUETOOTH_OPERATING_MODE                    OFF                                     // disabled

//*
//**************************************************************************************************
//**************************************************************************************************
//* END CONTROLS TIER
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//* DISPLAY TIER
//**************************************************************************************************
//**************************************************************************************************
//* DISPLAY - PROPERTIES

#define DEFAULT_DETECTOR_POWERUP_MODE                       DEFAULT_DET
#define DEFAULT_DETECTOR                                    INTERNAL

//**************************************************************************************************
//* DISPLAY - DETECTOR - VIEW SETUP - VIEW PROPERTIES

#define DEFAULT_VIEWS_POWERUP_MODE                          VIEW_DEFAULT
#define DEFAULT_CURRENT_VIEW                                0

//*
//**************************************************************************************************
//**************************************************************************************************
//* END DISPLAY TIER
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//* DISPLAY - DETECTOR
//**************************************************************************************************
//**************************************************************************************************
//* DISPLAY - DETECTOR - PROPERTIES

#define DEFAULT_DETECTOR_0                                  DETECTOR_9DP
#define DEFAULT_DETECTOR_1                                  NO_DETECTOR
#define DEFAULT_DETECTOR_2                                  NO_DETECTOR
#define DEFAULT_DETECTOR_3                                  NO_DETECTOR
#define DEFAULT_DETECTOR_4                                  NO_DETECTOR

#define DEFAULT_DETECTOR_SERIAL_NUMBER                      ""
#define DEFAULT_DETECTOR_USER_ID_NUMBER                     ""
#define DEFAULT_DETECTOR_USER_SERIAL_NUMBER                 ""
#define DEFAULT_DETECTOR_CALIBRATION_TECHNICIAN             ""

#define DEFAULT_DETECTOR_0_HV                               0
#define DEFAULT_DETECTOR_1_HV                               0
#define DEFAULT_DETECTOR_2_HV                               0
#define DEFAULT_DETECTOR_3_HV                               0
#define DEFAULT_DETECTOR_4_HV                               0

#define DEFAULT_DETECTOR_0_HV_OFFSET                        0
#define DEFAULT_DETECTOR_1_HV_OFFSET                        0
#define DEFAULT_DETECTOR_2_HV_OFFSET                        0
#define DEFAULT_DETECTOR_3_HV_OFFSET                        0
#define DEFAULT_DETECTOR_4_HV_OFFSET                        0

#define DEFAULT_DETECTOR_0_THRESHOLD_1                      0
#define DEFAULT_DETECTOR_1_THRESHOLD_1                      0
#define DEFAULT_DETECTOR_2_THRESHOLD_1                      0
#define DEFAULT_DETECTOR_3_THRESHOLD_1                      0
#define DEFAULT_DETECTOR_4_THRESHOLD_1                      0

#define DEFAULT_DETECTOR_0_THRESHOLD_1_OFFSET               0
#define DEFAULT_DETECTOR_1_THRESHOLD_1_OFFSET               0
#define DEFAULT_DETECTOR_2_THRESHOLD_1_OFFSET               0
#define DEFAULT_DETECTOR_3_THRESHOLD_1_OFFSET               0
#define DEFAULT_DETECTOR_4_THRESHOLD_1_OFFSET               0

#define DEFAULT_UNSIGNED_00                                 0
#define DEFAULT_UNSIGNED_01                                 0
#define DEFAULT_UNSIGNED_02                                 0
#define DEFAULT_UNSIGNED_03                                 0
#define DEFAULT_UNSIGNED_04                                 0
#define DEFAULT_UNSIGNED_05                                 0
#define DEFAULT_UNSIGNED_06                                 0
#define DEFAULT_UNSIGNED_07                                 0
#define DEFAULT_UNSIGNED_08                                 0
#define DEFAULT_UNSIGNED_09                                 0
#define DEFAULT_UNSIGNED_10                                 0
#define DEFAULT_UNSIGNED_11                                 0
#define DEFAULT_UNSIGNED_12                                 0
#define DEFAULT_UNSIGNED_13                                 0
#define DEFAULT_UNSIGNED_14                                 0
#define DEFAULT_UNSIGNED_15                                 0

#define DEFAULT_SIGNED_00                                   0
#define DEFAULT_SIGNED_01                                   0
#define DEFAULT_SIGNED_02                                   0
#define DEFAULT_SIGNED_03                                   0
#define DEFAULT_SIGNED_04                                   0
#define DEFAULT_SIGNED_05                                   0
#define DEFAULT_SIGNED_06                                   0
#define DEFAULT_SIGNED_07                                   0
#define DEFAULT_SIGNED_08                                   0
#define DEFAULT_SIGNED_09                                   0
#define DEFAULT_SIGNED_10                                   0
#define DEFAULT_SIGNED_11                                   0
#define DEFAULT_SIGNED_12                                   0
#define DEFAULT_SIGNED_13                                   0
#define DEFAULT_SIGNED_14                                   0
#define DEFAULT_SIGNED_15                                   0

#define DEFAULT_FLOAT_00                                    0.000
#define DEFAULT_FLOAT_01                                    0.000
#define DEFAULT_FLOAT_02                                    0.000
#define DEFAULT_FLOAT_03                                    0.000
#define DEFAULT_FLOAT_04                                    0.000
#define DEFAULT_FLOAT_05                                    0.000
#define DEFAULT_FLOAT_06                                    0.000
#define DEFAULT_FLOAT_07                                    0.000

//*
//**************************************************************************************************
//**************************************************************************************************
//* END DISPLAY TIER
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//* VIEW TIER (DISPLAY - DETECTOR)
//**************************************************************************************************
//**************************************************************************************************
//* VIEW - APPEARANCE

#define DEFAULT_APPEARANCE_DISPLAY_TIME                     OFF
#define DEFAULT_APPEARANCE_DISPLAY_VIEW_NAME                OFF

//**************************************************************************************************
//* VIEW - ELEMENT HEADER

#define DEFAULT_ELEMENT_STYLE_1                             ARC                                     // (v1.01.04)
#define DEFAULT_ELEMENT_STYLE_2                             ARC                                     // (v1.01.04)
#define DEFAULT_ELEMENT_STYLE_3                             ARC                                     // (v1.01.04)
#define DEFAULT_ELEMENT_STYLE_4                             NO_ELEMENT                              // (v1.01.04)

#define DEFAULT_ELEMENT_NAME_1                              "R/hr"                                  // (v1.01.12)
#define DEFAULT_ELEMENT_NAME_2                              "Sv/h"                                  // (v1.01.12)
#define DEFAULT_ELEMENT_NAME_3                              "Gy/h"                                  // (v1.01.12)
#define DEFAULT_ELEMENT_NAME_4                              ""                                      // (v1.01.01)

//**************************************************************************************************
//* VIEW - ELEMENT STYLE

#define DEFAULT_ELEMENT_1_ARC_TYPE_OF_SCALE                 LINEAR
#define DEFAULT_ELEMENT_1_ARC_NUMBER_OF_DIVISIONS           5
#define DEFAULT_ELEMENT_1_ARC_NUMBER_OF_RANGES              5
#define DEFAULT_ELEMENT_1_ARC_DIGITS                        3
#define DEFAULT_ELEMENT_1_ARC_UNITS                         R_HR
#define DEFAULT_ELEMENT_1_ARC_MULTIPLIER                    MICRO
#define DEFAULT_ELEMENT_1_ARC_END_POINT                     500
#define DEFAULT_ELEMENT_1_ARC_MULTIPLIER_1                  MILLI                                   // 9DP-1 (v1.01.05)
#define DEFAULT_ELEMENT_1_ARC_END_POINT_1                   5                                       // 9DP-1 (v1.01.05)

#define DEFAULT_ELEMENT_2_ARC_TYPE_OF_SCALE                 LINEAR
#define DEFAULT_ELEMENT_2_ARC_NUMBER_OF_DIVISIONS           5
#define DEFAULT_ELEMENT_2_ARC_NUMBER_OF_RANGES              5
#define DEFAULT_ELEMENT_2_ARC_DIGITS                        3
#define DEFAULT_ELEMENT_2_ARC_UNITS                         SV_HR
#define DEFAULT_ELEMENT_2_ARC_MULTIPLIER                    MICRO
#define DEFAULT_ELEMENT_2_ARC_END_POINT                     5
#define DEFAULT_ELEMENT_2_ARC_MULTIPLIER_1                  MICRO                                   // 9DP-1 (v1.01.05)
#define DEFAULT_ELEMENT_2_ARC_END_POINT_1                   50                                      // 9DP-1 (v1.01.05)

#define DEFAULT_ELEMENT_3_ARC_TYPE_OF_SCALE                 LINEAR
#define DEFAULT_ELEMENT_3_ARC_NUMBER_OF_DIVISIONS           5
#define DEFAULT_ELEMENT_3_ARC_NUMBER_OF_RANGES              5
#define DEFAULT_ELEMENT_3_ARC_DIGITS                        3
#define DEFAULT_ELEMENT_3_ARC_UNITS                         GY_HR
#define DEFAULT_ELEMENT_3_ARC_MULTIPLIER                    MICRO
#define DEFAULT_ELEMENT_3_ARC_END_POINT                     5
#define DEFAULT_ELEMENT_3_ARC_MULTIPLIER_1                  MICRO                                   // 9DP-1 (v1.01.05)
#define DEFAULT_ELEMENT_3_ARC_END_POINT_1                   50                                      // 9DP-1 (v1.01.05)

#define DEFAULT_ELEMENT_4_ARC_TYPE_OF_SCALE                 NO_SCALE                                // (v1.01.01)
#define DEFAULT_ELEMENT_4_ARC_NUMBER_OF_DIVISIONS           0                                       // (v1.01.01)
#define DEFAULT_ELEMENT_4_ARC_NUMBER_OF_RANGES              0                                       // (v1.01.01)
#define DEFAULT_ELEMENT_4_ARC_DIGITS                        0                                       // (v1.01.01)
#define DEFAULT_ELEMENT_4_ARC_UNITS                         NO_UNITS                                // (v1.01.01)
#define DEFAULT_ELEMENT_4_ARC_MULTIPLIER                    PICO                                    // PICO is software zero (v1.01.17)
#define DEFAULT_ELEMENT_4_ARC_END_POINT                     0                                       // (v1.01.01)
#define DEFAULT_ELEMENT_4_ARC_MULTIPLIER_1                  PICO                                    // PICO is software zero (9DP-1 v1.01.17)
#define DEFAULT_ELEMENT_4_ARC_END_POINT_1                   0                                       // (9DP-1 v1.01.05)

//**************************************************************************************************
//* VIEW - FUNCTIONS (VIEW DEPENDENT v1.01.04)

#define DEFAULT_VIEW_1_FUNCTION_1_ENABLE                    YES                                     // (v1.01.04)
#define DEFAULT_VIEW_1_FUNCTION_1_NUMBER                    0                                       // (v1.01.04)
#define DEFAULT_VIEW_1_FUNCTION_2_ENABLE                    YES                                     // (v1.01.04)
#define DEFAULT_VIEW_1_FUNCTION_2_NUMBER                    1                                       // (v1.01.04)
#define DEFAULT_VIEW_1_FUNCTION_3_ENABLE                    NO                                      // (v1.01.04)
#define DEFAULT_VIEW_1_FUNCTION_3_NUMBER                    255                                     // (v1.01.04)
#define DEFAULT_VIEW_1_FUNCTION_4_ENABLE                    NO                                      // (v1.01.04)
#define DEFAULT_VIEW_1_FUNCTION_4_NUMBER                    255                                     // (v1.01.04)

#define DEFAULT_VIEW_2_FUNCTION_1_ENABLE                    YES                                     // (v1.01.04)
#define DEFAULT_VIEW_2_FUNCTION_1_NUMBER                    0                                       // (v1.01.04)
#define DEFAULT_VIEW_2_FUNCTION_2_ENABLE                    YES                                     // (v1.01.04)
#define DEFAULT_VIEW_2_FUNCTION_2_NUMBER                    1                                       // (v1.01.04)
#define DEFAULT_VIEW_2_FUNCTION_3_ENABLE                    NO                                      // (v1.01.04)
#define DEFAULT_VIEW_2_FUNCTION_3_NUMBER                    255                                     // (v1.01.04)
#define DEFAULT_VIEW_2_FUNCTION_4_ENABLE                    NO                                      // (v1.01.04)
#define DEFAULT_VIEW_2_FUNCTION_4_NUMBER                    255                                     // (v1.01.04)

#define DEFAULT_VIEW_3_FUNCTION_1_ENABLE                    YES                                     // (v1.01.04)
#define DEFAULT_VIEW_3_FUNCTION_1_NUMBER                    0                                       // (v1.01.04)
#define DEFAULT_VIEW_3_FUNCTION_2_ENABLE                    YES                                     // (v1.01.04)
#define DEFAULT_VIEW_3_FUNCTION_2_NUMBER                    1                                       // (v1.01.04)
#define DEFAULT_VIEW_3_FUNCTION_3_ENABLE                    NO                                      // (v1.01.04)
#define DEFAULT_VIEW_3_FUNCTION_3_NUMBER                    255                                     // (v1.01.04)
#define DEFAULT_VIEW_3_FUNCTION_4_ENABLE                    NO                                      // (v1.01.04)
#define DEFAULT_VIEW_3_FUNCTION_4_NUMBER                    255                                     // (v1.01.04)

#define DEFAULT_VIEW_4_FUNCTION_1_ENABLE                    NO                                      // (v1.01.04)
#define DEFAULT_VIEW_4_FUNCTION_1_NUMBER                    255                                     // (v1.01.04)
#define DEFAULT_VIEW_4_FUNCTION_2_ENABLE                    NO                                      // (v1.01.04)
#define DEFAULT_VIEW_4_FUNCTION_2_NUMBER                    255                                     // (v1.01.04)
#define DEFAULT_VIEW_4_FUNCTION_3_ENABLE                    NO                                      // (v1.01.04)
#define DEFAULT_VIEW_4_FUNCTION_3_NUMBER                    255                                     // (v1.01.04)
#define DEFAULT_VIEW_4_FUNCTION_4_ENABLE                    NO                                      // (v1.01.04)
#define DEFAULT_VIEW_4_FUNCTION_4_NUMBER                    255                                     // (v1.01.04)

//**************************************************************************************************
//* VIEW - ALERTS RADIATION

#define DEFAULT_VIEW_1_RAD_ALERTS_UNITS                     R_HR
#define DEFAULT_VIEW_2_RAD_ALERTS_UNITS                     SV_HR
#define DEFAULT_VIEW_3_RAD_ALERTS_UNITS                     GY_HR
#define DEFAULT_VIEW_4_RAD_ALERTS_UNITS                     NO_UNITS                                // (v1.01.01)

#define DEFAULT_VIEW_RAD_ALERTS_ENABLE                      OFF
#define DEFAULT_VIEW_RAD_ALERTS_CUSTOM                      OFF
#define DEFAULT_VIEW_RAD_ALERTS_VALUE                       0
#define DEFAULT_VIEW_RAD_ALERTS_MULTIPLIER                  MICRO
#define DEFAULT_VIEW_RAD_ALERTS_RELATIVE_READING            0.0
#define DEFAULT_VIEW_RAD_ALERTS_MESSAGE                     ""
#define DEFAULT_VIEW_RAD_ALERTS_CUSTOM_MESSAGE              ""

//**************************************************************************************************
//* VIEW - ALERTS INTEGRATE

#define DEFAULT_VIEW_1_INT_ALERTS_UNITS                     R
#define DEFAULT_VIEW_2_INT_ALERTS_UNITS                     SV
#define DEFAULT_VIEW_3_INT_ALERTS_UNITS                     GY
#define DEFAULT_VIEW_4_INT_ALERTS_UNITS                     NO_UNITS                                // (v1.01.01)

#define DEFAULT_VIEW_INT_ALERTS_ENABLE                      OFF
#define DEFAULT_VIEW_INT_ALERTS_CUSTOM                      OFF
#define DEFAULT_VIEW_INT_ALERTS_VALUE                       0
#define DEFAULT_VIEW_INT_ALERTS_MULTIPLIER                  MICRO
#define DEFAULT_VIEW_INT_ALERTS_RELATIVE_READING            0.0
#define DEFAULT_VIEW_INT_ALERTS_MESSAGE                     ""
#define DEFAULT_VIEW_INT_ALERTS_CUSTOM_MESSAGE              ""

//**************************************************************************************************
//* VIEW - ALERTS FUNCTION

#define DEFAULT_VIEW_FUNCTION_SELECT                        0

#define DEFAULT_VIEW_1_FUN_ALERTS_UNITS                     NO_UNITS
#define DEFAULT_VIEW_2_FUN_ALERTS_UNITS                     NO_UNITS
#define DEFAULT_VIEW_3_FUN_ALERTS_UNITS                     NO_UNITS
#define DEFAULT_VIEW_4_FUN_ALERTS_UNITS                     NO_UNITS

#define DEFAULT_VIEW_FUN_ALERTS_ENABLE                      OFF
#define DEFAULT_VIEW_FUN_ALERTS_CUSTOM                      OFF
#define DEFAULT_VIEW_FUN_ALERTS_VALUE                       0
#define DEFAULT_VIEW_FUN_ALERTS_MULTIPLIER                  NO_MULTIPLIER
#define DEFAULT_VIEW_FUN_ALERTS_RELATIVE_READING            0.0
#define DEFAULT_VIEW_FUN_ALERTS_MESSAGE                     ""
#define DEFAULT_VIEW_FUN_ALERTS_CUSTOM_MESSAGE              ""

//**************************************************************************************************
//* VIEW - ALARMS RADIATION

#define DEFAULT_VIEW_1_RAD_ALARMS_UNITS                     DEFAULT_VIEW_1_RAD_ALERTS_UNITS
#define DEFAULT_VIEW_2_RAD_ALARMS_UNITS                     DEFAULT_VIEW_2_RAD_ALERTS_UNITS
#define DEFAULT_VIEW_3_RAD_ALARMS_UNITS                     DEFAULT_VIEW_3_RAD_ALERTS_UNITS
#define DEFAULT_VIEW_4_RAD_ALARMS_UNITS                     DEFAULT_VIEW_4_RAD_ALERTS_UNITS

#define DEFAULT_VIEW_RAD_ALARMS_ENABLE                      OFF
#define DEFAULT_VIEW_RAD_ALARMS_CUSTOM                      OFF
#define DEFAULT_VIEW_RAD_ALARMS_VALUE                       0
#define DEFAULT_VIEW_RAD_ALARMS_MULTIPLIER                  MICRO
#define DEFAULT_VIEW_RAD_ALARMS_RELATIVE_READING            0.0
#define DEFAULT_VIEW_RAD_ALARMS_MESSAGE                     ""
#define DEFAULT_VIEW_RAD_ALARMS_CUSTOM_MESSAGE              ""

//**************************************************************************************************
//* VIEW - ALARMS INTEGRATE

#define DEFAULT_VIEW_1_INT_ALARMS_UNITS                     DEFAULT_VIEW_1_INT_ALERTS_UNITS
#define DEFAULT_VIEW_2_INT_ALARMS_UNITS                     DEFAULT_VIEW_2_INT_ALERTS_UNITS
#define DEFAULT_VIEW_3_INT_ALARMS_UNITS                     DEFAULT_VIEW_3_INT_ALERTS_UNITS
#define DEFAULT_VIEW_4_INT_ALARMS_UNITS                     DEFAULT_VIEW_4_INT_ALERTS_UNITS

#define DEFAULT_VIEW_INT_ALARMS_ENABLE                      OFF
#define DEFAULT_VIEW_INT_ALARMS_CUSTOM                      OFF
#define DEFAULT_VIEW_INT_ALARMS_VALUE                       0
#define DEFAULT_VIEW_INT_ALARMS_MULTIPLIER                  MICRO
#define DEFAULT_VIEW_INT_ALARMS_RELATIVE_READING            0.0
#define DEFAULT_VIEW_INT_ALARMS_MESSAGE                     ""
#define DEFAULT_VIEW_INT_ALARMS_CUSTOM_MESSAGE              ""

//**************************************************************************************************
//* VIEW - ALARMS FUNCTION

#define DEFAULT_VIEW_1_FUN_ALARMS_UNITS                     DEFAULT_VIEW_1_FUN_ALERTS_UNITS
#define DEFAULT_VIEW_2_FUN_ALARMS_UNITS                     DEFAULT_VIEW_2_FUN_ALERTS_UNITS
#define DEFAULT_VIEW_3_FUN_ALARMS_UNITS                     DEFAULT_VIEW_3_FUN_ALERTS_UNITS
#define DEFAULT_VIEW_4_FUN_ALARMS_UNITS                     DEFAULT_VIEW_4_FUN_ALERTS_UNITS

#define DEFAULT_VIEW_FUN_ALARMS_ENABLE                      OFF
#define DEFAULT_VIEW_FUN_ALARMS_CUSTOM                      OFF
#define DEFAULT_VIEW_FUN_ALARMS_VALUE                       0
#define DEFAULT_VIEW_FUN_ALARMS_MULTIPLIER                  NO_MULTIPLIER
#define DEFAULT_VIEW_FUN_ALARMS_RELATIVE_READING            0.0
#define DEFAULT_VIEW_FUN_ALARMS_MESSAGE                     ""
#define DEFAULT_VIEW_FUN_ALARMS_CUSTOM_MESSAGE              ""

//*
//**************************************************************************************************
//**************************************************************************************************
//* END VIEW TIER
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//* FUNCTIONS TIER
//**************************************************************************************************
//**************************************************************************************************
//* FUNCTIONS - FUNCTION

#define DEFAULT_FUNCTION_1_TYPE                             INTEGRATE
#define DEFAULT_FUNCTION_1_NAME                             "Integrate"                             // (v1.01.01)
#define DEFAULT_FUNCTION_2_TYPE                             PEAK_RATE
#define DEFAULT_FUNCTION_2_NAME                             "Peak Rate"                             // (v1.01.01)
#define DEFAULT_FUNCTION_3_TYPE                             NO_FUNCTION
#define DEFAULT_FUNCTION_3_NAME                             "No Function"                           // (v1.01.01)
#define DEFAULT_FUNCTION_4_TYPE                             NO_FUNCTION
#define DEFAULT_FUNCTION_4_NAME                             "No Function"                           // (v1.01.01)

//**************************************************************************************************
//* FUNCTIONS - FUNCTION DEFINITIONS

#define DEFAULT_INTEGRATE_DETECTOR                          INTERNAL
//#define DEFAULT_INTEGRATE_UNITS                           R                                       // defined elsewhere
//#define DEFAULT_INTEGRATE_MULTIPLIER                      MICRO                                   // defined elsewhere
#define DEFAULT_INTEGRATE_DIGITS                            4
#define DEFAULT_INTEGRATE_RANGE_LOCK                        0
#define DEFAULT_INTEGRATE_TIME                              0

#define DEFAULT_PEAKRATE_DIGITS                             4
#define DEFAULT_PEAKRATE_RANGELOCK                          OFF

//*
//**************************************************************************************************
//**************************************************************************************************
//* END FUNCTIONS TIER
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//* ALERTS TIER
//**************************************************************************************************
//**************************************************************************************************
//* ALERTS - LOW_BATTERY

#define DEFAULT_ALERT_LOW_BATTERY_ENABLE                    OFF
#define DEFAULT_ALERT_LOW_BATTERY_VALUE                     8.5
#define DEFAULT_ALERT_LOW_BATTERY_CUSTOM_ENABLE             OFF
#define DEFAULT_ALERT_LOW_BATTERY_CUSTOM_MESSAGE            ""

//**************************************************************************************************
//* ALERTS - LOW_MEMORY

#define DEFAULT_ALERT_LOW_MEMORY_ENABLE                     OFF
#define DEFAULT_ALERT_LOW_MEMORY_VALUE                      0
#define DEFAULT_ALERT_LOW_MEMORY_CUSTOM_ENABLE              OFF
#define DEFAULT_ALERT_LOW_MEMORY_CUSTOM_MESSAGE             ""

//**************************************************************************************************
//* ALERTS - CALIBRATION_DUE

#define DEFAULT_ALERT_CALIBRATION_DUE_ENABLE                OFF
#define DEFAULT_ALERT_CALIBRATION_DUE_VALUE                 0
#define DEFAULT_ALERT_CALIBRATION_DUE_CUSTOM_ENABLE         OFF
#define DEFAULT_ALERT_CALIBRATION_DUE_CUSTOM_MESSAGE        ""

//**************************************************************************************************
//* ALERTS - SOURCE_CHECK

#define DEFAULT_ALERT_SOURCE_CHECK_ENABLE                   OFF
#define DEFAULT_ALERT_SOURCE_CHECK_VALUE                    0
#define DEFAULT_ALERT_SOURCE_CHECK_MULTIPLIER               DEFAULT_ELEMENT_1_ARC_MULTIPLIER
#define DEFAULT_ALERT_SOURCE_CHECK_UNITS                    DEFAULT_ELEMENT_1_ARC_UNITS
#define DEFAULT_ALERT_SOURCE_CHECK_CUSTOM_ENABLE            OFF
#define DEFAULT_ALERT_SOURCE_CHECK_CUSTOM_MESSAGE           ""

//**************************************************************************************************
//* ALERTS - BACKGROUND_CHECK

#define DEFAULT_ALERT_BACKGROUND_CHECK_ENABLE               OFF
#define DEFAULT_ALERT_BACKGROUND_CHECK_VALUE                0
#define DEFAULT_ALERT_BACKGROUND_CHECK_MULTIPLIER           DEFAULT_ELEMENT_1_ARC_MULTIPLIER
#define DEFAULT_ALERT_BACKGROUND_CHECK_UNITS                DEFAULT_ELEMENT_1_ARC_UNITS
#define DEFAULT_ALERT_BACKGROUND_CHECK_CUSTOM_ENABLE        OFF
#define DEFAULT_ALERT_BACKGROUND_CHECK_CUSTOM_MESSAGE       ""

//*
//**************************************************************************************************
//**************************************************************************************************
//* END ALERTS TIER
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//* ALARMS TIER
//**************************************************************************************************
//**************************************************************************************************
//* ALARMS - LOW_BATTERY

#define DEFAULT_ALARM_LOW_BATTERY_ENABLE                    OFF
#define DEFAULT_ALARM_LOW_BATTERY_VALUE                     8.0
#define DEFAULT_ALARM_LOW_BATTERY_CUSTOM_ENABLE             OFF
#define DEFAULT_ALARM_LOW_BATTERY_CUSTOM_MESSAGE            ""

//**************************************************************************************************
//* ALARMS - LOW_MEMORY

#define DEFAULT_ALARM_LOW_MEMORY_ENABLE                     OFF
#define DEFAULT_ALARM_LOW_MEMORY_VALUE                      0
#define DEFAULT_ALARM_LOW_MEMORY_CUSTOM_ENABLE              OFF
#define DEFAULT_ALARM_LOW_MEMORY_CUSTOM_MESSAGE             ""

//**************************************************************************************************
//* ALARMS - CALIBRATION_DUE

#define DEFAULT_ALARM_CALIBRATION_DUE_ENABLE                OFF
#define DEFAULT_ALARM_CALIBRATION_DUE_VALUE                 0
#define DEFAULT_ALARM_CALIBRATION_DUE_CUSTOM_ENABLE         OFF
#define DEFAULT_ALARM_CALIBRATION_DUE_CUSTOM_MESSAGE        ""

//**************************************************************************************************
//* ALARMS - SOURCE_CHECK

#define DEFAULT_ALARM_SOURCE_CHECK_ENABLE                   OFF
#define DEFAULT_ALARM_SOURCE_CHECK_VALUE                    0
#define DEFAULT_ALARM_SOURCE_CHECK_MULTIPLIER               DEFAULT_ELEMENT_1_ARC_MULTIPLIER
#define DEFAULT_ALARM_SOURCE_CHECK_UNITS                    DEFAULT_ELEMENT_1_ARC_UNITS
#define DEFAULT_ALARM_SOURCE_CHECK_CUSTOM_ENABLE            OFF
#define DEFAULT_ALARM_SOURCE_CHECK_CUSTOM_MESSAGE           ""

//**************************************************************************************************
//* ALARMS - BACKGROUND_CHECK

#define DEFAULT_ALARM_BACKGROUND_CHECK_ENABLE               OFF
#define DEFAULT_ALARM_BACKGROUND_CHECK_VALUE                0
#define DEFAULT_ALARM_BACKGROUND_CHECK_MULTIPLIER           DEFAULT_ELEMENT_1_ARC_MULTIPLIER
#define DEFAULT_ALARM_BACKGROUND_CHECK_UNITS                DEFAULT_ELEMENT_1_ARC_UNITS
#define DEFAULT_ALARM_BACKGROUND_CHECK_CUSTOM_ENABLE        OFF
#define DEFAULT_ALARM_BACKGROUND_CHECK_CUSTOM_MESSAGE       ""

//**************************************************************************************************
//* ALARMS - HV_LOW

#define DEFAULT_ALARM_HV_LOW_ENABLE                         OFF
#define DEFAULT_ALARM_HV_LOW_VALUE                          0
#define DEFAULT_ALARM_HV_LOW_CUSTOM_ENABLE                  OFF
#define DEFAULT_ALARM_HV_LOW_CUSTOM_MESSAGE                 ""

//**************************************************************************************************
//* ALARMS - HV_HIGH

#define DEFAULT_ALARM_HV_HIGH_ENABLE                        OFF
#define DEFAULT_ALARM_HV_HIGH_VALUE                         0
#define DEFAULT_ALARM_HV_HIGH_CUSTOM_ENABLE                 OFF
#define DEFAULT_ALARM_HV_HIGH_CUSTOM_MESSAGE                ""

//**************************************************************************************************
//* ALARMS - LOW_READING

#define DEFAULT_ALARM_LOW_READING_ENABLE                    OFF
#define DEFAULT_ALARM_LOW_READING_VALUE                     0
#define DEFAULT_ALARM_LOW_READING_TIME                      180 * TICKS_PER_SEC
#define DEFAULT_ALARM_LOW_READING_MULTIPLIER                DEFAULT_ELEMENT_1_ARC_MULTIPLIER
#define DEFAULT_ALARM_LOW_READING_UNITS                     DEFAULT_ELEMENT_1_ARC_UNITS
#define DEFAULT_ALARM_LOW_READING_CUSTOM_ENABLE             OFF
#define DEFAULT_ALARM_LOW_READING_CUSTOM_MESSAGE            ""

//*
//**************************************************************************************************
//**************************************************************************************************
//* END ALARMS TIER
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//* CALIBRATION TIER
//**************************************************************************************************
//**************************************************************************************************
//* CALIBRATION - HEADER

#define DEFAULT_CALIBRATION_INSTRUMENT_TYPE                 DEFAULT_MODEL_NUMBER
//* space holder                                            "1234567890123456"
#define DEFAULT_CALIBRATION_TECHNICIAN                      ""

//**************************************************************************************************
//* CALIBRATION - CHECKOUT

//* space holder                                            "1234567890123456"
#define DEFAULT_CHECKOUT_TECHNICIAN                         ""
#define DEFAULT_CHECKOUT_ADC_OFFSET                         0

//**************************************************************************************************
//* CALIBRATION - CONSTANTS

#define DEFAULT_RANGE_CAL_CONSTANT                          1000
#define DEFAULT_HIGH_VOLTAGE_CAL_CONSTANT                   1000
#define DEFAULT_BATTERY_CAL_CONSTANT                        1000

//**************************************************************************************************
//* CALIBRATION - OFFSETS

#define DEFAULT_ADC_OFFSET                                  0
#define DEFAULT_DAC_OFFSET                                  0                                       // (v1.01.15)

//**************************************************************************************************
//* CALIBRATION - HIGH VOLTAGE BOARD

#define DEFAULT_METER_OFFSET                                1575                                    // default
#define DEFAULT_ELECTROMETER_OFFSET                         0                                   // default
#define DEFAULT_ELECTROMETER_TEMPERATURE                    25.0                                    // default
#define DEFAULT_TEMPERATURE_OFFSET_CORRECTION               0.0                                     // default

#define DEFAULT_DAC_1_OFFSET                                0                                       // default
#define DEFAULT_DAC_1_OUTPUT_1                              DEFAULT_METER_OFFSET                    // meter offset
#define DEFAULT_DAC_1_OUTPUT_2                              DEFAULT_ELECTROMETER_OFFSET             // electrometer offset
#define DEFAULT_DAC_1_OUTPUT_3                              0                                       // unused
#define DEFAULT_DAC_1_OUTPUT_4                              0                                       // unused

#define DEFAULT_DAC_2_OFFSET                                0                                       // default
#define DEFAULT_DAC_2_OUTPUT_1                              0                                       // unused
#define DEFAULT_DAC_2_OUTPUT_2                              0                                       // unused
#define DEFAULT_DAC_2_OUTPUT_3                              0                                       // unused
#define DEFAULT_DAC_2_OUTPUT_4                              0                                       // unused

//**************************************************************************************************
//* CALIBRATION - CONTROLS

#define DEFAULT_CALIBRATION_CONTROLS_RANGE                  0
#define DEFAULT_CALIBRATION_CONTROLS_STREAM                 0
#define DEFAULT_CALIBRATION_CONTROLS_STREAM_TIME            16
#define DEFAULT_CALIBRATION_CONTROLS_HVR                    0
#define DEFAULT_CALIBRATION_CONTROLS_BATTERY                0
#define DEFAULT_CALIBRATION_CONTROLS_TEMPERATURE            0
#define DEFAULT_CALIBRATION_CONTROLS_METER_OFFSET           0
#define DEFAULT_CALIBRATION_CONTROLS_LOG                    0

//**************************************************************************************************
//* CALIBRATION - CONFIG

//#define DEFAULT_CALIBRATION_CONFIG_uR_FILTER              50.0
#define DEFAULT_CALIBRATION_CONFIG_uR_FILTER                30.0                                    // (v1.02.04)

//*
//**************************************************************************************************
//**************************************************************************************************
//* END CALIBRATION TIER
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//* EEPROM DEFINITIONS
//**************************************************************************************************
//**************************************************************************************************
//* CALIBRATION

#define EEPROM_CALIBRATION_LOCATION                         (0x0000)
#define EEPROM_CALIBRATION_HEADER_LOCATION                  (EEPROM_CALIBRATION_LOCATION)
#define EEPROM_CALIBRATION_CHECKOUT_LOCATION                (EEPROM_CALIBRATION_HEADER_LOCATION + TOTAL_CALIBRATION_HEADER)
#define EEPROM_CALIBRATION_CONSTANTS_LOCATION               (EEPROM_CALIBRATION_CHECKOUT_LOCATION + TOTAL_CALIBRATION_CHECKOUT)
#define EEPROM_CALIBRATION_OFFSETS_LOCATION                 (EEPROM_CALIBRATION_CONSTANTS_LOCATION + TOTAL_CALIBRATION_CONSTANTS)
#define EEPROM_CALIBRATION_HVB_LOCATION                     (EEPROM_CALIBRATION_OFFSETS_LOCATION + TOTAL_CALIBRATION_OFFSETS)
#define EEPROM_CALIBRATION_CONTROLS_LOCATION                (EEPROM_CALIBRATION_HVB_LOCATION + TOTAL_CALIBRATION_HVB)
#define EEPROM_CALIBRATION_CONFIG_LOCATION                  (EEPROM_CALIBRATION_CONTROLS_LOCATION + TOTAL_CALIBRATION_CONTROLS)

//**************************************************************************************************
//* SETTINGS

#define EEPROM_SETTINGS_LOCATION                            (0x1000)
#define EEPROM_PRODUCT_LOCATION                             (EEPROM_SETTINGS_LOCATION)
#define EEPROM_LANGUAGE_LOCATION                            (EEPROM_PRODUCT_LOCATION + TOTAL_PRODUCT)
#define EEPROM_CALENDAR_LOCATION                            (EEPROM_LANGUAGE_LOCATION + TOTAL_LANGUAGE)
#define EEPROM_CLOCK_LOCATION                               (EEPROM_CALENDAR_LOCATION + TOTAL_DATE)
#define EEPROM_PASSWORDS_LOCATION                           (EEPROM_CLOCK_LOCATION + TOTAL_TIME)
#define EEPROM_SETTINGS_MISC_LOCATION                       (EEPROM_PASSWORDS_LOCATION + TOTAL_PASSWORDS)
#define TOTAL_SETTINGS                                      (TOTAL_PRODUCT + TOTAL_LANGUAGE + TOTAL_DATE + TOTAL_TIME + TOTAL_PASSWORDS + TOTAL_SETTINGS_MISC)

//**************************************************************************************************
//* FEATURES

#define EEPROM_FEATURES_LOCATION                            (EEPROM_SETTINGS_LOCATION + TOTAL_SETTINGS)
#define EEPROM_SOURCE_CHECK_LOCATION                        (EEPROM_FEATURES_LOCATION)
#define EEPROM_BACKGROUND_CHECK_LOCATION                    (EEPROM_SOURCE_CHECK_LOCATION + TOTAL_SOURCE_CHECK)
#define EEPROM_INTEGRATE_LOCATION                           (EEPROM_BACKGROUND_CHECK_LOCATION + TOTAL_BACKGROUND_CHECK)
#define EEPROM_DATA_LOGGING_LOCATION                        (EEPROM_INTEGRATE_LOCATION + TOTAL_INTEGRATE)
#define TOTAL_FEATURES                                      (TOTAL_SOURCE_CHECK + TOTAL_BACKGROUND_CHECK + TOTAL_INTEGRATE + TOTAL_DATA_LOGGING)

//**************************************************************************************************
//* CONTROLS

#define EEPROM_CONTROLS_LOCATION                            (EEPROM_FEATURES_LOCATION + TOTAL_FEATURES)
#define EEPROM_BACKLIGHT_LOCATION                           (EEPROM_CONTROLS_LOCATION)
#define EEPROM_AUDIO_LOCATION                               (EEPROM_BACKLIGHT_LOCATION + TOTAL_BACKLIGHT)
#define EEPROM_GPS_LOCATION                                 (EEPROM_AUDIO_LOCATION + TOTAL_AUDIO)
#define EEPROM_BLUETOOTH_LOCATION                           (EEPROM_GPS_LOCATION + TOTAL_GPS)
#define TOTAL_CONTROLS                                      (TOTAL_BACKLIGHT + TOTAL_AUDIO + TOTAL_GPS + TOTAL_BLUETOOTH)

//**************************************************************************************************
//* DISPLAY

#define EEPROM_DISPLAY_LOCATION                             (EEPROM_CONTROLS_LOCATION + TOTAL_CONTROLS)

//#define EEPROM_DISPLAY_PROPERTIES_LOCATION                (EEPROM_DISPLAY_LOCATION)
#ifdef EEPROM_DISPLAY_PROPERTIES_LOCATION
    #define TOTAL_DISPLAY_PROPERTIES_EEPROM                 (TOTAL_DISPLAY_PROPERTIES)
#else
    #define TOTAL_DISPLAY_PROPERTIES_EEPROM                 0
#endif

//**************************************************************************************************
//* DISPLAY - DETECTOR

#define EEPROM_DETECTOR_LOCATION                            (EEPROM_DISPLAY_LOCATION + TOTAL_DISPLAY_PROPERTIES_EEPROM)

//#define EEPROM_DETECTOR_PROPERTIES_LOCATION               (EEPROM_DETECTOR_LOCATION)
#ifdef EEPROM_DETECTOR_PROPERTIES_LOCATION
    #define TOTAL_DETECTOR_PROPERTIES_EEPROM                (TOTAL_DETECTOR_PROPERTIES)
#else
    #define TOTAL_DETECTOR_PROPERTIES_EEPROM                0
#endif

//#define EEPROM_DETECTOR_CALIBRATION_LOCATION              (EEPROM_DETECTOR_LOCATION + TOTAL_DETECTOR_PROPERTIES_EEPROM)
#ifdef EEPROM_DETECTOR_CALIBRATION_LOCATION
    #define TOTAL_DETECTOR_CALIBRATION_EEPROM               (TOTAL_DETECTOR_CALIBRATION)
#else
    #define TOTAL_DETECTOR_CALIBRATION_EEPROM               0
#endif

#define EEPROM_DETECTOR_VIEW_SETUP_LOCATION                 (EEPROM_DETECTOR_LOCATION + TOTAL_DETECTOR_PROPERTIES_EEPROM + TOTAL_DETECTOR_CALIBRATION_EEPROM)
#ifdef EEPROM_DETECTOR_VIEW_SETUP_LOCATION
    #define TOTAL_DETECTOR_VIEW_SETUP_EEPROM                (TOTAL_DETECTOR_VIEW_SETUP)
#else
    #define TOTAL_DETECTOR_VIEW_SETUP_EEPROM                0
#endif

//**************************************************************************************************
//* DISPLAY - VIEW

#define EEPROM_VIEW_LOCATION                                (EEPROM_DETECTOR_LOCATION + TOTAL_DETECTOR_PROPERTIES_EEPROM + TOTAL_DETECTOR_CALIBRATION_EEPROM + TOTAL_DETECTOR_VIEW_SETUP_EEPROM)
#define EEPROM_APPEARANCE_LOCATION                          (EEPROM_VIEW_LOCATION)
#define EEPROM_ELEMENT_HEADER_LOCATION                      (EEPROM_APPEARANCE_LOCATION + TOTAL_APPEARANCE)
#define EEPROM_ELEMENT_LOCATION                             (EEPROM_ELEMENT_HEADER_LOCATION + TOTAL_ELEMENT_HEADER)
#define EEPROM_VIEW_FUNCTIONS_LOCATION                      (EEPROM_ELEMENT_LOCATION + TOTAL_ELEMENT)
#define EEPROM_ALERTS_RADIATION_LOCATION                    (EEPROM_VIEW_FUNCTIONS_LOCATION + TOTAL_VIEW_FUNCTIONS)
#define EEPROM_ALERTS_INTEGRATE_LOCATION                    (EEPROM_ALERTS_RADIATION_LOCATION + TOTAL_ALERTS_RADIATION)
#define EEPROM_ALERTS_FUNCTION_LOCATION                     (EEPROM_ALERTS_INTEGRATE_LOCATION + TOTAL_ALERTS_INTEGRATE)
#define EEPROM_ALARMS_RADIATION_LOCATION                    (EEPROM_ALERTS_FUNCTION_LOCATION + TOTAL_ALERTS_FUNCTION)
#define EEPROM_ALARMS_INTEGRATE_LOCATION                    (EEPROM_ALARMS_RADIATION_LOCATION + TOTAL_ALARMS_RADIATION)
#define EEPROM_ALARMS_FUNCTION_LOCATION                     (EEPROM_ALARMS_INTEGRATE_LOCATION + TOTAL_ALARMS_INTEGRATE)
//#define EEPROM_VIEW_CALIBRATION_LOCATION                  (EEPROM_ALARMS_FUNCTION_LOCATION + TOTAL_ALARMS_FUNCTION)
#ifdef EEPROM_VIEW_CALIBRATION_LOCATION
    #define TOTAL_VIEW                                      (TOTAL_APPEARANCE + TOTAL_ELEMENT_HEADER + TOTAL_ELEMENT + TOTAL_VIEW_FUNCTIONS + (6 * TOTAL_ALERTS_RADIATION) + TOTAL_VIEW_CALIBRATION)
#else
    #define TOTAL_VIEW                                      (TOTAL_APPEARANCE + TOTAL_ELEMENT_HEADER + TOTAL_ELEMENT + TOTAL_VIEW_FUNCTIONS + (6 * TOTAL_ALERTS_RADIATION))
#endif
#define TOTAL_VIEW_ARRAY                                    (NUMBER_OF_VIEWS * TOTAL_VIEW)

#define TOTAL_DETECTOR                                      (TOTAL_DETECTOR_PROPERTIES_EEPROM + TOTAL_DETECTOR_CALIBRATION_EEPROM + TOTAL_DETECTOR_VIEW_SETUP_EEPROM + TOTAL_VIEW_ARRAY)
#define TOTAL_DETECTOR_ARRAY                                (NUMBER_OF_DETECTORS * TOTAL_DETECTOR)

//**************************************************************************************************
//* FUNCTIONS

#define EEPROM_FUNCTIONS_LOCATION                           (EEPROM_DETECTOR_LOCATION + TOTAL_DETECTOR_ARRAY)
#define TOTAL_FUNCTIONS                                     (NUMBER_OF_FUNCTIONS * TOTAL_FUNCTION)

//**************************************************************************************************
//* ALERTS

#define EEPROM_ALERTS_LOCATION                              (EEPROM_FUNCTIONS_LOCATION + TOTAL_FUNCTIONS)
#define EEPROM_ALERTS_LOW_BATTERY_LOCATION                  (EEPROM_ALERTS_LOCATION)
#define EEPROM_ALERTS_LOW_MEMORY_LOCATION                   (EEPROM_ALERTS_LOW_BATTERY_LOCATION + TOTAL_ALERTS_LOW_BATTERY)
#define EEPROM_ALERTS_CALIBRATION_DUE_LOCATION              (EEPROM_ALERTS_LOW_MEMORY_LOCATION + TOTAL_ALERTS_LOW_MEMORY)
#define EEPROM_ALERTS_SOURCE_CHECK_LOCATION                 (EEPROM_ALERTS_CALIBRATION_DUE_LOCATION + TOTAL_ALERTS_CALIBRATION_DUE)
#define EEPROM_ALERTS_BACKGROUND_CHECK_LOCATION             (EEPROM_ALERTS_SOURCE_CHECK_LOCATION + TOTAL_ALERTS_SOURCE_CHECK)
#define NUMBER_OF_ALERTS                                    5
#define TOTAL_ALERTS                                        (NUMBER_OF_ALERTS * ALERTS_SIZE)

//**************************************************************************************************
//* ALARMS

#define EEPROM_ALARMS_LOCATION                              (EEPROM_ALERTS_LOCATION + TOTAL_ALERTS)
#define EEPROM_ALARMS_LOW_BATTERY_LOCATION                  (EEPROM_ALARMS_LOCATION)
#define EEPROM_ALARMS_LOW_MEMORY_LOCATION                   (EEPROM_ALARMS_LOW_BATTERY_LOCATION + TOTAL_ALARMS_LOW_BATTERY)
#define EEPROM_ALARMS_CALIBRATION_DUE_LOCATION              (EEPROM_ALARMS_LOW_MEMORY_LOCATION + TOTAL_ALARMS_LOW_BATTERY)
#define EEPROM_ALARMS_SOURCE_CHECK_LOCATION                 (EEPROM_ALARMS_CALIBRATION_DUE_LOCATION + TOTAL_ALARMS_CALIBRATION_DUE)
#define EEPROM_ALARMS_BACKGROUND_CHECK_LOCATION             (EEPROM_ALARMS_SOURCE_CHECK_LOCATION + TOTAL_ALARMS_SOURCE_CHECK)
#define EEPROM_ALARMS_HV_LOW_LOCATION                       (EEPROM_ALARMS_BACKGROUND_CHECK_LOCATION + TOTAL_ALARMS_BACKGROUND_CHECK)
#define EEPROM_ALARMS_HV_HIGH_LOCATION                      (EEPROM_ALARMS_HV_LOW_LOCATION + TOTAL_ALARMS_HV_LOW)
#define EEPROM_ALARMS_LOW_READING_LOCATION                  (EEPROM_ALARMS_HV_HIGH_LOCATION + TOTAL_ALARMS_HV_HIGH)
#define NUMBER_OF_ALARMS                                    8
#define TOTAL_ALARMS                                        (NUMBER_OF_ALARMS * ALARMS_SIZE)

//*
//**************************************************************************************************
//**************************************************************************************************
//* END EEPROM DEFINITIONS
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//* INSTRUMENT - DATA STRUCTURE
//**************************************************************************************************
//**************************************************************************************************
//*

typedef struct _INSTRUMENT
{
    TYPE_SETTINGS settings;
    TYPE_FEATURES features;
    TYPE_CONTROLS controls;
    struct
    {
        union
        {
            TYPE_DISPLAY_PROPERTIES info;
            unsigned char buffer[SIZE_DISPLAY_PROPERTIES];
        } properties;
        struct
        {
            union
            {
                TYPE_DETECTOR_PROPERTIES info;
                unsigned char buffer[SIZE_DETECTOR_PROPERTIES];
            } properties;
            union
            {
                TYPE_DETECTOR_CALIBRATION info;
                unsigned char buffer[SIZE_DETECTOR_CALIBRATION];
            } calibration;
            union
            {
                TYPE_DETECTOR_VIEW_SETUP info;
                unsigned char buffer[SIZE_DETECTOR_VIEW_SETUP];
            } view_setup;
            TYPE_VIEW view[NUMBER_OF_VIEWS];
        } detector[NUMBER_OF_DETECTORS];
    } display;
    TYPE_FUNCTION function[NUMBER_OF_FUNCTIONS];
    TYPE_ALERTS alerts;
    TYPE_ALARMS alarms;
    TYPE_CALIBRATION calibration;
} TYPE_INSTRUMENT;

//*
//**************************************************************************************************
//**************************************************************************************************
//* END INSTRUMENT DATA STRUCTURE
//**************************************************************************************************
//**************************************************************************************************

#endif

//*
//**************************************************************************************************
//**************************************************************************************************
//* LUDLUM MEASUREMENTS INC. COPYRIGHT (C) 2012
//**************************************************************************************************
//**************************************************************************************************
