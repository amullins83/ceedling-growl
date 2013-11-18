//**************************************************************************************************
//**************************************************************************************************
//*																		
//*	Filename:		EEPROM_Functions.c
//*	Date:			05/19/10
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
//* n2          09/13/13    Austin Mullins
//* - Added diagnostic helper functions
//*
//*	~~~~~~~~	06/17/10	James Comstock
//*	- continue where Clint left FALSE
//*
//*	~~~~~~~~	05/15/10	Clint Appling
//*	- Original Prototype
//*                                                                     
//**************************************************************************************************
//*                                                                     
//*	EEPROM Data Storage Routines
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
//*	solely and exclusively TRUE Microchip PICmicro Microcontroller products.
//*
//* The software is owned by the Company and/or its supplier, and is protected under applicable
//*	copyright laws. All rights are reserved. Any use in violation of the foregoing restrictions may
//*	subject the user to criminal sanctions under applicable laws, as well as to civil liability for
//*	the breach of the terms and conditions of this license.
//*
//* THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR
//*	STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//*	A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT, IN ANY CIRCUMSTANCES, BE
//*	LIABLE FOR SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES, FOR ANY REASTRUE WHATSOEVER.
//*
//**************************************************************************************************

#include "instrument.h"

//**************************************************************************************************
//**************************************************************************************************
//*	Data Structures
//**************************************************************************************************
//**************************************************************************************************

unsigned char test = 0;

//**************************************************************************************************
//**************************************************************************************************
//*	Variable Definitions
//**************************************************************************************************
//**************************************************************************************************

extern const FONT_FLASH Secondary_Arial_Black_14;

//**************************************************************************************************
//*	External - 1-Bit Variables

extern BOOL Update_Log_File;
extern BOOL Power_Up_Report_Integrate;
extern BOOL Power_Down_Report_Integrate;
extern BOOL Range_Lock;
extern BOOL First_System_Initialization;
extern BOOL Restart_System;																			// (9DP v1.01.12)

//**************************************************************************************************
//*	External - 8-Bit Variables

extern BYTE Current_Detector;
extern BYTE Current_View;																			// Current View (for easy coding)
extern BYTE Current_Function_Index;																	// Current Function (for easy coding)
extern BYTE Backlight_Mode;
extern BYTE Audio_Operating_Mode;
extern BYTE Default_Dose_Units;																		// (9DP v1.01.10)

//**************************************************************************************************
//*	External - 16-Bit Variables

extern WORD foreground;																				// Screen foreground color
extern WORD background;																				// Screen background color
//extern SHORT Default_Rate_Multiplier;
//extern SHORT Default_Dose_Multiplier;																// (9DP v1.01.10)

//**************************************************************************************************
//*	External - 32-Bit Variables

extern DWORD tick;										 											// tick counter
extern DWORD log_file_write_tick;																	// log file write tick timer
extern DWORD integrate_update_tick;																	// integrate update tick timer

//**************************************************************************************************
//*	External - Float Variables

extern float Rate_Accumulation;
extern float uR_filter;																				// range 1, uR/hr filter

//**************************************************************************************************
//*	External - Structures

extern TYPE_INSTRUMENT lmi;



//**************************************************************************************************
//**************************************************************************************************
//*	EEPROM Process Functions
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void EEPROM_Clear(UINT8 EEPROMchipAddress)
//*
//*	Description:
//* 	This routine will clear all of the memory locations in the selected EEPROM.
//*
//*	Precondition:
//* 	NTRUEe
//*
//*	Parameters:
//* 	EEPROMchipAddress - I2C address of the EEPROM
//*
//*	Return Values:
//* 	NTRUEe
//*
//*	Remarks:
//* 	Total memory space : 		0x10000 (65536)
//*

void EEPROM_Clear(UINT8 EEPROMchipAddress)
{
	DWORD address;
	UINT8 page_size = 0x80;

	for(address = 0x0000; address < 0x10000; address += page_size)
	{
		i2c2ClearPage(EEPROMchipAddress, (UINT16) address, 2, page_size);
	}

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void EEPROM_Transfer(UINT8 FromEEPROMchipAddress, UINT8 ToEEPROMchipAddress)
//*
//*	Description:
//*
//*	Precondition:
//* 	NTRUEe
//*
//*	Parameters:
//* 	FromEEPROMchipAddress - I2C address of the transfer EEPROM
//*		ToEEPROMchipAddress - I2C address of the receive EEPROM
//*
//*	Return Values:
//* 	NTRUEe
//*
//*	Remarks:
//* 	Total memory space : 		0x10000 (65536)
//*

void EEPROM_Transfer(UINT8 FromEEPROMchipAddress, UINT8 ToEEPROMchipAddress)
{
	DWORD address;
	UINT8 page_size = 0x80;
	char eeprom_data_string[0x80];

	EEPROM_Clear(ToEEPROMchipAddress);

	for(address = 0x0000; address < 0x10000; address += page_size)
	{
		i2c2ReadPage(FromEEPROMchipAddress, address, 2, eeprom_data_string, page_size);
		i2c2WritePage(ToEEPROMchipAddress, address, 2, eeprom_data_string, page_size);
	}

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void EEPROM_Initialize(BOOL wipe)
//*
//*	Description:
//* 	This routine will determine the status of the EEPROM and will perform TRUEe of the following:
//*			-	load settings from the primary EEPROM
//*			-	restore the primary EEPROM from the secondary EEPROM and load settings
//*			-	erase the primary and secondary EEPROM and load default settings
//*
//*	Precondition:
//* 	NTRUEe
//*
//*	Parameters:
//* 	wipe - force erasing of primary and secondary EEPROM
//*
//*	Return Values:
//* 	NTRUEe
//*
//*	Remarks:
//* 	NTRUEe
//*

void EEPROM_Initialize(BOOL wipe)
{
	UINT8 compare_firmware = 1;
	BYTE char_buffer[25] = {0};
	XCHAR xchar_buffer[25] = {0};
	BYTE eeprom_data_string[256];
    unsigned int int_status;

	int_status = INTDisableInterrupts();
	i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, EEPROM_SETTINGS_MISC_LOCATION, 2, lmi.settings.misc.buffer, SIZE_SETTINGS_MISC);
	compare_firmware = strncmp(FIRMWARE_NUMBER_VALUE, lmi.settings.misc.info.firmware, LENGTH_FIRMWARE_NUMBER);
	
	if((compare_firmware != 0) || (wipe == TRUE))
	{
		compare_firmware = strncmp(FIRMWARE_NUMBER_VALUE, lmi.settings.misc.info.firmware, 5);

		if((compare_firmware != 0) || (wipe == TRUE))
		{
			DisplaySystemProcess(1);
			EEPROM_Clear(i2cADDR_MAIN_E2P_PRI);
			DisplaySystemProcess(0);
			i2c2ReadPage(i2cADDR_MAIN_E2P_SEC, EEPROM_SETTINGS_MISC_LOCATION, 2, eeprom_data_string, SIZE_SETTINGS_MISC);
			compare_firmware = strncmp(FIRMWARE_NUMBER_VALUE, eeprom_data_string, 5);
			if((compare_firmware != 0) || (wipe == TRUE))
			{
				DisplaySystemProcess(2);
				EEPROM_Clear(i2cADDR_MAIN_E2P_SEC);
				DisplaySystemProcess(0);
				EEPROM_Load_System_Defaults();
				EEPROM_Load_Calibration_Defaults();
				First_System_Initialization = TRUE;
			}
			else
			{
				DisplaySystemProcess(4);
				EEPROM_Transfer(i2cADDR_MAIN_E2P_SEC, i2cADDR_MAIN_E2P_PRI);
				Software_Reset();
			}
		}
		else
		{
			strncpy(lmi.settings.misc.info.firmware, FIRMWARE_NUMBER_VALUE, 16);
			i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_SETTINGS_MISC_LOCATION, 2, lmi.settings.misc.buffer, SIZE_SETTINGS_MISC);
			EEPROM_Read_System_Data();
			EEPROM_Read_Calibration_Data();
		}
	}
	else
	{
		EEPROM_Read_System_Data();
		EEPROM_Read_Calibration_Data();
	}

	StructureInit();																				//* Initialize previously uninitialized structure variables
	EEPROM_Set_Modes();
	EEPROM_Assign_Global_Variables();
	UpdateFunctionVariables();
	INTRestoreInterrupts(int_status);
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void EEPROM_Read_System_Data(void)
//*
//*	Description:
//* 	Read all of the EEPROM stored parameters
//*
//*	Precondition:
//* 	NTRUEe
//*
//*	Parameters:
//* 	NTRUEe
//*
//*	Return Values:
//* 	NTRUEe
//*
//*	Remarks:
//* 	Checksum recovery to be added at a later date.
//*

void EEPROM_Read_System_Data(void)
{
	unsigned char detector_index;
	unsigned char view_index;
	unsigned char index;
	unsigned short address;
	unsigned char checksum = 0;

//*	Settings (EEPROM_SETTINGS_MISC_LOCATION has been read before thie routine is called)
	i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, EEPROM_PRODUCT_LOCATION, 2, (BYTE *)lmi.settings.product.buffer, SIZE_PRODUCT);
	i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, EEPROM_LANGUAGE_LOCATION, 2, (BYTE *)lmi.settings.language.buffer, SIZE_LANGUAGE);
	i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, EEPROM_CALENDAR_LOCATION, 2, (BYTE *)lmi.settings.calendar.buffer, SIZE_DATE);
	i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, EEPROM_CLOCK_LOCATION, 2, (BYTE *)lmi.settings.clock.buffer, SIZE_TIME);
	i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, EEPROM_PASSWORDS_LOCATION, 2, (BYTE *)lmi.settings.passwords.buffer, SIZE_PASSWORDS);

//* Features
	i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, EEPROM_SOURCE_CHECK_LOCATION, 2, (BYTE *)lmi.features.source.buffer, SIZE_SOURCE_CHECK);
	i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, EEPROM_BACKGROUND_CHECK_LOCATION, 2, (BYTE *)lmi.features.background.buffer, SIZE_BACKGROUND_CHECK);
	i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, EEPROM_INTEGRATE_LOCATION, 2, (BYTE *)lmi.features.integrate.buffer, SIZE_INTEGRATE);
	i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, EEPROM_DATA_LOGGING_LOCATION, 2, (BYTE *)lmi.features.data_logging.buffer, SIZE_DATA_LOGGING);

//* CONtrols
	i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, EEPROM_BACKLIGHT_LOCATION, 2, (BYTE *)lmi.controls.backlight.buffer, SIZE_BACKLIGHT);
	i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, EEPROM_AUDIO_LOCATION, 2, (BYTE *)lmi.controls.audio.buffer, SIZE_AUDIO);
	i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, EEPROM_GPS_LOCATION, 2, (BYTE *)lmi.controls.gps.buffer, SIZE_GPS);
	i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, EEPROM_BLUETOOTH_LOCATION, 2, (BYTE *)lmi.controls.bluetooth.buffer, SIZE_BLUETOOTH);

//* Display
#ifdef EEPROM_DISPLAY_PROPERTIES_LOCATION
	i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, EEPROM_DISPLAY_PROPERTIES_LOCATION, 2, (BYTE *)lmi.display.properties.buffer, SIZE_DISPLAY_PROPERTIES);
#endif

//* Display - Detectors
	for(detector_index = 0; detector_index < NUMBER_OF_DETECTORS; detector_index++)
	{
#ifdef EEPROM_DETECTOR_PROPERTIES_LOCATION
		address = EEPROM_DETECTOR_PROPERTIES_LOCATION + (detector_index * TOTAL_DETECTOR);
		i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].properties.buffer, SIZE_DETECTOR_PROPERTIES);
#endif
#ifdef EEPROM_DETECTOR_CALIBRATION_LOCATION
		address = EEPROM_DETECTOR_CALIBRATION_LOCATION + (detector_index * TOTAL_DETECTOR);
		i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].calibration.buffer, SIZE_DETECTOR_CALIBRATION);
#endif
#ifdef EEPROM_DETECTOR_VIEW_SETUP_LOCATION
		address = EEPROM_DETECTOR_VIEW_SETUP_LOCATION + (detector_index * TOTAL_DETECTOR);
		i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].view_setup.buffer, SIZE_DETECTOR_VIEW_SETUP);
#endif
		for (view_index = 0; view_index < NUMBER_OF_VIEWS; view_index++)
		{
			address = EEPROM_APPEARANCE_LOCATION + (detector_index * TOTAL_DETECTOR) + (view_index * TOTAL_VIEW);
			i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].view[view_index].appearance.buffer, SIZE_APPEARANCE);
			address = EEPROM_ELEMENT_HEADER_LOCATION + (detector_index * TOTAL_DETECTOR) + (view_index * TOTAL_VIEW);
			i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].view[view_index].element_header.buffer, SIZE_ELEMENT_HEADER);
			address = EEPROM_ELEMENT_LOCATION + (detector_index * TOTAL_DETECTOR) + (view_index * TOTAL_VIEW);
			i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].view[view_index].element.buffer, SIZE_ELEMENT);
			address = EEPROM_VIEW_FUNCTIONS_LOCATION + (detector_index * TOTAL_DETECTOR) + (view_index * TOTAL_VIEW);
			i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].view[view_index].functions.buffer, SIZE_VIEW_FUNCTIONS);
			address = EEPROM_ALERTS_RADIATION_LOCATION + (detector_index * TOTAL_DETECTOR) + (view_index * TOTAL_VIEW);
			i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].view[view_index].alerts_radiation.buffer, SIZE_ALERTS_RADIATION);
			address = EEPROM_ALERTS_INTEGRATE_LOCATION + (detector_index * TOTAL_DETECTOR) + (view_index * TOTAL_VIEW);
			i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].view[view_index].alerts_integrate.buffer, SIZE_ALERTS_INTEGRATE);
			address = EEPROM_ALERTS_FUNCTION_LOCATION + (detector_index * TOTAL_DETECTOR) + (view_index * TOTAL_VIEW);
			i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].view[view_index].alerts_function.buffer, SIZE_ALERTS_FUNCTION);
			address = EEPROM_ALARMS_RADIATION_LOCATION + (detector_index * TOTAL_DETECTOR) + (view_index * TOTAL_VIEW);
			i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].view[view_index].alarms_radiation.buffer, SIZE_ALARMS_RADIATION);
			address = EEPROM_ALARMS_INTEGRATE_LOCATION + (detector_index * TOTAL_DETECTOR) + (view_index * TOTAL_VIEW);
			i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].view[view_index].alarms_integrate.buffer, SIZE_ALARMS_INTEGRATE);
			address = EEPROM_ALARMS_FUNCTION_LOCATION + (detector_index * TOTAL_DETECTOR) + (view_index * TOTAL_VIEW);
			i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].view[view_index].alarms_function.buffer, SIZE_ALARMS_FUNCTION);
//* added (3D v1.00.03)
#ifdef EEPROM_DETECTOR_VIEW_CALIBRATION_LOCATION
			address = EEPROM_VIEW_CALIBRATION_LOCATION + (detector_index * TOTAL_DETECTOR) + (view_index * TOTAL_VIEW);
			i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].view[view_index].calibration.buffer, SIZE_VIEW_CALIBRATION);
#endif
//* end add
		}
	}

//* Functions
	for (index = 0; index < NUMBER_OF_FUNCTIONS; index++)
	{
		address = EEPROM_FUNCTIONS_LOCATION + (index * TOTAL_FUNCTION);
		i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.function[index].buffer, SIZE_FUNCTION);
	}

//*	Alerts
	i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALERTS_LOW_BATTERY_LOCATION, 2, (BYTE *)lmi.alerts.low_battery.buffer, SIZE_ALERTS_LOW_BATTERY);
	i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALERTS_LOW_MEMORY_LOCATION, 2, (BYTE *)lmi.alerts.low_memory.buffer, SIZE_ALERTS_LOW_MEMORY);
	i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALERTS_CALIBRATION_DUE_LOCATION, 2, (BYTE *)lmi.alerts.calibration_due.buffer, SIZE_ALERTS_CALIBRATION_DUE);
	i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALERTS_SOURCE_CHECK_LOCATION, 2, (BYTE *)lmi.alerts.source_check.buffer, SIZE_ALERTS_SOURCE_CHECK);
	i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALERTS_BACKGROUND_CHECK_LOCATION, 2, (BYTE *)lmi.alerts.background_check.buffer, SIZE_ALERTS_BACKGROUND_CHECK);
	
