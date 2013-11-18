/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  Solomon Systech. SSD1906 LCD controller driver
 *****************************************************************************
 * FileName:        SSD1906.c
 * Dependencies:    Graphics.h
 * Processor:       PIC24, PIC32
 * Compiler:       	MPLAB C30, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2008 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY
 * OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
 * PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR
 * OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,
 * BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT
 * DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL,
 * INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA,
 * COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY
 * CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
 * OR OTHER SIMILAR COSTS.
 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Austin Mullins       08/28/13    Replaced .FRZ setting with equivalent bitmask
 *                                  due to compiler errors. I'm scratching my head
 *                                  as to why this was necessary.
 * Anton Alkhimenok     12/19/07
 * Anton Alkhimenok     01/31/08    combined portrait and landscape, PIC32 support
 * Anton Alkhimenok     08/22/08    driver configuration is simplified,
 *                                  new image rotation modes
 *****************************************************************************/
#include "Graphics.h"
#include "SSD1906.h"

// Color
WORD  _color;
// Clipping region control
SHORT _clipRgn;
// Clipping region borders
SHORT _clipLeft;
SHORT _clipTop;
SHORT _clipRight;
SHORT _clipBottom;

WORD	TmpOFFSET;

		
/////////////////////// LOCAL FUNCTIONS PROTOTYPES ////////////////////////////


#ifdef __PIC32MX
DWORD _address; // current video buffer address for PIC32 implementation
	#define SetAddress(address) _address = (address<<1)
			//	#define SetAddress(address) _address = address
#else
	void SetAddress(DWORD address);
#endif

void SetReg(WORD index, WORD value);
WORD GetReg(WORD index);

void LcdSetup(void);
//void SPI1Put(LCDSETUP);														// hidden (9DP v1.01.21)

void PutImage1BPP(SHORT left, SHORT top, FLASH_BYTE* bitmap, BYTE stretch);
void PutImage4BPP(SHORT left, SHORT top, FLASH_BYTE* bitmap, BYTE stretch);
void PutImage8BPP(SHORT left, SHORT top, FLASH_BYTE* bitmap, BYTE stretch);
void PutImage16BPP(SHORT left, SHORT top, FLASH_BYTE* bitmap, BYTE stretch);

void PutImage1BPPExt(SHORT left, SHORT top, void* bitmap, BYTE stretch);
void PutImage4BPPExt(SHORT left, SHORT top, void* bitmap, BYTE stretch);
void PutImage8BPPExt(SHORT left, SHORT top, void* bitmap, BYTE stretch);
void PutImage16BPPExt(SHORT left, SHORT top, void* bitmap, BYTE stretch);

void InitializeBackLight(void);


/*********************************************************************
* Function:  void  DelayMs(WORD time)
*
* PreCondition: none
*
* Input: time - delay in ms
*
* Output: none
*
* Side Effects: none
*
* Overview: delays execution on time specified in ms
*
* Note: delay is defined for 16MIPS
*
********************************************************************/
#ifdef __PIC32MX
void  DelayMs(WORD time)
{
	DWORD configRegister;											// (9DP v1.02.00)

	configRegister = DEVCFG2 & 0x00000070;							// (9DP v1.02.00)

    while(time--)
    {
        unsigned int int_status;

        int_status = INTDisableInterrupts();
//		OpenCoreTimer(GetSystemClock() / 2000);     				// core timer is at 1/2 system clock
		if(configRegister == 0x00000030)							// FPLLMULT = 3 [18x] (9DP v1.02.00)
		{
        	OpenCoreTimer(GetSystemClock_72MHz() / 2000);   		// (9DP v1.02.00)
		}
		else														// FPLLMULT = 5 [20x] (9DP v1.02.00)
		{
        	OpenCoreTimer(GetSystemClock_80MHz() / 2000);    		// (9DP v1.02.00)
		}
        INTRestoreInterrupts(int_status);
        mCTClearIntFlag();
        while(!mCTGetIntFlag());									// was cleared
    }

    mCTClearIntFlag();
}
#else
#define DELAY_1MS 16000/5  											// for 16MIPS
void  DelayMs(WORD time)
{
	unsigned delay;

	while(time--)
		for(delay=0; delay<DELAY_1MS; delay++);	
}
#endif

/*********************************************************************
* Function:  void SetAddress(DWORD address)
*
* PreCondition: none
*
* Input: address - address 
*
* Output: none
*
* Side Effects: none
*
* Overview: sets the address for read/write operations
*
* Note: none
*
********************************************************************/
#ifndef __PIC32MX
void SetAddress(DWORD address)
{
	UINT16 status;													// (9DP v1.01.20)

	status = INTDisableInterrupts();								// (9DP v1.01.20)
	A17_LAT_BIT_CLR;												// address 17-19 = 0
	A16_LAT_BIT_CLR;												// address 16 = 0
    PMPWaitBusy();      											// (9DP v1.01.20)
	PMADDR = address;
    PMPWaitBusy();         											// (9DP v1.01.20)
	status = INTEnableInterrupts();									// (9DP v1.01.20)

	return;															// (9DP v1.01.20)
}
#endif

#ifdef __PIC32MX

/*********************************************************************
* Function:  void  WriteData(WORD value)
*
* PreCondition: none
*
* Input:  value - value to be written
*
* Output: none
*
* Side Effects: none
*
* Overview: writes 16-bit word into video buffer at current address.
*           The address pointer is incremented after the operation.
*
* Note: none
*
********************************************************************/
void WriteData(DWORD value)
{
	UINT16 status;													// (9DP v1.01.20)

	status = INTDisableInterrupts();								// (9DP v1.01.20)
	RS_LAT_BIT_SET;       											// set RS line to low for register space access / high to access the display buffer

	if(_address < 0x10000)
	{
		A17_LAT_BIT_CLR;											// set address 17-19 = 0
		A16_LAT_BIT_CLR;											// set address 16 = 0
	}
	else if(_address < 0x20000)										// was 0x20000
	{
		A16_LAT_BIT_SET;											// set address 16 = 1
		A17_LAT_BIT_CLR;											// set address 17 = 0
	}
	else if(_address < 0x30000)										// was 0x1FFFF		else if(_address > 0x12BF0)	// was 0x1FFFF
	{
		A16_LAT_BIT_CLR;											// set address 16 = 0
		A17_LAT_BIT_SET;											// set address 17 = 1
	}
	else if(_address > 0x2FFFF)
	{
		A16_LAT_BIT_SET;											// set address 16 = 1
		A17_LAT_BIT_SET;											// set address 17 = 1
	}

    PMPWaitBusy();     												// (9DP v1.01.20)
	PMADDR = _address;
	PMPWaitBusy();													// (9DP v1.01.20)
	PMDIN1 = value;													// write data
	PMPWaitBusy();													// wait for the transmission end

	status = INTEnableInterrupts();									// (9DP v1.01.20)

	return;															// (9DP v1.01.20)
}
#endif

/*******************************************************************
* Function: void SSDPMPInit(void)
*                                     
* Overview: Initializes PMP in preparation of access to the SSD1906.
* 		    PMP access timing must be set up to meet specs.		
*                                                                      
* Input: none                                                          
*                                                                       
* Output: none
*********************************************************************/
//void SSDPMPInit(void)
//{
//	while(mIsPMPBusy());	// wait until PMP not busy anymore
//
//	mPMPOpen(CONTROL, MODE, PORT, INT);
//}


