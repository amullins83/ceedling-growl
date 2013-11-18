//**************************************************************************************************
//**************************************************************************************************
//*																		
//*	Filename:		i2cConfig.h
//*	Date:			06/04/10
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
//*	~~~~~~~~	05/28/10	James Comstock
//*	- continue where Clint left off
//*
//*	~~~~~~~~	12/18/09	Clint Appling
//*	- Original Prototype
//*                                                                     
//**************************************************************************************************
//**************************************************************************************************
//*	I2C Peripherals
//**************************************************************************************************
//*
//*	9DP Main Board Addresses:
//*
//*		U7 	- 24AA512 Serial EEPROM				-	0x51 [1010 001x]
//*		U8 	- 24AA512 Serial EEPROM				-	0x50 [1010 000x]
//*		U9	- MCP23008 I/O Expander				-	0x27 [0100 111x]
//*		U10 - AD7998-0 12-bit ADC				-	0x21 [0100 001x]
//*		U12	- MCP23008 I/O Expander				-	0x20 [0100 000x]
//*		U15 - PIC18F26J11 Supervisor			-	0x76 [1110 110x]
//*		U17	- MCP4725 DAC						-	0x60 [1100 000x]
//*
//*	Where x = R/W Command
//*
//**************************************************************************************************
//*
//* HV Board Addresses:
//*
//*	9DP -
//*		U6	- MAX5842LEUB 12-bit DAC			-	0x3D [0111 101x]
//*		U7	- MCP23008 I/O Expander				-	0x24 [0100 100x]
//*
//*	Basic -
//*		U9	- MAX5842LEUB 12-bit DAC			-	0x3D [0111 101x]
//*
//*	Advanced
//*		U1	- MAX5842LEUB 12-bit DAC			-	0x3D [0111 101x]
//*		U8	- MAX5842LEUB 12-bit DAC			-	0x3C [0111 100x]
//*
//*	Inter/Ext
//*		U1	- MAX5842LEUB 12-bit DAC			-	0x3D [0111 101x]
//*		U2	- MAX5842LEUB 12-bit DAC			-	0x3C [0111 100x]
//*
//*	Where x = R/W Command
//*
//**************************************************************************************************
//*
//* Electrometer Board Addresses:
//*
//*	9DP -
//*		U2	- MCP9800 Temperature Sensor		-	0x48 [1001 100x]
//*		U3	- SSCxxxxxxxxx						-	0x28 [0101 000x]
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

#ifndef _I2CCONFIG_H
#define _I2CCONFIG_H
#include "GenericTypeDefs.h"
//#define CCLK					(80000000UL)														// 80000000 80 Mhz clk using PLL (hidden Test v0.00.01)
#define CCLK					(72000000UL)														// 72000000 72 Mhz clk using PLL (Test v0.00.01)
#define CCLK_72MHz				(72000000UL)														// 72000000 72 Mhz clk using PLL (Test v0.00.01)
#define CCLK_80MHz				(80000000UL)														// 80000000 80 Mhz clk using PLL (Test v0.00.01)
#define PBCLK					(CCLK/8)															//
#define PBCLK_72MHz				(CCLK_72MHz/8)														// (9DP v1.02.00)
#define PBCLK_80MHz				(CCLK_80MHz/8)														// (9DP v1.02.00)
#define Fsck					(12750)																// 100kHz
#define BRG_VAL					((PBCLK/2/Fsck) - 2)												//
#define BRG_VAL_72MHz			((PBCLK_72MHz/2/Fsck) - 2)											// (9DP v1.02.00)
#define BRG_VAL_80MHz			((PBCLK_80MHz/2/Fsck) - 2)											// (9DP v1.02.00)

//**************************************************************************************************
//* I2C Device Addresses
//**************************************************************************************************

//**************************************************************************************************
//*	I/O Expanders (Main Board)

