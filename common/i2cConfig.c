//**************************************************************************************************
//**************************************************************************************************
//*																		
//*	Filename:		i2cConfig.c
//*	Date:			06/16/10
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
//*	~~~~~~~~	06/16/10	James Comstock
//*	- continue where Clint left off
//*
//*	~~~~~~~~	12/18/09	Clint Appling
//*	- Original Prototype
//*                                                                     
//**************************************************************************************************
//*	I2C Peripherals
//**************************************************************************************************
//*
//*	9DP Main Board Addresses:
//*
//*		U7 	- 24AA512 Serial EEPROM				-	[1010 001x]
//*		U8 	- 24AA512 Serial EEPROM				-	[1010 000x]
//*		U9	- MCP23008 I/O Expander				-	[0100 111x]
//*		U10 - AD7998-0 12-bit ADC				-	[0100 001x]
//*		U12	- MCP23008 I/O Expander				-	[0100 000x]
//*		U15 - PIC18F26J11 Supervisor			-	[1110 110x]
//*		U17	- MCP4725 DAC						-	[1100 000x]
//*
//*	Where x = R/W Command
//*
//**************************************************************************************************
//*
//* HV Board Addresses:
//*
//*	9DP -
//*		U6	- MAX5842LEUB 12-bit DAC			-	[0111 101x]
//*		U7	- MCP23008 I/O Expander				-	[0100 100x]
//*
//*	Basic -
//*		U9	- MAX5842LEUB 12-bit DAC			-	[0111 101x]
//*
//*	Advanced
//*		U1	- MAX5842LEUB 12-bit DAC			-	[0111 101x]
//*		U8	- MAX5842LEUB 12-bit DAC			-	[0111 100x]
//*
//*	Inter/Ext
//*		U1	- MAX5842LEUB 12-bit DAC			-	[0111 101x]
//*		U2	- MAX5842LEUB 12-bit DAC			-	[0111 100x]
//*
//*	Where x = R/W Command
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

#include <plib.h>
#include "Graphics.h"
#include "I2C.h"
#include "i2cConfig.h"
#include "instrument.h"
#include "Definitions.h"
#include "System.h"

//**************************************************************************************************
//**************************************************************************************************
//*	Define Variables

extern TYPE_INSTRUMENT lmi;

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void setupi2c2_peripherals(void)
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

