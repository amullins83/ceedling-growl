//**************************************************************************************************
//**************************************************************************************************
//*																		
//*	Filename:		Commands.c
//*	Date:			09/13/10
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
//*	~~~~~~~~	05/23/12	James Comstock
//*	- updated the instrument to the latest Microchip applications library
//*	- eliminated all compiler warnings
//*
//*	~~~~~~~~	09/13/10	James Comstock
//*	- Original Prototype
//*                                                                     
//**************************************************************************************************
//*                                                                     
//*	Command Routines
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

#include "instrument.h"
 
//*
//**************************************************************************************************
//**************************************************************************************************
//* Defines

typedef enum _COMMAND_TYPE
{
	NO_COMMAND = 0,
	COMMAND_READ,
	COMMAND_WRITE,
	COMMAND_RESET,
	COMMAND_ERROR
} COMMAND_TYPE;

typedef enum _TIME_EVENTS
{
	NO_TIME_EVENT = 0,
	HOUR_EVENT,
	MINUTE_EVENT,
	SECOND_EVENT,
	FORMAT_EVENT
} TIME_EVENTS;

typedef enum _DATE_EVENTS
{
	NO_DATE_EVENT = 0,
	MONTH_EVENT,
	DAY_EVENT,
	YEAR_EVENT,
} DATE_EVENTS;

//*
//**************************************************************************************************
//**************************************************************************************************
//* External Memory Declarations

//**************************************************************************************************
//*	External - Instrument

extern TYPE_INSTRUMENT lmi;

//**************************************************************************************************
//*	External - Fonts

extern const FONT_FLASH Secondary_Arial_Black_14;

//**************************************************************************************************
//* External - States

extern COMPUTER_STATE PC_State;
extern SCREEN_STATES screenState;
extern SCREEN_STATES PreviousScreenState;

//**************************************************************************************************
//*	External - Variables

extern float Radiation_Alert_Rate;																	// Rate Alert Level
extern float Radiation_Alarm_Rate;																	// Rate Alarm Level
extern float Integrate_Alert_Rate;																	// Rate Alert Level
extern float Integrate_Alarm_Rate;																	// Rate Alert Level

extern float Rate_Accumulation;
extern float Battery_Reading;
extern float Temperature_Reading;
extern float High_Voltage_Reading;
extern float Peak_Rate_Reading;
extern float Temperature_Base;
extern float Temperature_Hot;
extern float Temperature_Cold;
extern float uR_filter;																				// lowest range uR filter
extern float Correction_Factor_Value;																// (9DP v1.02.02)

extern DWORD tick;										 											// tick counter (1/2 second)
extern DWORD log_file_write_tick;																	// log file write tick timer (1/2 second)
extern DWORD integrate_update_tick;																	// integrate update timer (1/2 seconds)
extern DWORD stream_report_tick;																	// data streaming tick timer

extern WORD foreground;
extern WORD background;

extern BYTE Current_View;
extern BYTE Current_Function_Index;
extern BYTE Current_Detector;
extern BYTE Range_Number;
extern BYTE Correction_Factor_Mode;																	// (9DP v1.02.02)

extern BYTE TXMessageIndex;

//**************************************************************************************************
//* External - Modes

extern BYTE Backlight_Mode;
extern BYTE Save_Config_To_Thumbdrive;																// changed to BYTE value (9DP v1.02.08)

//**************************************************************************************************
//* External - Flags

extern BOOL Transmit_Data;
extern BOOL Update_Log_File;
extern BOOL Data_Logging_Header_Sent;																// indicate if the data logging header has been sent
extern BOOL Power_Up_Report_Integrate;
extern BOOL Power_Down_Report_Integrate;
extern BOOL Range_Lock;
extern BOOL Range_Lock_Valid;
extern BOOL DisplayDetachOnce;
extern BOOL Set_Temperature_Offset;
extern BOOL Set_High_Temperature_Offset;
extern BOOL Set_High_Temperature_Offset_10;															// (9DP v1.02.07)
extern BOOL Set_High_Temperature_Offset_15;															// (9DP v1.02.07)
extern BOOL Set_High_Temperature_Offset_20;															// (9DP v1.02.07)
extern BOOL Set_Low_Temperature_Offset;
extern BOOL Set_Base_Temperature_Offset;
extern BOOL Data_Logging_Enabled;
extern BOOL Restart_System;																			// (9DP v1.01.12)
//extern BOOL Save_Config_To_Thumbdrive;															// (9DP v1.01.18)

extern InstrumentConstants *ic;

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Local Memory Declarations

volatile BYTE USB_RXMessage[MESSAGE_SIZE];
volatile BYTE USB_TXMessage[MESSAGE_SIZE];

char ColonSeparator[] 	= ":\0";
char MessageError[] 	= "ERROR\0";
char MessageOK[] 		= "OK\0";
char ReturnLinefeed[] 	= "\r\n\0";
char TimeEvent = 0;
char DateEvent = 0;

char CMD_Detector 		= 0;
char CMD_Range 			= 0;
char CMD_View 			= 0;
char CMD_Function 		= 0;

BOOL Write_EEPROM		= 0;

//**************************************************************************************************
//**************************************************************************************************
//*	INSTRUMENT HANDLER
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Message_Handler(char *message_string)
//*
//*	Description:
//* 	This is the message handler for any USB received message whether received via FTDI or via
//*		thumb drive.
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
//* 	Since all error messages must be indicated at the start of the message, ERROR: will be
//*		added to the beginning of the return message string and will not be transmitted if no
//*		error exists.
//*

void Message_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response;

	TXMessageIndex = 0;
	response = Terminate_Message((char *)USB_RXMessage);
	sprintf((char *)USB_TXMessage,"%s", MessageError);												// preload transmit buffer with error

	switch(response)
	{
		case VALID_MESSAGE:
		{
			Write_EEPROM = TRUE;
			message = Parse_Message((char *)USB_RXMessage);
			if(message != NULL)
			{
				//* Instrument
				sprintf(tier_command,"INS\0");
				response = strcmp(message,tier_command);
				if(response == 0)
				{
					strcat((char *)USB_TXMessage, ColonSeparator);									// ERROR:
					Append_Message(tier_command, ColonSeparator);									// ERROR:INS:
					if(!Instrument_Handler())
					{
						strcat((char *)USB_TXMessage, ReturnLinefeed);								// transmit return message beginning with the 1st byte
					}
					else							
					{
						TXMessageIndex = 6;															// transmit return message beginning with the 7th byte
					}
					Transmit_Data = TRUE;
					return;
				}

				//* Comment
				sprintf(tier_command,"//\0");														// (9DP v1.01.24)
				response = strcmp(message,tier_command);											// (9DP v1.01.24)
				if(response == 0)																	// (9DP v1.01.24)
				{																					// (9DP v1.01.24)
					strcat((char *)USB_TXMessage, ColonSeparator);									// (9DP v1.01.24)
					strcat((char *)USB_TXMessage, tier_command);									// (9DP v1.01.24)
					message = Parse_Message(NULL);													// (9DP v1.01.24)
					while(message != NULL)															// (9DP v1.01.24)
					{																				// (9DP v1.01.24)
						strcat((char *)USB_TXMessage, ColonSeparator);								// (9DP v1.01.24)
						strcat((char *)USB_TXMessage, message);										// (9DP v1.01.24)
						message = Parse_Message(NULL);												// (9DP v1.01.24)
					}																				// (9DP v1.01.24)
					strcat((char *)USB_TXMessage, ReturnLinefeed);									// (9DP v1.01.24)
					TXMessageIndex = 6;																// (9DP v1.01.24)
					Transmit_Data = TRUE;															// (9DP v1.01.24)
					return;																			// (9DP v1.01.24)
				}																					// (9DP v1.01.24)

				unsigned char index;																// (9DP v1.02.02)

				//* DEBUG!!!
				sprintf(tier_command,"BUG\0");														// (9DP v1.02.02)
				response = strcmp(message,tier_command);											// (9DP v1.02.02)
				if(response == 0)																	// (9DP v1.02.02)
				{																					// (9DP v1.02.02)
					strcat((char *)USB_TXMessage, ColonSeparator);									// (9DP v1.02.02)
					strcat((char *)USB_TXMessage, tier_command);									// (9DP v1.02.02)
					strcat((char *)USB_TXMessage, ColonSeparator);									// (9DP v1.02.02)
					message = Parse_Message(NULL);													// (9DP v1.02.02)
					response = (BYTE) atoi(message);												// (9DP v1.02.02)
					for(index = 0; index < SIZE_FUNCTION; index++)									// (9DP v1.02.02)
					{																				// (9DP v1.02.02)
						sprintf(tier_command,"%02X\0", lmi.function[response].buffer[index]);		// (9DP v1.02.02)
						strcat((char *)USB_TXMessage, tier_command);								// (9DP v1.02.02)
					}																				// (9DP v1.02.02)
					strcat((char *)USB_TXMessage, ReturnLinefeed);									// (9DP v1.02.02)
					TXMessageIndex = 6;																// (9DP v1.02.02)
					Transmit_Data = TRUE;															// (9DP v1.02.02)
					return;																			// (9DP v1.02.02)
				}																					// (9DP v1.02.02)
			}
			break;
		}
		case INVALID_MESSAGE_LENGTH:
		case INVALID_MESSAGE_TERMINATION:
		{
			break;
		}
	}

	strcat((char *)USB_TXMessage, ReturnLinefeed);
	Transmit_Data = TRUE;
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Instrument_Handler(void)
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
//* 	
//*		INS:STG:	- settings handler
//*		INS:FEA:	- features handler
//*		INS:CON:	- controls handler
//*		INS:DSP:	- display handler
//*		INS:FNC:	- function handler
//*		INS:ALE:	- alert handler
//*		INS:ALA:	- alarm handler
//*		INS:CAL:	- calibration handler
//*		---------------------------------
//*		INS:CNT		- connect
//*		INS:DCT		- disconnect
//*		INS:AOF		- main board ADC offset
//*		INS:MOF:	- meteroffset (9Dx only)
//*		INS:TOF		- temperature offset (9Dx only)
//*		INS:E2P:	- EEPROM routines
//*		INS:MEM		- SST39VF1601 routines
//*		INS:RST		- instrument reset
//*		---------------------------------
//*

BOOL Instrument_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response;
	BYTE process;
	BYTE value;
	UINT16 result;
	SHORT msgwidth;
	DWORD address;
	BYTE char_buffer[25] = {0};
	XCHAR xchar_buffer[25] = {0};

	message = Parse_Message(NULL);
	if(message != NULL)
	{
		//* Settings Handler
		sprintf(tier_command,"STG\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			return Settings_Handler();
		}
	
		//* Features Handler
		sprintf(tier_command,"FEA\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			return Features_Handler();
		}
	
		//* Controls Handler
		sprintf(tier_command,"CON\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			return Controls_Handler();
		}
	
		//* Display Handler
		sprintf(tier_command,"DSP\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			return Display_Handler();
		}
	
		//* Functions Handler
		sprintf(tier_command,"FNC\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			return Functions_Handler();
		}
	
		//* Alerts Handler
		sprintf(tier_command,"ALE\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			return Alerts_Handler();
		}
	
		//* Alarms Handler
		sprintf(tier_command,"ALA\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			return Alarms_Handler();
		}
	
		//* Calibration Handler
		sprintf(tier_command,"CAL\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			return Calibration_Handler();
		}

		//******************************************************************************************
		//* BEGIN DISCRETE COMMANDS

		//* Software Connect
		sprintf(tier_command,"CNT\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			PC_State = PC_CONNECTED;
			Append_Message(tier_command, ReturnLinefeed);
			return TRUE;
		}
	
		//* Software Disconnect
		sprintf(tier_command,"DCT\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			PC_State = PC_DISCONNECTED;
			Append_Message(tier_command, ReturnLinefeed);
			return TRUE;
		}
	
		//* ADC Offset Routine Handler
		sprintf(tier_command,"AOF\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			result = GetADCValue((BYTE) ADC_PORT5_EXTRA1);
			sprintf(tier_command,"%d\0",result);
			Append_Message(tier_command, ReturnLinefeed);
			return TRUE;
		}
	
#ifdef INSTRUMENT_9D																														// (9DP v1.01.13)
		//* Meter Offset Routine Handler (9D series only)
		sprintf(tier_command,"MOF\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			message = Parse_Message(NULL);
			if(message != NULL)
			{
				value = (BYTE) atoi(message);
				if(value < 4)
				{
					result = CalculateMeterOffset(value);
					i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_CALIBRATION_HVB_LOCATION, 2, lmi.calibration.high_voltage_board.buffer, SIZE_CALIBRATION_HVB);
					sprintf(tier_command,"%d\0",result);
					Append_Message(tier_command, ReturnLinefeed);
					return TRUE;
				}
			}
			return FALSE;
		}
#endif

#ifdef INSTRUMENT_9D
		//* Temperature Offset Routine Handler (9D series only) rewritten (9DP v1.02.08)
		sprintf(tier_command,"TOF\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			message = Parse_Message(NULL);
			if(message != NULL)
			{
				value = (BYTE) atoi(message);
				if(value == 0)
				{
					sprintf(tier_command,"%d\0", value);
					Append_Message(tier_command, ReturnLinefeed);
					lmi.display.detector[Current_Detector].calibration.info.f_arg[3] = lmi.calibration.high_voltage_board.info.temperature_offset_hot;
					lmi.display.detector[Current_Detector].calibration.info.f_arg[4] = lmi.calibration.high_voltage_board.info.temperature_offset_hot;
					lmi.display.detector[Current_Detector].calibration.info.f_arg[5] = lmi.calibration.high_voltage_board.info.temperature_offset_hot;
					StructureUpdate(6, Current_Detector);
					return TRUE;
				}
				else if(value == 1)
				{
					sprintf(tier_command,"%d\0", value);
					Append_Message(tier_command, ReturnLinefeed);
					lmi.display.detector[Current_Detector].calibration.info.f_arg[3] = lmi.calibration.high_voltage_board.info.temperature_offset_hot;
					lmi.display.detector[Current_Detector].calibration.info.f_arg[4] = lmi.calibration.high_voltage_board.info.temperature_offset_hot;
					lmi.display.detector[Current_Detector].calibration.info.f_arg[5] = lmi.calibration.high_voltage_board.info.temperature_offset_hot;
					StructureUpdate(5, Current_Detector);
					return TRUE;
				}
			}
			return FALSE;
		}
#endif
	
		//* EEPROM Routines
		sprintf(tier_command,"E2P\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			message = Parse_Message(NULL);
			if(message != NULL)
			{
				sprintf(tier_command,"ERP\0");
				response = strcmp(message,tier_command);
				if(response == 0)
				{
					DisplaySystemProcess(1);
					EEPROM_Clear(i2cADDR_MAIN_E2P_PRI);
					Restart_System = TRUE;																									// (9DP v1.01.12)
					screenState = NOT_DISPLAYED;																							// (9DP v1.01.12)
					Append_Message(tier_command, ReturnLinefeed);																			// (9DP v1.01.12)
					return TRUE;																											// (9DP v1.01.12)
				}

				sprintf(tier_command,"ERS\0");
				response = strcmp(message,tier_command);
				if(response == 0)
				{
					DisplaySystemProcess(2);
					EEPROM_Clear(i2cADDR_MAIN_E2P_SEC);
					Restart_System = TRUE;																									// (9DP v1.01.12)
					screenState = NOT_DISPLAYED;																							// (9DP v1.01.12)
					Append_Message(tier_command, ReturnLinefeed);																			// (9DP v1.01.12)
					return TRUE;																											// (9DP v1.01.12)
				}

				sprintf(tier_command,"TPS\0");
				response = strcmp(message,tier_command);
				if(response == 0)
				{
					DisplaySystemProcess(3);
					EEPROM_Transfer(i2cADDR_MAIN_E2P_PRI, i2cADDR_MAIN_E2P_SEC);
					Restart_System = TRUE;																									// (9DP v1.01.12)
					screenState = NOT_DISPLAYED;																							// (9DP v1.01.12)
					Append_Message(tier_command, ReturnLinefeed);																			// (9DP v1.01.12)
					return TRUE;																											// (9DP v1.01.12)
				}

				sprintf(tier_command,"TSP\0");
				response = strcmp(message,tier_command);
				if(response == 0)
				{
					DisplaySystemProcess(4);
					EEPROM_Transfer(i2cADDR_MAIN_E2P_SEC, i2cADDR_MAIN_E2P_PRI);
					Restart_System = TRUE;																									// (9DP v1.01.12)
					screenState = NOT_DISPLAYED;																							// (9DP v1.01.12)
					Append_Message(tier_command, ReturnLinefeed);																			// (9DP v1.01.12)
					return TRUE;																											// (9DP v1.01.12)
				}
			}
			return FALSE;
		}
	
		//* Multipurpose Memory Routines
		sprintf(tier_command,"MEM\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			message = Parse_Message(NULL);
			if(message != NULL)
			{
				sprintf(tier_command,"CLR\0");
				response = strcmp(message,tier_command);
				if(response == 0)
				{
					Append_Message(tier_command, ColonSeparator);
					Append_Message(MessageOK, ReturnLinefeed);
					SST39ChipErase();
					return TRUE;
				}
			}
			return FALSE;
		}
	
		//* Instrument Reset Handler
		sprintf(tier_command,"RST\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ReturnLinefeed);																					// (9DP v1.01.12)
			Restart_System = TRUE;																											// (9DP v1.01.12)
			return TRUE;																													// (9DP v1.01.12)
		}

		//* System - Get/Set/Reset
		process = Post_Process_Handler(message);
		if(process != COMMAND_ERROR)
		{
			return System_Process(process);
		}
	}

	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	END INSTRUMENT HANDLER
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//*	SYSTEM HANDLER
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL System_Process(BYTE process)
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
//* 	
//*		INS:GET:FIL
//*		INS:GET:HV1
//*		INS:GET:HV2
//*		INS:GET:BAT
//*		INS:GET:TEM
//*		INS:SET:FIL
//*		INS:RES:FIL
//*		INS:RES:CTR
//*

BOOL System_Process(BYTE process)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response;
	BYTE value;
	UINT16 result;
	DWORD address;
	BYTE char_buffer[25] = {0};
	XCHAR xchar_buffer[25] = {0};

	message = Parse_Message(NULL);
	if(message != NULL)
	{
#ifdef INSTRUMENT_9D																														// (3D v1.00.04)
		//* Filter
		sprintf(tier_command,"FIL\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
				{
					uR_filter = atof(message);
					lmi.calibration.config.info.uR_filter = uR_filter;
				}
				else
				{
					return FALSE;
				}
			}
			else if(process == COMMAND_RESET)
			{
				uR_filter = DEFAULT_CALIBRATION_CONFIG_uR_FILTER;
				lmi.calibration.config.info.uR_filter = uR_filter;
			}
			if(process != COMMAND_READ)
			{
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_CALIBRATION_CONFIG_LOCATION, 2, lmi.calibration.config.buffer, SIZE_CALIBRATION_CONFIG);
			}
			sprintf(tier_command,"%.1f\r\n\0", uR_filter);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
#endif

		//* High Voltage Reading Handler
		sprintf(tier_command,"HV1\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_READ)
			{
				Check_High_Voltage(1);
				sprintf(tier_command,"%.1f\r\n\0",High_Voltage_Reading);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
			return FALSE;
		}
	
//		//* High Voltage Reading Handler
//		sprintf(tier_command,"HV2\0");
//		response = strcmp(message,tier_command);
//		if(response == 0)
//		{
//			Append_Message(tier_command, ColonSeparator);
//			if(process == COMMAND_READ)
//			{
//				Check_High_Voltage(2);
//				sprintf(tier_command,"%.1f\r\n\0",High_Voltage_Reading);
//				strcat((char *)USB_TXMessage, tier_command);
//				return TRUE;
//			}
//			return FALSE;
//		}
	
		//* Battery Reading Handler
		sprintf(tier_command,"BAT\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_READ)
			{
				sprintf(tier_command,"%.2f\r\n\0",Battery_Reading);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
			return FALSE;
		}
	
#ifdef INSTRUMENT_9D																														// (3D v1.00.04)
		//* Temperature Reading Handler
		sprintf(tier_command,"TEM\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_READ)
			{
				Check_Temperature();
				sprintf(tier_command,"%.1f\r\n\0",Temperature_Reading);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
			return FALSE;
		}
#endif

		//* SST Memory Read
		sprintf(tier_command,"MEM\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			message = Parse_Message(NULL);
			if(message != NULL)
			{
				address = atol(message);
				sprintf(tier_command,"%08ld:\0", address);
				strcat((char *)USB_TXMessage, tier_command);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						result = atoi(message);
						SST39WriteWord(address, result);
						DelayMs(1);
						result = SST39ReadWord(address);
						sprintf(tier_command,"%05d\r\n\0", result);
						strcat((char *)USB_TXMessage, tier_command);
						return TRUE;
					}
				}
				else if(process == COMMAND_READ)
				{
					result = SST39ReadWord(address);
					sprintf(tier_command,"%05d\r\n\0", result);
					strcat((char *)USB_TXMessage, tier_command);
					return TRUE;
				}
			}
			return FALSE;
		}

		//* Calibration Controls
		sprintf(tier_command,"CTR\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_RESET)
			{
				lmi.calibration.controls.info.fixed_range = DEFAULT_CALIBRATION_CONTROLS_RANGE;
				lmi.calibration.controls.info.stream_data_reading = DEFAULT_CALIBRATION_CONTROLS_STREAM;
				lmi.calibration.controls.info.stream_report_time = DEFAULT_CALIBRATION_CONTROLS_STREAM_TIME;
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_CALIBRATION_CONTROLS_LOCATION, 2, (BYTE *)lmi.calibration.controls.buffer, SIZE_CALIBRATION_CONTROLS);
				Append_Message(MessageOK, ReturnLinefeed);
				return TRUE;
			}
			return FALSE;
		}

//		//* Radiation Alert Value Handler
//		sprintf(tier_command,"RAE\0");
//		response = strcmp(message,tier_command);
//		if(response == 0)
//		{
//			Append_Message(tier_command, ColonSeparator);
//			if(process == COMMAND_WRITE)
//			{
//				sprintf(tier_command,"%.1f\r\n\0",Radiation_Alert_Rate);
//				strcat((char *)USB_TXMessage, tier_command);
//				return TRUE;
//			}
//		}
//
//		//* Radiation Alarm Value Handler
//		sprintf(tier_command,"RAL\0");
//		response = strcmp(message,tier_command);
//		if(response == 0)
//		{
//			Append_Message(tier_command, ColonSeparator);
//			if(process == COMMAND_WRITE)
//			{
//				sprintf(tier_command,"%.1f\r\n\0",Radiation_Alarm_Rate);
//				strcat((char *)USB_TXMessage, tier_command);
//				return TRUE;
//			}
//		}
//
//		//* Integrate Alert Value Handler
//		sprintf(tier_command,"IAE\0");
//		response = strcmp(message,tier_command);
//		if(response == 0)
//		{
//			Append_Message(tier_command, ColonSeparator);
//			if(process == COMMAND_WRITE)
//			{
//				sprintf(tier_command,"%.1f\r\n\0",Integrate_Alert_Rate);
//				strcat((char *)USB_TXMessage, tier_command);
//				return TRUE;
//			}
//		}
//
//		//* Integrate Alarm Value Handler
//		sprintf(tier_command,"IAL\0");
//		response = strcmp(message,tier_command);
//		if(response == 0)
//		{
//			Append_Message(tier_command, ColonSeparator);
//			if(process == COMMAND_WRITE)
//			{
//				sprintf(tier_command,"%.1f\r\n\0",Integrate_Alarm_Rate);
//				strcat((char *)USB_TXMessage, tier_command);
//				return TRUE;
//			}
//		}
//
//		//* Dummy Handler
//		sprintf(tier_command,"XXX\0");
//		response = strcmp(message,tier_command);
//		if(response == 0)
//		{
//			Append_Message(tier_command, ColonSeparator);
//			if(process == COMMAND_READ)
//			{
//				sprintf(tier_command,"%ld\r\n\0",TOTAL_DETECTOR_ARRAY);
//				strcat((char *)USB_TXMessage, tier_command);
//				return TRUE;
//			}
//			return FALSE;
//		}
	}	

	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	END SYSTEM HANDLER
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//*	SETTINGS HANDLERS
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Settings_Handler(void)
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
//* 	
//*		INS:STG:PRD:
//*		INS:STG:LAN:
//*		INS:STG:CLN:
//*		INS:STG:CLO:
//*		INS:STG:PWD:
//*		INS:STG:MIS:
//*

BOOL Settings_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response;
	BYTE process;
	BOOL status;

	message = Parse_Message(NULL);
	if(message != NULL)
	{
		//* Settings - Product
		sprintf(tier_command,"PRD\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Settings_Product_Handler();
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_PRODUCT_LOCATION, 2, lmi.settings.product.buffer, SIZE_PRODUCT);
			}
			return status;
		}
	
		//* Settings - Language
		sprintf(tier_command,"LAN\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Settings_Language_Handler();
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_LANGUAGE_LOCATION, 2, lmi.settings.language.buffer, SIZE_LANGUAGE);
			}
			return status;
		}
	
		//* Settings - Calendar
		sprintf(tier_command,"CLN\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Settings_Calendar_Handler();
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_CALENDAR_LOCATION, 2, lmi.settings.calendar.buffer, SIZE_DATE);
			}
			return status;
		}
	
		//* Settings - Clock
		sprintf(tier_command,"CLO\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Settings_Clock_Handler();
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_CLOCK_LOCATION, 2, lmi.settings.clock.buffer, SIZE_TIME);
			}
			return status;
		}
	
		//* Settings - Passwords
		sprintf(tier_command,"PWD\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Settings_Passwords_Handler();
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_PASSWORDS_LOCATION, 2, lmi.settings.passwords.buffer, SIZE_PASSWORDS);
			}
			return status;
		}
	
		//* Settings - Firmware
		sprintf(tier_command,"FIR\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Settings_Firmware_Handler();
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_SETTINGS_MISC_LOCATION, 2, lmi.settings.misc.buffer, SIZE_SETTINGS_MISC);
			}
			return status;
		}
	}
	
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Settings_Product_Handler(void)
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
//* 	
//*		INS:STG:PRD:CAD:GET:MON
//*		INS:STG:PRD:CAD:GET:DAY
//*		INS:STG:PRD:CAD:GET:YEA
//*		INS:STG:PRD:CDD:GET:MON
//*		INS:STG:PRD:CDD:GET:DAY
//*		INS:STG:PRD:CDD:GET:YEA
//*		INS:STG:PRD:CAD:SET:MON
//*		INS:STG:PRD:CAD:SET:DAY
//*		INS:STG:PRD:CAD:SET:YEA
//*		INS:STG:PRD:CDD:SET:MON
//*		INS:STG:PRD:CDD:SET:DAY
//*		INS:STG:PRD:CDD:SET:YEA
//*		INS:STG:PRD:CAD:RES:MON
//*		INS:STG:PRD:CAD:RES:DAY
//*		INS:STG:PRD:CAD:RES:YEA
//*		INS:STG:PRD:CDD:RES:MON
//*		INS:STG:PRD:CDD:RES:DAY
//*		INS:STG:PRD:CDD:RES:YEA
//*		INS:STG:PRD:GET:
//*		INS:STG:PRD:SET:
//*
//*