//*	Alarms
	i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALARMS_LOW_BATTERY_LOCATION, 2, (BYTE *)lmi.alarms.low_battery.buffer, SIZE_ALARMS_LOW_BATTERY);
	i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALARMS_LOW_MEMORY_LOCATION, 2, (BYTE *)lmi.alarms.low_memory.buffer, SIZE_ALARMS_LOW_MEMORY);
	i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALARMS_CALIBRATION_DUE_LOCATION, 2, (BYTE *)lmi.alarms.calibration_due.buffer, SIZE_ALARMS_CALIBRATION_DUE);
	i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALARMS_SOURCE_CHECK_LOCATION, 2, (BYTE *)lmi.alarms.source_check.buffer, SIZE_ALARMS_SOURCE_CHECK);
	i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALARMS_BACKGROUND_CHECK_LOCATION, 2, (BYTE *)lmi.alarms.background_check.buffer, SIZE_ALARMS_BACKGROUND_CHECK);
	i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALARMS_HV_LOW_LOCATION, 2, (BYTE *)lmi.alarms.hv_low.buffer, SIZE_ALARMS_HV_LOW);
	i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALARMS_HV_HIGH_LOCATION, 2, (BYTE *)lmi.alarms.hv_high.buffer, SIZE_ALARMS_HV_HIGH);
	i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALARMS_LOW_READING_LOCATION, 2, (BYTE *)lmi.alarms.low_reading.buffer, SIZE_ALARMS_LOW_READING);

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void EEPROM_Read_Calibration_Data(void)
//*
//*	Description:
//* 	
//*
//*	Precondition:
//* 	NTRUEe
//*
//*	Parameters:
//* 	NTRUEe
//*
//*	Return Values:
//* 	NTRUEe
//*
//*	Remarks:
//* 	NTRUEe
//*

void EEPROM_Read_Calibration_Data(void)
{
	i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, EEPROM_CALIBRATION_HEADER_LOCATION, 2, (BYTE *)lmi.calibration.header.buffer, SIZE_CALIBRATION_HEADER);
	i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, EEPROM_CALIBRATION_CHECKOUT_LOCATION, 2, (BYTE *)lmi.calibration.checkout.buffer, SIZE_CALIBRATION_CHECKOUT);
	i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, EEPROM_CALIBRATION_CONSTANTS_LOCATION, 2, (BYTE *)lmi.calibration.constants.buffer, SIZE_CALIBRATION_CONSTANTS);
	i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, EEPROM_CALIBRATION_OFFSETS_LOCATION, 2, (BYTE *)lmi.calibration.offsets.buffer, SIZE_CALIBRATION_OFFSETS);
	i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, EEPROM_CALIBRATION_HVB_LOCATION, 2, (BYTE *)lmi.calibration.high_voltage_board.buffer, SIZE_CALIBRATION_HVB);
	i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, EEPROM_CALIBRATION_CONTROLS_LOCATION, 2, (BYTE *)lmi.calibration.controls.buffer, SIZE_CALIBRATION_CONTROLS);
	i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, EEPROM_CALIBRATION_CONFIG_LOCATION, 2, (BYTE *)lmi.calibration.config.buffer, SIZE_CALIBRATION_CONFIG);
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void EEPROM_Load_System_Defaults(void)
//*
//*	Description:
//*		Writes default values to store to EEPROM if TRUE new startup or if the EEPROM is corrupted.
//*
//*	Precondition:
//* 	NTRUEe
//*
//*	Parameters:
//* 	NTRUEe
//*
//*	Return Values:
//* 	NTRUEe
//*
//*	Remarks:
//* 	NTRUEe
//*

void EEPROM_Load_System_Defaults(void)
{
	unsigned char detector_index;
	unsigned char view_index;
	unsigned char index;
	unsigned char counter;
	unsigned short address;
	unsigned char address_index = 0;
	BYTE read_memory[128];

//*	Settings - Product Info
	strncpy(lmi.settings.product.info.model, DEFAULT_MODEL, LENGTH_MODEL);
	strncpy(lmi.settings.product.info.serial_number, DEFAULT_SERIAL_NUMBER, LENGTH_SERIAL_NUMBER);
	strncpy(lmi.settings.product.info.user_id_number, DEFAULT_USER_ID_NUMBER, LENGTH_USER_ID_NUMBER);
	strncpy(lmi.settings.product.info.user_serial_number, DEFAULT_USER_SERIAL_NUMBER, LENGTH_USER_SERIAL_NUMBER);
	lmi.settings.product.info.calibration_date.month = DEFAULT_DATE_MONTH;
	lmi.settings.product.info.calibration_date.day = DEFAULT_DATE_DAY;
	lmi.settings.product.info.calibration_date.year = DEFAULT_DATE_YEAR;
	lmi.settings.product.info.calibration_due_date.month = DEFAULT_DATE_MONTH;
	lmi.settings.product.info.calibration_due_date.day = DEFAULT_DATE_DAY;
	lmi.settings.product.info.calibration_due_date.year = DEFAULT_DATE_YEAR;
	lmi.settings.product.info.model_number = DEFAULT_MODEL_NUMBER;
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_PRODUCT_LOCATION, 2, (BYTE *)lmi.settings.product.buffer, SIZE_PRODUCT);

//*	Settings - Language Info
	lmi.settings.language.info.index = DEFAULT_LANGUAGE_INDEX;
	strncpy(lmi.settings.language.info.string, DEFAULT_LANGUAGE_NAME, LENGTH_LANGUAGE_STRING);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_LANGUAGE_LOCATION, 2, (BYTE *)lmi.settings.language.buffer, SIZE_LANGUAGE);

//* Settings - Calendar Info
	lmi.settings.calendar.info.date.month = DEFAULT_DATE_MONTH;
	lmi.settings.calendar.info.date.day = DEFAULT_DATE_DAY;
	lmi.settings.calendar.info.date.year = DEFAULT_DATE_YEAR;
	strncpy(lmi.settings.calendar.info.string, DEFAULT_DATE_STRING, LENGTH_DATE_STRING);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_CALENDAR_LOCATION, 2, (BYTE *)lmi.settings.calendar.buffer, SIZE_DATE);

//* Settings - Clock Info
	lmi.settings.clock.info.time.hours = DEFAULT_TIME_HOURS;
	lmi.settings.clock.info.time.minutes = DEFAULT_TIME_MINUTES;
	lmi.settings.clock.info.time.seconds = DEFAULT_TIME_SECONDS;
	lmi.settings.clock.info.time.format = DEFAULT_TIME_FORMAT;
	strncpy(lmi.settings.clock.info.string, DEFAULT_TIME_STRING, LENGTH_TIME_STRING);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_CLOCK_LOCATION, 2, (BYTE *)lmi.settings.clock.buffer, SIZE_TIME);

//*	Settings - Password Info
	lmi.settings.passwords.info.enable = DEFAULT_PASSWORD_ENABLE;
	strncpy(lmi.settings.passwords.info.password_1, DEFAULT_PASSWORD_1, LENGTH_PASSWORD);
	strncpy(lmi.settings.passwords.info.password_2, DEFAULT_PASSWORD_2, LENGTH_PASSWORD);
	strncpy(lmi.settings.passwords.info.password_3, DEFAULT_PASSWORD_3, LENGTH_PASSWORD);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_PASSWORDS_LOCATION, 2, (BYTE *)lmi.settings.passwords.buffer, SIZE_PASSWORDS);

//* Settings - Misc Info
	strncpy(lmi.settings.misc.info.firmware, FIRMWARE_NUMBER_VALUE, 16);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_SETTINGS_MISC_LOCATION, 2, (BYTE *)lmi.settings.misc.buffer, SIZE_SETTINGS_MISC);

//* Features - Source Check
	lmi.features.source.info.powerup_mode = DEFAULT_SOURCE_POWERUP_MODE;
	lmi.features.source.info.operating_mode = DEFAULT_SOURCE_OPERATING_MODE;
	lmi.features.source.info.delay = DEFAULT_SOURCE_DELAY;
	lmi.features.source.info.log_result = DEFAULT_SOURCE_LOG_RESULT;
	lmi.features.source.info.sample_time = DEFAULT_SOURCE_SAMPLE_TIME;
	lmi.features.source.info.initial_reading.value = 0.0;
	lmi.features.source.info.initial_reading.units = DEFAULT_SOURCE_UNITS;
	lmi.features.source.info.initial_reading.multiplier = DEFAULT_SOURCE_MULTIPLIER;
	lmi.features.source.info.initial_date.month = 0;
	lmi.features.source.info.initial_date.day = 0;
	lmi.features.source.info.initial_date.year = 0;
	lmi.features.source.info.initial_time.hours = 0;
	lmi.features.source.info.initial_time.minutes = 0;
	lmi.features.source.info.initial_time.format = 0;
	lmi.features.source.info.last_reading.value = 0.0;
	lmi.features.source.info.last_reading.units = DEFAULT_SOURCE_UNITS;
	lmi.features.source.info.last_reading.multiplier = DEFAULT_SOURCE_MULTIPLIER;
	lmi.features.source.info.last_date.month = 0;
	lmi.features.source.info.last_date.day = 0;
	lmi.features.source.info.last_date.year = 0;
	lmi.features.source.info.last_time.hours = 0;
	lmi.features.source.info.last_time.minutes = 0;
	lmi.features.source.info.last_time.format = 0;
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_SOURCE_CHECK_LOCATION, 2, (BYTE *)lmi.features.source.buffer, SIZE_SOURCE_CHECK);

//* Features - Background Check
	lmi.features.background.info.powerup_mode = DEFAULT_BACKGROUND_POWERUP_MODE;
	lmi.features.background.info.operating_mode = DEFAULT_BACKGROUND_OPERATING_MODE;
	lmi.features.background.info.delay = DEFAULT_BACKGROUND_DELAY;
	lmi.features.background.info.log_result = DEFAULT_BACKGROUND_LOG_RESULT;
	lmi.features.background.info.sample_time = DEFAULT_BACKGROUND_SAMPLE_TIME;
	lmi.features.background.info.initial_reading.value = 0.0;
	lmi.features.background.info.initial_reading.units = DEFAULT_BACKGROUND_UNITS;
	lmi.features.background.info.initial_reading.multiplier = DEFAULT_BACKGROUND_MULTIPLIER;
	lmi.features.background.info.initial_date.month = 0;
	lmi.features.background.info.initial_date.day = 0;
	lmi.features.background.info.initial_date.year = 0;
	lmi.features.background.info.initial_time.hours = 0;
	lmi.features.background.info.initial_time.minutes = 0;
	lmi.features.background.info.initial_time.format = 0;
	lmi.features.background.info.last_reading.value = 0.0;
	lmi.features.background.info.last_reading.units = DEFAULT_BACKGROUND_UNITS;
	lmi.features.background.info.last_reading.multiplier = DEFAULT_BACKGROUND_MULTIPLIER;
	lmi.features.background.info.last_date.month = 0;
	lmi.features.background.info.last_date.day = 0;
	lmi.features.background.info.last_date.year = 0;
	lmi.features.background.info.last_time.hours = 0;
	lmi.features.background.info.last_time.minutes = 0;
	lmi.features.background.info.last_time.format = 0;
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_BACKGROUND_CHECK_LOCATION, 2, (BYTE *)lmi.features.background.buffer, SIZE_BACKGROUND_CHECK);

// Features - Integrate CONtinuious
	lmi.features.integrate.info.powerup_mode = DEFAULT_INTEGRATE_POWERUP_MODE;
	lmi.features.integrate.info.operating_mode = DEFAULT_INTEGRATE_OPERATING_MODE;
	lmi.features.integrate.info.delay = DEFAULT_INTEGRATE_DELAY;
	lmi.features.integrate.info.startup_log = DEFAULT_INTEGRATE_LOG_RESULT;
	lmi.features.integrate.info.shutdown_log = DEFAULT_INTEGRATE_LOG_SHUTDOWN;
	lmi.features.integrate.info.save_on_shutdown = DEFAULT_INTEGRATE_SAVE_ON_SHUTDOWN;
	lmi.features.integrate.info.current_reading.value = DEFAULT_INTEGRATE_VALUE;
	lmi.features.integrate.info.current_reading.units = DEFAULT_INTEGRATE_UNITS;
	lmi.features.integrate.info.current_reading.multiplier = DEFAULT_INTEGRATE_MULTIPLIER;
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_INTEGRATE_LOCATION, 2, (BYTE *)lmi.features.integrate.buffer, SIZE_INTEGRATE);

// Features - Data Logging
	lmi.features.data_logging.info.powerup_mode = DEFAULT_DATA_LOGGING_POWERUP_MODE;
	lmi.features.data_logging.info.operating_mode = DEFAULT_DATA_LOGGING_OPERATING_MODE;
	lmi.features.data_logging.info.delay = DEFAULT_DATA_LOGGING_DELAY;
	lmi.features.data_logging.info.sample_period = DEFAULT_DATA_LOGGING_SAMPLE_PERIOD;
	lmi.features.data_logging.info.last_reading.value = 0.0;
	lmi.features.data_logging.info.last_reading.units = DEFAULT_DATA_LOGGING_UNITS;
	lmi.features.data_logging.info.last_reading.multiplier = DEFAULT_DATA_LOGGING_MULTIPLIER;
	lmi.features.data_logging.info.last_date.month = DEFAULT_DATE_MONTH;
	lmi.features.data_logging.info.last_date.day = DEFAULT_DATE_DAY;
	lmi.features.data_logging.info.last_date.year = DEFAULT_DATE_YEAR;
	lmi.features.data_logging.info.last_time.hours = DEFAULT_TIME_HOURS;
	lmi.features.data_logging.info.last_time.minutes = DEFAULT_TIME_MINUTES;
	lmi.features.data_logging.info.last_time.seconds = DEFAULT_TIME_SECONDS;
	lmi.features.data_logging.info.last_time.format = DEFAULT_TIME_FORMAT;
	lmi.features.data_logging.info.reported_data.model_number = DEFAULT_DATA_LOGGING_EVENT;
	lmi.features.data_logging.info.reported_data.serial_number = DEFAULT_DATA_LOGGING_EVENT;
	lmi.features.data_logging.info.reported_data.user_id_number = DEFAULT_DATA_LOGGING_EVENT;
	lmi.features.data_logging.info.reported_data.user_serial_number = DEFAULT_DATA_LOGGING_EVENT;
	lmi.features.data_logging.info.reported_data.date = DEFAULT_DATA_LOGGING_EVENT;
	lmi.features.data_logging.info.reported_data.time = DEFAULT_DATA_LOGGING_EVENT;
	lmi.features.data_logging.info.reported_data.location = DEFAULT_DATA_LOGGING_EVENT;
	lmi.features.data_logging.info.reported_data.type_of_report = DEFAULT_DATA_LOGGING_EVENT;
	lmi.features.data_logging.info.reported_data.reading = DEFAULT_DATA_LOGGING_EVENT;
	lmi.features.data_logging.info.reported_data.reading_units = DEFAULT_DATA_LOGGING_EVENT;
	lmi.features.data_logging.info.reported_data.reading_multiplier = DEFAULT_DATA_LOGGING_EVENT;
	lmi.features.data_logging.info.reported_data.system_status = DEFAULT_DATA_LOGGING_EVENT;
	lmi.features.data_logging.info.reported_data.integrated_reading = DEFAULT_DATA_LOGGING_EVENT;
	lmi.features.data_logging.info.reported_data.integrated_reading_units = DEFAULT_DATA_LOGGING_EVENT;
	lmi.features.data_logging.info.reported_data.integrated_reading_multiplier = DEFAULT_DATA_LOGGING_EVENT;
	lmi.features.data_logging.info.reported_data.temperature = DEFAULT_DATA_LOGGING_EVENT;
	lmi.features.data_logging.info.reported_data.high_voltage_1 = DEFAULT_DATA_LOGGING_EVENT;
	lmi.features.data_logging.info.reported_data.input_voltage = DEFAULT_DATA_LOGGING_EVENT;											// (9DP v1.02.06)
	lmi.features.data_logging.info.reported_data.battery_voltage = DEFAULT_DATA_LOGGING_EVENT;
	lmi.features.data_logging.info.reported_data.undefined_1 = DEFAULT_DATA_LOGGING_EVENT;
	lmi.features.data_logging.info.reported_data.undefined_2 = DEFAULT_DATA_LOGGING_EVENT;
	lmi.features.data_logging.info.reported_data.undefined_3 = DEFAULT_DATA_LOGGING_EVENT;
	lmi.features.data_logging.info.reported_data.undefined_4 = DEFAULT_DATA_LOGGING_EVENT;
	lmi.features.data_logging.info.reported_data.undefined_5 = DEFAULT_DATA_LOGGING_EVENT;
	lmi.features.data_logging.info.reported_data.undefined_6 = DEFAULT_DATA_LOGGING_EVENT;
	lmi.features.data_logging.info.reported_data.undefined_7 = DEFAULT_DATA_LOGGING_EVENT;
	lmi.features.data_logging.info.reported_data.undefined_8 = DEFAULT_DATA_LOGGING_EVENT;
	lmi.features.data_logging.info.reported_data.undefined_9 = DEFAULT_DATA_LOGGING_EVENT;
	lmi.features.data_logging.info.reported_data.undefined_10 = DEFAULT_DATA_LOGGING_EVENT;
	lmi.features.data_logging.info.reported_data.undefined_11 = DEFAULT_DATA_LOGGING_EVENT;
	lmi.features.data_logging.info.reported_data.undefined_12 = DEFAULT_DATA_LOGGING_EVENT;
	lmi.features.data_logging.info.reported_data.undefined_13 = DEFAULT_DATA_LOGGING_EVENT;
	lmi.features.data_logging.info.reported_data.undefined_14 = DEFAULT_DATA_LOGGING_EVENT;
	lmi.features.data_logging.info.reported_data.undefined_15 = DEFAULT_DATA_LOGGING_EVENT;
	lmi.features.data_logging.info.reported_data.undefined_16 = DEFAULT_DATA_LOGGING_EVENT;
	lmi.features.data_logging.info.reported_data.system_status = DEFAULT_DATA_LOGGING_EVENT;
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_DATA_LOGGING_LOCATION, 2, (BYTE *)lmi.features.data_logging.buffer, SIZE_DATA_LOGGING);

