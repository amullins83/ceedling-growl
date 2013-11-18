// HardwareProfile.h

#include "GraphicsConfig.h"

#ifndef _HARDWARE_PROFILE_H_
#define _HARDWARE_PROFILE_H_

#define USB_A0_SILICON_WORK_AROUND
//#define RUN_AT_48MHZ
//#define RUN_AT_24MHZ
//#define RUN_AT_60MHZ
#define RUN_AT_72MHZ																				// (Test v0.00.01)
//#define RUN_AT_80MHZ																				// hidden (Test v0.00.01)

// Various clock values
#if defined(RUN_AT_48MHZ)
    #define GetSystemClock()						48000000UL
    #define GetPeripheralClock()					48000000UL
    #define GetInstructionClock()					(GetSystemClock() / 2) ???
#elif defined(RUN_AT_24MHZ)
    #define GetSystemClock()						24000000UL
    #define GetPeripheralClock()					24000000UL
    #define GetInstructionClock()					(GetSystemClock() / 2) ???
#elif defined(RUN_AT_60MHZ)    
    #define GetSystemClock()						60000000UL
    #define GetPeripheralClock()					30000000UL
    #define GetInstructionClock()					(GetSystemClock() / 2) ???
#elif defined(RUN_AT_72MHZ)    
    #define GetSystemClock()						72000000UL										// (Test v0.00.01)
    #define GetPeripheralClock()					36000000UL  									// (Test v0.00.01)
    #define GetInstructionClock()					(GetSystemClock() / 2) ???						// (Test v0.00.01)
#elif defined(RUN_AT_80MHZ)    
    #define GetSystemClock()						80000000UL
    #define GetPeripheralClock()					40000000UL
    #define GetInstructionClock()					(GetSystemClock() / 2) ???
#else
    #error Choose a speed
#endif        

#define GetSystemClock_72MHz()						72000000UL										// (9DP v1.02.00)
#define GetPeripheralClock_72MHz()					36000000UL										// (9DP v1.02.00)
#define GetInstructionClock_72MHz()					(GetSystemClock_72MHz() / 2) ???				// (9DP v1.02.00)

#define GetSystemClock_80MHz()						80000000UL										// (9DP v1.02.00)
#define GetPeripheralClock_80MHz()					40000000UL										// (9DP v1.02.00)
#define GetInstructionClock_80MHz()					(GetSystemClock_80MHz() / 2) ???				// (9DP v1.02.00)

// Clock values
#define MILLISECONDS_PER_TICK       				10                  							// -0.000% error
#define TIMER_PRESCALER             				TIMER_PRESCALER_8   							// At 60MHz
#define TIMER_PERIOD                				37500               							// At 60MHz

// Define the baud rate constants
#define BAUDRATE2       							57600UL
#define BRG_DIV2        							4 
#define BRGH2           							1 

#include <p32xxxx.h>
#include <plib.h>
#include <uart.h>

/** TRIS ***********************************************************/
#define INPUT_PIN           						1
#define OUTPUT_PIN          						0

/*********************************************************************
 *********************************************************************
 * IOS FOR THE DISPLAY CONTROLLER
 ********************************************************************* 
*********************************************************************/

// Definitions for reset line - SSD1906
#define RST_TRIS_BIT_OUT							PORTSetPinsDigitalOut(IOPORT_C, BIT_4)
#define RST_LAT_BIT_SET								mPORTCSetBits(BIT_4)
#define RST_LAT_BIT_CLR								mPORTCClearBits(BIT_4)

// Definitions for RS line - SSD1906
#define RS_TRIS_BIT_OUT								PORTSetPinsDigitalOut(IOPORT_C, BIT_2)
#define RS_LAT_BIT_SET								mPORTCSetBits(BIT_2)
#define RS_LAT_BIT_CLR								mPORTCClearBits(BIT_2)

// Definitions for CS line - SSD1906
#define CS_TRIS_BIT_OUT								PORTSetPinsDigitalOut(IOPORT_D, BIT_1)
#define CS_LAT_BIT_SET								mPORTDSetBits(BIT_1)
#define CS_LAT_BIT_CLR								mPORTDClearBits(BIT_1)

