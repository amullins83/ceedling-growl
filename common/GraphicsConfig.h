/*********************************************************************
 * Module for Microchip Graphics Library
 * This file contains compile time options for the Graphics Library. 
 *********************************************************************
 * FileName:        none
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC24/PIC30/PIC32
 * Compiler:        C30 V3.00/C32
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright © 2007 Microchip Technology Inc.  All rights reserved.
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
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Anton Alkhimenok		10/28/2007
 ********************************************************************/

#ifndef _GRAPHICSCONFIG_H
#define _GRAPHICSCONFIG_H

/*********************************************************************
* Overview: Hardware profile.
*
*********************************************************************/
//#include "HardwareProfile.h"

/*********************************************************************
* Overview:
*   Graphics PICtail Plus Board Version setting. The value assigned 
*	to this macro determines the version of the PICtail Plus Board. 
*	- 1 - Uses Graphics PICtail Plus Board Version 1    
*	- 2 - Uses Graphics PICtail Plus Board Version 2    
*                                        
********************************************************************/
#define GRAPHICS_PICTAIL_VERSION   2

#if (GRAPHICS_PICTAIL_VERSION == 2)
	#define DISPLAY_CONTROLLER			SSD1906

	/*********************************************************************
	* Overview: Defines color depth. The 1,2,4,8,16 bit per pixel 
	*			color depths are valid.
	*
	********************************************************************/
	#define COLOR_DEPTH		16
			
	/*********************************************************************
	* Overview: Horizontal and vertical display resolution
	*                  (from the glass datasheet).
	*********************************************************************/
	#define DISP_HOR_RESOLUTION				240
	#define DISP_VER_RESOLUTION				320

	/*********************************************************************
	* Overview: Image orientation (can be 0, 90, 180, 270 degrees).
	*********************************************************************/
	#define DISP_ORIENTATION				270

	/*********************************************************************
	* Overview: Panel Data Width (can be 18 or 24 bits).
	*********************************************************************/
	#define DISP_DATA_WIDTH     			18
	
	/*********************************************************************
	* Overview: LSHIFT Polarity Swap
	* If defined LSHIFT is a falling trigger
	*********************************************************************/
	//#define DISP_INV_LSHIFT
	
	/*********************************************************************
	* Overview: Horizontal synchronization timing in pixels
	*                  (from the glass datasheet).
	*********************************************************************/
	#define DISP_HOR_PULSE_WIDTH		    8
	#define DISP_HOR_BACK_PORCH				0							// was 4
	#define DISP_HOR_FRONT_PORCH			16							// was 4
	
	/*********************************************************************
	* Overview: Vertical synchronization timing in lines
	*                  (from the glass datasheet).
	*********************************************************************/
	#define DISP_VER_PULSE_WIDTH		    2
	#define DISP_VER_BACK_PORCH				1
	#define DISP_VER_FRONT_PORCH			1
#endif

/*********************************************************************
* Overview: Blocking and Non-Blocking configuration selection. To
*			enable non-blocking configuration USE_NONBLOCKING_CONFIG
*			must be defined. If this is not defined, blocking 
*			configuration is assumed.
*
********************************************************************/
//#define USE_NONBLOCKING_CONFIG 									// Comment this line to use blocking configuration

/*********************************************************************
* Overview: Keyboard control on some objects can be used by enabling
*			the GOL Focus (USE_FOCUS)support. 
*
*********************************************************************/
#define USE_FOCUS

/*********************************************************************
* Overview: Input devices used defines the messages that Objects will
*			process. The following definitions indicate the usage of 
*			the different input device:
*			- USE_TOUCHSCREEN - enables the touch screen support.
*			- USE_KEYBOARD - enables the key board support.
*
*********************************************************************/

#define USE_KEYBOARD												// Enable key board support.
//#define USE_TOUCHSCREEN											// Enable touch screen support.
//#define USE_MOUSE													// Enable mouse support. (9DP v1.01.05)