//* CONtrols - Backlight
	lmi.controls.backlight.info.powerup_mode = DEFAULT_BACKLIGHT_POWERUP_MODE;
	lmi.controls.backlight.info.operating_mode = DEFAULT_BACKLIGHT_OPERATING_MODE;
	lmi.controls.backlight.info.current_level = DEFAULT_BACKLIGHT_CURRENT_LEVEL;
	lmi.controls.backlight.info.auto_low = DEFAULT_BACKLIGHT_AUTO_LOW;
	lmi.controls.backlight.info.auto_medium = DEFAULT_BACKLIGHT_AUTO_MEDIUM;
	lmi.controls.backlight.info.auto_high = DEFAULT_BACKLIGHT_AUTO_HIGH;
	lmi.controls.backlight.info.on_level = DEFAULT_BACKLIGHT_ON_LEVEL;
	lmi.controls.backlight.info.temp_time = DEFAULT_BACKLIGHT_TEMP_TIME;
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_BACKLIGHT_LOCATION, 2, (BYTE *)lmi.controls.backlight.buffer, SIZE_BACKLIGHT);

//* CONtrols - Audio
	lmi.controls.audio.info.powerup_mode = DEFAULT_AUDIO_POWERUP_MODE;
	lmi.controls.audio.info.operating_mode = DEFAULT_AUDIO_OPERATING_MODE;
	lmi.controls.audio.info.current_level = DEFAULT_AUDIO_CURRENT_LEVEL;
	lmi.controls.audio.info.alert_level = DEFAULT_AUDIO_ALERT_LEVEL;
	lmi.controls.audio.info.alarm_level = DEFAULT_AUDIO_ALARM_LEVEL;
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_AUDIO_LOCATION, 2, (BYTE *)lmi.controls.audio.buffer, SIZE_AUDIO);

//*	CONtrols - GPS
	lmi.controls.gps.info.powerup_mode = DEFAULT_GPS_POWERUP_MODE;
	lmi.controls.gps.info.operating_mode = DEFAULT_GPS_OPERATING_MODE;
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_GPS_LOCATION, 2, (BYTE *)lmi.controls.gps.buffer, SIZE_GPS);

//*	CONtrols - Bluetooth
	lmi.controls.bluetooth.info.powerup_mode = DEFAULT_BLUETOOTH_POWERUP_MODE;
	lmi.controls.bluetooth.info.operating_mode = DEFAULT_BLUETOOTH_OPERATING_MODE;
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_BLUETOOTH_LOCATION, 2, (BYTE *)lmi.controls.bluetooth.buffer, SIZE_BLUETOOTH);

//*	Display - Properties
	lmi.display.properties.info.powerup_mode = DEFAULT_DETECTOR_POWERUP_MODE;
	lmi.display.properties.info.current_detector = DEFAULT_DETECTOR;
#ifdef EEPROM_DISPLAY_PROPERTIES_LOCATION
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_DISPLAY_PROPERTIES_LOCATION, 2, (BYTE *)lmi.display.properties.buffer, SIZE_DISPLAY_PROPERTIES);
#endif

//* Display - Detector
	for(detector_index = 0; detector_index < NUMBER_OF_DETECTORS; detector_index++)
	{
		//*	Display - Detector - Properties
		if(detector_index == 0)
			lmi.display.detector[detector_index].properties.info.model = DEFAULT_DETECTOR_0;
		else if(detector_index == 1)
			lmi.display.detector[detector_index].properties.info.model = DEFAULT_DETECTOR_1;
		else if(detector_index == 2)
			lmi.display.detector[detector_index].properties.info.model = DEFAULT_DETECTOR_2;
		else if(detector_index == 3)
			lmi.display.detector[detector_index].properties.info.model = DEFAULT_DETECTOR_3;
		else if(detector_index == 4)
			lmi.display.detector[detector_index].properties.info.model = DEFAULT_DETECTOR_4;
		strncpy((BYTE *)lmi.display.detector[detector_index].properties.info.serial_number,DEFAULT_SERIAL_NUMBER,LENGTH_SERIAL_NUMBER);
		strncpy((BYTE *)lmi.display.detector[detector_index].properties.info.user_id_number,DEFAULT_USER_ID_NUMBER,LENGTH_USER_ID_NUMBER);
		strncpy((BYTE *)lmi.display.detector[detector_index].properties.info.user_serial_number,DEFAULT_USER_SERIAL_NUMBER,LENGTH_USER_SERIAL_NUMBER);
		strncpy((BYTE *)lmi.display.detector[detector_index].properties.info.technician,lmi.calibration.checkout.info.technician,LENGTH_CHECKOUT_TECHNICIAN);
#ifdef EEPROM_DETECTOR_PROPERTIES_LOCATION
		address = EEPROM_DETECTOR_PROPERTIES_LOCATION + (detector_index * TOTAL_DETECTOR);
		i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].properties.buffer, SIZE_DETECTOR_PROPERTIES);
#endif

		//*	Display - Detector - Calibration
		lmi.display.detector[detector_index].calibration.info.u_arg[0] = DEFAULT_UNSIGNED_00;
		lmi.display.detector[detector_index].calibration.info.u_arg[1] = DEFAULT_UNSIGNED_01;
		lmi.display.detector[detector_index].calibration.info.u_arg[2] = DEFAULT_UNSIGNED_02;
		lmi.display.detector[detector_index].calibration.info.u_arg[3] = DEFAULT_UNSIGNED_03;
		lmi.display.detector[detector_index].calibration.info.u_arg[4] = DEFAULT_UNSIGNED_04;
		lmi.display.detector[detector_index].calibration.info.u_arg[5] = DEFAULT_UNSIGNED_05;
		lmi.display.detector[detector_index].calibration.info.u_arg[6] = DEFAULT_UNSIGNED_06;
		lmi.display.detector[detector_index].calibration.info.u_arg[7] = DEFAULT_UNSIGNED_07;
		lmi.display.detector[detector_index].calibration.info.u_arg[8] = DEFAULT_UNSIGNED_08;
		lmi.display.detector[detector_index].calibration.info.u_arg[9] = DEFAULT_UNSIGNED_09;
		lmi.display.detector[detector_index].calibration.info.u_arg[10] = DEFAULT_UNSIGNED_10;
		lmi.display.detector[detector_index].calibration.info.u_arg[11] = DEFAULT_UNSIGNED_11;
		lmi.display.detector[detector_index].calibration.info.u_arg[12] = DEFAULT_UNSIGNED_12;
		lmi.display.detector[detector_index].calibration.info.u_arg[13] = DEFAULT_UNSIGNED_13;
		lmi.display.detector[detector_index].calibration.info.u_arg[14] = DEFAULT_UNSIGNED_14;
		lmi.display.detector[detector_index].calibration.info.u_arg[15] = DEFAULT_UNSIGNED_15;

		lmi.display.detector[detector_index].calibration.info.s_arg[0] = DEFAULT_SIGNED_00;
		lmi.display.detector[detector_index].calibration.info.s_arg[1] = DEFAULT_SIGNED_01;
		lmi.display.detector[detector_index].calibration.info.s_arg[2] = DEFAULT_SIGNED_02;
		lmi.display.detector[detector_index].calibration.info.s_arg[3] = DEFAULT_SIGNED_03;
		lmi.display.detector[detector_index].calibration.info.s_arg[4] = DEFAULT_SIGNED_04;
		lmi.display.detector[detector_index].calibration.info.s_arg[5] = DEFAULT_SIGNED_05;
		lmi.display.detector[detector_index].calibration.info.s_arg[6] = DEFAULT_SIGNED_06;
		lmi.display.detector[detector_index].calibration.info.s_arg[7] = DEFAULT_SIGNED_07;
		lmi.display.detector[detector_index].calibration.info.s_arg[8] = DEFAULT_SIGNED_08;
		lmi.display.detector[detector_index].calibration.info.s_arg[9] = DEFAULT_SIGNED_09;
		lmi.display.detector[detector_index].calibration.info.s_arg[10] = DEFAULT_SIGNED_10;
		lmi.display.detector[detector_index].calibration.info.s_arg[11] = DEFAULT_SIGNED_11;
		lmi.display.detector[detector_index].calibration.info.s_arg[12] = DEFAULT_SIGNED_12;
		lmi.display.detector[detector_index].calibration.info.s_arg[13] = DEFAULT_SIGNED_13;
		lmi.display.detector[detector_index].calibration.info.s_arg[14] = DEFAULT_SIGNED_14;
		lmi.display.detector[detector_index].calibration.info.s_arg[15] = DEFAULT_SIGNED_15;

		lmi.display.detector[detector_index].calibration.info.f_arg[0] = DEFAULT_FLOAT_00;
		lmi.display.detector[detector_index].calibration.info.f_arg[1] = DEFAULT_FLOAT_01;
		lmi.display.detector[detector_index].calibration.info.f_arg[2] = DEFAULT_FLOAT_02;
		lmi.display.detector[detector_index].calibration.info.f_arg[3] = DEFAULT_FLOAT_03;
		lmi.display.detector[detector_index].calibration.info.f_arg[4] = DEFAULT_FLOAT_04;
		lmi.display.detector[detector_index].calibration.info.f_arg[5] = DEFAULT_FLOAT_05;
		lmi.display.detector[detector_index].calibration.info.f_arg[6] = DEFAULT_FLOAT_06;
		lmi.display.detector[detector_index].calibration.info.f_arg[7] = DEFAULT_FLOAT_07;

#ifdef EEPROM_DETECTOR_CALIBRATION_LOCATION
		address = EEPROM_DETECTOR_CALIBRATION_LOCATION + (detector_index * TOTAL_DETECTOR);
		i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].calibration.buffer, SIZE_DETECTOR_CALIBRATION);
#endif

		//*	Display - Detector - View Setup
		lmi.display.detector[detector_index].view_setup.info.powerup_mode = DEFAULT_VIEWS_POWERUP_MODE;
		lmi.display.detector[detector_index].view_setup.info.current_view = DEFAULT_CURRENT_VIEW;
#ifdef EEPROM_DETECTOR_VIEW_SETUP_LOCATION
		address = EEPROM_DETECTOR_VIEW_SETUP_LOCATION + (detector_index * TOTAL_DETECTOR);
		i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].view_setup.buffer, SIZE_DETECTOR_VIEW_SETUP);