/***************************************************************************
*	Setup SPI struct / union for combining data for transfer
*
*	The order of the struct was flipped with DATAVALUE and TYPEDATA before
*	REGVALUE and TYPEREG since the data was backwords when viewing the 
* 	discrete SPI output on the oscilloscope.
***************************************************************************/
typedef struct tagSPIMESSAGE {
  union {
	struct {
		unsigned char DATAVALUE;	// 8-bit Data value
		unsigned char TYPEDATA;		// 8-bit Value that indicates Data values next
		unsigned char REGVALUE;		// 8-bit Register number
		unsigned char TYPEREG;		// 8-bit Value that indicates Register number next

	};
	struct {
	
		unsigned int WORDDATA;		//was uint -  16-bit Data info (TYPEDATA, DATAVALUE) combined
									// Value is before Type because data is swapped when sent.
		unsigned int WORDREG;		//was uint -  16-bit Register info (TYPEREG, REGVALUE) combined

	};
	struct {
		unsigned long DWORDSPI;		// try combining again to 32-bit
	};
  };
}  LCDSETUP;	

/*********************************************************************
* Function:  void  SetReg(WORD index, BYTE value)
*
* PreCondition: none
*
* Input: index - register number
*        value - value to be set
*
* Output: none
*
* Side Effects: none
*
* Overview: sets graphics controller register (byte access)
*
* Note: none
*
********************************************************************/
void  SetReg(WORD index, WORD value)
{
	UINT16 status;													// (9DP v1.01.20)

	status = INTDisableInterrupts();								// (9DP v1.01.20)
   	RS_LAT_BIT_CLR;													// set RS line Low for register space access
	CS_LAT_BIT_CLR;													// set CS Low to enable SSD1906

	A17_LAT_BIT_CLR;												// set address 17-19 = 0
	A16_LAT_BIT_CLR;												// set address 16 = 0

    PMPWaitBusy();       											// (9DP v1.01.20)
    PMADDR = index;         										// set address[15:0]
    PMPWaitBusy();          										// (9DP v1.01.20)
    PMDIN1 = value;        											// write value
    PMPWaitBusy();          										// wait for the transmission end
  
	CS_LAT_BIT_SET;													// set CS High - Disable SSD1906
	status = INTEnableInterrupts();									// (9DP v1.01.20)

	return;															// (9DP v1.01.20)
}

/*********************************************************************
* Function:  BYTE GetReg(WORD index)
*
* PreCondition: none
*
* Input: index - register number
*
* Output: none
*
* Side Effects: none
*
* Overview: returns graphics controller register value (byte access)
*
* Note: none
*
********************************************************************/
WORD  GetReg(WORD index)
{
	WORD indexnumb;
	WORD value;
	UINT16 status;													// (9DP v1.01.20)

	status = INTDisableInterrupts();								// (9DP v1.01.20)
	RS_LAT_BIT_CLR;													// set RS Low for register space access
	CS_LAT_BIT_CLR;													// set CS Low - Enable SSD1906

	A17_LAT_BIT_CLR;												// set address 17-19 = 0
	A16_LAT_BIT_CLR;												// set address 16 = 0

	indexnumb = index & 0xFFFE;										// make sure register will be 'Even' value

    PMPWaitBusy();    												// (9DP v1.01.20)
	PMADDR = indexnumb;												// set address{15:0]
    PMPWaitBusy();         											// (9DP v1.01.20)
    value = PMDIN1;        											// start transmission, read dummy value
    PMPWaitBusy();         											// wait for the transmission end

	CS_LAT_BIT_SET;													// set CS High  - Disable SSD1906

	DelayMs(1);														// -- add small delay loop

	CS_LAT_BIT_CLR;													// set CS Low - Enable SSD1906

    PMPWaitBusy();         											// (9DP v1.01.20)
	value = PMDIN1;        											// start transmission, read dummy value
    PMPWaitBusy();         											// wait for the transmission end

	CS_LAT_BIT_SET;													// set CS High  - Disable SSD1906

    PMCONbits.PMPEN  = 0;  											// suspend PMP

	if(index % 2 == 0)
	{
		value = PMDIN1;
		value = value & 0x00FF;
	}
	else
	{
		value = PMDIN2;
		value = ((value >> 8) & 0x00FF);
	}

    PMCONbits.PMPEN  = 1;  											// resume PMP
	status = INTEnableInterrupts();									// (9DP v1.01.20)

    return value;
}

