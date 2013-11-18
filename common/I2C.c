//**************************************************************************************************
//**************************************************************************************************
//*																		
//*	Filename:		I2C.c
//*	Date:			08/09/10
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
//*	~~~~~~~~	08/09/10	James Comstock
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

#include <plib.h>
#include "GenericTypeDefs.h"
#include "I2C.h"

//**************************************************************************************************
//**************************************************************************************************
//*	Test Variables

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void i2c_wait(unsigned int cnt)
//*
//*	Description:
//* 	This routine is designed to wait a predetermined amount of time or roughly machine cycles
//*		per count.
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	cnt - number of times to perform the loop (<65536)
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

void i2c_wait(unsigned int cnt)
{
	while(--cnt)
	{
		asm("nop");
		asm("nop");
	}
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void i2c2Write(UINT8 DeviceAddress, UINT16 RegisterAddress, UNIT8, RegisterSize, UINT16 RegisterData, UINT8 DataSize)
//*
//*	Description:
//* 	This routine will write one or two characters to the I2C port.
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	DeviceAddress - address of the I2C device (1 byte)
//*		RegisterAddress - address of the register within the I2C device (1 or 2 bytes)
//*		RegisterSize - 1 byte or 2 bytes register size
//*		RegisterData - data to be written to the register (1 or 2 bytes)
//*		RegisterDataSize - 1 byte or 2 bytes register data size
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//*

void i2c2Write(UINT8 DeviceAddress, UINT16 RegisterAddress, UINT8 RegisterSize, UINT16 RegisterData, UINT8 RegisterDataSize)
{
	char i2cData[5];
	UINT8 index;
	UINT8 DataSize;
	UINT8 URegisterAddress;																			// upper register address
	UINT8 LRegisterAddress;																			// lower register address
	UINT8 URegisterData;																			// upper data byte
	UINT8 LRegisterData;																			// lower data byte
	UINT16 status;																					// (9DP v1.01.20)

	URegisterAddress = RegisterAddress / 0x100;
	LRegisterAddress = RegisterAddress % 0x100;
	URegisterData = RegisterData / 0x100;
	LRegisterData = RegisterData % 0x100;

	DataSize = 0;
	i2cData[DataSize++] = (DeviceAddress | i2cWRITE);												// Setup and store Control Byte

	switch(RegisterSize)
	{
		case 2:																						// two bytes address, two bytes data
		{
			i2cData[DataSize++] = URegisterAddress;													// store register address
		}
		case 1:																						// two bytes address, one byte data
		{
			i2cData[DataSize++] = LRegisterAddress;													// store register address
			break;
		}
		default:
		{
			return;
		}
	}

	switch(RegisterDataSize)
	{
		case 2:																						// one byte address, one byte data
		{
			i2cData[DataSize++] = URegisterData;													// store register data
		}
		case 1:																						// one byte address, one byte data
		{
			i2cData[DataSize++] = LRegisterData;													// store register data
			break;
		}
	}

	index = 0;																						// initialize data index

	status = INTDisableInterrupts();																// (9DP v1.01.20)

	StartI2C2();																					// Send Start Bit
	IdleI2C2();																						// Wait to complete

	while(DataSize)
	{
		MasterWriteI2C2(i2cData[index++]);
		IdleI2C2();
		DataSize--;
		if(I2C2STATbits.ACKSTAT)																	// if not acknowledged and still 1 then stop data transfers...
		{
			break;																					// ...break from while loop
		}
	}
	
	StopI2C2();																						// Send the Stop condition
	IdleI2C2();																						// Wait to complete

	status = INTEnableInterrupts();																	// (9DP v1.01.20)

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void i2c2MultiWrite(UINT8 DeviceAddress, UINT16 RegisterData1, UINT16 RegisterData2, UINT16 RegisterData3, UINT16 RegisterData4)
//*
//*	Description:
//* 	This routine will write to all four DAC channels
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	DeviceAddress - address of the I2C device (1 byte)
//*		RegisterData1-4 - data to be written to the register (8 bytes)
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//*

void i2c2MultiWrite(UINT8 DeviceAddress, UINT16 RegisterData1, UINT16 RegisterData2, UINT16 RegisterData3, UINT16 RegisterData4)
{
	char i2cData[20];
	UINT8 index;
	UINT8 DataSize;
	UINT8 URegisterData;																			// upper data byte
	UINT8 LRegisterData;																			// lower data byte
	UINT16 status;																					// (9DP v1.01.20)

	DataSize = 0;
	i2cData[DataSize++] = (DeviceAddress | i2cWRITE);												// setup and store control byte

	i2cData[DataSize++] = (DAC_MultiWrite | DAC_Channel_A);											// setup channel A
	URegisterData = RegisterData1 / 0x100;
	LRegisterData = RegisterData1 % 0x100;
	i2cData[DataSize++] = URegisterData;															// store register data
	i2cData[DataSize++] = LRegisterData;															// store register data

	i2cData[DataSize++] = (DAC_MultiWrite | DAC_Channel_B);											// setup channel B
	URegisterData = RegisterData2 / 0x100;
	LRegisterData = RegisterData2 % 0x100;
	i2cData[DataSize++] = URegisterData;															// store register data
	i2cData[DataSize++] = LRegisterData;															// store register data

	i2cData[DataSize++] = (DAC_MultiWrite | DAC_Channel_C);											// setup channel C
	URegisterData = RegisterData3 / 0x100;
	LRegisterData = RegisterData3 % 0x100;
	i2cData[DataSize++] = URegisterData;															// store register data
	i2cData[DataSize++] = LRegisterData;															// store register data

	i2cData[DataSize++] = (DAC_MultiWrite | DAC_Channel_D);											// setup channel D
	URegisterData = RegisterData4 / 0x100;
	LRegisterData = RegisterData4 % 0x100;
	i2cData[DataSize++] = URegisterData;															// store register data
	i2cData[DataSize++] = LRegisterData;															// store register data

	index = 0;																						// initialize data index

	status = INTDisableInterrupts();																// (9DP v1.01.20)

	StartI2C2();																					// send start bit
	IdleI2C2();																						// wait till complete

	while(DataSize)
	{
		MasterWriteI2C2(i2cData[index++]);															// write data
		IdleI2C2();																					// wait till complete
		DataSize--;																					// decrement data counter
		if(I2C2STATbits.ACKSTAT)																	// if not acknowledged...
		{
			break;																					// ...break from while loop
		}
	}
	
	StopI2C2();																						// Send the Stop condition
	IdleI2C2();																						// Wait to complete

	status = INTEnableInterrupts();																	// (9DP v1.01.20)

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	unsigned char i2c2ReadByte(UINT8 DeviceAddress, UINT16 RegisterAddress, UINT8 RegisterSize)
//*
//*	Description:
//*		This routine will read a single character from the I2C device.
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	DeviceAddress - address of the I2C device (1 byte)
//*		RegisterAddress - address of the register within the I2C device
//*		RegisterSize - defines the size of the register (1 or 2 bytes)
//*
//*	Return Values:
//* 	unsigned char - character read
//*
//*	Remarks:
//* 	None
//*

BYTE i2c2ReadByte(UINT8 DeviceAddress, UINT16 RegisterAddress, UINT8 RegisterSize)
{
	int index;
	char i2cData[5];
	int DataSize;
	BYTE i2cByteRead;
	UINT8 URegisterAddress;																			// upper register address
	UINT8 LRegisterAddress;																			// lower register address
	UINT16 status;																					// (9DP v1.01.20)

	URegisterAddress = RegisterAddress / 0x100;
	LRegisterAddress = RegisterAddress % 0x100;

	DataSize = 0;

	i2cData[DataSize++] = (DeviceAddress | i2cWRITE);												// Setup and store Control Byte

	switch(RegisterSize)
	{
		case 2:																						// two byte address
		{
			i2cData[DataSize++] = URegisterAddress;													// store register address
		}
		case 1:																						// one byte address
		{
			i2cData[DataSize++] = LRegisterAddress;													// store register address
			break;
		}
		default:
		{
			return;
		}
	}

	index = 0;

	status = INTDisableInterrupts();																// (9DP v1.01.20)

	StartI2C2();																					// Send start bit
	IdleI2C2();																						// wait to complete

	while(DataSize)
	{
		MasterWriteI2C2(i2cData[index++]);															// send i2c information to read correct register
		IdleI2C2();
		DataSize--;
		if(I2C2STATbits.ACKSTAT)																	// if not acknowledged and still 1 then stop data transfers...
		{
			StopI2C2();																				// ...send the stop condition
			IdleI2C2();																				// ...wait to complete
			status = INTEnableInterrupts();															// ...(9DP v1.01.20)
			return 0;																				// ...return from routine (no point of continuing)
		}
	}

//	RestartI2C2();																					// send the restart condition
	StartI2C2();																					// send the restart condition
	IdleI2C2();																						// wait to complete

	MasterWriteI2C2(DeviceAddress | i2cREAD);														// transmit read command to device
	IdleI2C2();																						// wait to complete

	i2cByteRead = MasterReadI2C2();
	IdleI2C2();																						// wait to complete

	StopI2C2();																						// send the stop condition
	IdleI2C2();																						// wait to complete

	status = INTEnableInterrupts();																	// (9DP v1.01.20)

	return i2cByteRead;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	WORD i2c2ReadWord(UINT8 DeviceAddress, UINT16 RegisterAddress, UINT8 RegisterSize)
//*
//*	Description:
//*		This routine will read two characters (WORD) from the I2C device.
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	DeviceAddress - address of the I2C device (1 byte)
//*		RegisterAddress - address of the register within the I2C device
//*		RegisterSize - defines the size of the register (1 or 2 bytes)
//*
//*	Return Values:
//* 	WORD - characters read
//*
//*	Remarks:
//* 	None
//*

WORD i2c2ReadWord(UINT8 DeviceAddress, UINT16 RegisterAddress, UINT8 RegisterSize)
{
	int index;
	char i2cData[5];
	int DataSize;
	UINT8 URegisterAddress;																			// upper register address
	UINT8 LRegisterAddress;																			// lower register address
	UINT8 URegisterData;																			// upper data byte
	UINT8 LRegisterData;																			// lower data byte
	WORD Result;
	UINT16 status;																					// (9DP v1.01.20)

	index = 0;
	DataSize = 0;

	URegisterAddress = RegisterAddress / 0x100;
	LRegisterAddress = RegisterAddress % 0x100;

	i2cData[DataSize++] = (DeviceAddress | i2cWRITE);												// Setup and store Control Byte

	switch(RegisterSize)
	{
		case 1:																						// one byte address
		{
			i2cData[DataSize++] = LRegisterAddress;													// store register address
			break;
		}
		case 2:																						// two byte address
		{
			i2cData[DataSize++] = URegisterAddress;													// store register address
			i2cData[DataSize++] = LRegisterAddress;													// store register address
			break;
		}
		default:
		{
			return;																					// (9DP v1.02.03)
		}
	}

	status = INTDisableInterrupts();																// (9DP v1.01.20)

	StartI2C2();																					// Send start bit
	IdleI2C2();																						// wait to complete

	while(DataSize)
	{
		MasterWriteI2C2(i2cData[index++]);															// send i2c information to read correct register
		IdleI2C2();
		DataSize--;
		if(I2C2STATbits.ACKSTAT)																	// if not acknowledged and still 1 then stop data transfers...
		{
			StopI2C2();																				// ...send the stop condition
			IdleI2C2();																				// ...wait to complete
			status = INTEnableInterrupts();															// ...(9DP v1.01.20)
			return 0;																				// ...return from routine (no point of continuing)
		}
	}

	RestartI2C2();																					// send the restart condition
	IdleI2C2();																						// wait to complete

	MasterWriteI2C2(DeviceAddress | i2cREAD);														// transmit read command to device
	IdleI2C2();																						// wait to complete

	URegisterData = MasterReadI2C2();

	AckI2C2();																						// ACK
	IdleI2C2();																						// Wait to complete

	LRegisterData = MasterReadI2C2();
	NotAckI2C2();																					// NACK

	StopI2C2();																						// send the stop condition
	IdleI2C2();																						// wait to complete

	status = INTEnableInterrupts();																	// (9DP v1.01.20)

	Result = URegisterData;
	Result = Result << 8;
	Result = Result + LRegisterData;

	return Result;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	UINT32_VAL i2c2ReadDWord(UINT8 DeviceAddress) 												// (9DP v1.02.03)
//*
//*	Description:
//*		This routine will read two characters (WORD) from the I2C device.
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	DeviceAddress - address of the I2C device (1 byte)
//*		RegisterAddress - address of the register within the I2C device
//*		RegisterSize - defines the size of the register (1 or 2 bytes)
//*
//*	Return Values:
//* 	WORD - characters read
//*
//*	Remarks:
//* 	None
//*

UINT32_VAL i2c2ReadDWord(UINT8 DeviceAddress)
{
	UINT32_VAL temp_result;
	UINT16 status;
	BYTE index = 3;

	status = INTDisableInterrupts();																//

	StartI2C2();																					// Send start bit
	IdleI2C2();																						// wait to complete

	MasterWriteI2C2(DeviceAddress | i2cREAD);														// transmit read command to device
	IdleI2C2();																						// wait to complete

	temp_result.v[index--] = MasterReadI2C2();
	AckI2C2();																						// ACK
	IdleI2C2();																						// Wait to complete

	temp_result.v[index--] = MasterReadI2C2();
	AckI2C2();																						// ACK
	IdleI2C2();																						// Wait to complete

	temp_result.v[index--] = MasterReadI2C2();
	AckI2C2();																						// ACK
	IdleI2C2();																						// Wait to complete

	temp_result.v[index] = MasterReadI2C2();
	NotAckI2C2();																					// NACK
	StopI2C2();																						// send the stop condition
	IdleI2C2();																						// wait to complete

	status = INTEnableInterrupts();																	//

	return temp_result;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	WORD i2c2ReadADC(UINT8 DeviceAddress, UINT8 URegisterAddress, UINT8 LRegisterAddress, UINT8 RegisterData)
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
//* 	ADC Address 0x46		010 0011x
//*

WORD i2c2ReadADC(UINT8 DeviceAddress, UINT8 URegisterAddress, UINT8 LRegisterAddress, UINT8 RegisterData)
{
	char i2cData[10];
	int Index;
	int DataSize;
	float batterylevel;																				// battery
	BYTE i2cByte1Read = 0;
	BYTE i2cByte2Read = 0;
	BYTE i2cChannel = 0;
	BYTE upperbits = 0;
	WORD i2cWordRead = 0;
	UINT16 status;																					// (9DP v1.01.20)

	i2cData[0] = (DeviceAddress | i2cWRITE);
	i2cData[1] = URegisterAddress;
	i2cData[2] = LRegisterAddress;
	i2cData[3] = RegisterData;

	DataSize = 2;

	status = INTDisableInterrupts();																// (9DP v1.01.20)

	StartI2C2();																					// Send Start Bit
	IdleI2C2();																						// Wait to complete

	Index = 0;	
	while(DataSize)
	{
		MasterWriteI2C2(i2cData[Index++]);
		IdleI2C2();
		DataSize--;
		if(I2C2STATbits.ACKSTAT)																	// if not acknowledged and still 1 then stop data transfers...
		{
			StopI2C2();																				// ...send the stop condition
			IdleI2C2();																				// ...wait to complete
			status = INTEnableInterrupts();															// ...(9DP v1.01.20)
			return 0;																				// ...return from routine (no point of continuing)
		}
	}

	IdleI2C2();																						// wait to complete

	RestartI2C2();    																				// Send the Restart condition
	IdleI2C2();																						// Wait to complete

	MasterWriteI2C2(DeviceAddress | i2cREAD);														// tranmit read command to device
	IdleI2C2();																						// wait to complete

	i2cByte1Read = MasterReadI2C2();

	AckI2C2();																						// ACK
	IdleI2C2();																						// Wait to complete --------------------- hanging up

	i2cByte2Read = MasterReadI2C2();
	NotAckI2C2();																					// NACK

	StopI2C2();																						// send the stop condition
	IdleI2C2();																						//-- wait to complete   

	status = INTEnableInterrupts();																	// (9DP v1.01.20)

	i2cWordRead = (i2cByte1Read & 0x0F);
	i2cWordRead = (i2cWordRead << 8);
	i2cWordRead = (i2cWordRead | i2cByte2Read);

	return i2cWordRead;																				// send received battery voltage value
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void i2c2ClearPage(UINT8 DeviceAddress, UINT16 RegisterAddress, UINT8 RegisterSize, UINT8 PageSize)
//*
//*	Description:
//* 	This routine will clear one page.
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	DeviceAddress - address of the I2C device (1 byte)
//*		RegisterAddress - address of the register within the I2C device (1 or 2 bytes)
//*		RegisterSize - 1 byte or 2 bytes register size
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//*

void i2c2ClearPage(UINT8 DeviceAddress, UINT16 RegisterAddress, UINT8 RegisterSize, UINT8 PageSize)
{
	UINT8 index;
	UINT8 counter;
	UINT8 URegisterAddress;																			// upper register address
	UINT8 LRegisterAddress;																			// lower register address
	BYTE eeprom_data_string[4];
	UINT16 status;																					// (9DP v1.01.20)

	URegisterAddress = RegisterAddress / 0x100;
	LRegisterAddress = RegisterAddress % 0x100;

	counter = 0;
	eeprom_data_string[counter++] = (DeviceAddress | i2cWRITE);										// Setup and store Control Byte

	switch(RegisterSize)
	{
		case 2:																						// two byte address
		{
			eeprom_data_string[counter++] = URegisterAddress;
		}
		case 1:																						// one byte address
		{
			eeprom_data_string[counter++] = LRegisterAddress;
			break;
		}
		default:
		{
			return;
		}
	}

	status = INTDisableInterrupts();																// (9DP v1.01.20)

	StartI2C2();																					// Send Start Bit
	IdleI2C2();																						// Wait to complete

	for (index = 0; index < counter; index++)
	{
		MasterWriteI2C2(eeprom_data_string[index]);
		IdleI2C2();
		if(I2C2STATbits.ACKSTAT)																	// if not acknowledged and still 1 then stop data transfers...
		{
			StopI2C2();																				// ...send the stop condition
			IdleI2C2();																				// ...wait to complete
			status = INTEnableInterrupts();															// ...(9DP v1.01.20)
			return;																					// ...return from routine (no point of continuing)
		}
	}

	for (; index < (PageSize + counter); index++)
	{
		MasterWriteI2C2(0x00);
		IdleI2C2();
		if(I2C2STATbits.ACKSTAT)
		{
			break;																					// ...break from while loop
		}
	}

	StopI2C2();																						// Send the Stop condition
	IdleI2C2();																						// Wait to complete
	status = INTEnableInterrupts();																	// (9DP v1.01.20)
	DelayMs(25);																					// EEPROM should only require a 5ms delay

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void i2c2WritePage(UINT8 DeviceAddress, UINT16 RegisterAddress, UNIT8, RegisterSize, BYTE *buffer, UINT8 PageSize)
//*
//*	Description:
//* 	This routine will clear one page.
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	DeviceAddress - address of the I2C device (1 byte)
//*		RegisterAddress - address of the register within the I2C device (1 or 2 bytes)
//*		RegisterSize - 1 byte or 2 bytes register size
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//*

void i2c2WritePage(UINT8 DeviceAddress, UINT16 RegisterAddress, UINT8 RegisterSize, BYTE *page, UINT8 PageSize)
{
	BYTE checksum;
	UINT8 counter;
	UINT8 index, page_index;
	UINT8 URegisterAddress;																			// upper register address
	UINT8 LRegisterAddress;																			// lower register address
	BYTE eeprom_data_string[132];
	UINT16 status;																					// (9DP v1.01.20)

	URegisterAddress = RegisterAddress / 0x100;
	LRegisterAddress = RegisterAddress % 0x100;

	do
	{
		counter = 0;
		checksum = 0;
		eeprom_data_string[counter++] = (DeviceAddress | i2cWRITE);									// Setup and store Control Byte
	
		switch(RegisterSize)
		{
			case 2:																					// two byte address
			{
				eeprom_data_string[counter++] = URegisterAddress;
			}
			case 1:																					// one byte address
			{
				eeprom_data_string[counter++] = LRegisterAddress;
				break;
			}
			default:
			{
				return;
			}
		}
	
		for(index = counter, page_index = 0; page_index < (PageSize - 1); index++, page_index++)
		{
			eeprom_data_string[index] = page[page_index];
			checksum += page[page_index];
		}
	
		eeprom_data_string[index] = checksum;

		status = INTDisableInterrupts();															// (9DP v1.01.20)

		StartI2C2();																				// Send Start Bit
		IdleI2C2();																					// Wait to complete
	
		for (index = 0; index < (counter + PageSize); index++)
		{
			MasterWriteI2C2(eeprom_data_string[index]);
			IdleI2C2();
			if(I2C2STATbits.ACKSTAT)																// if not acknowledged and still 1 then stop data transfers...
			{
				break;																				// ...break from while loop
			}
		}
	
		StopI2C2();																					// Send the Stop condition
		IdleI2C2();																					// Wait to complete

		status = INTEnableInterrupts();																// (9DP v1.01.20)

		DelayMs(25);																				// EEPROM should only require a 5 ms delay
	} while(i2c2ReadPage(DeviceAddress, RegisterAddress, RegisterSize, (BYTE *)page, PageSize) == FALSE);

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BOOL i2c2ReadPage(UINT8 DeviceAddress, UINT16 RegisterAddress, UNIT8, RegisterSize, BYTE *page, UINT8 PageSize)
//*
//*	Description:
//* 	
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	DeviceAddress - address of the I2C device (1 byte)
//*		RegisterAddress - address of the register within the I2C device (1 or 2 bytes)
//*		RegisterSize - 1 byte or 2 bytes register size
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//*

BOOL i2c2ReadPage(UINT8 DeviceAddress, UINT16 RegisterAddress, UINT8 RegisterSize, BYTE *page, UINT8 PageSize)
{
	BYTE checksum;
	UINT8 counter;
	UINT8 index;
	UINT8 URegisterAddress;																			// upper register address
	UINT8 LRegisterAddress;																			// lower register address
	BYTE header_string[4];
	BYTE data_string[128];
	UINT16 status;																					// (9DP v1.01.20)

	URegisterAddress = RegisterAddress / 0x100;
	LRegisterAddress = RegisterAddress % 0x100;
	index = 0;

	counter = 0;
	header_string[counter++] = (DeviceAddress | i2cWRITE);											// Setup and store Control Byte

	switch(RegisterSize)
	{
		case 2:																						// two byte address
		{
			header_string[counter++] = URegisterAddress;
		}
		case 1:																						// one byte address
		{
			header_string[counter++] = LRegisterAddress;
			break;
		}
		default:
		{
			return;
		}
	}

	status = INTDisableInterrupts();																// (9DP v1.01.20)

	StartI2C2();																					// Send Start Bit
	IdleI2C2();																						// Wait to complete

	for (index = 0; index < counter; index++)
	{
		MasterWriteI2C2(header_string[index]);
		IdleI2C2();
		if(I2C2STATbits.ACKSTAT)																	// if not acknowledged and still 1 then stop data transfers...
		{
			StopI2C2();																				// ...send the stop condition
			IdleI2C2();																				// ...wait to complete
			status = INTEnableInterrupts();															// ...(9DP v1.01.20)
			return 0;																				// ...return from routine (no point of continuing)
		}
	}

	index = 0;

	StartI2C2();    																				// Send the Restart condition
	IdleI2C2();																						// Wait to complete

	MasterWriteI2C2(DeviceAddress | i2cREAD);														// tranmit read command to device
	IdleI2C2();																						// Wait to complete

	while(I2C2STATbits.RBF);
	data_string[index++] = MasterReadI2C2();														// read first byte

	while(index < PageSize)
	{
		AckI2C2();																					// send an acknowledgement
		IdleI2C2();																					// wait
		data_string[index++] = MasterReadI2C2();													// read byte
	}
	
	NotAckI2C2();																					// send an acknowledgement
	StopI2C2();																						// Send the Stop condition
	IdleI2C2();																						// Wait to complete

	status = INTEnableInterrupts();																	// (9DP v1.01.20)

	for(index = 0, checksum = 0; index < (PageSize - 1); index++, page++)
	{
		*page = data_string[index];
		checksum += data_string[index];
	}

	return (checksum == data_string[index]);
}