void setupi2c2_peripherals(void)
{
	setupi2c2_main_adc();
	setupi2c2_main_dac();
	setupi2c2_main_io_audio();																		// configure IOexpander to control the audio modes
	setupi2c2_main_io_buttons();																	// configure IOexpander to read the user buttons
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void setupi2c2_main_adc(void)
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
//*		06-6771 - AD7998
//*		Address: 0x46
//*

void setupi2c2_main_adc(void) 
{
//	i2c2Write(i2cADDR_MAIN_ADC, 0x03, 1, 0x00, 0);													// cycle timer register - mode not selected
	i2c2Write(i2cADDR_MAIN_ADC, 0x03, 1, 0x00, 1);													// cycle timer register - mode not selected (DEBUG!!! 9DP v1.02.04)
	i2c2Write(i2cADDR_MAIN_ADC, 0x02, 1, 0x00, 2);													// configuration register - everything disabled
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void setupi2c2_main_dac(void)
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
//* 	06-6795 - MCP4725
//*		Address: 0xC2
//*

void setupi2c2_main_dac(void)
{										
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void setupi2c2_main_io_audio(void)
//*
//*	Description:
//* 	Setup function to control the audio modes
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
//* 	06-6777 - MCP23008
//*		Address: 0x4E
//*

void setupi2c2_main_io_audio(void)
{
	i2c2Write(i2cADDR_MAIN_IO_AUDIO, 0x00, 1, 0x00, 1);												// set I/O direction, 0 = output, 1 = Input
	i2c2Write(i2cADDR_MAIN_IO_AUDIO, 0x01, 1, 0x00, 1);												// Input polarity, - Inputs not used in this configuration
	i2c2Write(i2cADDR_MAIN_IO_AUDIO, 0x02, 1, 0x00, 1);												// Interrupt on change, 0 = disabled
	i2c2Write(i2cADDR_MAIN_IO_AUDIO, 0x03, 1, 0x00, 1);												// Default value register, 0 is default level
	i2c2Write(i2cADDR_MAIN_IO_AUDIO, 0x04, 1, 0x00, 1);												// Interrupt on change, - does not apply
	i2c2Write(i2cADDR_MAIN_IO_AUDIO, 0x05, 1, 0x02, 1);												// No sequential operation, control slew rate, active output driver, INT active High
	i2c2Write(i2cADDR_MAIN_IO_AUDIO, 0x06, 1, 0x00, 1);												// Pull-up resistors disabled
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void setupi2c2_main_io_buttons(void)
//*
//*	Description:
//* 	Set up function to read user buttons.
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
//* 	06-6777 - MCP23008
//*		Address: 0x40
//*

void setupi2c2_main_io_buttons(void)
{
	i2c2Write(i2cADDR_MAIN_IO_BUTTON, 0x00, 1, 0x7F, 1);											// 0x0F set I/O direction, 0 = output, 1 = input
	i2c2Write(i2cADDR_MAIN_IO_BUTTON, 0x01, 1, 0x00, 1);											// 0x0F Input polarity, GPIO register will reflect opposite of logic input pin state
	i2c2Write(i2cADDR_MAIN_IO_BUTTON, 0x02, 1, 0x00, 1);											// Interrupt on change, 1 = Enabled
	i2c2Write(i2cADDR_MAIN_IO_BUTTON, 0x03, 1, 0x00, 1);											// Default value register, 0 is default level
	i2c2Write(i2cADDR_MAIN_IO_BUTTON, 0x04, 1, 0x0F, 1);											// Interrupt on change, compared to DEFVAL register to decide interrupt condition
	i2c2Write(i2cADDR_MAIN_IO_BUTTON, 0x05, 1, 0x02, 1);											// No sequential operation, control slew rate, active output driver, INT active High
	i2c2Write(i2cADDR_MAIN_IO_BUTTON, 0x06, 1, 0x00, 1);											// Pull-up resistors disabled
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void setupi2c2_hv_io_0x48(void)
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
//* 	06-6777 - MCP23008
//*		Address: 0x48
//*

void setupi2c2_hv_io_0x48(void)
{
	i2c2Write(i2cADDR_HV_IO_0x48, 0x00, 1, 0x00, 1);												// set I/O direction, 0 = output, 1 = Input
	i2c2Write(i2cADDR_HV_IO_0x48, 0x01, 1, 0x00, 1);												// Input polarity, - Inputs not used in this configuration
	i2c2Write(i2cADDR_HV_IO_0x48, 0x02, 1, 0x00, 1);												// Interrupt on change, 0 = disabled
	i2c2Write(i2cADDR_HV_IO_0x48, 0x03, 1, 0x00, 1);												// Default value register, 0 is default level
	i2c2Write(i2cADDR_HV_IO_0x48, 0x04, 1, 0x00, 1);												// Interrupt on change, - does not apply
	i2c2Write(i2cADDR_HV_IO_0x48, 0x05, 1, 0x02, 1);												// No sequential operation, control slew rate, active output driver, INT active High
	i2c2Write(i2cADDR_HV_IO_0x48, 0x06, 1, 0x00, 1);												// Pull-up resistors disabled
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void setupi2c2_hv_dac_0xc0(void)
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
//*		06-6791 - MCP4728
//*		Address: 0xC0
//*

void setupi2c2_hv_dac_0xc0(void)				
{
	PORTClearBits(IOPORT_B, BIT_2);																	// (9DP v1.01.13)

	i2c2Write(i2cADDR_HV_DAC_0xC0, 0x06, 1, 0x00, 0);												// general call reset
	i2c2Write(i2cADDR_HV_DAC_0xC0, 0x8F, 1, 0x00, 0);												// set reference to 2.048V for all channels
	i2c2Write(i2cADDR_HV_DAC_0xC0, 0xCF, 1, 0x00, 0);												// set gain of 2 for all channels
	i2c2Write(i2cADDR_HV_DAC_0xC0, 0xA000, 2, 0x00, 0);												// set power down selection to normal for all channels
	DelayMs(50);

	i2c2Write(i2cADDR_HV_DAC_0xC0, (DAC_SingleWrite | DAC_Channel_A), 1, 0x9000, 2);				// write channel A (0.000V)
	DelayMs(50);
	i2c2Write(i2cADDR_HV_DAC_0xC0, (DAC_SingleWrite | DAC_Channel_B), 1, 0x9000, 2);				// write channel B (0.000V)
	DelayMs(50);
	i2c2Write(i2cADDR_HV_DAC_0xC0, (DAC_SingleWrite | DAC_Channel_C), 1, 0x9000, 2);				// write channel C (0.000V)
	DelayMs(50);
	i2c2Write(i2cADDR_HV_DAC_0xC0, (DAC_SingleWrite | DAC_Channel_D), 1, 0x9000, 2);				// write channel D (0.000V)
	DelayMs(50);

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void setupi2c2_x_temp_0x90(void)
//*
//*	Description:
//*		The I2C interfaces with a temperature device. This routine configures the temperature
//*		device prior to use.
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
//*		06-6687 - MCP9800
//*		Address: 0x90
//*

void setupi2c2_x_temp_0x90(void)
{
	UINT16 tempint16 = 0;

	tempint16 = ( ONESHOT_DISABLED																	// oneshot disabled
				| ADC_12BIT																			// 12 bit ADC resolution
				| FAULT_QUEUE_1																		// 1 fault queue
				| ALERT_POL_LO																		// active low alert polarity
				| COMP_MODE																			// comparator mode
				| SHUTDOWN_DISABLED																	// shutdown disabled
				);
	i2c2Write(i2cADDR_X_TEMP_0x90, CONFIG_REG, 1, tempint16, 1);

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void setupi2c2_x_pres_0x28(void)
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
//* 	2312222 - SSCMNNN150PA2A3
//*		Address: 0x48
//*

void setupi2c2_x_pres_0x28(void)
{
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void setVolume_i2c(UINT8 DeviceAddress, UINT8 DeviceRegister, UINT16 volume)
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
//* 	Enter 0 - 1000 where 1000 is 100.0%
//*

void setVolume_i2c(UINT8 DeviceAddress, UINT8 DeviceRegister, UINT16 volume)
{
	float tempfloat;
	UINT16 tempint;
	UINT16 RegisterData;

	tempint = 100 - volume;																			// used to reverse output (full volume with DAC at 0.0 output)
	tempfloat = (float)tempint * 40.95;																// scale so 1000 is full scale 4095 going to the DAC
	RegisterData = (UINT16)tempfloat;

	if(RegisterData > MAXDACVALUE)																	// check if greater than 4095
		RegisterData = MAXDACVALUE;

	RegisterData = RegisterData << 4;
	i2c2Write(DeviceAddress, DeviceRegister, 1, RegisterData, 2);									// Set DAC channel
	
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	UINT16 getVolume_i2c(UINT8 DeviceAddress, UINT8 DeviceRegister)
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
//* 	Enter 0 - 1000 where 1000 is 100.0%
//*

UINT16 getVolume_i2c(UINT8 DeviceAddress, UINT8 DeviceRegister)
{
	float tempfloat;
	UINT8 ControlData;
	UINT8 URegisterData;
	UINT8 LRegisterData;
	UINT8 UEEPROMData;
	UINT8 LEEPROMData;
	UINT16 RegisterData;

	StartI2C2();																					// Send Start Bit
	IdleI2C2();																						// Wait to complete

	MasterWriteI2C2(DeviceAddress | i2cREAD);														// tranmit read command to device
	IdleI2C2();																						// wait to complete

	ControlData = MasterReadI2C2();
	AckI2C2();																						// ACK
	IdleI2C2();																						// Wait to complete --------------------- hanging up

	URegisterData = MasterReadI2C2();
	AckI2C2();																						// ACK
	IdleI2C2();																						// Wait to complete --------------------- hanging up

	LRegisterData = MasterReadI2C2();
	AckI2C2();																						// ACK
	IdleI2C2();																						// Wait to complete --------------------- hanging up

	UEEPROMData = MasterReadI2C2();
	AckI2C2();																						// ACK
	IdleI2C2();																						// Wait to complete --------------------- hanging up

	LEEPROMData = MasterReadI2C2();
	NotAckI2C2();																					// NACK

	StopI2C2();																						// send the stop condition
	IdleI2C2();																						//-- wait to complete   

	switch(DeviceRegister)
	{
		case VOLUME_REGISTER:
		{
			RegisterData = (URegisterData << 8) | LRegisterData;									// DAC 12 bit register data
			RegisterData = RegisterData >> 4;														// adjust bits
			tempfloat = (float)RegisterData / 4.095;												// convert to volume
			RegisterData = (UINT16) tempfloat;														// store volume
			RegisterData = 1000 - RegisterData;														// adjust for inverted control
			break;
		}
		case VOLUME_EEPROM:
		{
			RegisterData = (UEEPROMData << 8) | LEEPROMData;										// DAC 14 bit EEPROM data
			RegisterData = RegisterData & 0x0FFF;													// mask power down bits
			tempfloat = (float)RegisterData / 4.095;												// convert to volume
			RegisterData = (UINT16) tempfloat;														// store volume
			RegisterData = 1000 - RegisterData;														// adjust for inverted control
			break;
		}
		case VOLUME_CONTROL:
		{
			RegisterData = ControlData;																// DAC 8 bit control data
			break;
		}
	}

	return RegisterData;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void i2cRead_PIC18(void)
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

void i2cRead_PIC18(void)
{
	BYTE i2cbuffer[12];																				// generic I2C data buffer
	BYTE index = 0;
    unsigned int int_status;

	int_status = INTDisableInterrupts();															// disable all interrupts
	i2c2Write(i2cADDR_PIC_RTCC, i2cPIC_RTC_COMMAND, 1, RTCC_UPDATE_CODE, 1);						// send dummy command
	Delay_10uS(10);																					// delay
	for(index = 0; index < 12; index++)
	{
		StartI2C2();																				// send the restart condition
		IdleI2C2();																					// wait to complete
		MasterWriteI2C2(i2cADDR_PIC_RTCC | i2cREAD);												// transmit read command to device
		IdleI2C2();																					// wait to complete
		i2cbuffer[index] = MasterReadI2C2();														// read byte
		IdleI2C2();																					// wait to complete
		StopI2C2();																					// send the stop condition
		IdleI2C2();																					// wait to complete
		Delay_10uS(10);																				// delay
	}
	INTRestoreInterrupts(int_status);																// restore interrupt status

	lmi.settings.calendar.info.date.year = i2cbuffer[i2cPIC_RTC_YEAR];								// read the year from the PIC18
	lmi.settings.calendar.info.date.day = i2cbuffer[i2cPIC_RTC_DAY];								// read the day from the PIC18
	lmi.settings.calendar.info.date.month = i2cbuffer[i2cPIC_RTC_MONTH];							// read the month from the PIC18
	lmi.settings.clock.info.time.hours = i2cbuffer[i2cPIC_RTC_24HOUR];								// read the hours from the PIC18
	lmi.settings.clock.info.time.minutes = i2cbuffer[i2cPIC_RTC_MINUTE];							// read the minutes from the PIC18
	lmi.settings.clock.info.time.seconds = i2cbuffer[i2cPIC_RTC_SECOND];							// read the seconds from the PIC18

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//* LUDLUM MEASUREMENTS INC. COPYRIGHT (C) 2012
//**************************************************************************************************
//**************************************************************************************************
