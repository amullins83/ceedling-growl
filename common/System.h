//**************************************************************************************************
//**************************************************************************************************
//*																		
//*	Filename:		System.h
//*	Date:			07/12/11
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
//*	~~~~~~~~	07/12/11	James Comstock
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
//**************************************************************************************************

#ifndef _SYSTEM_H
#define _SYSTEM_H

//#include "instrument.h"
#include "Definitions.h"
#include "graphics.h"
#include "usb_host_hid.h"

//**************************************************************************************************
//**************************************************************************************************
//	Defines
//**************************************************************************************************
//**************************************************************************************************
//*

//#define SYS_FREQ 						80000000UL													// (hidden Test v0.00.01)
#define SYS_FREQ 						72000000UL													// (Test v0.00.01)
#define SYS_FREQ_72MHz 					72000000UL													// (9DP v1.02.00)
#define SYS_FREQ_80MHz 					80000000UL													// (9DP v1.02.00)
#define PB_DIV         					1
#define PRESCALE       					1
#define T1_TICK       					(32768/PB_DIV/PRESCALE/TICKS_PER_SEC)
#define SAMPLES_PER_SEC					8

#define MAX_ALLOWED_CURRENT             500         												// Maximum power we can supply in mA
#define MAX_ERROR_COUNTER               10
#define NUMBER_OF_ERRORS 				10															// moved here from screens.c (9DP v1.01.04)

#define USAGE_PAGE_LEDS                 0x08
#define USAGE_PAGE_KEY_CODES            0x07
#define USAGE_MIN_MODIFIER_KEY          0xE0
#define USAGE_MAX_MODIFIER_KEY          0xE7
#define USAGE_MIN_NORMAL_KEY            0x00
#define USAGE_MAX_NORMAL_KEY            0xFF

//**************************************************************************************************
//*	Array index for modifier keys

#define MODIFIER_LEFT_CONTROL           0
#define MODIFIER_LEFT_SHIFT             1
#define MODIFIER_LEFT_ALT               2
#define MODIFIER_LEFT_GUI               3
#define MODIFIER_RIGHT_CONTROL          4
#define MODIFIER_RIGHT_SHIFT            5
#define MODIFIER_RIGHT_ALT              6
#define MODIFIER_RIGHT_GUI              7

#define HID_CAPS_LOCK_VAL               0x39
#define HID_NUM_LOCK_VAL                0x53

//**************************************************************************************************
//*	Symbols

#define Symbol_Exclamation              0x1E
#define Symbol_AT                       0x1F
#define Symbol_Pound                    0x20
#define Symbol_Dollar                   0x21
#define Symbol_Percentage               0x22
#define Symbol_Cap                      0x23
#define Symbol_AND                      0x24
#define Symbol_Star                     0x25
#define Symbol_NormalBracketOpen        0x26
#define Symbol_NormalBracketClose       0x27

#define Symbol_Return                   0x28
#define Symbol_Escape                   0x29
#define Symbol_Backspace                0x2A
#define Symbol_Tab                      0x2B
#define Symbol_Space                    0x2C
#define Symbol_HyphenUnderscore         0x2D
#define Symbol_EqualAdd                 0x2E
#define Symbol_BracketOpen              0x2F
#define Symbol_BracketClose             0x30
#define Symbol_BackslashOR              0x31
#define Symbol_SemiColon                0x33
#define Symbol_InvertedComma            0x34
#define Symbol_Tilde                    0x35
#define Symbol_CommaLessThan            0x36
#define Symbol_PeriodGreaterThan        0x37
#define Symbol_FrontSlashQuestion       0x38

#define Symbol_F1						0x3A
#define Symbol_F2						0x3B
#define Symbol_F3						0x3C
#define Symbol_F4						0x3D
#define Symbol_F5						0x3E
#define Symbol_F6						0x3F
#define Symbol_F7						0x40
#define Symbol_F8						0x41
#define Symbol_F9						0x42
#define Symbol_F10						0x43
#define Symbol_F11						0x44
#define Symbol_F12						0x45

#define Symbol_PageUp					0x4B
#define Symbol_PageDown					0x4E
#define Symbol_RightArrow				0x4F
#define Symbol_LeftArrow				0x50
#define Symbol_DownArrow				0x51
#define Symbol_UpArrow					0x52

#define Symbol_Keypad_DownArrow			0x5A
#define Symbol_Keypad_PageDown			0x5B
#define Symbol_Keypad_LeftArrow			0x5C
#define Symbol_Keypad_RightArrow		0x5E
#define Symbol_Keypad_UpArrow			0x60
#define Symbol_Keypad_PageUp			0x61

//**************************************************************************************************
//**************************************************************************************************
//*	Data Structures
//**************************************************************************************************
//**************************************************************************************************

typedef struct _HID_REPORT_BUFFER
{
    WORD  Report_ID;
    WORD  ReportSize;
    BYTE* ReportData;
    WORD  ReportPollRate;
}   HID_REPORT_BUFFER;

typedef struct _HID_LED_REPORT_BUFFER
{
    BYTE  NUM_LOCK      : 1;
    BYTE  CAPS_LOCK     : 1;
    BYTE  SCROLL_LOCK   : 1;
    BYTE  UNUSED        : 5;
}   HID_LED_REPORT_BUFFER;

//**************************************************************************************************
//**************************************************************************************************
//	Variable Definitions
//**************************************************************************************************
//**************************************************************************************************
//*

