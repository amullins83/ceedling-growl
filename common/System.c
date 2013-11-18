//**************************************************************************************************
//**************************************************************************************************
//*																		
//*	Filename:		System.c
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
//* n2          09/13/13    Austin Mullins
//* - Reduced hold_cnt max for Check_Shutdown from 150 to 5: shutdown delay is 2 s total.
//*
//*
//*	~~~~~~~~	07/12/11	James Comstock
//*	- Original release
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
//**************************************************************************************************

#include "instrument.h"
#include "System.h"
#include "Definitions.h"
#include "SSD1906.h"
#include "Screens.h"

////**************************************************************************************************
////* Control Button Configuration
//
//#define CTRLBTN_XINDENT 							0
//#define CTRLBTN_HEIGHT  							35
//#define CTRLBTN_WIDTH   							(((GetMaxX()+1)-(CTRLBTN_XINDENT*2))/4)
//
//#define CtrlBtnTop() 								(GetMaxY()-CTRLBTN_HEIGHT)
//#define CtrlBtnBottom() 							GetMaxY()
//#define CtrlBtnLeft(column)  						(((column+1)*CTRLBTN_XINDENT)+(column*CTRLBTN_WIDTH))
//#define CtrlBtnRight(column)  					((column+1)*(CTRLBTN_XINDENT+CTRLBTN_WIDTH))
//
//**************************************************************************************************
//**************************************************************************************************
//* External Memory Declarations
//**************************************************************************************************
//**************************************************************************************************

extern TYPE_INSTRUMENT lmi;
extern COMPUTER_STATE PC_State;
extern APP_STATE App_State_Keyboard;
extern APP_STATE App_State_Thumbdrive;
extern APP_STATE App_State_FTDI;
extern IMAGE_STATE imageState;
extern TYPE_MESSAGE_BOX pmsgScreen;
extern SCREEN_STATES screenState;
extern SCREEN_STATES PreviousScreenState;
extern SCREEN_STATES nextscreenState;

extern float High_Voltage_Reading;
extern float Temperature_Reading;
extern float High_Voltage_Temperature_Reading;
extern float Battery_Reading;
extern float Rate_uR_hr;
extern float Rate_Accumulation;
extern float Peak_Rate_Reading;
extern float Radiation_Alert_Rate;																	// Rate Alert Level (9DP v1.01.08)
extern float Radiation_Alarm_Rate;																	// Rate Alarm Level (9DP v1.01.08)
extern float Integrate_Alert_Rate;																	// Rate Alert Level (9DP v1.01.08)
extern float Integrate_Alarm_Rate;																	// Rate Alarm Level (9DP v1.01.08)
extern float Chamber_Pressure_PSI;																	// (9DP v1.02.03)
extern float Signal_Voltage;																		// (9DP v1.02.06)
extern float Reading_Average;																		// (9DP v1.03.02)
extern float Temperature_Average;																	// (9DP v1.03.02)

extern DWORD tick;
extern DWORD wait_state_tick;	
extern DWORD log_file_write_tick;
extern DWORD message_event_tick;
extern DWORD MemoryAddress;
extern UINT32 Period;
extern DWORD integrate_update_tick;																	// integrate update timer (1/2 seconds)
extern DWORD system_error_tick;
extern DWORD debugValue;																			// (9DP v1.02.03)

extern WORD foreground;
extern WORD background;
extern XCHAR ExposureValue[7];
extern UINT16 Full_Scale_Arc_Range[7][2];

extern signed short MenuLine;
extern signed short SubMenuLine;
extern signed short LastMenuLine;

extern char DisplayMessage[29];
extern char Total_Chars;
extern char Last_Total_Chars;

extern BYTE USB_RXMessage[MESSAGE_SIZE];
extern BYTE USB_TXMessage[MESSAGE_SIZE];
extern BYTE Audio_IO_OutLAT;
extern BYTE Present_Level;
extern BYTE Audio_Mode;
extern BYTE Audio_Div;
extern BYTE Audio_Operating_Mode;
extern BYTE USBDeviceAddress;
extern BYTE Backlight_Mode;
extern BYTE Current_Detector;
extern BYTE Current_View;
extern BYTE Current_Function_Index;
extern BYTE ExposureMultiplier;
extern BYTE ExposureUnits;
extern BYTE ExposureRange;
extern BYTE USB_Port_Attachment;
extern BYTE Default_Rate_Units;
extern BYTE New_Alarm;
extern BYTE ViewMenu;
extern BYTE DetectorMenu;
//extern UINT8 Error_Recent;
//extern UINT8 Error_Last;
extern BYTE Range_Number;																			// (Test v0.00.06)

//extern SHORT Default_Dose_Multiplier;
//extern SHORT Default_Rate_Multiplier;
extern BYTE Default_Dose_Units;
extern BYTE Save_Config_To_Thumbdrive;																// changed to BYTE value (9DP v1.02.08)

//extern BYTE menu_line[];																			// (9DP v1.01.01)

extern BOOL Drained_Battery;
extern BOOL Alert_Clear_Flag;
extern BOOL Alarm_Clear_Flag;
extern BOOL Transmit_Data;
extern BOOL DisplayDetachOnce;
extern BOOL Data_Logging_Enabled;
extern BOOL Data_Logging_Header_Sent;
extern BOOL Update_Log_File;
extern BOOL Power_Up_Report_Integrate;
extern BOOL editScreen;
extern BOOL editMode;
extern BOOL Audio_Mute;
extern BOOL Audio_Enable;
extern BOOL System_Shutdown;
extern BOOL Power_Down_Report_Integrate;
extern BOOL Wait_State_Event;
extern BOOL Message_Event_Flag;
extern BOOL Flag_Status_Text_Displayed;
extern BOOL Light_On;

extern BOOL Integrate_Alert_Flag;
extern BOOL Integrate_Alert_Ack_Flag;
extern BOOL Integrate_Alarm_Flag;
extern BOOL Integrate_Alarm_Ack_Flag;

//extern BOOL Radiation_Alert_Flag;																	// (v1.01.11)
//extern BOOL Radiation_Alert_Ack_Flag;																// (v1.01.11)
//extern BOOL Radiation_Alarm_Flag;																	// (v1.01.11)
//extern BOOL Radiation_Alarm_Ack_Flag;																// (v1.01.11)

extern BOOL System_Error_Flag;

extern BOOL Flag_Status_Text_Displayed;
extern BOOL Flag_Note_Text_Displayed;

//extern BOOL Save_Config_To_Thumbdrive;															// (9DP v1.01.18)

extern BOOL MenuSelect;																				// (Test v0.00.01)

extern BOOL Report_Temperature_Offset;																// (9DP v1.02.01)

#ifdef TEST_MODE

extern float Light_Sensor_Voltage;																	// (Test v0.00.01)
extern float High_Voltage_1;																		// (Test v0.00.01)
extern float High_Voltage_2;																		// (Test v0.00.01)
extern float Input_Voltage_1;																		// (Test v0.00.01)
extern float Input_Voltage_2;																		// (Test v0.00.01)
extern float Battery_Voltage;																		// (Test v0.00.01)
extern float Display_Voltage;																		// (Test v0.00.04)

extern DWORD test_tick;																				// (Test v0.00.03)

extern BOOL Test_Update;																			// (Test v0.00.03)

#endif

//**************************************************************************************************
//**************************************************************************************************
//* Memory Declarations
//**************************************************************************************************
//**************************************************************************************************

FSFILE * LogFile								= NULL;
FSFILE * ReportFile								= NULL;
FSFILE * InstrumentFile							= NULL;
FILE_STATE USB_File_State						= FILE_NOT_OPEN;

// UINT32 hold_cnt 								= 0;

volatile BYTE File_RXMessage[MESSAGE_SIZE];
volatile BYTE File_TXMessage[MESSAGE_SIZE];
volatile BYTE TXMessageIndex					= 0;

BYTE USBData[128];
BYTE USB_buffer_index 							= 0;
BYTE NumOfBytesRcvd								= 0;
BYTE ErrorDriver								= 0;
BYTE ErrorCounter								= 0;
BYTE NUM_Lock_Pressed							= 0;
BYTE CAPS_Lock_Pressed 							= 0;
BYTE HeldKeyCount 								= 0;
BYTE HeldKey									= 0;

volatile UINT8 Error_Recent 					= 0;												// moved here from screens.c (9DP v1.01.04)
volatile UINT8 Error_Last 						= 0;												// moved here from screens.c (9DP v1.01.04)
volatile UINT8 Last_Saved_Error_Number 			= 0;												// moved here from screens.c (9DP v1.01.04)
volatile UINT8 Error_Buffer[NUMBER_OF_ERRORS];														// moved here from screens.c (9DP v1.01.04)

volatile BOOL Update_Data_Stream				= FALSE;
volatile BOOL ReportBufferUpdated				= FALSE;
volatile BOOL LED_Key_Pressed 					= FALSE;
volatile BOOL Custom_Error_Pending 				= FALSE;											// moved here from screens.c (9DP v1.01.04)

const XCHAR CountDownTxt[] 						= {'W','A','I','T',0};

volatile BOOL Flag_Custom_Msg_Enabled 			= FALSE;											// moved here from screens.c (9DP v1.01.11)

//**************************************************************************************************
//**************************************************************************************************
//*	Global Test Variables
//**************************************************************************************************
//**************************************************************************************************

#ifdef TEST_MODE

unsigned int color_array[] = {	BLACK,
								BRIGHTBLUE,
								BRIGHTGREEN,
								BRIGHTCYAN,
								BRIGHTRED,
								BRIGHTMAGENTA,
								BRIGHTYELLOW,
								BLUE,
								GREEN,
								CYAN,
								RED,
								MAGENTA,
								BROWN,
								LIGHTGRAY,
								DARKGRAY,
								LIGHTBLUE,
								LIGHTGREEN,
								LIGHTCYAN,
								LIGHTRED,
								LIGHTMAGENTA,
								YELLOW,
								WHITE,
								GRAY0,
								GRAY1,
								GRAY2,
								GRAY3,
								GRAY4,
								GRAY5,
								GRAY6};

#endif

//**************************************************************************************************
//**************************************************************************************************
//*	INITIALIZATION ROUTINES
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void SystemInit(void)
//*
//*	Description:
//* 	Configure the system clock, the processor port pins, and interrupt processing.
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