BOOL Settings_Product_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;

	message = Parse_Message(NULL);
	if(message != NULL)
	{
		//* Settings - Product - Calibration Date
		sprintf(tier_command,"CAD\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			process = Process_Handler();
			if(process != COMMAND_ERROR)
//				return Date_Handler(process, &lmi.settings.product.info.calibration_date.month);
				return Date_Handler(process, &lmi.settings.product.info.calibration_date);			// (9DP v1.01.21)
			else
				return FALSE;
		}
	
		//* Settings - Product - Calibration Due Date
		sprintf(tier_command,"CDD\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			process = Process_Handler();
			if(process != COMMAND_ERROR)
//				return Date_Handler(process, &lmi.settings.product.info.calibration_due_date.month);
				return Date_Handler(process, &lmi.settings.product.info.calibration_due_date);		// (9DP v1.01.21)
			else
				return FALSE;
		}

		//* Settings - Product - Get/Set/Reset
		process = Post_Process_Handler(message);
		if(process != COMMAND_ERROR)
		{
			return Settings_Product_Process(process);
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Settings_Product_Process(BYTE process)
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
//* 	
//*		INS:STG:PRD:GET:MOD
//*		INS:STG:PRD:GET:LSN
//*		INS:STG:PRD:GET:UID
//*		INS:STG:PRD:GET:USN
//*		INS:STG:PRD:SET:MOD
//*		INS:STG:PRD:SET:LSN
//*		INS:STG:PRD:SET:UID
//*		INS:STG:PRD:SET:USN
//*		INS:STG:PRD:RES:MOD
//*		INS:STG:PRD:RES:LSN
//*		INS:STG:PRD:RES:UID
//*		INS:STG:PRD:RES:USN
//*		INS:STG:PRD:RES:CCD
//*

BOOL Settings_Product_Process(BYTE process)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;

	message = Parse_Message(NULL);
	if(message != NULL)
	{
		//* Settings - Product - Model Number
		sprintf(tier_command,"MOD\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);																								// (3D v1.00.04)
				if(!Settings_Product_Model_Number(message))																					// (3D v1.00.04)
//				if(!Settings_Product_Model_Number())
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				strncpy(lmi.settings.product.info.model, DEFAULT_MODEL, LENGTH_MODEL);
				if(!Settings_Product_Model_Number((char *)lmi.settings.product.info.model))													// (3D v1.00.04)
					return FALSE;																											// (3D v1.00.04)
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			Format_String(tier_command, lmi.settings.product.info.model, LENGTH_MODEL);														// (9DP v1.01.04)
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
	
		//* Settings - Product - Serial Number
		sprintf(tier_command,"LSN\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
					strncpy(lmi.settings.product.info.serial_number, message, LENGTH_SERIAL_NUMBER);
				else
					strncpy(lmi.settings.product.info.serial_number, DEFAULT_SERIAL_NUMBER, LENGTH_SERIAL_NUMBER);							// (9DP v1.01.01)
			}
			else if(process == COMMAND_RESET)
			{
				strncpy(lmi.settings.product.info.serial_number, DEFAULT_SERIAL_NUMBER, LENGTH_SERIAL_NUMBER);
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			Format_String(tier_command, lmi.settings.product.info.serial_number, LENGTH_SERIAL_NUMBER);										// (9DP v1.01.04)
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
	
		//* Settings - Product - User ID Number
		sprintf(tier_command,"UID\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
					strncpy(lmi.settings.product.info.user_id_number, message, LENGTH_USER_ID_NUMBER);
				else
					strncpy(lmi.settings.product.info.user_id_number, DEFAULT_USER_ID_NUMBER, LENGTH_USER_ID_NUMBER);						// (9DP v1.01.01)
			}
			else if(process == COMMAND_RESET)
			{
				strncpy(lmi.settings.product.info.user_id_number, DEFAULT_USER_ID_NUMBER, LENGTH_USER_ID_NUMBER);
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			Format_String(tier_command, lmi.settings.product.info.user_id_number, LENGTH_USER_ID_NUMBER);									// (9DP v1.01.04)
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
	
		//* Settings - Product - User Serial Number
		sprintf(tier_command,"USN\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
					strncpy(lmi.settings.product.info.user_serial_number, message, LENGTH_USER_SERIAL_NUMBER);
				else
					strncpy(lmi.settings.product.info.user_serial_number, DEFAULT_USER_SERIAL_NUMBER, LENGTH_USER_SERIAL_NUMBER);			// (9DP v1.01.01)
			}
			else if(process == COMMAND_RESET)
			{
				strncpy(lmi.settings.product.info.user_serial_number, DEFAULT_USER_SERIAL_NUMBER, LENGTH_USER_SERIAL_NUMBER);
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			Format_String(tier_command, lmi.settings.product.info.user_serial_number, LENGTH_USER_SERIAL_NUMBER);							// (9DP v1.01.04)
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
	
		//* Settings - Product - Clear Calibration Date
		sprintf(tier_command,"CCD\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_RESET)
			{
				lmi.settings.product.info.calibration_date.month = DEFAULT_DATE_MONTH;
				lmi.settings.product.info.calibration_date.day = DEFAULT_DATE_DAY;
				lmi.settings.product.info.calibration_date.year = DEFAULT_DATE_YEAR;
				lmi.settings.product.info.calibration_due_date.month = DEFAULT_DATE_MONTH;
				lmi.settings.product.info.calibration_due_date.day = DEFAULT_DATE_DAY;
				lmi.settings.product.info.calibration_due_date.year = DEFAULT_DATE_YEAR;
				Append_Message(MessageOK, ReturnLinefeed);
				return TRUE;
			}
		}
	}

	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Settings_Product_Model_Number(void)
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
//* 	
//*		INS:STG:PRD:SET:MOD:9DP
//*		INS:STG:PRD:SET:MOD:9DP+
//*		INS:STG:PRD:SET:MOD:9DP-1
//*		INS:STG:PRD:SET:MOD:9DP-1+
//*		INS:STG:PRD:SET:MOD:3D
//*		INS:STG:PRD:SET:MOD:3D+
//*		INS:STG:PRD:SET:MOD:276D
//*		INS:STG:PRD:SET:MOD:V1
//*

BOOL Settings_Product_Model_Number(char *message)
//BOOL Settings_Product_Model_Number(void)
{
	char tier_command[MESSAGE_SIZE];
//	char *message;
	BYTE response = 0;
	BYTE index;
	BYTE new_model_number;

//	message = Parse_Message(NULL);
	if(message != NULL)
	{
		sprintf(tier_command,"9DP\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			new_model_number = (BYTE) MODEL_9DP;
			return(Change_Model_Number(new_model_number, message));
		}
	
		sprintf(tier_command,"9DP+\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			new_model_number = (BYTE) MODEL_9DP_PLUS;
			return(Change_Model_Number(new_model_number, message));
		}

		sprintf(tier_command,"9DP-1\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			new_model_number = (BYTE) MODEL_9DP_1;
			return(Change_Model_Number(new_model_number, message));
		}

		sprintf(tier_command,"9DP-1+\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			new_model_number = (BYTE) MODEL_9DP_1_PLUS;
			return(Change_Model_Number(new_model_number, message));
		}

		sprintf(tier_command,"9DLP\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			new_model_number = (BYTE) MODEL_9DLP;
			return(Change_Model_Number(new_model_number, message));
		}

		sprintf(tier_command,"9DLP+\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			new_model_number = (BYTE) MODEL_9DLP_PLUS;
			return(Change_Model_Number(new_model_number, message));
		}

		sprintf(tier_command,"3D\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			new_model_number = (BYTE) MODEL_3D;
			return(Change_Model_Number(new_model_number, message));
		}

		sprintf(tier_command,"3D+\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			new_model_number = (BYTE) MODEL_3D_PLUS;
			return(Change_Model_Number(new_model_number, message));
		}

		sprintf(tier_command,"276D\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			new_model_number = (BYTE) MODEL_276D;
			return(Change_Model_Number(new_model_number, message));
		}
	
		sprintf(tier_command,"v1\0");																// handle model number for RTI (9DP v1.01.21)
		response = strcmp(message,tier_command);													// handle model number for RTI (9DP v1.01.21)
		if(response == 0)																			// handle model number for RTI (9DP v1.01.21)
		{																							// handle model number for RTI (9DP v1.01.21)
			new_model_number = (BYTE) MODEL_V1;														// handle model number for RTI (9DP v1.01.21)
			return(Change_Model_Number(new_model_number, message));									// handle model number for RTI (9DP v1.01.21)
		}																							// handle model number for RTI (9DP v1.01.21)
	
		sprintf(tier_command,"9DP*\0");																// handle model number for 9DP* (9DP v1.01.24)
		response = strcmp(message,tier_command);													// handle model number for 9DP* (9DP v1.01.24)
		if(response == 0)																			// handle model number for 9DP* (9DP v1.01.24)
		{																							// handle model number for 9DP* (9DP v1.01.24)
			new_model_number = (BYTE) MODEL_9DP_STAR;												// handle model number for 9DP* (9DP v1.01.24)
			return(Change_Model_Number(new_model_number, message));									// handle model number for 9DP* (9DP v1.01.24)
		}																							// handle model number for 9DP* (9DP v1.01.24)

//		sprintf(tier_command,"9DP*+\0");															// handle model number for 9DP* (9DP v1.01.24)
//		response = strcmp(message,tier_command);													// handle model number for 9DP* (9DP v1.01.24)
//		if(response == 0)																			// handle model number for 9DP* (9DP v1.01.24)
//		{																							// handle model number for 9DP* (9DP v1.01.24)
//			new_model_number = (BYTE) MODEL_9DP_STAR_PLUS;											// handle model number for 9DP* (9DP v1.01.24)
//			return(Change_Model_Number(new_model_number, message));									// handle model number for 9DP* (9DP v1.01.24)
//		}																							// handle model number for 9DP* (9DP v1.01.24)

		sprintf(tier_command,"9DP-2\0");															// handle model number for 9DP-2 (9DP v1.01.24)
		response = strcmp(message,tier_command);													// handle model number for 9DP-2 (9DP v1.01.24)
		if(response == 0)																			// handle model number for 9DP-2 (9DP v1.01.24)
		{																							// handle model number for 9DP-2 (9DP v1.01.24)
			new_model_number = (BYTE) MODEL_9DP_2;													// handle model number for 9DP-2 (9DP v1.01.24)
			return(Change_Model_Number(new_model_number, message));									// handle model number for 9DP-2 (9DP v1.01.24)
		}																							// handle model number for 9DP-2 (9DP v1.01.24)
	
//		sprintf(tier_command,"9DP-2+\0");															// handle model number for 9DP-2+ (9DP v1.01.24)
//		response = strcmp(message,tier_command);													// handle model number for 9DP-2+ (9DP v1.01.24)
//		if(response == 0)																			// handle model number for 9DP-2+ (9DP v1.01.24)
//		{																							// handle model number for 9DP-2+ (9DP v1.01.24)
//			new_model_number = (BYTE) MODEL_9DP_2_PLUS;												// handle model number for 9DP-2+ (9DP v1.01.24)
//			return(Change_Model_Number(new_model_number, message));									// handle model number for 9DP-2+ (9DP v1.01.24)
//		}																							// handle model number for 9DP-2+ (9DP v1.01.24)
	}

	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Settings_Language_Handler(void)
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
//*
//*		INS:STG:LAN:GET:LSE:
//*		INS:STG:LAN:SET:LSE:
//*		INS:STG:LAN:RES:LSE:
//*

BOOL Settings_Language_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE value;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{
			//* Settings - Language - Index
			sprintf(tier_command,"LSE\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = (BYTE) atoi(message);																								// (9DP v1.01.13)
						if(value == 0)
						{
							lmi.settings.language.info.index = value;
						}
						else
							return FALSE;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.settings.language.info.index = DEFAULT_LANGUAGE_INDEX;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0",lmi.settings.language.info.index);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}

			//* Settings - Language - Name
			sprintf(tier_command,"NAM\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						strncpy(lmi.settings.language.info.string, message, LENGTH_LANGUAGE_STRING);
					}
					else
					{
						strncpy(lmi.settings.language.info.string, "\0", LENGTH_LANGUAGE_STRING);
					}
				}
				else if(process == COMMAND_RESET)
				{
					strncpy(lmi.settings.language.info.string, DEFAULT_LANGUAGE_NAME, LENGTH_LANGUAGE_STRING);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				Format_String(tier_command, lmi.settings.language.info.string, LENGTH_LANGUAGE_STRING);										// (9DP v1.01.04)
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Settings_Calendar_Handler(void)
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
//* 	
//*		INS:STG:CLN:GET:MON
//*		INS:STG:CLN:GET:DAY
//*		INS:STG:CLN:GET:YEA
//*		INS:STG:CLN:SET:MON
//*		INS:STG:CLN:SET:DAY
//*		INS:STG:CLN:SET:YEA
//*		INS:STG:CLN:RES:MON
//*		INS:STG:CLN:RES:DAY
//*		INS:STG:CLN:RES:YEA
//*

BOOL Settings_Calendar_Handler(void)
{
	TYPE_DATE_FORMAT calendarData;
	BYTE process = 0;
	unsigned int int_status;
	BOOL status;

	//* Settings - Calendar - Date
	process = Process_Handler();
	if(process == COMMAND_ERROR)
	{
		status = FALSE;
	}
	else if(process == COMMAND_READ)
	{
//		status = Date_Handler(process, &lmi.settings.calendar.info.date.month);
		status = Date_Handler(process, &lmi.settings.calendar.info.date);							// (9DP v1.01.21)
	}
	else
	{
		status = Date_Handler(process, &calendarData);
		if(status == TRUE)
		{
			switch(DateEvent)
			{
				case MONTH_EVENT:
				{
					do
					{
						int_status = INTDisableInterrupts();
						i2c2Write(i2cADDR_PIC_RTCC, i2cPIC_RTC_MONTH, 1, calendarData.month, 1);					// send the month to the PIC18
						INTRestoreInterrupts(int_status);
						DelayMs(25);
						RTCCUpdate();
					} while(calendarData.month != lmi.settings.calendar.info.date.month);
					break;
				}
				case DAY_EVENT:
				{
					do
					{
						int_status = INTDisableInterrupts();
						i2c2Write(i2cADDR_PIC_RTCC, i2cPIC_RTC_DAY, 1, calendarData.day, 1);						// send the day to the PIC18
						INTRestoreInterrupts(int_status);
						DelayMs(25);
						RTCCUpdate();
					} while(calendarData.day != lmi.settings.calendar.info.date.day);
					break;
				}
				case YEAR_EVENT:
				{
					do
					{
						int_status = INTDisableInterrupts();
						i2c2Write(i2cADDR_PIC_RTCC, i2cPIC_RTC_YEAR, 1, calendarData.year, 1);						// send the year to the PIC18
						INTRestoreInterrupts(int_status);
						DelayMs(25);
						RTCCUpdate();
					} while(calendarData.year != lmi.settings.calendar.info.date.year);
					break;
				}
			}
			DateEvent = NO_DATE_EVENT;
		}
	}		
	return status;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Settings_Clock_Handler(void)
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
//*
//*		INS:STG:CLO:GET:HRS
//*		INS:STG:CLO:GET:MIN
//*		INS:STG:CLO:GET:SEC
//*		INS:STG:CLO:GET:FOR
//*		INS:STG:CLO:SET:HRS
//*		INS:STG:CLO:SET:MIN
//*		INS:STG:CLO:SET:SEC
//*		INS:STG:CLO:SET:FOR
//*

BOOL Settings_Clock_Handler(void)
{
	TYPE_TIME_FORMAT clockData;
	BYTE process = 0;
	BYTE response;
	unsigned int int_status;
	BOOL status;

	//* Settings - Calendar - Date
	process = Process_Handler();
	if(process == COMMAND_ERROR)
	{
		status = FALSE;
	}
	else if(process == COMMAND_READ)
	{
//		status = Time_Handler(process, &lmi.settings.clock.info.time.hours);
		status = Time_Handler(process, &lmi.settings.clock.info.time);								// (9DP v1.01.21)
	}
	else
	{
		status = Time_Handler(process, &clockData);
		if(status == TRUE)
		{
			switch(TimeEvent)
			{
				case HOUR_EVENT:
				{
					do
					{
						int_status = INTDisableInterrupts();
						i2c2Write(i2cADDR_PIC_RTCC, i2cPIC_RTC_24HOUR, 1, clockData.hours, 1);						// write the hours to the PIC18
						INTRestoreInterrupts(int_status);
						DelayMs(25);
						RTCCUpdate();
					} while(clockData.hours != lmi.settings.clock.info.time.hours);
					break;
				}
				case MINUTE_EVENT:
				{
					do
					{
						int_status = INTDisableInterrupts();
						i2c2Write(i2cADDR_PIC_RTCC, i2cPIC_RTC_MINUTE, 1, clockData.minutes, 1);					// write the minutes to the PIC18
						INTRestoreInterrupts(int_status);
						DelayMs(25);
						RTCCUpdate();
					} while(clockData.minutes != lmi.settings.clock.info.time.minutes);
					break;
				}
				case SECOND_EVENT:
				{
					do
					{
						int_status = INTDisableInterrupts();
						i2c2Write(i2cADDR_PIC_RTCC, i2cPIC_RTC_SECOND, 1, clockData.seconds, 1);					// write the seconds to the PIC18
						INTRestoreInterrupts(int_status);
						DelayMs(25);
						RTCCUpdate();
					} while(clockData.seconds != lmi.settings.clock.info.time.seconds);
					break;
				}
				case FORMAT_EVENT:
				{
					lmi.settings.clock.info.time.format = clockData.format;
					break;
				}
			}
			TimeEvent = NO_TIME_EVENT;
		}
	}
	return status;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Settings_Passwords_Handler(void)
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
//*
//*		INS:STG:PWD:GET:ENB
//*		INS:STG:PWD:GET:PW1
//*		INS:STG:PWD:GET:PW2
//*		INS:STG:PWD:GET:PW3
//*		INS:STG:PWD:SET:ENB
//*		INS:STG:PWD:SET:PW1
//*		INS:STG:PWD:SET:PW2
//*		INS:STG:PWD:SET:PW3
//*		INS:STG:PWD:SET:RDE
//*

BOOL Settings_Passwords_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	int enable;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{
			//* Settings - Passwords - Enable
			sprintf(tier_command,"ENB\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						enable = (BYTE) atoi(message);
						if(enable < 2)
							lmi.settings.passwords.info.enable = (BYTE) enable;
						else
							return FALSE;
					}
					else
					{
						return FALSE;
					}
				}
				else if(process == COMMAND_RESET)
				{
					lmi.settings.passwords.info.enable = DEFAULT_PASSWORD_ENABLE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0",lmi.settings.passwords.info.enable);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Settings - Passwords - Password 1
			sprintf(tier_command,"PW1\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
						strncpy(lmi.settings.passwords.info.password_1, message, LENGTH_PASSWORD);
					else
						strncpy(lmi.settings.passwords.info.password_1, DEFAULT_PASSWORD_1, LENGTH_PASSWORD);
				}
				else if(process == COMMAND_RESET)
				{
					strncpy(lmi.settings.passwords.info.password_1, DEFAULT_PASSWORD_1, LENGTH_PASSWORD);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				Format_String(tier_command, lmi.settings.passwords.info.password_1, LENGTH_PASSWORD);										// (9DP v1.01.04)
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Settings - Passwords - Password 2
			sprintf(tier_command,"PW2\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
						strncpy(lmi.settings.passwords.info.password_2, message, LENGTH_PASSWORD);
					else
						strncpy(lmi.settings.passwords.info.password_2, DEFAULT_PASSWORD_2, LENGTH_PASSWORD);
				}
				else if(process == COMMAND_RESET)
				{
					strncpy(lmi.settings.passwords.info.password_2, DEFAULT_PASSWORD_2, LENGTH_PASSWORD);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				Format_String(tier_command, lmi.settings.passwords.info.password_2, LENGTH_PASSWORD);										// (9DP v1.01.04)
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Settings - Passwords - Password 3
			sprintf(tier_command,"PW3\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
						strncpy(lmi.settings.passwords.info.password_3, message, LENGTH_PASSWORD);
					else
						strncpy(lmi.settings.passwords.info.password_3, DEFAULT_PASSWORD_3, LENGTH_PASSWORD);
				}
				else if(process == COMMAND_RESET)
				{
					strncpy(lmi.settings.passwords.info.password_3, DEFAULT_PASSWORD_3, LENGTH_PASSWORD);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				Format_String(tier_command, lmi.settings.passwords.info.password_3, LENGTH_PASSWORD);										// (9DP v1.01.04)
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Settings - Passwords - Restore Defaults
			sprintf(tier_command,"RDE\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_RESET)
				{
					strncpy(lmi.settings.passwords.info.password_1, DEFAULT_PASSWORD_1, LENGTH_PASSWORD);
					strncpy(lmi.settings.passwords.info.password_2, DEFAULT_PASSWORD_2, LENGTH_PASSWORD);
					strncpy(lmi.settings.passwords.info.password_3, DEFAULT_PASSWORD_3, LENGTH_PASSWORD);
					Append_Message(MessageOK, ReturnLinefeed);
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Settings_Firmware_Handler(void)
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
//*
//*		INS:STG:FIR:GET:NUM
//*

#define DEFAULT_FIRMWARE_VALUE						""

BOOL Settings_Firmware_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{
			//* Settings - Firmware - Number
			sprintf(tier_command,"NUM\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_READ)
				{
					sprintf((char *)tier_command,"%s\r\n\0",lmi.settings.misc.info.firmware);
					strcat((char *)USB_TXMessage, tier_command);
					Write_EEPROM = FALSE;
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	END SETTINGS HANDLERS
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//*	FEATURES HANDLERS
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Features_Handler(void)
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
//*
//*		INS:FEA:SCH:
//*		INS:FEA:BCH:
//*		INS:FEA:INT:
//*		INS:FEA:DLG:
//*

BOOL Features_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response;
	BYTE process;
	BOOL status;

	message = Parse_Message(NULL);
	if(message != NULL)
	{
		//* Features - Source Check
		sprintf(tier_command,"SCH\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Features_Source_Handler();
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_SOURCE_CHECK_LOCATION, 2, lmi.features.source.buffer, SIZE_SOURCE_CHECK);
			}
			return status;
		}
	
		//* Features - Background Check
		sprintf(tier_command,"BCH\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Features_Background_Handler();
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_BACKGROUND_CHECK_LOCATION, 2, lmi.features.background.buffer, SIZE_BACKGROUND_CHECK);
			}
			return status;
		}
	
		//* Features - Integrate
		sprintf(tier_command,"INT\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Features_Integrate_Handler();
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_INTEGRATE_LOCATION, 2, lmi.features.integrate.buffer, SIZE_INTEGRATE);
			}
			return status;
		}
	
		//* Features - Data Logging
		sprintf(tier_command,"DLG\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Features_Data_Logging_Handler();
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_DATA_LOGGING_LOCATION, 2, lmi.features.data_logging.buffer, SIZE_DATA_LOGGING);
			}
			return status;
		}
	}

	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Features_Source_Handler(void)
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
//* 	
//*		INS:FEA:SCH:GET:
//*		INS:FEA:SCH:SET:
//*		INS:FEA:SCH:IRD:GET:MON
//*		INS:FEA:SCH:IRD:GET:DAY
//*		INS:FEA:SCH:IRD:GET:YEA
//*		INS:FEA:SCH:IRT:GET:HRS
//*		INS:FEA:SCH:IRT:GET:MIN
//*		INS:FEA:SCH:IRT:GET:FOR
//*		INS:FEA:SCH:IRR:GET:VAL
//*		INS:FEA:SCH:IRR:GET:UNI
//*		INS:FEA:SCH:IRR:GET:MUL
//*		INS:FEA:SCH:LRD:GET:MON
//*		INS:FEA:SCH:LRD:GET:DAY
//*		INS:FEA:SCH:LRD:GET:YEA
//*		INS:FEA:SCH:LRT:GET:HRS
//*		INS:FEA:SCH:LRT:GET:MIN
//*		INS:FEA:SCH:LRT:GET:FOR
//*		INS:FEA:SCH:LRR:GET:VAL
//*		INS:FEA:SCH:LRR:GET:UNI
//*		INS:FEA:SCH:LRR:GET:MUL
//*

BOOL Features_Source_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;

	message = Parse_Message(NULL);
	if(message != NULL)
	{
		//* Features - Source - Initial Reading
		sprintf(tier_command,"IRG\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			process = Process_Handler();
			if(process == COMMAND_READ)
				return Reading_Handler(process, &lmi.features.source.info.initial_reading.value, &lmi.features.source.info.initial_reading.units, &lmi.features.source.info.initial_reading.multiplier);
			else
				return FALSE;
		}
	
		//* Features - Source - Initial Date
		sprintf(tier_command,"IRD\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			process = Process_Handler();
			if(process == COMMAND_READ)
//				return Date_Handler(process, &lmi.features.source.info.initial_date.month);
				return Date_Handler(process, &lmi.features.source.info.initial_date);				// (9DP v1.01.21)
			else
				return FALSE;
		}
	
		//* Features - Source - Initial Time
		sprintf(tier_command,"IRT\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			process = Process_Handler();
			if(process == COMMAND_READ)
//				return Time_Handler(process, &lmi.features.source.info.initial_time.hours);
				return Time_Handler(process, &lmi.features.source.info.initial_time);				// (9DP v1.01.21)
			else
				return FALSE;
		}
	
		//* Features - Source - Last Reading
		sprintf(tier_command,"LRG\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			process = Process_Handler();
			if(process == COMMAND_READ)
				return Reading_Handler(process, &lmi.features.source.info.last_reading.value, &lmi.features.source.info.last_reading.units, &lmi.features.source.info.last_reading.multiplier);
			else
				return FALSE;
		}
	
		//* Features - Source - Last Date
		sprintf(tier_command,"LRD\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			process = Process_Handler();
			if(process == COMMAND_READ)
//				return Date_Handler(process, &lmi.features.source.info.last_date.month);
				return Date_Handler(process, &lmi.features.source.info.last_date);					// (9DP v1.01.21)
			else
				return FALSE;
		}
	
		//* Features - Source - Last Time
		sprintf(tier_command,"LRT\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			process = Process_Handler();
			if(process == COMMAND_READ)
//				return Time_Handler(process, &lmi.features.source.info.last_time.hours);
				return Time_Handler(process, &lmi.features.source.info.last_time);					// (9DP v1.01.21)
			else
				return FALSE;
	
		}
	
		//* Features - Source - Get/Set/Reset
		process = Post_Process_Handler(message);
		if(process != COMMAND_ERROR)
		{
			return Features_Source_Process(process);
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Features_Source_Process(BYTE process)
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
//* 	
//*		INS:FEA:SCH:GET:PUM
//*		INS:FEA:SCH:GET:OPM
//*		INS:FEA:SCH:GET:DLY
//*		INS:FEA:SCH:GET:LSC
//*		INS:FEA:SCH:GET:STI
//*		INS:FEA:SCH:SET:PUM
//*		INS:FEA:SCH:SET:OPM
//*		INS:FEA:SCH:SET:DLY
//*		INS:FEA:SCH:SET:LSC
//*		INS:FEA:SCH:SET:STI
//*		INS:FEA:SCH:SET:SSC
//*		INS:FEA:SCH:SET:STC
//*		INS:FEA:SCH:SET:CIR
//*		INS:FEA:SCH:SET:CLR
//*

BOOL Features_Source_Process(BYTE process)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;

	message = Parse_Message(NULL);
	if(message != NULL)
	{
		//* Features - Source - Start Check
		sprintf(tier_command,"SSC\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_WRITE)
			{
				// Set flag to start source check
				strcat((char *)USB_TXMessage, ReturnLinefeed);
				return TRUE;
			}
			return FALSE;
		}
	
		//* Features - Source - Stop Check
		sprintf(tier_command,"STC\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_WRITE)
			{
				// Set flag to stop source check
				strcat((char *)USB_TXMessage, ReturnLinefeed);
				return TRUE;
			}
			return FALSE;
		}
	
		//* Features - Source - Powerup Mode
		sprintf(tier_command,"PUM\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
					lmi.features.source.info.powerup_mode = (BYTE) atoi(message);
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.source.info.powerup_mode = DEFAULT_SOURCE_POWERUP_MODE;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.source.info.powerup_mode);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
	
		//* Features - Source - Operating Mode
		sprintf(tier_command,"OPM\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
					lmi.features.source.info.operating_mode = (BYTE) atoi(message);
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.source.info.operating_mode = DEFAULT_SOURCE_OPERATING_MODE;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.source.info.operating_mode);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
	
		//* Features - Source - Delay on Power-up
		sprintf(tier_command,"DLY\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
					lmi.features.source.info.delay = (UINT16) atoi(message);
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.source.info.delay = DEFAULT_SOURCE_DELAY;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.source.info.delay);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
	
		//* Features - Source - Log After Source Check
		sprintf(tier_command,"LSC\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
					lmi.features.source.info.log_result = (BYTE) atoi(message);
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.source.info.log_result = DEFAULT_SOURCE_LOG_RESULT;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.source.info.log_result);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
	
		//* Features - Source - Sample Time
		sprintf(tier_command,"STI\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
					lmi.features.source.info.sample_time = (BYTE) atoi(message);
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.source.info.sample_time = DEFAULT_SOURCE_SAMPLE_TIME;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.source.info.sample_time);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
	
		//* Features - Source - Clear Initial Reading
		sprintf(tier_command,"CIR\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_RESET)
			{
				lmi.features.source.info.initial_reading.value = 0.0;
				lmi.features.source.info.initial_reading.units = DEFAULT_SOURCE_UNITS;
				lmi.features.source.info.initial_reading.multiplier = DEFAULT_SOURCE_MULTIPLIER;
				sprintf((char *)tier_command,"01\r\n\0");
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
			return FALSE;
		}
	
		//* Features - Source - Clear Initial Date
		sprintf(tier_command,"CID\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_RESET)
			{
				lmi.features.source.info.initial_date.month = 0;
				lmi.features.source.info.initial_date.day = 0;
				lmi.features.source.info.initial_date.year = 0;
				sprintf((char *)tier_command,"01\r\n\0");
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
			return FALSE;
		}
	
		//* Features - Source - Clear Initial Time
		sprintf(tier_command,"CIT\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_RESET)
			{
				lmi.features.source.info.initial_time.hours = 0;
				lmi.features.source.info.initial_time.minutes = 0;
				lmi.features.source.info.initial_time.seconds = 0;
				lmi.features.source.info.initial_time.format = 0;
				sprintf((char *)tier_command,"01\r\n\0");
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
			return FALSE;
		}
	
		//* Features - Source - Clear Last Reading
		sprintf(tier_command,"CLR\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_RESET)
			{
				lmi.features.source.info.last_reading.value = 0.0;
				lmi.features.source.info.last_reading.units = DEFAULT_SOURCE_UNITS;
				lmi.features.source.info.last_reading.multiplier = DEFAULT_SOURCE_MULTIPLIER;
				sprintf((char *)tier_command,"01\r\n\0");
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
			return FALSE;
		}
	
		//* Features - Source - Clear Last Reading
		sprintf(tier_command,"CLD\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_RESET)
			{
				lmi.features.source.info.last_date.month = 0;
				lmi.features.source.info.last_date.day = 0;
				lmi.features.source.info.last_date.year = 0;
				sprintf((char *)tier_command,"01\r\n\0");
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
			return FALSE;
		}
	
		//* Features - Source - Clear Last Reading
		sprintf(tier_command,"CLT\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_RESET)
			{
				lmi.features.source.info.last_time.hours = 0;
				lmi.features.source.info.last_time.minutes = 0;
				lmi.features.source.info.last_time.seconds = 0;
				lmi.features.source.info.last_time.format = 0;
				sprintf((char *)tier_command,"01\r\n\0");
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Features_Background_Handler(void)
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
//* 	
//*		INS:FEA:BCH:GET:
//*		INS:FEA:BCH:SET:
//*		INS:FEA:BCH:RES:
//*		INS:FEA:BCH:IRR:GET:VAL
//*		INS:FEA:BCH:IRR:GET:UNI
//*		INS:FEA:BCH:IRR:GET:MUL
//*		INS:FEA:BCH:IRD:GET:MON
//*		INS:FEA:BCH:IRD:GET:DAY
//*		INS:FEA:BCH:IRD:GET:YEA
//*		INS:FEA:BCH:IRT:GET:HRS
//*		INS:FEA:BCH:IRT:GET:MIN
//*		INS:FEA:BCH:IRT:GET:FOR
//*		INS:FEA:BCH:LRD:GET:MON
//*		INS:FEA:BCH:LRD:GET:DAY
//*		INS:FEA:BCH:LRD:GET:YEA
//*		INS:FEA:BCH:LRT:GET:HRS
//*		INS:FEA:BCH:LRT:GET:MIN
//*		INS:FEA:BCH:LRT:GET:FOR
//*		INS:FEA:BCH:LRR:GET:VAL
//*		INS:FEA:BCH:LRR:GET:UNI
//*		INS:FEA:BCH:LRR:GET:MUL
//*		INS:FEA:BCH:IRR:SET:VAL
//*		INS:FEA:BCH:IRR:SET:UNI
//*		INS:FEA:BCH:IRR:SET:MUL
//*		INS:FEA:BCH:IRD:SET:MON
//*		INS:FEA:BCH:IRD:SET:DAY
//*		INS:FEA:BCH:IRD:SET:YEA
//*		INS:FEA:BCH:IRT:SET:HRS
//*		INS:FEA:BCH:IRT:SET:MIN
//*		INS:FEA:BCH:IRT:SET:FOR
//*		INS:FEA:BCH:LRD:SET:MON
//*		INS:FEA:BCH:LRD:SET:DAY
//*		INS:FEA:BCH:LRD:SET:YEA
//*		INS:FEA:BCH:LRT:SET:HRS
//*		INS:FEA:BCH:LRT:SET:MIN
//*		INS:FEA:BCH:LRT:SET:FOR
//*		INS:FEA:BCH:LRR:SET:VAL
//*		INS:FEA:BCH:LRR:SET:UNI
//*		INS:FEA:BCH:LRR:SET:MUL
//*		INS:FEA:BCH:IRR:RES:VAL
//*		INS:FEA:BCH:IRR:RES:UNI
//*		INS:FEA:BCH:IRR:RES:MUL
//*		INS:FEA:BCH:IRD:RES:MON
//*		INS:FEA:BCH:IRD:RES:DAY
//*		INS:FEA:BCH:IRD:RES:YEA
//*		INS:FEA:BCH:IRT:RES:HRS
//*		INS:FEA:BCH:IRT:RES:MIN
//*		INS:FEA:BCH:IRT:RES:FOR
//*		INS:FEA:BCH:LRD:RES:MON
//*		INS:FEA:BCH:LRD:RES:DAY
//*		INS:FEA:BCH:LRD:RES:YEA
//*		INS:FEA:BCH:LRT:RES:HRS
//*		INS:FEA:BCH:LRT:RES:MIN
//*		INS:FEA:BCH:LRT:RES:FOR
//*		INS:FEA:BCH:LRR:RES:VAL
//*		INS:FEA:BCH:LRR:RES:UNI
//*		INS:FEA:BCH:LRR:RES:MUL
//*

BOOL Features_Background_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;

	message = Parse_Message(NULL);
	if(message != NULL)
	{
		//* Features - Background - Initial Reading
		sprintf(tier_command,"IRG\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			process = Process_Handler();
			if(process == COMMAND_READ)
				return Reading_Handler(process, &lmi.features.background.info.initial_reading.value, &lmi.features.background.info.initial_reading.units, &lmi.features.background.info.initial_reading.multiplier);
			else
				return FALSE;
		}
	
		//* Features - Background - Initial Date
		sprintf(tier_command,"IRD\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			process = Process_Handler();
			if(process == COMMAND_READ)
//				return Date_Handler(process, &lmi.features.background.info.initial_date.month);
				return Date_Handler(process, &lmi.features.background.info.initial_date);			// (9DP v1.01.21)
			else
				return FALSE;
		}
	
		//* Features - Background - Initial Time
		sprintf(tier_command,"IRT\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			process = Process_Handler();
			if(process == COMMAND_READ)
//				return Time_Handler(process, &lmi.features.background.info.initial_time.hours);
				return Time_Handler(process, &lmi.features.background.info.initial_time);			// (9DP v1.01.21)
			else
				return FALSE;
		}
	
		//* Features - Background - Last Reading
		sprintf(tier_command,"LRG\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			process = Process_Handler();
			if(process == COMMAND_READ)
				return Reading_Handler(process, &lmi.features.background.info.last_reading.value, &lmi.features.background.info.last_reading.units, &lmi.features.background.info.last_reading.multiplier);
			else
				return FALSE;
		}
	
		//* Features - Background - Last Date
		sprintf(tier_command,"LRD\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			process = Process_Handler();
			if(process == COMMAND_READ)
//				return Date_Handler(process, &lmi.features.background.info.last_date.month);
				return Date_Handler(process, &lmi.features.background.info.last_date);				// (9DP v1.01.21)
			else
				return FALSE;
		}
	
		//* Features - Background - Last Time
		sprintf(tier_command,"LRT\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			process = Process_Handler();
			if(process == COMMAND_READ)
//				return Time_Handler(process, &lmi.features.background.info.last_time.hours);
				return Time_Handler(process, &lmi.features.background.info.last_time);				// (9DP v1.01.21)
			else
				return FALSE;
		}
	
		// Features - Background - Get/Set/Reset
		process = Post_Process_Handler(message);
		if(process != COMMAND_ERROR)
		{
			return Features_Background_Process(process);
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Features_Background_Process(BYTE process)
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
//* 	
//*		INS:FEA:BCH:GET:PUM
//*		INS:FEA:BCH:GET:OPM
//*		INS:FEA:BCH:GET:DLY
//*		INS:FEA:BCH:GET:LSC
//*		INS:FEA:BCH:GET:STI
//*		INS:FEA:BCH:GET:CIR
//*		INS:FEA:BCH:GET:CID
//*		INS:FEA:BCH:GET:CIT
//*		INS:FEA:BCH:GET:CLR
//*		INS:FEA:BCH:GET:CLR
//*		INS:FEA:BCH:GET:CLR
//*		INS:FEA:BCH:SET:PUM
//*		INS:FEA:BCH:SET:OPM
//*		INS:FEA:BCH:SET:DLY
//*		INS:FEA:BCH:SET:LBC
//*		INS:FEA:BCH:SET:STI
//*		INS:FEA:BCH:SET:CIR
//*		INS:FEA:BCH:SET:CID
//*		INS:FEA:BCH:SET:CIT
//*		INS:FEA:BCH:SET:CLR
//*		INS:FEA:BCH:SET:CLR
//*		INS:FEA:BCH:SET:CLR
//*		INS:FEA:BCH:RES:PUM
//*		INS:FEA:BCH:RES:OPM
//*		INS:FEA:BCH:RES:DLY
//*		INS:FEA:BCH:RES:LBC
//*		INS:FEA:BCH:RES:STI
//*		INS:FEA:BCH:RES:CIR
//*		INS:FEA:BCH:RES:CID
//*		INS:FEA:BCH:RES:CIT
//*		INS:FEA:BCH:RES:CLR
//*		INS:FEA:BCH:RES:CLR
//*		INS:FEA:BCH:RES:CLR
//*

BOOL Features_Background_Process(BYTE process)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;

	message = Parse_Message(NULL);
	if(message != NULL)
	{
		//* Features - Background - Start Check
		sprintf(tier_command,"SBC\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_WRITE)
			{
				// Set flag to start background check
				strcat((char *)USB_TXMessage, ReturnLinefeed);
				return TRUE;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			return FALSE;
		}
	
		//* Features - Background - Stop Check
		sprintf(tier_command,"STC\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_WRITE)
			{
				// Set flag to start background check
				strcat((char *)USB_TXMessage, ReturnLinefeed);
				return TRUE;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			return FALSE;
		}
	
		//* Features - Background - Powerup Mode
		sprintf(tier_command,"PUM\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
					lmi.features.background.info.powerup_mode = (BYTE) atoi(message);
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.background.info.powerup_mode = DEFAULT_BACKGROUND_POWERUP_MODE;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.background.info.powerup_mode);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
	
		//* Features - Background - Operating Mode
		sprintf(tier_command,"OPM\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
					lmi.features.background.info.operating_mode = (BYTE) atoi(message);
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.background.info.operating_mode = DEFAULT_BACKGROUND_OPERATING_MODE;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.background.info.operating_mode);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
	
		//* Features - Background - Delay on Power-up
		sprintf(tier_command,"DLY\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
					lmi.features.background.info.delay = (BYTE) atoi(message);
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.background.info.delay = DEFAULT_BACKGROUND_DELAY;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.background.info.delay);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
	
		//* Features - Background - Log After Background Check
		sprintf(tier_command,"LBC\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
					lmi.features.background.info.log_result = (BYTE) atoi(message);
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.background.info.log_result = DEFAULT_BACKGROUND_LOG_RESULT;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.background.info.log_result);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
	
		//* Features - Background - Sample Time
		sprintf(tier_command,"STI\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
					lmi.features.background.info.sample_time = (BYTE) atoi(message);
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.background.info.sample_time = DEFAULT_BACKGROUND_SAMPLE_TIME;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.background.info.sample_time);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
	
		//* Features - Background - Clear Initial Reading
		sprintf(tier_command,"CIR\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_RESET)
			{
				lmi.features.background.info.initial_reading.value = 0.0;
				lmi.features.background.info.initial_reading.units = DEFAULT_BACKGROUND_UNITS;
				lmi.features.background.info.initial_reading.multiplier = DEFAULT_BACKGROUND_MULTIPLIER;
				sprintf((char *)tier_command,"01\r\n\0");
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
			return FALSE;
		}
	
		//* Features - Background - Clear Initial Date
		sprintf(tier_command,"CID\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_RESET)
			{
				lmi.features.background.info.initial_date.month = 0;
				lmi.features.background.info.initial_date.day = 0;
				lmi.features.background.info.initial_date.year = 0;
				sprintf((char *)tier_command,"01\r\n\0");
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
			return FALSE;
		}
	
		//* Features - Background - Clear Initial Time
		sprintf(tier_command,"CIT\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_RESET)
			{
				lmi.features.background.info.initial_time.hours = 0;
				lmi.features.background.info.initial_time.minutes = 0;
				lmi.features.background.info.initial_time.seconds = 0;
				lmi.features.background.info.initial_time.format = 0;
				sprintf((char *)tier_command,"01\r\n\0");
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
			return FALSE;
		}
	
		//* Features - Background - Clear Last Reading
		sprintf(tier_command,"CLR\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_RESET)
			{
				lmi.features.background.info.last_reading.value = 0.0;
				lmi.features.background.info.last_reading.units = DEFAULT_BACKGROUND_UNITS;
				lmi.features.background.info.last_reading.multiplier = DEFAULT_BACKGROUND_MULTIPLIER;
				sprintf((char *)tier_command,"01\r\n\0");
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
			return FALSE;
		}
	
		//* Features - Background - Clear Last Date
		sprintf(tier_command,"CLD\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_RESET)
			{
				lmi.features.background.info.last_date.month = 0;
				lmi.features.background.info.last_date.day = 0;
				lmi.features.background.info.last_date.year = 0;
				sprintf((char *)tier_command,"01\r\n\0");
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
			return FALSE;
		}
	
		//* Features - Background - Clear Last Time
		sprintf(tier_command,"CLT\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_RESET)
			{
				lmi.features.background.info.last_time.hours = 0;
				lmi.features.background.info.last_time.minutes = 0;
				lmi.features.background.info.last_time.seconds = 0;
				lmi.features.background.info.last_time.format = 0;
				sprintf((char *)tier_command,"01\r\n\0");
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
			return FALSE;
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Features_Integrate_Handler(void)
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
//*
//*		INS:FEA:INT:GET:
//*		INS:FEA:INT:SET:
//*		INS:FEA:INT:CRG:GET:VAL
//*		INS:FEA:INT:CRG:GET:UNI
//*		INS:FEA:INT:CRG:GET:MUL
//*		INS:FEA:INT:CRG:SET:VAL
//*		INS:FEA:INT:CRG:SET:UNI
//*		INS:FEA:INT:CRG:SET:MUL
//*

BOOL Features_Integrate_Handler(void)
{
	float float_value = 0.0;
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE value;

	message = Parse_Message(NULL);
	if(message != NULL)
	{
		//* Features - Integrate - Current Reading
		sprintf(tier_command,"CRG\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			process = Process_Handler();
			if(process != COMMAND_ERROR)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
				{
					//* Features - Integrate - Value
					sprintf(tier_command,"VAL\0");
					response = strcmp(message,tier_command);
					if(response == 0)
					{
						Append_Message(tier_command, ColonSeparator);
						if(process == COMMAND_WRITE)
						{
//							message = Parse_Message(NULL);
//							if(message != NULL)
//							{
//								float_value = atof(message);
//								lmi.features.integrate.info.current_reading.value = float_value;
//							}
//							else
//								return FALSE;
							return FALSE;
						}
						else if(process == COMMAND_RESET)
						{
							lmi.features.integrate.info.current_reading.value = DEFAULT_INTEGRATE_VALUE;
							Rate_Accumulation = DEFAULT_INTEGRATE_VALUE;
						}
						else if(process == COMMAND_READ)
						{
							Write_EEPROM = FALSE;				
						}
						sprintf((char *)tier_command,"%.1f\r\n\0",lmi.features.integrate.info.current_reading.value);
						strcat((char *)USB_TXMessage, tier_command);
						return TRUE;
					}
	
					//* Features - Integrate - Multiplier
					sprintf(tier_command,"MUL\0");
					response = strcmp(message,tier_command);
					if(response == 0)
					{
						Append_Message(tier_command, ColonSeparator);
						if(process == COMMAND_WRITE)
						{
							message = Parse_Message(NULL);
							if(message != NULL)
							{
								value = (BYTE) atoi(message);
								lmi.features.integrate.info.current_reading.multiplier = value + MULTIPLIER_ADJUST;
							}
							else
								return FALSE;
						}
						else if(process == COMMAND_RESET)
						{
							lmi.features.integrate.info.current_reading.multiplier = DEFAULT_INTEGRATE_MULTIPLIER;
						}
						else if(process == COMMAND_READ)
						{
							Write_EEPROM = FALSE;				
						}
						sprintf((char *)tier_command,"%d\r\n\0",lmi.features.integrate.info.current_reading.multiplier - MULTIPLIER_ADJUST);
						strcat((char *)USB_TXMessage, tier_command);
						return TRUE;
					}
	
					//* Features - Integrate - Units
					sprintf(tier_command,"UNI\0");
					response = strcmp(message,tier_command);
					if(response == 0)
					{
						Append_Message(tier_command, ColonSeparator);
						if(process == COMMAND_WRITE)
						{
							message = Parse_Message(NULL);
							if(message != NULL)
							{
								value = (BYTE) atoi(message);
								lmi.features.integrate.info.current_reading.units = value;
							}
							else
								return FALSE;
						}
						else if(process == COMMAND_RESET)
						{
							lmi.features.integrate.info.current_reading.units = DEFAULT_INTEGRATE_UNITS;
						}
						else if(process == COMMAND_READ)
						{
							Write_EEPROM = FALSE;				
						}
						sprintf((char *)tier_command,"%d\r\n\0",lmi.features.integrate.info.current_reading.units);
						strcat((char *)USB_TXMessage, tier_command);
						return TRUE;
					}
				}
			}
			return FALSE;
		}
	
		// Features - Integrate - Get/Set/Reset
		process = Post_Process_Handler(message);
		if(process != COMMAND_ERROR)
		{
			return Features_Integrate_Process(process);
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Features_Integrate_Process(BYTE process)
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
//* 	
//*		INS:FEA:INT:GET:PUM
//*		INS:FEA:INT:GET:OPM
//*		INS:FEA:INT:GET:DLY
//*		INS:FEA:INT:GET:LAD
//*		INS:FEA:INT:GET:LOS
//*		INS:FEA:INT:GET:VAL
//*		INS:FEA:INT:GET:UNI
//*		INS:FEA:INT:GET:MUL
//*		INS:FEA:INT:SET:PUM
//*		INS:FEA:INT:SET:OPM
//*		INS:FEA:INT:SET:DLY
//*		INS:FEA:INT:SET:LAD
//*		INS:FEA:INT:SET:LOS
//*		INS:FEA:INT:RES:PUM
//*		INS:FEA:INT:RES:OPM
//*		INS:FEA:INT:RES:DLY
//*		INS:FEA:INT:RES:LAD
//*		INS:FEA:INT:RES:LOS
//*		INS:FEA:INT:RES:CIN
//*

BOOL Features_Integrate_Process(BYTE process)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	char value, maxvalue;
	BYTE response = 0;

	message = Parse_Message(NULL);
	if(message != NULL)
	{
		//* Features - Integrate - Powerup Mode
		sprintf(tier_command,"PUM\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
				{
					value = (BYTE) atoi(message);
					maxvalue = END_MODE_POWERUP;
					if(!Event_Handler(value, maxvalue, &lmi.features.integrate.info.powerup_mode))
						return FALSE;
				}
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.integrate.info.powerup_mode = DEFAULT_INTEGRATE_POWERUP_MODE;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.integrate.info.powerup_mode);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
	
		//* Features - Integrate - Operating Mode
		sprintf(tier_command,"OPM\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
				{
					value = (BYTE) atoi(message);
					if(!Event_Handler(value, 2, &lmi.features.integrate.info.operating_mode))
						return FALSE;
					integrate_update_tick = tick + (DWORD) TICKS_PER_SEC;									// doesn't matter if it is on or off (9DP v1.01.05)
				}
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.integrate.info.operating_mode = DEFAULT_INTEGRATE_OPERATING_MODE;			
				integrate_update_tick = tick + (DWORD) TICKS_PER_SEC;										// doesn't matter if it is on or off (9DP v1.01.05)
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.integrate.info.operating_mode);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
	
		//* Features - Integrate - Delay on Power-up
		sprintf(tier_command,"DLY\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
					lmi.features.integrate.info.delay = (BYTE) atoi(message);
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.integrate.info.delay = DEFAULT_INTEGRATE_DELAY;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.integrate.info.delay);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
	
		//* Features - Integrate - Log On Start Up
		sprintf(tier_command,"LAD\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
				{
					value = (BYTE) atoi(message);
					if(!Event_Handler(value, 2, &lmi.features.integrate.info.startup_log))
						return FALSE;
					Power_Up_Report_Integrate = (BOOL) lmi.features.integrate.info.startup_log;
				}
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.integrate.info.startup_log = DEFAULT_INTEGRATE_LOG_RESULT;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.integrate.info.startup_log);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
	
		//* Features - Integrate - Log on Shutdown
		sprintf(tier_command,"LOS\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
				{
					value = (BYTE) atoi(message);
					if(!Event_Handler(value, 2, &lmi.features.integrate.info.shutdown_log))
						return FALSE;
					Power_Down_Report_Integrate = (BOOL) lmi.features.integrate.info.shutdown_log;
				}
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.integrate.info.shutdown_log = DEFAULT_INTEGRATE_LOG_SHUTDOWN;
				Power_Down_Report_Integrate = (BOOL) lmi.features.integrate.info.shutdown_log;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.integrate.info.shutdown_log);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
	
		//* Features - Integrate - Save on Shutdown
		sprintf(tier_command,"SOS\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)	
				{
					value = (BYTE) atoi(message);
					if(!Event_Handler(value, 2, &lmi.features.integrate.info.save_on_shutdown))
						return FALSE;
				}
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.integrate.info.save_on_shutdown = DEFAULT_INTEGRATE_SAVE_ON_SHUTDOWN;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.integrate.info.save_on_shutdown);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
	
		//* Features - Integrate - Clear
		sprintf(tier_command,"CIN\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_RESET)
			{
				Rate_Accumulation = 0.0;
				Append_Message(MessageOK, ReturnLinefeed);
				return TRUE;
			}
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Features_Data_Logging_Handler(void)
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
//* 	
//*		INS:FEA:DLG:GET:
//*		INS:FEA:DLG:SET:
//*		INS:FEA:DLG:LRG:GET:VAL
//*		INS:FEA:DLG:LRG:GET:UNI
//*		INS:FEA:DLG:LRG:GET:MUL
//*		INS:FEA:DLG:LRD:GET:MON
//*		INS:FEA:DLG:LRD:GET:DAY
//*		INS:FEA:DLG:LRD:GET:YEA
//*		INS:FEA:DLG:LRT:GET:HRS
//*		INS:FEA:DLG:LRT:GET:MIN
//*		INS:FEA:DLG:LRT:GET:FOR
//*		INS:FEA:DLG:LRG:SET:VAL
//*		INS:FEA:DLG:LRG:SET:UNI
//*		INS:FEA:DLG:LRG:SET:MUL
//*		INS:FEA:DLG:LRD:SET:MON
//*		INS:FEA:DLG:LRD:SET:DAY
//*		INS:FEA:DLG:LRD:SET:YEA
//*		INS:FEA:DLG:LRT:SET:HRS
//*		INS:FEA:DLG:LRT:SET:MIN
//*		INS:FEA:DLG:LRT:SET:FOR
//*

BOOL Features_Data_Logging_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;

	message = Parse_Message(NULL);
	if(message != NULL)
	{
		//* Features - Data Logging - Last Reading
		sprintf(tier_command,"LRG\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			process = Process_Handler();
			if(process == COMMAND_READ)
				return Reading_Handler(process, &lmi.features.data_logging.info.last_reading.value, &lmi.features.data_logging.info.last_reading.units, &lmi.features.data_logging.info.last_reading.multiplier);
			else
				return FALSE;
		}
	
		//* Features - Data Logging - Last Reading Date
		sprintf(tier_command,"LRD\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			process = Process_Handler();
			if(process == COMMAND_READ)
//				return Date_Handler(process, &lmi.features.data_logging.info.last_date.month);
				return Date_Handler(process, &lmi.features.data_logging.info.last_date);			// (9DP v1.01.21)
			else
				return FALSE;
		}
	
		//* Features - Data Logging - Last Reading Time
		sprintf(tier_command,"LRT\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			process = Process_Handler();
			if(process == COMMAND_READ)
//				return Time_Handler(process, &lmi.features.data_logging.info.last_time.hours);
				return Time_Handler(process, &lmi.features.data_logging.info.last_time);			// (9DP v1.01.21)
			else
				return FALSE;
		}
	
		// Features - Data Logging - Get/Set/Reset
		process = Post_Process_Handler(message);
		if(process != COMMAND_ERROR)
		{
			return Features_Data_Logging_Process(process);
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Features_Data_Logging_Process(BYTE process)
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
//* 	
//*		INS:FEA:DLG:GET:PUM
//*		INS:FEA:DLG:GET:OPM
//*		INS:FEA:DLG:GET:DSP
//*		INS:FEA:DLG:GET:DLY
//*		INS:FEA:DLG:GET:LAD
//*		INS:FEA:DLG:GET:LOS
//*		INS:FEA:DLG:SET:PUM
//*		INS:FEA:DLG:SET:OPM
//*		INS:FEA:DLG:SET:DSP
//*		INS:FEA:DLG:SET:DLY
//*		INS:FEA:DLG:SET:LAD
//*		INS:FEA:DLG:SET:LOS
//*		INS:FEA:DLG:SET:SIN
//*		INS:FEA:DLG:SET:STN
//*		INS:FEA:DLG:SET:CIN
//*		INS:FEA:DLG:RES:PUM
//*		INS:FEA:DLG:RES:OPM
//*		INS:FEA:DLG:RES:DSP
//*		INS:FEA:DLG:RES:DLY
//*		INS:FEA:DLG:RES:LAD
//*		INS:FEA:DLG:RES:LOS
//*		INS:FEA:DLG:RES:SIN
//*		INS:FEA:DLG:RES:STN
//*		INS:FEA:DLG:RES:CIN
//*

BOOL Features_Data_Logging_Process(BYTE process)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE value, maxvalue, previousvalue;

	message = Parse_Message(NULL);
	if(message != NULL)
	{
		//* Features - Data Logging - Powerup Mode
		sprintf(tier_command,"PUM\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
				{
					value = (BYTE) atoi(message);
					maxvalue = END_MODE_POWERUP;
					if(!Event_Handler(value, maxvalue, &lmi.features.data_logging.info.powerup_mode))
						return FALSE;
				}
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.data_logging.info.powerup_mode = DEFAULT_DATA_LOGGING_POWERUP_MODE;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.data_logging.info.powerup_mode);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
	
		//* Features - Data Logging - Operating Mode
		sprintf(tier_command,"OPM\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
				{
					value = (BYTE) atoi(message);
					if(!Event_Handler(value, 2, &lmi.features.data_logging.info.operating_mode))
						return FALSE;
					Data_Logging_Enabled = (BOOL) value;
					log_file_write_tick = tick + (lmi.features.data_logging.info.sample_period * (DWORD)TICKS_PER_SEC);
					Update_Log_File = FALSE;
					Data_Logging_Header_Sent = FALSE;
				}
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.data_logging.info.operating_mode = DEFAULT_DATA_LOGGING_OPERATING_MODE;
				Data_Logging_Enabled = FALSE;
				Data_Logging_Header_Sent = FALSE;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.data_logging.info.operating_mode);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
	
		//* Features - Data Logging - Delay After Power Up
		sprintf(tier_command,"DLY\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
					lmi.features.data_logging.info.delay = (BYTE) atoi(message);
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.data_logging.info.delay = DEFAULT_DATA_LOGGING_DELAY;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.data_logging.info.delay);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
	
		//* Features - Data Logging - Sample Period
		sprintf(tier_command,"DSP\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
					lmi.features.data_logging.info.sample_period = (BYTE) atoi(message);
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.data_logging.info.sample_period = DEFAULT_DATA_LOGGING_SAMPLE_PERIOD;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.data_logging.info.sample_period);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
	
//		//* Features - Data Logging - Start Data Logging
//		sprintf(tier_command,"SDL\0");
//		response = strcmp(message,tier_command);
//		if(response == 0)
//		{
//			Append_Message(tier_command, ColonSeparator);
//			if(process == COMMAND_WRITE)
//			{
//				lmi.features.data_logging.info.operating_mode = 1;
//				Append_Message(MessageOK, ReturnLinefeed);
//				return TRUE;
//			}
//			return FALSE;
//		}
//	
//		//* Features - Data Logging - Stop Data Logging
//		sprintf(tier_command,"STL\0");
//		response = strcmp(message,tier_command);
//		if(response == 0)
//		{
//			if(process == COMMAND_WRITE)
//			{
//				lmi.features.data_logging.info.operating_mode = 0;
//				Append_Message(MessageOK, ReturnLinefeed);
//				return TRUE;
//			}
//			return FALSE;
//		}
	
		//* Features - Data Logging - Report Model Number
		sprintf(tier_command,"RMN\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			previousvalue = lmi.features.data_logging.info.reported_data.model_number;
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
				{
					value = (BYTE) atoi(message);
					if(!Event_Handler(value, 2, &lmi.features.data_logging.info.reported_data.model_number))
						return FALSE;
				}
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.data_logging.info.reported_data.model_number = DEFAULT_DATA_LOGGING_EVENT;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			Request_New_Header(previousvalue, lmi.features.data_logging.info.reported_data.model_number);
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.data_logging.info.reported_data.model_number);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
	
		//* Features - Data Logging - Report Serial Number
		sprintf(tier_command,"RSN\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			previousvalue = lmi.features.data_logging.info.reported_data.serial_number;
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
				{
					value = (BYTE) atoi(message);
					if(!Event_Handler(value, 2, &lmi.features.data_logging.info.reported_data.serial_number))
						return FALSE;
				}
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.data_logging.info.reported_data.serial_number = DEFAULT_DATA_LOGGING_EVENT;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			Request_New_Header(previousvalue, lmi.features.data_logging.info.reported_data.serial_number);
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.data_logging.info.reported_data.serial_number);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
	
		//* Features - Data Logging - User ID Number
		sprintf(tier_command,"RUI\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			previousvalue = lmi.features.data_logging.info.reported_data.user_id_number;
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
				{
					value = (BYTE) atoi(message);
					if(!Event_Handler(value, 2, &lmi.features.data_logging.info.reported_data.user_id_number))
						return FALSE;
				}
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.data_logging.info.reported_data.user_id_number = DEFAULT_DATA_LOGGING_EVENT;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			Request_New_Header(previousvalue, lmi.features.data_logging.info.reported_data.user_id_number);
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.data_logging.info.reported_data.user_id_number);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
	
		//* Features - Data Logging - Report User Serial Number
		sprintf(tier_command,"RUS\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			previousvalue = lmi.features.data_logging.info.reported_data.user_serial_number;
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
				{
					value = (BYTE) atoi(message);
					if(!Event_Handler(value, 2, &lmi.features.data_logging.info.reported_data.user_serial_number))
						return FALSE;
				}
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.data_logging.info.reported_data.user_serial_number = DEFAULT_DATA_LOGGING_EVENT;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			Request_New_Header(previousvalue, lmi.features.data_logging.info.reported_data.user_serial_number);
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.data_logging.info.reported_data.user_serial_number);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
	
		//* Features - Data Logging - Date
		sprintf(tier_command,"RDA\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			previousvalue = lmi.features.data_logging.info.reported_data.date;
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
				{
					value = (BYTE) atoi(message);
					if(!Event_Handler(value, 2, &lmi.features.data_logging.info.reported_data.date))
						return FALSE;
				}
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.data_logging.info.reported_data.date = DEFAULT_DATA_LOGGING_EVENT;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			Request_New_Header(previousvalue, lmi.features.data_logging.info.reported_data.date);
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.data_logging.info.reported_data.date);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
	
		//* Features - Data Logging - Time
		sprintf(tier_command,"RTI\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			previousvalue = lmi.features.data_logging.info.reported_data.time;
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
				{
					value = (BYTE) atoi(message);
					if(!Event_Handler(value, 2, &lmi.features.data_logging.info.reported_data.time))
						return FALSE;
				}
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.data_logging.info.reported_data.time = DEFAULT_DATA_LOGGING_EVENT;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			Request_New_Header(previousvalue, lmi.features.data_logging.info.reported_data.time);
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.data_logging.info.reported_data.time);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
	
		//* Features - Data Logging - Report Location
		sprintf(tier_command,"LOC\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			previousvalue = lmi.features.data_logging.info.reported_data.location;
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
				{
					value = (BYTE) atoi(message);
					if(!Event_Handler(value, 2, &lmi.features.data_logging.info.reported_data.location))
						return FALSE;
				}
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.data_logging.info.reported_data.location = DEFAULT_DATA_LOGGING_EVENT;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			Request_New_Header(previousvalue, lmi.features.data_logging.info.reported_data.location);
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.data_logging.info.reported_data.location);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
	
		//* Features - Data Logging - Type of Reading
		sprintf(tier_command,"TOR\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			previousvalue = lmi.features.data_logging.info.reported_data.type_of_report;
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
				{
					value = (BYTE) atoi(message);
					if(!Event_Handler(value, 2, &lmi.features.data_logging.info.reported_data.type_of_report))
						return FALSE;
				}
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.data_logging.info.reported_data.type_of_report = DEFAULT_DATA_LOGGING_EVENT;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			Request_New_Header(previousvalue, lmi.features.data_logging.info.reported_data.type_of_report);
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.data_logging.info.reported_data.type_of_report);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
	
		//* Features - Data Logging - Report Reading
		sprintf(tier_command,"RDG\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			previousvalue = lmi.features.data_logging.info.reported_data.reading;
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
				{
					value = (BYTE) atoi(message);
					if(!Event_Handler(value, 2, &lmi.features.data_logging.info.reported_data.reading))
						return FALSE;
				}
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.data_logging.info.reported_data.reading = DEFAULT_DATA_LOGGING_EVENT;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			Request_New_Header(previousvalue, lmi.features.data_logging.info.reported_data.reading);
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.data_logging.info.reported_data.reading);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
	
		//* Features - Data Logging - Report Reading Units
		sprintf(tier_command,"RDU\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			previousvalue = lmi.features.data_logging.info.reported_data.reading_units;
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
				{
					value = (BYTE) atoi(message);
					if(!Event_Handler(value, 2, &lmi.features.data_logging.info.reported_data.reading_units))
						return FALSE;
				}
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.data_logging.info.reported_data.reading_units = DEFAULT_DATA_LOGGING_EVENT;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			Request_New_Header(previousvalue, lmi.features.data_logging.info.reported_data.reading_units);
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.data_logging.info.reported_data.reading_units);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
	
		//* Features - Data Logging - Report Reading Multiplier
		sprintf(tier_command,"RDM\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			previousvalue = lmi.features.data_logging.info.reported_data.reading_multiplier;
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
				{
					value = (BYTE) atoi(message);
					if(!Event_Handler(value, 2, &lmi.features.data_logging.info.reported_data.reading_multiplier))
						return FALSE;
				}
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.data_logging.info.reported_data.reading_multiplier = DEFAULT_DATA_LOGGING_EVENT;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			Request_New_Header(previousvalue, lmi.features.data_logging.info.reported_data.reading_multiplier);
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.data_logging.info.reported_data.reading_multiplier);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
	
		//* Features - Data Logging - Report Integrated Reading
		sprintf(tier_command,"INR\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			previousvalue = lmi.features.data_logging.info.reported_data.integrated_reading;
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
				{
					value = (BYTE) atoi(message);
					if(!Event_Handler(value, 2, &lmi.features.data_logging.info.reported_data.integrated_reading))
						return FALSE;
				}
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.data_logging.info.reported_data.integrated_reading = DEFAULT_DATA_LOGGING_EVENT;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			Request_New_Header(previousvalue, lmi.features.data_logging.info.reported_data.integrated_reading);
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.data_logging.info.reported_data.integrated_reading);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
	
		//* Features - Data Logging - Report Integrated Reading Units
		sprintf(tier_command,"INU\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			previousvalue = lmi.features.data_logging.info.reported_data.integrated_reading_units;
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
				{
					value = (BYTE) atoi(message);
					if(!Event_Handler(value, 2, &lmi.features.data_logging.info.reported_data.integrated_reading_units))
						return FALSE;
				}
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.data_logging.info.reported_data.integrated_reading_units = DEFAULT_DATA_LOGGING_EVENT;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			Request_New_Header(previousvalue, lmi.features.data_logging.info.reported_data.integrated_reading_units);
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.data_logging.info.reported_data.integrated_reading_units);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
	
		//* Features - Data Logging - Report Integrated Reading Multiplier
		sprintf(tier_command,"INM\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			previousvalue = lmi.features.data_logging.info.reported_data.integrated_reading_multiplier;
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
				{
					value = (BYTE) atoi(message);
					if(!Event_Handler(value, 2, &lmi.features.data_logging.info.reported_data.integrated_reading_multiplier))
						return FALSE;
				}
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.data_logging.info.reported_data.integrated_reading_multiplier = DEFAULT_DATA_LOGGING_EVENT;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			Request_New_Header(previousvalue, lmi.features.data_logging.info.reported_data.integrated_reading_multiplier);
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.data_logging.info.reported_data.integrated_reading_multiplier);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}

		//* Features - Data Logging - Report Temperature
		sprintf(tier_command,"TEM\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			previousvalue = lmi.features.data_logging.info.reported_data.temperature;
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
				{
					value = (BYTE) atoi(message);
					if(!Event_Handler(value, 2, &lmi.features.data_logging.info.reported_data.temperature))
						return FALSE;
				}
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.data_logging.info.reported_data.temperature = DEFAULT_DATA_LOGGING_EVENT;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			Request_New_Header(previousvalue, lmi.features.data_logging.info.reported_data.temperature);
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.data_logging.info.reported_data.temperature);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}

		//* Features - Data Logging - Report High Voltage 1
		sprintf(tier_command,"HV1\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			previousvalue = lmi.features.data_logging.info.reported_data.high_voltage_1;
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
				{
					value = (BYTE) atoi(message);
					if(!Event_Handler(value, 2, &lmi.features.data_logging.info.reported_data.high_voltage_1))
						return FALSE;
				}
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.data_logging.info.reported_data.high_voltage_1 = DEFAULT_DATA_LOGGING_EVENT;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			Request_New_Header(previousvalue, lmi.features.data_logging.info.reported_data.high_voltage_1);
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.data_logging.info.reported_data.high_voltage_1);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}

		//* Features - Data Logging - Battery Voltage
		sprintf(tier_command,"BAT\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			previousvalue = lmi.features.data_logging.info.reported_data.battery_voltage;
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
				{
					value = (BYTE) atoi(message);
					if(!Event_Handler(value, 2, &lmi.features.data_logging.info.reported_data.battery_voltage))
						return FALSE;
				}
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.data_logging.info.reported_data.battery_voltage = DEFAULT_DATA_LOGGING_EVENT;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			Request_New_Header(previousvalue, lmi.features.data_logging.info.reported_data.battery_voltage);
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.data_logging.info.reported_data.battery_voltage);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}

		//* Features - Data Logging - Report System Status
		sprintf(tier_command,"SYS\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			previousvalue = lmi.features.data_logging.info.reported_data.system_status;
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
				{
					value = (BYTE) atoi(message);
					if(!Event_Handler(value, 2, &lmi.features.data_logging.info.reported_data.system_status))
						return FALSE;
				}
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.data_logging.info.reported_data.system_status = DEFAULT_DATA_LOGGING_EVENT;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			Request_New_Header(previousvalue, lmi.features.data_logging.info.reported_data.system_status);
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.data_logging.info.reported_data.system_status);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}

		//* Features - Data Logging - Report Input Voltage (9DP v1.02.06)
		sprintf(tier_command,"INP\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			previousvalue = lmi.features.data_logging.info.reported_data.input_voltage;
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
				{
					value = (BYTE) atoi(message);
					if(!Event_Handler(value, 2, &lmi.features.data_logging.info.reported_data.input_voltage))
						return FALSE;
				}
				else
					return FALSE;
			}
			else if(process == COMMAND_RESET)
			{
				lmi.features.data_logging.info.reported_data.input_voltage = DEFAULT_DATA_LOGGING_EVENT;
			}
			else if(process == COMMAND_READ)
			{
				Write_EEPROM = FALSE;
			}
			Request_New_Header(previousvalue, lmi.features.data_logging.info.reported_data.input_voltage);
			sprintf((char *)tier_command,"%d\r\n\0",lmi.features.data_logging.info.reported_data.input_voltage);
			strcat((char *)USB_TXMessage, tier_command);
			return TRUE;
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Request_New_Header(BYTE previousvalue, BYTE value);
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
//* 	

void Request_New_Header(BYTE previousvalue, BYTE value)
{
	if(previousvalue != value)
	{
		if((Data_Logging_Enabled == TRUE) && (Data_Logging_Header_Sent == TRUE))
		{
			Data_Logging_Header_Sent = FALSE;
		}
	}

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	END FEATURES HANDLERS
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//*	CONTROLS HANDLERS
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Controls_Handler(void)
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
//* 	
//*		INS:CON:BAK:
//*		INS:CON:AUD:
//*		INS:CON:GPS:
//*		INS:CON:BTH:
//*

BOOL Controls_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response;
	BYTE process;
	BOOL status;

	message = Parse_Message(NULL);
	if(message != NULL)
	{
		//* Controls - Backlight
		sprintf(tier_command,"BAK\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Controls_Backlight_Handler();
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_BACKLIGHT_LOCATION, 2, lmi.controls.backlight.buffer, SIZE_BACKLIGHT);
			}
			return status;
		}
	
		//* Controls - Audio
		sprintf(tier_command,"AUD\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Controls_Audio_Handler();
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_AUDIO_LOCATION, 2, lmi.controls.audio.buffer, SIZE_AUDIO);
			}
			return status;
		}
	
		//* Controls - GPS
		sprintf(tier_command,"GPS\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Controls_GPS_Handler();
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_GPS_LOCATION, 2, lmi.controls.gps.buffer, SIZE_GPS);
			}
			return status;
		}
	
		//* Controls - Bluetooth
		sprintf(tier_command,"BTH\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Controls_Bluetooth_Handler();
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_BLUETOOTH_LOCATION, 2, lmi.controls.bluetooth.buffer, SIZE_BLUETOOTH);
			}
			return status;
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Controls_Backlight_Handler(void)
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
//* 	
//*		INS:CON:BAK:GET:PUM
//*		INS:CON:BAK:GET:OPM
//*		INS:CON:BAK:GET:CBL
//*		INS:CON:BAK:GET:ALO
//*		INS:CON:BAK:GET:AME
//*		INS:CON:BAK:GET:AHI
//*		INS:CON:BAK:GET:ONL
//*		INS:CON:BAK:GET:TEM
//*		INS:CON:BAK:SET:PUM
//*		INS:CON:BAK:SET:OPM
//*		INS:CON:BAK:SET:CBL
//*		INS:CON:BAK:SET:ALO
//*		INS:CON:BAK:SET:AME
//*		INS:CON:BAK:SET:AHI
//*		INS:CON:BAK:SET:ONL
//*		INS:CON:BAK:RES:TEM
//*		INS:CON:BAK:RES:PUM
//*		INS:CON:BAK:RES:OPM
//*		INS:CON:BAK:RES:CBL
//*		INS:CON:BAK:RES:ALO
//*		INS:CON:BAK:RES:AME
//*		INS:CON:BAK:RES:AHI
//*		INS:CON:BAK:RES:ONL
//*		INS:CON:BAK:RES:TEM
//*

BOOL Controls_Backlight_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE value, maxvalue;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{
			//* Controls - Backlight - Powerup Mode
			sprintf(tier_command,"PUM\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = (BYTE) atoi(message);
						maxvalue = END_MODE_BACKLIGHT;
						if(!Event_Handler(value, maxvalue, &lmi.controls.backlight.info.powerup_mode))
							return FALSE;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.controls.backlight.info.powerup_mode = DEFAULT_BACKLIGHT_POWERUP_MODE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0",lmi.controls.backlight.info.powerup_mode);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Controls - Backlight - Operating Mode
			sprintf(tier_command,"OPM\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = (BYTE) atoi(message);
						maxvalue = END_MODE_BACKLIGHT - 2;
						if(!Event_Handler(value, maxvalue, &lmi.controls.backlight.info.operating_mode))
							return FALSE;
						Backlight_Mode = lmi.controls.backlight.info.operating_mode;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.controls.backlight.info.operating_mode = DEFAULT_BACKLIGHT_OPERATING_MODE;
					Backlight_Mode = lmi.controls.backlight.info.operating_mode;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0",lmi.controls.backlight.info.operating_mode);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Controls - Backlight - Current Level
			sprintf(tier_command,"CBL\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if((process == COMMAND_WRITE) || (process == COMMAND_RESET))
					return FALSE;
				Write_EEPROM = FALSE;
				sprintf((char *)tier_command,"%d\r\n\0",lmi.controls.backlight.info.current_level);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Controls - Backlight - Auto Low Power
			sprintf(tier_command,"ALO\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = (BYTE) atoi(message);
						lmi.controls.backlight.info.auto_low = value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.controls.backlight.info.auto_low = DEFAULT_BACKLIGHT_AUTO_LOW;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0",lmi.controls.backlight.info.auto_low);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Controls - Backlight - Auto Medium Power
			sprintf(tier_command,"AME\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = (BYTE) atoi(message);
						lmi.controls.backlight.info.auto_medium = value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.controls.backlight.info.auto_medium = DEFAULT_BACKLIGHT_AUTO_MEDIUM;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0",lmi.controls.backlight.info.auto_medium);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Controls - Backlight - Auto High Power
			sprintf(tier_command,"AHI\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = (BYTE) atoi(message);
						lmi.controls.backlight.info.auto_high = value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.controls.backlight.info.auto_high = DEFAULT_BACKLIGHT_AUTO_HIGH;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0",lmi.controls.backlight.info.auto_high);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Controls - Backlight - On Level Power
			sprintf(tier_command,"ONL\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = (BYTE) atoi(message);
						lmi.controls.backlight.info.on_level = value;
						if(Backlight_Mode == BK_LIGHT_ON)
						{
							Set_Backlight_Level(lmi.controls.backlight.info.on_level);
						}
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.controls.backlight.info.on_level = DEFAULT_BACKLIGHT_ON_LEVEL;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0",lmi.controls.backlight.info.on_level);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Controls - Backlight - Temp Time
			sprintf(tier_command,"TEM\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = (BYTE) atoi(message);
						lmi.controls.backlight.info.temp_time = value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.controls.backlight.info.temp_time = DEFAULT_BACKLIGHT_TEMP_TIME;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0",lmi.controls.backlight.info.temp_time);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Controls_Audio_Handler(void)
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
//* 	
//*		INS:CON:AUD:GET:PUM
//*		INS:CON:AUD:GET:OPM
//*		INS:CON:AUD:GET:CUL
//*		INS:CON:AUD:GET:ALE
//*		INS:CON:AUD:GET:ALM
//*		INS:CON:AUD:SET:PUM
//*		INS:CON:AUD:SET:OPM
//*		INS:CON:AUD:SET:CUL
//*		INS:CON:AUD:SET:ALE
//*		INS:CON:AUD:SET:ALM
//*		INS:CON:AUD:RES:PUM
//*		INS:CON:AUD:RES:OPM
//*		INS:CON:AUD:RES:CUL
//*		INS:CON:AUD:RES:ALE
//*		INS:CON:AUD:RES:ALM
//*

BOOL Controls_Audio_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE value, maxvalue;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{
			//* Controls - Audio - Powerup Mode
			sprintf(tier_command,"PUM\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = (BYTE) atoi(message);
						maxvalue = END_MODE_POWERUP;
						if(!Event_Handler(value, maxvalue, &lmi.controls.audio.info.powerup_mode))
							return FALSE;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.controls.audio.info.powerup_mode = DEFAULT_AUDIO_POWERUP_MODE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0",lmi.controls.audio.info.powerup_mode);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Controls - Audio - Operating Mode
			sprintf(tier_command,"OPM\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = (BYTE) atoi(message);
						maxvalue = END_LEVEL_AUDIO;
						if(!Event_Handler(value, maxvalue, &lmi.controls.audio.info.operating_mode))
							return FALSE;
						Set_Audio_Tone();
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.controls.audio.info.operating_mode = DEFAULT_AUDIO_OPERATING_MODE;
					Set_Audio_Tone();
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0",lmi.controls.audio.info.operating_mode);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Controls - Audio - Current Level
			sprintf(tier_command,"CUL\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
					sprintf((char *)tier_command,"%d\r\n\0",lmi.controls.audio.info.current_level);
					strcat((char *)USB_TXMessage, tier_command);
					return TRUE;
				}
				return FALSE;
			}
		
			//* Controls - Audio - Alert Audio Level
			sprintf(tier_command,"ALE\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = (BYTE) atoi(message);
						lmi.controls.audio.info.alert_level = value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.controls.audio.info.alert_level = DEFAULT_AUDIO_ALERT_LEVEL;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0",lmi.controls.audio.info.alert_level);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Controls - Audio - Alarm Audio Level
			sprintf(tier_command,"ALM\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = (BYTE) atoi(message);
						lmi.controls.audio.info.alarm_level = value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.controls.audio.info.alarm_level = DEFAULT_AUDIO_ALARM_LEVEL;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0",lmi.controls.audio.info.alarm_level);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Controls_GPS_Handler(void)
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
//* 	
//*		INS:CON:GPS:GET:PUM
//*		INS:CON:GPS:GET:OPM
//*		INS:CON:GPS:SET:PUM
//*		INS:CON:GPS:SET:OPM
//*		INS:CON:GPS:RES:PUM
//*		INS:CON:GPS:RES:OPM
//*

BOOL Controls_GPS_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE value, maxvalue;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{
			//* Controls - GPS - Powerup Mode
			sprintf(tier_command,"PUM\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = (BYTE) atoi(message);
						maxvalue = END_MODE_POWERUP;
						if(!Event_Handler(value, maxvalue, &lmi.controls.gps.info.powerup_mode))
							return FALSE;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.controls.gps.info.powerup_mode = DEFAULT_GPS_POWERUP_MODE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0",lmi.controls.gps.info.powerup_mode);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Controls - GPS - Operating Mode
			sprintf(tier_command,"OPM\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = (BYTE) atoi(message);
						maxvalue = 3;
						if(!Event_Handler(value, maxvalue, &lmi.controls.gps.info.operating_mode))
							return FALSE;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.controls.gps.info.operating_mode = DEFAULT_GPS_OPERATING_MODE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0",lmi.controls.gps.info.operating_mode);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Controls_Bluetooth_Handler(void)
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
//* 	
//*		INS:CON:BTH:GET:PUM
//*		INS:CON:BTH:GET:OPM
//*		INS:CON:BTH:SET:PUM
//*		INS:CON:BTH:SET:OPM
//*		INS:CON:BTH:RES:PUM
//*		INS:CON:BTH:RES:OPM
//*

BOOL Controls_Bluetooth_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE value, maxvalue;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{
			//* Controls - Bluetooth - Powerup Mode
			sprintf(tier_command,"PUM\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = (BYTE) atoi(message);
						maxvalue = END_MODE_POWERUP;
						if(!Event_Handler(value, maxvalue, &lmi.controls.bluetooth.info.powerup_mode))
							return FALSE;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.controls.bluetooth.info.powerup_mode = DEFAULT_BLUETOOTH_POWERUP_MODE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0",lmi.controls.bluetooth.info.powerup_mode);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Controls - Bluetooth - Operating Mode
			sprintf(tier_command,"OPM\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = (BYTE) atoi(message);
						maxvalue = 3;
						if(!Event_Handler(value, maxvalue, &lmi.controls.bluetooth.info.operating_mode))
							return FALSE;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.controls.bluetooth.info.operating_mode = DEFAULT_BLUETOOTH_OPERATING_MODE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0",lmi.controls.bluetooth.info.operating_mode);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	END CONTROLS HANDLERS
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//*	DISPLAY HANDLERS
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Display_Handler(void)
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
//* 	
//*		INS:DSP:PRO:
//*		INS:DSP:DET:
//*

BOOL Display_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response;
	BOOL status;

	message = Parse_Message(NULL);
	if(message != NULL)
	{
		sprintf(tier_command, "PRO\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Display_Properties_Handler();
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
#ifdef EEPROM_DISPLAY_PROPERTIES_LOCATION
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_DISPLAY_PROPERTIES_LOCATION, 2, lmi.display.properties.buffer, SIZE_DISPLAY_PROPERTIES);
#else
				StructureUpdate(1, Current_Detector);												// added detector (9DP v1.01.14)
#endif
			}
			return status;
		}
	
		sprintf(tier_command, "DET\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			return Display_Detector_Handler();
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Display_Properties_Handler(void)
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
//*
//*		INS:DSP:PRO:GET:PUM
//*		INS:DSP:PRO:SET:PUM
//*		INS:DSP:PRO:RES:PUM
//*		INS:DSP:PRO:GET:CUR
//*		INS:DSP:PRO:SET:CUR
//*		INS:DSP:PRO:RES:CUR
//*

BOOL Display_Properties_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE value, maxvalue;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{
#ifdef INSTRUMENT_MULTIPLE_DETECTOR
			sprintf(tier_command, "PUM\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = (BYTE) atoi(message);
						maxvalue = END_MODE_DETECTOR;
						if(!Event_Handler(value, maxvalue, &lmi.display.properties.info.powerup_mode))
							return FALSE;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.properties.info.powerup_mode = DEFAULT_DETECTOR_POWERUP_MODE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0",lmi.display.properties.info.powerup_mode);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
#endif

#ifdef INSTRUMENT_MULTIPLE_DETECTOR
			sprintf(tier_command, "CUR\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = (BYTE) atoi(message);
						if(Validate_Detector((char) value))
						{
							lmi.display.properties.info.current_detector = value;
							Current_Detector = lmi.display.properties.info.current_detector;
						}
						else																		// (9DP v1.01.07)
							return FALSE;															// (9DP v1.01.07)
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.properties.info.current_detector = DEFAULT_DETECTOR;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				if(Write_EEPROM)																	// (9DP v1.01.07)
				{																					// (9DP v1.01.07)
					HVBInit(TRUE);																	// (9DP v1.01.07)
				}																					// (9DP v1.01.07)
				sprintf((char *)tier_command,"%d\r\n\0",lmi.display.properties.info.current_detector);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
#endif
		}
	}

	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Display_Detector_Handler(void)
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
//*
//*		INS:DSP:DET:00x:
//*

BOOL Display_Detector_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE value;

	message = Parse_Message(NULL);
	if(message != NULL)
	{
		value = (BYTE) atoi(message);
		if(Validate_Detector((char) value))
		{
			CMD_Detector = value;
			sprintf(tier_command,"%03d:\0",CMD_Detector);
			strcat((char *)USB_TXMessage, tier_command);
			return Detector_Handler();
		}
	}

	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Detector_Handler(void)
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
//*
//*		INS:DSP:DET:00x:PRO:
//*		INS:DSP:DET:00x:CAL:
//*		INS:DSP:DET:00x:VSP:
//*

BOOL Detector_Handler(void)
{
	UINT16 address;
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE value, maxvalue;
	BOOL status;

	message = Parse_Message(NULL);
	if(message != NULL)
	{
		sprintf(tier_command, "PRO\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Detector_Properties_Handler();
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
#ifdef EEPROM_DETECTOR_PROPERTIES_LOCATION
				address = EEPROM_DETECTOR_PROPERTIES_LOCATION + (CMD_Detector * TOTAL_DETECTOR);	
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, lmi.display.detector[CMD_Detector].properties.buffer, SIZE_DETECTOR_PROPERTIES);
#else
				StructureUpdate(2, CMD_Detector);												// added detector (9DP v1.01.14)
#endif
			}
			return status;
		}
	
		sprintf(tier_command, "CAL\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Detector_Calibration_Handler();
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
#ifdef EEPROM_DETECTOR_CALIBRATION_LOCATION
				address = EEPROM_DETECTOR_CALIBRATION_LOCATION + (CMD_Detector * TOTAL_DETECTOR);	
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, lmi.display.detector[CMD_Detector].calibration.buffer, SIZE_DETECTOR_CALIBRATION);
#endif
			}
			return status;
		}
	
		sprintf(tier_command, "VSP\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			return Detector_Views_Handler();
		}
	}	
	return FALSE;
}

//**************************************************************************************************
//**************************************************************************************************

BOOL HandleProperty(
    char *tier_command,
    char *message,
    BYTE process,
    void (*write_callback)(void *, void *, UINT),
    void (*null_callback)(void *, void *, UINT),
    void (*reset_callback)(void *, void *, UINT),
    void *property, UINT value_length
)
{
    strcat((char *)USB_TXMessage, message);
    strcat((char *)USB_TXMessage, ColonSeparator);
    if(process == COMMAND_WRITE)
    {
        message = Parse_Message(NULL);
        if(message != NULL)
            write_callback(property, message, value_length);
        else
            null_callback(property, message, value_length);
    }
    else if(process == COMMAND_RESET)
    {
        reset_callback(property, message, value_length);
    }
    else
    {
        Write_EEPROM = FALSE;
    }
    Format_String(tier_command, property, value_length);		// (9DP v1.01.04)
	strcat((char *)USB_TXMessage, tier_command);
}

#ifdef DEFAULT_AUTORATE

//**************************************************************************************************
//   AutoRate Handlers
//**************************************************************************************************
void writeAutoRate(BOOL *autoRateOn, char *message, UINT value_length = 0) {
    *autoRateOn = atoi(message);
}

void nullAutoRate(BOOL *autoRateOn, char *message, UINT value_length = 0) {
    return;
}

void resetAutoRate(BOOL *autoRateOn, char *message, UINT value_length = 0) {
    *autoRateOn = DEFAULT_AUTORATE;
}

#endif


#ifdef DEFAULT_FIXEDRATE
//**************************************************************************************************
//   FixedRate Handlers
//**************************************************************************************************
void writeFixedRate(float *fixedRate, char *message, UINT value_length = 0) {
    *fixedRate = atof(message);
}

void nullFixedRate(float *fixedRate, char *message, UINT value_length = 0) {
    return;
}

void resetFixedRate(float *fixedRate, char *message, UINT value_length = 0) {
    *fixedRate = DEFAULT_FIXEDRATE;
}
#endif


//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Detector_Properties_Handler(void)
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
//*
//*		INS:DSP:DET:00x:PRO:GET:MOD
//*		INS:DSP:DET:00x:PRO:GET:LSN
//*		INS:DSP:DET:00x:PRO:GET:UID
//*		INS:DSP:DET:00x:PRO:GET:USN
//*

BOOL Detector_Properties_Handler(void)
{
	unsigned short value;
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE function;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{
#ifdef INSTRUMENT_DETECTOR
			//* Detector - Model Number
			sprintf(tier_command,"MOD\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				strcat((char *)USB_TXMessage, message);
				strcat((char *)USB_TXMessage, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = (unsigned short) atoi(message);
						lmi.display.detector[CMD_Detector].properties.info.model = value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					switch(CMD_Detector)
					{
						case 0:
						{
							lmi.display.detector[CMD_Detector].properties.info.model = DEFAULT_DETECTOR_0;
							break;
						}
						case 1:
						{
							lmi.display.detector[CMD_Detector].properties.info.model = DEFAULT_DETECTOR_1;
							break;
						}
						case 2:
						{
							lmi.display.detector[CMD_Detector].properties.info.model = DEFAULT_DETECTOR_2;
							break;
						}
						case 3:
						{
							lmi.display.detector[CMD_Detector].properties.info.model = DEFAULT_DETECTOR_3;
							break;
						}
						case 4:
						{
							lmi.display.detector[CMD_Detector].properties.info.model = DEFAULT_DETECTOR_4;
							break;
						}
					}
				}
				else
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].properties.info.model);		// (9DP v1.01.15)
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}

			//* Detector - Serial Number
			sprintf(tier_command,"LSN\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				strcat((char *)USB_TXMessage, message);
				strcat((char *)USB_TXMessage, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
						strncpy(lmi.display.detector[CMD_Detector].properties.info.serial_number, message, LENGTH_SERIAL_NUMBER);
					else
						strncpy(lmi.display.detector[CMD_Detector].properties.info.serial_number, "\0", LENGTH_SERIAL_NUMBER);
				}
				else if(process == COMMAND_RESET)
				{
					strncpy(lmi.display.detector[CMD_Detector].properties.info.serial_number, DEFAULT_DETECTOR_SERIAL_NUMBER, LENGTH_SERIAL_NUMBER);
				}
				else
				{
					Write_EEPROM = FALSE;
				}
				Format_String(tier_command, lmi.display.detector[CMD_Detector].properties.info.serial_number, LENGTH_SERIAL_NUMBER);		// (9DP v1.01.04)
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}

			//* Detector - User ID
			sprintf(tier_command,"UID\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				strcat((char *)USB_TXMessage, message);
				strcat((char *)USB_TXMessage, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
						strncpy(lmi.display.detector[CMD_Detector].properties.info.user_id_number, message, LENGTH_USER_ID_NUMBER);
					else
						strncpy(lmi.display.detector[CMD_Detector].properties.info.user_id_number, "\0", LENGTH_USER_ID_NUMBER);
				}
				else if(process == COMMAND_RESET)
				{
					strncpy(lmi.display.detector[CMD_Detector].properties.info.user_id_number, DEFAULT_DETECTOR_USER_ID_NUMBER, LENGTH_USER_ID_NUMBER);
				}
				else
				{
					Write_EEPROM = FALSE;
				}
				Format_String(tier_command, lmi.display.detector[CMD_Detector].properties.info.user_id_number, LENGTH_USER_ID_NUMBER);		// (9DP v1.01.04)
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}

			//* Detector - User Serial Number
			sprintf(tier_command,"USN\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				strcat((char *)USB_TXMessage, message);
				strcat((char *)USB_TXMessage, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
						strncpy(lmi.display.detector[CMD_Detector].properties.info.user_serial_number, message, LENGTH_USER_SERIAL_NUMBER);
					else
						strncpy(lmi.display.detector[CMD_Detector].properties.info.user_serial_number, "\0", LENGTH_USER_SERIAL_NUMBER);
				}
				else if(process == COMMAND_RESET)
				{
					strncpy(lmi.display.detector[CMD_Detector].properties.info.user_serial_number, DEFAULT_DETECTOR_USER_SERIAL_NUMBER, LENGTH_USER_SERIAL_NUMBER);
				}
				else
				{
					Write_EEPROM = FALSE;
				}
				Format_String(tier_command, lmi.display.detector[CMD_Detector].properties.info.user_serial_number, LENGTH_USER_SERIAL_NUMBER);	// (9DP v1.01.04)
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
#ifdef DEFAULT_AUTORATE
            // Detector - AutoRate
            sprintf(tier_command, "AUR\0");
            response = strcmp(message, tier_command);
            if(response == 0) {
                HandleProperty(tier_command, message, process, writeAutoRate, nullAutoRate, resetAutoRate, &lmi.display.detector[CMD_Detector].properties.info.autoRate);
            }
#endif

#ifdef DEFAULT_FIXEDRATE
            // Detector - FixedRate
            sprintf(tier_command, "FXR\0");
            response = strcmp(message, tier_command);
            if(response == 0) {
                HandleProperty(tier_command, message, process, writeFixedRate, nullFixedRate, resetFixedRate, &lmi.display.detector[CMD_Detector].properties.info.fixedRate);
            }
#endif
            
#endif // INSTRUMENT_DETECTOR
	
			//* Detector - Calibration Technician
			sprintf(tier_command,"TEC\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				strcat((char *)USB_TXMessage, message);
				strcat((char *)USB_TXMessage, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
						strncpy(lmi.display.detector[CMD_Detector].properties.info.technician, message, LENGTH_CALIBRATION_TECHNICIAN);
					else
						strncpy(lmi.display.detector[CMD_Detector].properties.info.technician, "\0", LENGTH_CALIBRATION_TECHNICIAN);
				}
				else if(process == COMMAND_RESET)
				{
					strncpy(lmi.display.detector[CMD_Detector].properties.info.technician, DEFAULT_CALIBRATION_TECHNICIAN, LENGTH_CALIBRATION_TECHNICIAN);
				}
				else
				{
					Write_EEPROM = FALSE;
				}
				Format_String(tier_command, lmi.display.detector[CMD_Detector].properties.info.technician, LENGTH_CALIBRATION_TECHNICIAN);	// (9DP v1.01.04)
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}
	}
	
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Detector_Calibration_Handler(void)
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
//*
//*		INS:DSP:DET:00x:CAL:GET:Rxx
//*		INS:DSP:DET:00x:CAL:GET:MOx
//*		INS:DSP:DET:00x:CAL:GET:EOS
//*		INS:DSP:DET:00x:CAL:GET:Uxx
//*		INS:DSP:DET:00x:CAL:GET:Sxx
//*		INS:DSP:DET:00x:CAL:GET:Fxx
//*		INS:DSP:DET:00x:CAL:SET:Rxx
//*		INS:DSP:DET:00x:CAL:SET:MOx
//*		INS:DSP:DET:00x:CAL:SET:EOS
//*		INS:DSP:DET:00x:CAL:SET:Uxx
//*		INS:DSP:DET:00x:CAL:SET:Sxx
//*		INS:DSP:DET:00x:CAL:SET:Fxx
//*		INS:DSP:DET:00x:CAL:RES:Rxx
//*		INS:DSP:DET:00x:CAL:RES:MOx
//*		INS:DSP:DET:00x:CAL:RES:EOS
//*		INS:DSP:DET:00x:CAL:RES:Uxx
//*		INS:DSP:DET:00x:CAL:RES:Sxx
//*		INS:DSP:DET:00x:CAL:RES:Fxx
//*

BOOL Detector_Calibration_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE value;
	BYTE function;
	BYTE length;
	BYTE index;
	INT16 int16_value;
	UINT16 uint16_value;
	UINT16 calibration_constant;
	float float_value;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{
#ifdef INSTRUMENT_9D
			// Detector - Calibration - Range
			sprintf(tier_command, "R0\0");
			response = strncmp(message,tier_command,2);
			if(response == 0)
			{
				strcat((char *)USB_TXMessage, message);
				strcat((char *)USB_TXMessage, ColonSeparator);
				message[0] = '0';					
				index = atoi(message) - 1;
				if(index > 5)
				{
					return FALSE;
				}
				else if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						calibration_constant = atoi(message);
						lmi.display.detector[CMD_Detector].calibration.info.u_arg[index] = calibration_constant;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].calibration.info.u_arg[index] = DEFAULT_RANGE_CAL_CONSTANT;
				}
				else
				{
					Write_EEPROM = FALSE;
				}
				if(Write_EEPROM)																						// (9DP v1.01.07)
				{																										// (9DP v1.01.07)
					StructureUpdate(3, CMD_Detector);																	// added detector (9DP v1.01.14)
				}																										// (9DP v1.01.07)
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].calibration.info.u_arg[index]);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
			
			// Detector - Calibration - Meter Offset x
			sprintf(tier_command, "MO\0");
			response = strncmp(message,tier_command,2);
			if(response == 0)
			{
				strcat((char *)USB_TXMessage, message);
				strcat((char *)USB_TXMessage, ColonSeparator);
				message[0] = '0';					
				message[1] = '0';					
				index = atoi(message) - 1;
				if(index > 2)
				{
					return FALSE;
				}
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						uint16_value = atoi(message);
						lmi.display.detector[CMD_Detector].calibration.info.u_arg[6 + index] = uint16_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].calibration.info.u_arg[6 + index] = DEFAULT_METER_OFFSET;
				}
				else
				{
					Write_EEPROM = FALSE;
				}
				if(Write_EEPROM)																						// (9DP v1.01.07)
				{																										// (9DP v1.01.07)
					StructureUpdate(4, CMD_Detector);																	// added detector (9DP v1.01.14)
					HVBUpdateOffset(TRUE);																				// (9DP v1.01.07)
				}																										// (9DP v1.01.07)
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].calibration.info.u_arg[6 + index]);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
			
			sprintf(tier_command, "EOS\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				strcat((char *)USB_TXMessage, message);
				strcat((char *)USB_TXMessage, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						uint16_value = atoi(message);
						lmi.display.detector[CMD_Detector].calibration.info.u_arg[9] = uint16_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].calibration.info.u_arg[9] = DEFAULT_ELECTROMETER_OFFSET;
				}
				else
				{
					Write_EEPROM = FALSE;
				}
				if(Write_EEPROM)																						// (9DP v1.01.07)
				{																										// (9DP v1.01.07)
					StructureUpdate(4, CMD_Detector);																	// added detector (9DP v1.01.14)
					HVBUpdateOffset(FALSE);																				// (9DP v1.01.07)
				}																										// (9DP v1.01.07)
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].calibration.info.u_arg[9]);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}

			sprintf(tier_command, "TEM\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				strcat((char *)USB_TXMessage, message);
				strcat((char *)USB_TXMessage, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						float_value = atof(message);
						lmi.display.detector[CMD_Detector].calibration.info.f_arg[0] = float_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].calibration.info.f_arg[0] = DEFAULT_ELECTROMETER_TEMPERATURE;
				}
				else
				{
					Write_EEPROM = FALSE;
				}
				if(Write_EEPROM)																						// (9DP v1.01.07)
				{																										// (9DP v1.01.07)
					StructureUpdate(5, CMD_Detector);																	// added detector (9DP v1.01.14)
				}																										// (9DP v1.01.07)
				sprintf((char *)tier_command,"%.1f\r\n\0", lmi.display.detector[CMD_Detector].calibration.info.f_arg[0]);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}

			sprintf(tier_command, "TOC\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				strcat((char *)USB_TXMessage, message);
				strcat((char *)USB_TXMessage, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						float_value = atof(message);
						lmi.display.detector[CMD_Detector].calibration.info.f_arg[1] = float_value;
						HVBUpdateOffset(FALSE);
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].calibration.info.f_arg[1] = DEFAULT_TEMPERATURE_OFFSET_CORRECTION;
					HVBUpdateOffset(FALSE);
				}
				else
				{
					Write_EEPROM = FALSE;
				}
				if(Write_EEPROM)																						// (9DP v1.01.07)
				{																										// (9DP v1.01.07)
					StructureUpdate(5, CMD_Detector);																	// added detector (9DP v1.01.14)
				}																										// (9DP v1.01.07)
				sprintf((char *)tier_command,"%.3f\r\n\0", lmi.display.detector[CMD_Detector].calibration.info.f_arg[1]);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}

			sprintf(tier_command, "TOH\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				strcat((char *)USB_TXMessage, message);
				strcat((char *)USB_TXMessage, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						float_value = atof(message);
						lmi.display.detector[CMD_Detector].calibration.info.f_arg[2] = float_value;
						HVBUpdateOffset(FALSE);
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].calibration.info.f_arg[2] = DEFAULT_TEMPERATURE_OFFSET_CORRECTION;
					HVBUpdateOffset(FALSE);
				}
				else
				{
					Write_EEPROM = FALSE;
				}
				if(Write_EEPROM)																						// (9DP v1.01.07)
				{																										// (9DP v1.01.07)
					StructureUpdate(5, CMD_Detector);																	// added detector (9DP v1.01.14)
				}																										// (9DP v1.01.07)
				sprintf((char *)tier_command,"%.3f\r\n\0", lmi.display.detector[CMD_Detector].calibration.info.f_arg[2]);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}

//**************************************************************************************************
//* The following was added by 9DP v1.02.07

			sprintf(tier_command, "T10\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				strcat((char *)USB_TXMessage, message);
				strcat((char *)USB_TXMessage, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						float_value = atof(message);
						lmi.display.detector[CMD_Detector].calibration.info.f_arg[3] = float_value;
						HVBUpdateOffset(FALSE);
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].calibration.info.f_arg[3] = DEFAULT_TEMPERATURE_OFFSET_CORRECTION;
					HVBUpdateOffset(FALSE);
				}
				else
				{
					Write_EEPROM = FALSE;
				}
				if(Write_EEPROM)																						
				{																										
					StructureUpdate(5, CMD_Detector);																	
				}																										
				sprintf((char *)tier_command,"%.3f\r\n\0", lmi.display.detector[CMD_Detector].calibration.info.f_arg[3]);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}

			sprintf(tier_command, "T15\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				strcat((char *)USB_TXMessage, message);
				strcat((char *)USB_TXMessage, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						float_value = atof(message);
						lmi.display.detector[CMD_Detector].calibration.info.f_arg[4] = float_value;
						HVBUpdateOffset(FALSE);
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].calibration.info.f_arg[4] = DEFAULT_TEMPERATURE_OFFSET_CORRECTION;
					HVBUpdateOffset(FALSE);
				}
				else
				{
					Write_EEPROM = FALSE;
				}
				if(Write_EEPROM)																						
				{																										
					StructureUpdate(5, CMD_Detector);																	
				}																										
				sprintf((char *)tier_command,"%.3f\r\n\0", lmi.display.detector[CMD_Detector].calibration.info.f_arg[4]);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}

			sprintf(tier_command, "T20\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				strcat((char *)USB_TXMessage, message);
				strcat((char *)USB_TXMessage, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						float_value = atof(message);
						lmi.display.detector[CMD_Detector].calibration.info.f_arg[5] = float_value;
						HVBUpdateOffset(FALSE);
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].calibration.info.f_arg[5] = DEFAULT_TEMPERATURE_OFFSET_CORRECTION;
					HVBUpdateOffset(FALSE);
				}
				else
				{
					Write_EEPROM = FALSE;
				}
				if(Write_EEPROM)																						
				{																										
					StructureUpdate(5, CMD_Detector);																	
				}																										
				sprintf((char *)tier_command,"%.3f\r\n\0", lmi.display.detector[CMD_Detector].calibration.info.f_arg[5]);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}

//*	End add 9DP v1.02.07
//**************************************************************************************************

#else
			// Detector - Calibration - Unsigned Integer
			sprintf(tier_command, "U\0");
			response = strncmp(message,tier_command,1);
			if(response == 0)
			{
				strcat((char *)USB_TXMessage, message);
				strcat((char *)USB_TXMessage, ColonSeparator);
				message[0] = '0';					
				index = atoi(message);
				if(index > 15)
				{
					return FALSE;
				}
				else if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						uint16_value = (UINT16) atoi(message);
						lmi.display.detector[CMD_Detector].calibration.info.u_arg[index] = uint16_value;
						HVBInit(TRUE);																					// (3D DEBUG!!!)
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].calibration.info.u_arg[index] = 0;
				}
				else
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].calibration.info.u_arg[index]);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
			
			// Detector - Calibration - Signed Integer
			sprintf(tier_command, "S\0");
			response = strncmp(message,tier_command,1);
			if(response == 0)
			{
				strcat((char *)USB_TXMessage, message);
				strcat((char *)USB_TXMessage, ColonSeparator);
				message[0] = '0';					
				index = atoi(message);
				if(index > 15)
				{
					return FALSE;
				}
				else if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						int16_value = atoi(message);
						lmi.display.detector[CMD_Detector].calibration.info.s_arg[index] = int16_value;
						HVBInit(TRUE);																					// (3D DEBUG!!!)
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].calibration.info.s_arg[index] = 0;
				}
				else
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].calibration.info.s_arg[index]);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
			
			// Detector - Calibration - Float
			sprintf(tier_command, "F\0");
			response = strncmp(message,tier_command,1);
			if(response == 0)
			{
				strcat((char *)USB_TXMessage, message);
				strcat((char *)USB_TXMessage, ColonSeparator);
				message[0] = '0';					
				index = atoi(message);
				if(index > 7)
				{
					return FALSE;
				}
				else if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						float_value = atof(message);
						lmi.display.detector[CMD_Detector].calibration.info.f_arg[index] = float_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].calibration.info.f_arg[index] = 0.000;
				}
				else
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%.3f\r\n\0", lmi.display.detector[CMD_Detector].calibration.info.f_arg[index]);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
#endif
		}	
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Detector_Views_Handler(void)
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
//*
//*		INS:DSP:DET:00x:VSP:VPR:
//*		INS:DSP:DET:00x:VSP:VIE:00x:APP:
//*		INS:DSP:DET:00x:VSP:VIE:00x:ELE:
//*		INS:DSP:DET:00x:VSP:VIE:00x:ELS:
//*		INS:DSP:DET:00x:VSP:VIE:00x:FUN:
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:
//*		INS:DSP:DET:00x:VSP:VIE:00x:CAL:
//*

BOOL Detector_Views_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE value;
	BYTE function;
	BYTE view;
	BOOL status;
	UINT16 address;

	message = Parse_Message(NULL);
	if(message != NULL)
	{
		sprintf(tier_command, "VPR\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Views_Properties_Handler();
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				address = EEPROM_DETECTOR_VIEW_SETUP_LOCATION + (CMD_Detector * TOTAL_DETECTOR);
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, lmi.display.detector[CMD_Detector].view_setup.buffer, SIZE_DETECTOR_VIEW_SETUP);						// (9DP v1.01.13)
			}
			return status;
		}
		
		sprintf(tier_command, "VIE\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			message = Parse_Message(NULL);
			if(message != NULL)
			{
				view = (BYTE) atoi(message) - 1;
				if(view < NUMBER_OF_VIEWS)
				{
					sprintf((char *)tier_command,"%03d\0",(view + 1));
					Append_Message(tier_command, ColonSeparator);
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						//* Views - Appearance
						sprintf(tier_command,"APP\0");
						response = strcmp(message,tier_command);
						if(response == 0)
						{
							Append_Message(tier_command, ColonSeparator);
							status = Views_Appearance_Handler(view);
							if((status == TRUE) && (Write_EEPROM == TRUE))
							{
								address = EEPROM_APPEARANCE_LOCATION + (CMD_Detector * TOTAL_DETECTOR) + (view * TOTAL_VIEW);
								i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, lmi.display.detector[CMD_Detector].view[view].appearance.buffer, SIZE_APPEARANCE);
							}
							return status;
						}
					
						//* Views - Element Header
						sprintf(tier_command,"ELE\0");
						response = strcmp(message,tier_command);
						if(response == 0)
						{
							Append_Message(tier_command, ColonSeparator);
							status = Views_Element_Header_Handler(view);
							if((status == TRUE) && (Write_EEPROM == TRUE))
							{
								address = EEPROM_ELEMENT_HEADER_LOCATION + (CMD_Detector * TOTAL_DETECTOR) + (view * TOTAL_VIEW);
								i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, lmi.display.detector[CMD_Detector].view[view].element_header.buffer, SIZE_ELEMENT_HEADER);
								Force_Screen_Redraw();																													// (9DP v1.01.20)
							}
							return status;
						}
					
						//* Views - Element Style
						sprintf(tier_command,"ELS\0");
						response = strcmp(message,tier_command);
						if(response == 0)
						{
							Append_Message(tier_command, ColonSeparator);
							status = Views_Element_Style_Handler(view);
							if((status == TRUE) && (Write_EEPROM == TRUE))
							{
								address = EEPROM_ELEMENT_LOCATION + (CMD_Detector * TOTAL_DETECTOR) + (view * TOTAL_VIEW);
								i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, lmi.display.detector[CMD_Detector].view[view].element.buffer, SIZE_ELEMENT);
								Force_Screen_Redraw();																													// (9DP v1.01.20)
							}
							return status;
						}
					
						//* Views - Function
						sprintf(tier_command,"FUN\0");
						response = strcmp(message,tier_command);
						if(response == 0)
						{
							Append_Message(tier_command, ColonSeparator);
							status = Views_Functions_Handler(view);
							if((status == TRUE) && (Write_EEPROM == TRUE))
							{
								address = EEPROM_VIEW_FUNCTIONS_LOCATION + (CMD_Detector * TOTAL_DETECTOR) + (view * TOTAL_VIEW);
								i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, lmi.display.detector[CMD_Detector].view[view].functions.buffer, SIZE_VIEW_FUNCTIONS);	// (9DP v1.01.13)
								Force_Screen_Redraw();																													// (9DP v1.01.20)
							}
							return status;
						}
					
						//* Views - Alerts
						sprintf(tier_command,"ALE\0");
						response = strcmp(message,tier_command);
						if(response == 0)
						{
							Append_Message(tier_command, ColonSeparator);
							return Views_Alerts_Handler(view);
						}
			
						//* Views - Alarms
						sprintf(tier_command,"ALA\0");
						response = strcmp(message,tier_command);
						if(response == 0)
						{
							Append_Message(tier_command, ColonSeparator);
							return Views_Alarms_Handler(view);
						}

#ifdef EEPROM_VIEW_CALIBRATION_LOCATION
						//* Views - Calibration
						sprintf(tier_command,"CAL\0");
						response = strcmp(message,tier_command);
						if(response == 0)
						{
							Append_Message(tier_command, ColonSeparator);
							status = Views_Calibration_Handler(view);
							if((status == TRUE) && (Write_EEPROM == TRUE))
							{
								address = EEPROM_VIEW_CALIBRATION_LOCATION + (CMD_Detector * TOTAL_DETECTOR) + (view * TOTAL_VIEW);
								i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, lmi.display.detector[CMD_Detector].view[view].calibration.buffer, SIZE_VIEW_CALIBRATION);
							}
							return status;
						}
#endif
					}
				}
			}
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Views_Properties_Handler(void)
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
//*
//*		INS:DSP:DET:00x:VSP:VPR:GET:PUM
//*		INS:DSP:DET:00x:VSP:VPR:GET:VCU
//*		INS:DSP:DET:00x:VSP:VPR:SET:PUM
//*		INS:DSP:DET:00x:VSP:VPR:SET:VCU
//*		INS:DSP:DET:00x:VSP:VPR:RES:PUM
//*		INS:DSP:DET:00x:VSP:VPR:RES:VCU
//*

BOOL Views_Properties_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE value;
	BYTE function;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{	
			//* Views - Powerup Mode
			sprintf(tier_command,"PUM\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = (BYTE) atoi(message);
						if(!Event_Handler(value, END_VIEW_MODES, &lmi.display.detector[CMD_Detector].view_setup.info.powerup_mode))
							return FALSE;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].view_setup.info.powerup_mode = DEFAULT_VIEWS_POWERUP_MODE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view_setup.info.powerup_mode);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Views - Current View
			sprintf(tier_command,"CVI\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = (BYTE) atoi(message) - 1;
						if(!Event_Handler(value, NUMBER_OF_VIEWS, &lmi.display.detector[CMD_Detector].view_setup.info.current_view))
							return FALSE;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].view_setup.info.current_view = DEFAULT_CURRENT_VIEW;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				if(process != COMMAND_READ)
				{
					Current_View = lmi.display.detector[CMD_Detector].view_setup.info.current_view;
					UpdateFunctionVariables();
					Force_Screen_Redraw();																													// (9DP v1.01.20)
//					if(screenState == MONITOR_RADIATION)
//					{
//						screenState = CREATE_ELEMENT;
//						DisplayDetachOnce = TRUE;
//					}
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view_setup.info.current_view + 1);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Views_Appearance_Handler(BYTE view)
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
//*
//*		INS:DSP:DET:00x:VSP:VIE:00x:APP:GET:BGC
//*		INS:DSP:DET:00x:VSP:VIE:00x:APP:GET:FGC
//*		INS:DSP:DET:00x:VSP:VIE:00x:APP:GET:DST
//*		INS:DSP:DET:00x:VSP:VIE:00x:APP:GET:DSV
//*		INS:DSP:DET:00x:VSP:VIE:00x:APP:SET:BGC
//*		INS:DSP:DET:00x:VSP:VIE:00x:APP:SET:FGC
//*		INS:DSP:DET:00x:VSP:VIE:00x:APP:SET:DST
//*		INS:DSP:DET:00x:VSP:VIE:00x:APP:SET:DSV
//*

BOOL Views_Appearance_Handler(BYTE view)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE value;
	BYTE function;

	process = Process_Handler();																													// get process
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);																												// get message
		if(message != NULL)
		{	
			//* Views - Appearance - Background Color
			sprintf(tier_command,"BGC\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = (BYTE) atoi(message);
						lmi.display.detector[CMD_Detector].view[view].appearance.info.background_color = value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].view[view].appearance.info.background_color = WHITE_EEPROM;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				if(process != COMMAND_READ)
				{
					if(screenState == MONITOR_RADIATION)
					{
						background = Designate_Color(lmi.display.detector[CMD_Detector].view[view].appearance.info.background_color);
						screenState = CREATE_ELEMENT;
						DisplayDetachOnce = TRUE;
					}
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].appearance.info.background_color);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Views - Appearance - Foreground Color
			sprintf(tier_command,"FGC\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = (BYTE) atoi(message);
						lmi.display.detector[CMD_Detector].view[view].appearance.info.foreground_color = value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].view[view].appearance.info.foreground_color = BRIGHTBLUE_EEPROM;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				if(process != COMMAND_READ)
				{
					if(screenState == MONITOR_RADIATION)
					{
						foreground = Designate_Color(lmi.display.detector[CMD_Detector].view[view].appearance.info.foreground_color);
						screenState = CREATE_ELEMENT;
						DisplayDetachOnce = TRUE;
					}
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].appearance.info.foreground_color);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Views - Appearance - Display Time
			sprintf(tier_command,"DST\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = (BYTE) atoi(message);
						lmi.display.detector[CMD_Detector].view[view].appearance.info.display_time = value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].view[view].appearance.info.display_time = DEFAULT_APPEARANCE_DISPLAY_TIME;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].appearance.info.display_time);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Views - Appearance - Display View Name
			sprintf(tier_command,"DSV\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = (BYTE) atoi(message);
						lmi.display.detector[CMD_Detector].view[view].appearance.info.display_view_name = value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].view[view].appearance.info.display_view_name = DEFAULT_APPEARANCE_DISPLAY_VIEW_NAME;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].appearance.info.display_view_name);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Views_Element_Header_Handler(BYTE view)
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
//*
//*		INS:DSP:DET:00x:VSP:VIE:00x:ELE:GET:STY
//*		INS:DSP:DET:00x:VSP:VIE:00x:ELE:GET:NAM
//*		INS:DSP:DET:00x:VSP:VIE:00x:ELE:SET:STY
//*		INS:DSP:DET:00x:VSP:VIE:00x:ELE:SET:NAM
//*

BOOL Views_Element_Header_Handler(BYTE view)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE value;
	BYTE function;

	process = Process_Handler();																													// get process
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);																												// get message
		if(message != NULL)
		{	
			//* Views - Element - Style
			sprintf(tier_command,"STY\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = (BYTE) atoi(message);
						if(value <= ARC)																											// (9DP v1.01.13)
						{
							lmi.display.detector[CMD_Detector].view[view].element_header.info.style = value;
						}
						else
							return FALSE;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					switch(view)
					{
						case VIEW_1:
						{
							lmi.display.detector[CMD_Detector].view[view].element_header.info.style = DEFAULT_ELEMENT_STYLE_1;
							break;
						}
						case VIEW_2:
						{
							lmi.display.detector[CMD_Detector].view[view].element_header.info.style = DEFAULT_ELEMENT_STYLE_2;
							break;
						}
						case VIEW_3:
						{
							lmi.display.detector[CMD_Detector].view[view].element_header.info.style = DEFAULT_ELEMENT_STYLE_3;
							break;
						}
						case VIEW_4:
						{
							lmi.display.detector[CMD_Detector].view[view].element_header.info.style = DEFAULT_ELEMENT_STYLE_4;
							break;
						}	
					}
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				if(process != COMMAND_READ)
				{
					Force_Screen_Redraw();																											// (9DP v1.01.20)
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].element_header.info.style);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Views - Element Header - Name
			sprintf(tier_command,"NAM\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
						strncpy(lmi.display.detector[CMD_Detector].view[view].element_header.info.name, message, SIZE_ELEMENT_HEADER_NAME);
					else
						strncpy(lmi.display.detector[CMD_Detector].view[view].element_header.info.name, DEFAULT_BLANK_STRING, SIZE_ELEMENT_HEADER_NAME);
					
				}
				else if(process == COMMAND_RESET)
				{
					switch(view)
					{
						case VIEW_1:
						{
							strncpy(lmi.display.detector[CMD_Detector].view[view].element_header.info.name, DEFAULT_ELEMENT_NAME_1, SIZE_ELEMENT_HEADER_NAME);
							break;
						}
						case VIEW_2:
						{
							strncpy(lmi.display.detector[CMD_Detector].view[view].element_header.info.name, DEFAULT_ELEMENT_NAME_2, SIZE_ELEMENT_HEADER_NAME);
							break;
						}
						case VIEW_3:
						{
							strncpy(lmi.display.detector[CMD_Detector].view[view].element_header.info.name, DEFAULT_ELEMENT_NAME_3, SIZE_ELEMENT_HEADER_NAME);
							break;
						}
						case VIEW_4:
						{
							strncpy(lmi.display.detector[CMD_Detector].view[view].element_header.info.name, DEFAULT_ELEMENT_NAME_4, SIZE_ELEMENT_HEADER_NAME);
							break;
						}
					}
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				Format_String(tier_command, lmi.display.detector[CMD_Detector].view[view].element_header.info.name, SIZE_ELEMENT_HEADER_NAME);	// (9DP v1.01.04)
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Views_Element_Style_Handler(BYTE view)
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
//*
//*		INS:DSP:DET:00x:VSP:VIE:00x:ELS:001:
//*

BOOL Views_Element_Style_Handler(BYTE view)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE style;

	message = Parse_Message(NULL);																	// read style number (should be 001)
	if(message != NULL)
	{
		style = (BYTE) atoi(message);
		
		switch(style)
		{
			case ARC:
			{
				sprintf(tier_command,"%03d:\0", style);
				strcat((char *)USB_TXMessage, tier_command);
				return Views_Element_Style_Arc_Handler(view);
			}
			default:
			{
				return FALSE;
			}
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Views_Element_Style_Arc_Handler(BYTE view)
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
//*
//*		INS:DSP:DET:00x:VSP:VIE:00x:ELS:001:GET:TOS
//*		INS:DSP:DET:00x:VSP:VIE:00x:ELS:001:GET:NOD
//*		INS:DSP:DET:00x:VSP:VIE:00x:ELS:001:GET:NOR
//*		INS:DSP:DET:00x:VSP:VIE:00x:ELS:001:GET:EPT
//*		INS:DSP:DET:00x:VSP:VIE:00x:ELS:001:GET:DIG
//*		INS:DSP:DET:00x:VSP:VIE:00x:ELS:001:GET:UNI
//*		INS:DSP:DET:00x:VSP:VIE:00x:ELS:001:GET:MUL
//*		INS:DSP:DET:00x:VSP:VIE:00x:ELS:001:SET:TOS
//*		INS:DSP:DET:00x:VSP:VIE:00x:ELS:001:SET:NOD
//*		INS:DSP:DET:00x:VSP:VIE:00x:ELS:001:SET:NOR
//*		INS:DSP:DET:00x:VSP:VIE:00x:ELS:001:SET:EPT
//*		INS:DSP:DET:00x:VSP:VIE:00x:ELS:001:SET:DIG
//*		INS:DSP:DET:00x:VSP:VIE:00x:ELS:001:SET:UNI
//*		INS:DSP:DET:00x:VSP:VIE:00x:ELS:001:SET:MUL
//*		INS:DSP:DET:00x:VSP:VIE:00x:ELS:001:RES:TOS
//*		INS:DSP:DET:00x:VSP:VIE:00x:ELS:001:RES:NOD
//*		INS:DSP:DET:00x:VSP:VIE:00x:ELS:001:RES:NOR
//*		INS:DSP:DET:00x:VSP:VIE:00x:ELS:001:RES:EPT
//*		INS:DSP:DET:00x:VSP:VIE:00x:ELS:001:RES:DIG
//*		INS:DSP:DET:00x:VSP:VIE:00x:ELS:001:RES:UNI
//*		INS:DSP:DET:00x:VSP:VIE:00x:ELS:001:RES:MUL
//*

BOOL Views_Element_Style_Arc_Handler(BYTE view)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE style;
	BYTE function;
	BYTE byte_value;
	UINT16 uint16_value;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{
			//* Views - Element - Style - Arc - Type of Scale
			sprintf(tier_command,"TOS\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
#ifndef INSTRUMENT_9D																																// (9DP v1.01.13)
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						if(byte_value == LINEAR)																									// (9DP v1.01.13)
							lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.type_of_scale = byte_value;
						else																														// (9DP v1.01.13)
							return FALSE;																											// (9DP v1.01.13)
					}
					else
#endif																																				// (9DP v1.01.13)
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
#ifndef INSTRUMENT_9D																																// (9DP v1.01.13)
					if(view == 0)
						lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.type_of_scale = DEFAULT_ELEMENT_1_ARC_TYPE_OF_SCALE;
					else if(view == 1)
						lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.type_of_scale = DEFAULT_ELEMENT_2_ARC_TYPE_OF_SCALE;
					else if(view == 2)
						lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.type_of_scale = DEFAULT_ELEMENT_3_ARC_TYPE_OF_SCALE;
					else if(view == 3)																												// (9DP v1.01.13)
						lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.type_of_scale = DEFAULT_ELEMENT_4_ARC_TYPE_OF_SCALE;
					else																															// (9DP v1.01.13)
#endif																																				// (9DP v1.01.13)
						return FALSE;																												// (9DP v1.01.13)
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.type_of_scale);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Views - Element - Style - Arc - Number of Divisions
			sprintf(tier_command,"NOD\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
#ifndef INSTRUMENT_9D																																// (9DP v1.01.13)
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.scale.number_of_divisions = byte_value;
					}
					else
#endif																																				// (9DP v1.01.13)
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
#ifndef INSTRUMENT_9D																																// (9DP v1.01.13)
					if(view == 0)
						lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.scale.number_of_divisions = DEFAULT_ELEMENT_1_ARC_NUMBER_OF_DIVISIONS;
					else if(view == 1)
						lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.scale.number_of_divisions = DEFAULT_ELEMENT_2_ARC_NUMBER_OF_DIVISIONS;
					else if(view == 2)
						lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.scale.number_of_divisions = DEFAULT_ELEMENT_3_ARC_NUMBER_OF_DIVISIONS;
					else if(view == 3)																												// (9DP v1.01.13)
						lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.scale.number_of_divisions = DEFAULT_ELEMENT_4_ARC_NUMBER_OF_DIVISIONS;
					else																															// (9DP v1.01.13)
#endif																																				// (9DP v1.01.13)
						return FALSE;																												// (9DP v1.01.13)
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				if(process != COMMAND_READ)
				{
					Force_Screen_Redraw();																											// (9DP v1.01.20)
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.scale.number_of_divisions);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Views - Element - Style - Arc - Number of Ranges
			sprintf(tier_command,"NOR\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
#ifndef INSTRUMENT_9D																																// (9DP v1.01.13)
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.scale.number_of_ranges = byte_value;
					}
					else
#endif																																				// (9DP v1.01.13)
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
#ifndef INSTRUMENT_9D																																// (9DP v1.01.13)
					if(view == 0)
						lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.scale.number_of_ranges = DEFAULT_ELEMENT_1_ARC_NUMBER_OF_RANGES;
					else if(view == 1)
						lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.scale.number_of_ranges = DEFAULT_ELEMENT_2_ARC_NUMBER_OF_RANGES;
					else if(view == 2)
						lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.scale.number_of_ranges = DEFAULT_ELEMENT_3_ARC_NUMBER_OF_RANGES;
					else if(view == 3)																												// (9DP v1.01.13)
						lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.scale.number_of_ranges = DEFAULT_ELEMENT_4_ARC_NUMBER_OF_RANGES;
					else																															// (9DP v1.01.13)
#endif																																				// (9DP v1.01.13)
						return FALSE;																												// (9DP v1.01.13)
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.scale.number_of_ranges);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Views - Element - Style - Arc - End Point
			sprintf(tier_command,"EPT\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
#ifndef INSTRUMENT_9D																																// (9DP v1.01.13)
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						uint16_value = atoi(message);
						lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.end_point = uint16_value;
					}
					else
#endif																																				// (9DP v1.01.13)
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
#ifndef INSTRUMENT_9D																																// (9DP v1.01.13)
					if(view == 0)
						lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.end_point = DEFAULT_ELEMENT_1_ARC_END_POINT;
					else if(view == 1)
						lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.end_point = DEFAULT_ELEMENT_2_ARC_END_POINT;
					else if(view == 2)
						lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.end_point = DEFAULT_ELEMENT_3_ARC_END_POINT;
					else if(view == 3)																												// (9DP v1.01.13)
						lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.end_point = DEFAULT_ELEMENT_1_ARC_END_POINT;
					else																															// (9DP v1.01.13)
#endif																																				// (9DP v1.01.13)
						return FALSE;																												// (9DP v1.01.13)
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				if(process != COMMAND_READ)
				{
					Force_Screen_Redraw();																											// (9DP v1.01.20)
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.end_point);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Views - Element - Style - Arc - Number of Digits
			sprintf(tier_command,"DIG\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
#ifndef INSTRUMENT_9D																																// (9DP v1.01.13)
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.digits = byte_value;
					}
					else
#endif																																				// (9DP v1.01.13)
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
#ifndef INSTRUMENT_9D																																// (9DP v1.01.13)
					if(view == 0)
						lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.digits = DEFAULT_ELEMENT_1_ARC_DIGITS;
					else if(view == 1)
						lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.digits = DEFAULT_ELEMENT_2_ARC_DIGITS;
					else if(view == 2)
						lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.digits = DEFAULT_ELEMENT_3_ARC_DIGITS;
					else if(view == 3)																												// (9DP v1.01.13)
						lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.digits = DEFAULT_ELEMENT_4_ARC_DIGITS;
					else																															// (9DP v1.01.13)
#endif																																				// (9DP v1.01.13)
						return FALSE;																												// (9DP v1.01.13)
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.digits);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Views - Element - Style - Arc - Units
			sprintf(tier_command,"UNI\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
#ifndef INSTRUMENT_9D																																// (9DP v1.01.13)
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.units = byte_value;
					}
					else
#endif																																				// (9DP v1.01.13)
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
#ifndef INSTRUMENT_9D																																// (9DP v1.01.13)
					if(view == 0)
						lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.units = DEFAULT_ELEMENT_1_ARC_UNITS;
					else if(view == 1)
						lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.units = DEFAULT_ELEMENT_2_ARC_UNITS;
					else if(view == 2)
						lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.units = DEFAULT_ELEMENT_3_ARC_UNITS;
					else if(view == 3)																												// (9DP v1.01.13)
						lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.units = DEFAULT_ELEMENT_1_ARC_UNITS;
					else																															// (9DP v1.01.13)
#endif																																				// (9DP v1.01.13)
						return FALSE;																												// (9DP v1.01.13)
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				if(process != COMMAND_READ)
				{
					Force_Screen_Redraw();																											// (9DP v1.01.20)
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.units);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Views - Element - Style - Arc - Multiplier
			sprintf(tier_command,"MUL\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
#ifndef INSTRUMENT_9D																																// (9DP v1.01.13)
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.multiplier = byte_value + MULTIPLIER_ADJUST;
					}
					else
#endif																																				// (9DP v1.01.13)
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
#ifndef INSTRUMENT_9D																																// (9DP v1.01.13)
					if(view == 0)
						lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.multiplier = DEFAULT_ELEMENT_1_ARC_MULTIPLIER;
					else if(view == 1)
						lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.multiplier = DEFAULT_ELEMENT_2_ARC_MULTIPLIER;
					else if(view == 2)
						lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.multiplier = DEFAULT_ELEMENT_3_ARC_MULTIPLIER;
					else if(view == 3)																												// (9DP v1.01.13)
						lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.multiplier = DEFAULT_ELEMENT_4_ARC_MULTIPLIER;
					else																															// (9DP v1.01.13)
#endif																																				// (9DP v1.01.13)
						return FALSE;																												// (9DP v1.01.13)
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				if(process != COMMAND_READ)
				{
					Force_Screen_Redraw();																											// (9DP v1.01.20)
				}
				sprintf((char *)tier_command,"%d\r\n\0", (lmi.display.detector[CMD_Detector].view[view].element.info.arc.control.multiplier - MULTIPLIER_ADJUST));
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Views_Functions_Handler(BYTE view)
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
//*
//*		INS:DSP:DET:00x:VSP:VIE:00x:FUN:00x:GET:SEL
//*		INS:DSP:DET:00x:VSP:VIE:00x:FUN:00x:GET:ENB
//*		INS:DSP:DET:00x:VSP:VIE:00x:FUN:00x:SET:SEL
//*		INS:DSP:DET:00x:VSP:VIE:00x:FUN:00x:SET:ENB
//*

BOOL Views_Functions_Handler(BYTE view)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE value;
	BYTE function;

	message = Parse_Message(NULL);																	// get function number
	if(message != NULL)
	{
		function = (BYTE) atoi(message) - 1;
	
		if(function >= DEFAULT_VIEW_NUMBER_OF_FUNCTIONS)
			return FALSE;
	
		sprintf((char *)tier_command,"%03d\0",(function + 1));
		Append_Message(tier_command, ColonSeparator);
		process = Process_Handler();																// get process
		if(process != COMMAND_ERROR)
		{
			message = Parse_Message(NULL);															// get message
			if(message != NULL)
			{
				//* Views - Functions Selected Number
				sprintf(tier_command,"SEL\0");
				response = strcmp(message,tier_command);
				if(response == 0)
				{
					Append_Message(tier_command, ColonSeparator);
					if(process == COMMAND_WRITE)
					{
						message = Parse_Message(NULL);
						if(message != NULL)
						{
							value = (BYTE) atoi(message) - 1;
							lmi.display.detector[CMD_Detector].view[view].functions.info.function[function].function_index = value;
						}
						else
							return FALSE;
					}
					else if(process == COMMAND_RESET)
					{
						switch(view)
						{
							case VIEW_1:
							{
								if(function == 0)
									lmi.display.detector[CMD_Detector].view[view].functions.info.function[function].function_index = DEFAULT_VIEW_1_FUNCTION_1_NUMBER;
								else if(function == 1)
									lmi.display.detector[CMD_Detector].view[view].functions.info.function[function].function_index = DEFAULT_VIEW_1_FUNCTION_2_NUMBER;
								else if(function == 2)
									lmi.display.detector[CMD_Detector].view[view].functions.info.function[function].function_index = DEFAULT_VIEW_1_FUNCTION_3_NUMBER;
								else if(function == 3)
									lmi.display.detector[CMD_Detector].view[view].functions.info.function[function].function_index = DEFAULT_VIEW_1_FUNCTION_4_NUMBER;
								else
									return FALSE;
								break;
							}
							case VIEW_2:
							{
								if(function == 0)
									lmi.display.detector[CMD_Detector].view[view].functions.info.function[function].function_index = DEFAULT_VIEW_2_FUNCTION_1_NUMBER;
								else if(function == 1)
									lmi.display.detector[CMD_Detector].view[view].functions.info.function[function].function_index = DEFAULT_VIEW_2_FUNCTION_2_NUMBER;
								else if(function == 2)
									lmi.display.detector[CMD_Detector].view[view].functions.info.function[function].function_index = DEFAULT_VIEW_2_FUNCTION_3_NUMBER;
								else if(function == 3)
									lmi.display.detector[CMD_Detector].view[view].functions.info.function[function].function_index = DEFAULT_VIEW_2_FUNCTION_4_NUMBER;
								else
									return FALSE;
								break;
							}
							case VIEW_3:
							{
								if(function == 0)
									lmi.display.detector[CMD_Detector].view[view].functions.info.function[function].function_index = DEFAULT_VIEW_3_FUNCTION_1_NUMBER;
								else if(function == 1)
									lmi.display.detector[CMD_Detector].view[view].functions.info.function[function].function_index = DEFAULT_VIEW_3_FUNCTION_2_NUMBER;
								else if(function == 2)
									lmi.display.detector[CMD_Detector].view[view].functions.info.function[function].function_index = DEFAULT_VIEW_3_FUNCTION_3_NUMBER;
								else if(function == 3)
									lmi.display.detector[CMD_Detector].view[view].functions.info.function[function].function_index = DEFAULT_VIEW_3_FUNCTION_4_NUMBER;
								else
									return FALSE;
								break;
							}
							case VIEW_4:
							{
								if(function == 0)
									lmi.display.detector[CMD_Detector].view[view].functions.info.function[function].function_index = DEFAULT_VIEW_4_FUNCTION_1_NUMBER;
								else if(function == 1)
									lmi.display.detector[CMD_Detector].view[view].functions.info.function[function].function_index = DEFAULT_VIEW_4_FUNCTION_2_NUMBER;
								else if(function == 2)
									lmi.display.detector[CMD_Detector].view[view].functions.info.function[function].function_index = DEFAULT_VIEW_4_FUNCTION_3_NUMBER;
								else if(function == 3)
									lmi.display.detector[CMD_Detector].view[view].functions.info.function[function].function_index = DEFAULT_VIEW_4_FUNCTION_4_NUMBER;
								else
									return FALSE;
								break;
							}
							default:
							{
								return FALSE;
							}
						}
					}
					else if(process == COMMAND_READ)
					{
						Write_EEPROM = FALSE;
					}
					sprintf((char *)tier_command,"%d\r\n\0",(lmi.display.detector[CMD_Detector].view[view].functions.info.function[function].function_index + 1)%256);
					strcat((char *)USB_TXMessage, tier_command);
					return TRUE;
				}
			
				//* Views - Function Enabled
				sprintf(tier_command,"ENB\0");
				response = strcmp(message,tier_command);
				if(response == 0)
				{
					Append_Message(tier_command, ColonSeparator);
					if(process == COMMAND_WRITE)
					{
						message = Parse_Message(NULL);
						if(message != NULL)
						{
							value = (BYTE) atoi(message);
							lmi.display.detector[CMD_Detector].view[view].functions.info.function[function].enable = value;
						}
						else
							return FALSE;
					}
					else if(process == COMMAND_RESET)
					{
						switch(view)
						{
							case VIEW_1:
							{
								if(function == 0)
									lmi.display.detector[CMD_Detector].view[view].functions.info.function[function].enable = DEFAULT_VIEW_1_FUNCTION_1_ENABLE;
								else if(function == 1)
									lmi.display.detector[CMD_Detector].view[view].functions.info.function[function].enable = DEFAULT_VIEW_1_FUNCTION_2_ENABLE;
								else if(function == 2)
									lmi.display.detector[CMD_Detector].view[view].functions.info.function[function].enable = DEFAULT_VIEW_1_FUNCTION_3_ENABLE;
								else if(function == 3)
									lmi.display.detector[CMD_Detector].view[view].functions.info.function[function].enable = DEFAULT_VIEW_1_FUNCTION_4_ENABLE;
								else
									return FALSE;
								break;
							}
							case VIEW_2:
							{
								if(function == 0)
									lmi.display.detector[CMD_Detector].view[view].functions.info.function[function].enable = DEFAULT_VIEW_2_FUNCTION_1_ENABLE;
								else if(function == 1)
									lmi.display.detector[CMD_Detector].view[view].functions.info.function[function].enable = DEFAULT_VIEW_2_FUNCTION_2_ENABLE;
								else if(function == 2)
									lmi.display.detector[CMD_Detector].view[view].functions.info.function[function].enable = DEFAULT_VIEW_2_FUNCTION_3_ENABLE;
								else if(function == 3)
									lmi.display.detector[CMD_Detector].view[view].functions.info.function[function].enable = DEFAULT_VIEW_2_FUNCTION_4_ENABLE;
								else
									return FALSE;
								break;
							}
							case VIEW_3:
							{
								if(function == 0)
									lmi.display.detector[CMD_Detector].view[view].functions.info.function[function].enable = DEFAULT_VIEW_3_FUNCTION_1_ENABLE;
								else if(function == 1)
									lmi.display.detector[CMD_Detector].view[view].functions.info.function[function].enable = DEFAULT_VIEW_3_FUNCTION_2_ENABLE;
								else if(function == 2)
									lmi.display.detector[CMD_Detector].view[view].functions.info.function[function].enable = DEFAULT_VIEW_3_FUNCTION_3_ENABLE;
								else if(function == 3)
									lmi.display.detector[CMD_Detector].view[view].functions.info.function[function].enable = DEFAULT_VIEW_3_FUNCTION_4_ENABLE;
								else
									return FALSE;
								break;
							}
							case VIEW_4:
							{
								if(function == 0)
									lmi.display.detector[CMD_Detector].view[view].functions.info.function[function].enable = DEFAULT_VIEW_4_FUNCTION_1_ENABLE;
								else if(function == 1)
									lmi.display.detector[CMD_Detector].view[view].functions.info.function[function].enable = DEFAULT_VIEW_4_FUNCTION_2_ENABLE;
								else if(function == 2)
									lmi.display.detector[CMD_Detector].view[view].functions.info.function[function].enable = DEFAULT_VIEW_4_FUNCTION_3_ENABLE;
								else if(function == 3)
									lmi.display.detector[CMD_Detector].view[view].functions.info.function[function].enable = DEFAULT_VIEW_4_FUNCTION_4_ENABLE;
								else
									return FALSE;
								break;
							}
							default:
							{
								return FALSE;
							}
						}
					}
					else if(process == COMMAND_READ)
					{
						Write_EEPROM = FALSE;
					}
					sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].functions.info.function[function].enable);
					strcat((char *)USB_TXMessage, tier_command);
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Views_Alerts_Handler(BYTE view)
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
//*
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:RAD
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:INT
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:FUN
//*

BOOL Views_Alerts_Handler(BYTE view)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	UINT16 uint16_value;
	unsigned short address;
	BYTE response = 0;
	BYTE process = 0;
	BYTE byte_value;
	BOOL status;

	message = Parse_Message(NULL);
	if(message != NULL)
	{
		//* Alerts - Radiation
		sprintf(tier_command,"RAD\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Views_Alerts_Radiation_Handler(view);
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				address = EEPROM_ALERTS_RADIATION_LOCATION + (CMD_Detector * TOTAL_DETECTOR) + (view * TOTAL_VIEW);
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, lmi.display.detector[CMD_Detector].view[view].alerts_radiation.buffer, SIZE_ALERTS_RADIATION);
			}
			return status;
		}
	
		//* Alerts - Integrate
		sprintf(tier_command,"INT\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Views_Alerts_Integrate_Handler(view);
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				address = EEPROM_ALERTS_INTEGRATE_LOCATION + (CMD_Detector * TOTAL_DETECTOR) + (view * TOTAL_VIEW);
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, lmi.display.detector[CMD_Detector].view[view].alerts_integrate.buffer, SIZE_ALERTS_INTEGRATE);
			}
			return status;
		}
	
		//* Alerts - Function
		sprintf(tier_command,"FUN\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Views_Alerts_Function_Handler(view);
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				address = EEPROM_ALERTS_FUNCTION_LOCATION + (CMD_Detector * TOTAL_DETECTOR) + (view * TOTAL_VIEW);
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, lmi.display.detector[CMD_Detector].view[view].alerts_function.buffer, SIZE_ALERTS_FUNCTION);
			}
			return status;
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Views_Alerts_Radiation_Handler(BYTE view)
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
//*
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:RAD:GET:ENB
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:RAD:GET:VAL
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:RAD:GET:UNI
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:RAD:GET:MUL
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:RAD:GET:CEN
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:RAD:GET:CMG
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:RAD:SET:ENB
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:RAD:SET:VAL
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:RAD:SET:UNI
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:RAD:SET:MUL
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:RAD:SET:CEN
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:RAD:SET:CMG
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:RAD:RES:ENB
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:RAD:RES:VAL
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:RAD:RES:UNI
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:RAD:RES:MUL
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:RAD:RES:CEN
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:RAD:RES:CMG
//*

BOOL Views_Alerts_Radiation_Handler(BYTE view)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE byte_value;
	UINT16 uint16_value;

	process = Process_Handler();																	
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);																
		if(message != NULL)
		{
			//* Alerts - Radiation - Enable
			sprintf(tier_command,"ENB\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.display.detector[CMD_Detector].view[view].alerts_radiation.info.feature.enable = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].view[view].alerts_radiation.info.feature.enable = DEFAULT_VIEW_RAD_ALERTS_ENABLE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				if(process != COMMAND_READ)																												// (9DP v1.01.20)
				{																																		// (9DP v1.01.20)
					Force_Screen_Redraw();																												// (9DP v1.01.20)
				}																																		// (9DP v1.01.20)
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].alerts_radiation.info.feature.enable);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alerts - Radiation - Value
			sprintf(tier_command,"VAL\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						uint16_value = atoi(message);
						lmi.display.detector[CMD_Detector].view[view].alerts_radiation.info.feature.reading.value = uint16_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].view[view].alerts_radiation.info.feature.reading.value = DEFAULT_VIEW_RAD_ALERTS_VALUE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				if(process != COMMAND_READ)
				{
					Calculate_New_Radiation_Alert(CMD_Detector, view);
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].alerts_radiation.info.feature.reading.value);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alerts - Radiation - Units
			sprintf(tier_command,"UNI\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.display.detector[CMD_Detector].view[view].alerts_radiation.info.feature.reading.units = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					if(view == 0)
						lmi.display.detector[CMD_Detector].view[view].alerts_radiation.info.feature.reading.units = DEFAULT_VIEW_1_RAD_ALERTS_UNITS;
					else if(view == 1)
						lmi.display.detector[CMD_Detector].view[view].alerts_radiation.info.feature.reading.units = DEFAULT_VIEW_2_RAD_ALERTS_UNITS;
					else if(view == 2)
						lmi.display.detector[CMD_Detector].view[view].alerts_radiation.info.feature.reading.units = DEFAULT_VIEW_3_RAD_ALERTS_UNITS;
					else
						lmi.display.detector[CMD_Detector].view[view].alerts_radiation.info.feature.reading.units = DEFAULT_VIEW_4_RAD_ALERTS_UNITS;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				if(process != COMMAND_READ)
				{
					Calculate_New_Radiation_Alert(CMD_Detector, view);
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].alerts_radiation.info.feature.reading.units);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alerts - Radiation - Multiplier
			sprintf(tier_command,"MUL\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.display.detector[CMD_Detector].view[view].alerts_radiation.info.feature.reading.multiplier = byte_value + MULTIPLIER_ADJUST;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].view[view].alerts_radiation.info.feature.reading.multiplier = DEFAULT_VIEW_RAD_ALERTS_MULTIPLIER;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				if(process != COMMAND_READ)
				{
					Calculate_New_Radiation_Alert(CMD_Detector, view);
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].alerts_radiation.info.feature.reading.multiplier - MULTIPLIER_ADJUST);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alerts - Radiation - Custom Enable
			sprintf(tier_command,"CEN\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.display.detector[CMD_Detector].view[view].alerts_radiation.info.feature.custom = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].view[view].alerts_radiation.info.feature.custom = DEFAULT_VIEW_RAD_ALERTS_CUSTOM;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].alerts_radiation.info.feature.custom);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alerts - Radiation - Custom Message
			sprintf(tier_command,"CMG\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
						strncpy(lmi.display.detector[CMD_Detector].view[view].alerts_radiation.info.feature.custom_message, message, SIZE_CUSTOM_MESSAGE);
					else
						strncpy(lmi.display.detector[CMD_Detector].view[view].alerts_radiation.info.feature.custom_message, DEFAULT_VIEW_RAD_ALERTS_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);	// (9DP v1.01.15)
//						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					strncpy(lmi.display.detector[CMD_Detector].view[view].alerts_radiation.info.feature.custom_message, DEFAULT_VIEW_RAD_ALERTS_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				Format_String(tier_command, lmi.display.detector[CMD_Detector].view[view].alerts_radiation.info.feature.custom_message, SIZE_CUSTOM_MESSAGE);								// (9DP v1.01.04)
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Views_Alerts_Integrate_Handler(BYTE view)
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
//*
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:INT:GET:ENB
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:INT:GET:VAL
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:INT:GET:UNI
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:INT:GET:MUL
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:INT:GET:CEN
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:INT:GET:CMG
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:INT:SET:ENB
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:INT:SET:VAL
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:INT:SET:UNI
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:INT:SET:MUL
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:INT:SET:CEN
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:INT:SET:CMG
//*

BOOL Views_Alerts_Integrate_Handler(BYTE view)
{
	char tier_command[MESSAGE_SIZE];
	char message_string[64];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE byte_value;
	UINT16 uint16_value;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{
			//* Alerts - Integrate - Enable
			sprintf(tier_command,"ENB\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.display.detector[CMD_Detector].view[view].alerts_integrate.info.feature.enable = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].view[view].alerts_integrate.info.feature.enable = DEFAULT_VIEW_INT_ALERTS_ENABLE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				if(process != COMMAND_READ)																												// (9DP v1.01.20)
				{																																		// (9DP v1.01.20)
					Force_Screen_Redraw();																												// (9DP v1.01.20)
				}																																		// (9DP v1.01.20)
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].alerts_integrate.info.feature.enable);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alerts - Integrate - Value
			sprintf(tier_command,"VAL\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						uint16_value = atoi(message);
						lmi.display.detector[CMD_Detector].view[view].alerts_integrate.info.feature.reading.value = uint16_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].view[view].alerts_integrate.info.feature.reading.value = DEFAULT_VIEW_INT_ALERTS_VALUE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				if(process != COMMAND_READ)
				{
					Calculate_New_Integrate_Alert(CMD_Detector, view);
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].alerts_integrate.info.feature.reading.value);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alerts - Integrate - Units
			sprintf(tier_command,"UNI\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.display.detector[CMD_Detector].view[view].alerts_integrate.info.feature.reading.units = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					if(view == 0)
						lmi.display.detector[CMD_Detector].view[view].alerts_integrate.info.feature.reading.units = DEFAULT_VIEW_1_INT_ALERTS_UNITS;
					else if(view == 1)
						lmi.display.detector[CMD_Detector].view[view].alerts_integrate.info.feature.reading.units = DEFAULT_VIEW_2_INT_ALERTS_UNITS;
					else if(view == 2)
						lmi.display.detector[CMD_Detector].view[view].alerts_integrate.info.feature.reading.units = DEFAULT_VIEW_3_INT_ALERTS_UNITS;
					else
						lmi.display.detector[CMD_Detector].view[view].alerts_integrate.info.feature.reading.units = DEFAULT_VIEW_4_INT_ALERTS_UNITS;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				if(process != COMMAND_READ)
				{
					Calculate_New_Integrate_Alert(CMD_Detector, view);
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].alerts_integrate.info.feature.reading.units);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alerts - Integrate - Multiplier
			sprintf(tier_command,"MUL\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.display.detector[CMD_Detector].view[view].alerts_integrate.info.feature.reading.multiplier = byte_value + MULTIPLIER_ADJUST;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].view[view].alerts_integrate.info.feature.reading.multiplier = DEFAULT_VIEW_INT_ALERTS_MULTIPLIER;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				if(process != COMMAND_READ)
				{
					Calculate_New_Integrate_Alert(CMD_Detector, view);
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].alerts_integrate.info.feature.reading.multiplier - MULTIPLIER_ADJUST);	// (9DP v1.01.13)
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alerts - Integrate - Custom Enable
			sprintf(tier_command,"CEN\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.display.detector[CMD_Detector].view[view].alerts_integrate.info.feature.custom = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].view[view].alerts_integrate.info.feature.custom = DEFAULT_VIEW_INT_ALERTS_CUSTOM;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].alerts_integrate.info.feature.custom);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alerts - Integrate - Custom Message
			sprintf(tier_command,"CMG\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
						strncpy(lmi.display.detector[CMD_Detector].view[view].alerts_integrate.info.feature.custom_message, message, SIZE_CUSTOM_MESSAGE);
					else
						strncpy(lmi.display.detector[CMD_Detector].view[view].alerts_integrate.info.feature.custom_message, DEFAULT_VIEW_INT_ALERTS_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);	// (9DP v1.01.15)
//						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					strncpy(lmi.display.detector[CMD_Detector].view[view].alerts_integrate.info.feature.custom_message, DEFAULT_VIEW_INT_ALERTS_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				Format_String(tier_command, lmi.display.detector[CMD_Detector].view[view].alerts_integrate.info.feature.custom_message, SIZE_CUSTOM_MESSAGE);								// (9DP v1.01.04)
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Views_Alerts_Function_Handler(BYTE view)
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
//*
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:FUN:GET:ENB
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:FUN:GET:VAL
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:FUN:GET:UNI
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:FUN:GET:MUL
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:FUN:GET:CEN
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:FUN:GET:CMG
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:FUN:SET:ENB
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:FUN:SET:VAL
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:FUN:SET:UNI
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:FUN:SET:MUL
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:FUN:SET:CEN
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALE:FUN:SET:CMG
//*

BOOL Views_Alerts_Function_Handler(BYTE view)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE byte_value;
	UINT16 uint16_value;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{
			//* Alerts - Function - Enable
			sprintf(tier_command,"ENB\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.display.detector[CMD_Detector].view[view].alerts_function.info.feature.enable = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].view[view].alerts_function.info.feature.enable = DEFAULT_VIEW_FUN_ALERTS_ENABLE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				if(process != COMMAND_READ)																												// (9DP v1.01.20)
				{																																		// (9DP v1.01.20)
					Force_Screen_Redraw();																												// (9DP v1.01.20)
				}																																		// (9DP v1.01.20)
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].alerts_function.info.feature.enable);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alerts - Function - Value
			sprintf(tier_command,"VAL\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						uint16_value = atoi(message);
						lmi.display.detector[CMD_Detector].view[view].alerts_function.info.feature.reading.value = uint16_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].view[view].alerts_function.info.feature.reading.value = DEFAULT_VIEW_FUN_ALERTS_VALUE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].alerts_function.info.feature.reading.value);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alerts - Function - Units
			sprintf(tier_command,"UNI\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.display.detector[CMD_Detector].view[view].alerts_function.info.feature.reading.units = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					if(view == 0)
						lmi.display.detector[CMD_Detector].view[view].alerts_function.info.feature.reading.units = DEFAULT_VIEW_1_FUN_ALERTS_UNITS;
					else if(view == 1)
						lmi.display.detector[CMD_Detector].view[view].alerts_function.info.feature.reading.units = DEFAULT_VIEW_2_FUN_ALERTS_UNITS;
					else if(view == 2)
						lmi.display.detector[CMD_Detector].view[view].alerts_function.info.feature.reading.units = DEFAULT_VIEW_3_FUN_ALERTS_UNITS;
					else
						lmi.display.detector[CMD_Detector].view[view].alerts_function.info.feature.reading.units = DEFAULT_VIEW_4_FUN_ALERTS_UNITS;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].alerts_function.info.feature.reading.units);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alerts - Function - Multiplier
			sprintf(tier_command,"MUL\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.display.detector[CMD_Detector].view[view].alerts_function.info.feature.reading.multiplier = byte_value + MULTIPLIER_ADJUST;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].alerts_function.info.feature.reading.multiplier - MULTIPLIER_ADJUST); 				// (9DP v1.01.13)
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alerts - Function - Custom Enable
			sprintf(tier_command,"CEN\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.display.detector[CMD_Detector].view[view].alerts_function.info.feature.custom = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].view[view].alerts_function.info.feature.custom = DEFAULT_VIEW_FUN_ALERTS_CUSTOM;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].alerts_function.info.feature.custom);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alerts - Function - Custom Message
			sprintf(tier_command,"CMG\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
						strncpy(lmi.display.detector[CMD_Detector].view[view].alerts_function.info.feature.custom_message, message, SIZE_CUSTOM_MESSAGE);
					else
						strncpy(lmi.display.detector[CMD_Detector].view[view].alerts_function.info.feature.custom_message, DEFAULT_VIEW_FUN_ALERTS_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);	// (9DP v1.01.15)
//						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					strncpy(lmi.display.detector[CMD_Detector].view[view].alerts_function.info.feature.custom_message, DEFAULT_VIEW_FUN_ALERTS_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				Format_String(tier_command, lmi.display.detector[CMD_Detector].view[view].alerts_function.info.feature.custom_message, SIZE_CUSTOM_MESSAGE);								// (9DP v1.01.04)
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Views_Alarms_Handler(BYTE view)
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
//*
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:RAD
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:INT
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:FUN
//*

BOOL Views_Alarms_Handler(BYTE view)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	UINT16 uint16_value;
	unsigned short address;
	BYTE response = 0;
	BYTE process = 0;
	BYTE byte_value;
	BOOL status;

	message = Parse_Message(NULL);
	if(message != NULL)
	{
		//* Alarms - Radiation
		sprintf(tier_command,"RAD\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Views_Alarms_Radiation_Handler(view);
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				address = EEPROM_ALARMS_RADIATION_LOCATION + (CMD_Detector * TOTAL_DETECTOR) + (view * TOTAL_VIEW);
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, lmi.display.detector[CMD_Detector].view[view].alarms_radiation.buffer, SIZE_ALARMS_RADIATION);
			}
			return status;
		}
	
		//* Alarms - Integrate
		sprintf(tier_command,"INT\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Views_Alarms_Integrate_Handler(view);
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				address = EEPROM_ALARMS_INTEGRATE_LOCATION + (CMD_Detector * TOTAL_DETECTOR) + (view * TOTAL_VIEW);
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, lmi.display.detector[CMD_Detector].view[view].alarms_integrate.buffer, SIZE_ALARMS_INTEGRATE);
			}
			return status;
		}
	
		//* Alarms - Function
		sprintf(tier_command,"FUN\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Views_Alarms_Function_Handler(view);
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				address = EEPROM_ALARMS_FUNCTION_LOCATION + (CMD_Detector * TOTAL_DETECTOR) + (view * TOTAL_VIEW);
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, lmi.display.detector[CMD_Detector].view[view].alarms_function.buffer, SIZE_ALARMS_FUNCTION);
			}
			return status;
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Views_Alarms_Radiation_Handler(BYTE view)
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
//*
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:RAD:GET:ENB
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:RAD:GET:VAL
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:RAD:GET:UNI
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:RAD:GET:MUL
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:RAD:GET:CEN
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:RAD:GET:CMG
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:RAD:SET:ENB
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:RAD:SET:VAL
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:RAD:SET:UNI
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:RAD:SET:MUL
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:RAD:SET:CEN
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:RAD:SET:CMG
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:RAD:RES:ENB
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:RAD:RES:VAL
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:RAD:RES:UNI
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:RAD:RES:MUL
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:RAD:RES:CEN
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:RAD:RES:CMG
//*

BOOL Views_Alarms_Radiation_Handler(BYTE view)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE byte_value;
	UINT16 uint16_value;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{
			//* Alarms - Radiation - Enable
			sprintf(tier_command,"ENB\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.display.detector[CMD_Detector].view[view].alarms_radiation.info.feature.enable = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].view[view].alarms_radiation.info.feature.enable = DEFAULT_VIEW_RAD_ALARMS_ENABLE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				if(process != COMMAND_READ)																												// (9DP v1.01.20)
				{																																		// (9DP v1.01.20)
					Force_Screen_Redraw();																												// (9DP v1.01.20)
				}																																		// (9DP v1.01.20)
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].alarms_radiation.info.feature.enable);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Radiation - Value
			sprintf(tier_command,"VAL\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						uint16_value = atoi(message);
						lmi.display.detector[CMD_Detector].view[view].alarms_radiation.info.feature.reading.value = uint16_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].view[view].alarms_radiation.info.feature.reading.value = DEFAULT_VIEW_RAD_ALARMS_VALUE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				if(process != COMMAND_READ)
				{
					Calculate_New_Radiation_Alarm(CMD_Detector, view);
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].alarms_radiation.info.feature.reading.value);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Radiation - Units
			sprintf(tier_command,"UNI\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.display.detector[CMD_Detector].view[view].alarms_radiation.info.feature.reading.units = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					if(view == 0)
						lmi.display.detector[CMD_Detector].view[view].alarms_radiation.info.feature.reading.units = DEFAULT_VIEW_1_RAD_ALARMS_UNITS;
					else if(view == 1)
						lmi.display.detector[CMD_Detector].view[view].alarms_radiation.info.feature.reading.units = DEFAULT_VIEW_2_RAD_ALARMS_UNITS;
					else if(view == 2)
						lmi.display.detector[CMD_Detector].view[view].alarms_radiation.info.feature.reading.units = DEFAULT_VIEW_3_RAD_ALARMS_UNITS;
					else
						lmi.display.detector[CMD_Detector].view[view].alarms_radiation.info.feature.reading.units = DEFAULT_VIEW_4_RAD_ALARMS_UNITS;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				if(process != COMMAND_READ)
				{
					Calculate_New_Radiation_Alarm(CMD_Detector, view);
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].alarms_radiation.info.feature.reading.units);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Radiation - Multiplier
			sprintf(tier_command,"MUL\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.display.detector[CMD_Detector].view[view].alarms_radiation.info.feature.reading.multiplier = byte_value + MULTIPLIER_ADJUST;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].view[view].alarms_radiation.info.feature.reading.multiplier = DEFAULT_VIEW_RAD_ALARMS_MULTIPLIER;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				if(process != COMMAND_READ)
				{
					Calculate_New_Radiation_Alarm(CMD_Detector, view);
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].alarms_radiation.info.feature.reading.multiplier - MULTIPLIER_ADJUST);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Radiation - Custom Enable
			sprintf(tier_command,"CEN\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.display.detector[CMD_Detector].view[view].alarms_radiation.info.feature.custom = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].view[view].alarms_radiation.info.feature.custom = DEFAULT_VIEW_RAD_ALARMS_CUSTOM;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].alarms_radiation.info.feature.custom);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Radiation - Custom Message
			sprintf(tier_command,"CMG\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
						strncpy(lmi.display.detector[CMD_Detector].view[view].alarms_radiation.info.feature.custom_message, message, SIZE_CUSTOM_MESSAGE);
					else
						strncpy(lmi.display.detector[CMD_Detector].view[view].alarms_radiation.info.feature.custom_message, DEFAULT_VIEW_RAD_ALARMS_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);	// (9DP v1.01.15)
//						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					strncpy(lmi.display.detector[CMD_Detector].view[view].alarms_radiation.info.feature.custom_message, DEFAULT_VIEW_RAD_ALARMS_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				Format_String(tier_command, lmi.display.detector[CMD_Detector].view[view].alarms_radiation.info.feature.custom_message, SIZE_CUSTOM_MESSAGE);								// (9DP v1.01.04)
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Views_Alarms_Integrate_Handler(BYTE view)
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
//*
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:INT:GET:ENB
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:INT:GET:VAL
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:INT:GET:UNI
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:INT:GET:MUL
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:INT:GET:CEN
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:INT:GET:CMG
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:INT:SET:ENB
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:INT:SET:VAL
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:INT:SET:UNI
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:INT:SET:MUL
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:INT:SET:CEN
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:INT:SET:CMG
//*

BOOL Views_Alarms_Integrate_Handler(BYTE view)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE byte_value;
	UINT16 uint16_value;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{
			//* Alarms - Integrate - Enable
			sprintf(tier_command,"ENB\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.display.detector[CMD_Detector].view[view].alarms_integrate.info.feature.enable = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].view[view].alarms_integrate.info.feature.enable = DEFAULT_VIEW_INT_ALARMS_ENABLE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				if(process != COMMAND_READ)																												// (9DP v1.01.20)
				{																																		// (9DP v1.01.20)
					Force_Screen_Redraw();																												// (9DP v1.01.20)
				}																																		// (9DP v1.01.20)
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].alarms_integrate.info.feature.enable);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Integrate - Value
			sprintf(tier_command,"VAL\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						uint16_value = atoi(message);
						lmi.display.detector[CMD_Detector].view[view].alarms_integrate.info.feature.reading.value = uint16_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].view[view].alarms_integrate.info.feature.reading.value = DEFAULT_VIEW_INT_ALARMS_VALUE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				if(process != COMMAND_READ)
				{
					Calculate_New_Integrate_Alarm(CMD_Detector, view);
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].alarms_integrate.info.feature.reading.value);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Integrate - Units
			sprintf(tier_command,"UNI\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.display.detector[CMD_Detector].view[view].alarms_integrate.info.feature.reading.units = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					if(view == 0)
						lmi.display.detector[CMD_Detector].view[view].alarms_integrate.info.feature.reading.units = DEFAULT_VIEW_1_INT_ALARMS_UNITS;
					else if(view == 1)
						lmi.display.detector[CMD_Detector].view[view].alarms_integrate.info.feature.reading.units = DEFAULT_VIEW_2_INT_ALARMS_UNITS;
					else if(view == 2)
						lmi.display.detector[CMD_Detector].view[view].alarms_integrate.info.feature.reading.units = DEFAULT_VIEW_3_INT_ALARMS_UNITS;
					else
						lmi.display.detector[CMD_Detector].view[view].alarms_integrate.info.feature.reading.units = DEFAULT_VIEW_4_INT_ALARMS_UNITS;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				if(process != COMMAND_READ)
				{
					Calculate_New_Integrate_Alarm(CMD_Detector, view);
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].alarms_integrate.info.feature.reading.units);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Integrate - Multiplier
			sprintf(tier_command,"MUL\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.display.detector[CMD_Detector].view[view].alarms_integrate.info.feature.reading.multiplier = byte_value + MULTIPLIER_ADJUST;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].view[view].alarms_integrate.info.feature.reading.multiplier = DEFAULT_VIEW_INT_ALARMS_MULTIPLIER;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				if(process != COMMAND_READ)
				{
					Calculate_New_Integrate_Alarm(CMD_Detector, view);
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].alarms_integrate.info.feature.reading.multiplier - MULTIPLIER_ADJUST); 				// (9DP v1.01.13)
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Integrate - Custom Enable
			sprintf(tier_command,"CEN\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.display.detector[CMD_Detector].view[view].alarms_integrate.info.feature.custom = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].view[view].alarms_integrate.info.feature.custom = DEFAULT_VIEW_INT_ALARMS_CUSTOM;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].alarms_integrate.info.feature.custom);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Integrate - Custom Message
			sprintf(tier_command,"CMG\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
						strncpy(lmi.display.detector[CMD_Detector].view[view].alarms_integrate.info.feature.custom_message, message, SIZE_CUSTOM_MESSAGE);
					else
						strncpy(lmi.display.detector[CMD_Detector].view[view].alarms_integrate.info.feature.custom_message, DEFAULT_VIEW_INT_ALARMS_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);	// (9DP v1.01.15)
//						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					strncpy(lmi.display.detector[CMD_Detector].view[view].alarms_integrate.info.feature.custom_message, DEFAULT_VIEW_INT_ALARMS_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				Format_String(tier_command, lmi.display.detector[CMD_Detector].view[view].alarms_integrate.info.feature.custom_message, SIZE_CUSTOM_MESSAGE);								// (9DP v1.01.04)
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Views_Alarms_Function_Handler(BYTE view)
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
//*
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:FUN:GET:ENB
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:FUN:GET:VAL
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:FUN:GET:UNI
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:FUN:GET:MUL
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:FUN:GET:CEN
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:FUN:GET:CMG
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:FUN:SET:ENB
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:FUN:SET:VAL
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:FUN:SET:UNI
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:FUN:SET:MUL
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:FUN:SET:CEN
//*		INS:DSP:DET:00x:VSP:VIE:00x:ALA:FUN:SET:CMG
//*

BOOL Views_Alarms_Function_Handler(BYTE view)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE byte_value;
	UINT16 uint16_value;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{
			//* Alarms - Function - Enable
			sprintf(tier_command,"ENB\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.display.detector[CMD_Detector].view[view].alarms_function.info.feature.enable = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].view[view].alarms_function.info.feature.enable = DEFAULT_VIEW_FUN_ALARMS_ENABLE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				if(process != COMMAND_READ)																												// (9DP v1.01.20)
				{																																		// (9DP v1.01.20)
					Force_Screen_Redraw();																												// (9DP v1.01.20)
				}																																		// (9DP v1.01.20)
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].alarms_function.info.feature.enable);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Function - Value
			sprintf(tier_command,"VAL\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						uint16_value = atoi(message);
						lmi.display.detector[CMD_Detector].view[view].alarms_function.info.feature.reading.value = uint16_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].view[view].alarms_function.info.feature.reading.value = DEFAULT_VIEW_FUN_ALARMS_VALUE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].alarms_function.info.feature.reading.value);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Function - Units
			sprintf(tier_command,"UNI\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.display.detector[CMD_Detector].view[view].alarms_function.info.feature.reading.units = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					if(view == 0)
						lmi.display.detector[CMD_Detector].view[view].alarms_function.info.feature.reading.units = DEFAULT_VIEW_1_FUN_ALARMS_UNITS;
					else if(view == 1)
						lmi.display.detector[CMD_Detector].view[view].alarms_function.info.feature.reading.units = DEFAULT_VIEW_2_FUN_ALARMS_UNITS;
					else if(view == 2)
						lmi.display.detector[CMD_Detector].view[view].alarms_function.info.feature.reading.units = DEFAULT_VIEW_3_FUN_ALARMS_UNITS;
					else
						lmi.display.detector[CMD_Detector].view[view].alarms_function.info.feature.reading.units = DEFAULT_VIEW_4_FUN_ALARMS_UNITS;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].alarms_function.info.feature.reading.units);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Function - Multiplier
			sprintf(tier_command,"MUL\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.display.detector[CMD_Detector].view[view].alarms_function.info.feature.reading.multiplier = byte_value + MULTIPLIER_ADJUST;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].view[view].alarms_function.info.feature.reading.multiplier = DEFAULT_VIEW_FUN_ALARMS_MULTIPLIER;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].alarms_function.info.feature.reading.multiplier - MULTIPLIER_ADJUST); 				// (9DP v1.01.13)
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Function - Custom Enable
			sprintf(tier_command,"CEN\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.display.detector[CMD_Detector].view[view].alarms_function.info.feature.custom = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].view[view].alarms_function.info.feature.custom = DEFAULT_VIEW_FUN_ALARMS_CUSTOM;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].alarms_function.info.feature.custom);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Function - Custom Message
			sprintf(tier_command,"CMG\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
						strncpy(lmi.display.detector[CMD_Detector].view[view].alarms_function.info.feature.custom_message, message, SIZE_CUSTOM_MESSAGE);
					else
						strncpy(lmi.display.detector[CMD_Detector].view[view].alarms_function.info.feature.custom_message, DEFAULT_VIEW_FUN_ALARMS_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);	// (9DP v1.01.15)
//						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					strncpy(lmi.display.detector[CMD_Detector].view[view].alarms_function.info.feature.custom_message, DEFAULT_VIEW_FUN_ALARMS_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				Format_String(tier_command, lmi.display.detector[CMD_Detector].view[view].alarms_function.info.feature.custom_message, SIZE_CUSTOM_MESSAGE);								// (9DP v1.01.04)
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}
	}
	return FALSE;
}

#ifdef EEPROM_VIEW_CALIBRATION_LOCATION

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Views_Calibration_Handler(BYTE view)
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
//*
//*		INS:DSP:DET:00x:VIE:00x:CAL:GET:Uxx
//*		INS:DSP:DET:00x:VIE:00x:CAL:GET:Sxx
//*		INS:DSP:DET:00x:VIE:00x:CAL:GET:Fxx
//*		INS:DSP:DET:00x:VIE:00x:CAL:SET:Uxx
//*		INS:DSP:DET:00x:VIE:00x:CAL:SET:Sxx
//*		INS:DSP:DET:00x:VIE:00x:CAL:SET:Fxx
//*		INS:DSP:DET:00x:VIE:00x:CAL:RES:Uxx
//*		INS:DSP:DET:00x:VIE:00x:CAL:RES:Sxx
//*		INS:DSP:DET:00x:VIE:00x:CAL:RES:Fxx
//*

BOOL Views_Calibration_Handler(BYTE view)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE value;
	BYTE function;
	BYTE length;
	BYTE index;
	INT16 int16_value;
	UINT16 uint16_value;
	UINT16 calibration_constant;
	float float_value;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{
			// Detector - Calibration - Unsigned Integer
			sprintf(tier_command, "U\0");
			response = strncmp(message,tier_command,1);
			if(response == 0)
			{
				strcat((char *)USB_TXMessage, message);
				strcat((char *)USB_TXMessage, ColonSeparator);
				message[0] = '0';					
				index = atoi(message);
				if(index > 15)
				{
					return FALSE;
				}
				else if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						uint16_value = (UINT16) atoi(message);
						lmi.display.detector[CMD_Detector].view[view].calibration.info.u_arg[index] = uint16_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].view[view].calibration.info.u_arg[index] = 0;												// (3D v1.00.03)
				}
				else
				{
					Write_EEPROM = FALSE;
				}
				if(Write_EEPROM)																													// (3D v1.00.04)
				{																																	// (3D v1.00.04)
					HVBInit(TRUE);																													// (3D v1.00.04)
				}																																	// (3D v1.00.04)
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].calibration.info.u_arg[index]);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
			
			// Detector - Calibration - Signed Integer
			sprintf(tier_command, "S\0");
			response = strncmp(message,tier_command,1);
			if(response == 0)
			{
				strcat((char *)USB_TXMessage, message);
				strcat((char *)USB_TXMessage, ColonSeparator);
				message[0] = '0';					
				index = atoi(message);
				if(index > 15)
				{
					return FALSE;
				}
				else if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						int16_value = atoi(message);
						lmi.display.detector[CMD_Detector].view[view].calibration.info.s_arg[index] = int16_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].view[view].calibration.info.s_arg[index] = 0;
				}
				else
				{
					Write_EEPROM = FALSE;
				}
				if(Write_EEPROM)																													// (3D v1.00.04)
				{																																	// (3D v1.00.04)
					HVBInit(TRUE);																													// (3D v1.00.04)
				}																																	// (3D v1.00.04)
				sprintf((char *)tier_command,"%d\r\n\0", lmi.display.detector[CMD_Detector].view[view].calibration.info.s_arg[index]);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
			
			// Detector - Calibration - Float
			sprintf(tier_command, "F\0");
			response = strncmp(message,tier_command,1);
			if(response == 0)
			{
				strcat((char *)USB_TXMessage, message);
				strcat((char *)USB_TXMessage, ColonSeparator);
				message[0] = '0';					
				index = atoi(message);
				if(index > 7)
				{
					return FALSE;
				}
				else if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						float_value = atof(message);
						lmi.display.detector[CMD_Detector].view[view].calibration.info.f_arg[index] = float_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.display.detector[CMD_Detector].view[view].calibration.info.f_arg[index] = 0.000;
				}
				else
				{
					Write_EEPROM = FALSE;
				}
				if(Write_EEPROM)																													// (3D v1.00.04)
				{																																	// (3D v1.00.04)
					HVBInit(TRUE);																													// (3D v1.00.04)
				}																																	// (3D v1.00.04)
				sprintf((char *)tier_command,"%.3f\r\n\0", lmi.display.detector[CMD_Detector].view[view].calibration.info.f_arg[index]);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}	
	}
	return FALSE;
}
#endif

//*
//**************************************************************************************************
//**************************************************************************************************
//*	END VIEWS HANDLERS
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//*	FUNCTIONS HANDLERS
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Functions_Handler(void)
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
//* 	
//*		INS:FNC:00x:HEA:
//*		INS:FNC:00x:TYP:xxx:
//*

BOOL Functions_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	unsigned short address;
	BYTE response;
	BYTE process;
	BYTE value;
	BYTE function_index;
	BYTE type;
	BOOL status;

	message = Parse_Message(NULL);
	if(message != NULL)
	{
		function_index = (BYTE) atoi(message) - 1;
	
		if(function_index >= NUMBER_OF_FUNCTIONS)
			return FALSE;
	
		sprintf((char *)tier_command,"%03d\0",function_index + 1);
		Append_Message(tier_command, ColonSeparator);
		message = Parse_Message(NULL);
		if(message != NULL)
		{
			sprintf(tier_command, "HEA\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				status = Functions_Header_Handler(function_index);
				if((status == TRUE) && (Write_EEPROM == TRUE))
				{
					address = EEPROM_FUNCTIONS_LOCATION + (function_index * TOTAL_FUNCTION);
					i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, lmi.function[function_index].buffer, SIZE_FUNCTION);
					Force_Screen_Redraw();																							// (9DP v1.02.03)
				}
				return status;
			}
		
			sprintf(tier_command, "TYP\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				message = Parse_Message(NULL);
				if(message != NULL)
				{
					type = (BYTE) atoi(message);
//					if(type < LAST_FUNCTION)
					if((type < LAST_FUNCTION) && (type == lmi.function[function_index].info.header.type))							// (9DP v1.01.15)
					{
						sprintf((char *)tier_command, "%03d\0", type);
						switch(type)
						{
							case TIMED_INTEGRATE:
							case INTEGRATE:
							{
								Append_Message(tier_command, ColonSeparator);
								status = Functions_Integrate_Handler(function_index, type);
								break;
							}
							case PEAK_HOLD:
							case PEAK_RATE:
							{
								Append_Message(tier_command, ColonSeparator);
								status = Functions_Peakrate_Handler(function_index, type);
								break;
							}
							case CORRECTION_FACTOR:																					// (9DP v1.02.02)
							{																										// (9DP v1.02.02)
								Append_Message(tier_command, ColonSeparator);														// (9DP v1.02.02)
								status = Functions_Correction_Factor_Handler(function_index, type);									// (9DP v1.02.02)
								break;																								// (9DP v1.02.02)
							}																										// (9DP v1.02.02)
							case TIME_CONSTANTS:       																				// (9DP v1.04.00)
							{
								Append_Message(tier_command, ColonSeparator);
								status = Functions_Time_Constants_Handler(function_index, type);
								break;
							}
							case NO_FUNCTION:
							case SCALER:
							case RATE_METER:
							case SOURCE_CHECK:
							case METER_RESET:
							case BACKGROUND_READING:
							case BACKGROUND_SUBTRACTION:
							case NET_READING:
							case PERCENT_ALARM:
							case SIGMA_ALARM:
							case HUNDRED_CM2:
							case PCI_GM:
							case DATA_LOGGING:
							case RECORD_EVENT:
							case AUDIO_DIVIDE:
							case BARCODE:
							case GPS:
							case CLOCK_TIME:
							case CALENDAR_DATE:
							case USER_DEFINED_1:
							case USER_DEFINED_2:
							case USER_DEFINED_3:
							case USER_DEFINED_4:
							default:
							{
								status = 0;
								break;
							}
						}
						if((status == TRUE) && (Write_EEPROM == TRUE))
						{
							address = EEPROM_FUNCTIONS_LOCATION + (function_index * TOTAL_FUNCTION);
							i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, lmi.function[function_index].buffer, SIZE_FUNCTION);
							Force_Screen_Redraw();																					// (9DP v1.01.20)
//							if(screenState == MONITOR_RADIATION)																	// (9DP v1.01.15)
//							{																										// (9DP v1.01.15)
//								screenState = CREATE_ELEMENT;																		// (9DP v1.01.15)
//								DisplayDetachOnce = TRUE;																			// (9DP v1.01.15)
//							}																										// (9DP v1.01.15)
						}
						return status;
					}
				}
			}
		}
	}	
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Functions_Header_Handler(BYTE function_index)
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
//* 	
//*		INS:FNC:00x:HEA:GET:TYP
//*		INS:FNC:00x:HEA:GET:NAM
//*		INS:FNC:00x:HEA:SET:TYP
//*		INS:FNC:00x:HEA:SET:NAM
//*		INS:FNC:00x:HEA:RES:TYP
//*		INS:FNC:00x:HEA:RES:NAM
//*

BOOL Functions_Header_Handler(BYTE function_index)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE value;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{	
			//* Functions - Header - Type
			sprintf(tier_command,"TYP\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = (BYTE) atoi(message);
						lmi.function[function_index].info.header.type = value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					if(function_index == 0)
						lmi.function[function_index].info.header.type = DEFAULT_FUNCTION_1_TYPE;
					else if(function_index == 1)
						lmi.function[function_index].info.header.type = DEFAULT_FUNCTION_2_TYPE;
					else if(function_index == 2)
						lmi.function[function_index].info.header.type = DEFAULT_FUNCTION_3_TYPE;
					else if(function_index == 3)
						lmi.function[function_index].info.header.type = DEFAULT_FUNCTION_4_TYPE;
					else
						return FALSE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.function[function_index].info.header.type);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Functions - Header - Name
			sprintf(tier_command,"NAM\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
						strncpy(lmi.function[function_index].info.header.name, message, SIZE_FUNCTION_NAME);
					else
						strncpy(lmi.function[function_index].info.header.name, DEFAULT_FUNCTION_X_NAME, SIZE_FUNCTION_NAME);
				}
				else if(process == COMMAND_RESET)
				{
					if(function_index == 0)
						strncpy(lmi.function[function_index].info.header.name, DEFAULT_FUNCTION_1_NAME, SIZE_FUNCTION_NAME);
					else if(function_index == 1)
						strncpy(lmi.function[function_index].info.header.name, DEFAULT_FUNCTION_2_NAME, SIZE_FUNCTION_NAME);
					else if(function_index == 2)
						strncpy(lmi.function[function_index].info.header.name, DEFAULT_FUNCTION_3_NAME, SIZE_FUNCTION_NAME);
					else if(function_index == 3)
						strncpy(lmi.function[function_index].info.header.name, DEFAULT_FUNCTION_4_NAME, SIZE_FUNCTION_NAME);
					else
						return FALSE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				Format_String(tier_command, lmi.function[function_index].info.header.name, SIZE_FUNCTION_NAME);								// (9DP v1.01.04)
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Functions_Integrate_Handler(BYTE function_index, BYTE type)
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
//*
//*		INS:FNC:00x:TYP:xxx:GET:DET
//*		INS:FNC:00x:TYP:xxx:GET:DIG
//*		INS:FNC:00x:TYP:xxx:GET:TIM
//*		INS:FNC:00x:TYP:xxx:SET:DET
//*		INS:FNC:00x:TYP:xxx:SET:DIG
//*		INS:FNC:00x:TYP:xxx:SET:TIM
//*		INS:FNC:00x:TYP:xxx:RES:DET
//*		INS:FNC:00x:TYP:xxx:RES:DIG
//*		INS:FNC:00x:TYP:xxx:RES:TIM
//*

BOOL Functions_Integrate_Handler(BYTE function_index, BYTE type)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE value;
	UINT16 UINT16_value;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{	
			//* Functions - Integrate - Detector
			sprintf(tier_command,"DET\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = (BYTE) atoi(message);
						if(!Validate_Detector((char) value))
							return FALSE;
						else																														// (9DP v1.01.11)
							lmi.function[function_index].info.integrate.detector = value;															// (9DP v1.01.11)
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.function[function_index].info.integrate.detector = DEFAULT_INTEGRATE_DETECTOR;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				if(process != COMMAND_READ)
				{
					if((screenState == MONITOR_RADIATION) && (lmi.function[Current_Function_Index].info.header.type == type))
					{
						if(type == INTEGRATE)
							Reset_Function(0.000);
					}
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.function[function_index].info.integrate.detector);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Functions - Integrate - Units (hidden 9DP v1.01.15)
//			sprintf(tier_command,"UNI\0");
//			response = strcmp(message,tier_command);
//			if(response == 0)
//			{
//				Append_Message(tier_command, ColonSeparator);
//				if(process == COMMAND_WRITE)
//				{
//					message = Parse_Message(NULL);
//					if(message != NULL)
//					{
//						value = (BYTE) atoi(message);
//						lmi.function[function_index].info.integrate.units = value;
//					}
//					else
//						return FALSE;
//				}
//				else if(process == COMMAND_RESET)
//				{
//					lmi.function[function_index].info.integrate.units = DEFAULT_INTEGRATE_UNITS;
//				}
//				else if(process == COMMAND_READ)
//				{
//					Write_EEPROM = FALSE;
//				}
//				if(process != COMMAND_READ)
//				{
//					if((screenState == MONITOR_RADIATION) && (lmi.function[Current_Function_Index].info.header.type == type))
//					{
//						if(type == INTEGRATE)
//							Reset_Function(Rate_Accumulation);
//					}
//				}
//				sprintf((char *)tier_command,"%d\r\n\0", lmi.function[function_index].info.integrate.units);
//				strcat((char *)USB_TXMessage, tier_command);
//				return TRUE;
//			}
		
			//* Functions - Integrate - Multiplier (hidden 9DP v1.01.15)
//			sprintf(tier_command,"MUL\0");
//			response = strcmp(message,tier_command);
//			if(response == 0)
//			{
//				Append_Message(tier_command, ColonSeparator);
//				if(process == COMMAND_WRITE)
//				{
//					message = Parse_Message(NULL);
//					if(message != NULL)
//					{
//						value = (BYTE) atoi(message);
//						lmi.function[function_index].info.integrate.multiplier = value + MULTIPLIER_ADJUST;
//					}
//					else
//						return FALSE;
//				}
//				else if(process == COMMAND_RESET)
//				{
//					lmi.function[function_index].info.integrate.multiplier = DEFAULT_INTEGRATE_MULTIPLIER;
//				}
//				else if(process == COMMAND_READ)
//				{
//					Write_EEPROM = FALSE;
//				}
//				if(process != COMMAND_READ)
//				{
//					if((screenState == MONITOR_RADIATION) && (lmi.function[Current_Function_Index].info.header.type == type))
//					{
//						if(type == INTEGRATE)
//							Reset_Function(Rate_Accumulation);
//					}
//				}
//				sprintf((char *)tier_command,"%d\r\n\0", (lmi.function[function_index].info.integrate.multiplier - MULTIPLIER_ADJUST));
//				strcat((char *)USB_TXMessage, tier_command);
//				return TRUE;
//			}
		
			//* Functions - Integrate - Digits
			sprintf(tier_command,"DIG\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = (BYTE) atoi(message);
						lmi.function[function_index].info.integrate.digits = value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.function[function_index].info.integrate.digits = DEFAULT_INTEGRATE_DIGITS;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				if(process != COMMAND_READ)
				{
					if((screenState == MONITOR_RADIATION) && (lmi.function[Current_Function_Index].info.header.type == type))
					{
						if(type == INTEGRATE)
							Reset_Function(Rate_Accumulation);
					}
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.function[function_index].info.integrate.digits);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}

			//* Functions - Integrate - Time
			sprintf(tier_command,"TIM\0");
			response = strcmp(message,tier_command);
//			if(response == 0)
			if((response == 0) && (type == TIMED_INTEGRATE))																						// (9DP v1.01.15)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						UINT16_value = atoi(message);
						lmi.function[function_index].info.integrate.time = UINT16_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.function[function_index].info.integrate.time = DEFAULT_INTEGRATE_TIME;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				if(process != COMMAND_READ)
				{
					if((screenState == MONITOR_RADIATION) && (lmi.function[Current_Function_Index].info.header.type == type))
					{
						if(type == INTEGRATE)
							Reset_Function(Rate_Accumulation);
					}
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.function[function_index].info.integrate.time);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Functions_Peakrate_Handler(BYTE function_index, BYTE type)
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
//* 	
//*		INS:FNC:00x:TYP:xxx:GET:DIG
//*		INS:FNC:00x:TYP:xxx:GET:RLK
//*		INS:FNC:00x:TYP:xxx:SET:DIG
//*		INS:FNC:00x:TYP:xxx:SET:RLK
//*		INS:FNC:00x:TYP:xxx:RES:DIG
//*		INS:FNC:00x:TYP:xxx:RES:RLK
//*

BOOL Functions_Peakrate_Handler(BYTE function_index, BYTE type)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE value;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{	
			//* Functions - Peakrate - Digits
			sprintf(tier_command,"DIG\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = (BYTE) atoi(message);
						lmi.function[function_index].info.peak_rate.digits = value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.function[function_index].info.peak_rate.digits = DEFAULT_PEAKRATE_DIGITS;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				if(process != COMMAND_READ)
				{
					if((screenState == MONITOR_RADIATION) && (lmi.function[Current_Function_Index].info.header.type == type))
						Reset_Function(Peak_Rate_Reading);
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.function[function_index].info.peak_rate.digits);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Functions - Peakrate - Element Range Lock
			sprintf(tier_command,"RLK\0");
			response = strcmp(message,tier_command);
//			if(response == 0)
			if((response == 0) && (type == PEAK_HOLD))																								// (9DP v1.01.15)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = (BYTE) atoi(message);
						lmi.function[function_index].info.peak_rate.element_range_lock = value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.function[function_index].info.peak_rate.element_range_lock = DEFAULT_PEAKRATE_RANGELOCK;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				if(process != COMMAND_READ)
				{
					Range_Lock = (BOOL) lmi.function[function_index].info.peak_rate.element_range_lock;
					Range_Lock_Valid = FALSE;
					if((screenState == MONITOR_RADIATION) && (lmi.function[Current_Function_Index].info.header.type == type))
						Reset_Function(Peak_Rate_Reading);
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.function[function_index].info.peak_rate.element_range_lock);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Functions_Correction_Factor_Handler(BYTE function_index, BYTE type)
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
//* 	
//*		INS:FNC:00x:TYP:023:GET:R0x
//*		INS:FNC:00x:TYP:023:GET:F0x
//*		INS:FNC:00x:TYP:023:SET:R0x
//*		INS:FNC:00x:TYP:023:SET:F0x
//*		INS:FNC:00x:TYP:023:RES:R0x
//*		INS:FNC:00x:TYP:023:RES:F0x
//*
//*		(added 9DP v1.02.02)
//*

BOOL Functions_Correction_Factor_Handler(BYTE function_index, BYTE type)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE value;
	BYTE index;
	float f_value;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{	
			//* Functions - Correction Factor - Range
			sprintf(tier_command,"R0\0");
			response = strncmp(message,tier_command,2);
			if((response == 0) && (type == CORRECTION_FACTOR))
			{
				Append_Message(message, ColonSeparator);
				message[0] = '0';					
				message[1] = '0';					
				index = atoi(message);
				if(index > 3)
				{
					return FALSE;
				}
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = (BYTE) atoi(message);
						lmi.function[function_index].info.correction_factor.range[index] = value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.function[function_index].info.correction_factor.range[index] = 0;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				if(index == Correction_Factor_Mode)
				{
					lmi.calibration.controls.info.fixed_range = lmi.function[function_index].info.correction_factor.range[index];
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.function[function_index].info.correction_factor.range[index]);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Functions - Correction Factor - Factor
			sprintf(tier_command,"F0\0");
			response = strncmp(message,tier_command,2);
			if((response == 0) && (type == CORRECTION_FACTOR))
			{
				Append_Message(message, ColonSeparator);
				message[0] = '0';					
				message[1] = '0';					
				index = atoi(message);
				if(index > 3)
				{
					return FALSE;
				}
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						f_value = atof(message);
						lmi.function[function_index].info.correction_factor.factor[index] = f_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.function[function_index].info.correction_factor.factor[index] = 1.000;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				if(index == Correction_Factor_Mode)
				{
					Correction_Factor_Value = lmi.function[function_index].info.correction_factor.factor[index];
				}
				sprintf((char *)tier_command,"%05.3f\r\n\0", lmi.function[function_index].info.correction_factor.factor[index]);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}
	}
	return FALSE;
}

//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Functions_Time_Constants_Handler(BYTE function_index, BYTE type)
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
//*
//*		INS:FNC:00x:TYP:xxx:GET:R0x
//*		INS:FNC:00x:TYP:xxx:SET:R0x
//*		INS:FNC:00x:TYP:xxx:RES:R0x
//*

BOOL Functions_Time_Constants_Handler(BYTE function_index, BYTE type) {
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE value;
	BYTE index;
	float f_value;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{	
			//* Functions - Time Constant - Range
			sprintf(tier_command,"R0\0");
			response = strncmp(message,tier_command,2);
			if((response == 0) && (type == TIME_CONSTANTS))
			{
				Append_Message(message, ColonSeparator);
				message[0] = '0';					
				message[1] = '0';					
				index = atoi(message) - 1;
				if(index > 4)  // Ranges 1 through 5 accepted
				{
					return FALSE;
				}
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						f_value = atof(message);
						lmi.function[function_index].info.time_constant.range[index] = f_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.function[function_index].info.time_constant.range[index] = 0.0;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				if(f_value < 1.0 / instrumentConstantsGetSampleRate(ic))
				{
					// Auto Mode
					ic->rangeConstants[index].isFixedTime = FALSE;
				}
				else {
					ic->rangeConstants[index].isFixedTime = TRUE;
					ic->rangeConstants[index].fixedTime = f_value;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.function[function_index].info.time_constant.range[index]);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}
	}	
	return FALSE;
}


//*
//**************************************************************************************************
//**************************************************************************************************
//*	END FUNCITONS HANDLERS
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//*	ALERTS HANDLERS
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Alerts_Handler(void)
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
//* 	
//*		INS:ALE:LOB:
//*		INS:ALE:LOM:
//*		INS:ALE:CDD:
//*		INS:ALE:SCH:
//*		INS:ALE:BCH:
//*

BOOL Alerts_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response;
	BYTE process;
	BYTE value;
	BYTE view;
	BOOL status;

	message = Parse_Message(NULL);
	if(message != NULL)
	{
		sprintf(tier_command, "LOB\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Alerts_Low_Battery_Handler();
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALERTS_LOW_BATTERY_LOCATION, 2, lmi.alerts.low_battery.buffer, SIZE_ALERTS_LOW_BATTERY);
			}
			return status;
		}
		
		sprintf(tier_command, "LOM\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Alerts_Low_Memory_Handler();
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALERTS_LOW_MEMORY_LOCATION, 2, lmi.alerts.low_memory.buffer, SIZE_ALERTS_LOW_MEMORY);
			}
			return status;
		}
		
		sprintf(tier_command, "CDD\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Alerts_Calibration_Due_Handler();
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALERTS_CALIBRATION_DUE_LOCATION, 2, lmi.alerts.calibration_due.buffer, SIZE_ALERTS_CALIBRATION_DUE);
			}
			return status;
		}
		
		sprintf(tier_command, "SCH\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Alerts_Source_Check_Handler();
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALERTS_SOURCE_CHECK_LOCATION, 2, lmi.alerts.source_check.buffer, SIZE_ALERTS_SOURCE_CHECK);
			}
			return status;
		}
		
		sprintf(tier_command, "BCH\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Alerts_Background_Check_Handler();
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALERTS_BACKGROUND_CHECK_LOCATION, 2, lmi.alerts.background_check.buffer, SIZE_ALERTS_BACKGROUND_CHECK);
			}
			return status;
		}
	}	
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Alerts_Low_Battery_Handler(void)
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
//* 	
//*		INS:ALE:LOB:GET:ENB
//*		INS:ALE:LOB:GET:VAL
//*		INS:ALE:LOB:GET:CEN
//*		INS:ALE:LOB:GET:CMG
//*		INS:ALE:LOB:SET:ENB
//*		INS:ALE:LOB:SET:VAL
//*		INS:ALE:LOB:SET:CEN
//*		INS:ALE:LOB:SET:CMG
//*		INS:ALE:LOB:RES:ENB
//*		INS:ALE:LOB:RES:VAL
//*		INS:ALE:LOB:RES:CEN
//*		INS:ALE:LOB:RES:CMG
//*

