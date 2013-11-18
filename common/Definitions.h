//**************************************************************************************************
//**************************************************************************************************
//*																		
//*	Filename:		Definitions.h
//*	Date:			08/26/11
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
//*     n3              11/13/13        Austin Mullins
//*     - Added Time Constants function type
//*
//*     n2              09/13/13        Austin Mullins
//*     - Uncommented the size diagnostic line regarding SIZE_ELEMENT to make sure EEPROM pages are
//*       valid. This showed that the MEMORY_ELEMENT size needed to be reduced by 2 bytes in order
//*       to fit TYPE_ELEMENT on a single 128-byte EEPROM page. It is unclear why this page-size
//*       conflict never occurs on 9DP, only on 3D.
//*
//*	~~~~~~~~	08/26/11	James Comstock
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

#ifndef _DEFINITIONS_INCLUDE_
#define _DEFINITIONS_INCLUDE_

#include "GenericTypeDefs.h"
#include "Detector.h"
#include "models.h"
//#include "instrument.h"

//**************************************************************************************************
//**************************************************************************************************
//	Definitions
//**************************************************************************************************
//**************************************************************************************************
//*

//**************************************************************************************************
//*	Instrument Configuration

//#define TICKS_PER_SEC								8												// 
//#define TICKS_PER_SEC								16												// changed to 62.5mS (9DP v1.01.06)
#define TICKS_PER_SEC								8												// changed back to 125mS (9DP v1.01.08)

//**************************************************************************************************
//*	Message Responses

#define	YES											1
#define	NO											0

//**************************************************************************************************
//*	Event States

//#define ON											1
//#define OFF											0

//**************************************************************************************************
//*	Window Configuration

#define X_SPACING									2												// 2 pixels
#define Y_SPACING									2												// 2 pixels
#define HEADER_HEIGHT 								20												// 20 pixels
#define ELEMENT_HEIGHT								140												// 140 pixels
#define DIVIDER_HEIGHT								5												// 5 pixels
#define FUNCTION_HEIGHT								31												// 31 pixels
#define ICON_HEIGHT									40												// 40 pixels
#define FUNCTION_Y_OFFSET							3												// 3 pixels

#define HEADER_Y									(Y_SPACING)
#define ELEMENT_Y									(Y_SPACING + HEADER_HEIGHT)
#define DIVIDER_Y									(Y_SPACING + HEADER_HEIGHT + ELEMENT_HEIGHT)
#define FUNCTION_Y									(Y_SPACING + HEADER_HEIGHT + ELEMENT_HEIGHT + DIVIDER_HEIGHT)
#define ICON_Y										(Y_SPACING + HEADER_HEIGHT + ELEMENT_HEIGHT + DIVIDER_HEIGHT + FUNCTION_HEIGHT)

//**************************************************************************************************
//*	Keypad Masking

#define	BTN_FUNCTION								0x01											// & with returned value 0x7E to see bit is 0
#define	BTN_AUDIO									0x02											// & with returned value 0x7D to see bit is 0
#define	BTN_ACK_RESET								0x04											// & with returned value 0x7B to see bit is 0
#define	BTN_HANDLE_1								0x08											// & with returned value 0x77 to see bit is 0
#define	BTN_HANDLE_2								0x10											// & with returned value 0x6F to see bit is 0
#define	BTN_HANDLE_3								0x20											// & with returned value 0x5F to see bit is 0

//**************************************************************************************************
//*	Alarm Conditions

#define	CLEAR										0
#define	NORMAL										0
#define	ALERT										1
#define	ALARM										2 
	
//**************************************************************************************************
//* Audio Defaults

#define AUDIO_DISABLED								0
#define TWENTY										1
#define FOURTY										2
#define SIXTY										3
#define EIGHTY										4
#define HUNDRED										5
#define PREVIOUS									6

//**************************************************************************************************
//* Number of ADC Samples to Gather for a Specific Reading

#define ADC_BATTERY_SAMPLES							2												// adc battery level readings to average together
#define ADC_LIGHT_SAMPLES							2												// adc ambient light level readings to average together
#define ADC_PIC_SAMPLES								4												// adc pressurized ion chamber readings to average 

//**************************************************************************************************
//* Generic Hardware Definitions

#define MAXDACVALUE									4095											// DAC maximum value for a 12-bit number

//**************************************************************************************************
//*	EEPROM Page Size

#define EEPROM_PAGE_SIZE							128

//**************************************************************************************************
//*	Text Messages

#define DEFAULT_BLANK_STRING						""

//**************************************************************************************************
//*	Audio Control Definitions (moved here from main program file 9DP v1.01.13)

#define RESET_AUDIO									0x00											// reset audio to defaults (9DP v1.01.13)
#define ENABLE_AUDIO								0x01
#define DISABLE_AUDIO								0xFE
#define ENABLE_ALARM								0x02
#define DISABLE_ALARM								0xFD
#define AUDIO_MODE_0								0x00
#define AUDIO_MODE_1								0x08
#define AUDIO_MODE_2								0x04
#define AUDIO_MODE_3								0x0C
#define AUDIO_MODE_CLEAR							0xF3
#define AUDIO_DIV_1									0x00
#define AUDIO_DIV_10								0x10
#define AUDIO_DIV_100								0x20
#define AUDIO_DIV_1K								0x30
#define AUDIO_DIV_CLEAR								0xCF
#define MUTE_AUDIO									0x7F											// to mute audio is to clear the bit (9DP v1.01.13)
#define UNMUTE_AUDIO								0x80											// to unmute the audio is to set the bit (9DP v1.01.13)

//**************************************************************************************************
//*	HV Control (9DP v1.01.13)

#define DISABLE_HV								0x40
#define ENABLE_HV								0xBF

//**************************************************************************************************
//**************************************************************************************************
//*	Enumerations
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//* Screen States

typedef enum _SCREEN_STATES
{
    NOT_DISPLAYED = 0,
	CREATE_SPLASH,
	DISPLAY_SPLASH,
	CREATE_LUDLUM,
	DISPLAY_LUDLUM,
	CREATE_STARTUP,
	DISPLAY_STARTUP,
	CREATE_ELEMENT,
	MONITOR_RADIATION,
	CREATE_LOCAL_ALERTS,
	DISPLAY_LOCAL_ALERTS,
	DISPLAY_EVENT,
	WAIT_STATE,
	CREATE_MENU,
	DISPLAY_MENU,
	CREATE_PASSWORD_ENTRY,
	DISPLAY_PASSWORD_ENTRY,
	CREATE_PASSWORD_RESPONSE,
	DISPLAY_PASSWORD_RESPONSE,
	CREATE_SETTINGS,
	DISPLAY_SETTINGS,
	CREATE_PRODUCT,
	DISPLAY_PRODUCT,
	CREATE_LANGUAGE,
	DISPLAY_LANGUAGE,
	CREATE_CALENDAR,
	DISPLAY_CALENDAR,
	CREATE_CLOCK,
	DISPLAY_CLOCK,
	CREATE_PASSWORD,
	DISPLAY_PASSWORD,
	CREATE_SYSTEM,
	DISPLAY_SYSTEM,
	CREATE_FEATURES,
	DISPLAY_FEATURES,
	CREATE_SOURCE_CHECK,
	DISPLAY_SOURCE_CHECK,
	CREATE_BACKGROUND_CHECK,
	DISPLAY_BACKGROUND_CHECK,
	CREATE_INTEGRATE,
	DISPLAY_INTEGRATE,
	CREATE_INTEGRATE_REPORT,
	DISPLAY_INTEGRATE_REPORT,
	CREATE_INTEGRATE_VALUE,
	DISPLAY_INTEGRATE_VALUE,
	CREATE_DATA_LOGGING,
	DISPLAY_DATA_LOGGING,
	CREATE_DATA_LOGGING_REPORT,
	DISPLAY_DATA_LOGGING_REPORT,
	CREATE_DATA_LOGGING_REPORT_SYSTEM,
	DISPLAY_DATA_LOGGING_REPORT_SYSTEM,
	CREATE_DATA_LOGGING_REPORT_INSTRUMENT,
	DISPLAY_DATA_LOGGING_REPORT_INSTRUMENT,
	CREATE_DATA_LOGGING_REPORT_READINGS,
	DISPLAY_DATA_LOGGING_REPORT_READINGS,
	CREATE_CONTROLS,
	DISPLAY_CONTROLS,
	CREATE_BACKLIGHT,
	DISPLAY_BACKLIGHT,
	CREATE_AUTO_BACKLIGHT,
	DISPLAY_AUTO_BACKLIGHT,
	CREATE_AUDIO,
	DISPLAY_AUDIO,
	CREATE_GPS,
	DISPLAY_GPS,
	CREATE_BLUETOOTH,
	DISPLAY_BLUETOOTH,
	CREATE_DETECTOR,
	DISPLAY_DETECTOR,
	CREATE_DETECTOR_SETUP,
	DISPLAY_DETECTOR_SETUP,
	CREATE_DETECTOR_0,
	CREATE_DETECTOR_1,
	CREATE_DETECTOR_2,
	CREATE_DETECTOR_3,
	CREATE_DETECTOR_4,
	CREATE_DISPLAY,
	DISPLAY_DISPLAY,
	CREATE_DISPLAY_SETUP,
	DISPLAY_DISPLAY_SETUP,
	CREATE_VIEW_1,
	CREATE_VIEW_2,
	CREATE_VIEW_3,
	CREATE_VIEW_4,
	DISPLAY_VIEW,
	CREATE_VIEW_APPEARANCE,
	DISPLAY_VIEW_APPEARANCE,
	CREATE_VIEW_FUNCTIONS,
	DISPLAY_VIEW_FUNCTIONS,
	CREATE_VIEW_FUNCTION_1,
	CREATE_VIEW_FUNCTION_2,
	CREATE_VIEW_FUNCTION_3,
	CREATE_VIEW_FUNCTION_4,
	DISPLAY_VIEW_FUNCTION,
	CREATE_VIEW_ALERTS,
	DISPLAY_VIEW_ALERTS,
	CREATE_VIEW_ALERTS_RAD,
	CREATE_VIEW_ALERTS_INT,
	CREATE_VIEW_ALERTS_FUN,
	DISPLAY_VIEW_ALERTS_READING,
	CREATE_VIEW_ALARMS,
	DISPLAY_VIEW_ALARMS,
	CREATE_VIEW_ALARMS_RAD,
	CREATE_VIEW_ALARMS_INT,
	CREATE_VIEW_ALARMS_FUN,
	DISPLAY_VIEW_ALARMS_READING,
	CREATE_ALERTS_ALARMS,
	DISPLAY_ALERTS_ALARMS,
	CREATE_ALERTS,
	DISPLAY_ALERTS,
	CREATE_ALARMS,
	DISPLAY_ALARMS,
	CREATE_FUNCTIONS,
	DISPLAY_FUNCTIONS,
	CREATE_CALIBRATION,
	DISPLAY_CALIBRATION,
	CREATE_LINE,
	DISPLAY_LINE,
	CREATE_EDIT_LINE,
	DISPLAY_EDIT_LINE,
	CREATE_EDIT_STRING_LINE,
	DISPLAY_EDIT_STRING_LINE,
	CREATE_BLANK_MENU,
	DISPLAY_BLANK_MENU,
	CREATE_INSTRUMENT_MENU,																			// (Test v0.00.05)
	DISPLAY_INSTRUMENT_MENU,																		// (Test v0.00.05)
	CREATE_TEST_MENU,																				// (Test v0.00.01)
	DISPLAY_TEST_MENU,																				// (Test v0.00.01)
	CREATE_9D_TEST_MENU,																			// (Test v0.00.05)
	DISPLAY_9D_TEST_MENU,																			// (Test v0.00.05)
	CREATE_3D_TEST_MENU,																			// (Test v0.00.05)
	DISPLAY_3D_TEST_MENU,																			// (Test v0.00.05)
	CREATE_276D_TEST_MENU,																			// (Test v0.00.05)
	DISPLAY_276D_TEST_MENU,																			// (Test v0.00.05)
	CREATE_TEST_MAIN_BOARD,																			// (Test v0.00.01)
	DISPLAY_TEST_MAIN_BOARD,																		// (Test v0.00.01)
	CREATE_TEST_KERNEL,																				// (Test v0.00.01)
	DISPLAY_TEST_KERNEL,																			// (Test v0.00.01)
	CREATE_TEST_AUDIO,																				// (Test v0.00.01)
	DISPLAY_TEST_AUDIO,																				// (Test v0.00.01)
	CREATE_TEST_RTCC,																				// (Test v0.00.01)
	DISPLAY_TEST_RTCC,																				// (Test v0.00.01)
	CREATE_TEST_EXT_MEM,																			// (Test v0.00.01)
	DISPLAY_TEST_EXT_MEM,																			// (Test v0.00.01)
	CREATE_TEST_EXT_MEM_RW,																			// (Test v0.00.01)
	DISPLAY_TEST_EXT_MEM_RW,																		// (Test v0.00.01)
	CREATE_TEST_EXT_MEM_ADDR,																		// (Test v0.00.01)
	DISPLAY_TEST_EXT_MEM_ADDR,																		// (Test v0.00.01)
	CREATE_TEST_EXT_MEM_DATA,																		// (Test v0.00.01)
	DISPLAY_TEST_EXT_MEM_DATA,																		// (Test v0.00.01)
	CREATE_TEST_EEPROM,																				// (Test v0.00.01)
	DISPLAY_TEST_EEPROM,																			// (Test v0.00.01)
	CREATE_TEST_EEPROM_PRIMARY_RW,																	// (Test v0.00.01)
	DISPLAY_TEST_EEPROM_PRIMARY_RW,																	// (Test v0.00.01)
	CREATE_TEST_EEPROM_PRIMARY_ERASE,																// (Test v0.00.01)
	DISPLAY_TEST_EEPROM_PRIMARY_ERASE,																// (Test v0.00.01)
	CREATE_TEST_EEPROM_SECONDARY_RW,																// (Test v0.00.01)
	DISPLAY_TEST_EEPROM_SECONDARY_RW,																// (Test v0.00.01)
	CREATE_TEST_EEPROM_SECONDARY_ERASE,																// (Test v0.00.01)
	DISPLAY_TEST_EEPROM_SECONDARY_ERASE,															// (Test v0.00.01)
	CREATE_TEST_HV_BOARD,																			// (Test v0.00.01)
	DISPLAY_TEST_HV_BOARD,																			// (Test v0.00.01)
	CREATE_TEST_LCD_BOARD,																			// (Test v0.00.01)
	DISPLAY_TEST_LCD_BOARD,																			// (Test v0.00.01)
	CREATE_TEST_LCD_COLOR_LINES,																	// (Test v0.00.03)
	DISPLAY_TEST_LCD_COLOR_LINES,																	// (Test v0.00.03)
	CREATE_TEST_LCD_COLORS,																			// (Test v0.00.01)
	DISPLAY_TEST_LCD_COLORS,																		// (Test v0.00.01)
	CREATE_TEST_LIGHT_SENSOR,																		// (Test v0.00.01)
	DISPLAY_TEST_LIGHT_SENSOR,																		// (Test v0.00.01)
	CREATE_TEST_HIGH_VOLTAGE_1,																		// (Test v0.00.01)
	DISPLAY_TEST_HIGH_VOLTAGE_1,																	// (Test v0.00.01)
	CREATE_TEST_HIGH_VOLTAGE_2,																		// (Test v0.00.01)
	DISPLAY_TEST_HIGH_VOLTAGE_2,																	// (Test v0.00.01)
	CREATE_TEST_INPUT_VOLTAGE_1,																	// (Test v0.00.01)
	DISPLAY_TEST_INPUT_VOLTAGE_1,																	// (Test v0.00.01)
	CREATE_TEST_INPUT_VOLTAGE_2,																	// (Test v0.00.01)
	DISPLAY_TEST_INPUT_VOLTAGE_2,																	// (Test v0.00.01)
	CREATE_TEST_METER_OFFSET,																		// (Test v0.00.06)
	DISPLAY_TEST_METER_OFFSET,																		// (Test v0.00.06)
	CREATE_TEST_ELECTROMETER_OFFSET,																// (Test v0.00.06)
	DISPLAY_TEST_ELECTROMETER_OFFSET,																// (Test v0.00.06)
	CREATE_TEST_BATTERY_VOLTAGE,																	// (Test v0.00.01)
	DISPLAY_TEST_BATTERY_VOLTAGE,																	// (Test v0.00.01)
	CREATE_TEST_DISPLAY_VOLTAGE,																	// (Test v0.00.04)
	DISPLAY_TEST_DISPLAY_VOLTAGE,																	// (Test v0.00.04)
	CREATE_TEST_KEYPAD,																				// (Test v0.00.01)
	DISPLAY_TEST_KEYPAD,																			// (Test v0.00.01)
	CREATE_TEST_ELECTROMETER_BOARD,																	// (Test v0.00.01)
	DISPLAY_TEST_ELECTROMETER_BOARD,																// (Test v0.00.01)
	CREATE_TEST_ELECTROMETER_TEMPERATURE,															// (Test v0.00.01)
	DISPLAY_TEST_ELECTROMETER_TEMPERATURE,															// (Test v0.00.01)
	CREATE_SHUTDOWN																					// (Test v0.00.01)
} SCREEN_STATES;