/*********************************************************************
* Function:  void ResetDevice()
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: resets LCD, initializes PMP
*
* Note: none
*
********************************************************************/
void ResetDevice(void)
{
	BYTE bReg;

	RST_TRIS_BIT_OUT;												// set reset line as output
	RST_LAT_BIT_CLR;												// Pull Low - Reset SSD1906

	A17_TRIS_BIT_OUT;												// set address 17-19 as output
	A16_TRIS_BIT_OUT;												// set address 16 as output

	A17_LAT_BIT_CLR;												// set address 17-19 = 0
	A16_LAT_BIT_CLR;												// set address 16 = 0	

	RS_TRIS_BIT_OUT;												// set register select line as output
	RS_LAT_BIT_CLR;													// set RS Low for register space access

	CS_TRIS_BIT_OUT;												// set CS line as output
	CS_LAT_BIT_SET;													// set CS High - SSD1906 Not selected by default
  
// PMP setup 
    PMMODE = 0;
	PMAEN = 0; 
	PMCON = 0;
    PMMODEbits.MODE0   = 0;  										// was 0 - Intel 80 master interface
	PMMODEbits.MODE1   = 1;  										// was 1 - Intel 80 master interface
    PMMODEbits.WAITB  = 3;
    PMMODEbits.WAITM  = 13;
    PMMODEbits.WAITE  = 3;
    PMMODEbits.INCM   = 1;
 	PMMODEbits.MODE16 = 1;  										// ---- changed to 1 for 16-bit mode ----  // 16 bit data are multiplexed
	PMAENbits.PTEN15 = 1;											// enable PMA Address Port bits
	PMAENbits.PTEN14 = 1;
	PMAENbits.PTEN13 = 1;
	PMAENbits.PTEN12 = 1;
	PMAENbits.PTEN11 = 1;
	PMAENbits.PTEN10 = 1;
	PMAENbits.PTEN9 = 1;
	PMAENbits.PTEN8 = 1;
	PMAENbits.PTEN7 = 1;
	PMAENbits.PTEN6 = 1;
	PMAENbits.PTEN5 = 1;
	PMAENbits.PTEN4 = 1;
	PMAENbits.PTEN3 = 1;
	PMAENbits.PTEN2 = 1;
    PMAENbits.PTEN0 = 1;   											// enable low address latch
    PMAENbits.PTEN1 = 1;   											// enable high address latch
	PMCONbits.w	 &= 0xffffbfff; // Clear FRZ bit: Compiler error when setting PMConBits.FRZ. I have no idea why.											// new -- added - continue operation in debug exception mode
	PMCONbits.SIDL   = 0;											// new -- added - continue ouperation in IDLE mode
    PMCONbits.ADRMUX = 0;   										// was 2 (address is multiplexed on data bus)
	PMCONbits.PMPTTL = 0;											// new -- added - Schmidt Triggered
    PMCONbits.CSF    = 0;											// 0 = PMCS2 and PMCS1 function as address bits 15 and 14
    PMCONbits.ALP    = 1;  											// set address latch control polarity active high
	PMCONbits.CS2P	 = 0;											// new -- added
	PMCONbits.CS1P	 = 0;											// new -- added
	PMCONbits.WRSP	 = 0;											// new -- added
	PMCONbits.RDSP	 = 0;											// new -- added
    PMCONbits.PTRDEN = 1;  											//1 enable RD line
    PMCONbits.PTWREN = 1;  											//1 was 1  ---enable WR line
    PMCONbits.ON  = 1;  											// enable PMP

    DelayMs(40);
	RST_LAT_BIT_SET;												// set RESET High - release SSD1906 from reset
    DelayMs(1);														// was 20ms ///////////////////////

/************ Setup LCD and configure the SSD1906 for the LCD	******************/

	LcdSetup();														// Send Register Data via SPI

	SetReg(REG_MEMCLK_CONFIG, 0x3200);								

	SetReg(REG_PANEL_TYPE, 0x0061);									

#define HT (DISP_HOR_PULSE_WIDTH+DISP_HOR_BACK_PORCH+DISP_HOR_FRONT_PORCH+DISP_HOR_RESOLUTION)

	SetReg(REG_HORIZ_TOTAL, (0x00FF & (HT/8-1)));

	SetReg(REG_HDP, (0x00FF & (DISP_HOR_RESOLUTION/8-1)));


#define HDPS (DISP_HOR_PULSE_WIDTH+DISP_HOR_BACK_PORCH)

	SetReg(REG_HDP_START_POS0, (0x00FF & HDPS));

	SetReg(REG_HSYNC_PULSE_WIDTH, (0x00FF & (DISP_HOR_PULSE_WIDTH-1)));

#define VT  (DISP_VER_PULSE_WIDTH+DISP_VER_BACK_PORCH+DISP_VER_FRONT_PORCH+DISP_VER_RESOLUTION)	// -1

	SetReg(REG_VERT_TOTAL0, VT);

#define VDP (DISP_VER_RESOLUTION-1)

	SetReg(REG_VDP0, VDP);


#define VDPS  (DISP_VER_PULSE_WIDTH+DISP_VER_BACK_PORCH)

	SetReg(REG_VDP_START_POS0, VDPS);

	SetReg(REG_VSYNC_PULSE_WIDTH, (DISP_VER_PULSE_WIDTH-1));

	SetReg(0x26, 0x0000);											// write 0x00 to both reg 26h and 27h

#define  WIN_START_ADDR		0x9560									// was 159 for 90deg and 0 for 0deg
#define  ROTATION           3										// was 1 for 90 --- 3 for 270

	SetReg(REG_DISPLAY_MODE, 0x0304);								// Reg[71h]	= 03h	-	Reg[70h] = 04h =4

	SetReg(REG_MAIN_WIN_DISP_START_ADDR0, WIN_START_ADDR & 0xFFFF);			

	SetReg(REG_MAIN_WIN_DISP_START_ADDR2, (WIN_START_ADDR>>16)&0x00FF);		

#define WIN_OFFSET	160												// was 120 0deg

	SetReg(REG_MAIN_WIN_ADDR_OFFSET0, WIN_OFFSET);					// Reg[79h] = 00h		-	Reg[78h] = A0h	--> A0h = 160

	SetReg(REG_POWER_SAVE_CONFIG, 0x0000);							// Reg[A1h] - NonExistent	- Reg[A0h]	= 00h

	return;															// (9DP v1.01.20)
}

/*********************************************************************
* Function: void PutPixel(SHORT x, SHORT y)
*
* PreCondition: none
*
* Input: x,y - pixel coordinates
*
* Output: none
*
* Side Effects: none
*
* Overview: puts pixel
*
* Note: none
*
********************************************************************/
void PutPixel(SHORT x, SHORT y)
{
	DWORD address;

//	if(_clipRgn){													// hidden (9DP v1.01.20)
//  	if(x<_clipLeft)												// hidden (9DP v1.01.20)
//      	return;													// hidden (9DP v1.01.20)
//		if(x>_clipRight)											// hidden (9DP v1.01.20)
//			return;													// hidden (9DP v1.01.20)
//		if(y<_clipTop)												// hidden (9DP v1.01.20)
//			return;													// hidden (9DP v1.01.20)
//		if(y>_clipBottom)											// hidden (9DP v1.01.20)
//			return;													// hidden (9DP v1.01.20)
//    }																// hidden (9DP v1.01.20)

	if(x < 0)														// (9DP v1.01.20)
		return;														// (9DP v1.01.20)
	if(x > 319)														// (9DP v1.01.20)
		return;														// (9DP v1.01.20)
	if(y < 0)														// (9DP v1.01.20)
		return;														// (9DP v1.01.20)
	if(y > 239)														// (9DP v1.01.20)
		return;														// (9DP v1.01.20)

//	address = (DWORD)(GetMaxX( ) +1)*y + x;
    address = (DWORD)(((GetMaxX( ) + 1) * y) + x);					// cleaned up just in case (9DP v1.01.20)

// Original coding line that was shifted
	SetAddress(address);											// multiply the address by 2 - Aik's Suggestion

	CS_LAT_BIT_CLR;													// set CS LOW - Enable SSD1906

    WriteData(_color);

	CS_LAT_BIT_SET;													// set CS High - Disable SSD1906
	
	return;															// (9DP v1.01.20)
}

/*******************************************************************
*	Output SPI for LCD Setup
*
********************************************************************/
void SPI1Put(LCDSETUP data1)
{
	signed long SPIBUFFER;
	char index;
	UINT16 status;													// (9DP v1.01.20)

	status = INTDisableInterrupts();								// (9DP v1.01.20)
	SERIALCLK_LAT_CLR;												// serial CLK pulled Low
	CHIPSELECT_LAT_CLR;												// CS Pulled Low - LCD Selected
	SPIBUFFER = data1.DWORDSPI;

	for(index=0; index<32; index++)
	{	
		if(SPIBUFFER < 0)											// check if negative
		{
			SERIALDATA_LAT_SET;										// Serial Data Pulled High
		}
		else
		{
			SERIALDATA_LAT_CLR;										// Serial Data Pulled Low
		}
		DelayMs(1);
		SERIALCLK_LAT_SET;											// Serial CLK Pulled High
		SPIBUFFER = (SPIBUFFER << 1);								// Bit shift left 1
		DelayMs(1);	
		SERIALCLK_LAT_CLR;											// Serial CLK Pulled Low
	}
	DelayMs(1);
	CHIPSELECT_LAT_SET;												// CS Pulled High - LCD not Selected
	status = INTEnableInterrupts();									// (9DP v1.01.20)

	return;															// (9DP v1.01.20)
}


/*******************************************************************
*	Output SPI to wake up LCD
*
********************************************************************/
void LcdWAKEUP(void)
{

	LCDSETUP LCDsetup;				// Let LCDsetup be used in place of LCDSETUP
	LCDsetup.TYPEREG	= 0x00;		// Set TYPEREG struct LCD hex value to indicate the next 8-bits are Register values
	LCDsetup.TYPEDATA 	= 0x01;		// Set TYPEDATA struct LCD hex value to indicate the next 8-bits are Data values

/****	Register Settings from NEC Datasheet Page 12	****/
/****	Wake Up - Command Sequence						****/

	LCDsetup.REGVALUE	= 0x01;		// Register 1 (01h) - Sets REGVALUE value in struct LCDsetup
	LCDsetup.DATAVALUE	= 0x00;		// Data (00h) - Sets DATAVALUE value in struct LCDsetup
	SPI1Put(LCDsetup);				// Send struct LCD data via bit bang SPI sequence

	LCDsetup.REGVALUE	= 0x18;		// Register 24 (18h) - Sets REGVALUE value in struct LCDsetup
	LCDsetup.DATAVALUE	= 0xFF;		// Data (FFh) - Sets DATAVALUE value in struct LCDsetup
	SPI1Put(LCDsetup);				// Send struct LCD data via bit bang SPI sequence

	DelayMs(1);						// 30us minimum wait time

	LCDsetup.REGVALUE	= 0x00;		// Register 0 (00h) - Sets REGVALUE value in struct LCDsetup
	LCDsetup.DATAVALUE	= 0x00;		// Data (00h) - Sets DATAVALUE value in struct LCDsetup
	SPI1Put(LCDsetup);				// Send struct LCD data via bit bang SPI sequence

	return;							// (9DP v1.01.20)
}