BOOL Alerts_Low_Battery_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE byte_value;
	float float_value;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{
			//* Alerts - Low Battery - Enable
			sprintf(tier_command,"ENB\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						if(!Event_Handler(byte_value, 2, &lmi.alerts.low_battery.info.feature.enable))
						{
							return FALSE;
						}
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alerts.low_battery.info.feature.enable = DEFAULT_ALERT_LOW_BATTERY_ENABLE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alerts.low_battery.info.feature.enable);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alerts - Low Battery - Custom Enable
			sprintf(tier_command,"CEN\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						if(!Event_Handler(byte_value, 2, &lmi.alerts.low_battery.info.feature.custom))
						{
							return FALSE;
						}
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alerts.low_battery.info.feature.custom = DEFAULT_ALERT_LOW_BATTERY_CUSTOM_ENABLE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alerts.low_battery.info.feature.custom);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alerts - Low Battery - Value
			sprintf(tier_command,"VAL\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						float_value = atof(message);
						lmi.alerts.low_battery.info.feature.value = float_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alerts.low_battery.info.feature.value = DEFAULT_ALERT_LOW_BATTERY_VALUE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%.1f\r\n\0", lmi.alerts.low_battery.info.feature.value);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alerts - Low Battery - Custom Message
			sprintf(tier_command,"CMG\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
						strncpy(lmi.alerts.low_battery.info.feature.custom_message, message, SIZE_CUSTOM_MESSAGE);
					else
						strncpy(lmi.alerts.low_battery.info.feature.custom_message, DEFAULT_ALERT_LOW_BATTERY_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);	// (9DP v1.01.15)
//						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					strncpy(lmi.alerts.low_battery.info.feature.custom_message, DEFAULT_ALERT_LOW_BATTERY_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				Format_String(tier_command, lmi.alerts.low_battery.info.feature.custom_message, SIZE_CUSTOM_MESSAGE);								// (9DP v1.01.04)
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Alerts_Low_Memory_Handler(void)
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
//* 	
//*		INS:ALE:LOM:GET:ENB
//*		INS:ALE:LOM:GET:VAL
//*		INS:ALE:LOM:GET:CEN
//*		INS:ALE:LOM:GET:CMG
//*		INS:ALE:LOM:SET:ENB
//*		INS:ALE:LOM:SET:VAL
//*		INS:ALE:LOM:SET:CEN
//*		INS:ALE:LOM:SET:CMG
//*		INS:ALE:LOM:RES:ENB
//*		INS:ALE:LOM:RES:VAL
//*		INS:ALE:LOM:RES:CEN
//*		INS:ALE:LOM:RES:CMG
//*

BOOL Alerts_Low_Memory_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE byte_value;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{
			//* Alerts - Low Memory - Enable
			sprintf(tier_command,"ENB\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.alerts.low_memory.info.feature.enable = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alerts.low_memory.info.feature.enable = DEFAULT_ALERT_LOW_MEMORY_ENABLE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alerts.low_memory.info.feature.enable);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alerts - Low Memory - Custom Enable
			sprintf(tier_command,"CEN\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.alerts.low_memory.info.feature.custom = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alerts.low_memory.info.feature.custom = DEFAULT_ALERT_LOW_MEMORY_CUSTOM_ENABLE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alerts.low_memory.info.feature.custom);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alerts - Low Memory - Value
			sprintf(tier_command,"VAL\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.alerts.low_memory.info.feature.value = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alerts.low_memory.info.feature.value = DEFAULT_ALERT_LOW_MEMORY_VALUE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alerts.low_memory.info.feature.value);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alerts - Low Memory - Custom Message
			sprintf(tier_command,"CMG\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
						strncpy(lmi.alerts.low_memory.info.feature.custom_message, message, SIZE_CUSTOM_MESSAGE);
					else
						strncpy(lmi.alerts.low_memory.info.feature.custom_message, DEFAULT_ALERT_LOW_MEMORY_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);	// (9DP v1.01.15)
//						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					strncpy(lmi.alerts.low_memory.info.feature.custom_message, DEFAULT_ALERT_LOW_MEMORY_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
//				sprintf((char *)tier_command,"%s\r\n\0", lmi.alerts.low_memory.info.feature.custom_message);										// (9DP v1.01.04)
				Format_String(tier_command, lmi.alerts.low_memory.info.feature.custom_message, SIZE_CUSTOM_MESSAGE);								// (9DP v1.01.04)
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Alerts_Calibration_Due_Handler(void)
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
//* 	
//*		INS:ALE:CDD:GET:ENB
//*		INS:ALE:CDD:GET:VAL
//*		INS:ALE:CDD:GET:CEN
//*		INS:ALE:CDD:GET:CMG
//*		INS:ALE:CDD:SET:ENB
//*		INS:ALE:CDD:SET:VAL
//*		INS:ALE:CDD:SET:CEN
//*		INS:ALE:CDD:SET:CMG
//*		INS:ALE:CDD:RES:ENB
//*		INS:ALE:CDD:RES:VAL
//*		INS:ALE:CDD:RES:CEN
//*		INS:ALE:CDD:RES:CMG
//*

BOOL Alerts_Calibration_Due_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE byte_value;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{
			//* Alerts - Calibration Due - Enable
			sprintf(tier_command,"ENB\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.alerts.calibration_due.info.feature.enable = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alerts.calibration_due.info.feature.enable = DEFAULT_ALERT_CALIBRATION_DUE_ENABLE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alerts.calibration_due.info.feature.enable);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alerts - Calibration Due - Custom Enable
			sprintf(tier_command,"CEN\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.alerts.calibration_due.info.feature.custom = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alerts.calibration_due.info.feature.custom = DEFAULT_ALERT_CALIBRATION_DUE_CUSTOM_ENABLE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alerts.calibration_due.info.feature.custom);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alerts - Calibration Due - Value
			sprintf(tier_command,"VAL\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.alerts.calibration_due.info.feature.value = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alerts.calibration_due.info.feature.value = DEFAULT_ALERT_CALIBRATION_DUE_VALUE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alerts.calibration_due.info.feature.value);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alerts - Calibration Due - Custom Message
			sprintf(tier_command,"CMG\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
						strncpy(lmi.alerts.calibration_due.info.feature.custom_message, message, SIZE_CUSTOM_MESSAGE);
					else
						strncpy(lmi.alerts.calibration_due.info.feature.custom_message, DEFAULT_ALERT_CALIBRATION_DUE_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);	// (9DP v1.01.15)
//						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					strncpy(lmi.alerts.calibration_due.info.feature.custom_message, DEFAULT_ALERT_CALIBRATION_DUE_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				Format_String(tier_command, lmi.alerts.calibration_due.info.feature.custom_message, SIZE_CUSTOM_MESSAGE);									// (9DP v1.01.04)
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Alerts_Source_Check_Handler(void)
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
//* 	
//*		INS:ALE:SCH:GET:ENB
//*		INS:ALE:SCH:GET:VAL
//*		INS:ALE:SCH:GET:UNI
//*		INS:ALE:SCH:GET:MUL
//*		INS:ALE:SCH:GET:CEN
//*		INS:ALE:SCH:GET:CMG
//*		INS:ALE:SCH:SET:ENB
//*		INS:ALE:SCH:SET:VAL
//*		INS:ALE:SCH:SET:UNI
//*		INS:ALE:SCH:SET:MUL
//*		INS:ALE:SCH:SET:CEN
//*		INS:ALE:SCH:SET:CMG
//*		INS:ALE:SCH:RES:ENB
//*		INS:ALE:SCH:RES:VAL
//*		INS:ALE:SCH:RES:UNI
//*		INS:ALE:SCH:RES:MUL
//*		INS:ALE:SCH:RES:CEN
//*		INS:ALE:SCH:RES:CMG
//*

BOOL Alerts_Source_Check_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE byte_value;
	UINT16 uint16_value;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{
			//* Alerts - Source Check - Enable
			sprintf(tier_command,"ENB\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.alerts.source_check.info.feature.enable = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alerts.source_check.info.feature.enable = DEFAULT_ALERT_SOURCE_CHECK_ENABLE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alerts.source_check.info.feature.enable);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alerts - Source Check - Custom Enable
			sprintf(tier_command,"CEN\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.alerts.source_check.info.feature.custom = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alerts.source_check.info.feature.custom = DEFAULT_ALERT_SOURCE_CHECK_CUSTOM_ENABLE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alerts.source_check.info.feature.custom);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alerts - Source Check - Value
			sprintf(tier_command,"VAL\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						uint16_value = atoi(message);
						lmi.alerts.source_check.info.feature.reading.value = uint16_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alerts.source_check.info.feature.reading.value = DEFAULT_ALERT_SOURCE_CHECK_VALUE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alerts.source_check.info.feature.reading.value);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alerts - Source Check - Units
			sprintf(tier_command,"UNI\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.alerts.source_check.info.feature.reading.units = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alerts.source_check.info.feature.reading.units = DEFAULT_ALERT_SOURCE_CHECK_UNITS;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alerts.source_check.info.feature.reading.units);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alerts - Source Check - Multiplier
			sprintf(tier_command,"MUL\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.alerts.source_check.info.feature.reading.multiplier = byte_value + MULTIPLIER_ADJUST;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alerts.source_check.info.feature.reading.multiplier = DEFAULT_ALERT_SOURCE_CHECK_MULTIPLIER;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alerts.source_check.info.feature.reading.multiplier - MULTIPLIER_ADJUST);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alerts - Source Check - Custom Message
			sprintf(tier_command,"CMG\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
						strncpy(lmi.alerts.source_check.info.feature.custom_message, message, SIZE_CUSTOM_MESSAGE);
					else
						strncpy(lmi.alerts.source_check.info.feature.custom_message, DEFAULT_ALERT_SOURCE_CHECK_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);	// (9DP v1.01.15)
//						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					strncpy(lmi.alerts.source_check.info.feature.custom_message, DEFAULT_ALERT_SOURCE_CHECK_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				Format_String(tier_command, lmi.alerts.source_check.info.feature.custom_message, SIZE_CUSTOM_MESSAGE);									// (9DP v1.01.04)
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Alerts_Background_Check_Handler(void)
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
//* 	
//*		INS:ALE:BCH:GET:ENB
//*		INS:ALE:BCH:GET:VAL
//*		INS:ALE:BCH:GET:UNI
//*		INS:ALE:BCH:GET:MUL
//*		INS:ALE:BCH:GET:CEN
//*		INS:ALE:BCH:GET:CMG
//*		INS:ALE:BCH:SET:ENB
//*		INS:ALE:BCH:SET:VAL
//*		INS:ALE:BCH:SET:UNI
//*		INS:ALE:BCH:SET:MUL
//*		INS:ALE:BCH:SET:CEN
//*		INS:ALE:BCH:SET:CMG
//*		INS:ALE:BCH:RES:ENB
//*		INS:ALE:BCH:RES:VAL
//*		INS:ALE:BCH:RES:UNI
//*		INS:ALE:BCH:RES:MUL
//*		INS:ALE:BCH:RES:CEN
//*		INS:ALE:BCH:RES:CMG
//*

BOOL Alerts_Background_Check_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE byte_value;
	UINT16 uint16_value;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{
			//* Alerts - Background Check - Enable
			sprintf(tier_command,"ENB\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.alerts.background_check.info.feature.enable = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alerts.background_check.info.feature.enable = DEFAULT_ALERT_BACKGROUND_CHECK_ENABLE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alerts.background_check.info.feature.enable);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alerts - Background Check - Custom Enable
			sprintf(tier_command,"CEN\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.alerts.background_check.info.feature.custom = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alerts.background_check.info.feature.custom = DEFAULT_ALERT_BACKGROUND_CHECK_CUSTOM_ENABLE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alerts.background_check.info.feature.custom);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alerts - Background Check - Value
			sprintf(tier_command,"VAL\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						uint16_value = atoi(message);
						lmi.alerts.background_check.info.feature.reading.value = uint16_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alerts.background_check.info.feature.reading.value = DEFAULT_ALERT_BACKGROUND_CHECK_VALUE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alerts.background_check.info.feature.reading.value);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alerts - Background Check - Units
			sprintf(tier_command,"UNI\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.alerts.background_check.info.feature.reading.units = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alerts.background_check.info.feature.reading.units = DEFAULT_ALERT_BACKGROUND_CHECK_UNITS;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alerts.background_check.info.feature.reading.units);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alerts - Background Check - Multiplier
			sprintf(tier_command,"MUL\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.alerts.background_check.info.feature.reading.multiplier = byte_value + MULTIPLIER_ADJUST;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alerts.background_check.info.feature.reading.multiplier = DEFAULT_ALERT_BACKGROUND_CHECK_MULTIPLIER;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alerts.background_check.info.feature.reading.multiplier - MULTIPLIER_ADJUST);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alerts - Background Check - Custom Message
			sprintf(tier_command,"CMG\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
						strncpy(lmi.alerts.background_check.info.feature.custom_message, message, SIZE_CUSTOM_MESSAGE);
					else
						strncpy(lmi.alerts.background_check.info.feature.custom_message, DEFAULT_ALERT_BACKGROUND_CHECK_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);	// (9DP v1.01.15)
//						return FALSE;
					
				}
				else if(process == COMMAND_RESET)
				{
					strncpy(lmi.alerts.background_check.info.feature.custom_message, DEFAULT_ALERT_BACKGROUND_CHECK_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				Format_String(tier_command, lmi.alerts.background_check.info.feature.custom_message, SIZE_CUSTOM_MESSAGE);										// (9DP v1.01.04)
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	END ALERTS HANDLERS
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//*	ALARM HANDLERS
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Alarms_Handler(void)
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
//* 	
//*		INS:ALA:LOB:
//*		INS:ALA:LOM:
//*		INS:ALA:CDD:
//*		INS:ALA:SCH:
//*		INS:ALA:BCH:
//*

BOOL Alarms_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response;
	BYTE process;
	BYTE value;
	BYTE view;
	BOOL status;

	message = Parse_Message(NULL);																	// read view descriptor
	if(message != NULL)
	{
		sprintf(tier_command, "LOB\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Alarms_Low_Battery_Handler();
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALARMS_LOW_BATTERY_LOCATION, 2, lmi.alarms.low_battery.buffer, SIZE_ALARMS_LOW_BATTERY);
			}
			return status;																			// (9DP v1.01.15)
		}
		
		sprintf(tier_command, "LOM\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Alarms_Low_Memory_Handler();
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{	
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALARMS_LOW_MEMORY_LOCATION, 2, lmi.alarms.low_memory.buffer, SIZE_ALARMS_LOW_MEMORY);
			}
			return status;																			// (9DP v1.01.15)
		}
		
		sprintf(tier_command, "CDD\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Alarms_Calibration_Due_Handler();
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALARMS_CALIBRATION_DUE_LOCATION, 2, lmi.alarms.calibration_due.buffer, SIZE_ALARMS_CALIBRATION_DUE);
			}
			return status;																			// (9DP v1.01.15)
		}
		
		sprintf(tier_command, "SCH\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Alarms_Source_Check_Handler();
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALARMS_SOURCE_CHECK_LOCATION, 2, lmi.alarms.source_check.buffer, SIZE_ALARMS_SOURCE_CHECK);
			}
			return status;																			// (9DP v1.01.15)
		}
		
		sprintf(tier_command, "BCH\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Alarms_Background_Check_Handler();
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALARMS_BACKGROUND_CHECK_LOCATION, 2, lmi.alarms.background_check.buffer, SIZE_ALARMS_BACKGROUND_CHECK);
			}
			return status;																			// (9DP v1.01.15)
		}
		
		sprintf(tier_command, "HVL\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Alarms_HV_Low_Handler();
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALARMS_HV_LOW_LOCATION, 2, lmi.alarms.hv_low.buffer, SIZE_ALARMS_HV_LOW);
			}
			return status;																			// (9DP v1.01.15)
		}
		
		sprintf(tier_command, "HVH\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Alarms_HV_High_Handler();
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALARMS_HV_HIGH_LOCATION, 2, lmi.alarms.hv_high.buffer, SIZE_ALARMS_HV_HIGH);
			}
			return status;																			// (9DP v1.01.15)
		}
		
		sprintf(tier_command, "LOR\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Alarms_Low_Reading_Handler();
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALARMS_LOW_READING_LOCATION, 2, lmi.alarms.low_reading.buffer, SIZE_ALARMS_LOW_READING);
			}
			return status;																			// (9DP v1.01.15)
		}
	}	
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Alarms_Low_Battery_Handler(void)
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
//* 	
//*		INS:ALA:LOB:GET:ENB
//*		INS:ALA:LOB:GET:VAL
//*		INS:ALA:LOB:GET:CEN
//*		INS:ALA:LOB:GET:CMG
//*		INS:ALA:LOB:SET:ENB
//*		INS:ALA:LOB:SET:VAL
//*		INS:ALA:LOB:SET:CEN
//*		INS:ALA:LOB:SET:CMG
//*		INS:ALA:LOB:RES:ENB
//*		INS:ALA:LOB:RES:VAL
//*		INS:ALA:LOB:RES:CEN
//*		INS:ALA:LOB:RES:CMG
//*

BOOL Alarms_Low_Battery_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE byte_value;
	float float_value;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{
			//* Alarms - Low Battery - Enable
			sprintf(tier_command,"ENB\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.alarms.low_battery.info.feature.enable = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alarms.low_battery.info.feature.enable = DEFAULT_ALARM_LOW_BATTERY_ENABLE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alarms.low_battery.info.feature.enable);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Low Battery - Custom Enable
			sprintf(tier_command,"CEN\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.alarms.low_battery.info.feature.custom = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alarms.low_battery.info.feature.custom = DEFAULT_ALARM_LOW_BATTERY_CUSTOM_ENABLE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alarms.low_battery.info.feature.custom);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Low Battery - Value
			sprintf(tier_command,"VAL\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						float_value = atof(message);
						lmi.alarms.low_battery.info.feature.value = float_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alarms.low_battery.info.feature.value = DEFAULT_ALARM_LOW_BATTERY_VALUE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%.3f\r\n\0", lmi.alarms.low_battery.info.feature.value);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Low Battery - Custom Message
			sprintf(tier_command,"CMG\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
						strncpy(lmi.alarms.low_battery.info.feature.custom_message, message, SIZE_CUSTOM_MESSAGE);
					else
						strncpy(lmi.alarms.low_battery.info.feature.custom_message, DEFAULT_ALARM_LOW_BATTERY_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);	// (9DP v1.01.15)
//						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					strncpy(lmi.alarms.low_battery.info.feature.custom_message, DEFAULT_ALARM_LOW_BATTERY_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				Format_String(tier_command, lmi.alarms.low_battery.info.feature.custom_message, SIZE_CUSTOM_MESSAGE);								// (9DP v1.01.04)
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Alarms_Low_Memory_Handler(void)
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
//* 	
//*		INS:ALA:LOM:GET:ENB
//*		INS:ALA:LOM:GET:VAL
//*		INS:ALA:LOM:GET:CEN
//*		INS:ALA:LOM:GET:CMG
//*		INS:ALA:LOM:SET:ENB
//*		INS:ALA:LOM:SET:VAL
//*		INS:ALA:LOM:SET:CEN
//*		INS:ALA:LOM:SET:CMG
//*		INS:ALA:LOM:RES:ENB
//*		INS:ALA:LOM:RES:VAL
//*		INS:ALA:LOM:RES:CEN
//*		INS:ALA:LOM:RES:CMG
//*

BOOL Alarms_Low_Memory_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE byte_value;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{
			//* Alarms - Low Memory - Enable
			sprintf(tier_command,"ENB\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.alarms.low_memory.info.feature.enable = byte_value;	
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alarms.low_memory.info.feature.enable = DEFAULT_ALARM_LOW_MEMORY_ENABLE;	
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alarms.low_memory.info.feature.enable);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Low Memory - Custom Enable
			sprintf(tier_command,"CEN\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.alarms.low_memory.info.feature.custom = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alarms.low_memory.info.feature.custom = DEFAULT_ALARM_LOW_MEMORY_CUSTOM_ENABLE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alarms.low_memory.info.feature.custom);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Low Memory - Value
			sprintf(tier_command,"VAL\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.alarms.low_memory.info.feature.value = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alarms.low_memory.info.feature.value = DEFAULT_ALARM_LOW_MEMORY_VALUE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alarms.low_memory.info.feature.value);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Low Memory - Custom Message
			sprintf(tier_command,"CMG\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
						strncpy(lmi.alarms.low_memory.info.feature.custom_message, message, SIZE_CUSTOM_MESSAGE);
					else
						strncpy(lmi.alarms.low_memory.info.feature.custom_message, DEFAULT_ALARM_LOW_MEMORY_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);	// (9DP v1.01.15)
//						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					strncpy(lmi.alarms.low_memory.info.feature.custom_message, DEFAULT_ALARM_LOW_MEMORY_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				Format_String(tier_command, lmi.alarms.low_memory.info.feature.custom_message, SIZE_CUSTOM_MESSAGE);								// (9DP v1.01.04)
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Alarms_Calibration_Due_Handler(void)
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
//* 	
//*		INS:ALA:CDD:GET:ENB
//*		INS:ALA:CDD:GET:VAL
//*		INS:ALA:CDD:GET:CEN
//*		INS:ALA:CDD:GET:CMG
//*		INS:ALA:CDD:SET:ENB
//*		INS:ALA:CDD:SET:VAL
//*		INS:ALA:CDD:SET:CEN
//*		INS:ALA:CDD:SET:CMG
//*		INS:ALA:CDD:RES:ENB
//*		INS:ALA:CDD:RES:VAL
//*		INS:ALA:CDD:RES:CEN
//*		INS:ALA:CDD:RES:CMG
//*

BOOL Alarms_Calibration_Due_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE byte_value;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{
			//* Alarms - Calibration Due - Enable
			sprintf(tier_command,"ENB\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.alarms.calibration_due.info.feature.enable = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alarms.calibration_due.info.feature.enable = DEFAULT_ALARM_CALIBRATION_DUE_ENABLE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alarms.calibration_due.info.feature.enable);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Calibration Due - Custom Enable
			sprintf(tier_command,"CEN\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.alarms.calibration_due.info.feature.custom = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alarms.calibration_due.info.feature.custom = DEFAULT_ALARM_CALIBRATION_DUE_VALUE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alarms.calibration_due.info.feature.custom);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Calibration Due - Value
			sprintf(tier_command,"VAL\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.alarms.calibration_due.info.feature.value = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alarms.calibration_due.info.feature.value = DEFAULT_ALARM_CALIBRATION_DUE_VALUE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alarms.calibration_due.info.feature.value);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Calibration Due - Custom Message
			sprintf(tier_command,"CMG\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
						strncpy(lmi.alarms.calibration_due.info.feature.custom_message, message, SIZE_CUSTOM_MESSAGE);
					else
						strncpy(lmi.alarms.calibration_due.info.feature.custom_message, DEFAULT_ALARM_CALIBRATION_DUE_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);	// (9DP v1.01.15)
//						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					strncpy(lmi.alarms.calibration_due.info.feature.custom_message, DEFAULT_ALARM_CALIBRATION_DUE_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				Format_String(tier_command, lmi.alarms.calibration_due.info.feature.custom_message, SIZE_CUSTOM_MESSAGE);									// (9DP v1.01.04)
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Alarms_Source_Check_Handler(void)
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
//* 	
//*		INS:ALA:SCH:GET:ENB
//*		INS:ALA:SCH:GET:VAL
//*		INS:ALA:SCH:GET:UNI
//*		INS:ALA:SCH:GET:MUL
//*		INS:ALA:SCH:GET:CEN
//*		INS:ALA:SCH:GET:CMG
//*		INS:ALA:SCH:SET:ENB
//*		INS:ALA:SCH:SET:VAL
//*		INS:ALA:SCH:SET:UNI
//*		INS:ALA:SCH:SET:MUL
//*		INS:ALA:SCH:SET:CEN
//*		INS:ALA:SCH:SET:CMG
//*		INS:ALA:SCH:RES:ENB
//*		INS:ALA:SCH:RES:VAL
//*		INS:ALA:SCH:RES:UNI
//*		INS:ALA:SCH:RES:MUL
//*		INS:ALA:SCH:RES:CEN
//*		INS:ALA:SCH:RES:CMG
//*

BOOL Alarms_Source_Check_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE byte_value;
	UINT16 uint16_value;

	process = Process_Handler();						
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);					
		if(message != NULL)
		{
			//* Alarms - Source Check - Enable
			sprintf(tier_command,"ENB\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.alarms.source_check.info.feature.enable = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alarms.source_check.info.feature.enable = DEFAULT_ALARM_SOURCE_CHECK_ENABLE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alarms.source_check.info.feature.enable);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Source Check - Custom Enable
			sprintf(tier_command,"CEN\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.alarms.source_check.info.feature.custom = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alarms.source_check.info.feature.custom = DEFAULT_ALARM_SOURCE_CHECK_CUSTOM_ENABLE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alarms.source_check.info.feature.custom);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Source Check - Value
			sprintf(tier_command,"VAL\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						uint16_value = atoi(message);
						lmi.alarms.source_check.info.feature.reading.value = uint16_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alarms.source_check.info.feature.reading.value = DEFAULT_ALARM_SOURCE_CHECK_VALUE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alarms.source_check.info.feature.reading.value);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Source Check - Units
			sprintf(tier_command,"UNI\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.alarms.source_check.info.feature.reading.units = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alarms.source_check.info.feature.reading.units = DEFAULT_ALARM_SOURCE_CHECK_UNITS;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alarms.source_check.info.feature.reading.units);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Source Check - Multiplier
			sprintf(tier_command,"MUL\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.alarms.source_check.info.feature.reading.multiplier = byte_value + MULTIPLIER_ADJUST;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alarms.source_check.info.feature.reading.multiplier = DEFAULT_ALARM_SOURCE_CHECK_MULTIPLIER;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alarms.source_check.info.feature.reading.multiplier - MULTIPLIER_ADJUST);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Source Check - Custom Message
			sprintf(tier_command,"CMG\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
						strncpy(lmi.alarms.source_check.info.feature.custom_message, message, SIZE_CUSTOM_MESSAGE);
					else
						strncpy(lmi.alarms.source_check.info.feature.custom_message, DEFAULT_ALARM_SOURCE_CHECK_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);	// (9DP v1.01.15)
//						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					strncpy(lmi.alarms.source_check.info.feature.custom_message, DEFAULT_ALARM_SOURCE_CHECK_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				Format_String(tier_command, lmi.alarms.source_check.info.feature.custom_message, SIZE_CUSTOM_MESSAGE);									// (9DP v1.01.04)
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Alarms_Background_Check_Handler(void)
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
//* 	
//*		INS:ALA:BCH:GET:ENB
//*		INS:ALA:BCH:GET:VAL
//*		INS:ALA:BCH:GET:UNI
//*		INS:ALA:BCH:GET:MUL
//*		INS:ALA:BCH:GET:CEN
//*		INS:ALA:BCH:GET:CMG
//*		INS:ALA:BCH:SET:ENB
//*		INS:ALA:BCH:SET:VAL
//*		INS:ALA:BCH:SET:UNI
//*		INS:ALA:BCH:SET:MUL
//*		INS:ALA:BCH:SET:CEN
//*		INS:ALA:BCH:SET:CMG
//*		INS:ALA:BCH:RES:ENB
//*		INS:ALA:BCH:RES:VAL
//*		INS:ALA:BCH:RES:UNI
//*		INS:ALA:BCH:RES:MUL
//*		INS:ALA:BCH:RES:CEN
//*		INS:ALA:BCH:RES:CMG
//*

BOOL Alarms_Background_Check_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE byte_value;
	UINT16 uint16_value;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{
			//* Alarms - Background Check - Enable
			sprintf(tier_command,"ENB\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.alarms.background_check.info.feature.enable = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alarms.background_check.info.feature.enable = DEFAULT_ALARM_BACKGROUND_CHECK_ENABLE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alarms.background_check.info.feature.enable);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Background Check - Custom Enable
			sprintf(tier_command,"CEN\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.alarms.background_check.info.feature.custom = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alarms.background_check.info.feature.custom = DEFAULT_ALARM_BACKGROUND_CHECK_CUSTOM_ENABLE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alarms.background_check.info.feature.custom);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Background Check - Value
			sprintf(tier_command,"VAL\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						uint16_value = atoi(message);
						lmi.alarms.background_check.info.feature.reading.value = uint16_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alarms.background_check.info.feature.reading.value = DEFAULT_ALARM_BACKGROUND_CHECK_VALUE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alarms.background_check.info.feature.reading.value);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Background Check - Units
			sprintf(tier_command,"UNI\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.alarms.background_check.info.feature.reading.units = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alarms.background_check.info.feature.reading.units = DEFAULT_ALARM_BACKGROUND_CHECK_UNITS;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alarms.background_check.info.feature.reading.units);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Background Check - Multiplier
			sprintf(tier_command,"MUL\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.alarms.background_check.info.feature.reading.multiplier = byte_value + MULTIPLIER_ADJUST;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alarms.background_check.info.feature.reading.multiplier = DEFAULT_ALARM_BACKGROUND_CHECK_MULTIPLIER;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alarms.background_check.info.feature.reading.multiplier - MULTIPLIER_ADJUST);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Background Check - Custom Message
			sprintf(tier_command,"CMG\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
						strncpy(lmi.alarms.background_check.info.feature.custom_message, message, SIZE_CUSTOM_MESSAGE);
					else
						strncpy(lmi.alarms.background_check.info.feature.custom_message, DEFAULT_ALARM_BACKGROUND_CHECK_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);	// (9DP v1.01.15)
//						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					strncpy(lmi.alarms.background_check.info.feature.custom_message, DEFAULT_ALARM_BACKGROUND_CHECK_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				Format_String(tier_command, lmi.alarms.background_check.info.feature.custom_message, SIZE_CUSTOM_MESSAGE);										// (9DP v1.01.04)
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Alarms_HV_Low_Handler(void)
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
//* 	
//*		INS:ALA:HVL:GET:ENB
//*		INS:ALA:HVL:GET:VAL
//*		INS:ALA:HVL:GET:CEN
//*		INS:ALA:HVL:GET:CMG
//*		INS:ALA:HVL:SET:ENB
//*		INS:ALA:HVL:SET:VAL
//*		INS:ALA:HVL:SET:CEN
//*		INS:ALA:HVL:SET:CMG
//*		INS:ALA:HVL:RES:ENB
//*		INS:ALA:HVL:RES:VAL
//*		INS:ALA:HVL:RES:CEN
//*		INS:ALA:HVL:RES:CMG
//*

BOOL Alarms_HV_Low_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE byte_value;
	INT16 int16_value;																														// (9DP v1.01.15)

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{
			//* Alarms - HV Low - Enable
			sprintf(tier_command,"ENB\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.alarms.hv_low.info.feature.enable = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alarms.hv_low.info.feature.enable = DEFAULT_ALARM_HV_LOW_ENABLE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alarms.hv_low.info.feature.enable);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - HV Low - Custom Enable
			sprintf(tier_command,"CEN\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.alarms.hv_low.info.feature.custom = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alarms.hv_low.info.feature.custom = DEFAULT_ALARM_HV_LOW_CUSTOM_ENABLE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alarms.hv_low.info.feature.custom);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - HV Low - Value
			sprintf(tier_command,"VAL\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						int16_value = atoi(message);																						// (9DP v1.01.15)
						lmi.alarms.hv_low.info.feature.value = int16_value;																	// (9DP v1.01.15)
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alarms.hv_low.info.feature.value = DEFAULT_ALARM_HV_LOW_VALUE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alarms.hv_low.info.feature.value);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - HV Low - Custom Message
			sprintf(tier_command,"CMG\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
						strncpy(lmi.alarms.hv_low.info.feature.custom_message, message, SIZE_CUSTOM_MESSAGE);								// (9DP v1.01.15)
					else
						strncpy(lmi.alarms.hv_low.info.feature.custom_message, DEFAULT_ALARM_HV_LOW_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);	// (9DP v1.01.15)
//						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					strncpy(lmi.alarms.hv_low.info.feature.custom_message, DEFAULT_ALARM_HV_LOW_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);		// (9DP v1.01.15)
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				Format_String(tier_command, lmi.alarms.hv_low.info.feature.custom_message, SIZE_CUSTOM_MESSAGE);							// (9DP v1.01.04)
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Alarms_HV_High_Handler(void)
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
//* 	
//*		INS:ALA:HVH:GET:ENB
//*		INS:ALA:HVH:GET:VAL
//*		INS:ALA:HVH:GET:CEN
//*		INS:ALA:HVH:GET:CMG
//*		INS:ALA:HVH:SET:ENB
//*		INS:ALA:HVH:SET:VAL
//*		INS:ALA:HVH:SET:CEN
//*		INS:ALA:HVH:SET:CMG
//*		INS:ALA:HVH:RES:ENB
//*		INS:ALA:HVH:RES:VAL
//*		INS:ALA:HVH:RES:CEN
//*		INS:ALA:HVH:RES:CMG
//*

BOOL Alarms_HV_High_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE byte_value;
	INT16 int16_value;																														// (9DP v1.01.15)

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{
			//* Alarms - HV High - Enable
			sprintf(tier_command,"ENB\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.alarms.hv_high.info.feature.enable = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alarms.hv_high.info.feature.enable = DEFAULT_ALARM_HV_HIGH_ENABLE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alarms.hv_high.info.feature.enable);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - HV High - Custom Enable
			sprintf(tier_command,"CEN\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.alarms.hv_high.info.feature.custom = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alarms.hv_high.info.feature.custom = DEFAULT_ALARM_HV_HIGH_CUSTOM_ENABLE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alarms.hv_high.info.feature.custom);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - HV High - Value
			sprintf(tier_command,"VAL\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						int16_value = atoi(message);																						// (9DP v1.01.15)
						lmi.alarms.hv_high.info.feature.value = int16_value;																// (9DP v1.01.15)
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alarms.hv_high.info.feature.value = DEFAULT_ALARM_HV_HIGH_VALUE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alarms.hv_high.info.feature.value);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - HV High - Custom Message
			sprintf(tier_command,"CMG\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
						strncpy(lmi.alarms.hv_high.info.feature.custom_message, message, SIZE_CUSTOM_MESSAGE);								// (9DP v1.01.15)
					else
						strncpy(lmi.alarms.hv_high.info.feature.custom_message, DEFAULT_ALARM_HV_HIGH_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);	// (9DP v1.01.15)
//						return FALSE;				
				}
				else if(process == COMMAND_RESET)
				{
					strncpy(lmi.alarms.hv_high.info.feature.custom_message, DEFAULT_ALARM_HV_HIGH_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);		// (9DP v1.01.15)
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				Format_String(tier_command, lmi.alarms.hv_high.info.feature.custom_message, SIZE_CUSTOM_MESSAGE);							// (9DP v1.01.04)
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Alarms_Low_Reading_Handler(void)
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
//* 	
//*		INS:ALA:LOR:GET:ENB
//*		INS:ALA:LOR:GET:VAL
//*		INS:ALA:LOR:GET:UNI
//*		INS:ALA:LOR:GET:MUL
//*		INS:ALA:LOR:GET:TIM
//*		INS:ALA:LOR:GET:CEN
//*		INS:ALA:LOR:GET:CMG
//*		INS:ALA:LOR:SET:ENB
//*		INS:ALA:LOR:SET:VAL
//*		INS:ALA:LOR:SET:UNI
//*		INS:ALA:LOR:SET:MUL
//*		INS:ALA:LOR:SET:TIM
//*		INS:ALA:LOR:SET:CEN
//*		INS:ALA:LOR:SET:CMG
//*		INS:ALA:LOR:RES:ENB
//*		INS:ALA:LOR:RES:VAL
//*		INS:ALA:LOR:RES:UNI
//*		INS:ALA:LOR:RES:MUL
//*		INS:ALA:LOR:RES:TIM
//*		INS:ALA:LOR:RES:CEN
//*		INS:ALA:LOR:RES:CMG
//*

BOOL Alarms_Low_Reading_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	BYTE process = 0;
	BYTE byte_value;
	UINT16 uint16_value;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{
			//* Alarms - Low Reading - Enable
			sprintf(tier_command,"ENB\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.alarms.low_reading.info.feature.enable = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alarms.low_reading.info.feature.enable = DEFAULT_ALARM_LOW_READING_ENABLE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alarms.low_reading.info.feature.enable);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Low Reading - Custom Enable
			sprintf(tier_command,"CEN\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.alarms.low_reading.info.feature.custom = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alarms.low_reading.info.feature.custom = DEFAULT_ALARM_LOW_READING_CUSTOM_ENABLE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alarms.low_reading.info.feature.custom);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Low Reading - Value
			sprintf(tier_command,"VAL\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						uint16_value = atoi(message);
						lmi.alarms.low_reading.info.feature.reading.value = uint16_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alarms.low_reading.info.feature.reading.value = DEFAULT_ALARM_LOW_READING_VALUE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alarms.low_reading.info.feature.reading.value);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Low Reading - Units
			sprintf(tier_command,"UNI\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.alarms.low_reading.info.feature.reading.units = byte_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alarms.low_reading.info.feature.reading.units = DEFAULT_ALARM_LOW_READING_UNITS;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alarms.low_reading.info.feature.reading.units);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Low Reading - Multiplier
			sprintf(tier_command,"MUL\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						byte_value = (BYTE) atoi(message);
						lmi.alarms.low_reading.info.feature.reading.multiplier = byte_value + MULTIPLIER_ADJUST;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alarms.low_reading.info.feature.reading.multiplier = DEFAULT_ALARM_LOW_READING_MULTIPLIER;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alarms.low_reading.info.feature.reading.multiplier - MULTIPLIER_ADJUST);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Low Reading - Custom Message
			sprintf(tier_command,"CMG\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
						strncpy(lmi.alarms.low_reading.info.feature.custom_message, message, SIZE_CUSTOM_MESSAGE);									// (9DP v1.01.15)
					else
						strncpy(lmi.alarms.low_reading.info.feature.custom_message, DEFAULT_ALARM_LOW_READING_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);	// (9DP v1.01.15)
//						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					strncpy(lmi.alarms.low_reading.info.feature.custom_message, DEFAULT_ALARM_LOW_READING_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);		// (9DP v1.01.15)
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				Format_String(tier_command, lmi.alarms.low_reading.info.feature.custom_message, SIZE_CUSTOM_MESSAGE);								// (9DP v1.01.04)
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		
			//* Alarms - Low Reading - Time
			sprintf(tier_command,"TIM\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						uint16_value = atoi(message);
						lmi.alarms.low_reading.info.time = uint16_value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.alarms.low_reading.info.time = DEFAULT_ALARM_LOW_READING_TIME;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.alarms.low_reading.info.time);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	END ALARMS HANDLERS
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//*	CALIBRATION HANDLERS
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Calibration_Handler(void)
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
//*
//*		INS:CAL:HEA:
//*		INS:CAL:CHK:
//*		INS:CAL:CON:
//*		INS:CAL:OFF:
//*		INS:CAL:HVB:
//*

BOOL Calibration_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response;
	BYTE process;
	BOOL status;

	message = Parse_Message(NULL);
	if(message != NULL)
	{
		sprintf(tier_command, "HEA\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Calibration_Header_Handler();
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_CALIBRATION_HEADER_LOCATION, 2, lmi.calibration.header.buffer, SIZE_CALIBRATION_HEADER);
			}
			return status;
		}
		
		sprintf(tier_command, "CHK\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Calibration_Checkout_Handler();
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_CALIBRATION_CHECKOUT_LOCATION, 2, lmi.calibration.checkout.buffer, SIZE_CALIBRATION_CHECKOUT);
			}
			return status;
		}
		
		sprintf(tier_command, "CON\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Calibration_Constants_Handler();
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_CALIBRATION_CONSTANTS_LOCATION, 2, lmi.calibration.constants.buffer, SIZE_CALIBRATION_CONSTANTS);
			}
			return status;
		}
		
		sprintf(tier_command, "CTR\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Calibration_Controls_Handler();
			return status;
		}
		
		sprintf(tier_command, "OFF\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Calibration_Offsets_Handler();
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_CALIBRATION_OFFSETS_LOCATION, 2, lmi.calibration.offsets.buffer, SIZE_CALIBRATION_OFFSETS);
			}
			return status;
		}
		
		sprintf(tier_command, "HVB\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Calibration_HV_Board_Handler();
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_CALIBRATION_HVB_LOCATION, 2, lmi.calibration.high_voltage_board.buffer, SIZE_CALIBRATION_HVB);
			}
			return status;
		}

#ifdef INSTRUMENT_9D																														// (3D v1.00.04)
		sprintf(tier_command, "CFG\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			status = Calibration_Config_Handler();
			if((status == TRUE) && (Write_EEPROM == TRUE))
			{
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_CALIBRATION_CONFIG_LOCATION, 2, lmi.calibration.config.buffer, SIZE_CALIBRATION_CONFIG);
			}
			return status;
		}
#endif
	}	
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Calibration_Header_Handler(void)
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
//* 	
//*		INS:CAL:HEA:GET:TYP
//*		INS:CAL:HEA:SET:TYP
//*		INS:CAL:HEA:RES:TYP
//*

BOOL Calibration_Header_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response;
	BYTE process;
	BYTE value;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{
			sprintf(tier_command, "TYP\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = (BYTE) atoi(message);
						lmi.calibration.header.info.instrument_type = value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.calibration.header.info.instrument_type = DEFAULT_CALIBRATION_INSTRUMENT_TYPE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.calibration.header.info.instrument_type);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}	
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Calibration_Checkout_Handler(void)
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
//* 	
//*		INS:CAL:CHK:GET:TEC
//*		INS:CAL:CHK:DAT:GET:MON
//*		INS:CAL:CHK:DAT:GET:DAY
//*		INS:CAL:CHK:DAT:GET:YEA
//*		INS:CAL:CHK:SET:TEC
//*		INS:CAL:CHK:DAT:SET:MON
//*		INS:CAL:CHK:DAT:SET:DAY
//*		INS:CAL:CHK:DAT:SET:YEA
//*		INS:CAL:CHK:RES:TEC
//*		INS:CAL:CHK:DAT:RES:MON
//*		INS:CAL:CHK:DAT:RES:DAY
//*		INS:CAL:CHK:DAT:RES:YEA
//*

BOOL Calibration_Checkout_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response;
	BYTE process;
	BYTE value;

	message = Parse_Message(NULL);
	if(message != NULL)
	{
		sprintf(tier_command, "CKD\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			process = Process_Handler();
			if(process != COMMAND_ERROR)
//				return Date_Handler(process, &lmi.calibration.checkout.info.date.month);
				return Date_Handler(process, &lmi.calibration.checkout.info.date);					// (9DP v1.01.21)
			else
				return FALSE;
		}

		process = Post_Process_Handler(message);
		if(process != COMMAND_ERROR)
		{
			message = Parse_Message(NULL);
			if(message != NULL)
			{
				sprintf(tier_command, "TEC\0");
				response = strcmp(message,tier_command);
				if(response == 0)
				{
					Append_Message(tier_command, ColonSeparator);
					if(process == COMMAND_WRITE)
					{
						message = Parse_Message(NULL);
						if(message != NULL)
						{
							strncpy(lmi.calibration.checkout.info.technician, message, LENGTH_CHECKOUT_TECHNICIAN);
						}
						else
						{
							strncpy(lmi.calibration.checkout.info.technician, DEFAULT_CHECKOUT_TECHNICIAN, LENGTH_CHECKOUT_TECHNICIAN);
						}
					}
					else if(process == COMMAND_RESET)
					{
						strncpy(lmi.calibration.checkout.info.technician, DEFAULT_CHECKOUT_TECHNICIAN, LENGTH_CHECKOUT_TECHNICIAN);
					}
					else if(process == COMMAND_READ)
					{
						Write_EEPROM = FALSE;
					}
					else
					{
						return FALSE;
					}
					Format_String(tier_command, lmi.calibration.checkout.info.technician, SIZE_CUSTOM_MESSAGE);								// (9DP v1.01.04)
					strcat((char *)USB_TXMessage, tier_command);																			// (9DP v1.01.15)
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Calibration_Constants_Handler(void)
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
//* 	
//*		INS:CAL:CON:GET:BAT
//*		INS:CAL:CON:GET:HVR
//*		INS:CAL:CON:SET:BAT
//*		INS:CAL:CON:SET:HVR
//*		INS:CAL:CON:RES:BAT
//*		INS:CAL:CON:RES:HVR
//*

BOOL Calibration_Constants_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response;
	BYTE process;
	BYTE range;
	BYTE length;
	UINT16 calibration_constant;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{
			sprintf(tier_command, "BAT\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						calibration_constant = atoi(message);
						lmi.calibration.constants.info.battery = calibration_constant;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.calibration.constants.info.battery = DEFAULT_BATTERY_CAL_CONSTANT;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.calibration.constants.info.battery);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
			
			sprintf(tier_command, "HVR\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						calibration_constant = atoi(message);
						lmi.calibration.constants.info.high_voltage = calibration_constant;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.calibration.constants.info.high_voltage = DEFAULT_HIGH_VOLTAGE_CAL_CONSTANT;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.calibration.constants.info.high_voltage);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}	
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Calibration_Controls_Handler(void)
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
//* 	
//*		INS:CAL:CTR:GET:RNG
//*		INS:CAL:CTR:GET:STR
//*		INS:CAL:CTR:GET:SRT
//*		INS:CAL:CTR:SET:RNG
//*		INS:CAL:CTR:SET:STR
//*		INS:CAL:CTR:SET:SRT
//*		INS:CAL:CTR:RES:RNG
//*		INS:CAL:CTR:RES:STR
//*		INS:CAL:CTR:RES:SRT
//*

BOOL Calibration_Controls_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response;
	BYTE process;
	BYTE range;
	BYTE length;
	BYTE value;
	UINT16 calibration_constant;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{
			sprintf(tier_command, "RNG\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = atoi(message);
						if(!Event_Handler(value, 6, &lmi.calibration.controls.info.fixed_range))
						{
							return FALSE;
						}
					}
					else
					{
						return FALSE;
					}
				}
				else if(process == COMMAND_RESET)
				{
					lmi.calibration.controls.info.fixed_range = DEFAULT_CALIBRATION_CONTROLS_RANGE;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.calibration.controls.info.fixed_range);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
			
			sprintf(tier_command, "STR\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = (BYTE) atoi(message);
						if(!Event_Handler(value, 2, &lmi.calibration.controls.info.stream_data_reading))
							return FALSE;
						stream_report_tick = tick + (DWORD) lmi.calibration.controls.info.stream_report_time;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.calibration.controls.info.stream_data_reading = DEFAULT_CALIBRATION_CONTROLS_STREAM;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.calibration.controls.info.stream_data_reading);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}

			sprintf(tier_command, "SRT\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						lmi.calibration.controls.info.stream_report_time = (BYTE) atoi(message);
						stream_report_tick = tick + (DWORD) lmi.calibration.controls.info.stream_report_time;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.calibration.controls.info.stream_report_time = DEFAULT_CALIBRATION_CONTROLS_STREAM_TIME;
					stream_report_tick = tick + (DWORD) lmi.calibration.controls.info.stream_report_time;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.calibration.controls.info.stream_report_time);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}

			sprintf(tier_command, "LOG\0");															// (9DP v1.01.18)
			response = strcmp(message,tier_command);												// (9DP v1.01.18)
			if(response == 0)																		// (9DP v1.01.18)
			{																						// (9DP v1.01.18)
				Append_Message(tier_command, ColonSeparator);										// (9DP v1.01.18)
				if(process == COMMAND_WRITE)														// (9DP v1.01.18)
				{																					// (9DP v1.01.18)
					message = Parse_Message(NULL);													// (9DP v1.01.18)
					if(message != NULL)																// (9DP v1.01.18)
					{																				// (9DP v1.01.18)
						Save_Config_To_Thumbdrive = (BYTE) atoi(message);							// (9DP v1.01.18)
					}																				// (9DP v1.01.18)
					else																			// (9DP v1.01.18)
						return FALSE;																// (9DP v1.01.18)
				}																					// (9DP v1.01.18)
				else if(process == COMMAND_RESET)													// (9DP v1.01.18)
				{																					// (9DP v1.01.18)
					Save_Config_To_Thumbdrive = DEFAULT_CALIBRATION_CONTROLS_LOG;					// (9DP v1.01.18)
				}																					// (9DP v1.01.18)
				else if(process == COMMAND_READ)													// (9DP v1.01.18)
				{																					// (9DP v1.01.18)
					Write_EEPROM = FALSE;															// (9DP v1.01.18)
				}																					// (9DP v1.01.18)
				sprintf((char *)tier_command,"%d\r\n\0", Save_Config_To_Thumbdrive);				// (9DP v1.01.18)
				strcat((char *)USB_TXMessage, tier_command);										// (9DP v1.01.18)
				return TRUE;																		// (9DP v1.01.18)
			}																						// (9DP v1.01.18)
		}	
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Calibration_Offsets_Handler(void)
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
//* 	
//*		INS:CAL:OFF:GET:ADC
//*		INS:CAL:OFF:SET:ADC
//*		INS:CAL:OFF:RES:ADC
//*		INS:CAL:OFF:GET:DAC
//*		INS:CAL:OFF:SET:DAC
//*		INS:CAL:OFF:RES:DAC
//*

BOOL Calibration_Offsets_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response;
	BYTE process;
	INT16 value;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{
			sprintf(tier_command, "ADC\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = atoi(message);
						lmi.calibration.offsets.info.adc_offset = value;
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.calibration.offsets.info.adc_offset = DEFAULT_ADC_OFFSET;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.calibration.offsets.info.adc_offset);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}

			sprintf(tier_command, "DAC\0");															// (9DP v1.01.15)
			response = strcmp(message,tier_command);												// (9DP v1.01.15)
			if(response == 0)																		// (9DP v1.01.15)
			{																						// (9DP v1.01.15)
				Append_Message(tier_command, ColonSeparator);										// (9DP v1.01.15)
				if(process == COMMAND_WRITE)														// (9DP v1.01.15)
				{																					// (9DP v1.01.15)
					message = Parse_Message(NULL);													// (9DP v1.01.15)
					if(message != NULL)																// (9DP v1.01.15)
					{																				// (9DP v1.01.15)
						value = atoi(message);														// (9DP v1.01.15)
						lmi.calibration.offsets.info.dac_offset = value;							// (9DP v1.01.15)
					}																				// (9DP v1.01.15)
					else																			// (9DP v1.01.15)
						return FALSE;																// (9DP v1.01.15)
				}																					// (9DP v1.01.15)
				else if(process == COMMAND_RESET)													// (9DP v1.01.15)
				{																					// (9DP v1.01.15)
					lmi.calibration.offsets.info.dac_offset = DEFAULT_DAC_OFFSET;					// (9DP v1.01.15)
				}																					// (9DP v1.01.15)
				else if(process == COMMAND_READ)													// (9DP v1.01.15)
				{																					// (9DP v1.01.15)
					Write_EEPROM = FALSE;															// (9DP v1.01.15)
				}																					// (9DP v1.01.15)
				sprintf((char *)tier_command,"%d\r\n\0", lmi.calibration.offsets.info.dac_offset);	// (9DP v1.01.15)
				strcat((char *)USB_TXMessage, tier_command);										// (9DP v1.01.15)
				return TRUE;																		// (9DP v1.01.15)
			}
		}	
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Calibration_HV_Board_Handler(void)
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
//* 	
//*		INS:CAL:HVB:GET:D01
//*		INS:CAL:HVB:SET:D01
//*		INS:CAL:HVB:RES:D01
//*		INS:CAL:HVB:GET:D02
//*		INS:CAL:HVB:SET:D02
//*		INS:CAL:HVB:RES:D02
//*

BOOL Calibration_HV_Board_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response;
	BYTE process;
	INT16 value;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{
#ifdef INSTRUMENT_9D
			//* DAC OFFSET 1
			sprintf(tier_command, "DO1\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = atoi(message);
						lmi.calibration.high_voltage_board.info.dac_1_offset = value;
						HVBUpdateOffset(FALSE);
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.calibration.high_voltage_board.info.dac_1_offset = DEFAULT_DAC_1_OFFSET;
					HVBUpdateOffset(FALSE);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.calibration.high_voltage_board.info.dac_1_offset);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}

			//* DAC OFFSET 2
			sprintf(tier_command, "DO2\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = atoi(message);
						lmi.calibration.high_voltage_board.info.dac_2_offset = value;
						HVBUpdateOffset(FALSE);
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.calibration.high_voltage_board.info.dac_2_offset = DEFAULT_DAC_2_OFFSET;
					HVBUpdateOffset(FALSE);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.calibration.high_voltage_board.info.dac_2_offset);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
#else
			//* DAC OFFSET 1
			sprintf(tier_command, "DO1\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = atoi(message);
						lmi.calibration.high_voltage_board.info.dac_1_offset = value;
						HVBInit(TRUE);
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.calibration.high_voltage_board.info.dac_1_offset = DEFAULT_DAC_1_OFFSET;
					HVBInit(TRUE);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.calibration.high_voltage_board.info.dac_1_offset);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}

			//* DAC OFFSET 2
			sprintf(tier_command, "DO2\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = atoi(message);
						lmi.calibration.high_voltage_board.info.dac_2_offset = value;
						HVBInit(TRUE);
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.calibration.high_voltage_board.info.dac_2_offset = DEFAULT_DAC_2_OFFSET;
					HVBInit(TRUE);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.calibration.high_voltage_board.info.dac_2_offset);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}

			//* THRESHOLD 1 SETPOINT 1
			sprintf(tier_command, "TS1\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = atoi(message);
						lmi.calibration.high_voltage_board.info.threshold_1_setpoint_1 = (UINT16) value;
						HVBUpdateOffset(1);
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.calibration.high_voltage_board.info.threshold_1_setpoint_1 = DEFAULT_THRESHOLD_SETPOINT_1;
					HVBUpdateOffset(1);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.calibration.high_voltage_board.info.threshold_1_setpoint_1);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}

			//* THRESHOLD 1 OFFSET 1
			sprintf(tier_command, "TO1\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = atoi(message);
						lmi.calibration.high_voltage_board.info.threshold_1_offset_1 = value;
						HVBUpdateOffset(1);
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.calibration.high_voltage_board.info.threshold_1_offset_1 = DEFAULT_THRESHOLD_OFFSET_1;
					HVBUpdateOffset(1);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.calibration.high_voltage_board.info.threshold_1_offset_1);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}

			//* THRESHOLD 1 SETPOINT 2
			sprintf(tier_command, "TS2\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = atoi(message);
						lmi.calibration.high_voltage_board.info.threshold_1_setpoint_2 = (UINT16) value;
						HVBUpdateOffset(2);
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.calibration.high_voltage_board.info.threshold_1_setpoint_2 = DEFAULT_THRESHOLD_SETPOINT_2;
					HVBUpdateOffset(2);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.calibration.high_voltage_board.info.threshold_1_setpoint_2);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}

			//* THRESHOLD 1 OFFSET 2
			sprintf(tier_command, "TO2\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = atoi(message);
						lmi.calibration.high_voltage_board.info.threshold_1_offset_2 = value;
						HVBUpdateOffset(2);
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.calibration.high_voltage_board.info.threshold_1_offset_2 = DEFAULT_THRESHOLD_OFFSET_2;
					HVBUpdateOffset(2);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.calibration.high_voltage_board.info.threshold_1_offset_2);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}

			//* WINDOW 1 SETPOINT 1
			sprintf(tier_command, "WS1\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = atoi(message);
						lmi.calibration.high_voltage_board.info.window_1_setpoint_1 = (UINT16) value;
						HVBUpdateOffset(3);
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.calibration.high_voltage_board.info.window_1_setpoint_1 = DEFAULT_WINDOW_SETPOINT_1;
					HVBUpdateOffset(5);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.calibration.high_voltage_board.info.window_1_setpoint_1);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}

			//* WINDOW 1 OFFSET 1
			sprintf(tier_command, "WO1\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = atoi(message);
						lmi.calibration.high_voltage_board.info.window_1_offset_1 = value;
						HVBUpdateOffset(3);
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.calibration.high_voltage_board.info.window_1_offset_1 = DEFAULT_WINDOW_OFFSET_1;
					HVBUpdateOffset(5);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.calibration.high_voltage_board.info.window_1_offset_1);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}

			//* WINDOW 1 SETPOINT 2
			sprintf(tier_command, "WS2\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = atoi(message);
						lmi.calibration.high_voltage_board.info.window_1_setpoint_2 = (UINT16) value;
						HVBUpdateOffset(6);
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.calibration.high_voltage_board.info.window_1_setpoint_2 = DEFAULT_WINDOW_SETPOINT_2;
					HVBUpdateOffset(4);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.calibration.high_voltage_board.info.window_1_setpoint_2);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}

			//* WINDOW 1 OFFSET 2
			sprintf(tier_command, "WO2\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = atoi(message);
						lmi.calibration.high_voltage_board.info.window_1_offset_2 = value;
						HVBUpdateOffset(4);
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.calibration.high_voltage_board.info.window_1_offset_2 = DEFAULT_WINDOW_OFFSET_2;
					HVBUpdateOffset(6);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.calibration.high_voltage_board.info.window_1_offset_2);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}

			//* THRESHOLD 2 SETPOINT 1
			sprintf(tier_command, "TS3\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = atoi(message);
						lmi.calibration.high_voltage_board.info.threshold_2_setpoint_1 = (UINT16) value;
						HVBUpdateOffset(5);
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.calibration.high_voltage_board.info.threshold_2_setpoint_1 = DEFAULT_THRESHOLD_SETPOINT_1;
					HVBUpdateOffset(3);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.calibration.high_voltage_board.info.threshold_2_setpoint_1);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}

			//* THRESHOLD 2 OFFSET 1
			sprintf(tier_command, "TO3\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = atoi(message);
						lmi.calibration.high_voltage_board.info.threshold_2_offset_1 = value;
						HVBUpdateOffset(5);
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.calibration.high_voltage_board.info.threshold_2_offset_1 = DEFAULT_THRESHOLD_OFFSET_1;
					HVBUpdateOffset(3);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.calibration.high_voltage_board.info.threshold_2_offset_1);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}

			//* THRESHOLD 2 SETPOINT 2
			sprintf(tier_command, "TS4\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = atoi(message);
						lmi.calibration.high_voltage_board.info.threshold_2_setpoint_2 = (UINT16) value;
						HVBUpdateOffset(6);
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.calibration.high_voltage_board.info.threshold_2_setpoint_2 = DEFAULT_THRESHOLD_SETPOINT_2;
					HVBUpdateOffset(4);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.calibration.high_voltage_board.info.threshold_2_setpoint_2);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}

			//* THRESHOLD 2 OFFSET 2
			sprintf(tier_command, "TO4\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = atoi(message);
						lmi.calibration.high_voltage_board.info.threshold_2_offset_2 = value;
						HVBUpdateOffset(6);
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.calibration.high_voltage_board.info.threshold_2_offset_2 = DEFAULT_THRESHOLD_OFFSET_2;
					HVBUpdateOffset(4);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.calibration.high_voltage_board.info.threshold_2_offset_2);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}

			//* WINDOW 2 SETPOINT 1
			sprintf(tier_command, "WS3\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = atoi(message);
						lmi.calibration.high_voltage_board.info.window_2_setpoint_1 = (UINT16) value;
						HVBUpdateOffset(7);
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.calibration.high_voltage_board.info.window_2_setpoint_1 = DEFAULT_WINDOW_SETPOINT_1;
					HVBUpdateOffset(7);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.calibration.high_voltage_board.info.window_2_setpoint_1);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}

			//* WINDOW 2 OFFSET 1
			sprintf(tier_command, "WO3\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = atoi(message);
						lmi.calibration.high_voltage_board.info.window_2_offset_1 = value;
						HVBUpdateOffset(7);
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.calibration.high_voltage_board.info.window_2_offset_1 = DEFAULT_WINDOW_OFFSET_1;
					HVBUpdateOffset(7);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.calibration.high_voltage_board.info.window_2_offset_1);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}

			//* WINDOW 2 SETPOINT 2
			sprintf(tier_command, "WS4\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = atoi(message);
						lmi.calibration.high_voltage_board.info.window_2_setpoint_2 = (UINT16) value;
						HVBUpdateOffset(8);
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.calibration.high_voltage_board.info.window_2_setpoint_2 = DEFAULT_WINDOW_SETPOINT_2;
					HVBUpdateOffset(8);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.calibration.high_voltage_board.info.window_2_setpoint_2);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}

			//* WINDOW 2 OFFSET 2
			sprintf(tier_command, "WO4\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						value = atoi(message);
						lmi.calibration.high_voltage_board.info.window_2_offset_2 = value;
						HVBUpdateOffset(8);
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					lmi.calibration.high_voltage_board.info.window_2_offset_2 = DEFAULT_WINDOW_OFFSET_2;
					HVBUpdateOffset(8);
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%d\r\n\0", lmi.calibration.high_voltage_board.info.window_2_offset_2);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
#endif
		}	
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Calibration_Config_Handler(void)
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
//* 	
//*		INS:CAL:CFG:GET:FIL
//*		INS:CAL:CFG:SET:FIL
//*		INS:CAL:CFG:RES:FIL
//*

BOOL Calibration_Config_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response;
	BYTE process;
	float value;

	process = Process_Handler();
	if(process != COMMAND_ERROR)
	{
		message = Parse_Message(NULL);
		if(message != NULL)
		{
			sprintf(tier_command, "FIL\0");
			response = strcmp(message,tier_command);
			if(response == 0)
			{
				Append_Message(tier_command, ColonSeparator);
				if(process == COMMAND_WRITE)
				{
					message = Parse_Message(NULL);
					if(message != NULL)
					{
						uR_filter = atof(message);
						lmi.calibration.config.info.uR_filter = uR_filter;							// (9DP v1.01.13)
					}
					else
						return FALSE;
				}
				else if(process == COMMAND_RESET)
				{
					uR_filter = DEFAULT_CALIBRATION_CONFIG_uR_FILTER;
					lmi.calibration.config.info.uR_filter = uR_filter;
				}
				else if(process == COMMAND_READ)
				{
					Write_EEPROM = FALSE;
				}
				sprintf((char *)tier_command,"%.1f\r\n\0", uR_filter);
				strcat((char *)USB_TXMessage, tier_command);
				return TRUE;
			}
		}	
	}
	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	END CALIBRATION HANDLERS
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//* GENERAL FUNCTIONS
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BYTE Terminate_Message(char *message_string)
//*
//*	Description:
//* 	Validates that the message ends with a CR/LF and determines the message length.
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

BYTE Terminate_Message(char *message_string)
{
	unsigned char length;
	char *response;
	BYTE location;

	length = strlen(message_string);
	if(length > MESSAGE_SIZE)
		return INVALID_MESSAGE_LENGTH;

	response = strchr(message_string, (char) CARRIAGE_RETURN);										// search for a carriage return
	if(response != NULL)
	{
		location = (BYTE) (response - message_string);	
		if((location != 0) && (location < MESSAGE_SIZE))											// if the location is valid
		{
			if(message_string[location - 1] != ':')													// if the previous character is not a ':'
			{
				message_string[location++] = ':';													// replace the carriage return with a ':'
				if(message_string[location] == (BYTE) LINEFEED)										// if the next character is a linefeed
				{
					message_string[location] = 0x00;												// NULL terminate the data string
					return VALID_MESSAGE;															// then the message is valid
				}
			}
			else																					// else
			{
				message_string[location++] = 0x00;													// NULL terminate the data string
				if(message_string[location] == (BYTE) LINEFEED)										// if the next character is a linefeed
				{
					message_string[location] = 0x00;												// NULL terminate the data string
					return VALID_MESSAGE;															// then the message is valid
				}
			}

		}
	}
	return INVALID_MESSAGE_TERMINATION;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Append_Message(char *tier_command, char *separator)
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

void Append_Message(char *tier_command, char *separator)
{
	strcat((char *)USB_TXMessage, tier_command);
	strcat((char *)USB_TXMessage, separator);
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BYTE Parse_Message(char *message_string, )
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

char * Parse_Message(char *message_string)
{
	return strtok(message_string,":");
}

//*
//**************************************************************************************************
//*	Function:
//* 	BYTE Process_Handler(void)
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

BYTE Process_Handler(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response;

	message = Parse_Message(NULL);
	if(message != NULL)
	{
		sprintf(tier_command,"GET");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			return COMMAND_READ;
		}
	
		sprintf(tier_command,"SET");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			return COMMAND_WRITE;
		}
	
		sprintf(tier_command,"RES");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			return COMMAND_RESET;
		}
	}

	return COMMAND_ERROR;
}

//*
//**************************************************************************************************
//*	Function:
//* 	BYTE Post_Process_Handler(char *message)
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

BYTE Post_Process_Handler(char *message)
{
	char tier_command[MESSAGE_SIZE];
	BYTE response;

	sprintf(tier_command,"GET");
	response = strcmp(message,tier_command);
	if(response == 0)
	{
		Append_Message(tier_command, ColonSeparator);
		return COMMAND_READ;
	}

	sprintf(tier_command,"SET");
	response = strcmp(message,tier_command);
	if(response == 0)
	{
		Append_Message(tier_command, ColonSeparator);
		return COMMAND_WRITE;
	}

	sprintf(tier_command,"RES");
	response = strcmp(message,tier_command);
	if(response == 0)
	{
		Append_Message(tier_command, ColonSeparator);
		return COMMAND_RESET;
	}

	return COMMAND_ERROR;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Time_Handler(BYTE process, TYPE_TIME_FORMAT *time_ptr)
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
//*
//*		...HRS
//*		...MIN
//*		...FOR
//*

BOOL Time_Handler(BYTE process, TYPE_TIME_FORMAT *time_ptr)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	unsigned int value;

	message = Parse_Message(NULL);
	if(message != NULL)
	{	
		//* Time - Hours
		sprintf(tier_command,"HRS\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			TimeEvent = HOUR_EVENT;
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
				{
					value = (BYTE) atoi(message);
					if(value < 24)
					{
						time_ptr->hours = value;													// (9DP v1.01.21)
					}
					else
					{
						return FALSE;
					}
				}
				else
				{
					return FALSE;
				}
			}
			else if(process == COMMAND_RESET)
			{
				value = DEFAULT_TIME_HOURS;
				time_ptr->hours = value;															// (9DP v1.01.21)
			}
			else if(process == COMMAND_READ)
			{
				value = time_ptr->hours;															// (9DP v1.01.21)
				Write_EEPROM = FALSE;
			}
			else
			{
				return FALSE;
			}
			sprintf(tier_command, "%02d\0", value);													// changed back to old format (9DP v1.01.18)
			Append_Message(tier_command, ReturnLinefeed);
			return TRUE;
		}
	
		//* Time - Minutes
		sprintf(tier_command,"MIN\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			TimeEvent = MINUTE_EVENT;
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
				{
					value = (BYTE) atoi(message);
					if(value < 60)
					{
						time_ptr->minutes = value;													// (9DP v1.01.21)
					}
					else
					{
						return FALSE;
					}
				}
				else
				{
					return FALSE;
				}
			}
			else if(process == COMMAND_RESET)
			{
				value = DEFAULT_TIME_MINUTES;
				time_ptr->minutes = value;															// (9DP v1.01.21)
			}
			else if(process == COMMAND_READ)
			{
				value = time_ptr->minutes;															// (9DP v1.01.21)
				Write_EEPROM = FALSE;
			}
			else
			{
				return FALSE;
			}
			sprintf(tier_command, "%02d\0", value);													// changed back to old format (9DP v1.01.18)
			Append_Message(tier_command, ReturnLinefeed);
			return TRUE;
		}
	
		//* Time - Seconds
		sprintf(tier_command,"SEC\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			TimeEvent = SECOND_EVENT;
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
				{
					value = (BYTE) atoi(message);
					if(value < 60)
					{
						time_ptr->seconds = value;													// (9DP v1.01.21)
					}
					else
					{
						return FALSE;
					}
				}
				else
				{
					return FALSE;
				}
			}
			else if(process == COMMAND_RESET)
			{
				value = DEFAULT_TIME_MINUTES;
				time_ptr->seconds = value;															// (9DP v1.01.21)
			}
			else if(process == COMMAND_READ)
			{
				value = time_ptr->seconds;															// (9DP v1.01.21)
				Write_EEPROM = FALSE;
			}
			else
			{
				return FALSE;
			}
			sprintf(tier_command, "%02d\0", value);													// changed back to old format (9DP v1.01.18)
			Append_Message(tier_command, ReturnLinefeed);
			return TRUE;
		}
	
		//* Time - Format
		sprintf(tier_command,"FOR\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			TimeEvent = FORMAT_EVENT;
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
				{
					value = (BYTE) atoi(message);
					if((value == 12) || (value == 24))
					{
						time_ptr->format = value;													// (9DP v1.01.21)
					}
					else
					{
						return FALSE;
					}
				}
				else
				{
					return FALSE;
				}
			}
			else if(process == COMMAND_RESET)
			{
				value = DEFAULT_TIME_FORMAT;
				time_ptr->format = value;															// (9DP v1.01.21)
			}
			else if(process == COMMAND_READ)
			{
				value = time_ptr->format;															// (9DP v1.01.21)
				Write_EEPROM = FALSE;
			}
			else
			{
				return FALSE;
			}
			sprintf(tier_command, "%02d\0", value);													// changed back to old format (9DP v1.01.18)
			Append_Message(tier_command, ReturnLinefeed);
			return TRUE;
		}
	}

	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Date_Handler(BYTE process, TYPE_DATE_FORMAT *date_ptr)
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
//* 	
//*		...MON
//*		...DAY
//*		...YEA
//*

//BOOL Date_Handler(BYTE process, char *date_ptr)
BOOL Date_Handler(BYTE process, TYPE_DATE_FORMAT *date_ptr)											// (9DP v1.01.21)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response = 0;
	unsigned int value;
	unsigned int year;

	message = Parse_Message(NULL);
	if(message != NULL)
	{
		//* Date - Month
		sprintf(tier_command,"MON\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			DateEvent = MONTH_EVENT;
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
				{
					value = (BYTE) atoi(message);
					if((value > 0) && (value < 13))
					{
						date_ptr->month = value;													// (9DP v1.01.21)
					}
					else
					{
						return FALSE;
					}
				}
				else
				{
					return FALSE;
				}
			}
			else if(process == COMMAND_RESET)
			{
				value = DEFAULT_DATE_MONTH;
				date_ptr->month = value;															// (9DP v1.01.21)
			}
			else if(process == COMMAND_READ)
			{
				value = date_ptr->month;															// (9DP v1.01.21)
				Write_EEPROM = FALSE;
			}
			else
			{
				return FALSE;
			}
			sprintf(tier_command, "%02d\0", value);													// changed back to old format (9DP v1.01.18)
			Append_Message(tier_command, ReturnLinefeed);
			return TRUE;
		}
	
		//* Date - Day
		sprintf(tier_command,"DAY\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			DateEvent = DAY_EVENT;
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
				{
					value = (BYTE) atoi(message);
					if((value > 0) && (value < 32))
					{
						date_ptr->day = value;														// (9DP v1.01.21)
					}
					else
					{
						return FALSE;
					}
				}
				else
				{
					return FALSE;
				}
			}
			else if(process == COMMAND_RESET)
			{
				value = DEFAULT_DATE_DAY;
				date_ptr->day = value;																// (9DP v1.01.21)
			}
			else if(process == COMMAND_READ)
			{
				value = date_ptr->day;																// (9DP v1.01.21)
				Write_EEPROM = FALSE;
			}
			else
			{
				return FALSE;
			}
			sprintf(tier_command, "%02d\0", value);													// changed back to old format (9DP v1.01.18)
			Append_Message(tier_command, ReturnLinefeed);
			return TRUE;
		}
	
		//* Date - Year
		sprintf(tier_command,"YEA\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			DateEvent = YEAR_EVENT;
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_WRITE)
			{
				message = Parse_Message(NULL);
				if(message != NULL)
				{
					value = atoi(message);
					if((value > 1999) || (value < 2100))
					{
						year = value - 2000;
						date_ptr->year = (BYTE) year;												// (9DP v1.01.21)
					}
					else
					{
						return FALSE;
					}
				}
				else
				{
					return FALSE;
				}
			}
			else if(process == COMMAND_RESET)
			{
				value = DEFAULT_DATE_YEAR;
				date_ptr->year = (BYTE) value;														// (9DP v1.01.21)
				value += 2000;
			}
			else if(process == COMMAND_READ)
			{
				value = date_ptr->year;																// (9DP v1.01.21)
				value += 2000;
				Write_EEPROM = FALSE;
			}
			else
			{
				return FALSE;
			}
			sprintf(tier_command, "%04d\0", value);
			Append_Message(tier_command, ReturnLinefeed);
			return TRUE;
		}
	}

	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Reading_Handler(BYTE process, float *value_ptr, char *units_ptr, char *multiplier_ptr)
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
//*
//*		...VAL
//*		...UNI
//*		...MUL
//*

BOOL Reading_Handler(BYTE process, float *value_ptr, char *units_ptr, char *multiplier_ptr)
{
	BYTE response = 0;
	char tier_command[MESSAGE_SIZE];
	char *message;
	char units, multiplier;
	float value;

	message = Parse_Message(NULL);
	if(message != NULL)
	{	
		//* Reading - Value
		sprintf(tier_command,"VAL\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_READ)
			{
				sprintf(tier_command, "%.3f\r\n\0", *value_ptr);
				strcat((char *)USB_TXMessage, tier_command);
				Write_EEPROM = FALSE;
				return TRUE;
			}
			return FALSE;
		}
	
		//* Reading - Units
		sprintf(tier_command,"UNI\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_READ)
			{
				units = *units_ptr;
				if(units >= END_UNIT_TYPE)
				{
					units = NO_UNITS;
					*units_ptr = units;
				}
				sprintf(tier_command, "%d\r\n\0", units);
				strcat((char *)USB_TXMessage, tier_command);
				Write_EEPROM = FALSE;
				return TRUE;
			}
			return FALSE;
		}
	
		//* Reading - Multiplier
		sprintf(tier_command,"MUL\0");
		response = strcmp(message,tier_command);
		if(response == 0)
		{
			Append_Message(tier_command, ColonSeparator);
			if(process == COMMAND_READ)
			{
				if((*multiplier_ptr > NANO) && (*multiplier_ptr < TERA))
				{
					multiplier = *multiplier_ptr - PICO;
				}
				else
				{
					multiplier = 0;
					*multiplier_ptr = multiplier;
				}
				sprintf(tier_command, "%d\r\n\0", multiplier);
				strcat((char *)USB_TXMessage, tier_command);
				Write_EEPROM = FALSE;
				return TRUE;
			}
		}
	}

	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Event_Handler(BYTE value, BYTE maxvalue, *event)
//*
//*	Description:
//* 	This routine stores "value" if it is less than "maxvalue".
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

BOOL Event_Handler(BYTE value, BYTE maxvalue, BYTE *event)
{
	if(value >= maxvalue)
		return FALSE;
	else
		*event = value;
	return TRUE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Validate_Detector(char value)
//*
//*	Description:
//* 	This routine determines if the detector is valid.
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
//* 	(9DP v1.01.02)
//*

BOOL Validate_Detector(char value)
{
	value &= 0x7F;																					
	if(value < (char) NUMBER_OF_INTERNAL_DETECTORS)
		return TRUE;
	else if((value <= (char) NUMBER_OF_EXTERNAL_DETECTORS) && (value != 0))
		return TRUE;
	else
		return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Format_String(char *generic_string, char *message, UINT8 message_length)
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

void Format_String(char *generic_string, char *message, UINT8 message_length)
{
	char message_string[51];

	strncpy(message_string, message, message_length);
	message_string[message_length] = '\0';
	sprintf(generic_string,"%s\r\n\0",message_string);
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Force_Screen_Redraw(void)
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
//* 	(9DP v1.01.20)
//*

void Force_Screen_Redraw(void)
{
	if(screenState == MONITOR_RADIATION)
	{									
		screenState = CREATE_ELEMENT;	
		DisplayDetachOnce = TRUE;		
	}									
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	END GENERAL FUNCTIONS HANDLER
//**************************************************************************************************
//**************************************************************************************************

//*
//**************************************************************************************************
//**************************************************************************************************
//* LUDLUM MEASUREMENTS INC. COPYRIGHT (C) 2012
//**************************************************************************************************
//**************************************************************************************************