#endif

		//*	View - Appearance
		for (view_index = 0; view_index < NUMBER_OF_VIEWS; view_index++)
		{
			lmi.display.detector[detector_index].view[view_index].appearance.info.background_color = WHITE_EEPROM;
			lmi.display.detector[detector_index].view[view_index].appearance.info.foreground_color = BRIGHTBLUE_EEPROM;
			lmi.display.detector[detector_index].view[view_index].appearance.info.display_time = DEFAULT_APPEARANCE_DISPLAY_TIME;
			lmi.display.detector[detector_index].view[view_index].appearance.info.display_view_name = DEFAULT_APPEARANCE_DISPLAY_VIEW_NAME;
			address = EEPROM_APPEARANCE_LOCATION + (detector_index * TOTAL_DETECTOR) + (view_index * TOTAL_VIEW);
			i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].view[view_index].appearance.buffer, SIZE_APPEARANCE);
		}
	
		//*	View - Element Header
		for (view_index = 0; view_index < NUMBER_OF_VIEWS; view_index++)
		{
			switch(view_index)
			{
				case VIEW_1:
				{
					lmi.display.detector[detector_index].view[view_index].element_header.info.style = DEFAULT_ELEMENT_STYLE_1;
					strncpy(lmi.display.detector[detector_index].view[view_index].element_header.info.name, DEFAULT_ELEMENT_NAME_1, SIZE_ELEMENT_HEADER_NAME);
					break;
				}
				case VIEW_2:
				{
					lmi.display.detector[detector_index].view[view_index].element_header.info.style = DEFAULT_ELEMENT_STYLE_2;
					strncpy(lmi.display.detector[detector_index].view[view_index].element_header.info.name, DEFAULT_ELEMENT_NAME_2, SIZE_ELEMENT_HEADER_NAME);
					break;
				}
				case VIEW_3:
				{
					lmi.display.detector[detector_index].view[view_index].element_header.info.style = DEFAULT_ELEMENT_STYLE_3;
					strncpy(lmi.display.detector[detector_index].view[view_index].element_header.info.name, DEFAULT_ELEMENT_NAME_3, SIZE_ELEMENT_HEADER_NAME);
					break;
				}
				case VIEW_4:
				{
					lmi.display.detector[detector_index].view[view_index].element_header.info.style = DEFAULT_ELEMENT_STYLE_4;
					strncpy(lmi.display.detector[detector_index].view[view_index].element_header.info.name, DEFAULT_ELEMENT_NAME_4, SIZE_ELEMENT_HEADER_NAME);
					break;
				}	
			}
			address = EEPROM_ELEMENT_HEADER_LOCATION + (detector_index * TOTAL_DETECTOR) + (view_index * TOTAL_VIEW);
			i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].view[view_index].element_header.buffer, SIZE_ELEMENT_HEADER);
		}
	
		//*	View - Element
		for (view_index = 0; view_index < NUMBER_OF_VIEWS; view_index++)
		{
			if(view_index == 0)
			{
				lmi.display.detector[detector_index].view[view_index].element.info.arc.control.type_of_scale = DEFAULT_ELEMENT_1_ARC_TYPE_OF_SCALE;
				lmi.display.detector[detector_index].view[view_index].element.info.arc.control.scale.number_of_divisions = DEFAULT_ELEMENT_1_ARC_NUMBER_OF_DIVISIONS;
				lmi.display.detector[detector_index].view[view_index].element.info.arc.control.digits = DEFAULT_ELEMENT_1_ARC_DIGITS;
				lmi.display.detector[detector_index].view[view_index].element.info.arc.control.multiplier = DEFAULT_ELEMENT_1_ARC_MULTIPLIER;
				lmi.display.detector[detector_index].view[view_index].element.info.arc.control.units = DEFAULT_ELEMENT_1_ARC_UNITS;
				lmi.display.detector[detector_index].view[view_index].element.info.arc.control.end_point = DEFAULT_ELEMENT_1_ARC_END_POINT;
			}
			else if(view_index == 1)
			{
				lmi.display.detector[detector_index].view[view_index].element.info.arc.control.type_of_scale = DEFAULT_ELEMENT_2_ARC_TYPE_OF_SCALE;
				lmi.display.detector[detector_index].view[view_index].element.info.arc.control.scale.number_of_divisions = DEFAULT_ELEMENT_2_ARC_NUMBER_OF_DIVISIONS;
				lmi.display.detector[detector_index].view[view_index].element.info.arc.control.digits = DEFAULT_ELEMENT_2_ARC_DIGITS;
				lmi.display.detector[detector_index].view[view_index].element.info.arc.control.multiplier = DEFAULT_ELEMENT_2_ARC_MULTIPLIER;
				lmi.display.detector[detector_index].view[view_index].element.info.arc.control.units = DEFAULT_ELEMENT_2_ARC_UNITS;
				lmi.display.detector[detector_index].view[view_index].element.info.arc.control.end_point = DEFAULT_ELEMENT_2_ARC_END_POINT;
			}
			else if(view_index == 2)
			{
				lmi.display.detector[detector_index].view[view_index].element.info.arc.control.type_of_scale = DEFAULT_ELEMENT_3_ARC_TYPE_OF_SCALE;
				lmi.display.detector[detector_index].view[view_index].element.info.arc.control.scale.number_of_divisions = DEFAULT_ELEMENT_3_ARC_NUMBER_OF_DIVISIONS;
				lmi.display.detector[detector_index].view[view_index].element.info.arc.control.digits = DEFAULT_ELEMENT_3_ARC_DIGITS;
				lmi.display.detector[detector_index].view[view_index].element.info.arc.control.multiplier = DEFAULT_ELEMENT_3_ARC_MULTIPLIER;
				lmi.display.detector[detector_index].view[view_index].element.info.arc.control.units = DEFAULT_ELEMENT_3_ARC_UNITS;
				lmi.display.detector[detector_index].view[view_index].element.info.arc.control.end_point = DEFAULT_ELEMENT_3_ARC_END_POINT;
			}
			else if(view_index == 3)
			{
				lmi.display.detector[detector_index].view[view_index].element.info.arc.control.type_of_scale = DEFAULT_ELEMENT_4_ARC_TYPE_OF_SCALE;
				lmi.display.detector[detector_index].view[view_index].element.info.arc.control.scale.number_of_divisions = DEFAULT_ELEMENT_4_ARC_NUMBER_OF_DIVISIONS;
				lmi.display.detector[detector_index].view[view_index].element.info.arc.control.digits = DEFAULT_ELEMENT_4_ARC_DIGITS;
				lmi.display.detector[detector_index].view[view_index].element.info.arc.control.multiplier = DEFAULT_ELEMENT_4_ARC_MULTIPLIER;
				lmi.display.detector[detector_index].view[view_index].element.info.arc.control.units = DEFAULT_ELEMENT_4_ARC_UNITS;
				lmi.display.detector[detector_index].view[view_index].element.info.arc.control.end_point = DEFAULT_ELEMENT_4_ARC_END_POINT;
			}
			address = EEPROM_ELEMENT_LOCATION + (detector_index * TOTAL_DETECTOR) + (view_index * TOTAL_VIEW);
			i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].view[view_index].element.buffer, SIZE_ELEMENT);
		}
	
		//*	View - Functions
		for(view_index = 0; view_index < NUMBER_OF_VIEWS; view_index++)
		{
			if(view_index == 0)
			{
				lmi.display.detector[detector_index].view[view_index].functions.info.current_function = DEFAULT_VIEW_FUNCTIONS_CURRENT_FUNCTION;
				lmi.display.detector[detector_index].view[view_index].functions.info.function[0].enable = DEFAULT_VIEW_1_FUNCTION_1_ENABLE;
				lmi.display.detector[detector_index].view[view_index].functions.info.function[0].function_index = DEFAULT_VIEW_1_FUNCTION_1_NUMBER;
				lmi.display.detector[detector_index].view[view_index].functions.info.function[1].enable = DEFAULT_VIEW_1_FUNCTION_2_ENABLE;
				lmi.display.detector[detector_index].view[view_index].functions.info.function[1].function_index = DEFAULT_VIEW_1_FUNCTION_2_NUMBER;
				lmi.display.detector[detector_index].view[view_index].functions.info.function[2].enable = DEFAULT_VIEW_1_FUNCTION_3_ENABLE;
				lmi.display.detector[detector_index].view[view_index].functions.info.function[2].function_index = DEFAULT_VIEW_1_FUNCTION_3_NUMBER;
				lmi.display.detector[detector_index].view[view_index].functions.info.function[3].enable = DEFAULT_VIEW_1_FUNCTION_4_ENABLE;
				lmi.display.detector[detector_index].view[view_index].functions.info.function[3].function_index = DEFAULT_VIEW_1_FUNCTION_4_NUMBER;
			}
			else if(view_index == 1)
			{
				lmi.display.detector[detector_index].view[view_index].functions.info.current_function = DEFAULT_VIEW_FUNCTIONS_CURRENT_FUNCTION;
				lmi.display.detector[detector_index].view[view_index].functions.info.function[0].enable = DEFAULT_VIEW_2_FUNCTION_1_ENABLE;
				lmi.display.detector[detector_index].view[view_index].functions.info.function[0].function_index = DEFAULT_VIEW_2_FUNCTION_1_NUMBER;
				lmi.display.detector[detector_index].view[view_index].functions.info.function[1].enable = DEFAULT_VIEW_2_FUNCTION_2_ENABLE;
				lmi.display.detector[detector_index].view[view_index].functions.info.function[1].function_index = DEFAULT_VIEW_2_FUNCTION_2_NUMBER;
				lmi.display.detector[detector_index].view[view_index].functions.info.function[2].enable = DEFAULT_VIEW_2_FUNCTION_3_ENABLE;
				lmi.display.detector[detector_index].view[view_index].functions.info.function[2].function_index = DEFAULT_VIEW_2_FUNCTION_3_NUMBER;
				lmi.display.detector[detector_index].view[view_index].functions.info.function[3].enable = DEFAULT_VIEW_2_FUNCTION_4_ENABLE;
				lmi.display.detector[detector_index].view[view_index].functions.info.function[3].function_index = DEFAULT_VIEW_2_FUNCTION_4_NUMBER;
			}	
			else if(view_index == 2)
			{
				lmi.display.detector[detector_index].view[view_index].functions.info.current_function = DEFAULT_VIEW_FUNCTIONS_CURRENT_FUNCTION;
				lmi.display.detector[detector_index].view[view_index].functions.info.function[0].enable = DEFAULT_VIEW_3_FUNCTION_1_ENABLE;
				lmi.display.detector[detector_index].view[view_index].functions.info.function[0].function_index = DEFAULT_VIEW_3_FUNCTION_1_NUMBER;
				lmi.display.detector[detector_index].view[view_index].functions.info.function[1].enable = DEFAULT_VIEW_3_FUNCTION_2_ENABLE;
				lmi.display.detector[detector_index].view[view_index].functions.info.function[1].function_index = DEFAULT_VIEW_3_FUNCTION_2_NUMBER;
				lmi.display.detector[detector_index].view[view_index].functions.info.function[2].enable = DEFAULT_VIEW_3_FUNCTION_3_ENABLE;
				lmi.display.detector[detector_index].view[view_index].functions.info.function[2].function_index = DEFAULT_VIEW_3_FUNCTION_3_NUMBER;
				lmi.display.detector[detector_index].view[view_index].functions.info.function[3].enable = DEFAULT_VIEW_3_FUNCTION_4_ENABLE;
				lmi.display.detector[detector_index].view[view_index].functions.info.function[3].function_index = DEFAULT_VIEW_3_FUNCTION_4_NUMBER;
			}	
			else if(view_index == 3)
			{
				lmi.display.detector[detector_index].view[view_index].functions.info.current_function = DEFAULT_VIEW_FUNCTIONS_CURRENT_FUNCTION;
				lmi.display.detector[detector_index].view[view_index].functions.info.function[0].enable = DEFAULT_VIEW_4_FUNCTION_1_ENABLE;
				lmi.display.detector[detector_index].view[view_index].functions.info.function[0].function_index = DEFAULT_VIEW_4_FUNCTION_1_NUMBER;
				lmi.display.detector[detector_index].view[view_index].functions.info.function[1].enable = DEFAULT_VIEW_4_FUNCTION_2_ENABLE;
				lmi.display.detector[detector_index].view[view_index].functions.info.function[1].function_index = DEFAULT_VIEW_4_FUNCTION_2_NUMBER;
				lmi.display.detector[detector_index].view[view_index].functions.info.function[2].enable = DEFAULT_VIEW_4_FUNCTION_3_ENABLE;
				lmi.display.detector[detector_index].view[view_index].functions.info.function[2].function_index = DEFAULT_VIEW_4_FUNCTION_3_NUMBER;
				lmi.display.detector[detector_index].view[view_index].functions.info.function[3].enable = DEFAULT_VIEW_4_FUNCTION_4_ENABLE;
				lmi.display.detector[detector_index].view[view_index].functions.info.function[3].function_index = DEFAULT_VIEW_4_FUNCTION_4_NUMBER;
			}	
			address = EEPROM_VIEW_FUNCTIONS_LOCATION + (detector_index * TOTAL_DETECTOR) + (view_index * TOTAL_VIEW);
			i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].view[view_index].functions.buffer, SIZE_VIEW_FUNCTIONS);
		}
	
		//*	View - Radiation Alerts
		lmi.display.detector[detector_index].view[0].alerts_radiation.info.feature.reading.units = DEFAULT_VIEW_1_RAD_ALERTS_UNITS;
		lmi.display.detector[detector_index].view[1].alerts_radiation.info.feature.reading.units = DEFAULT_VIEW_2_RAD_ALERTS_UNITS;
		lmi.display.detector[detector_index].view[2].alerts_radiation.info.feature.reading.units = DEFAULT_VIEW_3_RAD_ALERTS_UNITS;
		lmi.display.detector[detector_index].view[3].alerts_radiation.info.feature.reading.units = DEFAULT_VIEW_4_RAD_ALERTS_UNITS;
	
		for (view_index = 0; view_index < NUMBER_OF_VIEWS; view_index++)
		{
			lmi.display.detector[detector_index].view[view_index].alerts_radiation.info.feature.enable = DEFAULT_VIEW_RAD_ALERTS_ENABLE;
			lmi.display.detector[detector_index].view[view_index].alerts_radiation.info.feature.custom = DEFAULT_VIEW_RAD_ALERTS_CUSTOM;
			lmi.display.detector[detector_index].view[view_index].alerts_radiation.info.feature.reading.value = DEFAULT_VIEW_RAD_ALERTS_VALUE;
			lmi.display.detector[detector_index].view[view_index].alerts_radiation.info.feature.reading.multiplier = DEFAULT_VIEW_RAD_ALERTS_MULTIPLIER;
			lmi.display.detector[detector_index].view[view_index].alerts_radiation.info.feature.relative_reading = DEFAULT_VIEW_RAD_ALERTS_RELATIVE_READING;
			strncpy(lmi.display.detector[detector_index].view[view_index].alerts_radiation.info.feature.custom_message, DEFAULT_VIEW_RAD_ALERTS_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);
			address = EEPROM_ALERTS_RADIATION_LOCATION + (detector_index * TOTAL_DETECTOR) + (view_index * TOTAL_VIEW);
			i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].view[view_index].alerts_radiation.buffer, SIZE_ALERTS_RADIATION);
		}
	
		//*	View - Integrate Alerts
		lmi.display.detector[detector_index].view[0].alerts_integrate.info.feature.reading.units = DEFAULT_VIEW_1_INT_ALERTS_UNITS;
		lmi.display.detector[detector_index].view[1].alerts_integrate.info.feature.reading.units = DEFAULT_VIEW_2_INT_ALERTS_UNITS;
		lmi.display.detector[detector_index].view[2].alerts_integrate.info.feature.reading.units = DEFAULT_VIEW_3_INT_ALERTS_UNITS;
		lmi.display.detector[detector_index].view[3].alerts_integrate.info.feature.reading.units = DEFAULT_VIEW_4_INT_ALERTS_UNITS;
	
		for (view_index = 0; view_index < NUMBER_OF_VIEWS; view_index++)
		{
			lmi.display.detector[detector_index].view[view_index].alerts_integrate.info.feature.enable = DEFAULT_VIEW_INT_ALERTS_ENABLE;
			lmi.display.detector[detector_index].view[view_index].alerts_integrate.info.feature.custom = DEFAULT_VIEW_INT_ALERTS_CUSTOM;
			lmi.display.detector[detector_index].view[view_index].alerts_integrate.info.feature.reading.value = DEFAULT_VIEW_INT_ALERTS_VALUE;
			lmi.display.detector[detector_index].view[view_index].alerts_integrate.info.feature.reading.multiplier = DEFAULT_VIEW_INT_ALERTS_MULTIPLIER;
			lmi.display.detector[detector_index].view[view_index].alerts_integrate.info.feature.relative_reading = DEFAULT_VIEW_INT_ALERTS_RELATIVE_READING;
			strncpy(lmi.display.detector[detector_index].view[view_index].alerts_integrate.info.feature.custom_message, DEFAULT_VIEW_INT_ALERTS_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);
			address = EEPROM_ALERTS_INTEGRATE_LOCATION + (detector_index * TOTAL_DETECTOR) + (view_index * TOTAL_VIEW);
			i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].view[view_index].alerts_integrate.buffer, SIZE_ALERTS_INTEGRATE);
		}
	
		//*	View - Function Alerts
		lmi.display.detector[detector_index].view[0].alerts_function.info.feature.reading.units = DEFAULT_VIEW_1_FUN_ALERTS_UNITS;
		lmi.display.detector[detector_index].view[1].alerts_function.info.feature.reading.units = DEFAULT_VIEW_2_FUN_ALERTS_UNITS;
		lmi.display.detector[detector_index].view[2].alerts_function.info.feature.reading.units = DEFAULT_VIEW_3_FUN_ALERTS_UNITS;
		lmi.display.detector[detector_index].view[3].alerts_function.info.feature.reading.units = DEFAULT_VIEW_4_FUN_ALERTS_UNITS;
	
		for (view_index = 0; view_index < NUMBER_OF_VIEWS; view_index++)
		{
			lmi.display.detector[detector_index].view[view_index].alerts_function.info.function_select = DEFAULT_VIEW_FUNCTION_SELECT;
			lmi.display.detector[detector_index].view[view_index].alerts_function.info.feature.enable = DEFAULT_VIEW_FUN_ALERTS_ENABLE;
			lmi.display.detector[detector_index].view[view_index].alerts_function.info.feature.custom = DEFAULT_VIEW_FUN_ALERTS_CUSTOM;
			lmi.display.detector[detector_index].view[view_index].alerts_function.info.feature.reading.value = DEFAULT_VIEW_FUN_ALERTS_VALUE;
			lmi.display.detector[detector_index].view[view_index].alerts_function.info.feature.reading.multiplier = DEFAULT_VIEW_FUN_ALERTS_MULTIPLIER;
			lmi.display.detector[detector_index].view[view_index].alerts_function.info.feature.relative_reading = DEFAULT_VIEW_FUN_ALERTS_RELATIVE_READING;
			strncpy(lmi.display.detector[detector_index].view[view_index].alerts_function.info.feature.custom_message, DEFAULT_VIEW_FUN_ALERTS_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);
			address = EEPROM_ALERTS_FUNCTION_LOCATION + (detector_index * TOTAL_DETECTOR) + (view_index * TOTAL_VIEW);
			i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].view[view_index].alerts_function.buffer, SIZE_ALERTS_FUNCTION);
		}
	
		//*	View - Radiation Alarms
		lmi.display.detector[detector_index].view[0].alarms_radiation.info.feature.reading.units = DEFAULT_VIEW_1_RAD_ALARMS_UNITS;
		lmi.display.detector[detector_index].view[1].alarms_radiation.info.feature.reading.units = DEFAULT_VIEW_2_RAD_ALARMS_UNITS;
		lmi.display.detector[detector_index].view[2].alarms_radiation.info.feature.reading.units = DEFAULT_VIEW_3_RAD_ALARMS_UNITS;
		lmi.display.detector[detector_index].view[3].alarms_radiation.info.feature.reading.units = DEFAULT_VIEW_4_RAD_ALARMS_UNITS;
	
		for (view_index = 0; view_index < NUMBER_OF_VIEWS; view_index++)
		{
			lmi.display.detector[detector_index].view[view_index].alarms_radiation.info.feature.enable = DEFAULT_VIEW_RAD_ALARMS_ENABLE;
			lmi.display.detector[detector_index].view[view_index].alarms_radiation.info.feature.custom = DEFAULT_VIEW_RAD_ALARMS_CUSTOM;
			lmi.display.detector[detector_index].view[view_index].alarms_radiation.info.feature.reading.value = DEFAULT_VIEW_RAD_ALARMS_VALUE;
			lmi.display.detector[detector_index].view[view_index].alarms_radiation.info.feature.reading.multiplier = DEFAULT_VIEW_RAD_ALARMS_MULTIPLIER;
			lmi.display.detector[detector_index].view[view_index].alarms_radiation.info.feature.relative_reading = DEFAULT_VIEW_RAD_ALARMS_RELATIVE_READING;
			strncpy(lmi.display.detector[detector_index].view[view_index].alarms_radiation.info.feature.custom_message, DEFAULT_VIEW_RAD_ALARMS_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);
			address = EEPROM_ALARMS_RADIATION_LOCATION + (detector_index * TOTAL_DETECTOR) + (view_index * TOTAL_VIEW);
			i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].view[view_index].alarms_radiation.buffer, SIZE_ALARMS_RADIATION);
		}
	
		//*	View - Integrate Alarms
		lmi.display.detector[detector_index].view[0].alarms_integrate.info.feature.reading.units = DEFAULT_VIEW_1_INT_ALARMS_UNITS;
		lmi.display.detector[detector_index].view[1].alarms_integrate.info.feature.reading.units = DEFAULT_VIEW_2_INT_ALARMS_UNITS;
		lmi.display.detector[detector_index].view[2].alarms_integrate.info.feature.reading.units = DEFAULT_VIEW_3_INT_ALARMS_UNITS;
		lmi.display.detector[detector_index].view[3].alarms_integrate.info.feature.reading.units = DEFAULT_VIEW_4_INT_ALARMS_UNITS;
	
		for (view_index = 0; view_index < NUMBER_OF_VIEWS; view_index++)
		{
			lmi.display.detector[detector_index].view[view_index].alarms_integrate.info.feature.enable = DEFAULT_VIEW_INT_ALARMS_ENABLE;
			lmi.display.detector[detector_index].view[view_index].alarms_integrate.info.feature.custom = DEFAULT_VIEW_INT_ALARMS_CUSTOM;
			lmi.display.detector[detector_index].view[view_index].alarms_integrate.info.feature.reading.value = DEFAULT_VIEW_INT_ALARMS_VALUE;
			lmi.display.detector[detector_index].view[view_index].alarms_integrate.info.feature.reading.multiplier = DEFAULT_VIEW_INT_ALARMS_MULTIPLIER;
			lmi.display.detector[detector_index].view[view_index].alarms_integrate.info.feature.relative_reading = DEFAULT_VIEW_INT_ALARMS_RELATIVE_READING;
			strncpy(lmi.display.detector[detector_index].view[view_index].alarms_integrate.info.feature.custom_message, DEFAULT_VIEW_INT_ALARMS_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);
			address = EEPROM_ALARMS_INTEGRATE_LOCATION + (detector_index * TOTAL_DETECTOR) + (view_index * TOTAL_VIEW);
			i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].view[view_index].alarms_integrate.buffer, SIZE_ALARMS_INTEGRATE);
		}
	
		//*	View - Function Alarms
		lmi.display.detector[detector_index].view[0].alarms_function.info.feature.reading.units = DEFAULT_VIEW_1_FUN_ALARMS_UNITS;
		lmi.display.detector[detector_index].view[1].alarms_function.info.feature.reading.units = DEFAULT_VIEW_2_FUN_ALARMS_UNITS;
		lmi.display.detector[detector_index].view[2].alarms_function.info.feature.reading.units = DEFAULT_VIEW_3_FUN_ALARMS_UNITS;
		lmi.display.detector[detector_index].view[3].alarms_function.info.feature.reading.units = DEFAULT_VIEW_4_FUN_ALARMS_UNITS;
	
		for (view_index = 0; view_index < NUMBER_OF_VIEWS; view_index++)
		{
			lmi.display.detector[detector_index].view[view_index].alarms_function.info.function_select = DEFAULT_VIEW_FUNCTION_SELECT;
			lmi.display.detector[detector_index].view[view_index].alarms_function.info.feature.enable = DEFAULT_VIEW_FUN_ALARMS_ENABLE;
			lmi.display.detector[detector_index].view[view_index].alarms_function.info.feature.custom = DEFAULT_VIEW_FUN_ALARMS_CUSTOM;
			lmi.display.detector[detector_index].view[view_index].alarms_function.info.feature.reading.value = DEFAULT_VIEW_FUN_ALARMS_VALUE;
			lmi.display.detector[detector_index].view[view_index].alarms_function.info.feature.reading.multiplier = DEFAULT_VIEW_FUN_ALARMS_MULTIPLIER;
			lmi.display.detector[detector_index].view[view_index].alarms_function.info.feature.relative_reading = DEFAULT_VIEW_FUN_ALARMS_RELATIVE_READING;
			strncpy(lmi.display.detector[detector_index].view[view_index].alarms_function.info.feature.custom_message, DEFAULT_VIEW_FUN_ALARMS_CUSTOM_MESSAGE, SIZE_CUSTOM_MESSAGE);
			address = EEPROM_ALARMS_FUNCTION_LOCATION + (detector_index * TOTAL_DETECTOR) + (view_index * TOTAL_VIEW);
			i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].view[view_index].alarms_function.buffer, SIZE_ALARMS_FUNCTION);
		}