//**************************************************************************************************
//* Format Types

typedef enum _FORMAT_TYPES
{
	TYPE_BOOL = 1,
	TYPE_UINT8,
	TYPE_UINT16,
	TYPE_UINT32,
	TYPE_INT8,
	TYPE_INT16,
	TYPE_INT32,
	TYPE_FLOAT,
	TYPE_STRING,
	TYPE_ENABLE,
	TYPE_DATE_MONTH,
	TYPE_DATE_DAY,
	TYPE_DATE_YEAR,
	TYPE_TIME_HOURS,
	TYPE_TIME_MINUTES,
	TYPE_TIME_SECONDS,
	TYPE_TIME_FORM,
	TYPE_POWERUP_MODE,
	TYPE_PMODE_DETECTOR,
	TYPE_PMODE_BACKLIGHT,
	TYPE_PMODE_AUDIO,
	TYPE_PMODE_GPS,
	TYPE_PMODE_BLUETOOTH,
	TYPE_PMODE_LOGGING,
	TYPE_PMODE_DISPLAY,
	TYPE_OMODE_BACKLIGHT,
	TYPE_OMODE_AUDIO,
	TYPE_OMODE_GPS,
	TYPE_OMODE_BLUETOOTH,
	TYPE_OMODE_LOGGING,
	TYPE_CURRENT_VIEW,
	TYPE_COLOR,
	TYPE_1_HOUR,
	TYPE_3_MINUTES,
	TYPE_100_PERCENT,
	TYPE_VALUE,
	TYPE_MULTIPLIER,
	TYPE_UNITS,
	TYPE_MODEL_NUMBER,
	TYPE_LANGUAGE_SELECT,
	TYPE_HIDDEN_STRING,
	TYPE_VIEW_FUNCTION,
	TYPE_VIEW_APPEARANCE,
	TYPE_INSTRUMENT_FUNCTION,
	TYPE_DETECTOR,
	TYPE_INTEGRATE_MULTIPLIER,
	TYPE_INTEGRATE_UNITS,
	TYPE_TEST_COLOR,
	TYPE_OFFSET,																					// (Test v0.00.06)
	TYPE_RANGE																						// (Test v0.00.06)
} FORMAT_TYPES;


//**************************************************************************************************
//* USB Devices

typedef enum _USB_DEVICE
{
	NO_DEVICE_ATTACHED = 0,																			// no device is attached to the USB port
	FTDI,																							// FTDI device is attached
	KEYBOARD,																						// keyboard is attached
	THUMBDRIVE																						// thumbdrive is attached
} USB_DEVICE;

//**************************************************************************************************
//* Application States

typedef enum _APP_STATE
{
    DEVICE_NOT_CONNECTED,
    DEVICE_CONNECTED, 																				// Device Enumerated  - Report Descriptor Parsed
    READY_TO_TX_RX_REPORT,
    GET_INPUT_REPORT, 																				// perform operation on received report
    INPUT_REPORT_PENDING,
    SEND_OUTPUT_REPORT, 																			// Not needed in case of mouse
    OUTPUT_REPORT_PENDING,
	DEVICE_SUSPENDED,																				// NEW ----- ADDED 3/15
    ERROR_REPORTED 
} APP_STATE;

//**************************************************************************************************
//* Computer States

typedef enum _COMPUTER_STATE
{
    PC_NOT_CONNECTED = 0,																			// FTDI device is not plugged in
    PC_NEW_CONNECT,																					// FTDI device is plugged in for the first time
	PC_CONNECTED,																					// FTDI device is plugged in and communication is successful
	PC_NO_CHANGE,																					// no change in the active computer state since last time
	PC_DISCONNECTED																					// FTDI device is plugged in but communication has been disabled
} COMPUTER_STATE;

//**************************************************************************************************
//* File States

typedef enum _FILE_STATE
{
	FILE_NOT_OPEN = 0,
	FILE_OPEN_DIRECTORY,
	FILE_VIEW_DIRECTORY,
	FILE_OPEN_READ_CFG,
	FILE_OPEN_READ_DRW,
	FILE_OPEN_READ_C,
	FILE_READ_BYTE,
	FILE_READ_PENDING,
	FILE_OPEN_WRITE,
	FILE_OPEN_WRITE_CFG,
	FILE_OPEN_WRITE_TXT,
//	FILE_WRITE_PENDING,
	FILE_CLOSE,
	FILE_ERROR,
	FILE_ERROR_CLOSE
} FILE_STATE;

//**************************************************************************************************
//* Unit Type

typedef enum _UNIT_TYPE
{
	NO_UNITS = 0,
	R_HR,																							// roentgen per hour
	SV_HR,																							// sievert per hour
	REM_HR,																							// REM per hour
	GY_HR,																							// gray per hour
	CPS,																							// counts per second
	CPM,																							// counts per minute
	DPS,																							// disintegrations per second
	DPM,																							// disintegrations per minute
	BQ,																								// becquerel
	R,																								// roentgen
	SV,																								// sievert
	REM,																							// REM
	GY,																								// gray
	COUNTS,																							// counts
	DISINTEGRATIONS,																				// disintegrations
	END_UNIT_TYPE																					// last unit location
} UNIT_TYPE;
	
//**************************************************************************************************
//* Unit Multiplier

typedef enum _UNIT_MULTIPLIER
{
	NO_MULTIPLIER = 0,																				// undefined
	ZEPTO,																							// E-21 (not used by software)
	ATTO,																							// E-18 (not used by software)
	FEMTO,																							// E-15 (not used by software)
	PICO,																							// E-12 (not used by software)
	NANO,																							// E-09
	MICRO,																							// E-06
	MILLI,																							// E-03
	UNITY,																							// E+00
	KILO,																							// E+03
	MEGA,																							// E+06
	GIGA,																							// E+09
	TERA,																							// E+12 (not used by software)
	PETA,																							// E+15 (not used by software)
	EXA,																							// E+18 (not used by software)
	ZETTA,																							// E+21 (not used by software)
	END_MULTIPLIER_TYPE																				// last multiplier location
} UNIT_MULTIPLIER;

#define MULTIPLIER_ADJUST							(NANO - ZEPTO)

//**************************************************************************************************
//* Powerup Mode

typedef enum _MODE_POWERUP
{
	POWERUP_OFF = 0,																				// powerup in the off state
	POWERUP_ON,																						// powerup in the on state
	POWERUP_DEFAULT,																				// powerup in the default state
	POWERUP_LAST,																					// powerup in the last known state
	END_MODE_POWERUP
} MODE_POWERUP;

//**************************************************************************************************
//* Languages

typedef enum _LANGUAGES
{
	ENGLISH = 0,
	SPANISH,
	FRENCH,
	GERMAN,
	JAPANESE
} LANGUAGES;

//**************************************************************************************************
//*	Element Type

typedef enum _ELEMENT_STYLE
{
	NO_ELEMENT = 0,
	ARC,
	RULER
} ELEMENT_STYLE;

//**************************************************************************************************
//*	Scale Type

typedef enum _SCALE_STYLE
{
	NO_SCALE = 0,
	LINEAR,
	LOG
} SCALE_STYLE;

//**************************************************************************************************
//* Function Type

