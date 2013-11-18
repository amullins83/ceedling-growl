#ifndef __GLOBALS_H
#define __GLOBALS_H

#include <plib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Definitions.h"
#include "System.h"
#include "usb_config.h"
#include "usb.h"
#include "usb_host_hid_parser.h"
#include "usb_host_hid.h"
#include "usb_host_local.h"
#include "Graphics.h"
#include "SSD1906.h"
#include "p32mx460f512l.h"
#include "LCD-SST39VF1601v001.h"
#include "I2C.h"
#include "i2cConfig.h"
#include "UART.h"
#include "Message_Box.h"
#include "Elements.h"
#include "Functions.h"
#include "Conversions.h"
#include "GenericTypeDefs.h"
#include "Screens.h"
#include "Icons.h"
#include "ftdi.h"
#include "Commands.h"
#include <time.h>
#include "usb_host_generic.h"
#include "Keyboard_Menu.h"

//**************************************************************************************************
//**************************************************************************************************
//  Global Variables
//**************************************************************************************************
//**************************************************************************************************
//*

OBJ_HEADER    *pArcScreen;                                                                          // pointer to ARC Screen
OBJ_HEADER    *pNavList;                                                                            // pointer to navigation list
OBJ_HEADER    *pSettingsScrn;                                                                       // pointer to the settings screen
OBJ_HEADER    *pControlsScrn;                                                                       // pointer to the controls screen
OBJ_HEADER    *pDetectorScrn;                                                                       // pointer to the detector screen
OBJ_HEADER    *pAlertScrn;                                                                          // pointer to the alert screen
OBJ_HEADER    *pAlarmScrn;                                                                          // pointer to the alarm screen
OBJ_HEADER    *pFunctionScrn;                                                                       // pointer to the function screen
OBJ_HEADER    *pColorsScrn;                                                                         // pointer to the color screen
OBJ_HEADER    *PreviousScrn;                                                                        // pointer to the previous screen pointer

//*
//**************************************************************************************************
//**************************************************************************************************
//* End Global Variables
//**************************************************************************************************
//**************************************************************************************************
#endif