#define i2cADDR_MAIN_IO_BUTTON		0x40															// 0x20 << 1 (renamed 9DP v1.01.13)
#define i2cADDR_MAIN_IO_AUDIO		0x4E															// 0x27 << 1 (renamed 9DP v1.01.13)

//**************************************************************************************************
//*	ADC (Main Board)

#define i2cADDR_MAIN_ADC			0x46															// 0x23 << 1 (renamed 9DP v1.01.13)
#define ADC_PORT0_SIGNAL1			0x80
#define ADC_PORT1_SIGNAL2			0x90
#define ADC_PORT2_HV_REF			0xA0
#define ADC_PORT3_BATTERY			0xB0
#define ADC_PORT4_DISPLAY			0xC0
#define ADC_PORT5_EXTRA1			0xD0
#define ADC_PORT6_EXTRA2			0xE0
#define ADC_PORT7_LIGHT				0xF0

//**************************************************************************************************
//*	EEPROM Memory (Main Board)

#define i2cADDR_MAIN_E2P_PRI		0xA2															// 0x51 << 1
#define i2cADDR_MAIN_E2P_SEC		0xA0															// 0x50 << 1

//**************************************************************************************************
//*	DAC (Main Board)

#define i2cADDR_MAIN_DAC			0xC2    														// 0x61 << 1 (renamed 9DP v1.01.13)
#define VOLUME_CONTROL				0x00
#define VOLUME_REGISTER				0x40
#define VOLUME_EEPROM				0x60

//**************************************************************************************************
//*	I/O Expanders (High Voltage Board)

#define i2cADDR_HV_IO_0x48			0x48															// 0x24 << 1 (renamed 9DP v1.01.13)

//**************************************************************************************************
//*	DAC (High Voltage Board)

#define i2cADDR_HV_DAC_0xC0			0xC0															// 0x60 << 1 (renamed 9DP v1.01.13)

//**************************************************************************************************
//*	Temperature Sensor (9DP Electrometer Board, other instruments pending)

#define i2cADDR_X_TEMP_0x90			0x90															// 0x48 << 1

//**************************************************************************************************
//*	Pressure Sensor (9Dx chamber)

#define i2cADDR_X_PRES_0x50			(0x28 * 0x02)													// 0x28 << 1 (9DP v1.02.03)

//**************************************************************************************************
//*	PIC Supervisor/RTC

#define i2cADDR_PIC_RTCC			0xEC															// 0x76 << 1

//**************************************************************************************************
//*	Register locations on the PIC18 RTCC Chip

#define i2cPIC_RTC_COMMAND			0x00															// command register
#define i2cPIC_RTC_SYSTEM			0x01															// status register
#define i2cPIC_RTC_STATUS			0x02															// system register
#define i2cPIC_RTC_YEAR				0x03															// current year
#define i2cPIC_RTC_YEAR_DAY			0x04															// current year day
#define i2cPIC_RTC_DAY				0x05															// current day
#define i2cPIC_RTC_MONTH			0x06															// current month
#define i2cPIC_RTC_24HOUR			0x07															// current time (24 military time)
#define i2cPIC_RTC_WEEKDAY			0x08															// current day of the week
#define i2cPIC_RTC_SECOND			0x09															// current seconds
#define i2cPIC_RTC_MINUTE			0x0A															// current minute

#define RTCC_UPDATE_CODE			0x55															// code for PIC18 to update RTCC
#define SHUTDOWN_CODE				0x99															// code for PIC18 to turn off power

//#define i2cADDR_SUPERVISOR_A10	0xD0	

//**************************************************************************************************
//	Port Bits

#define ADC_ALERT					PORTBbits.RB0													// Alert input pin
#define ADC_CONVST_BUSY				PORTFbits.RF12													// ADC Busy