typedef enum FUNCTION_TYPE
{
	NO_FUNCTION = 0,																				// 0
	INTEGRATE,																						// 1
	PEAK_RATE,																						// 2
	SCALER,																							// 3
	PEAK_HOLD,																						// 4
	RATE_METER,																						// 5
	SOURCE_CHECK,																					// 6
	METER_RESET,																					// 7
	BACKGROUND_READING,																				// 8
	BACKGROUND_SUBTRACTION,																			// 9
	NET_READING,																					// 10
	PERCENT_ALARM,																					// 11
	SIGMA_ALARM,																					// 12
	HUNDRED_CM2,																					// 13
	PCI_GM,																							// 14
	DATA_LOGGING,																					// 15
	RECORD_EVENT,																					// 16
	AUDIO_DIVIDE,																					// 17
	BARCODE,																						// 18
	GPS,																							// 19
	TIMED_INTEGRATE,																				// 20
	PULSED_MODE,																					// 21
	RANGE_STEP,																						// 22
	CORRECTION_FACTOR,																				// 23 - correction factor (9DP v1.02.02)
	CHAMBER_PRESSURE,																				// 24 - chamber pressure (9DP v1.02.03)
	FUNCTION_25,																					// 25
	FUNCTION_26,																					// 26
	FUNCTION_27,																					// 27
	FUNCTION_28,																					// 28
	FUNCTION_29,																					// 29
	FUNCTION_30,																					// 30
	FUNCTION_31,																					// 31
	FUNCTION_32,																					// 32
	FUNCTION_33,																					// 33
	FUNCTION_34,																					// 34
	FUNCTION_35,																					// 35
	FUNCTION_36,																					// 36
	FUNCTION_37,																					// 37
	FUNCTION_38,																					// 38
	FUNCTION_39,																					// 39
	FUNCTION_40,																					// 40
	FUNCTION_41,																					// 41
	FUNCTION_42,																					// 42
	FUNCTION_43,																					// 43
	FUNCTION_44,																					// 44
	USER_DEFINED_1,																					// 45
	USER_DEFINED_2,																					// 46
	USER_DEFINED_3,																					// 47
	USER_DEFINED_4,																					// 48
	USER_DEFINED_5,																					// 49
	NO_FUNCTION_X,																					// 50 - keyboard changeable
	INPUT_VOLTAGE,																					// 51 - input voltage
	PEAK_VOLTAGE,																					// 52 - peak voltage
	CLOCK_TIME,																						// 53 - time
	CALENDAR_DATE,																					// 54 - date
	CHAMBER_TEMP,																					// 55 - chamber temperature
	INSTRUMENT_TEMP,																				// 56 - instrument temperature
	HIGH_VOLTAGE,																					// 57 - high voltage
	TEMPERATURE_OFFSET,																				// 58 - temperature offset routine
	FUNCTION_59,																					// 59
	SET_RANGE,																						// 60 - force particular range
	TIME_CONSTANTS = 70,																					// 70 - time constants
	DEBUG_FUNCTION = 99,																			// 99 - debug function
	LAST_FUNCTION
} FUNCTION_TYPE;

//**************************************************************************************************
//* Detector Assignment

typedef enum _DETECTOR_ASSIGNMENT
{
	INTERNAL = 0,
	DET_1,
	DET_2,
	DET_3,
	DET_4,
	DEFAULT_DET,
	LAST_DET,
	END_MODE_DETECTOR
} DETECTOR_ASSIGNMENT;

//**************************************************************************************************
//* Window Assignment

typedef enum _DISPLAY_WINDOW
{
	CLEAR_WINDOW = 0,
	RESET_WINDOW,
	UPDATE_WINDOW,
	NEW_WINDOW,
} DISPLAY_WINDOW;

//**************************************************************************************************
//* Data Logging Reading Type

typedef enum _TYPE_OF_REPORT
{
	NO_READING_EVENT = 0,
	LOG_EVENT,
	RATE_ALERT_EVENT,
	RATE_ALARM_EVENT,
	DOSE_EVENT,
	DOSE_ALERT_EVENT,
	DOSE_ALARM_EVENT,
	BACKGROUND_EVENT,
	SOURCE_CHECK_EVENT,
	TEMP_EVENT																						// (9DP v1.02.01)
} TYPE_OF_REPORT;

//**************************************************************************************************
//* Months of the Year

typedef enum _MONTHS_OF_THE_YEAR
{
	JANUARY = 1,
	FEBRUARY,
	MARCH,
	APRIL,
	MAY,
	JUNE,
	JULY,
	AUGUST,
	SEPTEMBER,
	OCTOBER,
	NOVEMBER,
	DECEMBER
} MONTHS_OF_THE_YEAR;

//**************************************************************************************************
//* View Modes

typedef enum _VIEW_MODES
{
	VIEW_1 = 0,
	VIEW_2,
	VIEW_3,
	VIEW_4,
	VIEW_DEFAULT,																					// powerup in the default state
	VIEW_LAST,																						// powerup in the last known state
	END_VIEW_MODES
} VIEW_MODES;

//**************************************************************************************************
//*	File Image State

typedef enum _IMAGE_STATE
{
	IMAGE_START = 0,
	MEMORY_ADDRESS,
	IMAGE_SIZE,
	IMAGE_PALLETE,
	IMAGE_DATA
} IMAGE_STATE;

//**************************************************************************************************
//*	Backlight Modes

typedef enum _MODE_BACKLIGHT
{
	BK_LIGHT_OFF = 0,
	BK_LIGHT_ON,
	BK_LIGHT_AUTO,
	BK_LIGHT_TEMP_ON,
	BK_LIGHT_DEFAULT,
	BK_LIGHT_LAST,
	END_MODE_BACKLIGHT
} TYPE_MODE_BACKLIGHT;

//**************************************************************************************************
//*	Audio Levels

typedef enum _LEVEL_AUDIO																			// will be used if audio mode exceeds 2 options
{
	AUDIO_0 = 0,
	AUDIO_20,
	AUDIO_40,
	AUDIO_60,
	AUDIO_80,
	AUDIO_100,
	END_LEVEL_AUDIO
} TYPE_LEVEL_AUDIO;

//**************************************************************************************************
//**************************************************************************************************
//*	Enumerations
//**************************************************************************************************
//**************************************************************************************************
//*	Memory Structures

typedef struct _SHORT_READING_FORMAT
{
	unsigned short value;
	unsigned char units;
	unsigned char multiplier;
} TYPE_SHORT_READING_FORMAT;

typedef struct _LONG_READING_FORMAT
{
	unsigned long value;
	unsigned char units;
	unsigned char multiplier;
} TYPE_LONG_READING_FORMAT;

typedef struct _DOUBLE_LONG_READING_FORMAT
{
	unsigned long long value;
	unsigned char units;
	unsigned char multiplier;
} TYPE_DOUBLE_LONG_READING_FORMAT;

typedef struct _FLOAT_READING_FORMAT
{
	float value;
	unsigned char units;
	unsigned char multiplier;
} TYPE_FLOAT_READING_FORMAT;

//**************************************************************************************************
//*	Time Structure

typedef struct _TIME_FORMAT
{
	unsigned char hours;																			// time - hours
	unsigned char minutes;																			// time - minutes
	unsigned char seconds;																			// time - seconds
	unsigned char format;																			// time - format
} TYPE_TIME_FORMAT;

//**************************************************************************************************
//*	Date Structure

typedef struct _DATE_FORMAT
{
	unsigned char month;																			// date - month
	unsigned char day;																				// date - day
	unsigned char year;																				// date - year
} TYPE_DATE_FORMAT;

//**************************************************************************************************
//**************************************************************************************************
//*	INSTURMENT
//**************************************************************************************************
//**************************************************************************************************
//*	The instrument contains multiple tiers of data:
//*
//*		SETTINGS
//*		FEATURES
//*		CONTROLS
//*		VIEWS
//*		FUNCTIONS
//*		ALERTS
//*		ALARMS
//*

//**************************************************************************************************
//**************************************************************************************************
//*	SETTINGS TIER
//**************************************************************************************************
//**************************************************************************************************
//*	SETTINGS - PRODUCT

#define LENGTH_MODEL								16
#define LENGTH_SERIAL_NUMBER						16
#define LENGTH_USER_ID_NUMBER						16
#define LENGTH_USER_SERIAL_NUMBER					16

typedef struct _PRODUCT
{
	unsigned char model[LENGTH_MODEL];																// Ludlum defined model
	unsigned char serial_number[LENGTH_SERIAL_NUMBER];												// Ludlum defined serial number
	unsigned char user_id_number[LENGTH_USER_ID_NUMBER];											// user defined ID
	unsigned char user_serial_number[LENGTH_USER_SERIAL_NUMBER];									// user defined serial number
	TYPE_DATE_FORMAT calibration_date;																// date instrument was calibrated
	TYPE_DATE_FORMAT calibration_due_date;															// date instrument requires calibration
	unsigned char model_number;																		// numerical model representation
	unsigned char checksum;																			// data structure checksum
} TYPE_PRODUCT;

#define TOTAL_PRODUCT								EEPROM_PAGE_SIZE
#define SIZE_PRODUCT 								sizeof(TYPE_PRODUCT)
#define UNUSED_PRODUCT 								(signed)(TOTAL_PRODUCT - SIZE_PRODUCT)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_product_error[1 - 2 * (UNUSED_PRODUCT < 0)];

//**************************************************************************************************
//*	SETTINGS - LANGUAGE

#define LENGTH_LANGUAGE_STRING						16

typedef struct _LANGUAGE
{
	unsigned char index;																			// primary index within the language array
	unsigned char string[LENGTH_LANGUAGE_STRING];													// string format
	unsigned char checksum;																			// data structure checksum
} TYPE_LANGUAGE;

#define TOTAL_LANGUAGE								EEPROM_PAGE_SIZE
#define SIZE_LANGUAGE 								sizeof(TYPE_LANGUAGE)
#define UNUSED_LANGUAGE 							(signed)(TOTAL_LANGUAGE - SIZE_LANGUAGE)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_language_error[1 - 2 * (UNUSED_LANGUAGE < 0)];

//**************************************************************************************************
//* SETTINGS - DATE

#define LENGTH_DATE_STRING							16

typedef struct _DATE
{
	TYPE_DATE_FORMAT date;																			// actual date
	unsigned char string[LENGTH_DATE_STRING];														// string format
	unsigned char checksum;																			// data structure checksum
} TYPE_DATE;

#define TOTAL_DATE									EEPROM_PAGE_SIZE
#define SIZE_DATE 									sizeof(TYPE_DATE)
#define UNUSED_DATE 								(signed)(TOTAL_DATE - SIZE_DATE)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_date_error[1 - 2 * (UNUSED_DATE < 0)];

//**************************************************************************************************
//* SETTINGS - TIME

#define LENGTH_TIME_STRING							16

typedef struct _TIME
{
	TYPE_TIME_FORMAT time;																			// actual time
	unsigned char string[LENGTH_TIME_STRING];														// string format
	unsigned char checksum;																			// data structure checksum
} TYPE_TIME;

#define TOTAL_TIME									EEPROM_PAGE_SIZE
#define SIZE_TIME 									sizeof(TYPE_TIME)
#define UNUSED_TIME	 								(signed)(TOTAL_TIME - SIZE_TIME)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_time_error[1 - 2 * (UNUSED_TIME < 0)];

//**************************************************************************************************
//*	SETTINGS - PASSWORDS

#define LENGTH_PASSWORD								16

typedef struct _PASSWORDS
{
	unsigned char enable;
	unsigned char password_1[LENGTH_PASSWORD];
	unsigned char password_2[LENGTH_PASSWORD];
	unsigned char password_3[LENGTH_PASSWORD];
	unsigned char checksum;																			// data structure checksum
} TYPE_PASSWORDS;

#define TOTAL_PASSWORDS								EEPROM_PAGE_SIZE
#define SIZE_PASSWORDS 								sizeof(TYPE_PASSWORDS)
#define UNUSED_PASSWORDS							(signed)(TOTAL_PASSWORDS - SIZE_PASSWORDS)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_passwords_error[1 - 2 * (UNUSED_PASSWORDS < 0)];

//**************************************************************************************************
//*	SETTINGS - MISC

#define LENGTH_FIRMWARE_NUMBER						16

typedef struct _SETTINGS_MISC
{
	unsigned char firmware[LENGTH_FIRMWARE_NUMBER];
	unsigned char checksum;																			// data structure checksum
} TYPE_SETTINGS_MISC;