/******************************************************************
*	Configure LCD Registers
*	Using Bit Bang SPI Interface
*
*	Standby Command Sequence
******************************************************************/
void LcdSTANDBY(void)
{

	LCDSETUP LCDsetup;				// Let LCDsetup be used in place of LCDSETUP
	LCDsetup.TYPEREG	= 0x00;		// Set TYPEREG struct LCD hex value to indicate the next 8-bits are Register values
	LCDsetup.TYPEDATA 	= 0x01;		// Set TYPEDATA struct LCD hex value to indicate the next 8-bits are Data values

/****	Register Settings from NEC Datasheet Page 12	****/
/****	StandBy - Command Sequence						****/

	LCDsetup.REGVALUE	= 0x00;		// Register 0 (00h) - Sets REGVALUE value in struct LCDsetup
	LCDsetup.DATAVALUE	= 0x08;		// Data (08h) - Sets DATAVALUE value in struct LCDsetup
	SPI1Put(LCDsetup);				// Send struct LCD data via bit bang SPI sequence

	DelayMs(25);					// 25ms minimum wait time

	LCDsetup.REGVALUE	= 0x18;		// Register 24 (18h)
	LCDsetup.DATAVALUE 	= 0x00;		// Data (00h)
	SPI1Put(LCDsetup);				// Send struct LCD data via bit bang SPI sequence

	DelayMs(20);					// 20ms minimum wait time

	LCDsetup.REGVALUE	= 0x01;		// Register 1 (01h)
	LCDsetup.DATAVALUE 	= 0x08;		// Data (08h)
	SPI1Put(LCDsetup);				// Send struct LCD data via bit bang SPI sequence

	return;							// (9DP v1.01.20)
}	


/******************************************************************
*	Configure LCD Registers
*	Using Bit Bang SPI Interface
*
*	Power OFF Command Sequence
******************************************************************/
void LcdPWROFF(void){

	LCDSETUP LCDsetup;				// Let LCDsetup be used in place of LCDSETUP
	LCDsetup.TYPEREG	= 0x00;		// Set TYPEREG struct LCD hex value to indicate the next 8-bits are Register values
	LCDsetup.TYPEDATA 	= 0x01;		// Set TYPEDATA struct LCD hex value to indicate the next 8-bits are Data values

/****	Register Settings from NEC Datasheet Page 11	****/
/****	Power Off - Command Sequence					****/

	LCDsetup.REGVALUE	= 0x00;		// Register 0 (00h) - Sets REGVALUE value in struct LCDsetup
	LCDsetup.DATAVALUE	= 0x08;		// Data (08h) - Sets DATAVALUE value in struct LCDsetup
	SPI1Put(LCDsetup);				// Send struct LCD data via bit bang SPI sequence

	DelayMs(25);					// 25ms minimum wait time

	LCDsetup.REGVALUE	= 0x18;		// Register 24 (18h)
	LCDsetup.DATAVALUE 	= 0x00;		// Data (00h)
	SPI1Put(LCDsetup);				// Send struct LCD data via bit bang SPI sequence

	DelayMs(20);					// 20ms minimum wait time

	LCDsetup.REGVALUE	= 0x01;		// Register 1 (01h)
	LCDsetup.DATAVALUE 	= 0x08;		// Data (08h)
	SPI1Put(LCDsetup);				// Send struct LCD data via bit bang SPI sequence

	return;							// (9DP v1.01.20)
}