//*
//**************************************************************************************************
//*	MCP9800AOT-M/OTG - 2 Wire High Accuracy Temperature Sensor
//*
//*		Vdd		- I/O controlled +3.3V
//*		SDA		- SDA2_i2c
//*		SCL		- SCL2_i2c
//*		ALERT	- N/C
//*		GND		- GND
//*
//*	Address Byte:
//*
//*		bits 7-4 Address Code
//*			1001 = Fixed
//*		bits 3-1 Slave Address
//*			000 = Address 0
//*			001 = Address 1
//*			010 = Address 2
//*			011 = Address 3
//*			100 = Address 4
//*			101 = Address 5
//*			110 = Address 6
//*			111 = Address 7
//*		bit 0 Read/Write
//*			1 = Read
//*			0 = Write
//*
//*	U2 - MCP9800A0	=	0x90
//*
//**************************************************************************************************
//*	Register Byte:
//*
//*		bits 7-2 Unimplemented
//*			000000 = Fixed
//*		bits 1-0 Pointer Bits
//*			00 = Temperature Register
//*			01 = Configuration Register
//*			10 = Temperature Hysteresis Register
//*			11 = Temperature Limit-set Register
//*

#define TEMP_REG			0x00
#define CONFIG_REG			0x01
#define TEMP_HYST_REG		0x02
#define TEMP_LS_REG			0x03

//*
//**************************************************************************************************
//*	Configuration Byte:
//*
//*		bit 7 ONE-SHOT bit
//*			1 = Enabled
//*			0 = Disabled (Power-up default)
//*		bits 6-5 S? ADC RESOLUTION bit
//*			00 = 9 bit (Power-up default)
//*			01 = 10 bit
//*			10 = 11 bit
//*			11 = 12 bit
//*		bits 4-3 FAULT QUEUE bit
//*			00 = 1 (Power-up default)
//*			01 = 2
//*			10 = 4
//*			11 = 6
//*		bit 2 ALERT POLARITY bit
//*			1 = Active-High
//*			0 = Active-Low (Power-up default)
//*		bit 1 COMP/INT bit
//*			1 = Interrupt Mode
//*			0 = Comparator Mode (Power-up default)
//*		bit 0 SHUTDOWN bit
//*			1 = Enable
//*			0 = Disable (Power-up default)
//*

#define ONESHOT_ENABLED		0x80
#define ONESHOT_DISABLED	0x00
#define ADC_9BIT			0x00
#define ADC_10BIT			0x20
#define ADC_11BIT			0x40
#define ADC_12BIT			0x60
#define FAULT_QUEUE_1		0x00
#define FAULT_QUEUE_2		0x08
#define FAULT_QUEUE_4		0x10
#define FAULT_QUEUE_6		0x18
#define ALERT_POL_HI		0x04
#define ALERT_POL_LO		0x00
#define INT_MODE			0x02
#define COMP_MODE			0x00
#define SHUTDOWN_ENABLED	0x01
#define SHUTDOWN_DISABLED	0x00

//*
//**************************************************************************************************
//*	Temperature Resolution and Conversion:
//*
//*		Bits		Resolution			Conversion Time
//*		9			0.5C					30ms
//*		10			0.25C					60ms
//*		11			0.125C					120ms
//*		12			0.0625C					240ms
//*

#define TEMP_CONST_9BIT		0.5000
#define TEMP_CONST_10BIT	0.2500
#define TEMP_CONST_11BIT	0.1250
#define TEMP_CONST_12BIT	0.0625

//*
//**************************************************************************************************
//*

void setupi2c2_peripherals(void);
void setupi2c2_main_adc(void);
void setupi2c2_main_dac(void);
void setupi2c2_main_io_audio(void);
void setupi2c2_main_io_buttons(void);
void setupi2c2_hv_io_0x48(void);
void setupi2c2_hv_dac_0xc0(void);
void setupi2c2_x_temp_0x90(void);
void setupi2c2_x_pres_0x28(void);																	// (v1.02.03)

void i2cRead_PIC18(void);

UINT16 getVolume_i2c(UINT8 DeviceAddress, UINT8 DeviceRegister);
void setVolume_i2c(UINT8 DeviceAddress, UINT8 DeviceRegister, UINT16 volume);

#endif