#define TOTAL_SETTINGS_MISC							EEPROM_PAGE_SIZE
#define SIZE_SETTINGS_MISC 							sizeof(TYPE_SETTINGS_MISC)
#define UNUSED_SETTINGS_MISC						(signed)(TOTAL_SETTINGS_MISC - SIZE_SETTINGS_MISC)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_settings_misc_error[1 - 2 * (UNUSED_SETTINGS_MISC < 0)];

//**************************************************************************************************
//*	INSTRUMENT TIER - SETTINGS

typedef struct _SETTINGS
{
	union
	{
		TYPE_PRODUCT info;
		unsigned char buffer[SIZE_PRODUCT];
	} product;

	union
	{
		TYPE_LANGUAGE info;
		unsigned char buffer[SIZE_LANGUAGE];
	} language;

	union
	{
		TYPE_DATE info;
		unsigned char buffer[SIZE_DATE];
	} calendar;

	union
	{
		TYPE_TIME info;
		unsigned char buffer[SIZE_TIME];
	} clock;

	union
	{
		TYPE_PASSWORDS info;
		unsigned char buffer[SIZE_PASSWORDS];
	} passwords;

	union
	{
		TYPE_SETTINGS_MISC info;
		unsigned char buffer[SIZE_SETTINGS_MISC];
	} misc;

} TYPE_SETTINGS;

//**************************************************************************************************
//**************************************************************************************************
//*	FEATURES TIER
//**************************************************************************************************
//**************************************************************************************************
//*	FEATURES - SOURCE CHECK

typedef struct _CHECK
{
	unsigned char powerup_mode;																		// enum MODE_POWERUP
	unsigned char operating_mode;																	// current operating mode
	unsigned char delay;																			// length of delay after power up before check
	unsigned char log_result;																		// log result after check?
	unsigned char sample_time;																		// time length of sample in seconds
	TYPE_FLOAT_READING_FORMAT initial_reading;														// initial reading
	TYPE_DATE_FORMAT initial_date;																	// initial reading date
	TYPE_TIME_FORMAT initial_time;																	// initial reading time
	TYPE_FLOAT_READING_FORMAT last_reading;															// last reading
	TYPE_DATE_FORMAT last_date;																		// last reading date
	TYPE_TIME_FORMAT last_time;																		// last reading time
	unsigned char checksum;																			// data structure checksum
} TYPE_CHECK;

#define TOTAL_SOURCE_CHECK							EEPROM_PAGE_SIZE
#define SIZE_SOURCE_CHECK 							sizeof(TYPE_CHECK)
#define UNUSED_SOURCE_CHECK							(signed)(TOTAL_SOURCE_CHECK - SIZE_SOURCE_CHECK)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_source_check_error[1 - 2 * (UNUSED_SOURCE_CHECK < 0)];

//**************************************************************************************************
//*	FEATURES - BACKGROUND CHECK

//* No structure defined as it is the same as the source check structure

#define TOTAL_BACKGROUND_CHECK						EEPROM_PAGE_SIZE
#define SIZE_BACKGROUND_CHECK						sizeof(TYPE_CHECK)
#define UNUSED_BACKGROUND_CHECK						(signed)(TOTAL_BACKGROUND_CHECK - SIZE_BACKGROUND_CHECK)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_background_check_error[1 - 2 * (UNUSED_BACKGROUND_CHECK < 0)];

//**************************************************************************************************
//*	FEATURES - INTEGRATE

typedef struct _INTEGRATE
{
	unsigned char powerup_mode;																		// enum MODE_POWERUP
	unsigned char operating_mode;																	// current operating mode
	unsigned char delay;																			// length of delay after power up before integration begins
	unsigned char startup_log;																		// log current value at startup
	unsigned char shutdown_log;																		// log current value at shutdown
	unsigned char save_on_shutdown;																	// save current integrated value on shutdown
	TYPE_FLOAT_READING_FORMAT current_reading;														// current reading
	unsigned char checksum;																			// data structure checksum
} TYPE_INTEGRATE;

#define TOTAL_INTEGRATE								EEPROM_PAGE_SIZE
#define SIZE_INTEGRATE								sizeof(TYPE_INTEGRATE)
#define UNUSED_INTEGRATE							(signed)(TOTAL_INTEGRATE - SIZE_INTEGRATE)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_integrate_error[1 - 2 * (UNUSED_INTEGRATE < 0)];

//**************************************************************************************************
//*	FEATURES - DATA LOGGING

typedef struct _DATA_LOGGING
{
	unsigned char powerup_mode;																		// enum MODE_POWERUP
	unsigned char operating_mode;																	// current operating mode
	unsigned char delay;																			// length of delay after power up before logging begins
	UINT16 sample_period;																			// time length between samples in seconds
	TYPE_FLOAT_READING_FORMAT last_reading;															// last reading
	TYPE_DATE_FORMAT last_date;																		// last reading date
	TYPE_TIME_FORMAT last_time;																		// last reading time
	struct
	{
		unsigned char model_number;																	// transmit model number
		unsigned char serial_number;																// transmit serial number
		unsigned char user_id_number;																// transmit user id number
		unsigned char user_serial_number;															// transmit user serial number
		unsigned char date;																			// transmit date
		unsigned char time;																			// transmit time
		unsigned char location;																		// transmit location
		unsigned char type_of_report;																// transmit type of report
		unsigned char reading;																		// transmit reading
		unsigned char reading_units;																// transmit reading units
		unsigned char reading_multiplier;															// transmit reading multiplier
		unsigned char integrated_reading;															// transmit integrated reading
		unsigned char integrated_reading_units;														// transmit integrated reading units
		unsigned char integrated_reading_multiplier;												// transmit integrated reading multiplier
		unsigned char temperature;																	// transmit temperature
		unsigned char high_voltage_1;																// transmit high voltage
		unsigned char input_voltage;																// transmit input voltage
		unsigned char battery_voltage;																// transmit battery voltage
		unsigned char undefined_1;																	// transmit
		unsigned char undefined_2;																	// transmit
		unsigned char undefined_3;																	// transmit
		unsigned char undefined_4;																	// transmit
		unsigned char undefined_5;																	// transmit
		unsigned char undefined_6;																	// transmit
		unsigned char undefined_7;																	// transmit
		unsigned char undefined_8;																	// transmit
		unsigned char undefined_9;																	// transmit
		unsigned char undefined_10;																	// transmit
		unsigned char undefined_11;																	// transmit
		unsigned char undefined_12;																	// transmit
		unsigned char undefined_13;																	// transmit
		unsigned char undefined_14;																	// transmit
		unsigned char undefined_15;																	// transmit
		unsigned char undefined_16;																	// transmit
		unsigned char system_status;																// transmit system status
	} reported_data;																				// which data to report
	unsigned char checksum;																			// data structure checksum
} TYPE_DATA_LOGGING;

#define TOTAL_DATA_LOGGING							EEPROM_PAGE_SIZE
#define SIZE_DATA_LOGGING							sizeof(TYPE_DATA_LOGGING)
#define UNUSED_DATA_LOGGING							(signed)(TOTAL_DATA_LOGGING - SIZE_DATA_LOGGING)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_data_logging_error[1 - 2 * (UNUSED_DATA_LOGGING < 0)];

//**************************************************************************************************
//*	INSTRUMENT TIER - FEATURES

typedef struct _FEATURES
{
	union
	{
		TYPE_CHECK info;
		unsigned char buffer[SIZE_SOURCE_CHECK];
	} source;
	union
	{
		TYPE_CHECK info;
		unsigned char buffer[SIZE_BACKGROUND_CHECK];
	} background;
	union
	{
		TYPE_INTEGRATE info;
		unsigned char buffer[SIZE_INTEGRATE];
	} integrate;
	union
	{
		TYPE_DATA_LOGGING info;
		unsigned char buffer[SIZE_DATA_LOGGING];
	} data_logging;

} TYPE_FEATURES;

//**************************************************************************************************
//**************************************************************************************************
//*	CONTROLS TIER
//**************************************************************************************************
//**************************************************************************************************
//*	CONTROLS - BACKLIGHT

typedef struct _BACKLIGHT
{
	unsigned char powerup_mode;																		// enum POWERUP_MODE
	unsigned char operating_mode;																	// enum BACKLIGHT_MODE
	unsigned char current_level;																	// percentage level: 0 - 100
	unsigned char auto_low;																			// percentage level: 0 - 100
	unsigned char auto_medium;																		// percentage level: 0 - 100
	unsigned char auto_high;																		// percentage level: 0 - 100
	unsigned char on_level;																			// percentage level: 0 - 100
	unsigned char temp_time;																		// temporary time in seconds
	unsigned char checksum;																			// data structure checksum
} TYPE_BACKLIGHT;

#define TOTAL_BACKLIGHT								EEPROM_PAGE_SIZE
#define SIZE_BACKLIGHT 								sizeof(TYPE_BACKLIGHT)
#define UNUSED_BACKLIGHT							(signed)(TOTAL_BACKLIGHT - SIZE_BACKLIGHT)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_backlight_error[1 - 2 * (UNUSED_BACKLIGHT < 0)];

//**************************************************************************************************
//*	CONTROLS - AUDIO

typedef struct _AUDIO
{
	unsigned char powerup_mode;
	unsigned char operating_mode;
	unsigned short current_level;
	unsigned short alert_level;
	unsigned short alarm_level;
	unsigned char checksum;																			// data structure checksum
} TYPE_AUDIO;

#define TOTAL_AUDIO									EEPROM_PAGE_SIZE
#define SIZE_AUDIO 									sizeof(TYPE_AUDIO)
#define UNUSED_AUDIO								(signed)(TOTAL_AUDIO - SIZE_AUDIO)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_audio_error[1 - 2 * (UNUSED_AUDIO < 0)];

//**************************************************************************************************
//*	CONTROLS - GPS

typedef struct _GPS
{
	unsigned char powerup_mode;
	unsigned char operating_mode;
	unsigned char checksum;																			// data structure checksum
} TYPE_GPS;

#define TOTAL_GPS									EEPROM_PAGE_SIZE
#define SIZE_GPS 									sizeof(TYPE_GPS)
#define UNUSED_GPS									(signed)(TOTAL_GPS - SIZE_GPS)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_gps_error[1 - 2 * (UNUSED_GPS < 0)];

//**************************************************************************************************
//*	CONTROLS - BLUETOOTH

typedef struct _BLUETOOTH
{
	unsigned char powerup_mode;
	unsigned char operating_mode;
	unsigned char checksum;																			// data structure checksum
} TYPE_BLUETOOTH;

#define TOTAL_BLUETOOTH								EEPROM_PAGE_SIZE
#define SIZE_BLUETOOTH 								sizeof(TYPE_BLUETOOTH)
#define UNUSED_BLUETOOTH							(signed)(TOTAL_BLUETOOTH - SIZE_BLUETOOTH)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_bluetooth_error[1 - 2 * (UNUSED_BLUETOOTH < 0)];

//**************************************************************************************************
//*	INSTRUMENT TIER - CONTROLS

typedef struct _CONTROLS
{
	union
	{
		TYPE_BACKLIGHT info;
		unsigned char buffer[SIZE_BACKLIGHT];
	} backlight;
	union
	{
		TYPE_AUDIO info;
		unsigned char buffer[SIZE_AUDIO];
	} audio;
	union
	{
		TYPE_GPS info;
		unsigned char buffer[SIZE_GPS];
	} gps;
	union
	{
		TYPE_BLUETOOTH info;
		unsigned char buffer[SIZE_BLUETOOTH];
	} bluetooth;

} TYPE_CONTROLS;

//**************************************************************************************************
//**************************************************************************************************
//*	DISPLAY TIER
//**************************************************************************************************
//**************************************************************************************************
//*	DISPLAY - PROPERTIES

typedef struct _DISPLAY_PROPERTIES
{
	unsigned char powerup_mode;
	unsigned char current_detector;
	unsigned char checksum;																			// data structure checksum
} TYPE_DISPLAY_PROPERTIES;

#define TOTAL_DISPLAY_PROPERTIES					EEPROM_PAGE_SIZE
#define SIZE_DISPLAY_PROPERTIES						sizeof(TYPE_DISPLAY_PROPERTIES)
#define UNUSED_DISPLAY_PROPERTIES					(signed) (TOTAL_DISPLAY_PROPERTIES - SIZE_DISPLAY_PROPERTIES)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_display_properties_error[1 - 2 * (UNUSED_DISPLAY_PROPERTIES < 0)];