GOL_SCHEME *altScheme;																				// alternative style scheme
GOL_SCHEME *btnScheme;																				// style scheme used for buttons

HID_DATA_DETAILS 								Appl_LED_Indicator;
HID_DATA_DETAILS 								Appl_ModifierKeysDetails;
HID_DATA_DETAILS 								Appl_NormalKeysDetails;
HID_USER_DATA_SIZE 								Appl_BufferModifierKeys[8];
HID_USER_DATA_SIZE 								Appl_BufferNormalKeys[6];
HID_USER_DATA_SIZE 								Appl_ShadowBuffer1[6];
HID_REPORT_BUFFER     							Appl_raw_report_buffer;
HID_LED_REPORT_BUFFER 							Appl_led_report_buffer;

//**************************************************************************************************
//**************************************************************************************************
//*	Function Definitions
//**************************************************************************************************
//**************************************************************************************************
//* Initialization Routines

void SystemInit(void);
void TimersInit(void);
void I2CInit(void);
void DisplayInit(void);
void MemoryInit(void);
void TimerInit(void);
void SchemeInit(void);
void VariableInit(void);
void RTCCInit(void);

//**************************************************************************************************
//*	System Routines

WORD GetADCValue(BYTE index);
WORD ExternalMemoryCallback(EXTDATA* memory, LONG offset, WORD nCount, void* buffer);

//**************************************************************************************************
//*	RTCC Routines

void RTCCUpdate(void);
void RTCCRead(void);
unsigned char convert_BCD(unsigned char binary_byte);
unsigned char unconvert_BCD(unsigned char BCD_byte);

//**************************************************************************************************
//*	I/O Routines

void Check_Audio_Status(void);
void Check_Battery(void);
void Check_High_Voltage(BYTE value);
void Check_Temperature(void);
#ifdef INSTRUMENT_9D
    void Check_Pressure(void);																			// (9DP v1.02.03)
#endif

//**************************************************************************************************
//*	Error Routines

void add_error_message_to_queue(unsigned char error);												// moved here from screens.h (9DP v1.01.04)
void clear_error_message_off_queue(void);															// moved here from screens.h (9DP v1.01.04)
void Check_Messages(void);																			// (9DP v1.01.04)
void Clear_All_Errors(void);																		// (9DP v1.01.08)
void Display_Error_Message(void);																	// moved here from screens.h (9DP v1.01.11)

//**************************************************************************************************
//*	Feature Routines

void Calculate_Integrate_Value(void);

//**************************************************************************************************
//*	Control Routines

void Set_Audio_Tone(void);
void Set_Audio_Volume(void);
void Set_Backlight_Level(char index);

//**************************************************************************************************
//*	Display Routines

void Create_Measurement_Screen(void);																// (9DP v1.01.04)
void Setup_Colors(void);																			// (9DP v1.01.04)
//void Delay_Reading_Xseconds(void);
//void CreateCtrlButtons(XCHAR *pTextA, XCHAR *pTextB, XCHAR *pTextC, XCHAR *pTextD);

//**************************************************************************************************
//*	Generic Calibration Routines

void CheckCalibrationDueDate(void);

//**************************************************************************************************
//*	USB Routines

BYTE USB_Detect_Device(void);
void Check_USB_Port(void);
void Check_FTDI_Adapter(void);
void Check_Thumbdrive(void);
void FTDI_Receive(void);
void FTDI_Transmit(void);
BOOL USBHostFTDI_DetectDevice(void);
BOOL File_System_Initialize(void);
BOOL USB_HID_DataCollectionHandler(void);
void Replace_Get_With_Set(void);																	// (9DP v1.02.08)

//**************************************************************************************************
//*	USB Keyboard Routines

void Check_Keyboard(void);
void USB_Keyboard_Routine(BYTE data, BYTE HIDData, BOOL state);
void USB_Keyboard_ProcessInputReport(void);
void USB_Keyboard_PrepareOutputReport(void);
void USB_Keyboard_ClearDataBuffer(void);
BOOL USB_Keyboard_CompareKeyPressedPrevBuf(BYTE data);
void USB_Keyboard_CopyToShadowBuffer(void);
BYTE USB_Keyboard_HID2ASCII(BYTE a);

//**************************************************************************************************
//*	Data Recording Routines

void Configure_Header(BYTE *message_buffer);
void Configure_Message(BYTE *message_buffer, BYTE message_type);
void Configure_Data_Stream(BYTE *message_buffer);
void Format_Text_String(char *generic_string, char *message, UINT8 message_length);
void Initialize_Reporting(BOOL log_state);

//**************************************************************************************************
//*	Alert and Alarm Routines

void Update_Alerts_Alarms(BYTE detector, BYTE view);
void Calculate_New_Radiation_Alert(BYTE detector, BYTE view);
void Calculate_New_Radiation_Alarm(BYTE detector, BYTE view);
void Calculate_New_Integrate_Alert(BYTE detector, BYTE view);
void Calculate_New_Integrate_Alarm(BYTE detector, BYTE view);

//**************************************************************************************************
//*	Shutdown Routines

void Check_Shutdown(void);
void Shutdown_Power(void);
void Software_Reset(void);

#endif

//*
//**************************************************************************************************
//**************************************************************************************************
//* LUDLUM MEASUREMENTS INC. COPYRIGHT (C) 2011
//**************************************************************************************************
//**************************************************************************************************
