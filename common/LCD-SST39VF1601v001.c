//**************************************************************************************************
//**************************************************************************************************
//*																		
//*	Filename:		LCD-SST39VF1601v001.c
//*	Date:			05/25/11
//*                                                                     
//*	Author:			Clint Appling and James Comstock
//*	Company:		Ludlum Measurements                               	
//*	Address:		501 Oak Street										
//*					Sweetwater, TX 79556								
//*                                                                     
//**************************************************************************************************
//**************************************************************************************************
//*                                                                     
//*	Revision Notes:                                                  	
//*																		
//*	- Original Working Release
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
#include "LCD-SST39VF1601v001.h"
#include "instrument.h"
#include "Definitions.h"
#include "System.h"

//*
//**************************************************************************************************
//*	External Global Variables

extern BYTE File_RXMessage[MESSAGE_SIZE];
extern TYPE_INSTRUMENT m9dp;
extern IMAGE_STATE imageState;

//*
//**************************************************************************************************
//* Global Variables

volatile DWORD BMP_Address = 0;
volatile DWORD MemoryAddress = 0;
volatile DWORD ImageSize = 0;
volatile DWORD FileErrorCounter = 0;

//*
//**************************************************************************************************
//*

char ProcessData[] = "0x\0";

typedef struct  _SST39STATREG_
{
	BYTE    RESERVED:5;
	BYTE    FAILED:1;
	BYTE    TOGGLE:1;
} SST39STATREG;

typedef union _SST39STATUS_
{
	SST39STATREG Bits;
	BYTE        Val;
} SST39STATUS;

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BYTE Parse_Data(char *data_string, )
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