//**************************************************************************************************
//*	DETECTOR x - PROPERTIES

//#define LENGTH_MODEL								16												// previously defined
//#define LENGTH_SERIAL_NUMBER						16												// previously defined
//#define LENGTH_USER_ID_NUMBER						16												// previously defined
//#define LENGTH_USER_SERIAL_NUMBER					16												// previously defined
#define LENGTH_CALIBRATION_TECHNICIAN				16

typedef struct _DETECTOR_PROPERTIES
{
	unsigned short model;																			// Ludlum defined model
	unsigned char serial_number[LENGTH_SERIAL_NUMBER];												// Ludlum defined serial number
	unsigned char user_id_number[LENGTH_USER_ID_NUMBER];											// user defined ID
	unsigned char user_serial_number[LENGTH_USER_SERIAL_NUMBER];									// user defined serial number
	unsigned char technician[LENGTH_CALIBRATION_TECHNICIAN];										// calibration technician
	unsigned char checksum;
        unsigned char autoRate;
        float fixedRate;
} TYPE_DETECTOR_PROPERTIES;

#define TOTAL_DETECTOR_PROPERTIES					EEPROM_PAGE_SIZE
#define SIZE_DETECTOR_PROPERTIES					sizeof(TYPE_DETECTOR_PROPERTIES)
#define UNUSED_DETECTOR_PROPERTIES					(signed)(TOTAL_DETECTOR_PROPERTIES - SIZE_DETECTOR_PROPERTIES)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_detector_properties_error[1 - 2 * (UNUSED_DETECTOR_PROPERTIES < 0)];

//**************************************************************************************************
//*	DETECTOR x - CALIBRATION

typedef struct _DETECTOR_CALIBRATION
{
	unsigned short u_arg[16];																		// 32 bytes
	signed short s_arg[16];																			// 32 bytes
	float f_arg[8];																					// 32 bytes
	unsigned char checksum;																			// 1 byte
} TYPE_DETECTOR_CALIBRATION;																		// 97 bytes total

#define TOTAL_DETECTOR_CALIBRATION					EEPROM_PAGE_SIZE
#define SIZE_DETECTOR_CALIBRATION					sizeof(TYPE_DETECTOR_CALIBRATION)
#define UNUSED_DETECTOR_CALIBRATION					(signed)(TOTAL_DETECTOR_CALIBRATION - SIZE_DETECTOR_CALIBRATION)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_detector_calibration_error[1 - 2 * (UNUSED_DETECTOR_CALIBRATION < 0)];

//**************************************************************************************************
//*	DETECTOR x - VIEWS SETTINGS

typedef struct _DETECTOR_VIEW_SETUP
{
	unsigned char powerup_mode;
	unsigned char current_view;
	unsigned char checksum;
} TYPE_DETECTOR_VIEW_SETUP;

#define TOTAL_DETECTOR_VIEW_SETUP					EEPROM_PAGE_SIZE
#define SIZE_DETECTOR_VIEW_SETUP					sizeof(TYPE_DETECTOR_VIEW_SETUP)
#define UNUSED_DETECTOR_VIEW_SETUP					(signed)(TOTAL_DETECTOR_VIEW_SETUP - SIZE_DETECTOR_VIEW_SETUP)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_view_settings_error[1 - 2 * (UNUSED_DETECTOR_VIEW_SETUP < 0)];

//**************************************************************************************************
//*	DETECTOR x - VIEWS

//typedef struct _VIEWS
//{
//	union
//	{
//		TYPE_VIEWS_SETUP info;
//		unsigned char buffer[SIZE_VIEWS_SETUP];
//	} settings;
//} TYPE_VIEWS;

//**************************************************************************************************
//*	VIEW - APPEARANCE

typedef struct _APPEARANCE
{
	unsigned short background_color;
	unsigned short foreground_color;
	unsigned char display_time;
	unsigned char display_view_name;
	unsigned char checksum;																			// data structure checksum
} TYPE_APPEARANCE;

#define TOTAL_APPEARANCE							EEPROM_PAGE_SIZE
#define SIZE_APPEARANCE								sizeof(TYPE_APPEARANCE)
#define UNUSED_APPEARANCE							(signed)(TOTAL_APPEARANCE - SIZE_APPEARANCE)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_appearance_error[1 - 2 * (UNUSED_APPEARANCE < 0)];

//**************************************************************************************************
//*	VIEW - ELEMENT HEADER

#define SIZE_ELEMENT_HEADER_NAME					16

typedef struct _ELEMENT_HEADER
{
	ELEMENT_STYLE style;
	unsigned char name[SIZE_ELEMENT_HEADER_NAME];
	unsigned char checksum;																			// data structure checksum
} TYPE_ELEMENT_HEADER;

#define TOTAL_ELEMENT_HEADER						EEPROM_PAGE_SIZE
#define SIZE_ELEMENT_HEADER							sizeof(TYPE_ELEMENT_HEADER)
#define UNUSED_ELEMENT_HEADER						(signed)(TOTAL_ELEMENT_HEADER - SIZE_ELEMENT_HEADER)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_element_header_error[1 - 2 * (UNUSED_ELEMENT_HEADER < 0)];

//**************************************************************************************************
//*	VIEW - ELEMENT PROTOTYPES

#define TOTAL_ELEMENT								EEPROM_PAGE_SIZE
#define MEMORY_ELEMENT								TOTAL_ELEMENT - 4

//**************************************************************************************************
//* ELEMENT - ARC

typedef struct _ARC
{
	unsigned char type_of_scale;
	union
	{
		unsigned char number_of_divisions;
		unsigned char number_of_ranges;
	} scale;
	UINT16 end_point;
	unsigned char digits;
	UNIT_TYPE units;
	UNIT_MULTIPLIER multiplier;
} TYPE_ARC;																									

#define SIZE_ARC									sizeof(TYPE_ARC)
#define UNUSED_ARC									(signed)(MEMORY_ELEMENT - SIZE_ARC)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_arc_error[1 - 2 * (UNUSED_ARC < 0)];

//**************************************************************************************************
//*	VIEW - ELEMENT

typedef struct _ELEMENT
{
	union
	{
		struct
		{
			TYPE_ARC control;
			unsigned char dummy[UNUSED_ARC];
		} arc;
		unsigned char buffer[MEMORY_ELEMENT];
	};
	unsigned char checksum;																			// data structure checksum
} TYPE_ELEMENT;																									

#define SIZE_ELEMENT								sizeof(TYPE_ELEMENT)
#define UNUSED_ELEMENT								(signed)(TOTAL_ELEMENT - SIZE_ELEMENT)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_element_error[1 - 2 * (UNUSED_ELEMENT < 0)];

//**************************************************************************************************
//*	VIEW - FUNCTIONS PROTOTYPE

typedef struct _VIEW_FUNCTIONS_DEFINE
{
	unsigned char enable;
	unsigned char function_index;
} TYPE_VIEW_FUNCTIONS_DEFINE;

//**************************************************************************************************
//*	VIEW - FUNCTIONS

#define DEFAULT_VIEW_NUMBER_OF_FUNCTIONS			4
#define DEFAULT_VIEW_FUNCTIONS_CURRENT_FUNCTION		0

typedef struct _VIEW_FUNCTIONS
{
	unsigned char current_function;
	TYPE_VIEW_FUNCTIONS_DEFINE function[DEFAULT_VIEW_NUMBER_OF_FUNCTIONS];
	unsigned char checksum;																			// data structure checksum
} TYPE_VIEW_FUNCTIONS;

#define TOTAL_VIEW_FUNCTIONS						EEPROM_PAGE_SIZE
#define SIZE_VIEW_FUNCTIONS							sizeof(TYPE_VIEW_FUNCTIONS)
#define UNUSED_VIEW_FUNCTIONS						(signed)(TOTAL_VIEW_FUNCTIONS - SIZE_VIEW_FUNCTIONS)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_view_functions_error[1 - 2 * (UNUSED_VIEW_FUNCTIONS < 0)];

//**************************************************************************************************
//*	VIEW - MESSAGE PROTOTYPE

#define SIZE_CUSTOM_MESSAGE							50

typedef struct _MESSAGE
{
	unsigned char enable;																			// enable message
	unsigned char custom;																			// enable custom message
	TYPE_SHORT_READING_FORMAT reading;																// alert value point
	float relative_reading;																			// reading relative to storage multiplier
	unsigned char custom_message[SIZE_CUSTOM_MESSAGE];												// custom message
} TYPE_ALERT_MESSAGE, TYPE_ALARM_MESSAGE;

//**************************************************************************************************
//*	VIEW - ALERTS RADIATION

typedef struct _ALERTS_RADIATION
{
	TYPE_ALERT_MESSAGE feature;
	unsigned char checksum;																			// data structure checksum
} TYPE_ALERTS_RADIATION;

#define TOTAL_ALERTS_RADIATION						EEPROM_PAGE_SIZE
#define SIZE_ALERTS_RADIATION						sizeof(TYPE_ALERTS_RADIATION)
#define UNUSED_ALERTS_RADIATION						(signed)(TOTAL_ALERTS_RADIATION - SIZE_ALERTS_RADIATION)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_alerts_radiation_error[1 - 2 * (UNUSED_ALERTS_RADIATION < 0)];

//**************************************************************************************************
//*	VIEW - ALERTS INTEGRATE

typedef struct _ALERTS_INTEGRATE
{
	TYPE_ALERT_MESSAGE feature;
	unsigned char checksum;																			// data structure checksum
} TYPE_ALERTS_INTEGRATE;

#define TOTAL_ALERTS_INTEGRATE						EEPROM_PAGE_SIZE
#define SIZE_ALERTS_INTEGRATE						sizeof(TYPE_ALERTS_INTEGRATE)
#define UNUSED_ALERTS_INTEGRATE						(signed)(TOTAL_ALERTS_INTEGRATE - SIZE_ALERTS_INTEGRATE)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_alerts_integrate_error[1 - 2 * (UNUSED_ALERTS_INTEGRATE < 0)];

//**************************************************************************************************
//*	VIEW - ALERTS FUNCTION

typedef struct _ALERTS_FUNCTION
{
	unsigned char function_select;
	TYPE_ALERT_MESSAGE feature;
	unsigned char checksum;																			// data structure checksum
} TYPE_ALERTS_FUNCTION;

#define TOTAL_ALERTS_FUNCTION						EEPROM_PAGE_SIZE
#define SIZE_ALERTS_FUNCTION						sizeof(TYPE_ALERTS_FUNCTION)
#define UNUSED_ALERTS_FUNCTION						(signed)(TOTAL_ALERTS_FUNCTION - SIZE_ALERTS_FUNCTION)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_alerts_function_error[1 - 2 * (UNUSED_ALERTS_FUNCTION < 0)];

//**************************************************************************************************
//*	VIEW - ALARMS RADIATION

typedef struct _ALARMS_RADIATION
{
	TYPE_ALERT_MESSAGE feature;
	unsigned char checksum;																			// data structure checksum
} TYPE_ALARMS_RADIATION;

#define TOTAL_ALARMS_RADIATION						EEPROM_PAGE_SIZE
#define SIZE_ALARMS_RADIATION						sizeof(TYPE_ALARMS_RADIATION)
#define UNUSED_ALARMS_RADIATION						(signed)(TOTAL_ALARMS_RADIATION - SIZE_ALARMS_RADIATION)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_alerts_radiation_error[1 - 2 * (UNUSED_ALARMS_RADIATION < 0)];

//**************************************************************************************************
//*	VIEW - ALARMS INTEGRATE

typedef struct _ALARMS_INTEGRATE
{
	TYPE_ALERT_MESSAGE feature;
	unsigned char checksum;																			// data structure checksum
} TYPE_ALARMS_INTEGRATE;

#define TOTAL_ALARMS_INTEGRATE						EEPROM_PAGE_SIZE
#define SIZE_ALARMS_INTEGRATE						sizeof(TYPE_ALARMS_INTEGRATE)
#define UNUSED_ALARMS_INTEGRATE						(signed)(TOTAL_ALARMS_INTEGRATE - SIZE_ALARMS_INTEGRATE)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_alerts_integrate_error[1 - 2 * (UNUSED_ALARMS_INTEGRATE < 0)];

//**************************************************************************************************
//*	VIEW - ALARMS FUNCTION