/*******************************************************************
*	Setup LCD Registers
*	Using Bit Bang SPI Interface
*
*	POWER ON Command Sequence
*******************************************************************/
void LcdSetup(void) {
	
	LCDSETUP LCDsetup;				// Let LCDsetup be used in place of LCDSETUP
	LCDsetup.TYPEREG 	= 0x00;		// Set TYPEREG struct LCD hex value to indicate that the next 8-bits are Register values
	LCDsetup.TYPEDATA	= 0x01;		// Set TYPEDATA struct LCD hex value to indicate that the next 8-bits is Data values
	
	LCDRESET_TRIS_OUT;				// Configure PORTD pin 8 (LCD RESET) line as output
	LCDRESET_LAT_CLR;				// Clear RESET Line to reset the LCD
	DelayMs(1);						// short 1ms delay
	LCDRESET_LAT_SET;				// Pull the RESET Line high to end the LCD reset operation

	DelayMs(5);						// 1ms min. wait	

/****	Register settings - values from NEC datasheet Page 11	********/
/****	Power On - Command Sequence		****/

	LCDsetup.REGVALUE = 0x03;		// Register 3 - Sets REGVALUE value in struct LCDsetup
	LCDsetup.DATAVALUE = 0x01;		// Data (01h) - Sets DATAVALUE value in struct LCDsetup
	SPI1Put(LCDsetup);				// Send struct LCD data via bit bang SPI sequence

	LCDsetup.REGVALUE = 0x01;		// Register 1
	LCDsetup.DATAVALUE = 0x00;		// Data (00h)
	SPI1Put(LCDsetup);

	LCDsetup.REGVALUE = 0x64;		// Register 100 (64h)
	LCDsetup.DATAVALUE = 0x0F;		// Data (0Fh)
	SPI1Put(LCDsetup);

	LCDsetup.REGVALUE = 0x65;		// Register 101 (65h)
	LCDsetup.DATAVALUE = 0x3F;		// Data (3Fh)
	SPI1Put(LCDsetup);

	LCDsetup.REGVALUE = 0x66;		// Register 102 (66h)
	LCDsetup.DATAVALUE = 0x3F;
	SPI1Put(LCDsetup);

	LCDsetup.REGVALUE = 0x67;		// Register 103 (67h)
	LCDsetup.DATAVALUE = 0x00;
	SPI1Put(LCDsetup);

	LCDsetup.REGVALUE = 0x68;		// Reg 104
	LCDsetup.DATAVALUE = 0x00;
	SPI1Put(LCDsetup);

	LCDsetup.REGVALUE = 0x69;		// Reg 105
	LCDsetup.DATAVALUE = 0x30;
	SPI1Put(LCDsetup);

	LCDsetup.REGVALUE = 0x6A;		// Reg 106
	LCDsetup.DATAVALUE = 0x04;
	SPI1Put(LCDsetup);

	LCDsetup.REGVALUE = 0x6B;		// Reg 107
	LCDsetup.DATAVALUE = 0x37;
	SPI1Put(LCDsetup);

	LCDsetup.REGVALUE = 0x6C;		// Reg 108
	LCDsetup.DATAVALUE = 0x17;
	SPI1Put(LCDsetup);

	LCDsetup.REGVALUE = 0x6D;		// Reg 109
	LCDsetup.DATAVALUE = 0x00;
	SPI1Put(LCDsetup);

	LCDsetup.REGVALUE = 0x6E;		// Reg 110
	LCDsetup.DATAVALUE = 0x40;
	SPI1Put(LCDsetup);

	LCDsetup.REGVALUE = 0x6F;		// Reg 111
	LCDsetup.DATAVALUE = 0x30;
	SPI1Put(LCDsetup);	

	LCDsetup.REGVALUE = 0x70;		// Reg 112
	LCDsetup.DATAVALUE = 0x04;
	SPI1Put(LCDsetup);

	LCDsetup.REGVALUE = 0x71;		// Reg 113
	LCDsetup.DATAVALUE = 0x37;
	SPI1Put(LCDsetup);

	LCDsetup.REGVALUE = 0x72;		// Reg 114
	LCDsetup.DATAVALUE = 0x17;
	SPI1Put(LCDsetup);

	LCDsetup.REGVALUE = 0x73;		// Reg 115
	LCDsetup.DATAVALUE = 0x00;
	SPI1Put(LCDsetup);

	LCDsetup.REGVALUE = 0x74;		// Reg 116
	LCDsetup.DATAVALUE = 0x40;
	SPI1Put(LCDsetup);

	LCDsetup.REGVALUE = 0x02;		// Reg 2
	LCDsetup.DATAVALUE = 0x40;
	SPI1Put(LCDsetup);

	LCDsetup.REGVALUE = 0x4B;		// Reg 75
	LCDsetup.DATAVALUE = 0x04;
	SPI1Put(LCDsetup);

	LCDsetup.REGVALUE = 0x4C;		// Reg 76
	LCDsetup.DATAVALUE = 0x01;
	SPI1Put(LCDsetup);

	LCDsetup.REGVALUE = 0x4D;		// Reg 77
	LCDsetup.DATAVALUE = 0x01;
	SPI1Put(LCDsetup);

	LCDsetup.REGVALUE = 0x50;		// Reg 80
	LCDsetup.DATAVALUE = 0x00;
	SPI1Put(LCDsetup);

	LCDsetup.REGVALUE = 0x51;		// Reg 81
	LCDsetup.DATAVALUE = 0x00;
	SPI1Put(LCDsetup);

	LCDsetup.REGVALUE = 0x52;		// Reg 82
	LCDsetup.DATAVALUE = 0x2E;
	SPI1Put(LCDsetup);

	LCDsetup.REGVALUE = 0x53;		// Reg 83
	LCDsetup.DATAVALUE = 0xC4;
	SPI1Put(LCDsetup);

	LCDsetup.REGVALUE = 0x56;		// Reg 86
	LCDsetup.DATAVALUE = 0x15;
	SPI1Put(LCDsetup);

	LCDsetup.REGVALUE = 0x57;		// Reg 87
	LCDsetup.DATAVALUE = 0xED;
	SPI1Put(LCDsetup);

	LCDsetup.REGVALUE = 0x5F;		// Reg 95
	LCDsetup.DATAVALUE = 0x3F;
	SPI1Put(LCDsetup);

	LCDsetup.REGVALUE = 0x60;		// Reg 96
	LCDsetup.DATAVALUE = 0x22;
	SPI1Put(LCDsetup);

	LCDsetup.REGVALUE = 0x19;		// Reg 25
	LCDsetup.DATAVALUE = 0x76;
	SPI1Put(LCDsetup);

	LCDsetup.REGVALUE = 0x1A;		// Reg 26
	LCDsetup.DATAVALUE = 0x54;
	SPI1Put(LCDsetup);

	LCDsetup.REGVALUE = 0x1B;		// Reg 27
	LCDsetup.DATAVALUE = 0x67;
	SPI1Put(LCDsetup);

	LCDsetup.REGVALUE = 0x1C;		// Reg 28
	LCDsetup.DATAVALUE = 0x60;
	SPI1Put(LCDsetup);

	LCDsetup.REGVALUE = 0x1D;		// Reg 29
	LCDsetup.DATAVALUE = 0x04;
	SPI1Put(LCDsetup);

	LCDsetup.REGVALUE = 0x1E;		// Reg 30
	LCDsetup.DATAVALUE = 0x1C;
	SPI1Put(LCDsetup);

	LCDsetup.REGVALUE = 0x1F;		// Reg 31
	LCDsetup.DATAVALUE = 0xA9;
	SPI1Put(LCDsetup);

	LCDsetup.REGVALUE = 0x20;		// Reg 32
	LCDsetup.DATAVALUE = 0x00;
	SPI1Put(LCDsetup);

	LCDsetup.REGVALUE = 0x21;		// Reg 33
	LCDsetup.DATAVALUE = 0x20;
	SPI1Put(LCDsetup);

	LCDsetup.REGVALUE = 0x18;		// Reg 24
	LCDsetup.DATAVALUE = 0x77;
	SPI1Put(LCDsetup);

	DelayMs(1);						// 30us minimum wait

	///// Data Input Start <-- is what data sheet says, not sure what to do here?

	LCDsetup.REGVALUE = 0x3B;		// Reg 59
	LCDsetup.DATAVALUE = 0x01;
	SPI1Put(LCDsetup);

	DelayMs(20);					// 20ms minimum wait

	LCDsetup.REGVALUE = 0x00;		// Reg 0
	LCDsetup.DATAVALUE = 0x00;
	SPI1Put(LCDsetup);

	return;							// (9DP v1.01.20)
}

/*********************************************************************
* Function: WORD GetPixel(SHORT x, SHORT y)
*
* PreCondition: none
*
* Input: x,y - pixel coordinates 
*
* Output: pixel color
*
* Side Effects: none
*
* Overview: returns pixel color at x,y position
*
* Note: none
*
********************************************************************/
WORD GetPixel(SHORT x, SHORT y)
{
	DWORD address;
	WORD  value;
	UINT16 status;													// (9DP v1.01.20)

    address = (long)(GetMaxX( ) +1)*y + x;
    SetAddress(address);
	status = INTDisableInterrupts();								// (9DP v1.01.20)
	CS_LAT_BIT_CLR;													// CS pulled Low - Enable SSD1906
	A17_LAT_BIT_CLR;												// Set address 17-19 = 0
	A16_LAT_BIT_CLR;												// Set address 16 = 0
	value = PMDIN1;													// start transmission, read dummy value
	PMPWaitBusy();													// wait for the transmission end
	value = PMDIN1;													// start transmission, read data
	PMPWaitBusy();													// wait for the transmission end
	CS_LAT_BIT_SET;													// CS pulled High - Disable SSD1906
	status = INTEnableInterrupts();									// (9DP v1.01.20)

	return value;
}

/*********************************************************************
* Function: void Bar(SHORT left, SHORT top, SHORT right, SHORT bottom)
*
* PreCondition: none
*
* Input: left,top - top left corner coordinates,
*        right,bottom - bottom right corner coordinates
*
* Output: none
*
* Side Effects: none
*
* Overview: draws rectangle filled with current color
*
* Note: none
*
********************************************************************/
#ifdef USE_DRV_BAR