//* changed (3D v1.00.04)
#ifdef EEPROM_VIEW_CALIBRATION_LOCATION
		for (view_index = 0; view_index < NUMBER_OF_VIEWS; view_index++)
		{
			for (index = 0; index < 16; index++)
			{
				lmi.display.detector[detector_index].view[view_index].calibration.info.u_arg[index] = 0;
				lmi.display.detector[detector_index].view[view_index].calibration.info.s_arg[index] = 0;
			}
			for (index = 0; index < 8; index++)
			{
				lmi.display.detector[detector_index].view[view_index].calibration.info.f_arg[index] = 0.000;
			}
			address = EEPROM_VIEW_CALIBRATION_LOCATION + (detector_index * TOTAL_DETECTOR) + (view_index * TOTAL_VIEW);
			i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].view[view_index].calibration.buffer, SIZE_VIEW_CALIBRATION);
		}
#endif
//* end add
	}

//* Functions
	index = 0;
	lmi.function[index].info.header.type = DEFAULT_FUNCTION_1_TYPE;
	strncpy(lmi.function[index].info.header.name, DEFAULT_FUNCTION_1_NAME, SIZE_FUNCTION_NAME);
	EEPROM_Function_Defaults(index, lmi.function[index].info.header.type);
	address = EEPROM_FUNCTIONS_LOCATION + (index * TOTAL_FUNCTION);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.function[index].buffer, SIZE_FUNCTION);

	index++;
	lmi.function[index].info.header.type = DEFAULT_FUNCTION_2_TYPE;
	strncpy(lmi.function[index].info.header.name, DEFAULT_FUNCTION_2_NAME, SIZE_FUNCTION_NAME);
	EEPROM_Function_Defaults(index, lmi.function[index].info.header.type);
	address = EEPROM_FUNCTIONS_LOCATION + (index * TOTAL_FUNCTION);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.function[index].buffer, SIZE_FUNCTION);

	index++;
	lmi.function[index].info.header.type = DEFAULT_FUNCTION_3_TYPE;
	strncpy(lmi.function[index].info.header.name, DEFAULT_FUNCTION_3_NAME, SIZE_FUNCTION_NAME);
	EEPROM_Function_Defaults(index, lmi.function[index].info.header.type);
	address = EEPROM_FUNCTIONS_LOCATION + (index * TOTAL_FUNCTION);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.function[index].buffer, SIZE_FUNCTION);

	index++;
	lmi.function[index].info.header.type = DEFAULT_FUNCTION_4_TYPE;
	strncpy(lmi.function[index].info.header.name, DEFAULT_FUNCTION_4_NAME, SIZE_FUNCTION_NAME);
	EEPROM_Function_Defaults(index, lmi.function[index].info.header.type);
	address = EEPROM_FUNCTIONS_LOCATION + (index * TOTAL_FUNCTION);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.function[index].buffer, SIZE_FUNCTION);

//*	Alerts
	lmi.alerts.low_battery.info.feature.enable = DEFAULT_ALERT_LOW_BATTERY_ENABLE;
	lmi.alerts.low_battery.info.feature.custom = DEFAULT_ALERT_LOW_BATTERY_CUSTOM_ENABLE;
	lmi.alerts.low_battery.info.feature.value = DEFAULT_ALERT_LOW_BATTERY_VALUE;
	strncpy(lmi.alerts.low_battery.info.feature.custom_message,DEFAULT_ALERT_LOW_BATTERY_CUSTOM_MESSAGE,SIZE_CUSTOM_MESSAGE);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALERTS_LOW_BATTERY_LOCATION, 2, (BYTE *)lmi.alerts.low_battery.buffer, SIZE_ALERTS_LOW_BATTERY);
	
	lmi.alerts.low_memory.info.feature.enable = DEFAULT_ALERT_LOW_MEMORY_ENABLE;
	lmi.alerts.low_memory.info.feature.custom = DEFAULT_ALERT_LOW_MEMORY_CUSTOM_ENABLE;
	lmi.alerts.low_memory.info.feature.value = DEFAULT_ALERT_LOW_MEMORY_VALUE;
	strncpy(lmi.alerts.low_memory.info.feature.custom_message,DEFAULT_ALERT_LOW_BATTERY_CUSTOM_MESSAGE,SIZE_CUSTOM_MESSAGE);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALERTS_LOW_MEMORY_LOCATION, 2, (BYTE *)lmi.alerts.low_memory.buffer, SIZE_ALERTS_LOW_MEMORY);
	
	lmi.alerts.calibration_due.info.feature.enable = DEFAULT_ALERT_CALIBRATION_DUE_ENABLE;
	lmi.alerts.calibration_due.info.feature.custom = DEFAULT_ALERT_CALIBRATION_DUE_CUSTOM_ENABLE;
	lmi.alerts.calibration_due.info.feature.value = DEFAULT_ALERT_CALIBRATION_DUE_VALUE;
	strncpy(lmi.alerts.calibration_due.info.feature.custom_message,DEFAULT_ALERT_CALIBRATION_DUE_CUSTOM_MESSAGE,SIZE_CUSTOM_MESSAGE);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALERTS_CALIBRATION_DUE_LOCATION, 2, (BYTE *)lmi.alerts.calibration_due.buffer, SIZE_ALERTS_CALIBRATION_DUE);
	
	lmi.alerts.source_check.info.feature.enable = DEFAULT_ALERT_SOURCE_CHECK_ENABLE;
	lmi.alerts.source_check.info.feature.custom = DEFAULT_ALERT_SOURCE_CHECK_CUSTOM_ENABLE;
	lmi.alerts.source_check.info.feature.reading.value = DEFAULT_ALERT_SOURCE_CHECK_VALUE;
	lmi.alerts.source_check.info.feature.reading.units = DEFAULT_ALERT_SOURCE_CHECK_UNITS;
	lmi.alerts.source_check.info.feature.reading.multiplier = DEFAULT_ALERT_SOURCE_CHECK_MULTIPLIER;
	lmi.alerts.source_check.info.feature.relative_reading = 0;
	strncpy(lmi.alerts.source_check.info.feature.custom_message,DEFAULT_ALERT_SOURCE_CHECK_CUSTOM_MESSAGE,SIZE_CUSTOM_MESSAGE);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALERTS_SOURCE_CHECK_LOCATION, 2, (BYTE *)lmi.alerts.source_check.buffer, SIZE_ALERTS_SOURCE_CHECK);
	
	lmi.alerts.background_check.info.feature.enable = DEFAULT_ALERT_BACKGROUND_CHECK_ENABLE;
	lmi.alerts.background_check.info.feature.custom = DEFAULT_ALERT_BACKGROUND_CHECK_CUSTOM_ENABLE;
	lmi.alerts.background_check.info.feature.reading.value = DEFAULT_ALERT_BACKGROUND_CHECK_VALUE;
	lmi.alerts.background_check.info.feature.reading.units = DEFAULT_ALERT_BACKGROUND_CHECK_UNITS;
	lmi.alerts.background_check.info.feature.reading.multiplier = DEFAULT_ALERT_BACKGROUND_CHECK_MULTIPLIER;
	lmi.alerts.background_check.info.feature.relative_reading = 0;
	strncpy(lmi.alerts.background_check.info.feature.custom_message,DEFAULT_ALERT_BACKGROUND_CHECK_CUSTOM_MESSAGE,SIZE_CUSTOM_MESSAGE);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALERTS_BACKGROUND_CHECK_LOCATION, 2, (BYTE *)lmi.alerts.background_check.buffer, SIZE_ALERTS_BACKGROUND_CHECK);
	