typedef struct _ALARMS_FUNCTION
{
	unsigned char function_select;
	TYPE_ALERT_MESSAGE feature;
	unsigned char checksum;																			// data structure checksum
} TYPE_ALARMS_FUNCTION;

#define TOTAL_ALARMS_FUNCTION						EEPROM_PAGE_SIZE
#define SIZE_ALARMS_FUNCTION						sizeof(TYPE_ALARMS_FUNCTION)
#define UNUSED_ALARMS_FUNCTION						(signed)(TOTAL_ALARMS_FUNCTION - SIZE_ALARMS_FUNCTION)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_alerts_function_error[1 - 2 * (UNUSED_ALARMS_FUNCTION < 0)];

//**************************************************************************************************
//*	VIEW - CALIBRATION

typedef struct _VIEW_CALIBRATION
{
	unsigned short u_arg[16];																		// 32 bytes
	signed short s_arg[16];																			// 32 bytes
	float f_arg[8];																					// 32 bytes
	unsigned char checksum;																			// 1 byte
} TYPE_VIEW_CALIBRATION;

#define TOTAL_VIEW_CALIBRATION						EEPROM_PAGE_SIZE
#define SIZE_VIEW_CALIBRATION						sizeof(TYPE_VIEW_CALIBRATION)
#define UNUSED_VIEW_CALIBRATION						(signed)(TOTAL_VIEW_CALIBRATION - SIZE_VIEW_CALIBRATION)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_view_calibration_error[1 - 2 * (UNUSED_VIEW_CALIBRATION < 0)];

//**************************************************************************************************
//*	INSTRUMENT TIER - VIEW - 9DP

typedef struct _VIEW
{
	union
	{
		TYPE_APPEARANCE info;
		unsigned char buffer[SIZE_APPEARANCE];
	} appearance;
	union
	{
		TYPE_ELEMENT_HEADER info;
		unsigned char buffer[SIZE_ELEMENT_HEADER];
	} element_header;
	union
	{
		TYPE_ELEMENT info;
		unsigned char buffer[SIZE_ELEMENT];
	} element;
	union
	{
		TYPE_VIEW_FUNCTIONS info;
		unsigned char buffer[SIZE_VIEW_FUNCTIONS];
	} functions;
	union
	{
		TYPE_ALERTS_RADIATION info;
		unsigned char buffer[SIZE_ALERTS_RADIATION];
	} alerts_radiation;
	union
	{
		TYPE_ALERTS_INTEGRATE info;
		unsigned char buffer[SIZE_ALERTS_INTEGRATE];
	} alerts_integrate;
	union
	{
		TYPE_ALERTS_FUNCTION info;
		unsigned char buffer[SIZE_ALERTS_FUNCTION];
	} alerts_function;
	union
	{
		TYPE_ALARMS_RADIATION info;
		unsigned char buffer[SIZE_ALARMS_RADIATION];
	} alarms_radiation;
	union
	{
		TYPE_ALARMS_INTEGRATE info;
		unsigned char buffer[SIZE_ALARMS_INTEGRATE];
	} alarms_integrate;
	union
	{
		TYPE_ALARMS_FUNCTION info;
		unsigned char buffer[SIZE_ALARMS_FUNCTION];
	} alarms_function;
} TYPE_VIEW;

//**************************************************************************************************
//*	INSTRUMENT TIER - VIEW - OTHER INSTRUMENTS

typedef struct _VIEW_OTHER
{
	union
	{
		TYPE_APPEARANCE info;
		unsigned char buffer[SIZE_APPEARANCE];
	} appearance;
	union
	{
		TYPE_ELEMENT_HEADER info;
		unsigned char buffer[SIZE_ELEMENT_HEADER];
	} element_header;
	union
	{
		TYPE_ELEMENT info;
		unsigned char buffer[SIZE_ELEMENT];
	} element;
	union
	{
		TYPE_VIEW_FUNCTIONS info;
		unsigned char buffer[SIZE_VIEW_FUNCTIONS];
	} functions;
	union
	{
		TYPE_ALERTS_RADIATION info;
		unsigned char buffer[SIZE_ALERTS_RADIATION];
	} alerts_radiation;
	union
	{
		TYPE_ALERTS_INTEGRATE info;
		unsigned char buffer[SIZE_ALERTS_INTEGRATE];
	} alerts_integrate;
	union
	{
		TYPE_ALERTS_FUNCTION info;
		unsigned char buffer[SIZE_ALERTS_FUNCTION];
	} alerts_function;
	union
	{
		TYPE_ALARMS_RADIATION info;
		unsigned char buffer[SIZE_ALARMS_RADIATION];
	} alarms_radiation;
	union
	{
		TYPE_ALARMS_INTEGRATE info;
		unsigned char buffer[SIZE_ALARMS_INTEGRATE];
	} alarms_integrate;
	union
	{
		TYPE_ALARMS_FUNCTION info;
		unsigned char buffer[SIZE_ALARMS_FUNCTION];
	} alarms_function;
	union
	{
		TYPE_VIEW_CALIBRATION info;
		unsigned char buffer[SIZE_VIEW_CALIBRATION];
	} calibration;
} TYPE_VIEW_OTHER;

//**************************************************************************************************
//**************************************************************************************************
//*	FUNCTIONS TIER
//**************************************************************************************************
//**************************************************************************************************
//*	FUNCTIONS - FUNCTION PROTOTYPES

#define SIZE_FUNCTION_NAME			16
#define SIZE_FUNCTION_CONTROL			100												// (9DP v1.02.02)
#define DEFAULT_FUNCTION_X_NAME			""

//* Function Header

//typedef struct _FUNCTION_HEADER
typedef struct __attribute__(( __packed__))															// (9DP v1.02.02)
{
	FUNCTION_TYPE type;																				// function type identifier
	UINT8 name[SIZE_FUNCTION_NAME];																	// function name
	UINT8 unused;																					// force packed structure to be 18 bytes long to match original unpacked structure (9DP v1.02.02)
} TYPE_FUNCTION_HEADER;																	

//* Integrate

//typedef struct _FUNCTION_INTEGRATE
typedef struct __attribute__(( __packed__))															// (9DP v1.02.02)
{
	UINT8 detector;																					// associated detector
	UINT8 multiplier;																				// units multiplier
	UINT8 units;																					// units identifier
	UINT8 digits;																					// number of digits to display
	UINT8 range_lock;																				// number of digits to display
	UINT8 unused;																					// force packed structure to be 8 bytes long to match original unpacked structure (9DP v1.02.02)
	UINT16 time;																					// time in seconds for timed integration
} TYPE_FUNCTION_INTEGRATE;

//* Peak Rate

//typedef struct _FUNCTION_PEAK_RATE
typedef struct __attribute__(( __packed__))															// (9DP v1.02.02)
{
	UINT8 digits;																					// number of digits to display
	UINT8 element_range_lock;																		// lock the range from changing
} TYPE_FUNCTION_PEAK_RATE;

//* Correction Factor (9DP v1.02.02)

#define SIZE_CORRECTION_FACTOR						4

//typedef struct _FUNCTION_CORRECTION_FACTOR														// (9DP v1.02.02)
typedef struct __attribute__(( __packed__))
{
	UINT8 range[SIZE_CORRECTION_FACTOR];															// correction factor range
	float factor[SIZE_CORRECTION_FACTOR];															// correction factor
} TYPE_FUNCTION_CORRECTION_FACTOR;


// Function Time Constants
typedef struct __attribute__(( __packed__))
{
	float range[5];													// The time constants for each range in seconds
} TYPE_FUNCTION_TIME_CONSTANTS;

//* Function Prototype

//typedef struct _FUNCTION_PROTOTYPE
typedef struct __attribute__(( __packed__))															// (9DP v1.02.02)
{
	TYPE_FUNCTION_HEADER header;
	union
	{
		TYPE_FUNCTION_INTEGRATE integrate;
		TYPE_FUNCTION_PEAK_RATE peak_rate;
		TYPE_FUNCTION_INTEGRATE timed_dose;
		TYPE_FUNCTION_CORRECTION_FACTOR correction_factor;											// correction factor (9DP v1.02.02)
        TYPE_FUNCTION_TIME_CONSTANTS time_constant;                                                           // time constants (9DP v1.04.00)
	};
	unsigned char checksum;																			// data structure checksum
} TYPE_FUNCTION_PROTOTYPE;																								

//**************************************************************************************************
//*	FUNCTIONS - FUNCTION

#define SIZE_FUNCTION_PROTOTYPE						sizeof(TYPE_FUNCTION_PROTOTYPE)

typedef struct _FUNCTION
{
	union
	{
		TYPE_FUNCTION_PROTOTYPE info;
		unsigned char buffer[SIZE_FUNCTION_PROTOTYPE];
	};
} TYPE_FUNCTION;

#define TOTAL_FUNCTION								EEPROM_PAGE_SIZE
#define SIZE_FUNCTION								sizeof(TYPE_FUNCTION)
#define UNUSED_FUNCTION								(signed)(TOTAL_FUNCTION - SIZE_FUNCTION)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_function_error[1 - 2 * (UNUSED_FUNCTION < 0)];

//**************************************************************************************************
//**************************************************************************************************
//*	ALERT/ALARM MESSAGE PROTOTYPES
//**************************************************************************************************
//**************************************************************************************************

typedef struct _FLOAT_ALERT_ALARM_PROTOTYPE
{
	unsigned char enable;																			// enable message
	unsigned char custom;																			// enable custom message
	float value;																					// alert value point
	unsigned char custom_message[SIZE_CUSTOM_MESSAGE];												// custom message
} TYPE_FLOAT_ALERT_ALARM;

typedef struct _BYTE_ALERT_ALARM_PROTOTYPE
{
	unsigned char enable;																			// enable message
	unsigned char custom;																			// enable custom message
	BYTE value;																						// alert value point
	unsigned char custom_message[SIZE_CUSTOM_MESSAGE];												// custom message
} TYPE_BYTE_ALERT_ALARM;

typedef struct _UINT16_ALERT_ALARM_PROTOTYPE
{
	unsigned char enable;																			// enable message
	unsigned char custom;																			// enable custom message
	UINT16 value;																					// alert value point
	unsigned char custom_message[SIZE_CUSTOM_MESSAGE];												// custom message
} TYPE_UINT16_ALERT_ALARM;

typedef struct _INT16_ALERT_ALARM_PROTOTYPE
{
	unsigned char enable;																			// enable message
	unsigned char custom;																			// enable custom message
	INT16 value;																					// alert value point
	unsigned char custom_message[SIZE_CUSTOM_MESSAGE];												// custom message
} TYPE_INT16_ALERT_ALARM;

//**************************************************************************************************
//**************************************************************************************************
//*	ALERTS TIER
//**************************************************************************************************
//**************************************************************************************************

#define ALERTS_SIZE									EEPROM_PAGE_SIZE

//**************************************************************************************************
//*	ALERTS - LOW_BATTERY

typedef struct _ALERTS_LOW_BATTERY
{
	TYPE_FLOAT_ALERT_ALARM feature;
	unsigned char checksum;																			// data structure checksum
} TYPE_ALERTS_LOW_BATTERY;

#define TOTAL_ALERTS_LOW_BATTERY					ALERTS_SIZE
#define SIZE_ALERTS_LOW_BATTERY						sizeof(TYPE_ALERTS_LOW_BATTERY)
#define UNUSED_ALERTS_LOW_BATTERY					(signed)(TOTAL_ALERTS_LOW_BATTERY - SIZE_ALERTS_LOW_BATTERY)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_alerts_low_battery[1 - 2 * (UNUSED_ALERTS_LOW_BATTERY < 0)];

//**************************************************************************************************
//*	ALERTS - LOW_MEMORY

typedef struct _ALERTS_LOW_MEMORY
{
	TYPE_BYTE_ALERT_ALARM feature;
	unsigned char checksum;																			// data structure checksum
} TYPE_ALERTS_LOW_MEMORY;

#define TOTAL_ALERTS_LOW_MEMORY						ALERTS_SIZE
#define SIZE_ALERTS_LOW_MEMORY						sizeof(TYPE_ALERTS_LOW_MEMORY)
#define UNUSED_ALERTS_LOW_MEMORY					(signed)(TOTAL_ALERTS_LOW_MEMORY - SIZE_ALERTS_LOW_MEMORY)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_alerts_low_memory[1 - 2 * (UNUSED_ALERTS_LOW_MEMORY < 0)];