void SystemInit(void)
{
	int value;
	DWORD configRegister;																			// (9DP v1.02.00)

//	SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);									// Configure cache, wait states, and peripheral bus clock. (hidden 9DP v1.02.00)
//																									// Configure the device for maximum performance but do not change the PBDIV.
//																									// Given the options, this function will change the flash wait states, RAM
//																									// wait state and enable prefetch cache but will not change the PBDIV.
//																									// The PBDIV value is already set via the pragma FPBDIV option above..

	configRegister = DEVCFG2 & 0x00000070;															// (9DP v1.02.00)
	if(configRegister == 0x00000030)																// FPLLMULT = 3 [18x] (9DP v1.02.00)
	{
		SYSTEMConfig(SYS_FREQ_72MHz, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);							// Configure cache, wait states, and peripheral bus clock. (9DP v1.02.00)
																									// Configure the device for maximum performance but do not change the PBDIV.
																									// Given the options, this function will change the flash wait states, RAM
																									// wait state and enable prefetch cache but will not change the PBDIV.
																									// The PBDIV value is already set via the pragma FPBDIV option above..
	}
	else if(configRegister == 0x00000050)															// FPLLMULT = 5 [20x] (9DP v1.02.00)
	{
		SYSTEMConfig(SYS_FREQ_80MHz, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);							// Configure cache, wait states, and peripheral bus clock. (9DP v1.02.00)
																									// Configure the device for maximum performance but do not change the PBDIV.
																									// Given the options, this function will change the flash wait states, RAM
																									// wait state and enable prefetch cache but will not change the PBDIV.
																									// The PBDIV value is already set via the pragma FPBDIV option above..
	}
	else																							// (9DP v1.02.00)
	{
		Shutdown_Power();																			// (9DP v1.02.00)
	}

	PORTSetPinsDigitalOut(IOPORT_A, BIT_0 | BIT_1 | BIT_4 | BIT_5 | BIT_6 | BIT_7 | BIT_9 | BIT_10);
	PORTSetPinsDigitalIn(IOPORT_B, BIT_0 | BIT_1 | BIT_3 | BIT_4 | BIT_8 | BIT_9);
	PORTSetPinsDigitalOut(IOPORT_B, BIT_2 | BIT_5 | BIT_10 | BIT_11 | BIT_12 | BIT_13 | BIT_14 | BIT_15);
	PORTSetPinsDigitalIn(IOPORT_C, BIT_1 | BIT_3);
	PORTSetPinsDigitalOut(IOPORT_C, BIT_2 | BIT_4);
	PORTSetPinsDigitalIn(IOPORT_C, BIT_13);
	PORTSetPinsDigitalIn(IOPORT_D, BIT_2 | BIT_9 | BIT_14);
	PORTSetPinsDigitalOut(IOPORT_D, BIT_0 | BIT_1 | BIT_3 | BIT_4 | BIT_5 | BIT_10 | BIT_11 | BIT_15);

//	PORTSetPinsDigitalIn(IOPORT_E, BIT_8);															// (v1.0.5) (hidden 9DP v1.01.21)
//	PORTSetPinsDigitalOut(IOPORT_E, BIT_9);															// (v1.0.5) (hidden 9DP v1.01.21)
//	PORTClearBits(IOPORT_E, BIT_9);																	// (v1.0.5) (hidden 9DP v1.01.21)
	PORTSetPinsDigitalIn(IOPORT_E, BIT_8 | BIT_9);													// (9DP v1.01.21)

	PORTSetPinsDigitalIn(IOPORT_F, BIT_2 | BIT_13);
	PORTSetPinsDigitalOut(IOPORT_F, BIT_3 | BIT_4 | BIT_5 | BIT_8 | BIT_12);
	PORTSetPinsDigitalIn(IOPORT_G, BIT_12);
	PORTSetPinsDigitalOut(IOPORT_G, BIT_6 | BIT_7 | BIT_8 | BIT_9 | BIT_13 | BIT_14 | BIT_15);

	AD1PCFG = 0xFFFF;   																			// Set analog pins to digital.
	DDPCONbits.TROEN = 0;																			// disable TRACE Pins
	DDPCONbits.JTAGEN = 0;																			// disable JTAG Pins
//	value = SYSTEMConfigWaitStatesAndPB(GetSystemClock());											// (hidden 9DP v1.02.00)
	if(configRegister == 0x00000030)																// FPLLMULT = 3 [18x] (9DP v1.02.00)
	{
		value = SYSTEMConfigWaitStatesAndPB(GetSystemClock_72MHz());								// (9DP v1.02.00)
	}
	else																							// FPLLMULT = 5 [20x] (9DP v1.02.00)
	{
		value = SYSTEMConfigWaitStatesAndPB(GetSystemClock_80MHz());								// (9DP v1.02.00)
	}
	CheKseg0CacheOn();
    INTEnableSystemMultiVectoredInt();																// enable multi-vector interrupts

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void TimersInit(void)
//*
//*	Description:
//* 	Configure and open all universal timers.
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

void TimersInit(void)
{
    OpenTimer1(T1_ON | T1_SOURCE_EXT | T1_PS_1_1, T1_TICK);											// configure Timer 1 using external clock, prescaler 1
    ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_6);													// set up the timer interrupt with a priority of 6
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void I2CInit(void)
//*
//*	Description:
//* 	Enables the I2C2 channel the sets up the peripherals.
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

void I2CInit(void)
{
	DWORD configRegister;																			// (9DP v1.02.00)

//	OpenI2C2(I2C_ON, BRG_VAL);																		// Enable channel

	configRegister = DEVCFG2 & 0x00000070;															// (9DP v1.02.00)
	if(configRegister == 0x00000030)																// FPLLMULT = 3 [18x] (9DP v1.02.00)
	{
		OpenI2C2(I2C_ON, BRG_VAL_72MHz);															// Enable channel (9DP v1.02.00)
	}	
	else if(configRegister == 0x00000050)															// FPLLMULT = 5 [20x] (9DP v1.02.00)
	{
		OpenI2C2(I2C_ON, BRG_VAL_80MHz);															// Enable channel (9DP v1.02.00)
	}	
	else																							// (9DP v1.02.00)
	{
		Shutdown_Power();																			// (9DP v1.02.00)
	}

	setupi2c2_peripherals();																		// Initialize i2c devices

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void DisplayInit(void)
//*
//*	Description:
//* 	None
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

void DisplayInit(void)
{
	ResetDevice();																					// reset solomon systech device
	GOLInit();																						// Initialize graphics library and create default style
	InitializeBackLight();																			// setup backlight PWM operations and turn-on
	Set_Backlight_Level(DEFAULT_BACKLIGHT_AUTO_HIGH);												// setup backlight
	SetColor(WHITE);																				// set the screen background color
	ClearDevice();																					// clears screen with current color
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void MemoryInit(void)
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

void MemoryInit(void)
{
	SST39_RST_TRIS_OUT;																				// set SST39 chip RST line for output
	SST39_CE_TRIS_OUT;																				// set SST39 chip enable for output
	SST39_OE_TRIS_OUT;																				// set SST39 output enable for output
	SST39_WE_TRIS_OUT;																				// set SST39 write enable for output

	SST39_OE_LAT_BIT_SET;																			// set OE - disable output
	SST39_CE_LAT_BIT_SET;																			// set CE High - Disable SST39V Chip
	SST39_WE_LAT_BIT_SET;																			// set WE - write disabled

	SST39_RST_LAT_BIT_CLR;																			// Reset SST39 chip
	DelayMs(1);																						// short wait
	SST39_RST_LAT_BIT_SET;																			// Set Reset line High on SST39 chip for normal operations

//	while(PMMODEbits.BUSY);
    PMPWaitBusy();         																			// (9DP v1.01.20)
	PMCON		= 0x0000;																			// clear all
	PMCONSET  	= 0x0300;																			// PMP off (bit 15)
																									// continue operation in debug (bit 14)
																									// continue operation in idle (bit 13)
																									// select separate address/data lines (bits 12-11)
																									// use Schmitt trigger input buffer (bit 10)
																									// enable write strobe port (bit 9)
																									// enable read strobe port (bit 8)
																									// PMCS2 and PMCS1 function as address bits (bits 7-6)
																									// address latch active low (bit 5)
																									// chip select 1 active low (bit 4)
																									// chip select 0 active low (bit 3)
																									// ignore (bit 2)
																									// write select active low (bit 1)
																									// read select active low (bit 0)

	PMMODE		= 0x0000;																			// clear all
	PMMODESET 	= 0x06FF;																			// read only (bit 15)
																									// no interrupt (bits 14-13)
																									// no increment of address (bits 12-11)	
																									// select 16 bit data mode (bit 10)
																									// select master mode 2 (bits 9-8)
																									// data setup wait 4 TPB (bits 7-6)
																									// data wait 16 TPB (bits 5-2)
																									// data hold wait 4 TPB write/3 TPB read (bits 1-0)

	PMAEN		= 0x0000;																			// clear all
	PMAENSET 	= 0xFFFF;																			// all lines function as address lines (bits 15-0)

    PMPWaitBusy();         																			// (9DP v1.01.20)
	PMADDR		= 0x0000;																			// clear all
    PMPWaitBusy();         																			// (9DP v1.01.20)
																									// chip select 2 inactive (pin 15)
																									// chip select 1 inactive (pin 14)


	PMCONSET 	= 0x8000;																			// turn on parallel master port

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void TimerInit(void)
//*
//*	Description:
//* 	This function initializes the tick timer.  It configures and starts the timer, and enables
//*		the timer interrupt.	
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

void TimerInit(void)
{
    T4CON = 0x0000; 																				// Stop and Init Timer
    T4CON = 0x0060;
    TMR4 = 0x0000; 																					// Clear timer register
    PR4 = 0x7FFF; 																					// Load period register
    IPC4SET = 0x00000005; 																			// Set priority level=1 and subpriority=1
    IFS0bits.T4IF = 0;
    IEC0bits.T4IE = 1;
    T4CONSET = 0x8000;																				// Start Timer
    return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void SchemeInit(void)
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

void SchemeInit(void)
{
	altScheme = GOLCreateScheme(); 
	altScheme->EmbossDkColor 		= BLUE;
	altScheme->EmbossLtColor 		= LIGHTBLUE;
	altScheme->TextColor0 			= BRIGHTGREEN; 													// text color for button NOT pressed
    altScheme->TextColor1 			= BRIGHTRED; 													// text color for button pressed
    altScheme->TextColorDisabled 	= DARKGRAY;
	altScheme->Color0 				= BRIGHTBLUE;
    altScheme->Color1 				= LIGHTBLUE;	
    altScheme->ColorDisabled 		= LIGHTGRAY;
	altScheme->CommonBkColor 		= background;
	altScheme->pFont 				= (void*)&GOLFontDefault;										// &Arial_Normal_12

	btnScheme = GOLCreateScheme(); 
	btnScheme->EmbossDkColor 		= DARKGRAY;
	btnScheme->EmbossLtColor 		= LIGHTGRAY;
	btnScheme->TextColor0 			= WHITE;														// text color for button
    btnScheme->TextColor1 			= WHITE;	 													// text color for button pressed
    btnScheme->TextColorDisabled 	= DARKGRAY;
	btnScheme->Color0 				= BLUE;
    btnScheme->Color1 				= BRIGHTBLUE;	
    btnScheme->ColorDisabled 		= LIGHTGRAY;
	btnScheme->CommonBkColor 		= background;
	btnScheme->pFont 				= (void*)&GOLFontDefault;										// &Arial_Normal_12
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void VariableInit(void)
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

void VariableInit(void)
{
	UINT16 i;

	pArcScreen		= NULL;																			// initialize pointer to NULL
	pNavList		= NULL;		 																	// initialize pointer to NULL
	pSettingsScrn	= NULL;		 																	// initialize pointer to NULL
	pControlsScrn	= NULL;		 																	// initialize pointer to NULL
	pDetectorScrn	= NULL;		 																	// initialize pointer to NULL
	pAlertScrn		= NULL;		 																	// initialize pointer to NULL
	pAlarmScrn		= NULL;		 																	// initialize pointer to NULL
	pFunctionScrn	= NULL;		 																	// initialize pointer to NULL
	pColorsScrn		= NULL;																			// initialize pointer to NULL
	ADC_CONVST_BUSY = 0;																			// set ADC CONVST/BUSY pin low
	pArcScreen = GOLGetList();																		// save the current list
	GOLNewList();																					// create new list

	for (i = 0; i < sizeof(USBData) / sizeof(BYTE); i++) 
	{
		USBData[i] = 0;
	}	
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void RTCCInit(void)
//*
//*	Description:
//*		Set the time/date values in the RTCC registers and enable the RTCC
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

void RTCCInit(void)
{
    unsigned int int_status;

	int_status = INTDisableInterrupts();															// Disable all interrupts
	SYSKEY = 0xAA996655;																			// write first unlock key to SYSKEY
	SYSKEY = 0x556699AA;																			// write second unlock key to SYSKEY
	RTCCONSET = 0x00000008;																			// set RTCWREN in RTCCONSET to enable writes
	INTRestoreInterrupts(int_status);																// restore interrupt status
	RTCCUpdate();																					// update internal RTC from supervisor RTC

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	END OF INITIALIZATION ROUTINES
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//*	SYSTEM ROUTINES
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	WORD ExternalMemoryCallback(EXTDATA* memory, LONG offset, WORD nCount, void* buffer)
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

#define SST39_MEMORY 0

WORD ExternalMemoryCallback(EXTDATA* memory, LONG offset, WORD nCount, void* buffer)
{
	if(memory->ID == SST39_MEMORY)
	{
		SST39ReadArray(memory->address+offset, (WORD*)buffer, nCount); 								// address to read from
    }

    return nCount;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	WORD GetADCValue(BYTE index)
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
//* 	ADC Address = 0x46 (010 0011x)
//*

WORD GetADCValue(BYTE index)
{	
	unsigned int t;
	WORD ADCSampleReading = 0;
	WORD ADCSampleTotal = 0;
	WORD ADCSampleAverage = 0;

	switch(index)
	{
		case ADC_PORT0_SIGNAL1:
		case ADC_PORT1_SIGNAL2:
		case ADC_PORT2_HV_REF:
		case ADC_PORT4_DISPLAY:
		case ADC_PORT5_EXTRA1:
		case ADC_PORT6_EXTRA2:
		{
			for(t = 0, ADCSampleTotal = 0; t < 4; t++)
			{
				ADCSampleReading = i2c2ReadADC(i2cADDR_MAIN_ADC, index, 0x00, 0x00);
				ADCSampleTotal += ADCSampleReading;
			}
			ADCSampleAverage = ADCSampleTotal / 4;
			break;
		}
		case ADC_PORT3_BATTERY:
		{
			for(t = 0, ADCSampleTotal = 0; t < ADC_BATTERY_SAMPLES; t++)
			{
				ADCSampleReading = i2c2ReadADC(i2cADDR_MAIN_ADC, 0xB0, 0x00, 0x00);
				ADCSampleTotal += ADCSampleReading;
			}
			ADCSampleAverage = ADCSampleTotal / ADC_BATTERY_SAMPLES;
			break;
		}
		case ADC_PORT7_LIGHT:
		{
			for(t = 0, ADCSampleTotal = 0; t < ADC_LIGHT_SAMPLES; t++)
			{
				ADCSampleReading = i2c2ReadADC(i2cADDR_MAIN_ADC, 0xF0, 0x00, 0x00);
				ADCSampleTotal += ADCSampleReading;
			}
			ADCSampleAverage = ADCSampleTotal / ADC_LIGHT_SAMPLES;
			break;
		}
	}

	if(ADCSampleAverage > lmi.calibration.offsets.info.adc_offset)
		ADCSampleAverage -= lmi.calibration.offsets.info.adc_offset;
	else
		ADCSampleAverage = 0;

	return ADCSampleAverage;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	END SYSTEM ROUTINES
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//*	REAL TIME CLOCK ROUTINES
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void RTCCUpdate(void)
//*
//*	Description:
//*		Update the time/date values in the RTCC registers with the values in the supervisor
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

void RTCCUpdate(void)
{
	unsigned long time;
	unsigned long date;
	unsigned char BCDbyte;

	i2cRead_PIC18();																				// read from the PIC18

	RTCCONCLR = 0x00008000;																			// turn off the RTC
	while(RTCCON & 0x40);																			// wait for CLK to turn off
	
	BCDbyte = convert_BCD(lmi.settings.clock.info.time.hours);
	time = BCDbyte;
	BCDbyte = convert_BCD(lmi.settings.clock.info.time.minutes);
	time = ((time << 8) | BCDbyte);
	BCDbyte = convert_BCD(lmi.settings.clock.info.time.seconds);
	time = ((time << 8) | BCDbyte);
	time = ((time << 8) | 0x00); 
	RTCTIME = time;																					// update the RTCC time register

	BCDbyte = convert_BCD(lmi.settings.calendar.info.date.year);
	date = BCDbyte;
	BCDbyte = convert_BCD(lmi.settings.calendar.info.date.month);
	date = ((date << 8) | BCDbyte);
	BCDbyte = convert_BCD(lmi.settings.calendar.info.date.day);
	date = ((date << 8) | BCDbyte);
	date = ((date << 8) | 0x00);																	// add day of the week if needed
	RTCDATE = date;																					// update the RTCC date register

	RTCCONSET = 0x00008000;																			// turn on the RTCC
	while(!(RTCCON & 0x40));																		// wait for the clock to turn on

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void RTCCRead(void)
//*
//*	Description:
//*		Read the time/date values from the RTCC registers
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

void RTCCRead(void)
{
	unsigned long time;
	unsigned long date;
	unsigned char dummy;

	time = RTCTIME;
	dummy = (unsigned char) (time & 0x000000FF);
	time = time >> 8;
	lmi.settings.clock.info.time.seconds = unconvert_BCD((unsigned char) (time & 0x000000FF));
	time = time >> 8;
	lmi.settings.clock.info.time.minutes = unconvert_BCD((unsigned char) (time & 0x000000FF));
	time = time >> 8;
	lmi.settings.clock.info.time.hours = unconvert_BCD((unsigned char) (time & 0x000000FF));

	date = RTCDATE;
	dummy = (unsigned char) (date & 0x000000FF);
	date = date >> 8;
	lmi.settings.calendar.info.date.day = unconvert_BCD((unsigned char) (date & 0x000000FF));
	date = date >> 8;
	lmi.settings.calendar.info.date.month = unconvert_BCD((unsigned char) (date & 0x000000FF));
	date = date >> 8;
	lmi.settings.calendar.info.date.year = unconvert_BCD((unsigned char) (date & 0x000000FF));

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	unsigned char convert_BCD(unsigned char binary_byte)
//*
//*	Description:
//*		Given a binary value, the function returns the BCD equivalent.
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

unsigned char convert_BCD(unsigned char binary_byte)
{
	unsigned char BCD_byte;
	unsigned char tens_digit;

	tens_digit = (binary_byte / 10) << 4;
	BCD_byte = tens_digit + (binary_byte % 10);
	return BCD_byte;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	unsigned char unconvert_BCD(unsigned char binary_byte)
//*
//*	Description:
//*		Given a BCD value, the function returns the binary equivalent.
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

unsigned char unconvert_BCD(unsigned char BCD_byte)
{
	unsigned char binary_byte;
	unsigned char tens_digit;

	tens_digit = (BCD_byte >> 4) * 10;
	binary_byte = (BCD_byte & 0x0F) + tens_digit;
	return binary_byte;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	END OF REAL TIME CLOCK ROUTINES
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//*	I/O ROUTINES
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Check_Audio_Status(void)
//*
//*	Description:
//* 	Checks the instrument status to determine if the unimorph should be on or off.
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

void Check_Audio_Status(void)
{
	if(PORTBbits.RB9 == 0)																			// if headphones are connected...
	{
		Audio_IO_OutLAT = (Audio_IO_OutLAT & MUTE_AUDIO);											// ...mute audio (9DP v1.01.13)
	}
	else
	{
		if((Present_Level != NORMAL) && (Alert_Clear_Flag == 0) && (Alarm_Clear_Flag == 0))			// if an alarm is in progress and the audio is not cleared yet...
		{
			Audio_IO_OutLAT = (Audio_IO_OutLAT | UNMUTE_AUDIO);										// ...unmute audio (9DP v1.01.13)
		}
		else if((Audio_Mute == TRUE) || (Audio_Enable == FALSE))									// else if headphones are connected, audio is muted, or audio is disabled...
		{
			Audio_IO_OutLAT = (Audio_IO_OutLAT & MUTE_AUDIO);										// ...mute audio (9DP v1.01.13)
		}
		else																						// else...
		{	
			Audio_IO_OutLAT = (Audio_IO_OutLAT | UNMUTE_AUDIO);										// ...unmute audio (9DP v1.01.13)
		}
	}
	i2c2Write(i2cADDR_MAIN_IO_AUDIO, 0x0A, 1, Audio_IO_OutLAT, 1);									// write to the OLAT Register...... I/O expander U9

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Check_Battery(void)
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
//*		None
//*

#define BATTERY_OFFSET	70																			// 44mV

void Check_Battery(void)
{
	WORD ADC_Battery_Level;

	ADC_Battery_Level = GetADCValue((BYTE) ADC_PORT3_BATTERY);										// get Battery ADC Value

	if(ADC_Battery_Level <= (2379 - BATTERY_OFFSET))												// 8.50 volts  --- determine if batteries are initially too low to operate instrument
	{																
		Drained_Battery = TRUE;																		// Battery too low for continued operation, prevent further startup - shutdown
		Shutdown_Power();																			// (9DP v1.01.23)
	}
    else
    {
        Drained_Battery = FALSE;
    }

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Check_High_Voltage(BYTE value)
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
//*		The high voltage reading uses a 500M/7.5M voltage divider. The resulting constant is:
//*
//*							7.5M / (500M + 7.5M) = 0.014778
//*

void Check_High_Voltage(BYTE value)
{
	WORD ADC_High_Voltage_Level = 0;

	if(value == 1)
	{
		ADC_High_Voltage_Level = GetADCValue((BYTE) ADC_PORT2_HV_REF);								// get high voltage (HV_REF)
	}
	else if(value == 2)
	{
		ADC_High_Voltage_Level = GetADCValue((BYTE) ADC_PORT6_EXTRA2);								// get high voltage (ADC_EXTRA2)
	}
	else
	{
		High_Voltage_Reading = 0.0;
		return;
	}
	
	switch(lmi.settings.product.info.model_number)
	{
		case MODEL_9DP:
		case MODEL_9DP_PLUS:
		case MODEL_9DP_1:
		case MODEL_9DP_1_PLUS:
		case MODEL_9DLP:
		case MODEL_9DLP_PLUS:
		case MODEL_V1:																				// added model number for RTI (9DP v1.01.21)
		case MODEL_9DP_STAR:																		// added model number 9DP* (9DP v1.01.24)
//		case MODEL_9DP_STAR_PLUS:																	// added model number 9DP*+ (9DP v1.01.24)
		case MODEL_9DP_2:																			// added model number 9DP-2 (9DP v1.01.24)
//		case MODEL_9DP_2_PLUS:																		// added model number 9DP-2+ (9DP v1.01.24)
		{
			High_Voltage_Reading = ((((float)ADC_High_Voltage_Level/4096.0) * 5.0) - 2.5) / 0.014778;
			High_Voltage_Reading = High_Voltage_Reading * ( lmi.calibration.constants.info.high_voltage / 1000.0);
			break;
		}
		case MODEL_3D:
		case MODEL_3D_PLUS:
		case MODEL_276D:
		{
			High_Voltage_Reading = (((float)ADC_High_Voltage_Level/4096.0) * 2.5) * 1000.0;
			High_Voltage_Reading = High_Voltage_Reading * ( lmi.calibration.constants.info.high_voltage / 1000.0);
			break;
		}
		default:
		{
			High_Voltage_Reading = 0.0;
			break;
		}
	}

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Check_Temperature(void)
//*
//*	Description:
//*		None.
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
//*		None.
//*

#define DISABLE_TEMP		0x08
#define ENABLE_TEMP			0xF7

void Check_Temperature(void)
{
	BOOL temp_sign;																					// if 1, temperature is < 0 C, if 0, temperature is >= 0 C
	WORD buffer;

	buffer = i2c2ReadWord(i2cADDR_X_TEMP_0x90, TEMP_REG, 1);										// read i2c temperature sensor temperature register

	if(buffer > 32767)																				// if temperature is negative...
	{																								// ...convert twos complement
		buffer = buffer ^ 0xFFFF;																	// ...invert
		buffer += 1;																				// ...add 1
		temp_sign = 1;																				// ...set negative sign flag
	}
	else																							// else if temperature is zero or positive...
	{		
		temp_sign = 0;																				// ...clear negative sign flag
	}

	Temperature_Reading = ((float) buffer / 16.0) * TEMP_CONST_12BIT;								// mask off necessary data

	if(temp_sign)																					// if temperature is negative...
		Temperature_Reading *= -1.0;																// ...convert to negative

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Check_Pressure(void) (9DP v1.02.03)
//*
//*	Description:
//*		None.
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
//*		None.
//*

#define OUTPUT_MAX			14746
#define OUTPUT_MIN			1638
#define OUTPUT_RANGE		(OUTPUT_MAX - OUTPUT_MIN)
#define PRESSURE_MAX		27.0
#define PRESSURE_MIN		3.0
#define PRESSURE_RANGE		(PRESSURE_MAX - PRESSURE_MIN)

#define PSI_NORMAL			0
#define PSI_COMMAND_MODE	1
#define PSI_STALE			2
#define PSI_DIAGNOSTIC		3

void Check_Pressure(void)
{
	UINT32_VAL output_result;
	UINT16 pressure_output_result;
	UINT8 status_result;

	output_result = i2c2ReadDWord(i2cADDR_X_PRES_0x50);
	pressure_output_result = output_result.w[1] & 0x3FFF;
	status_result = output_result.v[3] >> 6;

	if((status_result == PSI_NORMAL) || (status_result == PSI_STALE))
	{
		Chamber_Pressure_PSI = (((float)(pressure_output_result - OUTPUT_MIN) / OUTPUT_RANGE) * PRESSURE_RANGE) + PRESSURE_MIN;
		Chamber_Pressure_PSI -= 13.63;
	}

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	END I/O ROUTINES
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//*	ERROR ROUTINES
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Clear_All_Errors(void)
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
//* 	(9DP v1.01.08)
//*

void Clear_All_Errors(void)
{
	New_Alarm = CLEAR;
	Error_Recent = Error_Last;
	Alert_Clear_Flag = 0;
	Alarm_Clear_Flag = 0;
	Integrate_Alert_Flag = FALSE;
	Integrate_Alert_Ack_Flag = FALSE;
	Integrate_Alarm_Flag = FALSE;
	Integrate_Alarm_Ack_Flag = FALSE;
//	Radiation_Alert_Flag = FALSE;																	// (v1.01.11)
//	Radiation_Alert_Ack_Flag = FALSE;																// (v1.01.11)
//	Radiation_Alarm_Flag = FALSE;																	// (v1.01.11)
//	Radiation_Alarm_Ack_Flag = FALSE;																// (v1.01.11)
	ClearTopRightText();
	Flag_Status_Text_Displayed = FALSE;																// indicate text was cleared (9DP v1.01.11)
	Set_Audio_Tone();																				// re-initialize audio tone (9DP v1.01.11)

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void add_error_message_to_queue(unsigned char error)
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

void add_error_message_to_queue(unsigned char error)												// moved here from screens.c (9DP v1.01.04)
{
    if(error != Last_Saved_Error_Number)															// if the new error received is not the same as the last error received...
    {
        Error_Buffer[Error_Recent++] = error;														// ...add the error to the queue and increment the queue index
        Error_Recent %= NUMBER_OF_ERRORS;															// ...adjust queue index in the event of exceeding queue size
        if(Error_Recent == Error_Last)																// ...if the queue index is equal to the oldest error index...
        {
            Error_Last++;																			//    ...increment the oldest error index
            Error_Last %= NUMBER_OF_ERRORS;															//    ...adjust the oldest error index in the event of exceeding queue size
        }
        Last_Saved_Error_Number = error;															// ...save the most recent error
    }

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void clear_error_message_off_queue(void)
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
//* 	moved here from screens.c (9DP v1.01.04)
//*

void clear_error_message_off_queue(void)
{
    Error_Last++;
    Error_Last %= NUMBER_OF_ERRORS;
    Custom_Error_Pending = FALSE;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Check_Messages(void)
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

void Check_Messages(void)
{
	if(Error_Recent != Error_Last)																	// if an error is in the queue...
	{
	    if(!Custom_Error_Pending)																	// ...if a custom error is not pending acknowledgement...
	    {
			PreviousScreenState = screenState;														//    ...save the current screen state
			screenState = CREATE_LOCAL_ALERTS;														//    ...set the screen state to create local alerts
	    }
	}
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Display_Error_Message(void)
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

void Display_Error_Message(void)
{
	unsigned char ErrorMsg[51] = {'\0'};																											// declare error message char array
	XCHAR tempXmsg[51] = {'\0'};
	SHORT msgwidth;
	BYTE message_event;

	Custom_Error_Pending = FALSE;																													// clear error pending flag
	message_event = NORMAL;																															// clear event type

	switch(Error_Buffer[Error_Last])
	{
		case NORMAL_MESSAGE:
		{
		    clear_error_message_off_queue();
			Set_Audio_Tone();																														// update audio tone settings (9DP v1.01.11)
			return;
		}
		case ALERT_RADIATION:
		{
			if(lmi.display.detector[Current_Detector].view[Current_View].alerts_radiation.info.feature.enable)										// check if radiation alert enabled (9DP v1.01.04)
	        {
				Present_Level = ALERT;																												// enable alert audio
				if(lmi.display.detector[Current_Detector].view[Current_View].alerts_radiation.info.feature.custom)									// check if custom message enabled (9DP v1.01.04)
            	{
            		Custom_Error_Pending = TRUE;																									// waiting on user acknowledgement
					strncpy(ErrorMsg, lmi.display.detector[Current_Detector].view[Current_View].alerts_radiation.info.feature.custom_message, 50);	// (9DP v1.01.04)
					ErrorMsg[50] = '\0';																											// (9DP v1.01.04)
					Use_Custom_Message(ErrorMsg, MSG_H_E_F_HEADING, ALERT_RADIATION, CENTER_JUSTIFY);												// arrange custom message
				}
				else 																																// custom is disabled -- use default Message (no screen box used)
				{
					message_event = ALERT;
				}
			}
			else																																	// if alerts are disabled
			{		
				Present_Level = NORMAL;
			}
			break;
		}
		case ALERT_INTEGRATE:
		{
			if(lmi.display.detector[Current_Detector].view[Current_View].alerts_integrate.info.feature.enable)										// check if integrate alert enabled (9DP v1.01.04)
			{
				Present_Level = ALERT;																												// enable alert audio
				if(lmi.display.detector[Current_Detector].view[Current_View].alerts_integrate.info.feature.custom)									// check if custom message enabled (9DP v1.01.04)
				{
					Custom_Error_Pending = TRUE;																									// waiting on user acknowledgement
					strncpy(ErrorMsg, lmi.display.detector[Current_Detector].view[Current_View].alerts_integrate.info.feature.custom_message, 50);	// (9DP v1.01.04)
					ErrorMsg[50] = '\0';																											// (9DP v1.01.04)
					Use_Custom_Message(ErrorMsg, MSG_H_E_F_HEADING, ALERT_INTEGRATE, CENTER_JUSTIFY);												// arrange custom message
				}
				else 																																// custom is disabled -- use default message (no screen box used)
				{
					message_event = ALERT;
				}
			}
			else																																	// if alerts are disabled
			{	
				Present_Level = NORMAL;
			}
			break;
		}
		case ALERT_FUNCTION:
		{
			if(lmi.display.detector[Current_Detector].view[Current_View].alerts_function.info.feature.enable)										// check if function alert enabled (9DP v1.01.04)
			{
				Present_Level = ALERT;																												// enable alert audio
				if(lmi.display.detector[Current_Detector].view[Current_View].alerts_function.info.feature.custom)									// check if custom message enabled (9DP v1.01.04)
				{
					Custom_Error_Pending = TRUE;																									// waiting on user acknowledgement
					strncpy(ErrorMsg, lmi.display.detector[Current_Detector].view[Current_View].alerts_function.info.feature.custom_message, 50);	// (9DP v1.01.04)
					ErrorMsg[50] = '\0';																											// (9DP v1.01.04)
					Use_Custom_Message(ErrorMsg, MSG_H_E_F_HEADING, ALERT_INTEGRATE, CENTER_JUSTIFY);												// arrange custom message
				}
				else 																																// custom is disabled -- use default message (no screen box used)
				{
					message_event = ALERT;
				}
			}
			else																																	// if alerts are disabled
			{	
				Present_Level = NORMAL;
			}
			break;
		}
		case ALERT_LOW_BATTERY:
		{
			if(lmi.alerts.low_battery.info.feature.enable)
			{
				Present_Level = ALERT;																												// enable alert audio
				if(lmi.alerts.low_battery.info.feature.custom)
				{
					Custom_Error_Pending = TRUE;																									// waiting on user acknowledgement
					strncpy(ErrorMsg, lmi.alerts.low_battery.info.feature.custom_message, 50);
					ErrorMsg[50] = '\0';																											// (9DP v1.01.04)
					Use_Custom_Message(ErrorMsg, MSG_H_E_F_HEADING, ALERT_INTEGRATE, CENTER_JUSTIFY);												// arrange custom message
				}
				else
				{
					message_event = ALERT;
				}	
			}
			else
			{
				Present_Level = NORMAL;
			}	
			break;
		}
		case ALERT_LOW_MEMORY:
		{
			if(lmi.alerts.low_memory.info.feature.enable)
			{
				Present_Level = ALERT;																												// enable alert audio
				if(lmi.alerts.low_memory.info.feature.custom)
				{
					Custom_Error_Pending = TRUE;																									// waiting on user acknowledgement
					strncpy(ErrorMsg, lmi.alerts.low_memory.info.feature.custom_message, 50);
					ErrorMsg[50] = '\0';																											// (9DP v1.01.04)
					Use_Custom_Message(ErrorMsg, MSG_H_E_F_HEADING, ALERT_INTEGRATE, CENTER_JUSTIFY);												// arrange custom message
				}
				else
				{
					message_event = ALERT;
				}	
			}
			else
			{
				Present_Level = NORMAL;
			}	
			break;
		}
		case ALERT_CALIBRATION_DUE_DATE:
		{
			if(lmi.alerts.calibration_due.info.feature.enable)
			{
				Present_Level = ALERT;																												// enable alert audio
				if(lmi.alerts.calibration_due.info.feature.custom)
				{
					Custom_Error_Pending = TRUE;																									// waiting on user acknowledgement
					strncpy(ErrorMsg, lmi.alerts.calibration_due.info.feature.custom_message, 50);
					ErrorMsg[50] = '\0';																											// (9DP v1.01.04)
					Use_Custom_Message(ErrorMsg, MSG_H_E_F_HEADING, ALERT_INTEGRATE, CENTER_JUSTIFY);												// arrange custom message
				}
				else
				{
					message_event = ALERT;
				}	
			}
			else
			{
				Present_Level = NORMAL;
			}	
			break;
		}
		case ALERT_SOURCE_CHECK:
		{
			if(lmi.alerts.source_check.info.feature.enable)
			{
				Present_Level = ALERT;																												// enable alert audio
				if(lmi.alerts.source_check.info.feature.custom)
				{
					Custom_Error_Pending = TRUE;																									// waiting on user acknowledgement
					strncpy(ErrorMsg, lmi.alerts.source_check.info.feature.custom_message, 50);
					ErrorMsg[50] = '\0';																											// (9DP v1.01.04)
					Use_Custom_Message(ErrorMsg, MSG_H_E_F_HEADING, ALERT_INTEGRATE, CENTER_JUSTIFY);												// arrange custom message
				}
				else
				{
					message_event = ALERT;
				}	
			}
			else
			{
				Present_Level = NORMAL;
			}	
			break;
		}
		case ALERT_BACKGROUND_CHECK:
		{
			if(lmi.alerts.background_check.info.feature.enable)
			{
				Present_Level = ALERT;																												// enable alert audio
				if(lmi.alerts.background_check.info.feature.custom)
				{
					Custom_Error_Pending = TRUE;																									// waiting on user acknowledgement
					strncpy(ErrorMsg, lmi.alerts.background_check.info.feature.custom_message, 50);
					ErrorMsg[50] = '\0';																											// (9DP v1.01.04)
					Use_Custom_Message(ErrorMsg, MSG_H_E_F_HEADING, ALERT_INTEGRATE, CENTER_JUSTIFY);												// arrange custom message
				}
				else
				{
					message_event = ALERT;
				}	
			}
			else
			{
				Present_Level = NORMAL;
			}	
			break;
		}
		case ALERT_SCALER:
		{
			Present_Level = ALERT;																													// enable alert audio
			message_event = ALERT;
			break;
		}
		case ALARM_RADIATION:
		{
			if(lmi.display.detector[Current_Detector].view[Current_View].alarms_radiation.info.feature.enable)										// check if radiation alarm enabled (9DP v1.01.04)
			{
				Present_Level = ALARM;																												// enable alarm audio
				if(lmi.display.detector[Current_Detector].view[Current_View].alarms_radiation.info.feature.custom)									// check if custom message enabled (9DP v1.01.04)
				{
					Custom_Error_Pending = TRUE;																									// waiting on user acknowledgement
					strncpy(ErrorMsg, lmi.display.detector[Current_Detector].view[Current_View].alarms_radiation.info.feature.custom_message, 50);	// (9DP v1.01.04)
					ErrorMsg[50] = '\0';																											// (9DP v1.01.04)
					Use_Custom_Message(ErrorMsg, MSG_H_E_F_HEADING, ALARM_RADIATION, CENTER_JUSTIFY);												// arrange custom message
				}
				else 																																// custom is disabled -- use default message (no screen box used)
				{
					message_event = ALARM;
				}
			}
			else																																	// if alerts are disabled
			{		
				Present_Level = NORMAL;
			}
			break;
		}
		case ALARM_INTEGRATE:
		{
			if(lmi.display.detector[Current_Detector].view[Current_View].alarms_integrate.info.feature.enable)										// check if integrate alarm enabled (9DP v1.01.04)
			{
				Present_Level = ALARM;																												// enable alarm audio
				if(lmi.display.detector[Current_Detector].view[Current_View].alarms_integrate.info.feature.custom)									// check if custom message enabled (9DP v1.01.04)
				{
					Custom_Error_Pending = TRUE;																									// waiting on user acknowledgement
					strncpy(ErrorMsg, lmi.display.detector[Current_Detector].view[Current_View].alarms_integrate.info.feature.custom_message, 50);	// (9DP v1.01.04)
					ErrorMsg[50] = '\0';																											// (9DP v1.01.04)
					Use_Custom_Message(ErrorMsg, MSG_H_E_HEADING, ALARM_INTEGRATE, CENTER_JUSTIFY);													// arrange custom message
				}
				else 																																// custom is disabled -- use default message (no screen box used)
				{
					message_event = ALARM;
				}
			}
			else																																	// if alerts are disabled
			{		
				Present_Level = NORMAL;
			}
			break;
		}
		case ALARM_FUNCTION:
		{
			if(lmi.display.detector[Current_Detector].view[Current_View].alarms_function.info.feature.enable)										// (9DP v1.01.04)
			{
				Present_Level = ALARM;																												// enable alarm audio
				if(lmi.display.detector[Current_Detector].view[Current_View].alarms_function.info.feature.custom)									// (9DP v1.01.04)
				{
					Custom_Error_Pending = TRUE;																									// waiting on user acknowledgement
					strncpy(ErrorMsg, lmi.display.detector[Current_Detector].view[Current_View].alarms_function.info.feature.custom_message, 50);	// (9DP v1.01.04)
					ErrorMsg[50] = '\0';																											// (9DP v1.01.04)
					Use_Custom_Message(ErrorMsg, MSG_H_E_HEADING, ALARM_FUNCTION, CENTER_JUSTIFY);													// arrange custom message
				}
				else 																																// custom is disabled -- use default message (no screen box used)
				{
					message_event = ALARM;
				}
			}
			else																																	// if alerts are disabled
			{	
				Present_Level = NORMAL;
			}
			break;
		}		
		case ALARM_LOW_BATTERY:
		{
			break;
		}
		case WARNING_SHUTDOWN:
		{
			Custom_Error_Pending = TRUE;	
			sprintf(ErrorMsg, "Low Battery - Shutting down\0");																						// format string and save to array
			Use_Custom_Message(ErrorMsg, MSG_H_E_F_HEADING, WARNING_SHUTDOWN, CENTER_JUSTIFY);														// arrange custom message
			break;
		}
		case ERROR_FILE_SYSTEM:
		case ERROR_FILE_OPEN:
		{
			system_error_tick = tick + ((DWORD) TICKS_PER_SEC * 5);																					// calculate error message display time (9DP v1.01.05)
			System_Error_Flag = TRUE;																												// set system error flag
			File_Error_Message(Error_Buffer[Error_Last]);																							// call USB file error processing function
			break;
		}
		default:
		{
			if(Error_Buffer[Error_Last] >= USB_ERROR_MSG)																							// determine if the error is USB related
			{
				system_error_tick = tick + ((DWORD) TICKS_PER_SEC * 5);																				// calculate error message display time (9DP v1.01.05)
				System_Error_Flag = TRUE;																											// set system error flag
				USB_Error_Message(Error_Buffer[Error_Last]);																						// call USB error processing function
			}
			break;
		}	
	}

	if(Custom_Error_Pending == TRUE)
	{
		Flag_Custom_Msg_Enabled = TRUE;																												// set flag to indicate custom message placed
		Flag_Status_Text_Displayed = FALSE;																											// clear flag to indicate text
	}
	else
	{
		Flag_Custom_Msg_Enabled = FALSE;																											// clear flag to indicate custom message not placed
		if(PreviousScreenState == MONITOR_RADIATION)																								// (9DP v1.01.04)
		{
			if(Present_Level != NORMAL)																												// (9DP v1.01.04)
			{
				DisplayTopRightText(message_event);																									// (9DP v1.01.04)
				Flag_Status_Text_Displayed = TRUE;																									// set flag to indicate status text placed
			}
		}	
		clear_error_message_off_queue();						
	}

	Set_Audio_Tone();																																// update audio tone settings

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	END OF ERROR ROUTINES
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//*	FEATURE ROUTINES
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Calculate_Integrate_Value(void)
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

void Calculate_Integrate_Value(void)
{
	static BYTE last_integrate_mode = 0;																											// (9DP v1.01.19)
	short multiplier;
	float relative_conversion;
	float dose_value;
	float rate;

	dose_value = UnitConversions(Rate_Accumulation, Default_Dose_Units, lmi.features.integrate.info.current_reading.units);
	multiplier = RelativeMultiplier((short) DEFAULT_DOSE_MULTIPLIER, lmi.features.integrate.info.current_reading.multiplier);						// (9DP v1.01.12)
	relative_conversion = MultiplierConversion(multiplier);
	lmi.features.integrate.info.current_reading.value = dose_value * relative_conversion;

	if(lmi.features.integrate.info.operating_mode != last_integrate_mode)																			// (9DP v1.01.19)
		integrate_update_tick = tick + (DWORD) TICKS_PER_SEC;																						// (9DP v1.01.19)
	last_integrate_mode = lmi.features.integrate.info.operating_mode;																				// (9DP v1.01.19)

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	END FEATURE ROUTINES
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//*	CONTROL ROUTINES
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Set_Audio_Tone(void)
//*
//*	Description:
//* 	This routine sets the tone that will be supplied by the audio processor. In addition to setting
//*		the audio tone, it will also make the necessary changes depending on audio mode and division.
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

void Set_Audio_Tone(void)
{
	if((Present_Level == ALARM) && (Alarm_Clear_Flag == 0))
	{
		Audio_IO_OutLAT = (Audio_IO_OutLAT | ENABLE_ALARM);											// enable alarm tone (9DP v1.01.13)
	}
	else if((Present_Level == ALERT) && (Alert_Clear_Flag == 0))									// checking for Present_level != Normal Makes NON-Latching
	{
		Audio_IO_OutLAT = (Audio_IO_OutLAT | ENABLE_ALARM);											// enable alarm tone (9DP v1.01.13)
	}
	else
	{
		Audio_IO_OutLAT = (Audio_IO_OutLAT & DISABLE_ALARM);										// clear alarm tone bit (9DP v1.01.13)
		Present_Level = NORMAL;
	}
	
	switch(Audio_Mode)
	{
		case 0:																						// PULSE 1 is input
			Audio_IO_OutLAT = (Audio_IO_OutLAT & AUDIO_MODE_CLEAR);									// clear mode bits (9DP v1.01.13)
			Audio_IO_OutLAT = (Audio_IO_OutLAT | AUDIO_MODE_0);										// mode 0 (9DP v1.01.13)
			break;

		case 1:																						// PULSE 1 and PULSE 2 are inputs
			Audio_IO_OutLAT = (Audio_IO_OutLAT & AUDIO_MODE_CLEAR);									// clear mode bits (9DP v1.01.13)
			Audio_IO_OutLAT = (Audio_IO_OutLAT | AUDIO_MODE_1);										// mode 1 (9DP v1.01.13)
			break;

		case 2:																						// Audio from PIC32 is input
			Audio_IO_OutLAT = (Audio_IO_OutLAT & AUDIO_MODE_CLEAR);									// clear mode bits (9DP v1.01.13)
			Audio_IO_OutLAT = (Audio_IO_OutLAT | AUDIO_MODE_2);										// mode 2 (9DP v1.01.13)
			break;

		case 3:																						// simulate input PIC18 Test mode
			Audio_IO_OutLAT = (Audio_IO_OutLAT & AUDIO_MODE_CLEAR);									// clear mode bits (9DP v1.01.13)
			Audio_IO_OutLAT = (Audio_IO_OutLAT | AUDIO_MODE_3);										// mode 3 (9DP v1.01.13)
			break;

	}

	switch(Audio_Div)
	{
		case 0:																						// Divide by 10
			Audio_IO_OutLAT = (Audio_IO_OutLAT & AUDIO_DIV_CLEAR);									// clear audio divide (9DP v1.01.13)
			Audio_IO_OutLAT = (Audio_IO_OutLAT | AUDIO_DIV_1);										// divide by 1 (9DP v1.01.13)
			break;
				
		case 1:																						// Divide by 100
			Audio_IO_OutLAT = (Audio_IO_OutLAT & AUDIO_DIV_CLEAR);									// clear audio divide (9DP v1.01.13)
			Audio_IO_OutLAT = (Audio_IO_OutLAT | AUDIO_DIV_10);										// divide by 10 (9DP v1.01.13)
			break;

		case 2:																						// Divide by 1k
			Audio_IO_OutLAT = (Audio_IO_OutLAT & AUDIO_DIV_CLEAR);									// clear audio divide (9DP v1.01.13)
			Audio_IO_OutLAT = (Audio_IO_OutLAT | AUDIO_DIV_100);									// divide by 100 (9DP v1.01.13)
			break;

		case 3:																						// Divide by 10k
			Audio_IO_OutLAT = (Audio_IO_OutLAT & AUDIO_DIV_CLEAR);									// clear audio divide (9DP v1.01.13)
			Audio_IO_OutLAT = (Audio_IO_OutLAT | AUDIO_DIV_1K);										// divide by 1K (9DP v1.01.13)
			break;

	}

	i2c2Write(i2cADDR_MAIN_IO_AUDIO, 0x0A, 1, Audio_IO_OutLAT, 1);									// write to the OLAT Register...... I/O expander U9
	Set_Audio_Volume();

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Set_Audio_Volume(void)
//*
//*	Description:
//* 	This routine sets the audio volume depending on current audio status. It will set the alert
//*		and alarm audio volume as well. If the regular audio volume is 0, the routine will set the
//*		flag to mute the unimorph so no minor audio might be heard.
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

void Set_Audio_Volume(void)
{
	if((Present_Level == ALARM) && (Alarm_Clear_Flag == 0))
	{
		setVolume_i2c(i2cADDR_MAIN_DAC, VOLUME_REGISTER, lmi.controls.audio.info.alarm_level);
	}
	else if((Present_Level == ALERT) && (Alert_Clear_Flag == 0))													// checking for Present_level != Normal Makes NON-Latching
	{
		setVolume_i2c(i2cADDR_MAIN_DAC, VOLUME_REGISTER, lmi.controls.audio.info.alert_level);
	}
	else
	{
		Audio_Operating_Mode = lmi.controls.audio.info.operating_mode;
		lmi.controls.audio.info.current_level = lmi.controls.audio.info.operating_mode * 20;						// set the new audio level
		switch(Audio_Operating_Mode)
		{
			case AUDIO_0:
			{
				setVolume_i2c(i2cADDR_MAIN_DAC, VOLUME_REGISTER, lmi.controls.audio.info.current_level);
				Audio_Mute = 1;
				break;
			}
			case AUDIO_20:																							// SPKR 20%
			case AUDIO_40:																							// SPKR 40%
			case AUDIO_60:																							// SPKR 60%
			case AUDIO_80:																							// SPKR 80%
			case AUDIO_100:																							// SPKR 100%
			{
				setVolume_i2c(i2cADDR_MAIN_DAC, VOLUME_REGISTER, lmi.controls.audio.info.current_level);
				Audio_Mute = 0;
				break;
			}
		}
	}
	
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Set_Backlight_Level(char index)
//*
//*	Description:
//* 	
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	index -  received value in a percentage
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	Max backlight level is 255
//*

void Set_Backlight_Level(char index)
{
	WORD BackLight = 0;
	WORD level;
	
	lmi.controls.backlight.info.current_level = index;
	level = 255 * index / 100;
	BackLight = (level << 8);

	SetReg(0xB2, BackLight);																		// Reg[B2h] = 00h	-	Reg[B3h] = 80h  <---- 50% and FFh is 100%   ( 0x8000 )
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	END CONTROL ROUTINES
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//	CALIBRATION ROUTINES
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CheckCalibrationDueDate(void)
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

void CheckCalibrationDueDate(void)
{
	struct tm currentTime;
	struct tm calibrationTime;
	UINT16 leapyear;
	INT16 relativeDays = 0;

	currentTime.tm_year = lmi.settings.calendar.info.date.year + 100;								// year relative to 1900
	currentTime.tm_mon = lmi.settings.calendar.info.date.month - 1;
	currentTime.tm_mday = lmi.settings.calendar.info.date.day;
	currentTime.tm_hour = lmi.settings.clock.info.time.hours;
	currentTime.tm_min = lmi.settings.clock.info.time.minutes;
	currentTime.tm_sec = lmi.settings.clock.info.time.seconds;
	currentTime.tm_isdst = 0;
	mktime(&currentTime);

	calibrationTime.tm_year = lmi.settings.product.info.calibration_due_date.year + 100;			// year relative to 1900
	calibrationTime.tm_mon = lmi.settings.product.info.calibration_due_date.month - 1;
	calibrationTime.tm_mday = lmi.settings.product.info.calibration_due_date.day;
	calibrationTime.tm_hour = lmi.settings.clock.info.time.hours;
	calibrationTime.tm_min = lmi.settings.clock.info.time.minutes;
	calibrationTime.tm_sec = lmi.settings.clock.info.time.seconds;
	calibrationTime.tm_isdst = 0;
	mktime(&calibrationTime);

	if(calibrationTime.tm_year > currentTime.tm_year)
	{
		leapyear = currentTime.tm_year % 4;															// I'll be dead in 2100 so who cares
		if(leapyear == 0)
		{
			relativeDays = 365 + calibrationTime.tm_yday - currentTime.tm_yday;						// days(year) are relative to 0
		}
		else
		{
			relativeDays = 364 + calibrationTime.tm_yday - currentTime.tm_yday;						// days(year) are relative to 0
		}
	}
	else
	{
		relativeDays = calibrationTime.tm_yday - currentTime.tm_yday;
	}

	if((lmi.alarms.calibration_due.info.feature.enable == TRUE) && (lmi.alarms.calibration_due.info.feature.value >= relativeDays))
		add_error_message_to_queue(ALARM_CALIBRATION_DUE_DATE);
	else if((lmi.alerts.calibration_due.info.feature.enable == TRUE) && (lmi.alerts.calibration_due.info.feature.value >= relativeDays))
		add_error_message_to_queue(ALERT_CALIBRATION_DUE_DATE);

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
// END OF CALIBRATION ROUTINES
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//*	DISPLAY ROUTINES
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	 WORD GOLDrawCallback(void)
//*
//*	Description:
//* 	This function must be implemented by user. GOLDraw() function calls it each time when GOL
//*		objects drawing is completed. User drawing should be done here. GOL will not change color,
//*		line type and clipping region settings while this function returns zero.
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	None
//*
//*	Return Values:
//* 	If the function returns non-zero the draw control will be passed to GOL
//*
//*	Remarks:
//* 	None
//*

WORD GOLDrawCallback(void)
{
	static BYTE future_screenState = 0x00;
	static signed short last_SubMenuLine = -1;
	static BOOL last_editMode = FALSE;
	char stringconversion[29];
	char message_string[29];
	char index;

	INT8 	*INT8_value;
	INT16 	*INT16_value;
	INT32 	*INT32_value;
	UINT8 	*UINT8_value;
	UINT16 	*UINT16_value;
	UINT32 	*UINT32_value;
	float 	*float_value;
	unsigned int int_status;

#ifdef TEST_MODE																										// (Test v0.00.01)
	static UINT8 old_routine_status = 0;																				// (Test v0.00.01)
	static WORD test_color = 0x0000;																					// (Test v0.00.01)
	float temp_float = 0.0;																								// (Test v0.00.01)
	WORD temp_color = 0x0000;																							// (Test v0.00.01)
	WORD inverted_color = 0x0000;																						// (Test v0.00.01)
	SHORT msgwidth;																										// (Test v0.00.01)
	XCHAR xchar_buffer[25] = {0};																						// (Test v0.00.01)
	BYTE char_buffer[25] = {0};																							// (Test v0.00.01)
	UINT8 new_routine_status;																							// (Test v0.00.01)
#endif																													// (Test v0.00.01)

    switch(screenState)
	{
		case CREATE_SPLASH:																								// only used to prolong battery check
		{
			//Check_Battery();																							// check battery condition
			screenState = DISPLAY_SPLASH;
			return 1;
		}
		case DISPLAY_SPLASH:
		{
			//Check_Battery();																							// check battery condition
			if(ExtImage(0, 0, SPLASH_SCREEN))																			// display splash screen
			{
				Wait_State_Event = FALSE;																				// clear timer state event
				wait_state_tick = tick + ((DWORD) TICKS_PER_SEC * 3);													// delay 3 seconds (9DP v1.01.05)
				future_screenState = CREATE_STARTUP;
				screenState = WAIT_STATE;
			}
			else
			{
				screenState = CREATE_LUDLUM;
			}
			return 1;
		}
		case CREATE_LUDLUM:																								// only used to prolong battery check
		{
			//Check_Battery();																							// check battery condition
			Set_Backlight_Level(DEFAULT_BACKLIGHT_AUTO_HIGH);															// set backlight high
			SetColor(WHITE);																							// set screen background color
			ClearDevice();																								// Clear screen
			screenState = DISPLAY_LUDLUM;
			return 1;
		}
		case DISPLAY_LUDLUM:
		{
			//Check_Battery();																							// check battery condition
			if(lmi.settings.product.info.model_number != MODEL_V1)														// (9DP v1.01.21)
			{																											// (9DP v1.01.21)
				LudlumScreen();																							// Display the Ludlum Company info and check the battery charged status
				Wait_State_Event = FALSE;																				// clear timer state event
				wait_state_tick = tick + ((DWORD) TICKS_PER_SEC * 3);													// delay 3 seconds (9DP v1.01.05)
				future_screenState = CREATE_STARTUP;
				screenState = WAIT_STATE;
			}																											// (9DP v1.01.21)
			else																										// (9DP v1.01.21)
			{																											// (9DP v1.01.21)
				screenState = CREATE_STARTUP;																			// (9DP v1.01.21)
			}																											// (9DP v1.01.21)
			return 1;
		}
		case CREATE_STARTUP:																							// only used to prolong battery check
		{
			//Check_Battery();																							// check battery condition
			ScrambleClearScreen();																						// scramble/clear the previous screen info
			screenState = DISPLAY_STARTUP;
			return 1;
		}
		case DISPLAY_STARTUP:
		{
			//Check_Battery();																							// check battery condition
			StartupScreen();																							// Display the startup info and check the battery charged status
			Wait_State_Event = FALSE;																					// clear timer state event
			wait_state_tick = tick + ((DWORD) TICKS_PER_SEC * 3);														// delay 3 seconds (9DP v1.01.05)
			future_screenState = CREATE_ELEMENT;
			screenState = WAIT_STATE;
			return 1;
		}
		case CREATE_ELEMENT:
		{
			//Check_Battery();
			SetColor(WHITE);
			ClearDevice();																								// Clear screen
			GOLSetList(pArcScreen);
			Create_Measurement_Screen();
			Clear_All_Errors();																							// clears all errors off queue (9DP v1.01.08)
			screenState = MONITOR_RADIATION;																			// switch to monitoring instrument meter
			Update_Element(RESET_WINDOW);
			Update_Function(RESET_WINDOW);																				// (9DP v1.01.02)
			Update_Icon_Window(RESET_WINDOW);																			// (9DP v1.01.02)
			return 1;
		}
		case MONITOR_RADIATION:
		{
			Update_Element(UPDATE_WINDOW);
			Update_Function(UPDATE_WINDOW);
			Update_Icon_Window(UPDATE_WINDOW);
			Check_Buttons();																							// Check the status of the buttons
			Check_Shutdown();
			return 1;
		}
		case CREATE_LOCAL_ALERTS:
		{
			Display_Error_Message();
			if(Custom_Error_Pending)
			{
				message_event_tick = tick + ((DWORD) TICKS_PER_SEC * 3);												// set message event - calculate 3 second audio timer (9DP v1.01.05)
				Message_Event_Flag = FALSE;																				// clear message event flag
				screenState = DISPLAY_LOCAL_ALERTS;																		// switch to monitoring controls
			}
			else
			{
				if((Backlight_Mode == BK_LIGHT_TEMP_ON) && (Flag_Status_Text_Displayed == TRUE))
				{
					Light_On = TRUE;																					// initialise light flag
				}
				screenState = PreviousScreenState;																		// set state back to previous screen state
			}
			return 1;
		}
		case DISPLAY_LOCAL_ALERTS:
		{
			if((Message_Event_Flag == TRUE) && (Alert_Clear_Flag == 0))													// if audio timeout occurred
			{	
				Period = 2000000;																						// quieten audio level
				Alert_Clear_Flag = 1;																					// clear audio
				Set_Audio_Tone();																						// change audio
			}
			Check_Buttons();																							// Check the status of the buttons
			return 1;
		}
		case WAIT_STATE:
		{
			//Check_Battery();																							// check battery condition
			if(Wait_State_Event)
			{
				screenState = future_screenState;
				Wait_State_Event = FALSE;
			}
			return 1;
		}
		case NOT_DISPLAYED:																								// (9DP v1.01.12)
		{
			Check_Buttons();																							// Check the status of the buttons (Test v1.00.00)
			return 1;
		}

//**************************************************************************************************
// START KEYBOARD MENU
//**************************************************************************************************

		case CREATE_MENU:
		case CREATE_SETTINGS:
		case CREATE_PRODUCT:
		case CREATE_LANGUAGE:
		case CREATE_CALENDAR:
		case CREATE_CLOCK:
		case CREATE_PASSWORD:
		case CREATE_SYSTEM:
		case CREATE_FEATURES:
		case CREATE_CONTROLS:
		case CREATE_BACKLIGHT:
		case CREATE_AUTO_BACKLIGHT:
		case CREATE_AUDIO:
		case CREATE_GPS:
		case CREATE_BLUETOOTH:
		case CREATE_SOURCE_CHECK:
		case CREATE_BACKGROUND_CHECK:
		case CREATE_INTEGRATE:
		case CREATE_INTEGRATE_REPORT:																					// (9DP v1.01.02)
		case CREATE_INTEGRATE_VALUE:																					// (9DP v1.01.02)
		case CREATE_DATA_LOGGING:
		case CREATE_DATA_LOGGING_REPORT:
		case CREATE_DATA_LOGGING_REPORT_SYSTEM:
		case CREATE_DATA_LOGGING_REPORT_INSTRUMENT:
		case CREATE_DATA_LOGGING_REPORT_READINGS:
		case CREATE_DETECTOR:																							// (9DP v1.01.01)
		case CREATE_DETECTOR_SETUP:																						// (9DP v1.01.01)
		case CREATE_DETECTOR_0:																							// (9DP v1.01.01)
		case CREATE_DETECTOR_1:																							// (9DP v1.01.01)
		case CREATE_DETECTOR_2:																							// (9DP v1.01.01)
		case CREATE_DETECTOR_3:																							// (9DP v1.01.01)
		case CREATE_DETECTOR_4:																							// (9DP v1.01.01)
		case CREATE_DISPLAY:
		case CREATE_DISPLAY_SETUP:
		case CREATE_VIEW_1:
		case CREATE_VIEW_2:
		case CREATE_VIEW_3:
		case CREATE_VIEW_4:
		case CREATE_VIEW_APPEARANCE:																					// (9DP v1.01.01)
		case CREATE_VIEW_FUNCTIONS:																						// (9DP v1.01.01)
		case CREATE_VIEW_FUNCTION_1:																					// (9DP v1.01.01)
		case CREATE_VIEW_FUNCTION_2:																					// (9DP v1.01.01)
		case CREATE_VIEW_FUNCTION_3:																					// (9DP v1.01.01)
		case CREATE_VIEW_FUNCTION_4:																					// (9DP v1.01.01)
		case CREATE_VIEW_ALERTS:
		case CREATE_VIEW_ALERTS_RAD:
		case CREATE_VIEW_ALERTS_INT:
		case CREATE_VIEW_ALERTS_FUN:
		case CREATE_VIEW_ALARMS:
		case CREATE_VIEW_ALARMS_RAD:
		case CREATE_VIEW_ALARMS_INT:
		case CREATE_VIEW_ALARMS_FUN:
		case CREATE_ALERTS_ALARMS:
		case CREATE_ALERTS:
		case CREATE_ALARMS:
		case CREATE_FUNCTIONS:																							// (9DP v1.01.01)
		case CREATE_CALIBRATION:
		case CREATE_BLANK_MENU:
		case CREATE_LINE:
		case CREATE_EDIT_LINE:
		case CREATE_EDIT_STRING_LINE:
		case CREATE_PASSWORD_ENTRY:
		case CREATE_PASSWORD_RESPONSE:
		{
			editMode 		= FALSE;
			last_editMode 	= FALSE;
			CreateMenu();
			screenState = nextscreenState;
			return 1;
		}
#ifdef TEST_MODE
		case CREATE_9D_TEST_MENU:																						// (Test v0.00.05)
		case CREATE_3D_TEST_MENU:																						// (Test v0.00.05)
		case CREATE_276D_TEST_MENU:																						// (Test v0.00.05)
		{																												// (Test v0.00.05)
			HVBInit();																									// (Test v0.00.05)
		}																												// (Test v0.00.05)
		case CREATE_INSTRUMENT_MENU:																					// (Test v0.00.05)
		case CREATE_TEST_MENU:																							// (Test v0.00.01)
		case CREATE_TEST_MAIN_BOARD:																					// (Test v0.00.01)
		case CREATE_TEST_KERNEL:																						// (Test v0.00.01)
		case CREATE_TEST_RTCC:																							// (Test v0.00.01)
		case CREATE_TEST_EXT_MEM:																						// (Test v0.00.01)
		case CREATE_TEST_EXT_MEM_RW:																					// (Test v0.00.01)
		case CREATE_TEST_EXT_MEM_ADDR:																					// (Test v0.00.01)
		case CREATE_TEST_EXT_MEM_DATA:																					// (Test v0.00.01)
		case CREATE_TEST_EEPROM:																						// (Test v0.00.01)
		case CREATE_TEST_EEPROM_PRIMARY_RW:																				// (Test v0.00.01)
		case CREATE_TEST_EEPROM_PRIMARY_ERASE:																			// (Test v0.00.01)
		case CREATE_TEST_EEPROM_SECONDARY_RW:																			// (Test v0.00.01)
		case CREATE_TEST_EEPROM_SECONDARY_ERASE:																		// (Test v0.00.01)
		case CREATE_TEST_AUDIO:																							// (Test v0.00.01)
		case CREATE_TEST_BATTERY_VOLTAGE:																				// (Test v0.00.01)
		case CREATE_TEST_DISPLAY_VOLTAGE:																				// (Test v0.00.04)
		case CREATE_TEST_LCD_BOARD:																						// (Test v0.00.01)
		case CREATE_TEST_LIGHT_SENSOR:																					// (Test v0.00.01)
		case CREATE_TEST_HV_BOARD:																						// (Test v0.00.01)
		case CREATE_TEST_HIGH_VOLTAGE_1:																				// (Test v0.00.01)
		case CREATE_TEST_HIGH_VOLTAGE_2:																				// (Test v0.00.01)
		case CREATE_TEST_INPUT_VOLTAGE_1:																				// (Test v0.00.01)
		case CREATE_TEST_INPUT_VOLTAGE_2:																				// (Test v0.00.01)
		case CREATE_TEST_ELECTROMETER_BOARD:																			// (Test v0.00.01)
		case CREATE_TEST_ELECTROMETER_TEMPERATURE:																		// (Test v0.00.01)
		case CREATE_TEST_KEYPAD:																						// (Test v0.00.01)
		{																												// (Test v0.00.01)
			test_tick = tick + (DWORD) TICKS_PER_SEC;																	// (Test v0.00.03)
			Test_Update 	= FALSE;																					// (Test v0.00.03)
			editMode 		= FALSE;																					// (Test v0.00.01)
			last_editMode 	= FALSE;																					// (Test v0.00.01)
			CreateMainTestMenu();																						// (Test v0.00.01)
			old_routine_status = 0;																						// (Test v0.00.01)
			screenState = nextscreenState;																				// (Test v0.00.01)
			return 1;																									// (Test v0.00.01)
		}																												// (Test v0.00.01)
		case CREATE_TEST_LCD_COLORS:																					// (Test v0.00.01)
		case CREATE_TEST_LCD_COLOR_LINES:																				// (Test v0.00.03)
		{																												// (Test v0.00.01)
			test_color = 0;																								// (Test v0.00.03)
			test_tick = tick + ((DWORD) TICKS_PER_SEC * 2);																// (Test v0.00.03)
			Test_Update 	= FALSE;																					// (Test v0.00.03)
			editMode 		= FALSE;																					// (Test v0.00.01)
			last_editMode 	= FALSE;																					// (Test v0.00.01)
			CreateMainTestMenu();																						// (Test v0.00.01)
			old_routine_status = 0;																						// (Test v0.00.01)
			screenState = nextscreenState;																				// (Test v0.00.01)
			return 1;																									// (Test v0.00.01)
		}																												// (Test v0.00.01)
		case CREATE_SHUTDOWN:																							// (Test v0.00.01)
		{																												// (Test v0.00.01)
			Shutdown_Power();																							// (Test v0.00.01)
			return 1;																									// (Test v0.00.01)
		}																												// (Test v0.00.01)
#endif
		case DISPLAY_MENU:
		case DISPLAY_SETTINGS:
		case DISPLAY_PRODUCT:
		case DISPLAY_LANGUAGE:
		case DISPLAY_CALENDAR:
		case DISPLAY_CLOCK:
		case DISPLAY_PASSWORD:
		case DISPLAY_SYSTEM:
		case DISPLAY_FEATURES:
		case DISPLAY_SOURCE_CHECK:
		case DISPLAY_BACKGROUND_CHECK:
		case DISPLAY_INTEGRATE:
		case DISPLAY_INTEGRATE_REPORT:																					// (9DP v1.01.02)
		case DISPLAY_INTEGRATE_VALUE:																					// (9DP v1.01.02)
		case DISPLAY_DATA_LOGGING:
		case DISPLAY_DATA_LOGGING_REPORT:
		case DISPLAY_DATA_LOGGING_REPORT_SYSTEM:
		case DISPLAY_DATA_LOGGING_REPORT_INSTRUMENT:
		case DISPLAY_DATA_LOGGING_REPORT_READINGS:
		case DISPLAY_CONTROLS:
		case DISPLAY_BACKLIGHT:
		case DISPLAY_AUTO_BACKLIGHT:
		case DISPLAY_AUDIO:
		case DISPLAY_GPS:
		case DISPLAY_BLUETOOTH:
		case DISPLAY_DETECTOR:																							// (9DP v1.01.01)
		case DISPLAY_DETECTOR_SETUP:																					// (9DP v1.01.01)
		case DISPLAY_DISPLAY:
		case DISPLAY_DISPLAY_SETUP:
		case DISPLAY_VIEW:
		case DISPLAY_VIEW_APPEARANCE:																					// (9DP v1.01.01)
		case DISPLAY_VIEW_FUNCTIONS:																					// (9DP v1.01.01)
		case DISPLAY_VIEW_FUNCTION:																						// (9DP v1.01.01)
		case DISPLAY_VIEW_ALERTS:
		case DISPLAY_VIEW_ALERTS_READING:
		case DISPLAY_VIEW_ALARMS:
		case DISPLAY_VIEW_ALARMS_READING:
		case DISPLAY_ALERTS_ALARMS:
		case DISPLAY_ALERTS:
		case DISPLAY_ALARMS:
		case DISPLAY_FUNCTIONS:
		case DISPLAY_CALIBRATION:
		case DISPLAY_BLANK_MENU:
		case DISPLAY_PASSWORD_RESPONSE:
		case DISPLAY_LINE:
		{
			ChangeMenuLine();
			return 1;
		}
#ifdef TEST_MODE
		case DISPLAY_INSTRUMENT_MENU:																					// (Test v0.00.05)
		case DISPLAY_9D_TEST_MENU:																						// (Test v0.00.05)
		case DISPLAY_3D_TEST_MENU:																						// (Test v0.00.05)
		case DISPLAY_276D_TEST_MENU:																					// (Test v0.00.05)
		case DISPLAY_TEST_MENU:																							// (Test v0.00.01)
		case DISPLAY_TEST_MAIN_BOARD:																					// (Test v0.00.01)
		case DISPLAY_TEST_KERNEL:																						// (Test v0.00.01)
		case DISPLAY_TEST_EXT_MEM:																						// (Test v0.00.01)
		case DISPLAY_TEST_EEPROM:																						// (Test v0.00.01)
		case DISPLAY_TEST_LCD_BOARD:																					// (Test v0.00.01)
		case DISPLAY_TEST_HV_BOARD:																						// (Test v0.00.01)
		case DISPLAY_TEST_ELECTROMETER_BOARD:																			// (Test v0.00.01)
		{																												// (Test v0.00.03)
			ChangeMenuLine();																							// (Test v0.00.03)
			return 1;																									// (Test v0.00.03)
		}																												// (Test v0.00.03)
		case DISPLAY_TEST_RTCC:																							// (Test v0.00.01)
		{																												// (Test v0.00.01)
			ChangeTestMenuLine(MenuLine);																				// (Test v0.00.01)
			new_routine_status = RTCCTest(MenuLine);																	// (Test v0.00.01)
			if(new_routine_status != old_routine_status)																// (Test v0.00.01)
			{																											// (Test v0.00.01)
				switch(new_routine_status)																				// (Test v0.00.01)
				{																										// (Test v0.00.01)
					case 1:																								// (Test v0.00.01)
					{																									// (Test v0.00.01)
						break;																							// (Test v0.00.01)
					}																									// (Test v0.00.01)
					case 2:																								// (Test v0.00.01)
					case 3:																								// (Test v0.00.01)
					case 4:																								// (Test v0.00.01)
					{																									// (Test v0.00.01)
						MenuLine = (signed int) new_routine_status;														// (Test v0.00.01)
						break;																							// (Test v0.00.01)
					}																									// (Test v0.00.01)
				}																										// (Test v0.00.01)
				old_routine_status = new_routine_status;																// (Test v0.00.01)
			}																											// (Test v0.00.01)
			return 1;																									// (Test v0.00.01)
		}																												// (Test v0.00.01)
		case DISPLAY_TEST_AUDIO:																						// (Test v0.00.01)
		{																												// (Test v0.00.01)
			ChangeTestMenuLine(MenuLine);																				// (Test v0.00.01)
			new_routine_status = AudioTest(MenuLine);																	// (Test v0.00.01)
			if(new_routine_status != old_routine_status)																// (Test v0.00.01)
			{																											// (Test v0.00.01)
				switch(new_routine_status)																				// (Test v0.00.01)
				{																										// (Test v0.00.01)
					case 1:																								// (Test v0.00.01)
					{																									// (Test v0.00.01)
						break;																							// (Test v0.00.01)
					}																									// (Test v0.00.01)
					case 2:																								// (Test v0.00.01)
					case 3:																								// (Test v0.00.01)
					case 4:																								// (Test v0.00.01)
					{																									// (Test v0.00.01)
						MenuLine = (signed int) new_routine_status;														// (Test v0.00.01)
						break;																							// (Test v0.00.01)
					}																									// (Test v0.00.01)
				}																										// (Test v0.00.01)
				old_routine_status = new_routine_status;																// (Test v0.00.01)
			}																											// (Test v0.00.01)
			return 1;																									// (Test v0.00.01)
		}																												// (Test v0.00.01)
		case DISPLAY_TEST_EEPROM_PRIMARY_RW:																			// (Test v0.00.01)
		{																												// (Test v0.00.01)
			ChangeTestMenuLine(MenuLine);																				// (Test v0.00.01)
			new_routine_status = EEPROMPrimaryRWTest(MenuLine);															// (Test v0.00.01)
			if(new_routine_status != old_routine_status)																// (Test v0.00.01)
			{																											// (Test v0.00.01)
				switch(new_routine_status)																				// (Test v0.00.01)
				{																										// (Test v0.00.01)
					case 1:																								// (Test v0.00.01)
					{																									// (Test v0.00.01)
						break;																							// (Test v0.00.01)
					}																									// (Test v0.00.01)
					case 2:																								// (Test v0.00.01)
					case 3:																								// (Test v0.00.01)
					case 4:																								// (Test v0.00.01)
					{																									// (Test v0.00.01)
						MenuLine = (signed int) new_routine_status;														// (Test v0.00.01)
						break;																							// (Test v0.00.01)
					}																									// (Test v0.00.01)
				}																										// (Test v0.00.01)
				old_routine_status = new_routine_status;																// (Test v0.00.01)
			}																											// (Test v0.00.01)
			return 1;																									// (Test v0.00.01)
		}																												// (Test v0.00.01)
		case DISPLAY_TEST_EEPROM_PRIMARY_ERASE:																			// (Test v0.00.01)
		{																												// (Test v0.00.01)
			ChangeTestMenuLine(MenuLine);																				// (Test v0.00.01)
			new_routine_status = EEPROMPrimaryEraseTest(MenuLine);														// (Test v0.00.01)
			if(new_routine_status != old_routine_status)																// (Test v0.00.01)
			{																											// (Test v0.00.01)
				switch(new_routine_status)																				// (Test v0.00.01)
				{																										// (Test v0.00.01)
					case 1:																								// (Test v0.00.01)
					{																									// (Test v0.00.01)
						break;																							// (Test v0.00.01)
					}																									// (Test v0.00.01)
					case 2:																								// (Test v0.00.01)
					case 3:																								// (Test v0.00.01)
					case 4:																								// (Test v0.00.01)
					{																									// (Test v0.00.01)
						MenuLine = (signed int) new_routine_status;														// (Test v0.00.01)
						break;																							// (Test v0.00.01)
					}																									// (Test v0.00.01)
				}																										// (Test v0.00.01)
				old_routine_status = new_routine_status;																// (Test v0.00.01)
			}																											// (Test v0.00.01)
			return 1;																									// (Test v0.00.01)
		}																												// (Test v0.00.01)
		case DISPLAY_TEST_EEPROM_SECONDARY_RW:																			// (Test v0.00.01)
		{																												// (Test v0.00.01)
			ChangeTestMenuLine(MenuLine);																				// (Test v0.00.01)
			new_routine_status = EEPROMSecondaryRWTest(MenuLine);														// (Test v0.00.01)
			if(new_routine_status != old_routine_status)																// (Test v0.00.01)
			{																											// (Test v0.00.01)
				switch(new_routine_status)																				// (Test v0.00.01)
				{																										// (Test v0.00.01)
					case 1:																								// (Test v0.00.01)
					{																									// (Test v0.00.01)
						break;																							// (Test v0.00.01)
					}																									// (Test v0.00.01)
					case 2:																								// (Test v0.00.01)
					case 3:																								// (Test v0.00.01)
					case 4:																								// (Test v0.00.01)
					{																									// (Test v0.00.01)
						MenuLine = (signed int) new_routine_status;														// (Test v0.00.01)
						break;																							// (Test v0.00.01)
					}																									// (Test v0.00.01)
				}																										// (Test v0.00.01)
				old_routine_status = new_routine_status;																// (Test v0.00.01)
			}																											// (Test v0.00.01)
			return 1;																									// (Test v0.00.01)
		}																												// (Test v0.00.01)
		case DISPLAY_TEST_EEPROM_SECONDARY_ERASE:																		// (Test v0.00.01)
		{																												// (Test v0.00.01)
			ChangeTestMenuLine(MenuLine);																				// (Test v0.00.01)
			new_routine_status = EEPROMSecondaryEraseTest(MenuLine);													// (Test v0.00.01)
			if(new_routine_status != old_routine_status)																// (Test v0.00.01)
			{																											// (Test v0.00.01)
				switch(new_routine_status)																				// (Test v0.00.01)
				{																										// (Test v0.00.01)
					case 1:																								// (Test v0.00.01)
					{																									// (Test v0.00.01)
						break;																							// (Test v0.00.01)
					}																									// (Test v0.00.01)
					case 2:																								// (Test v0.00.01)
					case 3:																								// (Test v0.00.01)
					case 4:																								// (Test v0.00.01)
					{																									// (Test v0.00.01)
						MenuLine = (signed int) new_routine_status;														// (Test v0.00.01)
						break;																							// (Test v0.00.01)
					}																									// (Test v0.00.01)
				}																										// (Test v0.00.01)
				old_routine_status = new_routine_status;																// (Test v0.00.01)
			}																											// (Test v0.00.01)
			return 1;																									// (Test v0.00.01)
		}																												// (Test v0.00.01)
		case DISPLAY_TEST_EXT_MEM_RW:																					// (Test v0.00.01)
		{																												// (Test v0.00.01)
			ChangeTestMenuLine(MenuLine);																				// (Test v0.00.01)
			new_routine_status = ExternalMemoryRWTest(MenuLine);														// (Test v0.00.01)
			if(new_routine_status != old_routine_status)																// (Test v0.00.01)
			{																											// (Test v0.00.01)
				switch(new_routine_status)																				// (Test v0.00.01)
				{																										// (Test v0.00.01)
					case 1:																								// (Test v0.00.01)
					{																									// (Test v0.00.01)
						break;																							// (Test v0.00.01)
					}																									// (Test v0.00.01)
					case 2:																								// (Test v0.00.01)
					case 3:																								// (Test v0.00.01)
					case 4:																								// (Test v0.00.01)
					{																									// (Test v0.00.01)
						MenuLine = (signed int) new_routine_status;														// (Test v0.00.01)
						break;																							// (Test v0.00.01)
					}																									// (Test v0.00.01)
				}																										// (Test v0.00.01)
				old_routine_status = new_routine_status;																// (Test v0.00.01)
			}																											// (Test v0.00.01)
			return 1;																									// (Test v0.00.01)
		}																												// (Test v0.00.01)
		case DISPLAY_TEST_EXT_MEM_ADDR:																					// (Test v0.00.01)
		{																												// (Test v0.00.01)
			ChangeTestMenuLine(MenuLine);																				// (Test v0.00.01)
			new_routine_status = ExternalMemoryAddressTest(MenuLine);													// (Test v0.00.01)
			if(new_routine_status != old_routine_status)																// (Test v0.00.01)
			{																											// (Test v0.00.01)
				switch(new_routine_status)																				// (Test v0.00.01)
				{																										// (Test v0.00.01)
					case 1:																								// (Test v0.00.01)
					{																									// (Test v0.00.01)
						break;																							// (Test v0.00.01)
					}																									// (Test v0.00.01)
					case 2:																								// (Test v0.00.01)
					case 3:																								// (Test v0.00.01)
					case 4:																								// (Test v0.00.01)
					{																									// (Test v0.00.01)
						MenuLine = (signed int) new_routine_status;														// (Test v0.00.01)
						break;																							// (Test v0.00.01)
					}																									// (Test v0.00.01)
				}																										// (Test v0.00.01)
				old_routine_status = new_routine_status;																// (Test v0.00.01)
			}																											// (Test v0.00.01)
			return 1;																									// (Test v0.00.01)
		}																												// (Test v0.00.01)
		case DISPLAY_TEST_EXT_MEM_DATA:																					// (Test v0.00.01)
		{																												// (Test v0.00.01)
			ChangeTestMenuLine(MenuLine);																				// (Test v0.00.01)
			new_routine_status = ExternalMemoryDataTest(MenuLine);														// (Test v0.00.01)
			if(new_routine_status != old_routine_status)																// (Test v0.00.01)
			{																											// (Test v0.00.01)
				switch(new_routine_status)																				// (Test v0.00.01)
				{																										// (Test v0.00.01)
					case 1:																								// (Test v0.00.01)
					{																									// (Test v0.00.01)
						break;																							// (Test v0.00.01)
					}																									// (Test v0.00.01)
					case 2:																								// (Test v0.00.01)
					case 3:																								// (Test v0.00.01)
					case 4:																								// (Test v0.00.01)
					{																									// (Test v0.00.01)
						MenuLine = (signed int) new_routine_status;														// (Test v0.00.01)
						break;																							// (Test v0.00.01)
					}																									// (Test v0.00.01)
				}																										// (Test v0.00.01)
				old_routine_status = new_routine_status;																// (Test v0.00.01)
			}																											// (Test v0.00.01)
			return 1;																									// (Test v0.00.01)
		}																												// (Test v0.00.01)
		case DISPLAY_TEST_LCD_COLOR_LINES:																				// (Test v0.00.03)	
		{																												// (Test v0.00.01)
			if(Test_Update)																								// (Test v0.00.03)
			{																											// (Test v0.00.01)
				test_tick = tick + ((DWORD) TICKS_PER_SEC * 2);															// (Test v0.00.03)
				Test_Update = FALSE;																					// (Test v0.00.03)
				if(test_color == 0)																						// (Test v0.00.01)
					test_color = 0x0001;																				// (Test v0.00.01)
				else																									// (Test v0.00.01)
					test_color = test_color << 1;																		// (Test v0.00.01)
				temp_color = GetColor();																				// (Test v0.00.01)
				SetColor(test_color);																					// (Test v0.00.01)
				Bar(18, 160, 295, 219);																					// (Test v0.00.01)
				SetFont((void*)&GOLFontDefault);																		// (Test v0.00.01)
				inverted_color = test_color ^ 0xFFFF;																	// (Test v0.00.01)
				SetColor(inverted_color);																				// (Test v0.00.01)
				sprintf(char_buffer, "BKG. COLOR: 0x%04X\0", test_color);												// (Test v0.00.01)
				Convert_For_Display(xchar_buffer, NULL, char_buffer, 25);												// (Test v0.00.01)
				msgwidth = GetTextWidth((XCHAR*) xchar_buffer, (void*)&GOLFontDefault);									// (Test v0.00.01)
				msgwidth = (320 - msgwidth) / 2;																		// (Test v0.00.01)
				MoveTo(msgwidth, 180);																					// (Test v0.00.01)
				while(!OutText((XCHAR*)xchar_buffer));																	// (Test v0.00.01)
				SetColor(temp_color);																					// (Test v0.00.01)
			}																											// (Test v0.00.01)
			ChangeMenuLine();																							// (Test v0.00.01)
			return 1;																									// (Test v0.00.01)
		}																												// (Test v0.00.01)
		case DISPLAY_TEST_LCD_COLORS:																					// (Test v0.00.03)	
		{																												// (Test v0.00.03)
			if(Test_Update)																								// (Test v0.00.03)
			{																											// (Test v0.00.03)
				test_tick = tick + ((DWORD) TICKS_PER_SEC * 2);															// (Test v0.00.03)
				Test_Update = FALSE;																					// (Test v0.00.03)
				if(test_color == 28)																					// (Test v0.00.03)
					test_color = 0;																						// (Test v0.00.03)
				else																									// (Test v0.00.03)
					test_color++;																						// (Test v0.00.03)
				temp_color = GetColor();																				// (Test v0.00.03)
				SetColor(color_array[test_color]);																		// (Test v0.00.03)
//				Bar(18, 160, 295, 219);																					// (Test v0.00.03)
				Bar(0, 0, 319, 239);																					// cover full screen (Test v0.00.04)
				SetFont((void*)&GOLFontDefault);																		// (Test v0.00.03)
				inverted_color = color_array[test_color] ^ 0x8410;														// (Test v0.00.03)
				SetColor(inverted_color);																				// (Test v0.00.03)
				CreateColorList(test_color, char_buffer);																// (Test v0.00.03)
				Convert_For_Display(xchar_buffer, NULL, char_buffer, 25);												// (Test v0.00.03)
				msgwidth = GetTextWidth((XCHAR*) xchar_buffer, (void*)&GOLFontDefault);									// (Test v0.00.03)
				msgwidth = (320 - msgwidth) / 2;																		// (Test v0.00.03)
				MoveTo(msgwidth, 180);																					// (Test v0.00.03)
				while(!OutText((XCHAR*)xchar_buffer));																	// (Test v0.00.03)

				sprintf(char_buffer, "<press ENTER to exit>\0");														// (Test v0.00.04)
				Convert_For_Display(xchar_buffer, NULL, char_buffer, 25);												// (Test v0.00.04)
				msgwidth = GetTextWidth((XCHAR*) xchar_buffer, (void*)&GOLFontDefault);									// (Test v0.00.04)
				msgwidth = (320 - msgwidth) / 2;																		// (Test v0.00.04)
				MoveTo(msgwidth, 210);																					// (Test v0.00.04)
				while(!OutText((XCHAR*)xchar_buffer));																	// (Test v0.00.04)

				SetColor(temp_color);																					// (Test v0.00.03)
			}																											// (Test v0.00.03)
			ChangeMenuLine();																							// (Test v0.00.03)
			return 1;																									// (Test v0.00.03)
		}																												// (Test v0.00.03)
		case DISPLAY_TEST_BATTERY_VOLTAGE:																				// (Test v0.00.01)
		{																												// (Test v0.00.01)
			if(Test_Update)																								// (Test v0.00.03)
			{																											// (Test v0.00.03)
				Battery_Voltage = BatteryVoltageTest();																	// (Test v0.00.01)
				sprintf(char_buffer, "%.3f\0", Battery_Voltage);														// changed to 3 digits of precision (Test v0.00.04)
				Convert_For_Display(pmsgScreen.txt_text[1], NULL, char_buffer, 25);										// (Test v0.00.01)
				ChangeMenuLine();																						// (Test v0.00.03)
				test_tick = tick + (DWORD) TICKS_PER_SEC;																// (Test v0.00.03)
				Test_Update = FALSE;																					// (Test v0.00.03)
			}																											// (Test v0.00.03)
			else																										// (Test v0.00.03)
			{																											// (Test v0.00.03)
				ChangeMenuLine();																						// (Test v0.00.03)
			}																											// (Test v0.00.03)
			return 1;																									// (Test v0.00.01)
		}																												// (Test v0.00.01)
		case DISPLAY_TEST_DISPLAY_VOLTAGE:																				// (Test v0.00.04)
		{																												// (Test v0.00.04)
			if(Test_Update)																								// (Test v0.00.04)
			{																											// (Test v0.00.04)
				Display_Voltage = DisplayVoltageTest();																	// (Test v0.00.04)
				sprintf(char_buffer, "%.3f\0", Display_Voltage);														// (Test v0.00.04)
				Convert_For_Display(pmsgScreen.txt_text[1], NULL, char_buffer, 25);										// (Test v0.00.04)
				ChangeMenuLine();																						// (Test v0.00.04)
				test_tick = tick + (DWORD) TICKS_PER_SEC;																// (Test v0.00.04)
				Test_Update = FALSE;																					// (Test v0.00.04)
			}																											// (Test v0.00.04)
			else																										// (Test v0.00.04)
			{																											// (Test v0.00.04)
				ChangeMenuLine();																						// (Test v0.00.04)
			}																											// (Test v0.00.04)
			return 1;																									// (Test v0.00.04)
		}																												// (Test v0.00.04)
		case DISPLAY_TEST_LIGHT_SENSOR:																					// (Test v0.00.01)
		{																												// (Test v0.00.01)
			if(Test_Update)																								// (Test v0.00.03)
			{																											// (Test v0.00.03)
				Light_Sensor_Voltage = LightSensorTest();																// (Test v0.00.01)
				sprintf(char_buffer, "%.3f\0", Light_Sensor_Voltage);													// changed to 3 digits of precision (Test v0.00.04)
				Convert_For_Display(pmsgScreen.txt_text[1], NULL, char_buffer, 25);										// (Test v0.00.01)
				ChangeMenuLine();																						// (Test v0.00.03)
				test_tick = tick + (DWORD) TICKS_PER_SEC;																// (Test v0.00.03)
				Test_Update = FALSE;																					// (Test v0.00.03)
			}																											// (Test v0.00.03)
			else																										// (Test v0.00.03)
			{																											// (Test v0.00.03)
				ChangeMenuLine();																						// (Test v0.00.03)
			}																											// (Test v0.00.03)
			return 1;																									// (Test v0.00.01)
		}																												// (Test v0.00.01)
		case DISPLAY_TEST_HIGH_VOLTAGE_1:																				// (Test v0.00.01)
		{																												// (Test v0.00.01)
			if(Test_Update)																								// (Test v0.00.03)
			{																											// (Test v0.00.03)
				High_Voltage_1 = HighVoltageTest(1);																	// (Test v0.00.01)
				sprintf(char_buffer, "%.1f\0", High_Voltage_1);															// (Test v0.00.01)
				Convert_For_Display(pmsgScreen.txt_text[1], NULL, char_buffer, 25);										// (Test v0.00.01)
				ChangeMenuLine();																						// (Test v0.00.03)
				test_tick = tick + (DWORD) TICKS_PER_SEC;																// (Test v0.00.03)
				Test_Update = FALSE;																					// (Test v0.00.03)
			}																											// (Test v0.00.03)
			else																										// (Test v0.00.03)
			{																											// (Test v0.00.03)
				ChangeMenuLine();																						// (Test v0.00.03)
			}																											// (Test v0.00.03)
			return 1;																									// (Test v0.00.01)
		}																												// (Test v0.00.01)
		case DISPLAY_TEST_INPUT_VOLTAGE_1:																				// (Test v0.00.01)
		{																												// (Test v0.00.01)
			if(Test_Update)																								// (Test v0.00.03)
			{																											// (Test v0.00.03)
				Input_Voltage_1 = InputVoltageTest(1);																	// (Test v0.00.01)
				sprintf(char_buffer, "%.3f\0", Input_Voltage_1);														// (Test v0.00.01)
				Convert_For_Display(pmsgScreen.txt_text[1], NULL, char_buffer, 25);										// (Test v0.00.01)
				ChangeMenuLine();																						// (Test v0.00.03)
				test_tick = tick + (DWORD) TICKS_PER_SEC;																// (Test v0.00.03)
				Test_Update = FALSE;																					// (Test v0.00.03)
			}																											// (Test v0.00.03)
			else																										// (Test v0.00.03)
			{																											// (Test v0.00.03)
				ChangeMenuLine();																						// (Test v0.00.03)
			}																											// (Test v0.00.03)
			return 1;																									// (Test v0.00.01)
		}																												// (Test v0.00.01)
//		case DISPLAY_TEST_HIGH_VOLTAGE_2:																				// (Test v0.00.01)
//		{																												// (Test v0.00.01)
//			High_Voltage_2 = HighVoltageTest(2);																		// (Test v0.00.01)
//			sprintf(char_buffer, "%.1f\0", High_Voltage_2);																// (Test v0.00.01)
//			Convert_For_Display(pmsgScreen.txt_text[1], NULL, char_buffer, 25);											// (Test v0.00.01)
//			ChangeMenuLine();																							// (Test v0.00.01)
//			return 1;																									// (Test v0.00.01)
//		}																												// (Test v0.00.01)
//		case DISPLAY_TEST_INPUT_VOLTAGE_2:																				// (Test v0.00.01)
//		{																												// (Test v0.00.01)
//			Input_Voltage_2 = InputVoltageTest(2);																		// (Test v0.00.01)
//			sprintf(char_buffer, "%.3f\0", Input_Voltage_2);															// (Test v0.00.01)
//			Convert_For_Display(pmsgScreen.txt_text[1], NULL, char_buffer, 25);											// (Test v0.00.01)
//			ChangeMenuLine();																							// (Test v0.00.01)
//			return 1;																									// (Test v0.00.01)
//		}																												// (Test v0.00.01)
		case DISPLAY_TEST_ELECTROMETER_TEMPERATURE:																		// (Test v0.00.01)
		{																												// (Test v0.00.01)
			if(Test_Update)																								// (Test v0.00.03)
			{																											// (Test v0.00.03)
				TemperatureTest();																						// (Test v0.00.01)
				sprintf(char_buffer, "%.1f\0", Temperature_Reading);													// (Test v0.00.01)
				Convert_For_Display(pmsgScreen.txt_text[1], NULL, char_buffer, 25);										// (Test v0.00.01)
				ChangeMenuLine();																						// (Test v0.00.03)
				test_tick = tick + (DWORD) TICKS_PER_SEC;																// (Test v0.00.03)
				Test_Update = FALSE;																					// (Test v0.00.03)
			}																											// (Test v0.00.03)
			else																										// (Test v0.00.03)
			{																											// (Test v0.00.03)
				ChangeMenuLine();																						// (Test v0.00.03)
			}																											// (Test v0.00.03)
			return 1;																									// (Test v0.00.01)
		}																												// (Test v0.00.01)
		case DISPLAY_TEST_KEYPAD:																						// (Test v0.00.01)
		{																												// (Test v0.00.01)
//			Check_Buttons();																							// (Test v0.00.01)
			ChangeMenuLine();																							// (Test v0.00.01)
			return 1;																									// (Test v0.00.01)
		}																												// (Test v0.00.01)
#endif
		case DISPLAY_EDIT_LINE:
		{
			if(editMode == TRUE)
			{
				if(SubMenuLine != last_SubMenuLine)
				{
					switch(pmsgScreen.line_item_type[MenuLine])
					{
						case TYPE_TIME_HOURS:
						{
							sprintf(stringconversion,"%02d\0",SubMenuLine);
							do
							{
								int_status = INTDisableInterrupts();
								i2c2Write(i2cADDR_PIC_RTCC, i2cPIC_RTC_24HOUR, 1, SubMenuLine, 1);
								INTRestoreInterrupts(int_status);
								DelayMs(25);
								RTCCUpdate();
							} while(SubMenuLine != lmi.settings.clock.info.time.hours);
							break;
						}
						case TYPE_TIME_MINUTES:
						{
							sprintf(stringconversion,"%02d\0",SubMenuLine);
							do
							{
								int_status = INTDisableInterrupts();
								i2c2Write(i2cADDR_PIC_RTCC, i2cPIC_RTC_MINUTE, 1, SubMenuLine, 1);
								INTRestoreInterrupts(int_status);
								DelayMs(25);
								RTCCUpdate();
							} while(SubMenuLine != lmi.settings.clock.info.time.minutes);
							break;
						}
						case TYPE_TIME_SECONDS:
						{
							sprintf(stringconversion,"%02d\0",SubMenuLine);
							do
							{
								int_status = INTDisableInterrupts();
								i2c2Write(i2cADDR_PIC_RTCC, i2cPIC_RTC_SECOND, 1, SubMenuLine, 1);
								INTRestoreInterrupts(int_status);
								DelayMs(25);
								RTCCUpdate();
							} while(SubMenuLine != lmi.settings.clock.info.time.seconds);
							break;
						}
						case TYPE_TIME_FORM:
						{
							if(last_SubMenuLine == 12)
								SubMenuLine = 24;
							else
								SubMenuLine = 12;
							lmi.settings.clock.info.time.format = SubMenuLine;
							sprintf(stringconversion,"%02d\0",SubMenuLine);
							break;
						}
						case TYPE_DATE_MONTH:
						{
							CreateMonthList(SubMenuLine, (char *)stringconversion);
							do
							{
								int_status = INTDisableInterrupts();
								i2c2Write(i2cADDR_PIC_RTCC, i2cPIC_RTC_MONTH, 1, SubMenuLine, 1);
								INTRestoreInterrupts(int_status);
								DelayMs(25);
								RTCCUpdate();
							} while(SubMenuLine != lmi.settings.calendar.info.date.month);
							break;
						}
						case TYPE_DATE_DAY:
						{
							sprintf(stringconversion,"%02d\0",SubMenuLine);
							do
							{
								int_status = INTDisableInterrupts();
								i2c2Write(i2cADDR_PIC_RTCC, i2cPIC_RTC_DAY, 1, SubMenuLine, 1);
								INTRestoreInterrupts(int_status);
								DelayMs(25);
								RTCCUpdate();
							} while(SubMenuLine != lmi.settings.calendar.info.date.day);
							break;
						}
						case TYPE_DATE_YEAR:
						{
							sprintf(stringconversion,"20%02d\0",SubMenuLine);
							do
							{
								int_status = INTDisableInterrupts();
								i2c2Write(i2cADDR_PIC_RTCC, i2cPIC_RTC_YEAR, 1, SubMenuLine, 1);
								INTRestoreInterrupts(int_status);
								DelayMs(25);
								RTCCUpdate();
							} while(SubMenuLine != lmi.settings.calendar.info.date.year);
							break;
						}
						case TYPE_PMODE_AUDIO:
						case TYPE_PMODE_GPS:
						case TYPE_PMODE_BLUETOOTH:
						case TYPE_PMODE_LOGGING:
						{
							CreatePowerUpModeList(SubMenuLine, (char *)stringconversion);
							break;
						}
						case TYPE_PMODE_BACKLIGHT:																		// (9DP v1.01.01)
						case TYPE_OMODE_BACKLIGHT:
						case TYPE_OMODE_GPS:
						case TYPE_OMODE_BLUETOOTH:
						case TYPE_ENABLE:
						{
							CreateOperatingModeList(SubMenuLine, (char *)stringconversion);
							break;
						}
						case TYPE_OMODE_AUDIO:
						{
							CreateAudioOperatingModeList(SubMenuLine, (char *)stringconversion);
							break;
						}
						case TYPE_100_PERCENT:
						{																								// (9DP v1.01.01)
							CreatePercentageList(SubMenuLine, (char *)stringconversion);								// (9DP v1.01.01)
							break;																						// (9DP v1.01.01)
						}																								// (9DP v1.01.01)
						case TYPE_3_MINUTES:
						case TYPE_1_HOUR:
						case TYPE_VALUE:
						case TYPE_OFFSET:																				// (Test v0.00.06)
						{
							sprintf(stringconversion,"%d\0",SubMenuLine);
							break;
						}
						case TYPE_COLOR:
						{
							CreateColorList(SubMenuLine, (char *)stringconversion);
							break;
						}
						case TYPE_CURRENT_VIEW:
						case TYPE_PMODE_DISPLAY:
						{
							CreateCurrentViewList(SubMenuLine, (char *)stringconversion);
							break;
						}
						case TYPE_UNITS:
						case TYPE_INTEGRATE_UNITS:																		// (9DP v1.01.02)
						{
							CreateUnitsList(SubMenuLine, (char *)stringconversion);
							break;
						}
						case TYPE_MULTIPLIER:
						case TYPE_INTEGRATE_MULTIPLIER:																	// (9DP v1.01.02)
						{
							CreateMultiplierList(SubMenuLine, (char *)stringconversion);
							break;
						}
						case TYPE_MODEL_NUMBER:
						{
							CreateModelList(SubMenuLine, (char *)stringconversion);
							break;
						}
						case TYPE_LANGUAGE_SELECT:
						{
							CreateLanguageList(SubMenuLine, (char *)stringconversion);
							break;
						}
						case TYPE_VIEW_FUNCTION:																		// (9DP v1.01.01)
						{																								// (9DP v1.01.01)
							CreateViewFunctionList(SubMenuLine, (char *)stringconversion);								// (9DP v1.01.01)
							break;																						// (9DP v1.01.01)
						}																								// (9DP v1.01.01)
						case TYPE_INSTRUMENT_FUNCTION:																	// (9DP v1.01.01)
						{																								// (9DP v1.01.01)
							CreateInstrumentFunctionsList(SubMenuLine, (char *)stringconversion);						// (9DP v1.01.01)
							break;																						// (9DP v1.01.01)
						}																								// (9DP v1.01.01)
#ifdef EEPROM_DETECTOR_PROPERTIES_LOCATION																				// (9DP v1.01.01)
						case TYPE_DETECTOR:																				// (9DP v1.01.01)
						{																								// (9DP v1.01.01)
							CreateDetectorList((unsigned short)SubMenuLine, (char *)stringconversion);					// (9DP v1.01.01)
							break;																						// (9DP v1.01.01)
						}																								// (9DP v1.01.01)
#endif																													// (9DP v1.01.01)
						case TYPE_RANGE:																				// (Test v0.00.06)
						{																								// (Test v0.00.06)
							CreateRangeList(SubMenuLine, (char *)stringconversion);										// (Test v0.00.06)
							break;																						// (Test v0.00.06)
						}																								// (Test v0.00.06)
						default:
						{
							return 1;
						}
					}

					Convert_For_Display(pmsgScreen.txt_text[MenuLine], NULL, stringconversion, 28);
					ChangeEditLine();
				}	
				else if(last_editMode != editMode)
				{
					ChangeEditLine();
				}
			}
			else
			{
				if(last_editMode != editMode)
				{
					ChangeEditLine();
					switch(pmsgScreen.line_item_type[MenuLine])
					{
						case TYPE_BOOL:
						{
							break;
						}
						case TYPE_OMODE_AUDIO:
						{
							UINT8_value = (UINT8 *)pmsgScreen.line_item[MenuLine];
							*UINT8_value = SubMenuLine;
							Set_Audio_Tone();
							break;
						}
						case TYPE_OMODE_BACKLIGHT:
						{
							UINT8_value = (UINT8 *)pmsgScreen.line_item[MenuLine];
							*UINT8_value = SubMenuLine;
							Backlight_Mode = lmi.controls.backlight.info.operating_mode;
							break;
						}
						case TYPE_COLOR:
						{
							UINT16_value = (UINT16 *)pmsgScreen.line_item[MenuLine];
							*UINT16_value = SubMenuLine;
							foreground = Designate_Color(lmi.display.detector[Current_Detector].view[Current_View].appearance.info.foreground_color);
							background = Designate_Color(lmi.display.detector[Current_Detector].view[Current_View].appearance.info.background_color);
							break;
						}
						case TYPE_VALUE:
						{
							UINT16_value = (UINT16 *)pmsgScreen.line_item[MenuLine];
							*UINT16_value = SubMenuLine;
							break;
						}
						case TYPE_OFFSET:																				// (Test v0.00.06)
						{																								// (Test v0.00.06)
							UINT16_value = (UINT16 *)pmsgScreen.line_item[MenuLine];									// (Test v0.00.06)
							*UINT16_value = SubMenuLine;																// (Test v0.00.06)
							HVBUpdateOffset(FALSE);																		// (Test v0.00.06)
							break;																						// (Test v0.00.06)			
						}																								// (Test v0.00.06)
						case TYPE_RANGE:																				// (Test v0.00.06)
						{																								// (Test v0.00.06)
							UINT8_value = (UINT8 *)pmsgScreen.line_item[MenuLine];										// (Test v0.00.06)
							*UINT8_value = SubMenuLine;																	// (Test v0.00.06)
							Range_Number = SubMenuLine;																	// (Test v0.00.06)
							Change_Range(Range_Number);																	// (Test v0.00.06)
							break;																						// (Test v0.00.06)			
						}																								// (Test v0.00.06)
						case TYPE_MULTIPLIER:
						{
							UINT8_value = (UINT8 *)pmsgScreen.line_item[MenuLine];
							*UINT8_value = SubMenuLine;
							break;
						}
						case TYPE_INTEGRATE_MULTIPLIER:																	// (9DP v1.01.02)
						{																								// (9DP v1.01.02)
							UINT8_value = (UINT8 *)pmsgScreen.line_item[MenuLine];										// (9DP v1.01.02)
							*UINT8_value = SubMenuLine;																	// (9DP v1.01.02)
							break;																						// (9DP v1.01.02)
						}																								// (9DP v1.01.02)
						case TYPE_100_PERCENT:
						case TYPE_PMODE_GPS:
						case TYPE_PMODE_BLUETOOTH:
						case TYPE_PMODE_BACKLIGHT:
						case TYPE_PMODE_AUDIO:
						case TYPE_PMODE_DISPLAY:
						case TYPE_PMODE_LOGGING:
						case TYPE_OMODE_GPS:
						case TYPE_OMODE_BLUETOOTH:
						case TYPE_ENABLE:
						case TYPE_3_MINUTES:
						case TYPE_UNITS:
						case TYPE_INTEGRATE_UNITS:																		// (9DP v1.01.02)
						case TYPE_LANGUAGE_SELECT:
						case TYPE_UINT8:
						{
							UINT8_value = (UINT8 *)pmsgScreen.line_item[MenuLine];
							*UINT8_value = SubMenuLine;
							break;
						}
						case TYPE_CURRENT_VIEW:
						{
							UINT8_value = (UINT8 *)pmsgScreen.line_item[MenuLine];
							*UINT8_value = SubMenuLine;
							Current_View = lmi.display.detector[Current_Detector].view_setup.info.current_view;
							Current_Function_Index = lmi.display.detector[Current_Detector].view[Current_View].functions.info.function[lmi.display.detector[Current_Detector].view[Current_View].functions.info.current_function].function_index;
							break;
						}
						case TYPE_1_HOUR:
						case TYPE_UINT16:
						{
							UINT16_value = (UINT16 *)pmsgScreen.line_item[MenuLine];
							*UINT16_value = SubMenuLine;
							break;
						}
						case TYPE_UINT32:
						{
							UINT32_value = (UINT32 *)pmsgScreen.line_item[MenuLine];
							*UINT32_value = SubMenuLine;
							break;
						}
						case TYPE_INT8:
						{
							INT8_value = (INT8 *)pmsgScreen.line_item[MenuLine];
							*INT8_value = SubMenuLine;
							break;
						}
						case TYPE_INT16:
						{
							INT16_value = (INT16 *)pmsgScreen.line_item[MenuLine];
							*INT16_value = SubMenuLine;
							break;
						}
						case TYPE_INT32:
						{
							INT32_value = (INT32 *)pmsgScreen.line_item[MenuLine];
							*INT32_value = SubMenuLine;
							break;
						}
						case TYPE_FLOAT:
						{
							float_value = (float *)pmsgScreen.line_item[MenuLine];
							*float_value = SubMenuLine;
							break;
						}
						case TYPE_MODEL_NUMBER:
						{
//							Change_Model_Number(SubMenuLine);
//							lmi.settings.product.info.model = SubMenuLine;
//							CreateModelList(lmi.settings.product.info.model, (char *)pmsgScreen.line_item[MenuLine]);
//							i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_PRODUCT_LOCATION, 2, (BYTE *)lmi.settings.product.buffer, SIZE_PRODUCT);
							break;
						}
#ifdef EEPROM_DETECTOR_PROPERTIES_LOCATION																				// (9DP v1.01.01)
						case TYPE_DETECTOR:																				// (9DP v1.01.01)
						{																								// (9DP v1.01.01)
							break;																						// (9DP v1.01.01)
						}																								// (9DP v1.01.01)
#endif																													// (9DP v1.01.01)
						case TYPE_VIEW_FUNCTION:																		// (9DP v1.01.01)
						{																								// (9DP v1.01.01)
							INT8_value = (INT8 *)pmsgScreen.line_item[MenuLine];										// (9DP v1.01.01)
							*INT8_value = SubMenuLine;																	// (9DP v1.01.01)
							break;																						// (9DP v1.01.01)
						}																								// (9DP v1.01.01)
						case TYPE_INSTRUMENT_FUNCTION:																	// (9DP v1.01.01)
						{																								// (9DP v1.01.01)
							UINT8_value = (UINT8 *)pmsgScreen.line_item[MenuLine];										// (9DP v1.01.01)
							*UINT8_value = SubMenuLine;																	// (9DP v1.01.01)
							break;																						// (9DP v1.01.01)
						}																								// (9DP v1.01.01)
						case TYPE_STRING:
						{
							break;
						}
					}
				}
				ChangeMenuLine();
			}
			last_editMode = editMode;
			last_SubMenuLine = SubMenuLine;
			return 1;
		}
		case DISPLAY_PASSWORD_ENTRY:
		case DISPLAY_EDIT_STRING_LINE:
		{
			if(editMode == TRUE)
			{
				if(Total_Chars != Last_Total_Chars)
				{
					switch(pmsgScreen.line_item_type[MenuLine])
					{
						case TYPE_HIDDEN_STRING:
						{																								// (9DP v1.01.04)
							for(index = 0; index < Total_Chars; index++)												// (9DP v1.01.04)
								stringconversion[index] = '*';															// (9DP v1.01.04)
							stringconversion[index++] = '>';															// (9DP v1.01.04)
							stringconversion[index++] = 0;																// (9DP v1.01.04)
							break;																						// (9DP v1.01.04)
						}																								// (9DP v1.01.04)
						case TYPE_STRING:
						default:
						{
							FormatString(message_string, (unsigned char *)pmsgScreen.line_item[MenuLine], 16);			// (9DP v1.01.04)
							sprintf(stringconversion, "%s>\0",message_string);											// (9DP v1.01.04)
							break;;
						}
					}

					Convert_For_Display(pmsgScreen.txt_text[MenuLine], NULL, stringconversion, 28);
					ChangeEditStringLine();
					Last_Total_Chars = Total_Chars;
				}	
				else if(last_editMode != editMode)
				{
					ChangeEditStringLine();
				}
			}
			else
			{
				if(last_editMode != editMode)
				{
					switch(pmsgScreen.line_item_type[MenuLine])
					{
						case TYPE_MODEL_NUMBER:
						{
//							Change_Model_Number();
						}
						case TYPE_STRING:
						{
							FormatString(stringconversion, (unsigned char *)pmsgScreen.line_item[MenuLine], 16);		// (9DP v1.01.04)
							break;
						}
						case TYPE_HIDDEN_STRING:
						{
							for(index = 0; index < Total_Chars; index++)
								stringconversion[index] = '*';
							stringconversion[index] = 0;
							screenState = CREATE_PASSWORD_RESPONSE;
							break;
						}
					}
					Convert_For_Display(pmsgScreen.txt_text[MenuLine], NULL, stringconversion, 28);
					ChangeEditStringLine();
					Last_Total_Chars = -1;
				}
				ChangeMenuLine();
			}
			last_editMode = editMode;
			last_SubMenuLine = SubMenuLine;
			return 1;
		}
		default:
		{
			return 1;
		}
	}
	return 1;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	WORD GOLMsgCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
//*
//*	Description:
//* 	This function must be implemented by user. GOLMsg() function calls it each time the valid
//*		message for the object received
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	objMsg - translated message for the object,
//* 	pObj - pointer to the object,
//* 	pMsg - pointer to the non-translated, raw GOL message 
//*
//*	Return Values:
//* 	if the function returns non-zero the draw control will be passed to GOL
//*
//*	Remarks:
//* 	None
//*

WORD GOLMsgCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
{
	switch(screenState)
	{
		case MONITOR_RADIATION:
		{
			Check_Buttons();																		// Check the status of the buttons
			i2c2Write(i2cADDR_MAIN_IO_BUTTON, 0x0A, 1, 0x80, 1);									// write to the OLAT Register...... I/O expander pin 7 output
			return 1;
		}
		case DISPLAY_ALERTS:
		{
			return UserDefinedMsgCallback(objMsg, pObj, pMsg);
		}
		default:
		{
			return 1;
		}
	}	
	return 1;	
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Create_Measurement_Screen(void)
//*
//*	Description:
//* 	Create screen
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

void Create_Measurement_Screen(void)
{
	Current_Detector = lmi.display.properties.info.current_detector;
	Current_View = lmi.display.detector[Current_Detector].view_setup.info.current_view;
	Current_Function_Index = lmi.display.detector[Current_Detector].view[Current_View].functions.info.function[lmi.display.detector[Current_Detector].view[Current_View].functions.info.current_function].function_index;

	foreground = Designate_Color(lmi.display.detector[Current_Detector].view[Current_View].appearance.info.foreground_color);			// assign foreground color
	background = Designate_Color(lmi.display.detector[Current_Detector].view[Current_View].appearance.info.background_color);			// assign background color

	Update_Alerts_Alarms(Current_Detector, Current_View);																				// (9DP v1.01.08)

	New_Element_Window();																												// display the current element
	New_Function_Window();																												// display the current function
	New_Icon_Window();																													// display current icons
	AudioPulseInit();																													// initialize audio pulses

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Delay_Reading_Xseconds(void)
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
//* 	called by Create_Measurement_Screen()
//*

//#define COUNTDOWN_TEXT_HEIGHT					82
//#define Y_COUNTDOWN								(Y_ORIGIN - UNITS_TEXT_HEIGHT - COUNTDOWN_TEXT_HEIGHT)
//#define Y_WAIT									Y_COUNTDOWN
//
//void Delay_Reading_Xseconds(void)
//{
//	static unsigned char Last_Xsec_cnt_down = 0;
//	unsigned char interval_numb = 0;
//	unsigned char count_down = 0;
//	unsigned char Xsec_interval;
//	unsigned char X_Samples = 10;																	// moved from global variable
//	unsigned char pp;
//	SHORT centerX_value = 0;
//	char i;
//
//	XCHAR cntdownvalue[3] = {'0'};
//	char  tempcntdownvalue[3] = {'0'};
//	
//	Xsec_interval = X_Samples;
//
//	if(Reset_Pressed == 1)
//	{
//		GetNewSamples = 1;																			// set variable to indicate getting new samples
//		Update_Element(UPDATE_WINDOW);
//	}
//
//	SetColor(foreground);																			// set color to write new units
//	SetFont((void*)&GOLFontDefault);
//
//	centerX_value = GetTextWidth((XCHAR*)CountDownTxt, (void*)&GOLFontDefault);						// display "Wait" message in &Arial_Normal_12
//	centerX_value = 160 - (centerX_value / 2);
//	MoveTo(centerX_value, Y_WAIT);
//	while(!OutText((XCHAR*)CountDownTxt));
//
//	while(interval_numb < (X_Samples * 2))															// times 2 since samples every 1/2 second and X_Samples is in seconds
//	{
//		if(Update_Count_Rate_Flag) 																	// check if Flag is 1 -- due to 500ms interrupt
//		{
//			Update_Count_Rate_Flag = 0;																// clear update flag
//			interval_numb++;																		// increment loop number
//			Xsec_interval = X_Samples - (interval_numb / 2);										// calculate interval
//		}
//
//		if(Xsec_interval != Last_Xsec_cnt_down)
//		{
//			for(pp=0;pp<2;pp++)																		// loop twice to erase and then write
//			{
//				if((pp == 0) && (Last_Xsec_cnt_down != 0))											// erase
//				{
//					SetColor(background);															// set color to clear previous units
//					SetFont((void*)&Normal_Arial_Black_44);
//					sprintf(tempcntdownvalue , "%2d", Last_Xsec_cnt_down);
//				}
//				else if(Xsec_interval != 0)
//				{
//					SetColor(foreground);															// set color to clear previous units
//					SetFont((void*)&Normal_Arial_Black_44);
//					sprintf(tempcntdownvalue , "%2d", Xsec_interval);
//				}
//												
//				for(i = 0; i < 2; i++) 																// was 1
//				{
//					cntdownvalue[i] = (XCHAR)tempcntdownvalue[i];												
//				}
//				
//				centerX_value = GetTextWidth((XCHAR*)cntdownvalue, (void*)&Normal_Arial_Black_44);
//				centerX_value = 160 - (centerX_value / 2);
//				MoveTo(centerX_value, Y_COUNTDOWN);	
//				while(!OutText((XCHAR*)cntdownvalue));
//			}
//			Last_Xsec_cnt_down = Xsec_interval;														// update last value
//		}
//	}
//
//	SetColor(background);																			// set color to write new units
//	SetFont((void*)&GOLFontDefault);
//	centerX_value = GetTextWidth((XCHAR*)CountDownTxt, (void*)&GOLFontDefault);						// &Arial_Normal_12	
//	centerX_value = 160 - (centerX_value / 2);
//	MoveTo(centerX_value, Y_WAIT);
//
//	while(!OutText((XCHAR*)CountDownTxt));
//
//	GetNewSamples = 0;																				// indicate finished getting new samples
//	Update_Arc_Flag = TRUE;																			// update arc
//	Update_Display_Rate_Flag = TRUE;																// update rate
//
//	return;
//}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void CreateCtrlButtons(XCHAR *pTextA, XCHAR *pTextB, XCHAR *pTextC, XCHAR *pTextD)
//*
//*	Description:
//* 	Creates the navigation and control buttons of the menu.
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	pTextA - pointer to button A label.
//*		pTextB - pointer to button B label.
//*		pTextC - pointer to button C label.
//*		pTextB - pointer to button D label.
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

//void CreateCtrlButtons(XCHAR *pTextA, XCHAR *pTextB, XCHAR *pTextC, XCHAR *pTextD)
//{
//	WORD state;
//
//    state = BTN_DRAW;
//    if(pTextA == NULL)
//        state = BTN_DRAW|BTN_DISABLED;
//    BtnCreate(ID_BUTTON_A,
//              CtrlBtnLeft(0),
//              CtrlBtnTop(),
//              CtrlBtnRight(0),
//              CtrlBtnBottom(),
//              0,
//              state,
//              NULL,
//              pTextA,
//              altScheme );
//
//    state = BTN_DRAW;
//    if(pTextB == NULL)
//        state = BTN_DRAW|BTN_DISABLED;
//    BtnCreate(ID_BUTTON_B,
//              CtrlBtnLeft(1),
//              CtrlBtnTop(),
//              CtrlBtnRight(1),
//              CtrlBtnBottom(),
//              0,
//              state,
//              (void*)&LB_Up_Arrow,																	// was NULL
//              pTextB,
//              altScheme );
//
//    state = BTN_DRAW;
//    if(pTextC == NULL)
//        state = BTN_DRAW|BTN_DISABLED;
//    BtnCreate(ID_BUTTON_C,
//              CtrlBtnLeft(2),
//              CtrlBtnTop(),
//              CtrlBtnRight(2),
//              CtrlBtnBottom(),
//              0,
//              state,
//              (void*)&LB_Down_Arrow,																// was NULL
//              pTextC,
//              altScheme );
//
//    state = BTN_DRAW;
//    if(pTextD == NULL)
//        state = BTN_DRAW|BTN_DISABLED;
//    BtnCreate(ID_BUTTON_D,
//              CtrlBtnLeft(3),
//              CtrlBtnTop(),
//              CtrlBtnRight(3),
//              CtrlBtnBottom(),
//              0,
//              state,
//              NULL,
//              pTextD,
//              altScheme );         
//	return;
//}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	END OF DISPLAY ROUTINES
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//*	USB ROUTINES
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL USB_ApplicationEventHandler( BYTE address, USB_EVENT event, void *data, DWORD size )
//*
//*	Description:
//* 	Pulled from the USB configuration to allow more control over the event system.
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

BOOL USB_ApplicationEventHandler( BYTE address, USB_EVENT event, void *data, DWORD size )
{
	unsigned char index;
	unsigned char btnpress = 1;
	DWORD datacount;
	BYTE *UserData;

    switch(event)
    {
        case EVENT_GENERIC_ATTACH:
		{
            if (size == sizeof(GENERIC_DEVICE_ID))
            {
                USBDeviceAddress   = ((GENERIC_DEVICE_ID *)data)->deviceAddress;
                return TRUE;
            }
            break;
		}
        case EVENT_GENERIC_DETACH:
		{
            USBDeviceAddress   = 0;
            return TRUE;
		}
        case EVENT_GENERIC_TX_DONE:           														// The main state machine will poll the driver.
		{
			// With USB_ENABLE_TRANSFER_EVENT disabled, this event is never generated!
			Transmit_Data = FALSE;
            return TRUE;
		}
        case EVENT_GENERIC_RX_DONE:
		{
			// With USB_ENABLE_TRANSFER_EVENT disabled, this event is never generated!
			datacount = ((HOST_TRANSFER_DATA *)data)->dataCount;
            return TRUE;
		}
        case EVENT_VBUS_REQUEST_POWER:
		{
            // The data pointer points to a byte that represents the amount of power
            // requested in mA, divided by two.  If the device wants too much power,
            // we reject it.
            if (((USB_VBUS_POWER_EVENT_DATA*)data)->current <= (MAX_ALLOWED_CURRENT / 2))
            {
                return TRUE;
            }
            else
            {
               UARTPutString( "\r\n***** USB Error - device requires too much current *****\r\n" ); // was UART2PrintString
            }
            break;
		}
        case EVENT_VBUS_RELEASE_POWER:
		{			
            return TRUE;			// turn off usb power
		}
		case EVENT_HID_RPT_DESC_PARSED:
		{
			 #ifdef APPL_COLLECT_PARSED_DATA
			     return(APPL_COLLECT_PARSED_DATA());
		     #else
				 return TRUE;
			 #endif
			break;
		}
        case EVENT_HUB_ATTACH:
		{
			add_error_message_to_queue(USB_ERROR_HUB);
		    break;
		}
        case EVENT_UNSUPPORTED_DEVICE:
		{
			add_error_message_to_queue(USB_ERROR_NO_DEVICE_SUPPORT);
		    break;
		}
        case EVENT_CANNOT_ENUMERATE:
		{
			add_error_message_to_queue(USB_ERROR_ENUMERATION);
		    break;
		}
        case EVENT_CLIENT_INIT_ERROR:
		{
			add_error_message_to_queue(USB_ERROR_CLIENT_INIT);
		    break;
		}
        case EVENT_OUT_OF_MEMORY:
		{
			add_error_message_to_queue(USB_ERROR_NO_MEMORY);
        	break;
  		}      	
        case EVENT_UNSPECIFIED_ERROR:
		{
			add_error_message_to_queue(USB_ERROR_UNSPECIFIED);
		    break;
  		}      
	}

    return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Check_USB_Port(void)
//*
//*	Description:
//* 	This routine determines what type of device is connected to the USB port after a call to
//*		the USB Detect Device routine.
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

void Check_USB_Port(void)
{
	static BYTE lastUSBdevice = (BYTE) NO_DEVICE_ATTACHED;
    unsigned int int_status;

	USBTasks();
	USB_Port_Attachment = USB_Detect_Device();

	switch(USB_Port_Attachment)
	{
		case NO_DEVICE_ATTACHED:
		{
			if(lastUSBdevice == KEYBOARD)
			{
			    T4CONCLR = 0x8000;																					// Stop Timer
		    	App_State_Keyboard = DEVICE_NOT_CONNECTED;
       			DisplayDetachOnce = TRUE;																			// forces element value reset
#ifndef TEST_MODE																									// (Test v0.00.01)
				screenState = CREATE_ELEMENT;																		// restart screen state
#else																												// (Test v0.00.01)
				screenState = CREATE_INSTRUMENT_MENU;																// (Test v0.00.05)
#endif																												// (Test v0.00.01)
				if((Backlight_Mode == BK_LIGHT_TEMP_ON) || (Backlight_Mode == BK_LIGHT_OFF))
				{
					Set_Backlight_Level(0);
				}
#ifndef TEST_MODE
				EEPROM_Write_All();
#endif
			}
			else if(lastUSBdevice == FTDI)
			{
				if(lmi.features.data_logging.info.operating_mode == 1)
				{
					Initialize_Reporting(FALSE);																	// (9DP v1.01.09)
				}
				PC_State = PC_NOT_CONNECTED;
			}
			else if(lastUSBdevice == THUMBDRIVE)
			{
				if(lmi.features.data_logging.info.operating_mode == 1)
				{
					Initialize_Reporting(FALSE);																	// (9DP v1.01.09)
				}
				USB_File_State = FILE_NOT_OPEN;																		// moved outside the if statement (9DP v1.01.09)
			}
			break;
		}
		case KEYBOARD:
		{
			Check_Keyboard();
			break;
		}
		case FTDI:
		{
			Check_FTDI_Adapter();
			break;
		}
		case THUMBDRIVE:
		{
			if(lastUSBdevice != THUMBDRIVE)
			{
				USB_File_State = FILE_OPEN_READ_CFG;
				do
				{
					Check_Thumbdrive();
				} while(USB_File_State != FILE_NOT_OPEN);

				USB_File_State = FILE_OPEN_READ_DRW;
				do
				{
					Check_Thumbdrive();
				} while(USB_File_State != FILE_NOT_OPEN);

				USB_File_State = FILE_OPEN_READ_C;
				do
				{
					Check_Thumbdrive();
				} while(USB_File_State != FILE_NOT_OPEN);

				if(lmi.features.data_logging.info.operating_mode == 1)
				{
					Initialize_Reporting(TRUE);																		// (9DP v1.01.09)
				}
				else
				{
					Data_Logging_Enabled = FALSE;
				}
			}
			else if(Data_Logging_Enabled == TRUE)
			{
				USB_File_State = FILE_OPEN_WRITE;
				Check_Thumbdrive();
			}
			break;
		}
	}

	lastUSBdevice = USB_Port_Attachment;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BYTE USB_Detect_Device(void)
//*
//*	Description:
//* 	This function monitors the status of device connected/disconnected	
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

BYTE USB_Detect_Device(void)
{
	//* FTDI Serial Cable
	if(USBHostFTDI_DetectDevice())
	{
		if(App_State_FTDI == DEVICE_NOT_CONNECTED)
		{
			App_State_FTDI = DEVICE_CONNECTED;
		}
		return FTDI;
	}
	else
	{
		if(App_State_FTDI != DEVICE_NOT_CONNECTED)
		{
			App_State_FTDI = DEVICE_NOT_CONNECTED;
			return FTDI;
		}
	}

	//* Generic USB Keyboard
  	if(USBHostHID_ApiDeviceDetect())
  	{
		if(App_State_Keyboard == DEVICE_NOT_CONNECTED)
		{
    		App_State_Keyboard = DEVICE_CONNECTED;
		}
		return KEYBOARD;
	}
	else
	{
		if(App_State_Keyboard != DEVICE_NOT_CONNECTED)
		{
    		App_State_Keyboard = DEVICE_NOT_CONNECTED;
			return KEYBOARD;
		}
  	}

	//* Generic USB Thumbdrive
    if(USBHostMSDSCSIMediaDetect())
    {
		if(App_State_Thumbdrive == DEVICE_NOT_CONNECTED)
		{
			App_State_Thumbdrive = DEVICE_CONNECTED;
		}
		return THUMBDRIVE;
	}
	else
	{
		if(App_State_Thumbdrive == DEVICE_CONNECTED)
		{
	        App_State_Thumbdrive = DEVICE_NOT_CONNECTED;
			return THUMBDRIVE;
		}
	}

	return NO_DEVICE_ATTACHED;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL USBHostFTDI_DetectDevice(void)
//*
//*	Description:
//* 	This routine checks to see if a new FTDI device has been attached.  If it has, it records the
//*		address.
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	None
//*
//*	Return Values:
//* 	TRUE if a new device has been attached.
//*		FALSE otherwise.
//*
//*	Remarks:
//*		None
//*

BOOL USBHostFTDI_DetectDevice(void)
{
	GENERIC_DEVICE_ID DevID;

	DevID.vid   = 0x0403;
	DevID.pid   = 0x6001;
#ifdef USB_GENERIC_SUPPORT_SERIAL_NUMBERS
    DevID.serialNumberLength = 0;
    DevID.serialNumber = NULL;
#endif

    // Try to get the device address, if we don't have one.
    if(PC_State == PC_NOT_CONNECTED)
    {
        if(USBHostGenericGetDeviceAddress(&DevID))
        {
            USBDeviceAddress = DevID.deviceAddress;
            PC_State = PC_DISCONNECTED;
			return TRUE;
        }
    }
	else
	{
        if(!USBHostGenericGetDeviceAddress(&DevID))
        {
            USBDeviceAddress = 0;
			PC_State = PC_NOT_CONNECTED;
        }
		else
		{
			return TRUE;
		}
	}

	return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL USB_HID_DataCollectionHandler(void)	
//*
//*	Description:
//*		This function is invoked by HID client , purpose is to collect the details extracted from
//*		the report descriptor. HID client will store information extracted from the report descriptor
//*		in data structures. Application needs to create object for each report type it needs to 
//*    	extract.
//*
//*    	For ex: HID_DATA_DETAILS Appl_ModifierKeysDetails;
//*    	HID_DATA_DETAILS is defined in file usb_host_hid_appl_interface.h
//*    	Each member of the structure must be initialized inside this function.
//*    	Application interface layer provides functions :
//*    	USBHostHID_ApiFindBit()
//*    	USBHostHID_ApiFindValue()
//*    	These functions can be used to fill in the details as shown in the demo code.
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	None
//*
//*	Return Values:
//*   TRUE    - If the report details are collected successfully.
//*   FALSE   - If the application does not find the the supported format.
//*
//*	Remarks:
//* 	None
//*

BOOL USB_HID_DataCollectionHandler(void)
{
	BYTE NumOfReportItem = 0;
	BYTE i;
	USB_HID_ITEM_LIST* pitemListPtrs;
	USB_HID_DEVICE_RPT_INFO* pDeviceRptinfo;
	HID_REPORTITEM *reportItem;
	HID_USAGEITEM *hidUsageItem;
	BYTE usageIndex;
	BYTE reportIndex;
	BOOL foundLEDIndicator = FALSE;
	BOOL foundModifierKey = FALSE;
	BOOL foundNormalKey = FALSE;
	
	pDeviceRptinfo = USBHostHID_GetCurrentReportInfo(); 												// Get current Report Info pointer
	pitemListPtrs = USBHostHID_GetItemListPointers();   												// Get pointer to list of item pointers
	
	BOOL status = FALSE;
	/* Find Report Item Index for Modifier Keys */
	/* Once report Item is located , extract information from data structures provided by the parser */
	NumOfReportItem = pDeviceRptinfo->reportItems;
	for(i = 0; i < NumOfReportItem; i++)
    {
       reportItem = &pitemListPtrs->reportItemList[i];
       if((reportItem->reportType==hidReportInput) && (reportItem->dataModes == HIDData_Variable)&&
           (reportItem->globals.usagePage==USAGE_PAGE_KEY_CODES))
        {
           /* We now know report item points to modifier keys */
           /* Now make sure usage Min & Max are as per application */
            usageIndex = reportItem->firstUsageItem;
            hidUsageItem = &pitemListPtrs->usageItemList[usageIndex];
            if((hidUsageItem->usageMinimum == USAGE_MIN_MODIFIER_KEY)
                &&(hidUsageItem->usageMaximum == USAGE_MAX_MODIFIER_KEY)) 							//else application cannot suuport
            {
               reportIndex = reportItem->globals.reportIndex;
               Appl_ModifierKeysDetails.reportLength = (pitemListPtrs->reportList[reportIndex].inputBits + 7)/8;
               Appl_ModifierKeysDetails.reportID = (BYTE)reportItem->globals.reportID;
               Appl_ModifierKeysDetails.bitOffset = (BYTE)reportItem->startBit;
               Appl_ModifierKeysDetails.bitLength = (BYTE)reportItem->globals.reportsize;
               Appl_ModifierKeysDetails.count=(BYTE)reportItem->globals.reportCount;
               Appl_ModifierKeysDetails.interfaceNum= USBHostHID_ApiGetCurrentInterfaceNum();
               foundModifierKey = TRUE;
            }
        }
        else if((reportItem->reportType==hidReportInput) && (reportItem->dataModes == HIDData_Array)&&
           (reportItem->globals.usagePage==USAGE_PAGE_KEY_CODES))
        {
           /* We now know report item points to modifier keys */
           /* Now make sure usage Min & Max are as per application */
            usageIndex = reportItem->firstUsageItem;
            hidUsageItem = &pitemListPtrs->usageItemList[usageIndex];
            if((hidUsageItem->usageMinimum == USAGE_MIN_NORMAL_KEY)
                &&(hidUsageItem->usageMaximum <= USAGE_MAX_NORMAL_KEY)) 							//else application cannot suuport
            {
               reportIndex = reportItem->globals.reportIndex;
               Appl_NormalKeysDetails.reportLength = (pitemListPtrs->reportList[reportIndex].inputBits + 7)/8;
               Appl_NormalKeysDetails.reportID = (BYTE)reportItem->globals.reportID;
               Appl_NormalKeysDetails.bitOffset = (BYTE)reportItem->startBit;
               Appl_NormalKeysDetails.bitLength = (BYTE)reportItem->globals.reportsize;
               Appl_NormalKeysDetails.count=(BYTE)reportItem->globals.reportCount;
               Appl_NormalKeysDetails.interfaceNum= USBHostHID_ApiGetCurrentInterfaceNum();
               foundNormalKey = TRUE;
            }
        }
        else if((reportItem->reportType==hidReportOutput) &&
                (reportItem->globals.usagePage==USAGE_PAGE_LEDS))
        {
            usageIndex = reportItem->firstUsageItem;
            hidUsageItem = &pitemListPtrs->usageItemList[usageIndex];

            reportIndex = reportItem->globals.reportIndex;
            Appl_LED_Indicator.reportLength = (pitemListPtrs->reportList[reportIndex].outputBits + 7)/8;
            Appl_LED_Indicator.reportID = (BYTE)reportItem->globals.reportID;
            Appl_LED_Indicator.bitOffset = (BYTE)reportItem->startBit;
            Appl_LED_Indicator.bitLength = (BYTE)reportItem->globals.reportsize;
            Appl_LED_Indicator.count=(BYTE)reportItem->globals.reportCount;
            Appl_LED_Indicator.interfaceNum= USBHostHID_ApiGetCurrentInterfaceNum();
            foundLEDIndicator = TRUE;
        }
    }

	if(pDeviceRptinfo->reports == 1)
    {
        Appl_raw_report_buffer.Report_ID = 0;
        Appl_raw_report_buffer.ReportSize = (pitemListPtrs->reportList[reportIndex].inputBits + 7)/8;
        Appl_raw_report_buffer.ReportData = (BYTE*)malloc(Appl_raw_report_buffer.ReportSize);
        Appl_raw_report_buffer.ReportPollRate = pDeviceRptinfo->reportPollingRate;
        if((foundNormalKey == TRUE)&&(foundModifierKey == TRUE))
        status = TRUE;
    }

    return(status);
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	END USB ROUTINES
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//*	USB FTDI ROUTINES
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Check_FTDI_Adapter(void)
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

void Check_FTDI_Adapter(void)
{
	static DWORD counter = 0;

	switch(App_State_FTDI)
	{
		case DEVICE_NOT_CONNECTED:
		{
			Data_Logging_Enabled = FALSE;
			break;
		}
		case DEVICE_CONNECTED:
		{
			if(Transmit_Data)
			{
				FTDI_Transmit();
			}
			else if((lmi.calibration.controls.info.stream_data_reading == 1) && (Update_Data_Stream == TRUE))
			{
				Update_Data_Stream = FALSE;
				Configure_Data_Stream((BYTE *)&USB_TXMessage);
				Transmit_Data = TRUE;
			}
			else if((lmi.features.data_logging.info.operating_mode == 1) && (Data_Logging_Enabled == FALSE))
			{
				Initialize_Reporting(TRUE);																												// (9DP v1.01.09)
			}
			else if((Data_Logging_Enabled == TRUE) && (Data_Logging_Header_Sent == FALSE) && (Update_Log_File == TRUE))
			{
				Data_Logging_Header_Sent = TRUE;
				Configure_Header((BYTE *)&USB_TXMessage);
				Transmit_Data = TRUE;
				counter = 1;
			}
			else if((Data_Logging_Enabled == TRUE) && (Data_Logging_Header_Sent == TRUE) && (Power_Up_Report_Integrate == TRUE))
			{
				Power_Up_Report_Integrate = FALSE;
				sprintf((char *)USB_TXMessage,"%d\0",counter++);
				Configure_Message((BYTE *)&USB_TXMessage, (BYTE) DOSE_EVENT);
				Transmit_Data = TRUE;
			}
			else if((Data_Logging_Enabled == TRUE) && (Data_Logging_Header_Sent == TRUE) && (Power_Down_Report_Integrate == TRUE) && (System_Shutdown == TRUE))
			{
				Power_Down_Report_Integrate = FALSE;
				sprintf((char *)USB_TXMessage,"%d\0",counter++);
				Configure_Message((BYTE *)&USB_TXMessage, (BYTE) DOSE_EVENT);
				Transmit_Data = TRUE;
			}
			else if((Data_Logging_Enabled == TRUE) && (Data_Logging_Header_Sent == TRUE) && (Update_Log_File == TRUE) && (System_Shutdown == FALSE))	// added system_shutdown check (9DP v1.01.08)
			{
				Update_Log_File = FALSE;
				sprintf((char *)USB_TXMessage,"%d\0",counter++);
				Configure_Message((BYTE *)&USB_TXMessage, (BYTE) LOG_EVENT);
				Transmit_Data = TRUE;
			}
			else
			{
				FTDI_Receive();
			}
			break;
		}
	}

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void FTDI_Receive(void)
//*
//*	Description:
//* 	This routine polls the USB port to see if data is available.
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
//*		None
//*

void FTDI_Receive(void)
{
	BYTE retValue = 0;
    BYTE index = 0;
	BYTE modemStatus = 0;
	BYTE lineStatus = 0;
	BYTE bufferfull = 0;																			// (9DP v1.01.15)

    if(USBDeviceAddress != 0) 
    {
		if(App_State_FTDI == DEVICE_CONNECTED)
		{
		    if(!USBHostGenericRxIsBusy(USBDeviceAddress))
			{
				retValue = USBHostGenericRead(USBDeviceAddress, &USBData, sizeof(USBData));
				switch(retValue)
				{
					case USB_SUCCESS:
					{
						index = 0;
						modemStatus = USBData[index++];
						lineStatus = USBData[index++];
						if((lineStatus & 0x9F) == 0x00)
						{
							bufferfull = 2;															// (9DP v1.01.15)
							for(; USBData[index] != 0x00; index++)
							{
								USB_RXMessage[USB_buffer_index++] = USBData[index];
								bufferfull++;														// (9DP v1.01.15)
								if(USBData[index] == 0x0A)
								{
									USB_RXMessage[USB_buffer_index] = 0x00;
									Message_Handler();
									USB_buffer_index = 0x00;
								}
								if(bufferfull == 64)												// (9DP v1.01.15)
								{																	// (9DP v1.01.15)
									modemStatus = USBData[index++];									// (9DP v1.01.15)
									lineStatus = USBData[index++];									// (9DP v1.01.15)
									bufferfull = 2;													// (9DP v1.01.15)
								}																	// (9DP v1.01.15)
							}
						}
						for(index = 0; index < sizeof(USBData) / sizeof(BYTE); index++) 
						{
							USBData[index] = 0x00;
						}	
						break;
					}
					case USB_INVALID_STATE:
					case USB_BUSY:
					{
						break;
					}
					default:
					{
						break;
					}
				}
			}
		}
    }    
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void FTDI_Transmit(void)
//*
//*	Description:
//* 	This routine checks to see if data is ready to be transmitted to the USB port.
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
//*		None
//*

void FTDI_Transmit(void)
{
	DWORD byteCount;
	BYTE errorCode;
	BYTE returnVal;

    // Watch for device detaching
    if(USBHostGenericDeviceDetached(USBDeviceAddress) && USBDeviceAddress != 0)
    {
        USBDeviceAddress = 0;
    }

    if(USBDeviceAddress != 0) 
    {
		if(App_State_FTDI == DEVICE_CONNECTED)
		{
		    if(!USBHostGenericRxIsBusy(USBDeviceAddress))
			{
				byteCount = strlen(USB_TXMessage) - TXMessageIndex;
				returnVal = USBHostGenericWrite(USBDeviceAddress, &USB_TXMessage[TXMessageIndex], byteCount);
				TXMessageIndex = 0;
				Transmit_Data = FALSE;
			}
		}
    }    
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	END OF USB FTDI ROUTINES
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//*	USB KEYBOARD ROUTINES
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Check_Keyboard(void)
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

void Check_Keyboard(void)
{
	static int failCount = 0;
    BYTE index;

	USBTasks();
	switch(App_State_Keyboard)
	{
		case DEVICE_NOT_CONNECTED:
		{
			break;
		}
		case DEVICE_CONNECTED:
		{
			TimerInit();
			App_State_Keyboard = READY_TO_TX_RX_REPORT;
  			break;
		}
		case READY_TO_TX_RX_REPORT:
		{
			/* dummy state waiting on keyboard condition change */
			break;
		}
		case GET_INPUT_REPORT:
		{
   			if(USBHostHID_ApiGetReport(Appl_raw_report_buffer.Report_ID,Appl_ModifierKeysDetails.interfaceNum,Appl_raw_report_buffer.ReportSize, Appl_raw_report_buffer.ReportData))
   			{
   				/* Host may be busy/error -- keep trying */
   				failCount++;
   				if(failCount >= 16)
   				{
   					App_State_Keyboard = ERROR_REPORTED;
   					failCount = 0;
   				}
   			}
   			else
   			{
       			App_State_Keyboard = INPUT_REPORT_PENDING;
       			failCount = 0;
   			}
			break;
		}		
		case INPUT_REPORT_PENDING:
		{	
			if(USBHostHID_ApiTransferIsComplete(&ErrorDriver,&NumOfBytesRcvd))
   			{
       			if(ErrorDriver || (NumOfBytesRcvd != Appl_raw_report_buffer.ReportSize))
       			{
       				ErrorCounter++ ; 
       				if(MAX_ERROR_COUNTER <= ErrorDriver)
           				App_State_Keyboard = ERROR_REPORTED;
       				else
           				App_State_Keyboard = READY_TO_TX_RX_REPORT;
       			}
       			else
       			{
       				ErrorCounter = 0; 
       				ReportBufferUpdated = TRUE;
       				App_State_Keyboard = READY_TO_TX_RX_REPORT;
		          	USB_Keyboard_ProcessInputReport();
       				USB_Keyboard_PrepareOutputReport();
       			}
		    }
			break;
		}	
		case SEND_OUTPUT_REPORT:
		{
   			if(USBHostHID_ApiSendReport(Appl_LED_Indicator.reportID,Appl_LED_Indicator.interfaceNum, Appl_LED_Indicator.reportLength,(BYTE*)&Appl_led_report_buffer))
   			{
   				/* Host may be busy/error -- keep trying */
   			}
   			else
   			{
       			App_State_Keyboard = OUTPUT_REPORT_PENDING;
   			}
     		break;
		}
		case OUTPUT_REPORT_PENDING:
		{
   			if(USBHostHID_ApiTransferIsComplete(&ErrorDriver,&NumOfBytesRcvd))
   			{
       			if(ErrorDriver)
       			{
       				ErrorCounter++ ; 
       				if(MAX_ERROR_COUNTER <= ErrorDriver)
           				App_State_Keyboard = ERROR_REPORTED;
				}
       			else
       			{
       				ErrorCounter = 0; 
       				App_State_Keyboard = READY_TO_TX_RX_REPORT;
       			}
   			}
			break;
		}
		case DEVICE_SUSPENDED:
		{
			break;
		}
		case ERROR_REPORTED:
		{
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
//* 	void USB_Keyboard_Routine(BYTE data , BYTE HIDData, BOOL state)	
//*
//*	Description:
//* 	
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	data - ASCII code for the key pressed
//*	    HIDData - HID code for the key pressed, this is needed to take action for keys like Esc, 	
//*			Enter, Tab etc.
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

void USB_Keyboard_Routine(BYTE data , BYTE HIDData, BOOL state)
{
	static UINT16 HeldKeyCounter = 0;
	char stringconversion[29];
	SHORT string_length;

	if(state == FALSE)
	{
		HeldKeyCounter = 0;
	}
	else
	{
		HeldKeyCounter++;
		if(HeldKeyCounter == 0)																		// if overflow occurred...
			HeldKeyCounter = 20;																	// ...set to increased value
	}

	if(screenState >= CREATE_MENU)
	{
	    if(HIDData == Symbol_Escape)
	    {
#ifndef TEST_MODE
			screenState = CREATE_ELEMENT;
			DisplayDetachOnce = TRUE;
			if((Backlight_Mode == BK_LIGHT_TEMP_ON) || (Backlight_Mode == BK_LIGHT_OFF))
			{
				Set_Backlight_Level(0);
			}
			EEPROM_Write_All();																		// Save all data to the EEPROM
#else
			screenState = CREATE_SHUTDOWN;															// (Test v0.00.01)
#endif
	    }
		else if(HIDData == Symbol_Return)
		{
			if((editScreen == FALSE) || (MenuLine != 1))
			{
				if(pmsgScreen.sub_menu[MenuLine] != 0)
					screenState = pmsgScreen.sub_menu[MenuLine];
				else if(screenState != DISPLAY_PASSWORD_ENTRY)										// (Test v0.00.07)
					MenuLine++;																		// (Test v0.00.07)
			}
			else
			{
				editMode = !editMode;
			}
		}
	    else if(((HIDData >= Symbol_PageUp) && (HIDData <= Symbol_UpArrow)) || (((HIDData >= Symbol_Keypad_DownArrow) && (HIDData <= Symbol_Keypad_PageUp) && (NUM_Lock_Pressed == 0))))
	    {
	        switch(HIDData)
	        {
				case Symbol_PageUp:
				{
					screenState = pmsgScreen.parent_menu;
					break;
				}
	            case Symbol_RightArrow:
	            case Symbol_Keypad_RightArrow:
				{
	                break;
				}
	            case Symbol_LeftArrow:
	            case Symbol_Keypad_LeftArrow:
				{
	                break;
				}
	            case Symbol_DownArrow:
	            case Symbol_Keypad_DownArrow:
				{
					if(MenuSelect == TRUE)															// (Test v0.00.01)
					{																				// (Test v0.00.01)
						if(editMode == FALSE)
						{
							MenuLine++;																			
							if(MenuLine > pmsgScreen.txt_total_lines)
								MenuLine = 1;
						}
						else
						{
							if(HeldKeyCounter > 19)
								SubMenuLine -= 50;
							else if(HeldKeyCounter > 9)
								SubMenuLine -= 5;
							else
								SubMenuLine--;
							if(SubMenuLine < pmsgScreen.minimum_value)
								SubMenuLine = pmsgScreen.maximum_value;
						}
					}																				// (Test v0.00.01)
	                break;
				}
	            case Symbol_UpArrow:
	            case Symbol_Keypad_UpArrow:
				{
					if(MenuSelect == TRUE)															// (Test v0.00.01)
					{																				// (Test v0.00.01)
						if(editMode == FALSE)
						{
							MenuLine--;
							if(MenuLine < 1)
								MenuLine = pmsgScreen.txt_total_lines;
						}
						else
						{
							if(HeldKeyCounter > 19)
								SubMenuLine += 50;
							else if(HeldKeyCounter > 9)
								SubMenuLine += 5;
							else
								SubMenuLine++;
							if(SubMenuLine > pmsgScreen.maximum_value)
								SubMenuLine = pmsgScreen.minimum_value;
						}
					}																				// (Test v0.00.01)
	                break;
				}
	            default :
				{
	                break;
				}
			}
	    }
	    else if(HIDData == Symbol_Space)
	    {
			if((editMode == TRUE) && ((screenState == DISPLAY_EDIT_STRING_LINE) || (screenState == DISPLAY_PASSWORD_ENTRY)) && (Total_Chars < pmsgScreen.maximum_value))
			{
				strncpy(stringconversion, (char *)pmsgScreen.line_item[MenuLine], Total_Chars);		//*	(9DP v1.01.04)
				stringconversion[Total_Chars++] = ' ';												//*	(9DP v1.01.04)
				strncpy((char *)pmsgScreen.line_item[MenuLine], stringconversion, Total_Chars);		//*	(9DP v1.01.04)
			}
	    }
	    else if(HIDData == Symbol_Backspace)
	    {
			if((editMode == TRUE) && ((screenState == DISPLAY_EDIT_STRING_LINE) || (screenState == DISPLAY_PASSWORD_ENTRY)) && (Total_Chars > pmsgScreen.minimum_value))
			{
				Total_Chars--;
				strncpy(stringconversion, (char *)pmsgScreen.line_item[MenuLine], Total_Chars);		//*	(9DP v1.01.04)
				stringconversion[Total_Chars] = 0x00;
				strcpy((unsigned char *)pmsgScreen.line_item[MenuLine],stringconversion);
			}
	    }
	   	else if((HIDData>=0x04 && HIDData<=0x1D) ||  (HIDData>=0x1E && HIDData<=0x27) ||  (HIDData>=0x2D && HIDData<=0x38) ||  ((HIDData>=0x59 && HIDData<=0x62) && (NUM_Lock_Pressed == 1)))
	    {
			if((editMode == TRUE) && ((screenState == DISPLAY_EDIT_STRING_LINE) || (screenState == DISPLAY_PASSWORD_ENTRY)) && (Total_Chars < pmsgScreen.maximum_value))
			{
				strncpy(stringconversion, (char *)pmsgScreen.line_item[MenuLine], Total_Chars);		//*	(9DP v1.01.04)
				stringconversion[Total_Chars++] = data;												//*	(9DP v1.01.04)
				strncpy((char *)pmsgScreen.line_item[MenuLine], stringconversion, Total_Chars);		//*	(9DP v1.01.04)
			}
	    }
	}
	else
	{
		if((HIDData >= Symbol_F1) && (HIDData <= Symbol_F12))
		{
			switch(HIDData)
			{
				case Symbol_F1:
				{
					if(lmi.settings.passwords.info.enable == TRUE)
					{
						screenState = CREATE_PASSWORD_ENTRY;
					}
					else
					{
						screenState = CREATE_MENU;
					}
					break;
				}
				default:
				{
					break;
				}
			}
		}
	}

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void USB_Keyboard_ProcessInputReport(void)
//*
//*	Description:
//* 	This function processes input report received from HID device
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

void USB_Keyboard_ProcessInputReport(void)
{
	static BYTE HeldKeyTimer = 0;
	static BYTE HeldKeyValue = 128;
    BYTE  i;
    BYTE  data;
	
   /* process input report received from device */
    USBHostHID_ApiImportData(Appl_raw_report_buffer.ReportData, Appl_raw_report_buffer.ReportSize
                          ,Appl_BufferModifierKeys, &Appl_ModifierKeysDetails);
    USBHostHID_ApiImportData(Appl_raw_report_buffer.ReportData, Appl_raw_report_buffer.ReportSize
                          ,Appl_BufferNormalKeys, &Appl_NormalKeysDetails);

    for(i = 0; i < (sizeof(Appl_BufferNormalKeys)/sizeof(Appl_BufferNormalKeys[0])); i++)
    {
        if(Appl_BufferNormalKeys[i] != 0)
        {
            if(Appl_BufferNormalKeys[i] == HID_CAPS_LOCK_VAL)
            {
               CAPS_Lock_Pressed = !CAPS_Lock_Pressed;
               LED_Key_Pressed = TRUE;
               Appl_led_report_buffer.CAPS_LOCK = CAPS_Lock_Pressed;
            }
			else if(Appl_BufferNormalKeys[i] == HID_NUM_LOCK_VAL)
            {
                NUM_Lock_Pressed = !NUM_Lock_Pressed;
                LED_Key_Pressed = TRUE;
                Appl_led_report_buffer.NUM_LOCK = NUM_Lock_Pressed;
            }
			else
            {
               	if(!USB_Keyboard_CompareKeyPressedPrevBuf(Appl_BufferNormalKeys[i]))
                {
                    data = USB_Keyboard_HID2ASCII(Appl_BufferNormalKeys[i]);  								// convert data to ascii
                    USB_Keyboard_Routine(data,Appl_BufferNormalKeys[i], FALSE);
					HeldKeyTimer = 0;																		// reset timer
					HeldKeyValue = 128;																		// initialize timer compare value
                }
				else
				{
					HeldKeyTimer++;
					if(HeldKeyTimer == HeldKeyValue)
					{
	                    data = USB_Keyboard_HID2ASCII(Appl_BufferNormalKeys[i]);							// convert data to ascii
    	                USB_Keyboard_Routine(data,Appl_BufferNormalKeys[i], TRUE);
						HeldKeyTimer = 0;
					}
				}
            }
        }
        else
        {
			if(i==0)
			{
				HeldKeyCount = 0;
			}
			else
			{
				if(Appl_BufferNormalKeys[i-1] == HeldKey)
				{
					if(HeldKeyCount < 3)
					{
						HeldKeyCount++;
					}
					else
					{
						data = USB_Keyboard_HID2ASCII(HeldKey);  											// convert data to ascii
						USB_Keyboard_Routine(data, HeldKey, TRUE);
					}
				}
				else
				{
					HeldKeyCount = 0;
					HeldKey = Appl_BufferNormalKeys[i-1];
				}
			}
			break;
		}
	}
    USB_Keyboard_CopyToShadowBuffer();
    USB_Keyboard_ClearDataBuffer();
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void USB_Keyboard_PrepareOutputReport(void)
//*
//*	Description:
//* 	This function schedules output report if any LED indicator key is pressed.
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

void USB_Keyboard_PrepareOutputReport(void)
{
//    if((READY_TO_TX_RX_REPORT == App_State_Keyboard) && (ReportBufferUpdated == TRUE))
	if(ReportBufferUpdated == TRUE)
    {
        ReportBufferUpdated = FALSE;
        if(LED_Key_Pressed)
        {
            App_State_Keyboard = SEND_OUTPUT_REPORT;
            LED_Key_Pressed = FALSE;
        }
    }
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void USB_Keyboard_CopyToShadowBuffer(void)	
//*
//*	Description:
//* 	This function updates the shadow buffers with previous reports.	
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

void USB_Keyboard_CopyToShadowBuffer(void)
{
    BYTE i;

    for(i=0;i<(sizeof(Appl_BufferNormalKeys)/sizeof(Appl_BufferNormalKeys[0]));i++)
    {
        Appl_ShadowBuffer1[i] = Appl_BufferNormalKeys[i];
    }
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL USB_Keyboard_CompareKeyPressedPrevBuf(BYTE data)
//*
//*	Description:
//* 	This function compares if the data byte received in report was sent in previous report. 
//*		This is to avoid duplication incase user key in strokes at fast rate.
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	data - data byte that needs to be compared with previous report
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

BOOL USB_Keyboard_CompareKeyPressedPrevBuf(BYTE data)
{
    BYTE i;

    for(i=0;i<(sizeof(Appl_BufferNormalKeys)/sizeof(Appl_BufferNormalKeys[0]));i++)
    {
        if(data == Appl_ShadowBuffer1[i])
        {
            return TRUE;
        }
    }
    return FALSE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void USB_Keyboard_ClearDataBuffer(void)	
//*
//*	Description:
//* 	This function clears the content of report buffer after reading	
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

void USB_Keyboard_ClearDataBuffer(void)
{
    BYTE i;

    for(i=0;i<(sizeof(Appl_BufferNormalKeys)/sizeof(Appl_BufferNormalKeys[0]));i++)
    {
        Appl_BufferNormalKeys[i] = 0;
    }

    for(i=0;i<Appl_raw_report_buffer.ReportSize;i++)
    {
        Appl_raw_report_buffer.ReportData[i] = 0;
    }
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BYTE USB_Keyboard_HID2ASCII(BYTE a)	
//*
//*	Description:
//* 	This function converts the HID code of the key pressed to corresponding ASCII value. For
//*		Key strokes like Esc, Enter, Tab etc it returns 0.	
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	a - HID code for the key pressed
//*
//*	Return Values:
//* 	ASCII code for the key pressed
//*
//*	Remarks:
//* 	None
//*

BYTE USB_Keyboard_HID2ASCII(BYTE a)
{
	BYTE AsciiVal;
	BYTE ShiftkeyStatus = 0;
	if((Appl_BufferModifierKeys[MODIFIER_LEFT_SHIFT] == 1)||(Appl_BufferModifierKeys[MODIFIER_RIGHT_SHIFT] == 1))
	{
		ShiftkeyStatus = 1;
	}

	if(a>=0x1E && a<=0x27) 
	{
		if(ShiftkeyStatus)
		{
			switch(a)
			{
				case Symbol_Exclamation:
					AsciiVal = 0x21;           
					break;                                          
				case Symbol_AT:
					AsciiVal = 0x40;            
					break;                                          
				case Symbol_Pound:
					AsciiVal = 0x23;         
					break;                                  
				case Symbol_Dollar:
					AsciiVal = 0x24;        
					break;                                  
				case Symbol_Percentage:
					AsciiVal = 0x25;    
					break;                                  
				case Symbol_Cap:
					AsciiVal = 0x5E;   
					break;                                  
				case Symbol_AND:
					AsciiVal = 0x26;           
					break;                                  
				case Symbol_Star:
					AsciiVal = 0x2A;          
					break;                                  
				case Symbol_NormalBracketOpen:
					AsciiVal = 0x28;     
					break;                                          
				case Symbol_NormalBracketClose:
					AsciiVal = 0x29;    
					break;
				default:
					break;
			}	

			return(AsciiVal);
		}
		else
		{
			if(a==0x27)
			{
				return(0x30);
			}
			else
			{
				return(a+0x13);
			}
		} 
	}

	if((a>=0x59 && a<=0x61)&&(NUM_Lock_Pressed == 1))
	{
		return(a-0x28);
	}

	if((a==0x62) &&(NUM_Lock_Pressed == 1))
	{
		return(0x30);
	}

	if(a>=0x04 && a<=0x1D)
	{
		if(((CAPS_Lock_Pressed == 1)&&((Appl_BufferModifierKeys[MODIFIER_LEFT_SHIFT] == 0)&&
			(Appl_BufferModifierKeys[MODIFIER_RIGHT_SHIFT] == 0)))||
			((CAPS_Lock_Pressed == 0)&&((Appl_BufferModifierKeys[MODIFIER_LEFT_SHIFT] == 1) ||
        	(Appl_BufferModifierKeys[MODIFIER_RIGHT_SHIFT] == 1))))
		{
			return(a+0x3d); 																		// return capital
		}
		else
		{
			return(a+0x5d); 																		// return small case
		}
	}

	if(a>=0x2D && a<=0x38)
	{
		switch(a)
		{
			case Symbol_HyphenUnderscore:
			{
				if(!ShiftkeyStatus)
					AsciiVal = 0x2D;
				else
					AsciiVal = 0x5F;
				break;                      
			}
			case Symbol_EqualAdd:
			{
				if(!ShiftkeyStatus)
					AsciiVal = 0x3D;
				else
					AsciiVal = 0x2B;
				break;                                      
			}
			case Symbol_BracketOpen:
			{
				if(!ShiftkeyStatus)
					AsciiVal = 0x5B;
				else
					AsciiVal = 0x7B;        
				break;                                      
			}
			case Symbol_BracketClose:
			{
				if(!ShiftkeyStatus)
					AsciiVal = 0x5D;
				else
					AsciiVal = 0x7D;        
				break;                                      
			}
			case Symbol_BackslashOR:
			{
				if(!ShiftkeyStatus)
					AsciiVal = 0x5C;
				else
					AsciiVal = 0x7C;        
				break;                                      
			}
			case Symbol_SemiColon:
			{
				if(!ShiftkeyStatus)
					AsciiVal = 0x3B;
				else
					AsciiVal = 0x3A;        
				break;                                      
			}
			case Symbol_InvertedComma:
			{
				if(!ShiftkeyStatus)
					AsciiVal = 0x27;
				else
					AsciiVal = 0x22;
				break;                                      
			}
			case Symbol_Tilde:
			{
				if(!ShiftkeyStatus)
					AsciiVal = 0x60;
				else
					AsciiVal = 0x7E;            
				break;                                      
			}
			case Symbol_CommaLessThan:
			{
				if(!ShiftkeyStatus)
					AsciiVal = 0x2C;
				else
					AsciiVal = 0x3C;        
				break;                                      
			}
			case Symbol_PeriodGreaterThan:
			{
				if(!ShiftkeyStatus)
					AsciiVal = 0x2E;
				else
					AsciiVal = 0x3E;    
				break;                                      
			}
			case Symbol_FrontSlashQuestion:
			{
				if(!ShiftkeyStatus)
					AsciiVal = 0x2F;
				else
					AsciiVal = 0x3F;
				break;                                      
			}
			default:
			{
				break;
			}
		}
		return(AsciiVal);
	}   

	return(0);
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	END OF USB KEYBOARD ROUTINES
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//*	USB THUMBDRIVE ROUTINES
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Check_Thumbdrive(void)
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
//*	typedef struct {
//*		char filename[FILE_NAME_SIZE + 2];
//* 	unsigned char attributes;
//* 	unsigned long filesize;
//* 	unsigned long timestamp;
//* 	unsigned int entry;
//* 	char searchname[FILE_NAME_SIZE + 2];
//* 	unsigned char searchattr;
//* 	unsigned long cwdclus;
//* 	unsigned char initialized;
//*	} SearchRec; 
//*

void Check_Thumbdrive(void)
{
	static DWORD line = 0;
	static DWORD line_index = 0;
	static DWORD c_file_index = 0;
	static SearchRec instrumentFileRecord;
	static UINT32 counter = 0;
	static UINT32 file_index = 0;
	static BYTE read_file_type = 0;

	DWORD byteCount;
	UINT32 byte_counter;
	UINT16 length;
	UINT16 line_length;
	char ReturnLinefeed[] = "\r\n\0";
	char char_buffer[2];

	USBTasks();
	switch(USB_File_State)
	{
		case FILE_NOT_OPEN:
		{
			USB_File_State = FILE_OPEN_WRITE;
			break;
		}
		case FILE_OPEN_DIRECTORY:
		{
			USB_File_State = FILE_VIEW_DIRECTORY;
			break;
		}
		case FILE_VIEW_DIRECTORY:
		{
			USB_File_State = FILE_NOT_OPEN;
			break;
		}
		case FILE_OPEN_READ_CFG:
		{
			if(File_System_Initialize())
			{
				if(FindFirst("*.cfg", (unsigned int) ATTR_MASK, &instrumentFileRecord) == 0)
				{
					if((InstrumentFile = FSfopen(instrumentFileRecord.filename,"r")) != 0)
					{
						read_file_type = 1;
						USB_File_State = FILE_READ_BYTE;
						file_index = 0;
						return;
					}
					else
					{
						add_error_message_to_queue(ERROR_FILE_OPEN);
					}
				}
			}
			else
			{
				add_error_message_to_queue(ERROR_FILE_SYSTEM);
			}
			USB_File_State = FILE_NOT_OPEN;
			break;
		}
		case FILE_OPEN_READ_DRW:
		{
			if(File_System_Initialize())
			{
				if(FindFirst("*.drw", (unsigned int) ATTR_MASK, &instrumentFileRecord) == 0)
				{
					if((InstrumentFile = FSfopen(instrumentFileRecord.filename,"r")) != 0)
					{
						MemoryAddress = 0x000000;																					// clear memory address
						imageState = MEMORY_ADDRESS;																				// set image state
						read_file_type = 2;
						USB_File_State = FILE_READ_BYTE;
						file_index = 0;
						return;
					}
					else
					{
						add_error_message_to_queue(ERROR_FILE_OPEN);
					}
				}
			}
			else
			{
				add_error_message_to_queue(ERROR_FILE_SYSTEM);
			}
			USB_File_State = FILE_NOT_OPEN;
			break;
		}
		case FILE_OPEN_READ_C:
		{
			if(File_System_Initialize())
			{
				if(FindFirst("*.c", (unsigned int) ATTR_MASK, &instrumentFileRecord) == 0)
				{
					if((InstrumentFile = FSfopen(instrumentFileRecord.filename,"r")) != 0)
					{
						read_file_type = 3;
						USB_File_State = FILE_READ_BYTE;
						line = 1;
						return;
					}
					else
					{
						add_error_message_to_queue(ERROR_FILE_OPEN);
					}
				}
			}
			else
			{
				add_error_message_to_queue(ERROR_FILE_SYSTEM);
			}
			USB_File_State = FILE_NOT_OPEN;
			break;
		}
		case FILE_READ_BYTE:
		{
			for(byte_counter = 0; file_index < instrumentFileRecord.filesize; file_index++, byte_counter++)
			{
				if(FSfread(char_buffer, 1, 1, InstrumentFile))
				{
					File_RXMessage[byte_counter] = char_buffer[0];
					if(File_RXMessage[byte_counter] == 0x0A)
					{
						File_RXMessage[byte_counter+1] = 0x00;
						USB_File_State = FILE_READ_PENDING;
						return;
					}
				}
			}
			SetClockVars (	(unsigned int) (lmi.settings.calendar.info.date.year + 2000), \
							(unsigned char) lmi.settings.calendar.info.date.month, \
							(unsigned char) lmi.settings.calendar.info.date.day, \
							(unsigned char) lmi.settings.clock.info.time.hours, \
							(unsigned char) lmi.settings.clock.info.time.minutes, \
							(unsigned char) lmi.settings.clock.info.time.seconds);
            FSfclose(InstrumentFile);
			USB_File_State = FILE_NOT_OPEN;
			break;
		}		
		case FILE_READ_PENDING:
		{
			if(read_file_type == 1)
			{
				strcpy((char *)USB_RXMessage, (char *)File_RXMessage);
				Message_Handler();
//				if(Save_Config_To_Thumbdrive)																						// (9DP v1.01.18)
//				{																													// (9DP v1.01.18)
//					USB_File_State = FILE_OPEN_WRITE_TXT;																			// (9DP v1.01.17)
//					return;																											// (9DP v1.01.17)
//				}																													// (9DP v1.01.18)
				switch(Save_Config_To_Thumbdrive)																					// (9DP v1.02.08)
				{																													// (9DP v1.02.08)
					case 1:																											// (9DP v1.02.08)
					{																												// (9DP v1.02.08)
						USB_File_State = FILE_OPEN_WRITE_TXT;																		// (9DP v1.02.08)
						return;																										// (9DP v1.02.08)
					}																												// (9DP v1.02.08)
					case 2:																											// (9DP v1.02.08)
					{																												// (9DP v1.02.08)
						Save_Config_To_Thumbdrive++;																				// (9DP v1.02.08)
						break;																										// (9DP v1.02.08)
					}																												// (9DP v1.02.08)
					case 3:																											// (9DP v1.02.08)
					{																												// (9DP v1.02.08)
						USB_File_State = FILE_OPEN_WRITE_CFG;																		// (9DP v1.02.08)
						return;																										// (9DP v1.02.08)
					}																												// (9DP v1.02.08)
				}																													// (9DP v1.02.08)
			}
			else if(read_file_type == 2)
			{
				Image_Handler();
			}
			else if(read_file_type == 3)
			{
	            LogFile = FSfopen("image.drw","a");
				if(LogFile != NULL)
				{
					length = strlen((char *)File_RXMessage);
					line_index = 0;
					if(line < 3)
					{
	            		FSfwrite((char *)File_RXMessage, 1, length, LogFile);
						SetClockVars (	(unsigned int) (lmi.settings.calendar.info.date.year + 2000), \
										(unsigned char) lmi.settings.calendar.info.date.month, \
										(unsigned char) lmi.settings.calendar.info.date.day, \
										(unsigned char) lmi.settings.clock.info.time.hours, \
										(unsigned char) lmi.settings.clock.info.time.minutes, \
										(unsigned char) lmi.settings.clock.info.time.seconds);
			           	FSfclose(LogFile);
						c_file_index = 0;
						line++;
					}
					else
					{
						line_index = 0;
						while((length + c_file_index) > 85)
						{
							line_length = 80 - c_file_index;
	            			FSfwrite((char *)&File_RXMessage[line_index], 1, line_length, LogFile);
	            			FSfwrite((char *)ReturnLinefeed, 1, 2, LogFile);
							line_index += line_length;
							length = length - line_length;
							c_file_index = 0;
							line++;
						}

						if(length > 80)
						{
            				FSfwrite((char *)&File_RXMessage[line_index], 1, length, LogFile);
						}
						else
						{
							c_file_index = length - 2;
            				FSfwrite((char *)&File_RXMessage[line_index], 1, c_file_index, LogFile);
						}

						SetClockVars (	(unsigned int) (lmi.settings.calendar.info.date.year + 2000), \
										(unsigned char) lmi.settings.calendar.info.date.month, \
										(unsigned char) lmi.settings.calendar.info.date.day, \
										(unsigned char) lmi.settings.clock.info.time.hours, \
										(unsigned char) lmi.settings.clock.info.time.minutes, \
										(unsigned char) lmi.settings.clock.info.time.seconds);
			           	FSfclose(LogFile);
						line_index = 0;
					}
				}
			}
			USB_File_State = FILE_READ_BYTE;
			break;
		}
		case FILE_OPEN_WRITE:
		{
			if(Update_Log_File == TRUE)
			{
				if(Data_Logging_Header_Sent == FALSE)
				{
					counter = 1;																									// start counter at 1 (9DP v1.01.08)
//					Configure_Header((BYTE *)&File_TXMessage);
					if(File_System_Initialize())
			        {
			            LogFile = FSfopen("output.log","a");
						if(LogFile != NULL)
						{
							Configure_Header((BYTE *)&File_TXMessage);																// moved here (9DP v1.03.02)
							length = strlen((char *)File_TXMessage);
			            	FSfwrite((char *)File_TXMessage, 1, length, LogFile);
							SetClockVars (	(unsigned int) (lmi.settings.calendar.info.date.year + 2000), \
											(unsigned char) lmi.settings.calendar.info.date.month, \
											(unsigned char) lmi.settings.calendar.info.date.day, \
											(unsigned char) lmi.settings.clock.info.time.hours, \
											(unsigned char) lmi.settings.clock.info.time.minutes, \
											(unsigned char) lmi.settings.clock.info.time.seconds);
				           	FSfclose(LogFile);
							Data_Logging_Header_Sent = TRUE;
//							Update_Log_File = FALSE;																				// (hidden 9DP v1.03.02)
						}
						else
						{
							add_error_message_to_queue(ERROR_FILE_OPEN);
						}
					}
					else
					{
						add_error_message_to_queue(ERROR_FILE_SYSTEM);
					}
				}

				if(Power_Up_Report_Integrate == TRUE)																				// added integrate report on power on (9DP v1.01.08)
				{
//					sprintf((char *)File_TXMessage,"%d\0",counter++);			
//					Configure_Message((BYTE *)&File_TXMessage, (BYTE) DOSE_EVENT);
					if(File_System_Initialize())
			        {
			            LogFile = FSfopen("output.log","a");
						if(LogFile != NULL)
						{
							sprintf((char *)File_TXMessage,"%d\0",counter++);														// moved here (9DP v1.03.02)
							Configure_Message((BYTE *)&File_TXMessage, (BYTE) DOSE_EVENT);											// moved here (9DP v1.03.02)
							length = strlen((char *)File_TXMessage);
			            	FSfwrite((char *)File_TXMessage, 1, length, LogFile);
							SetClockVars (	(unsigned int) (lmi.settings.calendar.info.date.year + 2000), \
											(unsigned char) lmi.settings.calendar.info.date.month, \
											(unsigned char) lmi.settings.calendar.info.date.day, \
											(unsigned char) lmi.settings.clock.info.time.hours, \
											(unsigned char) lmi.settings.clock.info.time.minutes, \
											(unsigned char) lmi.settings.clock.info.time.seconds);
				           	FSfclose(LogFile);
							Power_Up_Report_Integrate = FALSE;
						}
						else
						{
							add_error_message_to_queue(ERROR_FILE_OPEN);
						}
					}
					else
					{
						add_error_message_to_queue(ERROR_FILE_SYSTEM);
					}
				}

				if(Report_Temperature_Offset == TRUE)																				// (9DP v1.03.02)
				{																													// (9DP v1.03.02)
					if(File_System_Initialize())																					// (9DP v1.03.02)
			        {																												// (9DP v1.03.02)
			            LogFile = FSfopen("output.log","a");																		// (9DP v1.03.02)
						if(LogFile != NULL)																							// (9DP v1.03.02)
						{																											// (9DP v1.03.02)
							sprintf((char *)File_TXMessage,"%d\0",counter++);														// (9DP v1.03.02)
							Configure_Message((BYTE *)&File_TXMessage, (BYTE) TEMP_EVENT);											// (9DP v1.03.02)
							length = strlen((char *)File_TXMessage);																// (9DP v1.03.02)
			            	FSfwrite((char *)File_TXMessage, 1, length, LogFile);													// (9DP v1.03.02)
							SetClockVars (	(unsigned int) (lmi.settings.calendar.info.date.year + 2000), \
											(unsigned char) lmi.settings.calendar.info.date.month, \
											(unsigned char) lmi.settings.calendar.info.date.day, \
											(unsigned char) lmi.settings.clock.info.time.hours, \
											(unsigned char) lmi.settings.clock.info.time.minutes, \
											(unsigned char) lmi.settings.clock.info.time.seconds);									// (9DP v1.03.02)
				           	FSfclose(LogFile);																						// (9DP v1.03.02)
							Report_Temperature_Offset = FALSE;																		// (9DP v1.03.02)
						}																											// (9DP v1.03.02)
						else																										// (9DP v1.03.02)
						{																											// (9DP v1.03.02)
							add_error_message_to_queue(ERROR_FILE_OPEN);															// (9DP v1.03.02)
						}																											// (9DP v1.03.02)
					}																												// (9DP v1.03.02)
					else																											// (9DP v1.03.02)
					{																												// (9DP v1.03.02)
						add_error_message_to_queue(ERROR_FILE_SYSTEM);																// (9DP v1.03.02)
					}																												// (9DP v1.03.02)
				}																													// (9DP v1.03.02)

				if(System_Shutdown == FALSE)																						// added shutdown check due to integrate feature (9DP v1.01.08)
				{
					if(File_System_Initialize())
			        {
			            LogFile = FSfopen("output.log","a");
						if(LogFile != NULL)
						{
							sprintf((char *)File_TXMessage,"%d\0",counter++);														// increment counter after report (9DP v1.01.09)
							Configure_Message((BYTE *)&File_TXMessage, (BYTE) LOG_EVENT);
							length = strlen((char *)File_TXMessage);
			            	FSfwrite((char *)File_TXMessage, 1, length, LogFile);
							SetClockVars (	(unsigned int) (lmi.settings.calendar.info.date.year + 2000), \
											(unsigned char) lmi.settings.calendar.info.date.month, \
											(unsigned char) lmi.settings.calendar.info.date.day, \
											(unsigned char) lmi.settings.clock.info.time.hours, \
											(unsigned char) lmi.settings.clock.info.time.minutes, \
											(unsigned char) lmi.settings.clock.info.time.seconds);
				           	FSfclose(LogFile);
//							Update_Log_File = FALSE;																				// hidden (9DP v1.03.02)
						}
						else
						{
							add_error_message_to_queue(ERROR_FILE_OPEN);
						}
					}
					else
					{
						add_error_message_to_queue(ERROR_FILE_SYSTEM);
					}
				}

				if((Power_Down_Report_Integrate == TRUE) && (System_Shutdown == TRUE))												// added integrate report on shutdown (9DP v1.01.08)
				{
					sprintf((char *)File_TXMessage,"%d\0",counter++);
					Configure_Message((BYTE *)&File_TXMessage, (BYTE) DOSE_EVENT);
					if(File_System_Initialize())
			        {
			            LogFile = FSfopen("output.log","a");
						if(LogFile != NULL)
						{
							length = strlen((char *)File_TXMessage);
			            	FSfwrite((char *)File_TXMessage, 1, length, LogFile);
							SetClockVars (	(unsigned int) (lmi.settings.calendar.info.date.year + 2000), \
											(unsigned char) lmi.settings.calendar.info.date.month, \
											(unsigned char) lmi.settings.calendar.info.date.day, \
											(unsigned char) lmi.settings.clock.info.time.hours, \
											(unsigned char) lmi.settings.clock.info.time.minutes, \
											(unsigned char) lmi.settings.clock.info.time.seconds);
				           	FSfclose(LogFile);
							Power_Down_Report_Integrate = FALSE;
						}
						else
						{
							add_error_message_to_queue(ERROR_FILE_OPEN);
						}
					}
					else
					{
						add_error_message_to_queue(ERROR_FILE_SYSTEM);
					}
				}
				Update_Log_File = FALSE;																							// (9DP v1.03.02)
			}
			USB_File_State = FILE_NOT_OPEN;		
			break;
		}
		case FILE_OPEN_WRITE_TXT:																									// (9DP v1.02.08)
		{																															// (9DP v1.01.17)
			ReportFile = FSfopen("config.txt","a");																					// (9DP v1.01.17)
			if(ReportFile != NULL)																									// (9DP v1.01.17)
			{																														// (9DP v1.01.17)
				byteCount = strlen(USB_TXMessage) - TXMessageIndex;																	// (9DP v1.01.17)
				if(byteCount)																										// (9DP v1.01.17)
				{																													// (9DP v1.01.17)
           			FSfwrite(&USB_TXMessage[TXMessageIndex], 1, byteCount, ReportFile);												// (9DP v1.01.17)
				}																													// (9DP v1.01.17)
				SetClockVars (	(unsigned int) (lmi.settings.calendar.info.date.year + 2000), \
								(unsigned char) lmi.settings.calendar.info.date.month, \
								(unsigned char) lmi.settings.calendar.info.date.day, \
								(unsigned char) lmi.settings.clock.info.time.hours, \
								(unsigned char) lmi.settings.clock.info.time.minutes, \
								(unsigned char) lmi.settings.clock.info.time.seconds);												// (9DP v1.01.17)
				FSfclose(ReportFile);																								// (9DP v1.01.17)
			}																														// (9DP v1.01.17)
			TXMessageIndex = 0;																										// (9DP v1.01.17)
			Transmit_Data = FALSE;																									// (9DP v1.01.17)
			USB_File_State = FILE_READ_BYTE;																						// (9DP v1.01.17)
			break;																													// (9DP v1.01.17)
		}																															// (9DP v1.01.17)
		case FILE_OPEN_WRITE_CFG:																									// (9DP v1.02.08)
		{																															// (9DP v1.02.08)
			ReportFile = FSfopen("backup.cfg","a");																					// (9DP v1.02.08)
			if(ReportFile != NULL)																									// (9DP v1.02.08)
			{																														// (9DP v1.02.08)
				byteCount = strlen(USB_TXMessage) - TXMessageIndex;																	// (9DP v1.02.08)
				if(byteCount)																										// (9DP v1.02.08)
				{																													// (9DP v1.02.08)
					Replace_Get_With_Set();																							// (9DP v1.02.08)
           			FSfwrite(&USB_TXMessage[TXMessageIndex], 1, byteCount, ReportFile);												// (9DP v1.02.08)
				}																													// (9DP v1.02.08)
				SetClockVars (	(unsigned int) (lmi.settings.calendar.info.date.year + 2000), \
								(unsigned char) lmi.settings.calendar.info.date.month, \
								(unsigned char) lmi.settings.calendar.info.date.day, \
								(unsigned char) lmi.settings.clock.info.time.hours, \
								(unsigned char) lmi.settings.clock.info.time.minutes, \
								(unsigned char) lmi.settings.clock.info.time.seconds);												// (9DP v1.02.08)
				FSfclose(ReportFile);																								// (9DP v1.02.08)
			}																														// (9DP v1.02.08)
			TXMessageIndex = 0;																										// (9DP v1.02.08)
			Transmit_Data = FALSE;																									// (9DP v1.02.08)
			USB_File_State = FILE_READ_BYTE;																						// (9DP v1.02.08)
			break;																													// (9DP v1.02.08)
		}																															// (9DP v1.02.08)
		default:
		{
			USB_File_State = FILE_NOT_OPEN;
			break;
		}
	}
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Replace_Get_With_Set(void) (9DP v1.02.08)
//*
//*	Description:
//*		None
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

void Replace_Get_With_Set(void)
{
	char tier_command[MESSAGE_SIZE];
	char *message;
	BYTE response;
	BYTE index = 0;

	message = strchr((char *)USB_TXMessage, ':');
	while(message != NULL)
	{
		sprintf(tier_command,":GET\0");
		response = strncmp(message,tier_command,4);
		index = (message - (char *)USB_TXMessage) + 1;
		if(response == 0)
		{
			USB_TXMessage[index] = 'S';
			return;
		}
		message = strchr(message + 1, ':');
	}

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL File_System_Initialize(void)
//*
//*	Description:
//* 	The routine will return FALSE if the file system fails to initialize in 1 second;
//*		otherwise, the routine will return TRUE.
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

BOOL File_System_Initialize(void)
{
	UINT32 timeout_counter;
	BOOL FSresult;

	FSresult = FALSE;
	timeout_counter = tick + (UINT32) TICKS_PER_SEC;

	while((timeout_counter != tick) && (FSresult == FALSE))
	{
		DelayMs(10);
		FSresult = FSInit();
	}
	return FSresult;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	END OF USB THUMBDRIVE ROUTINES
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//	DATA RECORDING ROUTINES
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Configure_Header(void)
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

void Configure_Header(BYTE *message_buffer)
{
	char buffer[MESSAGE_SIZE];

	sprintf((char *)message_buffer,"Event,Type\0");													// (9DP v1.01.08)

	if(lmi.features.data_logging.info.reported_data.location)
	{
		sprintf(buffer,",Location\0");																// 9 characters
		strcat((char *)message_buffer,buffer);	
	}

	if(lmi.features.data_logging.info.reported_data.system_status)
	{
		sprintf(buffer,",Status\0", NULL);															// 7 characters
		strcat((char *)message_buffer,buffer);	
	}

	if(lmi.features.data_logging.info.reported_data.date)
	{
		sprintf(buffer,",Date\0");																	// 5 characters
		strcat((char *)message_buffer,buffer);	
	}

	if(lmi.features.data_logging.info.reported_data.time)
	{
		sprintf(buffer,",Time\0");																	// 5 characters
		strcat((char *)message_buffer,buffer);	
	}

	if(lmi.features.data_logging.info.reported_data.model_number)
	{
		sprintf(buffer,",Model\0");																	// 6 characters
		strcat((char *)message_buffer,buffer);	
	}

	if(lmi.features.data_logging.info.reported_data.serial_number)
	{
		sprintf(buffer,",Serial #\0");																// 9 characters
		strcat((char *)message_buffer,buffer);	
	}

	if(lmi.features.data_logging.info.reported_data.user_id_number)
	{
		sprintf(buffer,",User ID #\0");																// 10 characters
		strcat((char *)message_buffer,buffer);	
	}

	if(lmi.features.data_logging.info.reported_data.user_serial_number)
	{
		sprintf(buffer,",User Serial #\0");															// 14 characters
		strcat((char *)message_buffer,buffer);	
	}

	if(lmi.features.data_logging.info.reported_data.temperature)
	{
		sprintf(buffer,",Chamber Temp\0");															// 13 characters
		strcat((char *)message_buffer,buffer);	
	}

	if(lmi.features.data_logging.info.reported_data.high_voltage_1)
	{
		sprintf(buffer,",High Voltage\0");															// 12 characters
		strcat((char *)message_buffer,buffer);	
	}

	if(lmi.features.data_logging.info.reported_data.battery_voltage)
	{
		sprintf(buffer,",Batt Voltage\0");															// 12 characters
		strcat((char *)message_buffer,buffer);	
	}

	if(lmi.features.data_logging.info.reported_data.input_voltage)									// (9DP v1.02.06)
	{
		sprintf(buffer,",Input Voltage\0");															// 14 characters (9DP v1.02.06)
		strcat((char *)message_buffer,buffer);	
	}

	if(lmi.features.data_logging.info.reported_data.reading)
	{
		sprintf(buffer,",Reading\0");																// 8 characters
		strcat((char *)message_buffer,buffer);	
	}

	if(lmi.features.data_logging.info.reported_data.reading_multiplier)
	{
		sprintf(buffer,",Multiplier\0");															// 11 characters
		strcat((char *)message_buffer,buffer);	
	}

	if(lmi.features.data_logging.info.reported_data.reading_units)
	{
		sprintf(buffer,",Units\0");																	// 6 characters
		strcat((char *)message_buffer,buffer);	
	}

	if(lmi.features.data_logging.info.reported_data.integrated_reading)
	{
		sprintf(buffer,",Int. Reading\0");															// 13 characters (9DP v1.01.23)
		strcat((char *)message_buffer,buffer);	
	}

	if(lmi.features.data_logging.info.reported_data.integrated_reading_multiplier)
	{
		sprintf(buffer,",Int. Multiplier\0");														// 16 characters (9DP v1.01.23)
		strcat((char *)message_buffer,buffer);	
	}

	if(lmi.features.data_logging.info.reported_data.integrated_reading_units)
	{
		sprintf(buffer,",Int. Units\0");															// 11 characters (9DP v1.01.23)
		strcat((char *)message_buffer,buffer);	
	}

	sprintf(buffer,"\r\n\0");																		// 3 characters
	strcat((char *)message_buffer,buffer);	

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Configure_Message(BYTE *message_buffer, BYTE message_type)
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

void Configure_Message(BYTE *message_buffer, BYTE message_type)
{
	char buffer[MESSAGE_SIZE];
	float rate_result;

	switch(message_type)																											// Always include message type (9DP v1.01.08)
	{
		case NO_READING_EVENT:
		{
			sprintf(buffer,",Status\0");																							// (9DP v1.01.08)
			break;
		}
		case LOG_EVENT:
		{
			sprintf(buffer,",Log\0");																								// (9DP v1.01.08)
			break;
		}
		case TEMP_EVENT:																											// (9DP v1.03.02)
		{																															// (9DP v1.03.02)
			sprintf(buffer,",Temp\0");																								// (9DP v1.03.02)
			break;																													// (9DP v1.03.02)
		}																															// (9DP v1.03.02)
//		case RATE_ALERT_EVENT:
//		{
//			sprintf(buffer,",RATE ALERT\0");																						// (9DP v1.01.08)
//			break;
//		}
//		case RATE_ALARM_EVENT:
//		{
//			sprintf(buffer,",RATE ALARM\0");																						// (9DP v1.01.08)
//			break;
//		}
		case DOSE_EVENT:
		{
			sprintf(buffer,",Dose\0");																								// (9DP v1.01.08)
			break;
		}
//		case DOSE_ALERT_EVENT:
//		{
//			sprintf(buffer,",DOSE ALERT\0");																						// (9DP v1.01.08)
//			break;
//		}
//		case DOSE_ALARM_EVENT:
//		{
//			sprintf(buffer,",DOSE ALARM\0");																						// (9DP v1.01.08)
//			break;
//		}
		default:
		{
			sprintf(buffer,",UNKNOWN\0");																							// (9DP v1.01.08)
			break;
		}
	}
	strcat((char *)message_buffer,buffer);																							// Always include message type (9DP v1.01.08)

	if(lmi.features.data_logging.info.reported_data.location)
	{
		sprintf(buffer,",%s\0",NULL);
		strcat((char *)message_buffer,buffer);	
	}

	if(lmi.features.data_logging.info.reported_data.system_status)
	{
		sprintf(buffer,",%s\0", NULL);
		strcat((char *)message_buffer,buffer);	
	}

	if(lmi.features.data_logging.info.reported_data.date)
	{
		lmi.features.data_logging.info.last_date.month = lmi.settings.calendar.info.date.month;
		lmi.features.data_logging.info.last_date.day = lmi.settings.calendar.info.date.day;
		lmi.features.data_logging.info.last_date.year = lmi.settings.calendar.info.date.year;
		DateStringConversion(&lmi.features.data_logging.info.last_date, (BYTE *)&buffer);											// (v1.02.00)
		strcat((char *)message_buffer,",\0");	
		strcat((char *)message_buffer,buffer);	
	}

	if(lmi.features.data_logging.info.reported_data.time)
	{
		lmi.features.data_logging.info.last_time.hours = lmi.settings.clock.info.time.hours;
		lmi.features.data_logging.info.last_time.minutes = lmi.settings.clock.info.time.minutes;
		lmi.features.data_logging.info.last_time.seconds = lmi.settings.clock.info.time.seconds;
		TimeStringConversion(&lmi.features.data_logging.info.last_time, (BYTE *)&buffer);											// (v1.02.00)
		strcat((char *)message_buffer,",\0");	
		strcat((char *)message_buffer,buffer);	
	}

	if(lmi.features.data_logging.info.reported_data.model_number)
	{
		Format_Text_String(buffer, lmi.settings.product.info.model, LENGTH_MODEL);													// (9DP v1.01.08)
		strcat((char *)message_buffer,buffer);	
	}

	if(lmi.features.data_logging.info.reported_data.serial_number)
	{
		Format_Text_String(buffer, lmi.settings.product.info.serial_number, LENGTH_SERIAL_NUMBER);									// (9DP v1.01.08)
		strcat((char *)message_buffer,buffer);	
	}

	if(lmi.features.data_logging.info.reported_data.user_id_number)
	{
		Format_Text_String(buffer, lmi.settings.product.info.user_id_number, LENGTH_USER_ID_NUMBER);								// (9DP v1.01.08)
		strcat((char *)message_buffer,buffer);	
	}

	if(lmi.features.data_logging.info.reported_data.user_serial_number)
	{
		Format_Text_String(buffer, lmi.settings.product.info.user_serial_number, LENGTH_USER_SERIAL_NUMBER);						// (9DP v1.01.08)
		strcat((char *)message_buffer,buffer);	
	}

	if(lmi.features.data_logging.info.reported_data.temperature)
	{
		if(message_type == TEMP_EVENT)																								// (9DP v1.03.02)
		{																															// (9DP v1.03.02)
			sprintf(buffer,",%.1f\0", Temperature_Average);																			// (9DP v1.03.02)
		}																															// (9DP v1.03.02)
		else																														// (9DP v1.03.02)
		{																															// (9DP v1.03.02)
			sprintf(buffer,",%.1f\0", Temperature_Reading);
		}																															// (9DP v1.03.02)
		strcat((char *)message_buffer,buffer);	
	}

	if(lmi.features.data_logging.info.reported_data.high_voltage_1)
	{
		sprintf(buffer,",%.1f\0", High_Voltage_Reading);
		strcat((char *)message_buffer,buffer);	
	}

	if(lmi.features.data_logging.info.reported_data.battery_voltage)
	{
		sprintf(buffer,",%.2f\0", Battery_Reading);
		strcat((char *)message_buffer,buffer);	
	}

	if(lmi.features.data_logging.info.reported_data.input_voltage)																	// (9DP v1.02.06)
	{
		sprintf(buffer,",%5.3f\0", Signal_Voltage);																					// (9DP v1.02.06)
		strcat((char *)message_buffer,buffer);	
	}

	if(lmi.features.data_logging.info.reported_data.reading)
	{
		if(message_type == LOG_EVENT)
		{
			rate_result = UnitConversions(Rate_uR_hr, Default_Rate_Units, lmi.display.detector[Current_Detector].view[Current_View].element.info.arc.control.units);
			lmi.features.data_logging.info.last_reading.value = rate_result;
			sprintf(buffer,",%0.3f\0",rate_result);																					// (9DP v1.01.22)
		}
		else if(message_type == DOSE_EVENT)
		{
			lmi.features.data_logging.info.last_reading.value = lmi.features.integrate.info.current_reading.value;					// (9DP v1.01.02)
			sprintf(buffer,",%0.3f\0",lmi.features.integrate.info.current_reading.value);											// (9DP v1.01.22)
		}
		else if(message_type == TEMP_EVENT)																																	// (9DP v1.03.02)
		{																																									// (9DP v1.03.02)
			rate_result = UnitConversions(Reading_Average, Default_Rate_Units, lmi.display.detector[Current_Detector].view[Current_View].element.info.arc.control.units); 	// (9DP v1.03.02)
			sprintf(buffer,",%0.3f\0",rate_result);																															// (9DP v1.03.02)
		}																																									// (9DP v1.03.02)
		else
		{
			sprintf(buffer,",0\0");
		}
		strcat((char *)message_buffer,buffer);	
	}

	if(lmi.features.data_logging.info.reported_data.reading_multiplier)
	{
		if(message_type == LOG_EVENT)
		{
			lmi.features.data_logging.info.last_reading.multiplier = (short) DEFAULT_RATE_MULTIPLIER;								// (9DP v1.01.12)
			rate_result = MultiplierConversion((short) DEFAULT_RATE_MULTIPLIER);													// (9DP v1.01.12)
			sprintf(buffer,",%.0E\0",rate_result);																					// (9DP v1.01.02)
		}
		else if(message_type == DOSE_EVENT)																							// (9DP v1.01.02)
		{																															// (9DP v1.01.02)
			lmi.features.data_logging.info.last_reading.multiplier = lmi.features.integrate.info.current_reading.multiplier;		// (9DP v1.01.02)
			rate_result = MultiplierConversion(lmi.features.integrate.info.current_reading.multiplier);								// (9DP v1.01.02)
			sprintf(buffer,",%.0E\0",rate_result);																					// (9DP v1.01.02)
		}																															// (9DP v1.01.02)
		else if(message_type == TEMP_EVENT)																							// (9DP v1.03.02)
		{																															// (9DP v1.03.02)
			rate_result = MultiplierConversion((short) DEFAULT_RATE_MULTIPLIER);													// (9DP v1.03.02)
			sprintf(buffer,",%.0E\0",rate_result);																					// (9DP v1.03.02)
		}																															// (9DP v1.03.02)
		else
		{
			sprintf(buffer,",0\0");
		}
		strcat((char *)message_buffer,buffer);	
	}

	if(lmi.features.data_logging.info.reported_data.reading_units)
	{
		if((message_type == LOG_EVENT) || (message_type == TEMP_EVENT))																// temp event added (9DP v1.03.02)
		{
			switch(lmi.display.detector[Current_Detector].view[Current_View].element.info.arc.control.units)
			{
				case R_HR:
				{
					lmi.features.data_logging.info.last_reading.units = R_HR;
					sprintf(buffer,",R/hr\0");
					break;
				}
				case SV_HR:
				{
					lmi.features.data_logging.info.last_reading.units = SV_HR;
					sprintf(buffer,",Sv/h\0");																						// (9DP v1.01.12)
					break;
				}
				case GY_HR:
				{
					lmi.features.data_logging.info.last_reading.units = GY_HR;
					sprintf(buffer,",Gy/h\0");																						// (9DP v1.01.12)
					break;
				}
			}
		}
		else if(message_type == DOSE_EVENT)																							// (9DP v1.01.02)
		{																															// (9DP v1.01.02)
			lmi.features.data_logging.info.last_reading.units = lmi.features.integrate.info.current_reading.units;					// (9DP v1.01.02)
			switch(lmi.features.integrate.info.current_reading.units)																// (9DP v1.01.02)
			{																														// (9DP v1.01.02)
				case R:																												// (9DP v1.01.02)
				{																													// (9DP v1.01.02)
					sprintf(buffer,",R\0");																							// (9DP v1.01.02)
					break;																											// (9DP v1.01.02)
				}																													// (9DP v1.01.02)
				case SV:																											// (9DP v1.01.02)
				{																													// (9DP v1.01.02)
					sprintf(buffer,",Sv\0");																						// (9DP v1.01.02)
					break;																											// (9DP v1.01.02)
				}																													// (9DP v1.01.02)
				case GY:																											// (9DP v1.01.02)
				{																													// (9DP v1.01.02)
					sprintf(buffer,",Gy\0");																						// (9DP v1.01.02)
					break;																											// (9DP v1.01.02)
				}																													// (9DP v1.01.02)
			}																														// (9DP v1.01.02)
		}																															// (9DP v1.01.02)
		else
		{
			sprintf(buffer,",0\0");
		}
		strcat((char *)message_buffer,buffer);	
	}

	if(lmi.features.data_logging.info.reported_data.integrated_reading)
	{
		sprintf(buffer,",%0.3f\0",lmi.features.integrate.info.current_reading.value);												// (9DP v1.01.22)
		strcat((char *)message_buffer,buffer);	
	}

	if(lmi.features.data_logging.info.reported_data.integrated_reading_multiplier)
	{
		rate_result = MultiplierConversion(lmi.features.integrate.info.current_reading.multiplier);									// (9DP v1.01.02)
		sprintf(buffer,",%.0E\0",rate_result);																						// (9DP v1.01.02)
		strcat((char *)message_buffer,buffer);	
	}

	if(lmi.features.data_logging.info.reported_data.integrated_reading_units)
	{
		switch(lmi.features.integrate.info.current_reading.units)																	// (9DP v1.01.02)
		{
			case R:
			{
				sprintf(buffer,",R\0");
				break;
			}
			case SV:
			{
				sprintf(buffer,",Sv\0");
				break;
			}
			case GY:
			{
				sprintf(buffer,",Gy\0");
				break;
			}
			default:
			{
				sprintf(buffer,",ERROR\0");
				break;
			}
		}
		strcat((char *)message_buffer,buffer);	
	}

	sprintf(buffer,"\r\n\0");
	strcat((char *)message_buffer,buffer);	

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Format_Text_String(char *generic_string, char *message, UINT8 message_length)
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
//* 	(9DP v1.01.08)
//*

void Format_Text_String(char *generic_string, char *message, UINT8 message_length)
{
	char message_string[51];

	strncpy(message_string, message, message_length);
	message_string[message_length] = '\0';
	sprintf(generic_string,",%s\0",message_string);
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Configure_Data_Stream(BYTE *message_buffer)
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

void Configure_Data_Stream(BYTE *message_buffer)
{
	char stringconversion[29];
	short peak_conversion;
	float peak_value;

	Unconvert_Display(stringconversion, ExposureValue, 7);												// get last displayed reading
	peak_conversion = RelativeMultiplier((short) DEFAULT_RATE_MULTIPLIER, ExposureMultiplier);			// get relative multiplier for last displayed reading (9DP v1.01.12)
	peak_value = FloatValueConversion(Peak_Rate_Reading, peak_conversion);								// convert peak rate reading based on last multiplier reading
	peak_value = UnitConversions(peak_value, Default_Rate_Units, ExposureUnits);						// convert reading based on last displayed units (9DP v1.01.16)	
	sprintf((BYTE *)message_buffer,"%s,%d,%d,%d,%.1f\r\n\0", stringconversion, (ExposureMultiplier - 4), ExposureUnits, Full_Scale_Arc_Range[ExposureRange][0], peak_value);
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Initialize_Reporting(BOOL log_state)
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
//* 	rewritten (9DP v1.01.09)
//*

void Initialize_Reporting(BOOL log_state)
{
	Data_Logging_Enabled = log_state;
	Data_Logging_Header_Sent = FALSE;
	Power_Up_Report_Integrate = (BOOL) lmi.features.integrate.info.startup_log;
	Power_Down_Report_Integrate = (BOOL) lmi.features.integrate.info.shutdown_log;

	if(log_state == TRUE)
	{
		log_file_write_tick = tick + (lmi.features.data_logging.info.delay * (DWORD) TICKS_PER_SEC);
		Update_Log_File = FALSE;
	}

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//	END DATA RECORDING ROUTINES
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//*	ALERT AND ALARM ROUTINES
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Update_Alerts_Alarms(BYTE detector, BYTE view)
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

void Update_Alerts_Alarms(BYTE detector, BYTE view)
{
	Calculate_New_Radiation_Alert(detector, view);
	Calculate_New_Radiation_Alarm(detector, view);
	Calculate_New_Integrate_Alert(detector, view);
	Calculate_New_Integrate_Alarm(detector, view);

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Calculate_New_Radiation_Alert(BYTE detector, BYTE view)
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

void Calculate_New_Radiation_Alert(BYTE detector, BYTE view)
{
	short multiplier;
	float rate;

//	multiplier = RelativeMultiplier(lmi.display.detector[detector].view[view].alerts_radiation.info.feature.reading.multiplier, Default_Rate_Multiplier);
	multiplier = RelativeMultiplier(lmi.display.detector[detector].view[view].alerts_radiation.info.feature.reading.multiplier, (short) DEFAULT_RATE_MULTIPLIER);	// (9DP v1.01.12)
	rate = ShortValueConversion(lmi.display.detector[detector].view[view].alerts_radiation.info.feature.reading.value, multiplier);
	lmi.display.detector[detector].view[view].alerts_radiation.info.feature.relative_reading = rate;

	if((detector == Current_Detector) && (view == Current_View))
		Radiation_Alert_Rate = rate;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Calculate_New_Radiation_Alert(BYTE detector, BYTE view)
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

void Calculate_New_Radiation_Alarm(BYTE detector, BYTE view)
{
	short multiplier;
	float rate;

//	multiplier = RelativeMultiplier(lmi.display.detector[detector].view[view].alarms_radiation.info.feature.reading.multiplier, Default_Rate_Multiplier);
	multiplier = RelativeMultiplier(lmi.display.detector[detector].view[view].alarms_radiation.info.feature.reading.multiplier, (short) DEFAULT_RATE_MULTIPLIER);	// (9DP v1.01.12)
	rate = ShortValueConversion(lmi.display.detector[detector].view[view].alarms_radiation.info.feature.reading.value, multiplier);
	lmi.display.detector[detector].view[view].alarms_radiation.info.feature.relative_reading = rate;

	if((detector == Current_Detector) && (view == Current_View))
		Radiation_Alarm_Rate = rate;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Calculate_New_Integrate_Alert(BYTE detector, BYTE view)
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

void Calculate_New_Integrate_Alert(BYTE detector, BYTE view)
{
	short multiplier;
	float rate;

//	multiplier = RelativeMultiplier(lmi.display.detector[detector].view[view].alerts_integrate.info.feature.reading.multiplier, Default_Dose_Multiplier);			// (9DP v1.01.10)
	multiplier = RelativeMultiplier(lmi.display.detector[detector].view[view].alerts_integrate.info.feature.reading.multiplier, (short) DEFAULT_DOSE_MULTIPLIER);	// (9DP v1.01.12)
	rate = ShortValueConversion(lmi.display.detector[detector].view[view].alerts_integrate.info.feature.reading.value, multiplier);
	lmi.display.detector[detector].view[view].alerts_integrate.info.feature.relative_reading = rate;

	if((detector == Current_Detector) && (view == Current_View))
		Integrate_Alert_Rate = rate;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Calculate_New_Integrate_Alert(BYTE detector, BYTE view)
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

void Calculate_New_Integrate_Alarm(BYTE detector, BYTE view)
{
	short multiplier;
	float rate;

//	multiplier = RelativeMultiplier(lmi.display.detector[detector].view[view].alarms_integrate.info.feature.reading.multiplier, Default_Dose_Multiplier);			// (9DP v1.01.10)
	multiplier = RelativeMultiplier(lmi.display.detector[detector].view[view].alarms_integrate.info.feature.reading.multiplier, (short) DEFAULT_DOSE_MULTIPLIER);	// (9DP v1.01.12)
	rate = ShortValueConversion(lmi.display.detector[detector].view[view].alarms_integrate.info.feature.reading.value, multiplier);
	lmi.display.detector[detector].view[view].alarms_integrate.info.feature.relative_reading = rate;

	if((detector == Current_Detector) && (view == Current_View))
		Integrate_Alarm_Rate = rate;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	END ALERT AND ALARM ROUTINES
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//*	SHUTDOWN ROUTINES
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Check_Shutdown(void)
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

void Check_Shutdown(void)
{
    static UINT32 hold_cnt = 0;
	if(PORTEbits.RE8 == 0)
	{
		hold_cnt++;
		if(hold_cnt > 5)        // n2 - reduced hold_cnt check from 150 to 5. Each 5 counts is about 1 second, so you have to hold the button for 2 s to shut down.
		{
			if(System_Shutdown)
			{
				Shutdown_Power();
			}
			else
			{
				System_Shutdown = TRUE;
				Update_Log_File = TRUE;																					// force final log file update if necessary (9DP v1.01.08)
				hold_cnt = 0;
			}
		}
	}
	else
	{
		if(System_Shutdown == TRUE)																						// if system was previously shutting down... (9DP v1.01.09)
		{
			Power_Down_Report_Integrate = (BOOL) lmi.features.integrate.info.shutdown_log;								// ...reset power down report (9DP v1.01.09)
			log_file_write_tick = tick + (lmi.features.data_logging.info.delay * (DWORD) TICKS_PER_SEC);				// ...reset log timer (9DP v1.01.09)
			Update_Log_File = FALSE;																					// ...clear log file update flag (9DP v1.01.09)
			System_Shutdown = FALSE;																					// ...cancel system shutdown (9DP v1.01.09)
		}
		hold_cnt = 0;
	}	
	
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Shutdown_Power(void)
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
//*		None
//*

void Shutdown_Power(void)
{
	char stringchar[51] = {'\0'};

	T2CON = 0x0; 																					// Stop Timer2 and clear
	T3CON = 0x0; 																					// Stop Timer3 and clear

 	INTDisableInterrupts();																			// Disable all interrupts
#ifndef TEST_MODE
	EEPROM_Write_All();																				// Save all data to the EEPROM
#endif
	SetFont((void*)&GOLFontDefault);																// &Arial_Normal_12
	Set_Backlight_Level(100);																		// max backlight
	SetColor(WHITE);																				// set background color
	ClearDevice();																					// clear LCD
	Audio_IO_OutLAT = (Audio_IO_OutLAT & DISABLE_AUDIO & MUTE_AUDIO & DISABLE_ALARM);				// disable and mute audio, clear any alarm tone outputs (9DP v1.01.13)
	Audio_IO_OutLAT = (Audio_IO_OutLAT | DISABLE_HV);												// shutdown the HV supply (9DP v1.01.13)
	i2c2Write(i2cADDR_MAIN_IO_AUDIO, 0x0A, 1, Audio_IO_OutLAT, 1);									// write to the OLAT Register...... I/O expander GP 6 output -- Activate HV Shutdown
	if(Drained_Battery)																				// check if flag set for batteries almost dead
	{
		sprintf(stringchar, "Low Battery! Shutting Down ADC_Battery_Level = %d", GetADCValue((BYTE) ADC_PORT3_BATTERY));
		Use_Custom_Message(stringchar, MSG_FULL_HEADING, WARNING_SHUTDOWN, CENTER_JUSTIFY);			// arrange custom message
	}
	else
	{
		sprintf((char *)DisplayMessage, "SYSTEM STATUS\0");
		sprintf(stringchar, "Instrument Shutting Down");
		Use_Custom_Message(stringchar, MSG_FULL_HEADING, NORMAL_MESSAGE, CENTER_JUSTIFY);			// arrange custom message
	}
	DelayMs(2000);																					// add delay to allow display to finish drawing message	
	Set_Backlight_Level(0);																			// shutdown backlight										
	SetColor(BLACK);																				// set background color
	ClearDevice();																					// clear LCD
	DelayMs(2000);																					// add delay to allow display to finish drawing message	
	i2c2Write(i2cADDR_PIC_RTCC, i2cPIC_RTC_COMMAND, 1, SHUTDOWN_CODE, 1);							// signal supervisor to remove power (v1.0.4 supervisor)
	i2c2Write(i2cADDR_PIC_RTCC, i2cPIC_RTC_STATUS, 1, SHUTDOWN_CODE, 1);							// signal supervisor to remove power (v1.0.3 supervisor)
//	PORTSetBits(IOPORT_E, BIT_9);																	// (v1.0.5) (hidden 9DP v1.01.21)

    
	while(1);																						// hang up program for shutdown to occur
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Software_Reset(void)	
//*
//*	Description:
//* 	This function forces a software reset of the system.
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

void Software_Reset(void)
{
	UINT16 response;

	INTDisableInterrupts();																			// Disable all interrupts

	SYSKEY = 0x12345678;
	SYSKEY = 0xAA996655;
	SYSKEY = 0x556699AA;
	RSWRSTSET = 1;
	response = RSWRST;
	while(1);
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	END SHUTDOWN ROUTINES
//**************************************************************************************************
//**************************************************************************************************

//*
//**************************************************************************************************
//**************************************************************************************************
//* LUDLUM MEASUREMENTS INC. COPYRIGHT (C) 2012
//**************************************************************************************************
//**************************************************************************************************