//void Bar(SHORT left, SHORT top, SHORT right, SHORT bottom){
WORD Bar(SHORT left, SHORT top, SHORT right, SHORT bottom)
{
	DWORD address;
	register SHORT  x,y;

//	if(_clipRgn){													// hidden (9DP v1.01.25)
//  	if(left<_clipLeft)											// hidden (9DP v1.01.25)
//      	left = _clipLeft;										// hidden (9DP v1.01.25)
//      if(right>_clipRight)										// hidden (9DP v1.01.25)
//          right= _clipRight;										// hidden (9DP v1.01.25)
//      if(top<_clipTop)											// hidden (9DP v1.01.25)
//          top = _clipTop;											// hidden (9DP v1.01.25)
//      if(bottom>_clipBottom)										// hidden (9DP v1.01.25)
//          bottom = _clipBottom;									// hidden (9DP v1.01.25)
//	}																// hidden (9DP v1.01.25)

	if(left < 0)													// (9DP v1.01.25)
		return;														// (9DP v1.01.25)
	if(right > 319)													// (9DP v1.01.25)
		return;														// (9DP v1.01.25)
	if(top < 0)														// (9DP v1.01.25)
		return;														// (9DP v1.01.25)
	if(bottom > 239)												// (9DP v1.01.25)
		return;														// (9DP v1.01.25)

    address = (DWORD)(GetMaxX( ) +1)*top + left;

	CS_LAT_BIT_CLR;													// CS pulled Low - Enable SSD1906

    for(y=top; y<bottom+1; y++)
	{
        SetAddress(address);										// multiply the address by 2 - Aik's Suggestion		//SetAddress(address);
        for(x=left; x<right+1; x++)
		{
            WriteData(_color);
        }
        address += (GetMaxX( ) + 1);
    }
	CS_LAT_BIT_SET;													// CS pulled High - Disable SSD1906

	return;															// (9DP v1.01.20)
}

#endif // USE_DRV_BAR


/*********************************************************************
* Function: void ClearDevice(void)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: clears screen with current color 
*
* Note: none
*
********************************************************************/
//void ClearDevice(void){
//DWORD     counter;
//
//    SetAddress(0);
//	
//	CS_LAT_BIT_CLR;		// CS Pulled Low - Enable SSD1906
//
//    for(counter=0; counter<(DWORD)(GetMaxX()+1)*(GetMaxY()+1); counter++){
////	for(counter=0; counter<(DWORD)(320)*(240); counter++){
//
//        WriteData(_color);
//    }
//	CS_LAT_BIT_SET;		// CS Pulled High - Disable SSD1906
//}


#ifdef USE_DRV_PUTIMAGE

/*********************************************************************
* Function: void PutImage(SHORT left, SHORT top, void* bitmap, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner,
*        bitmap - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
//void PutImage(SHORT left, SHORT top, void* bitmap, BYTE stretch){
WORD PutImage(SHORT left, SHORT top, void* bitmap, BYTE stretch){
FLASH_BYTE* flashAddress;
BYTE colorDepth;
WORD colorTemp;

    // Save current color
    colorTemp = _color;

    switch(*((SHORT*)bitmap))
    {
#ifdef USE_BITMAP_FLASH
        case FLASH:
            // Image address
            flashAddress = ((BITMAP_FLASH*)bitmap)->address;
            // Read color depth
            colorDepth = *(flashAddress+1);
            // Draw picture
            switch(colorDepth){
                case 1:
                    PutImage1BPP(left, top, flashAddress, stretch);
                    break;
                case 4:
                    PutImage4BPP(left, top, flashAddress, stretch);
                    break;
                case 8:
                    PutImage8BPP(left, top, flashAddress, stretch);
                    break;
                case 16:
                    PutImage16BPP(left, top, flashAddress, stretch);
                    break;
            }
            break;
#endif
#ifdef USE_BITMAP_EXTERNAL
        case EXTERNAL:
            // Get color depth
            ExternalMemoryCallback(bitmap, 1, 1, &colorDepth);
//PutImage16BPPExt(left, top, bitmap, stretch); /// added in
            // Draw picture
            switch(colorDepth){
                case 1:
                    PutImage1BPPExt(left, top, bitmap, stretch);
                    break;
                case 4:
                    PutImage4BPPExt(left, top, bitmap, stretch);
                    break;
                case 8:
                    PutImage8BPPExt(left, top, bitmap, stretch);
                    break;
                case 16:
                    PutImage16BPPExt(left, top, bitmap, stretch);
                    break;
                default:
                    break;
            }
            break;
#endif
        default:
            break;
    }

    // Restore current color
    _color = colorTemp;

	return;							// (9DP v1.01.20)
}



	#ifdef USE_BITMAP_FLASH
/*********************************************************************
* Function: void PutImage1BPP(SHORT left, SHORT top, FLASH_BYTE* bitmap, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner,
*        bitmap - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
void PutImage1BPP(SHORT left, SHORT top, FLASH_BYTE* bitmap, BYTE stretch){
register DWORD address;
register FLASH_BYTE* flashAddress;
register FLASH_BYTE* tempFlashAddress;
BYTE temp;
WORD sizeX, sizeY;
WORD x,y;
BYTE stretchX,stretchY;
WORD pallete[2];
BYTE mask;

    // Move pointer to size information
    flashAddress = bitmap + 2;

    // Set start address
    address = (long)(GetMaxX( ) +1)*top+ left;

    // Read image size
    sizeY = *((FLASH_WORD*)flashAddress);
    flashAddress += 2;
    sizeX = *((FLASH_WORD*)flashAddress);
    flashAddress += 2;
    pallete[0] = *((FLASH_WORD*)flashAddress);
    flashAddress += 2;
    pallete[1] = *((FLASH_WORD*)flashAddress);
    flashAddress += 2;

	CS_LAT_BIT_CLR;		// CS pulled Low - enable SSD1906

    for(y=0; y<sizeY; y++){
        tempFlashAddress = flashAddress;
        for(stretchY = 0; stretchY<stretch; stretchY++){
            flashAddress = tempFlashAddress;
            SetAddress(address);
            mask = 0;
            for(x=0; x<sizeX; x++){

                // Read 8 pixels from flash
                if(mask == 0){
                    temp = *flashAddress;
                    flashAddress++;
                    mask = 0x80;
                }
                
                // Set color
                if(mask&temp){
                    SetColor(pallete[1]);
                }else{
                    SetColor(pallete[0]);
                }

                // Write pixel to screen
                for(stretchX=0; stretchX<stretch; stretchX++){
                    WriteData(_color);
                }

                // Shift to the next pixel
                mask >>= 1;
           }
           address += (GetMaxX( ) +1); 
        }
    }
	CS_LAT_BIT_SET;		// CS pulled High - Disable SSD1906

	return;							// (9DP v1.01.20)
}

/*********************************************************************
* Function: void PutImage4BPP(SHORT left, SHORT top, FLASH_BYTE* bitmap, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, bitmap - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs 16 color image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
void PutImage4BPP(SHORT left, SHORT top, FLASH_BYTE* bitmap, BYTE stretch){
register DWORD address;
register FLASH_BYTE* flashAddress;
register FLASH_BYTE* tempFlashAddress;
WORD sizeX, sizeY;
register WORD x,y;
BYTE temp;
register BYTE stretchX,stretchY;
WORD pallete[16];
WORD counter;

    // Move pointer to size information
    flashAddress = bitmap + 2;

    // Set start address
    address = (long)(GetMaxX( ) +1)*top+ left;

    // Read image size
    sizeY = *((FLASH_WORD*)flashAddress);
    flashAddress += 2;
    sizeX = *((FLASH_WORD*)flashAddress);
    flashAddress += 2;

    // Read pallete
    for(counter=0;counter<16;counter++){
        pallete[counter] = *((FLASH_WORD*)flashAddress);
        flashAddress += 2;
    }

	CS_LAT_BIT_CLR;		// CS pulled Low - Enable SSD1906
    
    for(y=0; y<sizeY; y++){
        tempFlashAddress = flashAddress;
        for(stretchY = 0; stretchY<stretch; stretchY++){
            flashAddress = tempFlashAddress;
            SetAddress(address);
            for(x=0; x<sizeX; x++){
                // Read 2 pixels from flash
                if(x&0x0001){
                    // second pixel in byte
                    SetColor(pallete[temp>>4]);
                }else{
                    temp = *flashAddress;
                    flashAddress++;
                    // first pixel in byte
                    SetColor(pallete[temp&0x0f]);
                }

                // Write pixel to screen       
                for(stretchX=0; stretchX<stretch; stretchX++){
                    //WriteData(_color);
					PutPixel((left+x), (top+y));	// I added to test
                }

                // Shift to the next pixel
                //temp >>= 4;
            }
            address += (GetMaxX( ) +1);
        }
    }
	CS_LAT_BIT_SET;		// CS pulled High - Disable SSD1906

	return;							// (9DP v1.01.20)
}

/*********************************************************************
* Function: void PutImage8BPP(SHORT left, SHORT top, FLASH_BYTE* bitmap, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, bitmap - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs 256 color image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
void PutImage8BPP(SHORT left, SHORT top, FLASH_BYTE* bitmap, BYTE stretch){
register DWORD address;
register FLASH_BYTE* flashAddress;
register FLASH_BYTE* tempFlashAddress;
WORD sizeX, sizeY;
WORD x,y;
BYTE temp;
BYTE stretchX, stretchY;
WORD pallete[256];
WORD counter;

    // Move pointer to size information
    flashAddress = bitmap + 2;

    // Set start address
    address = (long)(GetMaxX( ) +1)*top+ left;

    // Read image size
    sizeY = *((FLASH_WORD*)flashAddress);
    flashAddress += 2;
    sizeX = *((FLASH_WORD*)flashAddress);
    flashAddress += 2;

    // Read pallete
    for(counter=0;counter<256;counter++){
        pallete[counter] = *((FLASH_WORD*)flashAddress);
        flashAddress += 2;
    }

	CS_LAT_BIT_CLR;		// CS pulled Low - Enable SSD1906
   
    for(y=0; y<sizeY; y++){
        tempFlashAddress = flashAddress;
        for(stretchY = 0; stretchY<stretch; stretchY++){
            flashAddress = tempFlashAddress;
            SetAddress(address);
            for(x=0; x<sizeX; x++){
                // Read pixels from flash
                temp = *flashAddress;
                flashAddress++;

                // Set color
                SetColor(pallete[temp]);

                // Write pixel to screen       
                for(stretchX=0; stretchX<stretch; stretchX++){
                    //WriteData(_color);
					PutPixel((left+x), (top+y));	// I added to test
                }
            }
            address += (GetMaxX( ) +1);
        }
    }
	CS_LAT_BIT_SET;		// CS pulled high - Disable SSD1906

	return;							// (9DP v1.01.20)
}

/*********************************************************************
* Function: void PutImage16BPP(SHORT left, SHORT top, FLASH_BYTE* bitmap, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, bitmap - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs hicolor image starting from left,top coordinates
*
* Note: image must be located in flash
*
********************************************************************/
void PutImage16BPP(SHORT left, SHORT top, FLASH_BYTE* bitmap, BYTE stretch){
register DWORD address;
register FLASH_WORD* flashAddress;
register FLASH_WORD* tempFlashAddress;
WORD sizeX, sizeY;
register WORD x,y;
WORD temp;
register BYTE stretchX,stretchY;

    // Move pointer to size information
    flashAddress = (FLASH_WORD*)bitmap + 1;

    // Set start address
    address = (long)(GetMaxX( ) +1)*top+ left;

    // Read image size
    sizeY = *flashAddress;
    flashAddress++;
    sizeX = *flashAddress;
    flashAddress++;

	CS_LAT_BIT_CLR;		// CS pulled Low - Enable SSD1906

    for(y=0; y<sizeY; y++){
        tempFlashAddress = flashAddress;
        for(stretchY = 0; stretchY<stretch; stretchY++){
            flashAddress = tempFlashAddress;
            SetAddress(address);
            for(x=0; x<sizeX; x++){
                // Read pixels from flash
                temp = *flashAddress;
                flashAddress++;

                // Set color
                SetColor(temp);

                // Write pixel to screen       
                for(stretchX=0; stretchX<stretch; stretchX++){
                    //WriteData(_color);
					PutPixel((left+x), (top+y));	// I added to test  -- worked
                }
            }
            address += (GetMaxX( ) +1);
        }
    }
	CS_LAT_BIT_SET;		// CS pulled High - Disable SSD1906

	return;							// (9DP v1.01.20)
}

	#endif // USE_BITMAP_FLASH

	#ifdef USE_BITMAP_EXTERNAL