//*	Alarms
	lmi.alarms.low_battery.info.feature.enable = DEFAULT_ALARM_LOW_BATTERY_ENABLE;
	lmi.alarms.low_battery.info.feature.custom = DEFAULT_ALARM_LOW_BATTERY_CUSTOM_ENABLE;
	lmi.alarms.low_battery.info.feature.value = DEFAULT_ALARM_LOW_BATTERY_VALUE;
	strncpy(lmi.alarms.low_battery.info.feature.custom_message,DEFAULT_ALARM_LOW_BATTERY_CUSTOM_MESSAGE,SIZE_CUSTOM_MESSAGE);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALARMS_LOW_BATTERY_LOCATION, 2, (BYTE *)lmi.alarms.low_battery.buffer, SIZE_ALARMS_LOW_BATTERY);
	
	lmi.alarms.low_memory.info.feature.enable = DEFAULT_ALARM_LOW_MEMORY_ENABLE;
	lmi.alarms.low_memory.info.feature.custom = DEFAULT_ALARM_LOW_MEMORY_CUSTOM_ENABLE;
	lmi.alarms.low_memory.info.feature.value = DEFAULT_ALARM_LOW_MEMORY_VALUE;
	strncpy(lmi.alarms.low_memory.info.feature.custom_message,DEFAULT_ALARM_LOW_MEMORY_CUSTOM_MESSAGE,SIZE_CUSTOM_MESSAGE);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALARMS_LOW_MEMORY_LOCATION, 2, (BYTE *)lmi.alarms.low_memory.buffer, SIZE_ALARMS_LOW_MEMORY);
	
	lmi.alarms.calibration_due.info.feature.enable = DEFAULT_ALARM_CALIBRATION_DUE_ENABLE;
	lmi.alarms.calibration_due.info.feature.custom = DEFAULT_ALARM_CALIBRATION_DUE_CUSTOM_ENABLE;
	lmi.alarms.calibration_due.info.feature.value = DEFAULT_ALARM_CALIBRATION_DUE_VALUE;
	strncpy(lmi.alarms.calibration_due.info.feature.custom_message,DEFAULT_ALARM_CALIBRATION_DUE_CUSTOM_MESSAGE,SIZE_CUSTOM_MESSAGE);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALARMS_CALIBRATION_DUE_LOCATION, 2, (BYTE *)lmi.alarms.calibration_due.buffer, SIZE_ALARMS_CALIBRATION_DUE);
	
	lmi.alarms.source_check.info.feature.enable = DEFAULT_ALARM_SOURCE_CHECK_ENABLE;
	lmi.alarms.source_check.info.feature.custom = DEFAULT_ALARM_SOURCE_CHECK_CUSTOM_ENABLE;
	lmi.alarms.source_check.info.feature.reading.value = DEFAULT_ALARM_SOURCE_CHECK_VALUE;
	lmi.alarms.source_check.info.feature.reading.units = DEFAULT_ALARM_SOURCE_CHECK_UNITS;
	lmi.alarms.source_check.info.feature.reading.multiplier = DEFAULT_ALARM_SOURCE_CHECK_MULTIPLIER;
	lmi.alarms.source_check.info.feature.relative_reading = 0;
	strncpy(lmi.alarms.source_check.info.feature.custom_message,DEFAULT_ALARM_SOURCE_CHECK_CUSTOM_MESSAGE,SIZE_CUSTOM_MESSAGE);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALARMS_SOURCE_CHECK_LOCATION, 2, (BYTE *)lmi.alarms.source_check.buffer, SIZE_ALARMS_SOURCE_CHECK);
	
	lmi.alarms.background_check.info.feature.enable = DEFAULT_ALARM_BACKGROUND_CHECK_ENABLE;
	lmi.alarms.background_check.info.feature.custom = DEFAULT_ALARM_BACKGROUND_CHECK_CUSTOM_ENABLE;
	lmi.alarms.background_check.info.feature.reading.value = DEFAULT_ALARM_BACKGROUND_CHECK_VALUE;
	lmi.alarms.background_check.info.feature.reading.units = DEFAULT_ALARM_BACKGROUND_CHECK_UNITS;
	lmi.alarms.background_check.info.feature.reading.multiplier = DEFAULT_ALARM_BACKGROUND_CHECK_MULTIPLIER;
	lmi.alarms.background_check.info.feature.relative_reading = 0;
	strncpy(lmi.alarms.background_check.info.feature.custom_message,DEFAULT_ALARM_BACKGROUND_CHECK_CUSTOM_MESSAGE,SIZE_CUSTOM_MESSAGE);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALARMS_BACKGROUND_CHECK_LOCATION, 2, (BYTE *)lmi.alarms.background_check.buffer, SIZE_ALARMS_BACKGROUND_CHECK);
	
	lmi.alarms.hv_low.info.feature.enable = DEFAULT_ALARM_HV_LOW_ENABLE;
	lmi.alarms.hv_low.info.feature.custom = DEFAULT_ALARM_HV_LOW_CUSTOM_ENABLE;
	lmi.alarms.hv_low.info.feature.value = DEFAULT_ALARM_HV_LOW_VALUE;
	strncpy(lmi.alarms.hv_low.info.feature.custom_message,DEFAULT_ALARM_HV_LOW_CUSTOM_MESSAGE,SIZE_CUSTOM_MESSAGE);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALARMS_HV_LOW_LOCATION, 2, (BYTE *)lmi.alarms.hv_low.buffer, SIZE_ALARMS_HV_LOW);
	
	lmi.alarms.hv_high.info.feature.enable = DEFAULT_ALARM_HV_HIGH_ENABLE;
	lmi.alarms.hv_high.info.feature.custom = DEFAULT_ALARM_HV_HIGH_CUSTOM_ENABLE;
	lmi.alarms.hv_high.info.feature.value = DEFAULT_ALARM_HV_HIGH_VALUE;
	strncpy(lmi.alarms.hv_high.info.feature.custom_message,DEFAULT_ALARM_HV_HIGH_CUSTOM_MESSAGE,SIZE_CUSTOM_MESSAGE);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALARMS_HV_HIGH_LOCATION, 2, (BYTE *)lmi.alarms.hv_high.buffer, SIZE_ALARMS_HV_HIGH);
	
	lmi.alarms.low_reading.info.feature.enable = DEFAULT_ALARM_LOW_READING_ENABLE;
	lmi.alarms.low_reading.info.feature.custom = DEFAULT_ALARM_LOW_READING_CUSTOM_ENABLE;
	lmi.alarms.low_reading.info.feature.reading.value = DEFAULT_ALARM_LOW_READING_VALUE;
	lmi.alarms.low_reading.info.feature.reading.units = DEFAULT_ALARM_LOW_READING_UNITS;
	lmi.alarms.low_reading.info.feature.reading.multiplier = DEFAULT_ALARM_LOW_READING_MULTIPLIER;
	lmi.alarms.low_reading.info.feature.relative_reading = 0;
	lmi.alarms.low_reading.info.time = DEFAULT_ALARM_LOW_READING_TIME;
	strncpy(lmi.alarms.low_reading.info.feature.custom_message,DEFAULT_ALARM_LOW_READING_CUSTOM_MESSAGE,SIZE_CUSTOM_MESSAGE);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALARMS_LOW_READING_LOCATION, 2, (BYTE *)lmi.alarms.low_reading.buffer, SIZE_ALARMS_LOW_READING);

//*	Calibration CONtrol
//	lmi.calibration.controls.info.stream_report_time = DEFAULT_CALIBRATION_CONTROLS_STREAM_TIME;
//	lmi.calibration.controls.info.fixed_range = DEFAULT_CALIBRATION_CONTROLS_RANGE;
//	lmi.calibration.controls.info.stream_data_reading = DEFAULT_CALIBRATION_CONTROLS_STREAM;
//	i2c2ReadPage(i2cADDR_MAIN_E2P_PRI, EEPROM_CALIBRATION_CONTROLS_LOCATION, 2, (BYTE *)lmi.calibration.controls.buffer, SIZE_CALIBRATION_CONTROLS);

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void EEPROM_Load_Calibration_Defaults(void)
//*
//*	Description:
//*		Writes default values to store to EEPROM if TRUE new startup or if the EEPROM is corrupted.
//*
//*	Precondition:
//* 	NTRUEe
//*
//*	Parameters:
//* 	NTRUEe
//*
//*	Return Values:
//* 	NTRUEe
//*
//*	Remarks:
//* 	NTRUEe
//*

void EEPROM_Load_Calibration_Defaults(void)
{
	char index;

//* Calibration - Header Info
	lmi.calibration.header.info.instrument_type = DEFAULT_CALIBRATION_INSTRUMENT_TYPE;
	strncpy((BYTE *)lmi.calibration.header.info.technician, DEFAULT_CALIBRATION_TECHNICIAN, LENGTH_CALIBRATION_TECHNICIAN);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_CALIBRATION_HEADER_LOCATION, 2, (BYTE *)lmi.calibration.header.buffer, SIZE_CALIBRATION_HEADER);

//*	Calibration - Checkout Info
	strncpy((BYTE *)lmi.calibration.checkout.info.technician, DEFAULT_CHECKOUT_TECHNICIAN, LENGTH_CHECKOUT_TECHNICIAN);
	lmi.calibration.checkout.info.date.month = DEFAULT_DATE_MONTH;
	lmi.calibration.checkout.info.date.day = DEFAULT_DATE_DAY;
	lmi.calibration.checkout.info.date.year = DEFAULT_DATE_YEAR;
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_CALIBRATION_CHECKOUT_LOCATION, 2, (BYTE *)lmi.calibration.checkout.buffer, SIZE_CALIBRATION_CHECKOUT);

//* Calibration - CONstants Info
	for(index = 0; index < NUMBER_OF_RANGES; index++)
		lmi.calibration.constants.info.range[index] = DEFAULT_RANGE_CAL_CONSTANT;
	lmi.calibration.constants.info.high_voltage = DEFAULT_HIGH_VOLTAGE_CAL_CONSTANT;
	lmi.calibration.constants.info.battery = DEFAULT_BATTERY_CAL_CONSTANT;
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_CALIBRATION_CONSTANTS_LOCATION, 2, (BYTE *)lmi.calibration.constants.buffer, SIZE_CALIBRATION_CONSTANTS);

//* Calibration - offsets Info
	lmi.calibration.offsets.info.adc_offset = DEFAULT_ADC_OFFSET;
	lmi.calibration.offsets.info.dac_offset = DEFAULT_DAC_OFFSET;
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_CALIBRATION_OFFSETS_LOCATION, 2, (BYTE *)lmi.calibration.offsets.buffer, SIZE_CALIBRATION_OFFSETS);

//* Calibration - High Voltage Board CONstants
	lmi.calibration.high_voltage_board.info.dac_1_offset = DEFAULT_DAC_1_OFFSET;																	// (9DP v1.01.02)
	lmi.calibration.high_voltage_board.info.dac_2_offset = DEFAULT_DAC_2_OFFSET;																	// (9DP v1.01.02)
#ifdef INSTRUMENT_9D																																// (3D v1.00.04)
	lmi.calibration.high_voltage_board.info.dac_1_output_1 = DEFAULT_DAC_1_OUTPUT_1;																// no lTRUEger used (9DP v1.01.02)
	lmi.calibration.high_voltage_board.info.dac_1_output_2 = DEFAULT_DAC_1_OUTPUT_2;																// no lTRUEger used (9DP v1.01.02)
	lmi.calibration.high_voltage_board.info.dac_1_output_3 = DEFAULT_DAC_1_OUTPUT_3;																// no lTRUEger used (9DP v1.01.02)
	lmi.calibration.high_voltage_board.info.dac_1_output_4 = DEFAULT_DAC_1_OUTPUT_4;																// no lTRUEger used (9DP v1.01.02)
	lmi.calibration.high_voltage_board.info.dac_2_output_1 = DEFAULT_DAC_2_OUTPUT_1;																// no lTRUEger used (9DP v1.01.02)
	lmi.calibration.high_voltage_board.info.dac_2_output_2 = DEFAULT_DAC_2_OUTPUT_2;																// no lTRUEger used (9DP v1.01.02)
	lmi.calibration.high_voltage_board.info.dac_2_output_3 = DEFAULT_DAC_2_OUTPUT_3;																// no lTRUEger used (9DP v1.01.02)
	lmi.calibration.high_voltage_board.info.dac_2_output_4 = DEFAULT_DAC_2_OUTPUT_4;																// no lTRUEger used (9DP v1.01.02)
	lmi.calibration.high_voltage_board.info.meter_offset_gain_1 = DEFAULT_METER_OFFSET;
	lmi.calibration.high_voltage_board.info.meter_offset_gain_2 = DEFAULT_METER_OFFSET;
	lmi.calibration.high_voltage_board.info.meter_offset_gain_3 = DEFAULT_METER_OFFSET;
	lmi.calibration.high_voltage_board.info.cal_temperature = DEFAULT_ELECTROMETER_TEMPERATURE;
	lmi.calibration.high_voltage_board.info.temperature_offset_cold = DEFAULT_TEMPERATURE_OFFSET_CORRECTION;
	lmi.calibration.high_voltage_board.info.temperature_offset_hot = DEFAULT_TEMPERATURE_OFFSET_CORRECTION;
	lmi.calibration.high_voltage_board.info.temperature_offset_hot_10 = DEFAULT_TEMPERATURE_OFFSET_CORRECTION;										// (9DP v1.02.07)
	lmi.calibration.high_voltage_board.info.temperature_offset_hot_15 = DEFAULT_TEMPERATURE_OFFSET_CORRECTION;										// (9DP v1.02.07)
	lmi.calibration.high_voltage_board.info.temperature_offset_hot_20 = DEFAULT_TEMPERATURE_OFFSET_CORRECTION;										// (9DP v1.02.07)
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_CALIBRATION_HVB_LOCATION, 2, (BYTE *)lmi.calibration.high_voltage_board.buffer, SIZE_CALIBRATION_HVB);
#else																																				// (3D v1.00.04)
	lmi.calibration.high_voltage_board.info.threshold_1_setpoint_1 = DEFAULT_THRESHOLD_SETPOINT_1;													// (3D v1.00.04)
	lmi.calibration.high_voltage_board.info.threshold_1_setpoint_2 = DEFAULT_THRESHOLD_SETPOINT_2;													// (3D v1.00.04)
	lmi.calibration.high_voltage_board.info.threshold_2_setpoint_1 = DEFAULT_THRESHOLD_SETPOINT_1;													// (3D v1.00.04)
	lmi.calibration.high_voltage_board.info.threshold_2_setpoint_2 = DEFAULT_THRESHOLD_SETPOINT_2;													// (3D v1.00.04)
	lmi.calibration.high_voltage_board.info.threshold_1_offset_1 = DEFAULT_THRESHOLD_OFFSET_1;														// (3D v1.00.04)
	lmi.calibration.high_voltage_board.info.threshold_1_offset_2 = DEFAULT_THRESHOLD_OFFSET_2;														// (3D v1.00.04)
	lmi.calibration.high_voltage_board.info.threshold_2_offset_1 = DEFAULT_THRESHOLD_OFFSET_1;														// (3D v1.00.04)
	lmi.calibration.high_voltage_board.info.threshold_2_offset_2 = DEFAULT_THRESHOLD_OFFSET_2;														// (3D v1.00.04)
	lmi.calibration.high_voltage_board.info.window_1_setpoint_1 = DEFAULT_WINDOW_SETPOINT_1;														// (3D v1.00.04)
	lmi.calibration.high_voltage_board.info.window_1_setpoint_2 = DEFAULT_WINDOW_SETPOINT_2;														// (3D v1.00.04)
	lmi.calibration.high_voltage_board.info.window_2_setpoint_1 = DEFAULT_WINDOW_SETPOINT_1;														// (3D v1.00.04)
	lmi.calibration.high_voltage_board.info.window_2_setpoint_2 = DEFAULT_WINDOW_SETPOINT_2;														// (3D v1.00.04)
	lmi.calibration.high_voltage_board.info.window_1_offset_1 = DEFAULT_WINDOW_OFFSET_1;															// (3D v1.00.04)
	lmi.calibration.high_voltage_board.info.window_1_offset_2 = DEFAULT_WINDOW_OFFSET_2;															// (3D v1.00.04)
	lmi.calibration.high_voltage_board.info.window_2_offset_1 = DEFAULT_WINDOW_OFFSET_1;															// (3D v1.00.04)
	lmi.calibration.high_voltage_board.info.window_2_offset_2 = DEFAULT_WINDOW_OFFSET_2;															// (3D v1.00.04)
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_CALIBRATION_HVB_LOCATION, 2, (BYTE *)lmi.calibration.high_voltage_board.buffer, SIZE_CALIBRATION_HVB_OTHER);
#endif																																				// (3D v1.00.04)

//* Calibration - CONtrols
	lmi.calibration.controls.info.fixed_range = DEFAULT_CALIBRATION_CONTROLS_RANGE;
	lmi.calibration.controls.info.stream_data_reading = DEFAULT_CALIBRATION_CONTROLS_STREAM;
	lmi.calibration.controls.info.stream_report_time = DEFAULT_CALIBRATION_CONTROLS_STREAM_TIME;
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_CALIBRATION_CONTROLS_LOCATION, 2, (BYTE *)lmi.calibration.controls.buffer, SIZE_CALIBRATION_CONTROLS);

//* Calibration - CONfig
	lmi.calibration.config.info.uR_filter = DEFAULT_CALIBRATION_CONFIG_uR_FILTER;
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_CALIBRATION_CONFIG_LOCATION, 2, (BYTE *)lmi.calibration.config.buffer, SIZE_CALIBRATION_CONFIG);

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void EEPROM_Function_Defaults(void)
//*
//*	Description:
//*		Writes function defaults to function index.
//*
//*	Precondition:
//* 	NTRUEe
//*
//*	Parameters:
//* 	NTRUEe
//*
//*	Return Values:
//* 	NTRUEe
//*
//*	Remarks:
//* 	NTRUEe
//*

