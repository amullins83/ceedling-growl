//**************************************************************************************************
//**************************************************************************************************
//*																		
//*	Filename:		UART.c
//*	Date:			08/09/10
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
//*	~~~~~~~~	08/09/10	James Comstock
//*	- continue where Clint left off
//*
//*	~~~~~~~~	05/15/10	Clint Appling
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
//*

#include <plib.h>
#include "Graphics.h"
#include "LCD-SST39VF1601v001.h"
#include "i2cConfig.h"
#include "p32mx460f512l.h"
#include "UART.h"

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void UARTInit(void)
//*
//*	Description:
//* 	Initializes UART
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

void UARTInit()
{
	DWORD configRegister;																			// (9DP v1.02.00)

	TX_TRIS = 0;
	RX_TRIS = 1;
//	U1BRG = GetPeripheralClock() / 4 / BAUDRATE2 - 1;
	configRegister = DEVCFG2 & 0x00000070;															// (9DP v1.02.00)
	if(configRegister == 0x00000030)																// FPLLMULT = 3 [18x] (9DP v1.02.00)
	{
    	U1BRG = GetPeripheralClock_72MHz() / 4 / BAUDRATE2 - 1;										// (9DP v1.02.00)
	}
	else																							// FPLLMULT = 5 [20x] (9DP v1.02.00)
	{
	    U1BRG = GetPeripheralClock_80MHz() / 4 / BAUDRATE2 - 1;										// (9DP v1.02.00)
	}
	U1MODE = 0;
	U1STA = 0;
	U1MODEbits.UARTEN = 1;
	U1MODEbits.STSEL = 0;
	U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;
    U1MODEbits.BRGH = 1;
    U1STAbits.OERR = 0;
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void UARTPutChar(BYTE ch)
//*
//*	Description:
//* 	Sends data the size of CHAR to the UART port.
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	ch - the CHAR to be transmitted to the UART port
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

void UARTPutChar(BYTE ch)
{
  	while(U1STAbits.UTXBF == 1);																	// 	wait for Tx buffer is not full
	U1TXREG = ch;
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void UARTPutByte(BYTE byte)
//*
//*	Description:
//* 	Sends data the size of BYTE to the UART port
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	byte - the BYTE to be transmitted to the UART port
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

void UARTPutByte(BYTE byte)
{
	UARTPutChar(Hex2Char(byte>>4));
  	UARTPutChar(Hex2Char(byte&0x0f));
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void UARTPutWord(WORD_VAL word)
//*
//*	Description:
//* 	Sends data the size of WORD to the UART port
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	word - the WORD to be transmitted to the UART port
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

void UARTPutWord(WORD word)
{
 	UARTPutByte(((WORD_VAL)word).v[1]);
 	UARTPutByte(((WORD_VAL)word).v[0]);
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void UARTPutDWord(DWORD_VAL dword)
//*
//*	Description:
//* 	Sends data the size of DOUBLE WORD to the UART port
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	dword - the DOUBLE to be transmitted to the UART port
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

void UARTPutDWord(DWORD dword)
{
  	UARTPutWord(((DWORD_VAL)dword).w[1]);
  	UARTPutWord(((DWORD_VAL)dword).w[0]);
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void UARTPutString(char* str)
//*
//*	Description:
//* 	Sends data the size of CHAR, until a '/0' is detected in str, to the UART port
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

void UARTPutString(char* str)
{
	while(*str)
    	UARTPutChar(*str++);    
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BYTE UARTGetChar(void)
//*
//*	Description:
//* 	Reads the last character received on the UART port
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	None
//*
//*	Return Values:
//* 	BYTE - last UART character received
//*
//*	Remarks:
//* 	None
//*

BYTE UARTGetChar(void)
{
	BYTE temp;

    temp = U1RXREG;
    U1STAbits.OERR = 0;    																			// Clear error flag
    return temp;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BYTE UARTWaitChar()
//*
//*	Description:
//* 	Waits until a character is present on the UART port then returns the character.
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	None
//*
//*	Return Values:
//* 	BYTE - UART character
//*
//*	Remarks:
//* 	None
//*

BYTE UARTWaitChar(void)
{
    if(U1STAbits.OERR)
		U1STAbits.OERR = 0;																			// Clear error flag

    while(U1STAbits.URXDA == 0);																	// Wait for new data (was URXDA)

    return UARTGetChar();
}


//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BYTE Char2Hex(BYTE ch)
//*
//*	Description:
//* 	Converts ASCII coded digit into hexadecimal
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

BYTE Char2Hex(BYTE ch)
{
	// Wrong char
  	if(ch > 102)													
    	return 0;  
  
  	// From a to f     
  	if(ch > 96)
    	return (ch-87);
    
  	// Wrong char    
  	if(ch > 70)
    	return 0;
  	
  	// From A to F    
  	if(ch > 64)
    	return (ch-55);
	
  	// Wrong char  
  	if(ch > 57)
    	return 0;
  	
  	// From 0 - 9
  	if(ch > 47)
    	return(ch-48);

  	// Wrong char
   	return 0;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	BYTE Hex2Char(BYTE hex)
//*
//*	Description:
//* 	Converts low nibble into ASCII coded digit.
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	BYTE hex - hexadecimal character
//*
//*	Return Values:
//* 	BYTE - ASCII character
//*
//*	Remarks:
//* 	None
//*

BYTE Hex2Char(BYTE hex)
{
	BYTE h;

  	h = hex & 0x0f;
  	// From 0xa to 0xf
  	if (h > 9)
    	return (h+55);
  	else
    	return (h+48);
}