/*********************************************************************
* Function: void PutImage1BPPExt(SHORT left, SHORT top, void* bitmap, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, bitmap - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: image must be located in external memory
*
********************************************************************/
void PutImage1BPPExt(SHORT left, SHORT top, void* bitmap, BYTE stretch){
register DWORD      address;
register DWORD      memOffset;
BITMAP_HEADER       bmp;
WORD                pallete[2];
BYTE                lineBuffer[(DISP_HOR_RESOLUTION/8)+1];
BYTE*               pData; 
SHORT               byteWidth;

BYTE                temp;
BYTE                mask;
WORD                sizeX, sizeY;
WORD                x,y;
BYTE                stretchX, stretchY;

    // Set start address
    address = (long)(GetMaxX( ) +1)*top+ left;

    // Get bitmap header
    ExternalMemoryCallback(bitmap, 0, sizeof(BITMAP_HEADER), &bmp);

    // Get pallete (2 entries)
    ExternalMemoryCallback(bitmap, sizeof(BITMAP_HEADER), 2*sizeof(WORD), pallete);

    // Set offset to the image data
    memOffset = sizeof(BITMAP_HEADER) + 2*sizeof(WORD);

    // Line width in bytes
    byteWidth = bmp.width>>3;
    if(bmp.width&0x0007)
        byteWidth++;

    // Get size
    sizeX = bmp.width;
    sizeY = bmp.height;

    for(y=0; y<sizeY; y++){
        // Get line
        ExternalMemoryCallback(bitmap, memOffset, byteWidth, lineBuffer);
        memOffset += byteWidth;

		CS_LAT_BIT_CLR;		// CS pulled Low - Enable SSD1906

        for(stretchY = 0; stretchY<stretch; stretchY++){
            pData = lineBuffer;
            SetAddress(address);
            mask = 0;
            for(x=0; x<sizeX; x++){

                // Read 8 pixels from flash
                if(mask == 0){
                    temp = *pData++;
                    mask = 0x80;
                }
                
                // Set color
                if(mask&temp){
                    SetColor(pallete[1]);
                }else{
                    SetColor(pallete[0]);
                }

                // Write pixel to screen
                for(stretchX=0; stretchX<stretch; stretchX++){
                    WriteData(_color);
                }

                // Shift to the next pixel
                mask >>= 1;
           }
           address += (GetMaxX( ) +1); 
        }
		CS_LAT_BIT_SET;		// CS pulled High - Disable SSD1906
    }

	return;							// (9DP v1.01.20)
}