char * Parse_Data(char *data_string)
{
	return strtok(data_string,",");
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//*		void Delay_10uS(WORD time)
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

void Delay_10uS(WORD time)
{
	unsigned delay;
	while(time--)
		for(delay=0; delay < 32; delay++);	

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//*		void Image_Handler(void)
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

void Image_Handler(void)
{
	static WORD counter = 0;
	static BOOL bpp_16 = FALSE;
	static BOOL second_byte = FALSE;
	WORD memory_word = 0;
	WORD lower_byte = 0;
	WORD size_x = 0;
	WORD size_y = 0;
	BYTE size_BPP = 0;
	BYTE response;
	char memory_byte[MESSAGE_SIZE];
	char *message;
	char null_char[] = "\0";

	strcat((char *)File_RXMessage, null_char);
	message = Parse_Data((char *)File_RXMessage);
	while(message != NULL)
	{
		strcpy(memory_byte, message);
		response = strncmp(memory_byte,ProcessData,2);
		if(response == 0)
		{
			counter++;
			memory_word = (16 * UART2Char2Hex(memory_byte[2])) + (UART2Char2Hex(memory_byte[3]));
			switch(imageState)
			{
				case MEMORY_ADDRESS:
				{
					MemoryAddress = MemoryAddress << 8;
					MemoryAddress = MemoryAddress + memory_word;
					if(counter == 3)
					{
						imageState = IMAGE_SIZE;
						counter = 0;
					}
					break;
				}
				case IMAGE_SIZE:
				{
					if(SST39WriteWord(MemoryAddress++, memory_word) == TRUE)
						FileErrorCounter++;

					switch(counter)
					{
						case 2:
						{
							size_BPP = memory_word;
							break;
						}
						case 3:
						{
							size_y = memory_word;
							break;
						}
						case 4:
						{
							size_y = size_y + (memory_word<<8);
							break;
						}
						case 5:
						{
							size_x = memory_word;
							break;
						}
						case 6:
						{
							size_x = size_x + (memory_word<<8);
							ImageSize = (size_BPP / 8) * size_x * size_y;
							counter = 0;
							if(size_BPP == 8)
							{
								imageState = IMAGE_PALLETE;
								bpp_16 = FALSE;
							}
							else
							{
								imageState = IMAGE_DATA;
								bpp_16 = TRUE;
							}
							second_byte = FALSE;
							break;
						}
					}
					break;
				}
				case IMAGE_PALLETE:
				{
					if(second_byte == FALSE)
					{
						lower_byte = memory_word & 0x00FF;
						second_byte = TRUE;
						break;
					}
					else
					{
						memory_word = (memory_word << 8) + lower_byte;
						second_byte = FALSE;
					}

					if(SST39WriteWord(MemoryAddress++, memory_word) == TRUE)
						FileErrorCounter++;

					if(counter == 512)
					{
						counter = 0;
						imageState = IMAGE_DATA;
					}
					break;
				}
				case IMAGE_DATA:
				{
					if(bpp_16 == TRUE)
					{
						if(second_byte == FALSE)
						{
							lower_byte = memory_word & 0x00FF;
							second_byte = TRUE;
							break;
						}
						else
						{
							memory_word = (memory_word << 8) + lower_byte;
							second_byte = FALSE;
						}
					}

					if(SST39WriteWord(MemoryAddress++, memory_word) == TRUE)
						FileErrorCounter++;

					if(counter == ImageSize)
					{
						counter = 0;
						ImageSize = 0;
						MemoryAddress = 0x000000;
						imageState = MEMORY_ADDRESS;
					}	
					break;
				}
			}
			message = Parse_Data(NULL);
		}
		else
		{
			return;
		}
	}
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//*		BYTE Terminate_Data(char *message_string)
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

//BYTE Terminate_Data(char *message_string)
//{
//	unsigned char length;
//	char *response;
//	BYTE location;
//
//	length = strlen(message_string);
//	if(length > MESSAGE_SIZE)
//		return INVALID_MESSAGE_LENGTH;
//
//	response = strchr(message_string, (char) CARRIAGE_RETURN);										// search for a carriage return
//	if(response != NULL)
//	{
//		location = (BYTE) (response - message_string);	
//		if((location != 0) && (location < MESSAGE_SIZE))											// if the location is valid
//		{
//			if(message_string[location - 1] != ',')													// if the previous character is not a ','
//			{
//				message_string[location++] = ',';													// replace the carriage return with a ','
//				if(message_string[location] == (BYTE) LINEFEED)										// if the next character is a linefeed
//				{
//					message_string[location] = 0x00;												// NULL terminate the data string
//					return VALID_MESSAGE;															// then the message is valid
//				}
//			}
//			else																					// else
//			{
//				message_string[location++] = 0x00;													// NULL terminate the data string
//				if(message_string[location] == (BYTE) LINEFEED)										// if the next character is a linefeed
//				{
//					message_string[location] = 0x00;												// NULL terminate the data string
//					return VALID_MESSAGE;															// then the message is valid
//				}
//			}
//
//		}
//	}
//	return INVALID_MESSAGE_TERMINATION;
//}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//*		BOOL ExtImage(SHORT left, SHORT top, DWORD address)
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

BOOL ExtImage(SHORT left, SHORT top, DWORD address)
{
	WORD test;

	BMP_Address = address;
	test = SST39ReadWord(BMP_Address++);																// read file type

	if(test == EXTERNAL)
	{
		switch(SST39ReadWord(BMP_Address++))															// switch based on number of colors
		{
			case 0x08:
			{
				ExtImage8BPP(left, top);
				return TRUE;
			}
			case 0x10:
			{
				ExtImage16BPP(left, top);
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
//*		void ExtImage8BPP(SHORT left, SHORT top, DWORD address)
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

void ExtImage8BPP(SHORT left, SHORT top)
{
    WORD sizeX, sizeY;
	WORD pallete[256];
	WORD counter;
    WORD index;
	SHORT x, y;

    // Read image size
	sizeY = SST39ReadFormedWord(BMP_Address);
    BMP_Address += 2;
	sizeX = SST39ReadFormedWord(BMP_Address);
    BMP_Address += 2;

    // Read pallete
    for(counter = 0; counter < 256; counter++)
    {
		pallete[counter] = SST39ReadWord(BMP_Address);
	    BMP_Address++;
    }

    for(y = 0; y < sizeY; y++)
    {
        for(x = 0; x < sizeX; x++, BMP_Address++)
        {
			index = SST39ReadWord(BMP_Address);														// read pallete index
			SetColor(pallete[index]);																// assign pallete color
			PutPixel(left + x, top + y);															// place pixel
        }
    }

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//*		void ExtImage16BPP(SHORT left, SHORT top)
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

void ExtImage16BPP(SHORT left, SHORT top)
{
    WORD sizeX, sizeY;
    WORD temp;
    SHORT x, y;

	SetReg(REG_DISPLAY_MODE, 0x0384);

    // Read image size
	sizeY = SST39ReadFormedWord(BMP_Address);
	BMP_Address += 2;
	sizeX = SST39ReadFormedWord(BMP_Address);
	BMP_Address += 2;

    for(y = 0; y < sizeY; y++)
    {
        for(x = 0; x < sizeX; x++, BMP_Address++)
        {
			temp = SST39ReadWord(BMP_Address);
			SetColor(temp);	
			PutPixel(left + x, top + y);
        }
    }

	SetReg(REG_DISPLAY_MODE, 0x0304);

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//*		void SST39SetAddress(DWORD address)
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

void SST39SetAddress(DWORD address)
{
    if(((DWORD_VAL)address).bits.b16)
        A16_LAT_BIT_SET;																			// set address 16 = 1
    else
        A16_LAT_BIT_CLR;																			// set address 16 = 0

    if(((DWORD_VAL)address).bits.b17)
        A17_LAT_BIT_SET;																			// set address 17 = 1
    else
        A17_LAT_BIT_CLR;																			// set address 17 = 0

    PMPWaitBusy();         																			// (9DP v1.01.20)
    PMADDR = ((DWORD_VAL)address).w[0];																// set address 0 - 15
    PMPWaitBusy();         																			// (9DP v1.01.20)
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//*		BOOL SST39WriteWord(DWORD address, WORD data)
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

BOOL SST39WriteWord(DWORD address, WORD data)
{
	PMPWaitBusy();																					// wait for the transmission end
	SST39SetAddress(0x5555);
	PMPWaitBusy();																					// wait for the transmission end
	SST39_CE_LAT_BIT_CLR;																			// set CE Low - Enable SST39V Chip
    PMDIN1 = 0x00AA;

	PMPWaitBusy();																					// wait for the transmission end
	SST39SetAddress(0x2AAA);
	PMPWaitBusy();																					// wait for the transmission end
    PMDIN1 = 0x0055;
    
	PMPWaitBusy();																					// wait for the transmission end
	SST39SetAddress(0x5555);
	PMPWaitBusy();																					// wait for the transmission end
    PMDIN1 = 0x00A0;
    
	PMPWaitBusy();																					// wait for the transmission end
	SST39SetAddress(address);
	PMPWaitBusy();																					// wait for the transmission end
    PMDIN1 = data;

	PMPWaitBusy();																					// wait for the transmission end
	SST39_CE_LAT_BIT_SET;																			// set CE High - Disable SST39V Chip

	Delay_10uS(10);																					//

	if(SST39ReadWord(address) == data)
 	    return 0; 																					// passed
	else
    	return 1; 																					// failed
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//*		WORD SST39ReadWord(DWORD address)
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

WORD SST39ReadWord(DWORD address)
{
	WORD temp;

	PMPWaitBusy();																					// wait for the transmission end
    SST39SetAddress(address); 
	PMPWaitBusy();																					// wait for the transmission end
    SST39_CE_LAT_BIT_CLR;																			// clear CE - enable SST39V Chip
    temp = PMDIN1;																					// latch result, ignore data

	PMPWaitBusy();																					// wait for the transmission end
    temp = PMDIN1;																					// read result    

	PMPWaitBusy();																					// wait for the transmission end
	SST39_CE_LAT_BIT_SET;																			// set CE - disable SST39V Chip

    return temp;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//*		WORD SST39ReadFormedWord(DWORD address)
//*
//*	Description:
//* 	Reads two words from external memory that were saved as bytes and combines the results to
//*		make a word.
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

WORD SST39ReadFormedWord(DWORD address)
{
	WORD_VAL temp;

    temp.v[0] = SST39ReadWord(address);
    temp.v[1] = SST39ReadWord(address+1);

    return temp.Val;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//*		BYTE SST39BlockErase(DWORD address)
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

BYTE SST39BlockErase(DWORD address)
{
	PMPWaitBusy();																					// wait for the transmission end
    SST39SetAddress(0x5555);
	PMPWaitBusy();																					// wait for the transmission end
	SST39_CE_LAT_BIT_CLR;																			// set CS Low - Enable SST39V Chip
    PMDIN1=0x00AA;
	
	PMPWaitBusy();																					// wait for the transmission end
    SST39SetAddress(0x2AAA);
	PMPWaitBusy();																					// wait for the transmission end
    PMDIN1=0x0055;

	PMPWaitBusy();																					// wait for the transmission end
    SST39SetAddress(0x5555);
	PMPWaitBusy();																					// wait for the transmission end
    PMDIN1=0x0080;

	PMPWaitBusy();																					// wait for the transmission end
    SST39SetAddress(0x5555);
	PMPWaitBusy();																					// wait for the transmission end
    PMDIN1=0x00AA;

	PMPWaitBusy();																					// wait for the transmission end
    SST39SetAddress(0x2AAA);
	PMPWaitBusy();																					// wait for the transmission end
    PMDIN1=0x0055;

	PMPWaitBusy();																					// wait for the transmission end
    SST39SetAddress(address);
	PMPWaitBusy();																					// wait for the transmission end
    PMDIN1=0x0050;

	PMPWaitBusy();																					// wait for the transmission end
	SST39_CE_LAT_BIT_SET;																			// set CS High - Disable SST39V Chip

	DelayMs(25);																					// wait for block erase to finish
    return 1; 
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//*		void SST39ChipErase(void)
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

void SST39ChipErase(void)
{
	PMPWaitBusy();																					// wait for the transmission end
    SST39SetAddress(0x5555);
	PMPWaitBusy();																					// wait for the transmission end
	SST39_CE_LAT_BIT_CLR;																			// set CS Low - Enable SST39V Chip
    PMDIN1=0x00AA;
	
	PMPWaitBusy();																					// wait for the transmission end
    SST39SetAddress(0x2AAA);
	PMPWaitBusy();																					// wait for the transmission end
    PMDIN1=0x0055;

    PMPWaitBusy();																					// wait for the transmission end
    SST39SetAddress(0x5555);
	PMPWaitBusy();																					// wait for the transmission end
    PMDIN1=0x0080;

	PMPWaitBusy();																					// wait for the transmission end
    SST39SetAddress(0x5555);
	PMPWaitBusy();																					// wait for the transmission end
    PMDIN1=0x00AA;

	PMPWaitBusy();																					// wait for the transmission end
    SST39SetAddress(0x2AAA);
	PMPWaitBusy();																					// wait for the transmission end
    PMDIN1=0x0055;

	PMPWaitBusy();																					// wait for the transmission end
    SST39SetAddress(0x5555);
	PMPWaitBusy();																					// wait for the transmission end
    PMDIN1=0x0010;

	PMPWaitBusy();																					// wait for the transmission end
	SST39_CE_LAT_BIT_SET;																			// set CS High - Disable SST39V Chip

	DelayMs(50);																					// wait for chip erase to finish
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//*		BYTE SST39WriteArray(DWORD address, WORD* pData, WORD nCount)
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

BYTE SST39WriteArray(DWORD address, WORD* pData, WORD nCount)
{
	WORD    counter;
	WORD*   pD;
	DWORD   addr;

    pD = pData;
    addr = address;

    // write
    for(counter = 0; counter < nCount; counter++)
	{
       SST39WriteWord(addr,*pD);
       addr++; pD++;
    }

    pD = pData;
    addr = address;

    // verify
    for(counter = 0; counter < nCount; counter++)
	{
        if(*pD++ != SST39ReadWord(addr++))
            return 0;
    }

    return 1;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//*		void SST39ReadArray(DWORD address, WORD* pData, WORD nCount)
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

void SST39ReadArray(DWORD address, WORD* pData, WORD nCount)
{
	WORD counter;
	WORD temp;

	PMMODESET 	= 0x0800;																			// increment ADDR 1 every read
	PMPWaitBusy();																					// wait for the transmission end
    SST39SetAddress(address); 
	PMPWaitBusy();																					// wait for the transmission end
	SST39_CE_LAT_BIT_CLR;																			// set CS Low - Enable SST39V Chip
    temp = PMDIN1;																					// first byte is garbage
	PMPWaitBusy();																					// wait for the transmission end

	for(counter = 0; counter < nCount; counter++)
    {
        *pData++ = PMDIN1;
        PMPWaitBusy();																				// wait for the transmission end
    }

	PMMODECLR 	= 0x0800;																			// no address increment or decrement
    SST39_CE_LAT_BIT_SET;																			// set CS High - Disable SST39V Chip
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//*		void SST39ReadWordArray(DWORD address, WORD* pData, WORD nCount)
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

void SST39ReadWordArray(DWORD address, WORD* pData, WORD nCount)
{
	WORD counter;
	WORD dummy;
	WORD_VAL temp;

	PMMODESET 	= 0x0800;																			// increment ADDR 1 every read

	PMPWaitBusy();																					// wait for the transmission end
    SST39SetAddress(address); 																		// set memory address
	PMPWaitBusy();																					// wait for the transmission end
	SST39_CE_LAT_BIT_CLR;																			// set CS Low - Enable SST39V Chip
    dummy = PMDIN1;																					// first byte is garbage
	PMPWaitBusy();																					// wait for the transmission end

	for(counter = 0; counter < nCount; counter++)
    {
		dummy = PMDIN1;																				// read first byte
		PMPWaitBusy();																				// wait for the transmission end
		temp.v[0] = (BYTE) dummy;																	// save as low byte
		dummy = PMDIN1;																				// read second byte
        PMPWaitBusy();																				// wait for the transmission end
		temp.v[1] = (BYTE) dummy;																	// save as high byte
		*pData++ = temp.Val;																		// store value
    }

	PMMODECLR 	= 0x0800;																			// no address increment or decrement
    SST39_CE_LAT_BIT_SET;																			// set CS High - Disable SST39V Chip
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//*		void SST39WaitProgram(void)
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

void SST39WaitProgram(void)
{
	SST39STATUS stat;
	SST39STATUS prevStat;

	SST39_CE_LAT_BIT_CLR;																			// set CS Low - Enable SST39V Chip

    prevStat.Val = PMDIN1;
    PMPWaitBusy();																					// wait for the transmission end

    prevStat.Val = PMDIN1;
    PMPWaitBusy();																					// wait for the transmission end

    stat.Val = PMDIN1;
    PMPWaitBusy();																					// wait for the transmission end

    while(stat.Bits.TOGGLE != prevStat.Bits.TOGGLE)
	{
        prevStat.Val = stat.Val;
        stat.Val = PMDIN1;
        PMPWaitBusy();																				// wait for the transmission end
    }

	SST39_CE_LAT_BIT_SET;																			// set CS High - Disable SST39V Chip
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	END OF SST COMMANDS
//**************************************************************************************************
//**************************************************************************************************

//*
//**************************************************************************************************
//**************************************************************************************************
//* LUDLUM MEASUREMENTS INC. COPYRIGHT (C) 2011
//**************************************************************************************************
//**************************************************************************************************