void EEPROM_Function_Defaults(BYTE function_index, FUNCTION_TYPE function)
{
	switch(function)
	{
		case INTEGRATE:
		{
			lmi.function[function_index].info.integrate.detector = DEFAULT_INTEGRATE_DETECTOR;
			lmi.function[function_index].info.integrate.units = DEFAULT_INTEGRATE_UNITS;
			lmi.function[function_index].info.integrate.multiplier = DEFAULT_INTEGRATE_MULTIPLIER;
			lmi.function[function_index].info.integrate.digits = DEFAULT_INTEGRATE_DIGITS;
			lmi.function[function_index].info.integrate.range_lock = DEFAULT_INTEGRATE_RANGE_LOCK;
			lmi.function[function_index].info.integrate.time = DEFAULT_INTEGRATE_TIME;
			break;
		}
		case PEAK_RATE:
		{
			lmi.function[function_index].info.peak_rate.digits = DEFAULT_PEAKRATE_DIGITS;
			lmi.function[function_index].info.peak_rate.element_range_lock = DEFAULT_PEAKRATE_RANGELOCK;
			break;
		}
		case NO_FUNCTION:
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
//*	EEPROM_Write_All Helpers:
//*      showAddress(address)
//*      getAddress(start_location, detector, view)
//*      showDetectorAndView(detector, view)
//*
//*	Description:
//*		Displays runtime information during EEPROM_Write_All function
//*
//*     showAddress() draws the current EEPROM address being written
//*     getAddress() finds the write address based TRUE the parameter, detector, and view
//*     showDetectorAndView() shows the current 1-based index of the detector and view
//*
//*	Precondition:
//* 	Text display properties and functions must be included somewhere in the include hierarchy for these to work.
//*     Also, the EEPROM chip must be set up for writing at the targeted addresses.
//*     NUMBER_OF_DETECTORS, NUMBER_OF_VIEWS, TOTAL_DETECTOR, TOTAL_VIEW, and all parameter default locations
//*           must be defined with valid data.
//*
//*	Parameters:
//* 	showAddress:
//*       unsigned short address:       The address to display in the format "Address: %x"
//*     getAddress:
//*       unsigned int start_location:  The pre-defined parameter location for detector 0 view 0
//*       unsigned char detector:       The 0-based index of the current detector
//*       unsigned char view:           The 0-based index of the current view
//*     showDetectorAndView:
//*       unsigned char detector_index: The 0-based index of the current detector
//*       unsigned char view_index:     The 0-based index of the current view
//*
//*	Return Values:
//* 	showAddress and showDetectorAndView: nTRUEe
//*     getAddress: unsigned short address, the address in EEPROM to write
//*
//*	Remarks:
//* 	n2: Created
//*         show functions will TRUEly be called if the compiler macro TEST_EEPROM_MODE is defined.
//*

extern char DisplayMessage[29];

void showAddress(unsigned short address)
{
    char stringchar[51];
    sprintf(stringchar, "Address: %x", address);                                                // Set display format
    Use_Custom_Message(stringchar, MSG_ADJ_NO_HEADING, NORMAL_MESSAGE, LEFT_JUSTIFY);			// Display address TRUE the screen
}

unsigned short getAddress(unsigned int start_location, unsigned char detector, unsigned char view)
{
    unsigned short address = start_location + (detector * TOTAL_DETECTOR) + (view * TOTAL_VIEW); // offset start by detector and view, basically array indexing
#ifdef TEST_EEPROM_MODE
    showAddress(address);                                                                        // Display address if in TEST_EEPROM_MODE
#endif
    return address;
}

void showDetectorAndView(unsigned char detector_index, unsigned char view_index)
{
    char stringchar[51];
    sprintf((char *)DisplayMessage, "Detector %d", detector_index + 1);                         // Set header format (Detector)
    sprintf(stringchar, "View %d", view_index + 1);                                             // Set message format (View)
    Use_Custom_Message(stringchar, MSG_FULL_HEADING, NORMAL_MESSAGE, CENTER_JUSTIFY);			// Display header and message
}


//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void EEPROM_Write_All(void)
//*
//*	Description:
//*		Writes all values to the EEPROM.
//*
//*	Precondition:
//* 	NTRUEe
//*
//*	Parameters:
//* 	NTRUEe
//*
//*	Return Values:
//* 	NTRUEe
//*
//*	Remarks:
//* 	NTRUEe
//*



void EEPROM_Write_All(void)
{
	unsigned char detector_index;
	unsigned char view_index;
	unsigned char index;
	unsigned short address;
    char stringchar[51];

//*	Settings
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_PRODUCT_LOCATION, 2, (BYTE *)lmi.settings.product.buffer, SIZE_PRODUCT);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_LANGUAGE_LOCATION, 2, (BYTE *)lmi.settings.language.buffer, SIZE_LANGUAGE);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_CALENDAR_LOCATION, 2, (BYTE *)lmi.settings.calendar.buffer, SIZE_DATE);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_CLOCK_LOCATION, 2, (BYTE *)lmi.settings.clock.buffer, SIZE_TIME);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_PASSWORDS_LOCATION, 2, (BYTE *)lmi.settings.passwords.buffer, SIZE_PASSWORDS);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_SETTINGS_MISC_LOCATION, 2, (BYTE *)lmi.settings.misc.buffer, SIZE_SETTINGS_MISC);

//* Features
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_SOURCE_CHECK_LOCATION, 2, (BYTE *)lmi.features.source.buffer, SIZE_SOURCE_CHECK);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_BACKGROUND_CHECK_LOCATION, 2, (BYTE *)lmi.features.background.buffer, SIZE_BACKGROUND_CHECK);

//	if(lmi.features.integrate.info.save_on_shutdown == 0)																					// hidden by (9DP v1.01.04)
//	{																																		// hidden by (9DP v1.01.04)
//		lmi.features.integrate.info.current_reading.value = 0.0;																			// hidden by (9DP v1.01.04)
//		lmi.features.integrate.info.current_reading.units = DEFAULT_INTEGRATE_UNITS;														// hidden by (9DP v1.01.04)
//		lmi.features.integrate.info.current_reading.multiplier = DEFAULT_INTEGRATE_MULTIPLIER;												// hidden by (9DP v1.01.04)
//	}																																		// hidden by (9DP v1.01.04)

	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_INTEGRATE_LOCATION, 2, (BYTE *)lmi.features.integrate.buffer, SIZE_INTEGRATE);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_DATA_LOGGING_LOCATION, 2, (BYTE *)lmi.features.data_logging.buffer, SIZE_DATA_LOGGING);

//* CONtrols
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_BACKLIGHT_LOCATION, 2, (BYTE *)lmi.controls.backlight.buffer, SIZE_BACKLIGHT);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_AUDIO_LOCATION, 2, (BYTE *)lmi.controls.audio.buffer, SIZE_AUDIO);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_GPS_LOCATION, 2, (BYTE *)lmi.controls.gps.buffer, SIZE_GPS);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_BLUETOOTH_LOCATION, 2, (BYTE *)lmi.controls.bluetooth.buffer, SIZE_BLUETOOTH);

//* Display
#ifdef EEPROM_DISPLAY_PROPERTIES_LOCATION
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_DISPLAY_PROPERTIES_LOCATION, 2, (BYTE *)lmi.display.properties.buffer, SIZE_DISPLAY_PROPERTIES);
#endif

//* Display - Detectors
	for(detector_index = 0; detector_index < NUMBER_OF_DETECTORS; detector_index++)
	{
#ifdef EEPROM_DETECTOR_PROPERTIES_LOCATION
		address = EEPROM_DETECTOR_PROPERTIES_LOCATION + (detector_index * TOTAL_DETECTOR);
		i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].properties.buffer, SIZE_DETECTOR_PROPERTIES);
#endif
#ifdef EEPROM_DETECTOR_CALIBRATION_LOCATION
		address = EEPROM_DETECTOR_CALIBRATION_LOCATION + (detector_index * TOTAL_DETECTOR);
		i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].calibration.buffer, SIZE_DETECTOR_CALIBRATION);
#endif
#ifdef EEPROM_DETECTOR_VIEW_SETUP_LOCATION
		address = EEPROM_DETECTOR_VIEW_SETUP_LOCATION + (detector_index * TOTAL_DETECTOR);
		i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].view_setup.buffer, SIZE_DETECTOR_VIEW_SETUP);
#endif


        

		for (view_index = 0; view_index < NUMBER_OF_VIEWS; view_index++)
		{
#ifdef TEST_EEPROM_MODE                                        // n2 Show diagnostic information indicating current detector and view if in TEST_EEPROM_MODE
            showCompass();                                     // n2 Show compass icon
            showDetectorAndView(detector_index, view_index);   // n2 Show detector and view
#endif
            address = getAddress(EEPROM_APPEARANCE_LOCATION, detector_index, view_index);   // n2 Moved indexing function to getAddress to facilitate diagnostics
			i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].view[view_index].appearance.buffer, SIZE_APPEARANCE);
			address = getAddress(EEPROM_ELEMENT_HEADER_LOCATION, detector_index, view_index);
			i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].view[view_index].element_header.buffer, SIZE_ELEMENT_HEADER);
			address = getAddress(EEPROM_ELEMENT_LOCATION, detector_index, view_index);
			i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].view[view_index].element.buffer, SIZE_ELEMENT);
			address = getAddress(EEPROM_VIEW_FUNCTIONS_LOCATION, detector_index, view_index);
			i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].view[view_index].functions.buffer, SIZE_VIEW_FUNCTIONS);
			address = getAddress(EEPROM_ALERTS_RADIATION_LOCATION, detector_index, view_index);
			i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].view[view_index].alerts_radiation.buffer, SIZE_ALERTS_RADIATION);
			address = getAddress(EEPROM_ALERTS_INTEGRATE_LOCATION, detector_index, view_index);
			i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].view[view_index].alerts_integrate.buffer, SIZE_ALERTS_INTEGRATE);
			address = getAddress(EEPROM_ALERTS_FUNCTION_LOCATION, detector_index, view_index);
			i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].view[view_index].alerts_function.buffer, SIZE_ALERTS_FUNCTION);
			address = getAddress(EEPROM_ALARMS_RADIATION_LOCATION, detector_index, view_index);
			i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].view[view_index].alarms_radiation.buffer, SIZE_ALARMS_RADIATION);
			address = getAddress(EEPROM_ALARMS_INTEGRATE_LOCATION, detector_index, view_index);
			i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].view[view_index].alarms_integrate.buffer, SIZE_ALARMS_INTEGRATE);
			address = getAddress(EEPROM_ALARMS_FUNCTION_LOCATION, detector_index, view_index);
			i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].view[view_index].alarms_function.buffer, SIZE_ALARMS_FUNCTION);
//* added (3D v1.00.03)
#ifdef EEPROM_DETECTOR_VIEW_CALIBRATION_LOCATION
			address = EEPROM_VIEW_CALIBRATION_LOCATION + (detector_index * TOTAL_DETECTOR) + (view_index * TOTAL_VIEW);
			i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].view[view_index].calibration.buffer, SIZE_VIEW_CALIBRATION);
#endif
//* end add
#ifdef TEST_EEPROM_MODE         // n2 If compass icon was displayed, hide it now
                hideCompass();
#endif
		}

        
	}


//*	Alerts
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALERTS_LOW_BATTERY_LOCATION, 2, (BYTE *)lmi.alerts.low_battery.buffer, SIZE_ALERTS_LOW_BATTERY);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALERTS_LOW_MEMORY_LOCATION, 2, (BYTE *)lmi.alerts.low_memory.buffer, SIZE_ALERTS_LOW_MEMORY);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALERTS_CALIBRATION_DUE_LOCATION, 2, (BYTE *)lmi.alerts.calibration_due.buffer, SIZE_ALERTS_CALIBRATION_DUE);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALERTS_SOURCE_CHECK_LOCATION, 2, (BYTE *)lmi.alerts.source_check.buffer, SIZE_ALERTS_SOURCE_CHECK);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALERTS_BACKGROUND_CHECK_LOCATION, 2, (BYTE *)lmi.alerts.background_check.buffer, SIZE_ALERTS_BACKGROUND_CHECK);
	
//*	Alarms
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALARMS_LOW_BATTERY_LOCATION, 2, (BYTE *)lmi.alarms.low_battery.buffer, SIZE_ALARMS_LOW_BATTERY);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALARMS_LOW_MEMORY_LOCATION, 2, (BYTE *)lmi.alarms.low_memory.buffer, SIZE_ALARMS_LOW_MEMORY);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALARMS_CALIBRATION_DUE_LOCATION, 2, (BYTE *)lmi.alarms.calibration_due.buffer, SIZE_ALARMS_CALIBRATION_DUE);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALARMS_SOURCE_CHECK_LOCATION, 2, (BYTE *)lmi.alarms.source_check.buffer, SIZE_ALARMS_SOURCE_CHECK);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALARMS_BACKGROUND_CHECK_LOCATION, 2, (BYTE *)lmi.alarms.background_check.buffer, SIZE_ALARMS_BACKGROUND_CHECK);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALARMS_HV_LOW_LOCATION, 2, (BYTE *)lmi.alarms.hv_low.buffer, SIZE_ALARMS_HV_LOW);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALARMS_HV_HIGH_LOCATION, 2, (BYTE *)lmi.alarms.hv_high.buffer, SIZE_ALARMS_HV_HIGH);
	i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_ALARMS_LOW_READING_LOCATION, 2, (BYTE *)lmi.alarms.low_reading.buffer, SIZE_ALARMS_LOW_READING);


    

//* Functions
	for (index = 0; index < NUMBER_OF_FUNCTIONS; index++)
	{
		address = EEPROM_FUNCTIONS_LOCATION + (index * TOTAL_FUNCTION);
		i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.function[index].buffer, SIZE_FUNCTION);
	}


	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void EEPROM_Set_Modes(void)
//*
//*	Description:
//* 	After reading the parameters from the EEPROM, this routine will set the appropriate operating
//*		modes.
//*
//*	Precondition:
//* 	NTRUEe
//*
//*	Parameters:
//* 	NTRUEe
//*
//*	Return Values:
//* 	NTRUEe
//*
//*	Remarks:
//* 	NTRUEe
//*