/*********************************************************************
* Overview: To save program memory, unused Widgets or Objects can be 
*			removed at compile time. 
*
*********************************************************************/
#define USE_GOL                 									// Enable Graphics Object Layer.
#define USE_BUTTON													// Enable Button Object.
//#define USE_PICTURE												// Enable Picture Object.
//#define USE_ROUNDDIAL												// Enable Dial Object.
//#define USE_METER													// Enable Meter Object.
//#define USE_CUSTOM												// Enable Custom Control Object.

//#define USE_GRID													// Enable Grid Object. (9DP v1.01.05)
//#define USE_WINDOW												// Enable Window Object. (9DP v1.01.05)
//#define USE_CHECKBOX												// Enable Checkbox Object. (9DP v1.01.05)
//#define USE_RADIOBUTTON											// Enable Radio Button Object. (9DP v1.01.05)
//#define USE_EDITBOX												// Enable Edit Box Object. (9DP v1.01.05)
//#define USE_LISTBOX												// Enable List Box Object. (9DP v1.01.05)
//#define USE_SLIDER 												// Enable Slider or Scroll Bar Object. (9DP v1.01.05)
//#define USE_PROGRESSBAR											// Enable Progress Bar Object. (9DP v1.01.05)
//#define USE_GROUPBOX												// Enable Group Box Object. (9DP v1.01.05)
//#define USE_STATICTEXT											// Enable Static Text Object. (9DP v1.01.05)

/*********************************************************************
* Overview: To enable support for unicode fonts, USE_MULTIBYTECHAR  
*			must be defined. This changes XCHAR definition. See XCHAR 
*			for details.
*
*********************************************************************/
#define USE_MULTIBYTECHAR

/*********************************************************************
* Overview: Font data can be placed in two locations. One is in 
*			FLASH memory and the other is from external memory.
*			Definining one or both enables the support for fonts located
*			in internal flash and external memory. 
*	- USE_FONT_FLASH - Font in internal flash memory support.	
*	- USE_FONT_EXTERNAL - Font in external memory support.	
*
*********************************************************************/
#define USE_FONT_FLASH 												// Support for fonts located in internal flash
//#define USE_FONT_EXTERNAL											// Support for fonts located in external memory

/*********************************************************************
* Overview: Similar to Font data bitmaps can also be placed in 
*			two locations. One is in FLASH memory and the other is 
*			from external memory.
*			Definining one or both enables the support for bitmaps located
*			in internal flash and external memory. 
*	- USE_BITMAP_FLASH - Font in internal flash memory support.	
*	- USE_BITMAP_EXTERNAL - Font in external memory support.	
*
*********************************************************************/
#define USE_BITMAP_FLASH											// Support for bitmaps located in internal flash
//#define USE_BITMAP_EXTERNAL										// Support for bitmaps located in external memory

#define USE_USB_PICTAIL												// Option to enable the switching of LCD controller signals
																	// when USB PICtail is used with the graphics PICtail.
/*********************************************************************
* Overview: LCD driver.
*
********************************************************************/

//#include "Graphics\LGDP4531_R61505_S6D0129_S6D0139_SPFD5408.h"

#include "SSD1906.h"   												// Driver for Solomon Systech. SSD1906 controller 
//#include "Graphics\CustomDisplayDriver.h"
//#include "Graphics\SSD1906_forRTC.h"   							// Driver for Solomon Systech. SSD1906 controller
//#include "Graphics\SH1101A_SSD1303.h"								// Driver for the HS1101A or SSD1303 (monochrome OLED display) controller
//#include "Graphics\HIT1270.h"   									// Landscape driver for HIT1270 controller
//#include "Graphics\SSD1339.h"   									// Driver for Solomon Systech. SSD1339 controller
//#include "Graphics\ST7529.h"   									// Driver for Sitronix ST7529 controller

#endif