//**************************************************************************************************
//*	ALERTS - CALIBRATION_DUE

typedef struct _ALERTS_CALIBRATION_DUE
{
	TYPE_BYTE_ALERT_ALARM feature;
	unsigned char checksum;																			// data structure checksum
} TYPE_ALERTS_CALIBRATION_DUE;

#define TOTAL_ALERTS_CALIBRATION_DUE				ALERTS_SIZE
#define SIZE_ALERTS_CALIBRATION_DUE					sizeof(TYPE_ALERTS_CALIBRATION_DUE)
#define UNUSED_ALERTS_CALIBRATION_DUE				(signed)(TOTAL_ALERTS_CALIBRATION_DUE - SIZE_ALERTS_CALIBRATION_DUE)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_alerts_calibration_due[1 - 2 * (UNUSED_ALERTS_CALIBRATION_DUE < 0)];

//**************************************************************************************************
//*	ALERTS - SOURCE_CHECK

typedef struct _ALERTS_SOURCE_CHECK
{
	TYPE_ALERT_MESSAGE feature;
	unsigned char checksum;																			// data structure checksum
} TYPE_ALERTS_SOURCE_CHECK;

#define TOTAL_ALERTS_SOURCE_CHECK					ALERTS_SIZE
#define SIZE_ALERTS_SOURCE_CHECK					sizeof(TYPE_ALERTS_SOURCE_CHECK)
#define UNUSED_ALERTS_SOURCE_CHECK					(signed)(TOTAL_ALERTS_SOURCE_CHECK - SIZE_ALERTS_SOURCE_CHECK)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_alerts_source_check[1 - 2 * (UNUSED_ALERTS_SOURCE_CHECK < 0)];

//**************************************************************************************************
//*	ALERTS - BACKGROUND_CHECK

typedef struct _ALERTS_BACKGROUND_CHECK
{
	TYPE_ALERT_MESSAGE feature;
	unsigned char checksum;																			// data structure checksum
} TYPE_ALERTS_BACKGROUND_CHECK;

#define TOTAL_ALERTS_BACKGROUND_CHECK				ALERTS_SIZE
#define SIZE_ALERTS_BACKGROUND_CHECK				sizeof(TYPE_ALERTS_BACKGROUND_CHECK)
#define UNUSED_ALERTS_BACKGROUND_CHECK				(signed)(TOTAL_ALERTS_BACKGROUND_CHECK - SIZE_ALERTS_BACKGROUND_CHECK)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_alerts_background_check[1 - 2 * (UNUSED_ALERTS_BACKGROUND_CHECK < 0)];

//**************************************************************************************************
//*	ALERTS

typedef struct _ALERTS
{
	union
	{
		TYPE_ALERTS_LOW_BATTERY info;
		unsigned char buffer[SIZE_ALERTS_LOW_BATTERY];
	} low_battery;
	union
	{
		TYPE_ALERTS_LOW_MEMORY info;
		unsigned char buffer[SIZE_ALERTS_LOW_MEMORY];
	} low_memory;
	union
	{
		TYPE_ALERTS_CALIBRATION_DUE info;
		unsigned char buffer[SIZE_ALERTS_CALIBRATION_DUE];
	} calibration_due;
	union
	{
		TYPE_ALERTS_SOURCE_CHECK info;
		unsigned char buffer[SIZE_ALERTS_SOURCE_CHECK];
	} source_check;
	union
	{
		TYPE_ALERTS_BACKGROUND_CHECK info;
		unsigned char buffer[SIZE_ALERTS_BACKGROUND_CHECK];
	} background_check;
} TYPE_ALERTS;

//**************************************************************************************************
//**************************************************************************************************
//*	ALARMS TIER
//**************************************************************************************************
//**************************************************************************************************

#define ALARMS_SIZE									EEPROM_PAGE_SIZE

//**************************************************************************************************
//*	ALARMS - LOW_BATTERY

typedef struct _ALARMS_LOW_BATTERY
{
	TYPE_FLOAT_ALERT_ALARM feature;
	unsigned char checksum;																			// data structure checksum
} TYPE_ALARMS_LOW_BATTERY;

#define TOTAL_ALARMS_LOW_BATTERY					ALARMS_SIZE
#define SIZE_ALARMS_LOW_BATTERY						sizeof(TYPE_ALARMS_LOW_BATTERY)
#define UNUSED_ALARMS_LOW_BATTERY					(signed)(TOTAL_ALARMS_LOW_BATTERY - SIZE_ALARMS_LOW_BATTERY)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_alarms_low_battery[1 - 2 * (UNUSED_ALARMS_LOW_BATTERY < 0)];

//**************************************************************************************************
//*	ALARMS - LOW_MEMORY

typedef struct _ALARMS_LOW_MEMORY
{
	TYPE_BYTE_ALERT_ALARM feature;
	unsigned char checksum;																			// data structure checksum
} TYPE_ALARMS_LOW_MEMORY;

#define TOTAL_ALARMS_LOW_MEMORY						ALARMS_SIZE
#define SIZE_ALARMS_LOW_MEMORY						sizeof(TYPE_ALARMS_LOW_MEMORY)
#define UNUSED_ALARMS_LOW_MEMORY					(signed)(TOTAL_ALARMS_LOW_MEMORY - SIZE_ALARMS_LOW_MEMORY)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_alarms_low_memory[1 - 2 * (UNUSED_ALARMS_LOW_MEMORY < 0)];

//**************************************************************************************************
//*	ALARMS - CALIBRATION_DUE

typedef struct _ALARMS_CALIBRATION_DUE
{
	TYPE_BYTE_ALERT_ALARM feature;
	unsigned char checksum;																			// data structure checksum
} TYPE_ALARMS_CALIBRATION_DUE;

#define TOTAL_ALARMS_CALIBRATION_DUE				ALARMS_SIZE
#define SIZE_ALARMS_CALIBRATION_DUE					sizeof(TYPE_ALARMS_CALIBRATION_DUE)
#define UNUSED_ALARMS_CALIBRATION_DUE				(signed)(TOTAL_ALARMS_CALIBRATION_DUE - SIZE_ALARMS_CALIBRATION_DUE)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_alarms_calibration_due[1 - 2 * (UNUSED_ALARMS_CALIBRATION_DUE < 0)];

//**************************************************************************************************
//*	ALARMS - SOURCE_CHECK

typedef struct _ALARMS_SOURCE_CHECK
{
	TYPE_ALARM_MESSAGE feature;
	unsigned char checksum;																			// data structure checksum
} TYPE_ALARMS_SOURCE_CHECK;

#define TOTAL_ALARMS_SOURCE_CHECK					ALARMS_SIZE
#define SIZE_ALARMS_SOURCE_CHECK					sizeof(TYPE_ALARMS_SOURCE_CHECK)
#define UNUSED_ALARMS_SOURCE_CHECK					(signed)(TOTAL_ALARMS_SOURCE_CHECK - SIZE_ALARMS_SOURCE_CHECK)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_alarms_source_check[1 - 2 * (UNUSED_ALARMS_SOURCE_CHECK < 0)];

//**************************************************************************************************
//*	ALARMS - BACKGROUND_CHECK

typedef struct _ALARMS_BACKGROUND_CHECK
{
	TYPE_ALARM_MESSAGE feature;
	unsigned char checksum;																			// data structure checksum
} TYPE_ALARMS_BACKGROUND_CHECK;

#define TOTAL_ALARMS_BACKGROUND_CHECK				ALARMS_SIZE
#define SIZE_ALARMS_BACKGROUND_CHECK				sizeof(TYPE_ALARMS_BACKGROUND_CHECK)
#define UNUSED_ALARMS_BACKGROUND_CHECK				(signed)(TOTAL_ALARMS_BACKGROUND_CHECK - SIZE_ALARMS_BACKGROUND_CHECK)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_alarms_background_check[1 - 2 * (UNUSED_ALARMS_BACKGROUND_CHECK < 0)];

//**************************************************************************************************
//*	ALARMS - HV LOW

typedef struct _ALARMS_HV_LOW
{
	TYPE_INT16_ALERT_ALARM feature;
	unsigned char checksum;																			// data structure checksum
} TYPE_ALARMS_HV_LOW;

#define TOTAL_ALARMS_HV_LOW							ALARMS_SIZE
#define SIZE_ALARMS_HV_LOW							sizeof(TYPE_ALARMS_HV_LOW)
#define UNUSED_ALARMS_HV_LOW						(signed)(TOTAL_ALARMS_HV_LOW - SIZE_ALARMS_HV_LOW)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_alarms_hv_low[1 - 2 * (UNUSED_ALARMS_HV_LOW < 0)];

//**************************************************************************************************
//*	ALARMS - HV HIGH

typedef struct _ALARMS_HV_HIGH
{
	TYPE_INT16_ALERT_ALARM feature;
	unsigned char checksum;																			// data structure checksum
} TYPE_ALARMS_HV_HIGH;

#define TOTAL_ALARMS_HV_HIGH						ALARMS_SIZE
#define SIZE_ALARMS_HV_HIGH							sizeof(TYPE_ALARMS_HV_HIGH)
#define UNUSED_ALARMS_HV_HIGH						(signed)(TOTAL_ALARMS_HV_HIGH - SIZE_ALARMS_HV_HIGH)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_alarms_hv_high[1 - 2 * (UNUSED_ALARMS_HV_HIGH < 0)];

//**************************************************************************************************
//*	ALARMS - LOW_READING

typedef struct _ALARMS_LOW_READING
{
	TYPE_ALARM_MESSAGE feature;
	UINT16 time;
	unsigned char checksum;																			// data structure checksum
} TYPE_ALARMS_LOW_READING;

#define TOTAL_ALARMS_LOW_READING					ALARMS_SIZE
#define SIZE_ALARMS_LOW_READING						sizeof(TYPE_ALARMS_LOW_READING)
#define UNUSED_ALARMS_LOW_READING					(signed)(TOTAL_ALARMS_LOW_READING - SIZE_ALARMS_LOW_READING)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_alarms_low_reading[1 - 2 * (UNUSED_ALARMS_LOW_READING < 0)];

//**************************************************************************************************
//*	ALARMS

typedef struct _ALARMS
{
	union
	{
		TYPE_ALARMS_LOW_BATTERY info;
		unsigned char buffer[SIZE_ALARMS_LOW_BATTERY];
	} low_battery;
	union
	{
		TYPE_ALARMS_LOW_MEMORY info;
		unsigned char buffer[SIZE_ALARMS_LOW_MEMORY];
	} low_memory;
	union
	{
		TYPE_ALARMS_CALIBRATION_DUE info;
		unsigned char buffer[SIZE_ALARMS_CALIBRATION_DUE];
	} calibration_due;
	union
	{
		TYPE_ALARMS_SOURCE_CHECK info;
		unsigned char buffer[SIZE_ALARMS_SOURCE_CHECK];
	} source_check;
	union
	{
		TYPE_ALARMS_BACKGROUND_CHECK info;
		unsigned char buffer[SIZE_ALARMS_BACKGROUND_CHECK];
	} background_check;
	union
	{
		TYPE_ALARMS_HV_LOW info;
		unsigned char buffer[SIZE_ALARMS_HV_LOW];
	} hv_low;
	union
	{
		TYPE_ALARMS_HV_HIGH info;
		unsigned char buffer[SIZE_ALARMS_HV_HIGH];
	} hv_high;
	union
	{
		TYPE_ALARMS_LOW_READING info;
		unsigned char buffer[SIZE_ALARMS_LOW_READING];
	} low_reading;
} TYPE_ALARMS;

//**************************************************************************************************
//**************************************************************************************************
//* INSTRUMENT TIER - FUNCTIONS
//**************************************************************************************************
//**************************************************************************************************

//* no tier description needed

//**************************************************************************************************
//**************************************************************************************************
//* CALIBRATION TIER
//**************************************************************************************************
//**************************************************************************************************

#define CALIBRATION_BUFFER_SIZE						0x1000

//**************************************************************************************************
//* CALIBRATION - HEADER

//#define LENGTH_CALIBRATION_TECHNICIAN				16												// previously defined

typedef struct _CALIBRATION_HEADER
{
	unsigned char instrument_type;
	unsigned char technician[LENGTH_CALIBRATION_TECHNICIAN];
	unsigned char checksum;																			// data structure checksum
} TYPE_CALIBRATION_HEADER;																									