// Definitions for address 16 lines
#define A16_TRIS_BIT_OUT							PORTSetPinsDigitalOut(IOPORT_G, BIT_14)
#define A16_LAT_BIT_SET								mPORTGSetBits(BIT_14)
#define A16_LAT_BIT_CLR								mPORTGClearBits(BIT_14)

// Definitions for address 17-19 line
#define A17_TRIS_BIT_OUT							PORTSetPinsDigitalOut(IOPORT_G, BIT_13)
#define A17_LAT_BIT_SET								mPORTGSetBits(BIT_13)
#define A17_LAT_BIT_CLR								mPORTGClearBits(BIT_13)

/*********************************************************************
 *********************************************************************
 * IOS FOR THE SIDE BUTTONS
 *********************************************************************/

#define BTN_S1 										PORTEbits.RE8
#define BTN_S2 										((i2c2ReadByte(i2cADDR_MAIN_IO_BUTTON, 0x09, 1)) & BTN_FUNCTION)
#define BTN_S3 										((i2c2ReadByte(i2cADDR_MAIN_IO_BUTTON, 0x09, 1)) & BTN_AUDIO)
#define BTN_S4 										((i2c2ReadByte(i2cADDR_MAIN_IO_BUTTON, 0x09, 1)) & BTN_ACK_RESET)

/*********************************************************************
 *********************************************************************
* IOS FOR THE UART
 *********************************************************************
*********************************************************************/

//#define TX_TRIS         							TRISFbits.TRISF5
//#define RX_TRIS         							TRISFbits.TRISF4

/*********************************************************************
 *********************************************************************
 * IO FOR THE BEEPER PWM
 ********************************************************************* 
*********************************************************************/

//#define BEEP_TRIS_BIT         					TRISDbits.TRISD0
//#define BEEP_LAT_BIT         						LATDbits.LATD0

/*********************************************************************
 *********************************************************************
 * IOS FOR THE EEPROM SPI
 * If using GFX PICtail 2 and 250(modified 2) EEPROM is in Explorer 16
 * If using GFX PICtail 3 SPI Flash in GFX 3 is used.
 ********************************************************************* 
*********************************************************************/

// ***** Discrete SPI I/O Definitions **************************
// Serial Data
#define SERIALDATA_TRIS_OUT							PORTSetPinsDigitalOut(IOPORT_A, BIT_5)
#define SERIALDATA_LAT_SET							mPORTASetBits(BIT_5)
#define SERIALDATA_LAT_CLR							mPORTAClearBits(BIT_5)

// Serial Clock
#define SERIALCLK_TRIS_OUT							PORTSetPinsDigitalOut(IOPORT_A, BIT_6)
#define SERIALCLK_LAT_SET							mPORTASetBits(BIT_6)
#define SERIALCLK_LAT_CLR							mPORTAClearBits(BIT_6)

// Chip Select Line - LCD Enable
#define CHIPSELECT_TRIS_OUT							PORTSetPinsDigitalOut(IOPORT_A, BIT_7)
#define CHIPSELECT_LAT_SET							mPORTASetBits(BIT_7)
#define CHIPSELECT_LAT_CLR							mPORTAClearBits(BIT_7)

// LCD /RESET Line
#define LCDRESET_TRIS_OUT							PORTSetPinsDigitalOut(IOPORT_D, BIT_8)
#define LCDRESET_LAT_SET							mPORTDSetBits(BIT_8)
#define LCDRESET_LAT_CLR							mPORTDClearBits(BIT_8)

/*********************************************************************
 *********************************************************************
 * IOS FOR THE GFX ONBOARD FLASH MEMORY
 * This is only used GFX PICtail 2 and 250(modified 2) 
 ********************************************************************* 
*********************************************************************/

//#define SST39_CS_TRIS    							TRISDbits.TRISD11
//#define SST39_CS_LAT     							LATDbits.LATD11
//#define SST39_CS_TRIS    							TRISDbits.TRISD1
//#define SST39_CS_LAT     							LATDbits.LATD1
//#define SST39_A18_TRIS   							TRISCbits.TRISC2
//#define SST39_A18_LAT    							LATCbits.LATC2
//
//#define SST39_A17_TRIS   							TRISGbits.TRISG15
//#define SST39_A17_LAT    							LATGbits.LATG15
//#define SST39_A16_TRIS   							TRISGbits.TRISG14
//#define SST39_A16_LAT    							LATGbits.LATG14	

#endif