void EEPROM_Set_Modes(void)
{
	UINT8 index;
	UINT16 value;

// Features - Integrate
	switch(lmi.features.integrate.info.powerup_mode)
	{
		case POWERUP_OFF:
		{
			lmi.features.integrate.info.operating_mode = FALSE;
			break;
		}
		case POWERUP_ON:
		{
			lmi.features.integrate.info.operating_mode = TRUE;
			break;
		}
		case POWERUP_DEFAULT:
		{
			lmi.features.integrate.info.operating_mode = DEFAULT_INTEGRATE_OPERATING_MODE;
			break;
		}
	}

// Features - Data Logging
	switch(lmi.features.data_logging.info.powerup_mode)
	{
		case POWERUP_OFF:
		{
			lmi.features.data_logging.info.operating_mode = FALSE;
			break;
		}
		case POWERUP_ON:
		{
			lmi.features.data_logging.info.operating_mode = TRUE;
			break;
		}
		case POWERUP_DEFAULT:
		{
			lmi.features.data_logging.info.operating_mode = DEFAULT_DATA_LOGGING_OPERATING_MODE;
			break;
		}
	}

// CONtrols - Backlight
	switch(lmi.controls.backlight.info.powerup_mode)
	{
		case BK_LIGHT_OFF:
		case BK_LIGHT_ON:
		case BK_LIGHT_AUTO:
		case BK_LIGHT_TEMP_ON:
		{
			lmi.controls.backlight.info.operating_mode = lmi.controls.backlight.info.powerup_mode;
			break;
		}
		case BK_LIGHT_DEFAULT:
		{
			lmi.controls.backlight.info.operating_mode = DEFAULT_BACKLIGHT_OPERATING_MODE;
			break;
		}
	}
	
	Backlight_Mode = lmi.controls.backlight.info.operating_mode;

//* CONtrols - Audio
	switch(lmi.controls.audio.info.powerup_mode)
	{
		case POWERUP_OFF:
		{
			lmi.controls.audio.info.operating_mode = AUDIO_0;
			break;
		}
		case POWERUP_ON:
		{
			lmi.controls.audio.info.operating_mode = AUDIO_40;
			break;
		}
		case POWERUP_DEFAULT:
		{
			lmi.controls.audio.info.operating_mode = DEFAULT_AUDIO_OPERATING_MODE;
			break;
		}
	}

	Audio_Operating_Mode = lmi.controls.audio.info.operating_mode;
	Set_Audio_Tone();

//*	CONtrols - GPS
	switch(lmi.controls.gps.info.powerup_mode)
	{
		case POWERUP_OFF:
		{
			lmi.controls.gps.info.operating_mode = FALSE;
			break;
		}
		case POWERUP_ON:
		{
			lmi.controls.gps.info.operating_mode = TRUE;
			break;
		}
		case POWERUP_DEFAULT:
		{
			lmi.controls.gps.info.operating_mode = DEFAULT_GPS_OPERATING_MODE;
			break;
		}
	}
	
//*	CONtrols - Bluetooth
	switch(lmi.controls.bluetooth.info.powerup_mode)
	{
		case POWERUP_OFF:
		{
			lmi.controls.bluetooth.info.operating_mode = FALSE;
			break;
		}
		case POWERUP_ON:
		{
			lmi.controls.bluetooth.info.operating_mode = TRUE;
			break;
		}
		case POWERUP_DEFAULT:
		{
			lmi.controls.bluetooth.info.operating_mode = DEFAULT_BLUETOOTH_OPERATING_MODE;
			break;
		}
	}

//* Display - Detector
#ifdef EEPROM_DISPLAY_PROPERTIES_LOCATION
	switch(lmi.display.properties.info.powerup_mode)
	{
		case INTERNAL:
		case DET_1:
		case DET_2:
		case DET_3:
		case DET_4:
		{
			lmi.display.properties.info.current_detector = lmi.display.properties.info.powerup_mode;
			break;
		}
		case DEFAULT_DET:
		{
			lmi.display.properties.info.current_detector = DEFAULT_DETECTOR;
			break;
		}
	}
#else
	lmi.display.properties.info.powerup_mode = DEFAULT_DETECTOR_POWERUP_MODE;
	lmi.display.properties.info.current_detector = DEFAULT_DETECTOR;
#endif

	Current_Detector = lmi.display.properties.info.current_detector;

//* Display - Detector - View
	switch(lmi.display.detector[Current_Detector].view_setup.info.powerup_mode)
	{
		case VIEW_1:
		case VIEW_2:
		case VIEW_3:
		case VIEW_4:
		{
			lmi.display.detector[Current_Detector].view_setup.info.current_view = lmi.display.detector[Current_Detector].view_setup.info.powerup_mode;
			break;
		}
		case VIEW_DEFAULT:
		{
			lmi.display.detector[Current_Detector].view_setup.info.current_view = DEFAULT_CURRENT_VIEW;
			break;
		}
	}

	Current_View = lmi.display.detector[Current_Detector].view_setup.info.current_view;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void EEPROM_Assign_Global_Variables(void)
//*
//*	Description:
//* 	Read all of the EEPROM stored parameters
//*
//*	Precondition:
//* 	NTRUEe
//*
//*	Parameters:
//* 	NTRUEe
//*
//*	Return Values:
//* 	NTRUEe
//*
//*	Remarks:
//* 	NTRUEe
//*

void EEPROM_Assign_Global_Variables(void)
{
	short multiplier_index;
	float multiplier;

	if(lmi.features.integrate.info.save_on_shutdown == 0)																																	// moved here by (9DP v1.01.05)
	{
		lmi.features.integrate.info.current_reading.value = 0.0;																															// moved here by (9DP v1.01.05)
		Rate_Accumulation = 0.0;																																							// (9DP v1.01.10)
	}
	else																																													// (9DP v1.01.10)
	{																																														// (9DP v1.01.10)
		multiplier_index = (unsigned char) RelativeMultiplier(lmi.features.integrate.info.current_reading.multiplier, (short) DEFAULT_DOSE_MULTIPLIER);										// (9DP v1.01.12)
		multiplier = MultiplierConversion(multiplier_index);																																// (9DP v1.01.10)
		Rate_Accumulation =	UnitConversions((lmi.features.integrate.info.current_reading.value * multiplier), lmi.features.integrate.info.current_reading.units, Default_Dose_Units);		// (9DP v1.01.10)
	}																																														// (9DP v1.01.10)

	Current_Function_Index = lmi.display.detector[Current_Detector].view[Current_View].functions.info.function[lmi.display.detector[Current_Detector].view[Current_View].functions.info.current_function].function_index;

	foreground = Designate_Color(lmi.display.detector[Current_Detector].view[Current_View].appearance.info.foreground_color);																// assign foreground color
	background = Designate_Color(lmi.display.detector[Current_Detector].view[Current_View].appearance.info.background_color);																// assign background color	

	uR_filter = lmi.calibration.config.info.uR_filter;

	Update_Alerts_Alarms(Current_Detector, Current_View);																																	// (9DP v1.01.08)

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	END EEPROM Process Functions
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//* 
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	short Designate_Color(short)
//*
//*	Description:
//* 	
//*
//*	Precondition:
//* 	NTRUEe
//*
//*	Parameters:
//* 	NTRUEe
//*
//*	Return Values:
//* 	NTRUEe
//*
//*	Remarks:
//* 	NTRUEe
//*

short Designate_Color(short selected_color)
{
	switch(selected_color)																			// return the appropriate RGB color for the selected color
	{
		case BLACK_EEPROM:																			// 0
			return	BLACK;
		case BRIGHTBLUE_EEPROM:																		// 1
			return BRIGHTBLUE;
		case BRIGHTGREEN_EEPROM:																	// 2
			return BRIGHTGREEN;
		case BRIGHTCYAN_EEPROM:																		// 3
			return BRIGHTCYAN;
		case BRIGHTRED_EEPROM:																		// 4
			return BRIGHTRED;
		case BRIGHTMAGENTA_EEPROM: 																	// 5
			return BRIGHTMAGENTA;
		case BRIGHTYELLOW_EEPROM:																	// 6
			return BRIGHTYELLOW;
		case BLUE_EEPROM:																			// 7
			return BLUE;
		case GREEN_EEPROM:																			// 8 
			return GREEN;
		case CYAN_EEPROM:																			// 9
			return CYAN;
		case RED_EEPROM:																			// 10
			return RED;
		case MAGENTA_EEPROM:																		// 11
			return MAGENTA;
		case BROWN_EEPROM:																			// 12
			return BROWN;
		case LIGHTGRAY_EEPROM:																		// 13
			return LIGHTGRAY;
		case DARKGRAY_EEPROM:																		// 14
			return DARKGRAY;
		case LIGHTBLUE_EEPROM:																		// 15
			return LIGHTBLUE;
		case LIGHTGREEN_EEPROM:																		// 16
			return LIGHTGREEN;
		case LIGHTCYAN_EEPROM:																		// 17
			return LIGHTCYAN;
		case LIGHTRED_EEPROM:																		// 18
			return LIGHTRED;
		case LIGHTMAGENTA_EEPROM:																	// 19	
			return LIGHTMAGENTA;
		case YELLOW_EEPROM:																			// 20
			return YELLOW;
		case WHITE_EEPROM:																			// 21
			return WHITE;
		case GRAY0_EEPROM:																			// 22
			return GRAY0;
		case GRAY1_EEPROM:																			// 23
			return GRAY1;
		case GRAY2_EEPROM:																			// 24
			return GRAY2;
		case GRAY3_EEPROM:																			// 25
			return GRAY3;
		case GRAY4_EEPROM:																			// 26
			return GRAY4;
		case GRAY5_EEPROM:																			// 27
			return GRAY5;
		case GRAY6_EEPROM:																			// 28
			return GRAY6;
		default:																					// > 28
			return WHITE;
	}
	return -1;
}


//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL Change_Model_Number(BYTE new_model_number, char *new_model)
//*
//*	Description:
//* 	
//*
//*	Precondition:
//* 	NTRUEe
//*
//*	Parameters:
//* 	NTRUEe
//*
//*	Return Values:
//* 	NTRUEe
//*
//*	Remarks:
//* 	NTRUEe
//*

BOOL Change_Model_Number(BYTE new_model_number, char *new_model)
{
	UINT8 view_index, detector_index;
	UINT16 conversion;
	unsigned short address;

	switch(DEFAULT_MODEL_NUMBER)
	{
		case MODEL_9DP:
		{
			switch(new_model_number)
			{
				case MODEL_9DP:
				case MODEL_9DP_PLUS:
				case MODEL_9DP_1:
				case MODEL_9DP_1_PLUS:
				case MODEL_9DLP:
				case MODEL_9DLP_PLUS:
				case MODEL_V1:																																		// model number for RTI (9DP v1.01.21)
				case MODEL_9DP_STAR:																																// model number for 9DP* (9DP v1.01.24)
//				case MODEL_9DP_STAR_PLUS:																															// model number for 9DP*+ (9DP v1.01.24)
				case MODEL_9DP_2:																																	// model number for 9DP-2 (9DP v1.01.24)
//				case MODEL_9DP_2_PLUS:																																// model number for 9DP-2+ (9DP v1.01.24)
				{
					break;
				}
				default:
				{
					return FALSE;
				}
			}
			break;
		}
		case MODEL_3D:
		{
			switch(new_model_number)
			{
				case MODEL_3D:
				case MODEL_3D_PLUS:
				{
					break;
				}
				default:
				{
					return FALSE;
				}
			}
			break;
		}
		case MODEL_276D:
		{
			switch(new_model_number)
			{
				case MODEL_276D:
				{
					break;
				}
				default:
				{
					return FALSE;
				}
			}
			break;
		}
		default:
		{
			return FALSE;
		}
	}

	if(lmi.settings.product.info.model_number != new_model_number)
	{
		lmi.settings.product.info.model_number = new_model_number;
		strncpy(lmi.settings.product.info.model, new_model, LENGTH_MODEL);
		i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_PRODUCT_LOCATION, 2, (BYTE *)lmi.settings.product.buffer, SIZE_PRODUCT);
		switch(new_model_number)
		{
			case MODEL_9DP:
			case MODEL_9DP_PLUS:
			case MODEL_9DLP:
			case MODEL_9DLP_PLUS:
			case MODEL_3D:
			case MODEL_3D_PLUS:
			case MODEL_276D:
			case MODEL_V1:																																			// model number for RTI (9DP v1.01.21)
			case MODEL_9DP_STAR:																																	// model number for 9DP* (9DP v1.01.24)
//			case MODEL_9DP_STAR_PLUS:																																// model number for 9DP*+ (9DP v1.01.24)
			case MODEL_9DP_2:																																		// model number for 9DP-2 (9DP v1.01.24)
//			case MODEL_9DP_2_PLUS:																																	// model number for 9DP-2+ (9DP v1.01.24)
			{
				for(detector_index = 0; detector_index < NUMBER_OF_DETECTORS; detector_index++)																		// (9DP v1.01.05)
				{
					lmi.display.detector[detector_index].view[0].element.info.arc.control.end_point = DEFAULT_ELEMENT_1_ARC_END_POINT;
					lmi.display.detector[detector_index].view[0].element.info.arc.control.multiplier = DEFAULT_ELEMENT_1_ARC_MULTIPLIER;
					lmi.display.detector[detector_index].view[1].element.info.arc.control.end_point = DEFAULT_ELEMENT_2_ARC_END_POINT;
					lmi.display.detector[detector_index].view[1].element.info.arc.control.multiplier = DEFAULT_ELEMENT_2_ARC_MULTIPLIER;
					lmi.display.detector[detector_index].view[2].element.info.arc.control.end_point = DEFAULT_ELEMENT_3_ARC_END_POINT;
					lmi.display.detector[detector_index].view[2].element.info.arc.control.multiplier = DEFAULT_ELEMENT_3_ARC_MULTIPLIER;
					lmi.display.detector[detector_index].view[3].element.info.arc.control.end_point = DEFAULT_ELEMENT_4_ARC_END_POINT;
					lmi.display.detector[detector_index].view[3].element.info.arc.control.multiplier = DEFAULT_ELEMENT_4_ARC_MULTIPLIER;
				}																																					// (9DP v1.01.05)
				break;
			}
			case MODEL_9DP_1:
			case MODEL_9DP_1_PLUS:
			{
				for(detector_index = 0; detector_index < NUMBER_OF_DETECTORS; detector_index++)																		// (9DP v1.01.05)
				{																																					// (9DP v1.01.05)
					if((UINT16) DEFAULT_ELEMENT_1_ARC_END_POINT > 99)																								// (9DP v1.01.05)
					{																																				// (9DP v1.01.05)
						lmi.display.detector[detector_index].view[0].element.info.arc.control.end_point = ((UINT16) DEFAULT_ELEMENT_1_ARC_END_POINT * 10) / 1000;	// (9DP v1.01.05)
						lmi.display.detector[detector_index].view[0].element.info.arc.control.multiplier = DEFAULT_ELEMENT_1_ARC_MULTIPLIER + 1;					// (9DP v1.01.05)
					}																																				// (9DP v1.01.05)
					else																																			// (9DP v1.01.05)
					{																																				// (9DP v1.01.05)
						lmi.display.detector[detector_index].view[0].element.info.arc.control.end_point = ((UINT16) DEFAULT_ELEMENT_1_ARC_END_POINT * 10);			// (9DP v1.01.05)
						lmi.display.detector[detector_index].view[0].element.info.arc.control.multiplier = DEFAULT_ELEMENT_1_ARC_MULTIPLIER;						// (9DP v1.01.05)
					}																																				// (9DP v1.01.05)
					if((UINT16) DEFAULT_ELEMENT_2_ARC_END_POINT > 99)																								// (9DP v1.01.05)
					{																																				// (9DP v1.01.05)
						lmi.display.detector[detector_index].view[1].element.info.arc.control.end_point = ((UINT16) DEFAULT_ELEMENT_2_ARC_END_POINT * 10) / 1000;	// (9DP v1.01.05)
						lmi.display.detector[detector_index].view[1].element.info.arc.control.multiplier = DEFAULT_ELEMENT_2_ARC_MULTIPLIER + 1;					// (9DP v1.01.05)
					}																																				// (9DP v1.01.05)
					else																																			// (9DP v1.01.05)
					{																																				// (9DP v1.01.05)
						lmi.display.detector[detector_index].view[1].element.info.arc.control.end_point = ((UINT16) DEFAULT_ELEMENT_2_ARC_END_POINT * 10);			// (9DP v1.01.05)
						lmi.display.detector[detector_index].view[1].element.info.arc.control.multiplier = DEFAULT_ELEMENT_2_ARC_MULTIPLIER;						// (9DP v1.01.05)
					}																																				// (9DP v1.01.05)
					if((UINT16) DEFAULT_ELEMENT_3_ARC_END_POINT > 99)																								// (9DP v1.01.05)
					{																																				// (9DP v1.01.05)
						lmi.display.detector[detector_index].view[2].element.info.arc.control.end_point = ((UINT16) DEFAULT_ELEMENT_3_ARC_END_POINT * 10) / 1000;	// (9DP v1.01.05)
						lmi.display.detector[detector_index].view[2].element.info.arc.control.multiplier = DEFAULT_ELEMENT_3_ARC_MULTIPLIER + 1;					// (9DP v1.01.05)
					}																																				// (9DP v1.01.05)
					else																																			// (9DP v1.01.05)
					{																																				// (9DP v1.01.05)
						lmi.display.detector[detector_index].view[2].element.info.arc.control.end_point = ((UINT16) DEFAULT_ELEMENT_3_ARC_END_POINT * 10);			// (9DP v1.01.05)
						lmi.display.detector[detector_index].view[2].element.info.arc.control.multiplier = DEFAULT_ELEMENT_3_ARC_MULTIPLIER;						// (9DP v1.01.05)
					}																																				// (9DP v1.01.05)
					if((UINT16) DEFAULT_ELEMENT_4_ARC_END_POINT > 99)																								// (9DP v1.01.05)
					{																																				// (9DP v1.01.05)
						lmi.display.detector[detector_index].view[3].element.info.arc.control.end_point = ((UINT16) DEFAULT_ELEMENT_4_ARC_END_POINT * 10) / 1000;	// (9DP v1.01.05)
						lmi.display.detector[detector_index].view[3].element.info.arc.control.multiplier = DEFAULT_ELEMENT_4_ARC_MULTIPLIER + 1;					// (9DP v1.01.05)
					}																																				// (9DP v1.01.05)
					else																																			// (9DP v1.01.05)
					{																																				// (9DP v1.01.05)
						lmi.display.detector[detector_index].view[3].element.info.arc.control.end_point = ((UINT16) DEFAULT_ELEMENT_4_ARC_END_POINT * 10);			// (9DP v1.01.05)
						lmi.display.detector[detector_index].view[3].element.info.arc.control.multiplier = DEFAULT_ELEMENT_4_ARC_MULTIPLIER;						// (9DP v1.01.05)
					}																																				// (9DP v1.01.05)
				}																																					// (9DP v1.01.05)
				break;
			}
			default:
			{
				return FALSE;
			}
		}

		for(detector_index = 0; detector_index < NUMBER_OF_DETECTORS; detector_index++)
		{
			for(view_index = 0; view_index < NUMBER_OF_VIEWS; view_index++)
			{
//				address = EEPROM_ELEMENT_LOCATION + (index * TOTAL_VIEW);
//				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, lmi.display.detector[Current_Detector].view[index].element.buffer, SIZE_ELEMENT);
				address = EEPROM_ELEMENT_LOCATION + (detector_index * TOTAL_DETECTOR) + (view_index * TOTAL_VIEW);													// (9DP v1.01.05)
				i2c2WritePage(i2cADDR_MAIN_E2P_PRI, address, 2, (BYTE *)lmi.display.detector[detector_index].view[view_index].element.buffer, SIZE_ELEMENT);		// (9DP v1.01.05)
			}
		}
//		DelayMs(100);
//		Software_Reset();
		Restart_System = TRUE;																																		// (9DP v1.01.12)
	}

	return TRUE;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	
//**************************************************************************************************
//**************************************************************************************************

//*
//**************************************************************************************************
//**************************************************************************************************
//* LUDLUM MEASUREMENTS INC. COPYRIGHT (C) 2012
//**************************************************************************************************
//**************************************************************************************************