#define TOTAL_CALIBRATION_HEADER					EEPROM_PAGE_SIZE
#define SIZE_CALIBRATION_HEADER						sizeof(TYPE_CALIBRATION_HEADER)
#define UNUSED_CALIBRATION_HEADER					(signed)(TOTAL_CALIBRATION_HEADER - SIZE_CALIBRATION_HEADER)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_calibration_header_error[1 - 2 * (UNUSED_CALIBRATION_HEADER < 0)];

//**************************************************************************************************
//* CALIBRATION - CHECKOUT

#define LENGTH_CHECKOUT_TECHNICIAN					16

typedef struct _CALIBRATION_CHECKOUT
{
	unsigned char technician[LENGTH_CALIBRATION_TECHNICIAN];
	TYPE_DATE_FORMAT date;																			// date main board was checked out
	unsigned char checksum;																			// data structure checksum
} TYPE_CALIBRATION_CHECKOUT;

#define TOTAL_CALIBRATION_CHECKOUT					EEPROM_PAGE_SIZE
#define SIZE_CALIBRATION_CHECKOUT					sizeof(TYPE_CALIBRATION_CHECKOUT)
#define UNUSED_CALIBRATION_CHECKOUT					(signed)(TOTAL_CALIBRATION_CHECKOUT - SIZE_CALIBRATION_CHECKOUT)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_calibration_checkout_error[1 - 2 * (UNUSED_CALIBRATION_CHECKOUT < 0)];

//**************************************************************************************************
//* CALIBRATION - CONSTANTS

typedef struct _CALIBRATION_CONSTANTS
{
	unsigned short range[5];
	unsigned short high_voltage;
	unsigned short battery;
	unsigned char checksum;																			// data structure checksum
} TYPE_CALIBRATION_CONSTANTS;

#define TOTAL_CALIBRATION_CONSTANTS					EEPROM_PAGE_SIZE
#define SIZE_CALIBRATION_CONSTANTS					sizeof(TYPE_CALIBRATION_CONSTANTS)
#define UNUSED_CALIBRATION_CONSTANTS				(signed)(TOTAL_CALIBRATION_CONSTANTS - SIZE_CALIBRATION_CONSTANTS)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_calibration_constants_error[1 - 2 * (UNUSED_CALIBRATION_CONSTANTS < 0)];

//**************************************************************************************************
//* CALIBRATION - OFFSETS

typedef struct _CALIBRATION_OFFSETS
{
	signed short adc_offset;
	signed short dac_offset;
	unsigned char checksum;																			// data structure checksum
} TYPE_CALIBRATION_OFFSETS;

#define TOTAL_CALIBRATION_OFFSETS					EEPROM_PAGE_SIZE
#define SIZE_CALIBRATION_OFFSETS					sizeof(TYPE_CALIBRATION_OFFSETS)
#define UNUSED_CALIBRATION_OFFSETS					(signed)(TOTAL_CALIBRATION_OFFSETS - SIZE_CALIBRATION_OFFSETS)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_calibration_offsets_error[1 - 2 * (UNUSED_CALIBRATION_OFFSETS < 0)];

//**************************************************************************************************
//* CALIBRATION - HIGH VOLTAGE BOARD - 9DP

typedef struct _CALIBRATION_HVB
{
	signed short dac_1_offset;
	signed short dac_2_offset;																		// unused placeholder
	unsigned short dac_1_output_1;																	// (9DP v1.01.02)
	unsigned short dac_1_output_2;																	// (9DP v1.01.02)
	unsigned short dac_1_output_3;																	// unused placeholder
	unsigned short dac_1_output_4;																	// unused placeholder
	unsigned short dac_2_output_1;																	// unused placeholder
	unsigned short dac_2_output_2;																	// unused placeholder
	unsigned short dac_2_output_3;																	// unused placeholder
	unsigned short dac_2_output_4;																	// unused placeholder
	UINT16 meter_offset_gain_1;																		// meter offset for range x1/x100
	UINT16 meter_offset_gain_2;																		// meter offset for range x10/x1k
	UINT16 meter_offset_gain_3;																		// meter offset for range x10k
	float cal_temperature;																			// calibration temperature
	float temperature_offset_cold;																	// correction for cold temperatures
	float temperature_offset_hot;																	// correction for hot temperatures
	float temperature_offset_hot_10;																// correction for hot temperatures (9DP v1.02.07)
	float temperature_offset_hot_15;																// correction for hot temperatures (9DP v1.02.07)
	float temperature_offset_hot_20;																// correction for hot temperatures (9DP v1.02.07)
	unsigned char checksum;																			// data structure checksum
} TYPE_CALIBRATION_HVB;

#define TOTAL_CALIBRATION_HVB						EEPROM_PAGE_SIZE
#define SIZE_CALIBRATION_HVB						sizeof(TYPE_CALIBRATION_HVB)
#define UNUSED_CALIBRATION_HVB						(signed)(TOTAL_CALIBRATION_HVB - SIZE_CALIBRATION_HVB)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_calibration_hvb_error[1 - 2 * (UNUSED_CALIBRATION_HVB < 0)];

//**************************************************************************************************
//* CALIBRATION - HIGH VOLTAGE BOARD - OTHER

typedef struct _CALIBRATION_HVB_OTHER
{
	signed short dac_1_offset;																		// single channel
	signed short dac_2_offset;																		// dual channel
	unsigned short threshold_1_setpoint_1;															// threshold 1 setpoint 1 (3D v1.00.04)
	unsigned short threshold_1_setpoint_2;															// threshold 1 setpoint 2 (3D v1.00.04)
	unsigned short threshold_2_setpoint_1;															// threshold 2 setpoint 1 (3D v1.00.04)
	unsigned short threshold_2_setpoint_2;															// threshold 2 setpoint 2 (3D v1.00.04)
	signed short threshold_1_offset_1;																// threshold 1 offset 1 (3D v1.00.04)
	signed short threshold_1_offset_2;																// threshold 1 offset 2 (3D v1.00.04)
	signed short threshold_2_offset_1;																// threshold 2 offset 1 (3D v1.00.04)
	signed short threshold_2_offset_2;																// threshold 2 offset 2 (3D v1.00.04)
	unsigned short window_1_setpoint_1;																// window 1 setpoint 1 (3D v1.00.04)
	unsigned short window_1_setpoint_2;																// window 1 setpoint 2 (3D v1.00.04)
	unsigned short window_2_setpoint_1;																// window 2 setpoint 1 (3D v1.00.04)
	unsigned short window_2_setpoint_2;																// window 2 setpoint 2 (3D v1.00.04)
	signed short window_1_offset_1;																	// window 1 offset 1 (3D v1.00.04)
	signed short window_1_offset_2;																	// window 1 offset 2 (3D v1.00.04)
	signed short window_2_offset_1;																	// window 2 offset 1 (3D v1.00.04)
	signed short window_2_offset_2;																	// window 2 offset 2 (3D v1.00.04)
	unsigned char checksum;																			// data structure checksum
} TYPE_CALIBRATION_HVB_OTHER;

#define TOTAL_CALIBRATION_HVB_OTHER					EEPROM_PAGE_SIZE
#define SIZE_CALIBRATION_HVB_OTHER					sizeof(TYPE_CALIBRATION_HVB_OTHER)
#define UNUSED_CALIBRATION_HVB_OTHER				(signed)(TOTAL_CALIBRATION_HVB_OTHER - SIZE_CALIBRATION_HVB_OTHER)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_calibration_hvb_other_error[1 - 2 * (UNUSED_CALIBRATION_HVB_OTHER < 0)];

//**************************************************************************************************
//* CALIBRATION - CONTROLS

typedef struct _CALIBRATION_CONTROLS
{
	unsigned char fixed_range;
	unsigned char stream_data_reading;
	unsigned char stream_report_time;
	unsigned char checksum;																			// data structure checksum
} TYPE_CALIBRATION_CONTROLS;

#define TOTAL_CALIBRATION_CONTROLS					EEPROM_PAGE_SIZE
#define SIZE_CALIBRATION_CONTROLS					sizeof(TYPE_CALIBRATION_CONTROLS)
#define UNUSED_CALIBRATION_CONTROLS					(signed)(TOTAL_CALIBRATION_CONTROLS - SIZE_CALIBRATION_CONTROLS)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_calibration_controls_error[1 - 2 * (UNUSED_CALIBRATION_CONTROLS < 0)];

//**************************************************************************************************
//* CALIBRATION - CONFIG

typedef struct _CALIBRATION_CONFIG
{
	float uR_filter;
	unsigned char checksum;																			// data structure checksum
} TYPE_CALIBRATION_CONFIG;

#define TOTAL_CALIBRATION_CONFIG					EEPROM_PAGE_SIZE
#define SIZE_CALIBRATION_CONFIG						sizeof(TYPE_CALIBRATION_CONFIG)
#define UNUSED_CALIBRATION_CONFIG					(signed)(TOTAL_CALIBRATION_CONFIG - SIZE_CALIBRATION_CONFIG)

// The following is used after every EEPROM structure is defined. This will generate an error if the structure exceeds the allowed
// memory space because it will try to allocate an array with negative elements. This may be commented out after the program has
// been compiled.

//BYTE sizeof_calibration_config_error[1 - 2 * (UNUSED_CALIBRATION_CONFIG < 0)];

//**************************************************************************************************
//*	INSTRUMENT TIER - CALIBRATION - 9DP

typedef struct _CALIBRATION
{
	union
	{
		TYPE_CALIBRATION_HEADER info;
		unsigned char buffer[SIZE_CALIBRATION_HEADER];
	} header;
	union
	{
		TYPE_CALIBRATION_CHECKOUT info;
		unsigned char buffer[SIZE_CALIBRATION_CHECKOUT];
	} checkout;
	union
	{
		TYPE_CALIBRATION_CONSTANTS info;
		unsigned char buffer[SIZE_CALIBRATION_CONSTANTS];
	} constants;
	union
	{
		TYPE_CALIBRATION_OFFSETS info;
		unsigned char buffer[SIZE_CALIBRATION_OFFSETS];
	} offsets;
	union
	{
		TYPE_CALIBRATION_HVB info;
		unsigned char buffer[SIZE_CALIBRATION_HVB];
	} high_voltage_board;
	union
	{
		TYPE_CALIBRATION_CONTROLS info;
		unsigned char buffer[SIZE_CALIBRATION_CONTROLS];
	} controls;
	union
	{
		TYPE_CALIBRATION_CONFIG info;
		unsigned char buffer[SIZE_CALIBRATION_CONFIG];
	} config;
} TYPE_CALIBRATION;

//**************************************************************************************************
//*	INSTRUMENT TIER - CALIBRATION - OTHER INSTRUMENTS

typedef struct _CALIBRATION_OTHER
{
	union
	{
		TYPE_CALIBRATION_HEADER info;
		unsigned char buffer[SIZE_CALIBRATION_HEADER];
	} header;
	union
	{
		TYPE_CALIBRATION_CHECKOUT info;
		unsigned char buffer[SIZE_CALIBRATION_CHECKOUT];
	} checkout;
	union
	{
		TYPE_CALIBRATION_CONSTANTS info;
		unsigned char buffer[SIZE_CALIBRATION_CONSTANTS];
	} constants;
	union
	{
		TYPE_CALIBRATION_OFFSETS info;
		unsigned char buffer[SIZE_CALIBRATION_OFFSETS];
	} offsets;
	union
	{
		TYPE_CALIBRATION_HVB_OTHER info;
		unsigned char buffer[SIZE_CALIBRATION_HVB_OTHER];
	} high_voltage_board;
	union
	{
		TYPE_CALIBRATION_CONTROLS info;
		unsigned char buffer[SIZE_CALIBRATION_CONTROLS];
	} controls;
	union
	{
		TYPE_CALIBRATION_CONFIG info;
		unsigned char buffer[SIZE_CALIBRATION_CONFIG];
	} config;
} TYPE_CALIBRATION_OTHER;

//*
//**************************************************************************************************
//**************************************************************************************************
//* END CALIBRATION
//**************************************************************************************************
//**************************************************************************************************

#endif

//*
//**************************************************************************************************
//**************************************************************************************************
//* LUDLUM MEASUREMENTS INC. COPYRIGHT (C) 2011
//**************************************************************************************************
//**************************************************************************************************