/*********************************************************************
* Function: void PutImage4BPPExt(SHORT left, SHORT top, void* bitmap, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, bitmap - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: image must be located in external memory
*
********************************************************************/
void PutImage4BPPExt(SHORT left, SHORT top, void* bitmap, BYTE stretch){
register DWORD      address;
register DWORD      memOffset;
BITMAP_HEADER       bmp;
WORD                pallete[16];
BYTE                lineBuffer[(DISP_HOR_RESOLUTION/2)+1];
BYTE*               pData; 
SHORT               byteWidth;

BYTE                temp;
WORD                sizeX, sizeY;
WORD                x,y;
BYTE                stretchX, stretchY;

    // Set start address
    address = (long)(GetMaxX( ) +1)*top+ left;

    // Get bitmap header
    ExternalMemoryCallback(bitmap, 0, sizeof(BITMAP_HEADER), &bmp);

    // Get pallete (16 entries)
    ExternalMemoryCallback(bitmap, sizeof(BITMAP_HEADER), 16*sizeof(WORD), pallete);

    // Set offset to the image data
    memOffset = sizeof(BITMAP_HEADER) + 16*sizeof(WORD);

    // Line width in bytes
    byteWidth = bmp.width>>1;
    if(bmp.width&0x0001)
        byteWidth++;

    // Get size
    sizeX = bmp.width;
    sizeY = bmp.height;

    for(y=0; y<sizeY; y++){

        // Get line
        ExternalMemoryCallback(bitmap, memOffset, byteWidth, lineBuffer);
        memOffset += byteWidth;

		CS_LAT_BIT_CLR;			// CS pulled low - Enabled SSD1906

        for(stretchY = 0; stretchY<stretch; stretchY++){

            pData = lineBuffer;
            SetAddress(address);

            for(x=0; x<sizeX; x++){

                // Read 2 pixels from flash
                if(x&0x0001){
                    // second pixel in byte
                    SetColor(pallete[temp>>4]);
                }else{
                    temp = *pData++;
                    // first pixel in byte
                    SetColor(pallete[temp&0x0f]);
                }

                // Write pixel to screen       
                for(stretchX=0; stretchX<stretch; stretchX++){
                    //WriteData(_color);
					PutPixel((left+x), (top+y));	// I added to test  -- worked
                }
           }
           address += (GetMaxX( ) +1); 
        }
		CS_LAT_BIT_SET;			// CS pulled high - Disable SSD1906
    }

	return;							// (9DP v1.01.20)
}

/*********************************************************************
* Function: void PutImage8BPPExt(SHORT left, SHORT top, void* bitmap, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, bitmap - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: image must be located in external memory
*
********************************************************************/
void PutImage8BPPExt(SHORT left, SHORT top, void* bitmap, BYTE stretch){
register DWORD      address;
register DWORD      memOffset;
BITMAP_HEADER       bmp;
WORD                pallete[256];
BYTE                lineBuffer[DISP_HOR_RESOLUTION];
BYTE*               pData; 

BYTE                temp;
WORD                sizeX, sizeY;
WORD                x,y;
BYTE                stretchX, stretchY;

    // Set start address
    address = (long)(GetMaxX( ) +1)*top+ left;

    // Get bitmap header
    ExternalMemoryCallback(bitmap, 0, sizeof(BITMAP_HEADER), &bmp);

    // Get pallete (256 entries)
    ExternalMemoryCallback(bitmap, sizeof(BITMAP_HEADER), 256*sizeof(WORD), pallete);

    // Set offset to the image data
    memOffset = sizeof(BITMAP_HEADER) + 256*sizeof(WORD);

    // Get size
    sizeX = bmp.width;
    sizeY = bmp.height;

    for(y=0; y<sizeY; y++){

        // Get line
        ExternalMemoryCallback(bitmap, memOffset, sizeX, lineBuffer);
        memOffset += sizeX;

		CS_LAT_BIT_CLR;			// CS pulled Low - Enable SSD1906

        for(stretchY = 0; stretchY<stretch; stretchY++){

            pData = lineBuffer;
            SetAddress(address);

            for(x=0; x<sizeX; x++){

                temp = *pData++;
                SetColor(pallete[temp]);                    

                // Write pixel to screen       
                for(stretchX=0; stretchX<stretch; stretchX++){
                    WriteData(_color);
                }
           }
           address += (GetMaxX( ) +1); 
        }
		CS_LAT_BIT_SET;			// CS pulled High - Disable SSD1906
    }

	return;							// (9DP v1.01.20)
}

/*********************************************************************
* Function: void PutImage16BPPExt(SHORT left, SHORT top, void* bitmap, BYTE stretch)
*
* PreCondition: none
*
* Input: left,top - left top image corner, bitmap - image pointer,
*        stretch - image stretch factor
*
* Output: none
*
* Side Effects: none
*
* Overview: outputs monochrome image starting from left,top coordinates
*
* Note: image must be located in external memory
*
********************************************************************/
void PutImage16BPPExt(SHORT left, SHORT top, void* bitmap, BYTE stretch){
register DWORD      address;
register DWORD      memOffset;
BITMAP_HEADER       bmp;
WORD                lineBuffer[DISP_VER_RESOLUTION];	//<< -- set to VER_RESOLUTION because screen is 270 degrees, not 0 degrees
WORD*               pData; 
WORD                byteWidth;

WORD                temp;
WORD                sizeX, sizeY;
WORD                x,y;
BYTE                stretchX, stretchY;
WORD	test = 0;

    // Set start address
    address = (long)(GetMaxX( ) +1)*top+ left;

    // Get bitmap header
    ExternalMemoryCallback(bitmap, 0, sizeof(BITMAP_HEADER), &bmp);

    // Set offset to the image data
    memOffset = sizeof(BITMAP_HEADER);

//test = GetImageWidth(bitmap);


    // Get size
    sizeX = bmp.width;
    sizeY = bmp.height;

    byteWidth = sizeX<<1;

    for(y=0; y<sizeY; y++){
        // Get line
        ExternalMemoryCallback(bitmap, memOffset, byteWidth, lineBuffer);
        memOffset += byteWidth;

		CS_LAT_BIT_CLR;			// CS pulled Low - Enable SSD1906

        for(stretchY = 0; stretchY<stretch; stretchY++){

            pData = lineBuffer;
            SetAddress(address);

            for(x=0; x<sizeX; x++){

                temp = *pData++;
                SetColor(temp);                    

                // Write pixel to screen       
                for(stretchX=0; stretchX<stretch; stretchX++){
                    //WriteData(_color);
					PutPixel((left+x), (top+y));	// I added to test  -- worked
                }

           }

           address += (GetMaxX( ) +1); 

        }
		CS_LAT_BIT_SET;			// CS pulled High - Disable SSD1906
    }

	return;							// (9DP v1.01.20)
}

	#endif // USE_BITMAP_EXTERNAL

#endif // USE_DRV_PUTIMAGE

void InitializeBackLight(void)
{
	
	SetReg(0xB0, 0x2010);		// Reg[B0h] = 10h	-	Reg[B1h] = B0h
			
									// Enable PWM Clock -- was  0x10
									// bit 4 = 1 is for PWM CLK enable
									// bit 7 = 0 for PWM forced Low and PWM enabled

									// Select PWM divide by value and PWM Source
									// bit  0 = 0 is for PWMCLK is CLKI
									// bit7-4 = 5h is for PWM CLK divide by 32 == CLK of 46.875 kHz

	// Set PWM duty cycle to 50% - 128 out of 256 clock periods

	SetReg(0xB2, 0x0000);		// Reg[B2h] = 00h	-	Reg[B3h] = 80h
	return;							// (9DP v1.01.20)
}
