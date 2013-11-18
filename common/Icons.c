//**************************************************************************************************
//**************************************************************************************************
//*																		
//*	Filename:		Icons.c
//*	Date:			09/01/10
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
//*	~~~~~~~~	09/01/10	James Comstock
//*	- Original Prototype
//*                                                                     
//**************************************************************************************************
//*                                                                     
//*	Function Screen Routines
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

#include "instrument.h"
#include "Definitions.h"
#include "System.h"
#include "Icons.h"
#include "Screens.h"

//**************************************************************************************************
//*	Backlight Sensor Trip Point

#define	AUTO_BK_LIGHT_BRIGHT_TRIP					3400	
#define	AUTO_BK_LIGHT_MID_TRIP						1200	
#define	AUTO_BK_LIGHT_DARK_TRIP						250	

//**************************************************************************************************
//*	External - Bitmaps

extern BITMAP_FLASH LMI_Icon;
extern BITMAP_FLASH LB_Down_Arrow;
extern BITMAP_FLASH LB_Up_Arrow;

extern BITMAP_FLASH range_1;
extern BITMAP_FLASH range_2;
extern BITMAP_FLASH range_3;
extern BITMAP_FLASH range_4;
extern BITMAP_FLASH range_5;

extern BITMAP_FLASH battery_charging_smaller;
extern BITMAP_FLASH battery_100_smaller;
extern BITMAP_FLASH battery_80_smaller;
extern BITMAP_FLASH battery_60_smaller;
extern BITMAP_FLASH battery_40_smaller;
extern BITMAP_FLASH battery_20_smaller;
extern BITMAP_FLASH battery_0_smaller;

extern BITMAP_FLASH audio_100_stamp;
extern BITMAP_FLASH audio_80_stamp;
extern BITMAP_FLASH audio_60_stamp;
extern BITMAP_FLASH audio_40_stamp;
extern BITMAP_FLASH audio_20_stamp;
extern BITMAP_FLASH audio_0_stamp;

extern BITMAP_FLASH backlight_on_stamp;
extern BITMAP_FLASH backlight_off_stamp;
extern BITMAP_FLASH backlight_auto_stamp;

extern BITMAP_FLASH thumbdrive_stamp;
extern BITMAP_FLASH keyboard_stamp;
extern BITMAP_FLASH computer_connect_stamp;
extern BITMAP_FLASH computer_disconnect_stamp;
extern BITMAP_FLASH cf_1_stamp;																		// (9DP v1.02.02)
extern BITMAP_FLASH cf_2_stamp;																		// (9DP v1.02.02)
extern BITMAP_FLASH cf_3_stamp;																		// (9DP v1.02.02)

//extern BITMAP_FLASH alpha_cell;
//extern BITMAP_FLASH beta_cell;
//extern BITMAP_FLASH gamma_cell;

//extern BITMAP_FLASH turtle_stamp;
//extern BITMAP_FLASH rabbit_stamp;
//extern BITMAP_FLASH human_stamp;

//**************************************************************************************************
//*	External - Display

extern WORD foreground;
extern WORD background;
extern BYTE USB_Port_Attachment;
extern BYTE Current_Detector;
extern BYTE Current_View;
extern BYTE Range_Number;
extern BYTE Backlight_Mode;
extern BYTE Audio_Operating_Mode;
extern BYTE Correction_Factor_Mode;																	// (9DP v1.02.02)
extern BOOL Light_On;
extern BOOL Light_Temp_On;
extern BOOL System_Error_Flag;
extern BOOL Audio_Mute;
extern BOOL Audio_Enable;

//**************************************************************************************************
//*	External - States

extern COMPUTER_STATE PC_State;
extern APP_STATE App_State_Keyboard;
extern APP_STATE App_State_Thumbdrive;

//**************************************************************************************************
//*	External - System

extern float Battery_Reading;
extern DWORD tick;
extern BOOL Drained_Battery;

//**************************************************************************************************
//*	External - Instrument

extern TYPE_INSTRUMENT lmi;

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Clear_Icon_Window(void) 	
//*
//*	Description:
//* 	Clears the current icon window.
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

void Clear_Icon_Window(void)
{
	SetColor(background);
	Bar(0, ICON_Y, 319, (ICON_Y + ICON_HEIGHT));
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void New_Icon_Window(void) 	
//*
//*	Description:
//* 	Clears the current icon window.
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

void New_Icon_Window(void)
{
	Clear_Icon_Window();
	Update_Icon_Window(NEW_WINDOW);
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Update_Icon_Window(UINT8 update_type) 	
//*
//*	Description:
//* 	Clears the current icon window.
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	update_type:
//* 		CLEAR_WINDOW
//*			RESET_WINDOW
//*			UPDATE_WINDOW
//*			NEW_WINDOW
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//*

void Update_Icon_Window(UINT8 update_type)
{
	static BOOL last_system_error = FALSE;

	if(update_type == CLEAR_WINDOW)
	{
		Clear_Icon_Window();
	 	return;
	}

	if(!System_Error_Flag)
	{
		if(last_system_error)																		// if the last time through there was an error...
		{
			Clear_Icon_Window();																	// ...clear window
			update_type = NEW_WINDOW;																// ...force all icon updates
		}
		Update_Audio_Icon(update_type);																// display the current audio level and icon
		Update_Battery_Icon(update_type);															// display the current battery status
		Update_Backlight_Icon(update_type);															// display the current backlight level and icon
		Update_Range_Icon(update_type);																// display the range icon if not in auto ranging

		// only one can be attached at a time and they share the same icon location
		switch(USB_Port_Attachment)
		{
			case NO_DEVICE_ATTACHED:
			{
				Update_No_Attached_Icon(update_type);
				break;
			}
			case FTDI:
			{
				Update_PC_Icon(update_type);														// display the current computer state
				break;
			}
			case KEYBOARD:
			{
				Update_Keyboard_Icon(update_type);
				break;
			}
			case THUMBDRIVE:
			{
				Update_Thumbdrive_Icon(update_type);
				break;
			}
		}

		Update_Correction_Factor_Icon(update_type);													// display the correction factor icon (9DP v1.02.02)
	}

	last_system_error = System_Error_Flag;
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Update_Audio_Icon(UINT8 update_type)
//*
//*	Description:
//* 	This routine dislays the appropriate icon for the chosen audio level.
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	index - represents the index within the audio control
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

void Update_Audio_Icon(UINT8 update_type)
{
	static BYTE last_audio_mode = 255;
	static BOOL last_audio_enable = TRUE;

//	if(Audio_Enable != last_audio_enable)
//	{
//		if(Audio_Enable == FALSE)
//		{
//			if(ExtImage(285, 205, AUDIO_DISABLE_ICON) == FALSE)											// audio 0% icon
//			{
//				PutImage(285,205,(void*)&audio_0_stamp,IMAGE_NORMAL);
//			}
//		}
//		else
//		{
//			last_audio_mode = 255;
//		}
//	}
//
//	if(Audio_Enable == TRUE)
//	{
//		if((last_audio_mode != Audio_Operating_Mode) || (update_type == NEW_WINDOW))
//		{
//			switch(Audio_Operating_Mode) 
//			{
//				case AUDIO_0:																			// audio 0% - Mute
//				{
//					if(ExtImage(285, 205, AUDIO_0_ICON) == FALSE)										// audio 0% icon
//					{
//						PutImage(285,205,(void*)&audio_0_stamp,IMAGE_NORMAL);
//					}
//					break;
//				}
//				case AUDIO_20:																			// audio 20% 
//				{
//					if(ExtImage(285, 205, AUDIO_20_ICON) == FALSE)										// audio 20% icon
//					{
//						PutImage(285,205,(void*)&audio_20_stamp,IMAGE_NORMAL);
//					}
//					break;
//				}
//				case AUDIO_40:																			// audio 40%
//				{
//					if(ExtImage(285, 205, AUDIO_40_ICON) == FALSE)										// audio 40% icon
//					{
//						PutImage(285,205,(void*)&audio_40_stamp,IMAGE_NORMAL);
//					}
//					break;
//				}
//				case AUDIO_60:																			// audio 60%
//				{
//					if(ExtImage(285, 205, AUDIO_60_ICON) == FALSE)										// audio 60% icon
//					{
//						PutImage(285,205,(void*)&audio_60_stamp,IMAGE_NORMAL);
//					}
//					break;
//				}
//				case AUDIO_80:																			// audio 80%
//				{
//					if(ExtImage(285, 205, AUDIO_80_ICON) == FALSE)										// audio 80% icon
//					{
//						PutImage(285,205,(void*)&audio_80_stamp,IMAGE_NORMAL);
//					}
//					break;
//				}
//				case AUDIO_100:																			// audio 100%
//				{
//					if(ExtImage(285, 205, AUDIO_100_ICON) == FALSE)										// audio 100% icon
//					{
//						PutImage(285,205,(void*)&audio_100_stamp,IMAGE_NORMAL);
//					}
//					break;
//				}
//			}
//		}
//	}
//
//	if(background != WHITE)
//	{
//		Color_Framed_Corners(285,205);																// cover framed bmp edges with background color
//	}
//
//	last_audio_mode = Audio_Operating_Mode;
//	last_audio_enable = Audio_Enable;

	if((last_audio_mode != Audio_Operating_Mode) || (update_type == NEW_WINDOW))
	{
		switch(Audio_Operating_Mode) 
		{
			case AUDIO_0:																			// audio 0% - Mute
			{
				if(ExtImage(285, 205, AUDIO_0_ICON) == FALSE)										// audio 0% icon
				{
					PutImage(285,205,(void*)&audio_0_stamp,IMAGE_NORMAL);
				}
				break;
			}
			case AUDIO_20:																			// audio 20% 
			{
				if(ExtImage(285, 205, AUDIO_20_ICON) == FALSE)										// audio 20% icon
				{
					PutImage(285,205,(void*)&audio_20_stamp,IMAGE_NORMAL);
				}
				break;
			}
			case AUDIO_40:																			// audio 40%
			{
				if(ExtImage(285, 205, AUDIO_40_ICON) == FALSE)										// audio 40% icon
				{
					PutImage(285,205,(void*)&audio_40_stamp,IMAGE_NORMAL);
				}
				break;
			}
			case AUDIO_60:																			// audio 60%
			{
				if(ExtImage(285, 205, AUDIO_60_ICON) == FALSE)										// audio 60% icon
				{
					PutImage(285,205,(void*)&audio_60_stamp,IMAGE_NORMAL);
				}
				break;
			}
			case AUDIO_80:																			// audio 80%
			{
				if(ExtImage(285, 205, AUDIO_80_ICON) == FALSE)										// audio 80% icon
				{
					PutImage(285,205,(void*)&audio_80_stamp,IMAGE_NORMAL);
				}
				break;
			}
			case AUDIO_100:																			// audio 100%
			{
				if(ExtImage(285, 205, AUDIO_100_ICON) == FALSE)										// audio 100% icon
				{
					PutImage(285,205,(void*)&audio_100_stamp,IMAGE_NORMAL);
				}
				break;
			}
		}
	}

	if(background != WHITE)
	{
		Color_Framed_Corners(285,205);																// cover framed bmp edges with background color
	}

	last_audio_mode = Audio_Operating_Mode;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Update_Backlight_Icon(UINT8 update_type)
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

void Update_Backlight_Icon(UINT8 update_type)
{
	static BOOL Last_Temp_BKLT_Setting = FALSE;
	static char Last_BKLT_Setting = 5;
	static DWORD prevTick1 = 0;																		// gets the tick value upon entering a function
	char BKLT_Setting;
	WORD ADC_Light_Level;
	
	if(Backlight_Mode == BK_LIGHT_AUTO)
	{	
//		if((tick - prevTick1) > 10)																	// ---was 3000 slow down switching intervals
		if((tick - prevTick1) > ((DWORD) TICKS_PER_SEC * 2))										// ---was 3000 slow down switching intervals (9DP v1.01.06)
		{
			ADC_Light_Level = GetADCValue((BYTE) ADC_PORT7_LIGHT);
	
			if(ADC_Light_Level >= AUTO_BK_LIGHT_BRIGHT_TRIP)										// was ADC >= 3200
			{										
				Set_Backlight_Level(lmi.controls.backlight.info.auto_low);							// low level
			}
			else if((ADC_Light_Level >= AUTO_BK_LIGHT_MID_TRIP)	&
				(ADC_Light_Level < (AUTO_BK_LIGHT_BRIGHT_TRIP - 700)))								// ADC >= 1700  &  ADC < 2700
			{
				Set_Backlight_Level(lmi.controls.backlight.info.auto_medium);						// mid level
			}
			else if(ADC_Light_Level <= AUTO_BK_LIGHT_DARK_TRIP)										// was ADC < 1000
			{
				Set_Backlight_Level(lmi.controls.backlight.info.auto_high);							// high level
			}
			prevTick1 = tick;																		// get previous tick value
		}
	}

	if((update_type == NEW_WINDOW) || (Last_BKLT_Setting != Backlight_Mode) || (Last_Temp_BKLT_Setting != (Light_Temp_On || Light_On)))
	{		
		switch(Backlight_Mode)
		{
			case BK_LIGHT_OFF:																		// Backlight OFF
			{
				Set_Backlight_Level(0);																// Turn Backlight OFF
				if(ExtImage(250, 205, BACKLIGHT_OFF_ICON) == FALSE)									// backlight off icon
				{
					PutImage(250,205,(void*)&backlight_off_stamp,IMAGE_NORMAL);
				}
				break;
			}
			case BK_LIGHT_ON:																		// Backlight ON - Fixed
			{
				Set_Backlight_Level(lmi.controls.backlight.info.on_level);							// Turn Backlight ON to defined setpoint
				if(ExtImage(250, 205, BACKLIGHT_ON_ICON) == FALSE)									// backlight on icon
				{
					PutImage(250,205,(void*)&backlight_on_stamp,IMAGE_NORMAL);
				}
				break;
			}
			case BK_LIGHT_AUTO: 																	// Backlight Auto Change
			{
				if(ExtImage(250, 205, BACKLIGHT_AUTO_ICON) == FALSE)								// backlight auto icon
				{
					PutImage(250,205,(void*)&backlight_auto_stamp,IMAGE_NORMAL);
				}
				break;
			}
			case BK_LIGHT_TEMP_ON:
			{
				if((Light_Temp_On) || (Light_On))
				{
					if(ExtImage(250, 205, BACKLIGHT_ON_ICON) == FALSE)								// backlight on icon
					{
						PutImage(250,205,(void*)&backlight_on_stamp,IMAGE_NORMAL);
					}
					Set_Backlight_Level(lmi.controls.backlight.info.on_level);
				}
				else	
				{
					if(ExtImage(250, 205, BACKLIGHT_OFF_ICON) == FALSE)								// backlight off icon
					{
						PutImage(250,205,(void*)&backlight_off_stamp,IMAGE_NORMAL);
					}
					Set_Backlight_Level(0);
				}
				break;
			}
		}
		Last_BKLT_Setting = Backlight_Mode;
		Last_Temp_BKLT_Setting = Light_Temp_On || Light_On;

		if(background != WHITE)
		{
			Color_Framed_Corners(250,205);															//215, 205  -- cover framed bmp edges with background color
		}
	}
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Color_Framed_Corners(SHORT Xaxis, SHORT Yaxis)
//*
//*	Description:
//* 	Function fills in the framed bitmap icon corners with the current background color.
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
//* 	called by Update_Audio_Icon() and Update_Backlight_Icon()
//*

void Color_Framed_Corners(SHORT Xaxis, SHORT Yaxis)
{
	UINT8 bmp_fill;
	SHORT bmp_tempX, bmp_tempY;

	SHORT bmp_Top_Y[17] 	=  {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 3, 3, 4, 5};
	SHORT bmp_Btm_Y[17] 	= {33,33,33,33,33,33,32,32,32,32,31,31,31,30,30,29,28};
	SHORT bmp_Left_X[17] 	=  {0, 1, 2, 3, 4, 5, 3, 2, 1, 0, 0, 1, 2, 1, 0, 0, 0};
	SHORT bmp_Right_X[17] 	= {28,29,30,31,32,33,33,32,31,30,31,32,33,33,32,33,33};

	SetColor(background);																			// for blending the edge of the bitmaps with the background color

	for(bmp_fill=0;bmp_fill<17;bmp_fill++)															// fill top left corner with background color
	{
		bmp_tempX = (Xaxis + bmp_Left_X[bmp_fill]);
		bmp_tempY = (Yaxis + bmp_Top_Y[bmp_fill]);
		PutPixel(bmp_tempX, bmp_tempY);
	}

	for(bmp_fill=0;bmp_fill<17;bmp_fill++)															// fill top right corner with background color
	{
		bmp_tempX = (Xaxis + bmp_Right_X[bmp_fill]);
		bmp_tempY = (Yaxis + bmp_Top_Y[bmp_fill]);
		PutPixel(bmp_tempX, bmp_tempY);
	}

	for(bmp_fill=0;bmp_fill<17;bmp_fill++)															// fill bottom right corner with background color
	{
		bmp_tempX = (Xaxis + bmp_Right_X[bmp_fill]);
		bmp_tempY = (Yaxis + bmp_Btm_Y[bmp_fill]);
		PutPixel(bmp_tempX, bmp_tempY);
	}

	for(bmp_fill=0;bmp_fill<17;bmp_fill++)															// fill bottom left corner with background color
	{
		bmp_tempX = (Xaxis + bmp_Left_X[bmp_fill]);
		bmp_tempY = (Yaxis + bmp_Btm_Y[bmp_fill]);
		PutPixel(bmp_tempX, bmp_tempY);
	}
	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Update_Battery_Icon(UINT8 update_type)
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
//* 	battery voltage divider is 1.5Meg ohm in series with 309k ohm (divider constant 0.1708126)
//*

void Update_Battery_Icon(UINT8 update_type)
{
	WORD ADC_Battery_level;
	static unsigned char Bat_Level = 5;	
	static unsigned char last_bat_level = 255;
	static DWORD prevTick2 = 0;																							// gets the tick value upon entering a function
	DWORD tick_count;																									// (9DP v1.01.06)

	tick_count = tick - prevTick2;																						// (9DP v1.01.06)

#ifdef THIS_IS_MODEL_276D
	if((PORTBbits.RB8 == 1) & (PORTFbits.RF13 == 0))																	// save here for 276D
#else
	if(PORTFbits.RF13 == 0)																								// if battery is currently charging...
#endif
	{
		Bat_Level = 6;																									// battery charging
		if(tick_count > ((DWORD)TICKS_PER_SEC * 3))																		// was 1.25 sec (9DP v1.01.06)
		{
			ADC_Battery_level = GetADCValue((BYTE) ADC_PORT3_BATTERY);													// get Battery ADC Value
			Battery_Reading = (float) ADC_Battery_level * 0.003574;														// ADC value times conversion constant
			Battery_Reading = Battery_Reading * ( (float) lmi.calibration.constants.info.battery / 1000.0);
			prevTick2 = tick;																							// get previous tick value
		}
	}
	else
	{
		if(tick_count > ((DWORD)TICKS_PER_SEC * 3))																		// was 0.75 sec (9DP v1.01.06)
		{
			ADC_Battery_level = GetADCValue((BYTE) ADC_PORT3_BATTERY);													// get Battery ADC Value
			Battery_Reading = (float) ADC_Battery_level * 0.003574;														// ADC value times conversion constant
			Battery_Reading = Battery_Reading * ( (float) lmi.calibration.constants.info.battery / 1000.0);

			if(Battery_Reading >= 10.58)																				//
			{															
				Bat_Level = 5; 																							// battery - 100%
			}
			else if(Battery_Reading >= 9.97)																			//
			{	
				Bat_Level = 4;																							// battery -  80%
			}		
			else if(Battery_Reading >= 9.35)																			//
			{	
				Bat_Level = 3;																							// battery -  60%
			}	
			else if(Battery_Reading >= 8.73)																			//
			{	
				Bat_Level = 2;																							// battery -  40%
			}	
			else if(Battery_Reading >= 8.12)																			//
			{	
				Bat_Level = 1;																							// battery -  20%
			}
			else if(Battery_Reading >= 7.50)																			//
			{	
				Bat_Level = 0;																							// battery -   0%
			}
			else																										// shutdown instrument to prevent malfunctions
			{								
				Bat_Level = 0;
//				Drained_Battery = TRUE;																					// Battery too low for continued operation, shutting down
//				Shutdown_Power();																						// Call function to shutdown instrument
//				Check_Battery();																						// (9DP v1.01.23)
			}
			prevTick2 = tick;																							// get previous tick value
		}	
	}

	if(((Bat_Level != last_bat_level)) || (update_type == NEW_WINDOW))
	{
		switch(Bat_Level)
		{
			case 0:																										// Battery 0% - Entirely Depleted
			{
				if(ExtImage(2, 213, BATTERY_0_ICON) == FALSE)
				{
					PutImage(2,213,(void*)&battery_0_smaller,IMAGE_NORMAL);
				}
				break;
			}
			case 1:																										// Battery 20%
			{
				if(ExtImage(2, 213, BATTERY_20_ICON) == FALSE)
				{
					PutImage(2,213,(void*)&battery_20_smaller,IMAGE_NORMAL);
				}
				break;
			}
			case 2: 																									// Battery 40%
			{
				if(ExtImage(2, 213, BATTERY_40_ICON) == FALSE)
				{
					PutImage(2,213,(void*)&battery_40_smaller,IMAGE_NORMAL);
				}
				break;
			}
			case 3: 																									// Battery 60%
			{
				if(ExtImage(2, 213, BATTERY_60_ICON) == FALSE)
				{
					PutImage(2,213,(void*)&battery_60_smaller,IMAGE_NORMAL);
				}
				break;
			}
			case 4: 																									// Battery 80%
			{
				if(ExtImage(2, 213, BATTERY_80_ICON) == FALSE)
				{
					PutImage(2,213,(void*)&battery_80_smaller,IMAGE_NORMAL);
				}
				break;
			}
			case 5: 																									// Battery 100%
			{
				if(ExtImage(2, 213, BATTERY_100_ICON) == FALSE)
				{
					PutImage(2,213,(void*)&battery_100_smaller,IMAGE_NORMAL);
				}
				break;	
			}
			case 6: 																									// Battery Charging
			{
				if(ExtImage(2, 213, BATTERY_CHARGE_ICON) == FALSE)
				{
					PutImage(2,213,(void*)&battery_charging_smaller,IMAGE_NORMAL);
				}
				break;
			}
		}

		last_bat_level = Bat_Level;																						// assign last battery level to image currently displayed

		if(background != WHITE)
		{
			Color_Battery_Edges(2,213);
		}
	}

	if((lmi.alarms.low_battery.info.feature.enable == TRUE) && (Battery_Reading < lmi.alarms.low_battery.info.feature.value))
		add_error_message_to_queue(ALARM_LOW_BATTERY);
	else if((lmi.alerts.low_battery.info.feature.enable == TRUE) && (Battery_Reading < lmi.alerts.low_battery.info.feature.value))
		add_error_message_to_queue(ALERT_LOW_BATTERY);

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Color_Battery_Edges(SHORT Xaxis, SHORT Yaxis)
//*
//*	Description:
//*		Function fills in the battery bitmap icon corners with the current background color.
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
//* 	called by Update_Battery_Icon()
//*

void Color_Battery_Edges(SHORT Xaxis, SHORT Yaxis)
{
	UINT8 bat_top, bat_btm;
	SHORT bat_tempX, bat_tempY;
	SHORT TopSection_X[17] = {0,64,65,65,65,65,65,66,66,66,66,66,67,67,67,67,67};
	SHORT TopSection_Y[17] = {0, 0, 0, 1, 2, 3, 4, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4};

	SHORT BtmSection_X[17] =  {0,64,65,65,65,65,65,66,66,66,66,66,67,67,67,67,67};
	SHORT BtmSection_Y[17] = {19,19,15,16,17,18,19,19,18,17,16,15,15,16,17,18,19};

	SetColor(background);																			// for blending the edge of the bitmaps with the background color
	
	for(bat_top=0;bat_top<17;bat_top++)
	{
		bat_tempX = (Xaxis + TopSection_X[bat_top]);
		bat_tempY = (Yaxis + TopSection_Y[bat_top]);
		PutPixel(bat_tempX, bat_tempY);
	}
	
	for(bat_btm=0;bat_btm<17;bat_btm++)
	{
		bat_tempX = (Xaxis + BtmSection_X[bat_btm]);
		bat_tempY = (Yaxis + BtmSection_Y[bat_btm]);
		PutPixel(bat_tempX, bat_tempY);
	}

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Update_No_Attached_Icon(UINT8 update_type)
//*
//*	Description:
//* 	This routine dislays the appropriate icon for the current communication port status.
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//		None
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

void Update_No_Attached_Icon(UINT8 update_type)
{
	static BYTE last_port_state = 255;

	if((last_port_state != USB_Port_Attachment) || (update_type == NEW_WINDOW))
	{
		SetColor(background);
		Bar(215, 205, 249, 239);																	// blank the icon
	}

	last_port_state = USB_Port_Attachment;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Update_PC_Icon(UINT8 update_type)
//*
//*	Description:
//* 	This routine dislays the appropriate icon for the current communication port status.
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//		None
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

void Update_PC_Icon(UINT8 update_type)
{
	static BYTE last_pc_state = 255;

	if((last_pc_state != PC_State) || (update_type == NEW_WINDOW))
	{
		SetColor(background);
		switch(PC_State) 
		{
			case PC_NOT_CONNECTED:
			{
				Bar(215, 205, 249, 239);															// blank the icon
				break;
			}
			case PC_DISCONNECTED:
			{
				if(ExtImage(215, 205, COMPUTER_DISCONNECT_ICON) == FALSE)							// computer disconnect icon
				{
					PutImage(215,205,(void*)&computer_disconnect_stamp,IMAGE_NORMAL);
				}
				break;
			}
			case PC_CONNECTED:
			{
				if(ExtImage(215, 205, COMPUTER_CONNECT_ICON) == FALSE)								// computer connect icon
				{
					PutImage(215,205,(void*)&computer_connect_stamp,IMAGE_NORMAL);
				}
				break;
			}
			case PC_NO_CHANGE:
			{
				break;
			}
		}
		if(background != WHITE)
		{
			Color_Framed_Corners(215,205);															// cover framed bmp edges with background color
		}
	}

	last_pc_state = PC_State;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Update_Thumbdrive_Icon(UINT8 update_type)
//*
//*	Description:
//* 	This routine indicates the thumbdrive is attached.
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//*		None
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

void Update_Thumbdrive_Icon(UINT8 update_type)
{
	static BYTE last_thumbdrive_state = 255;

	if((last_thumbdrive_state != App_State_Thumbdrive) || (update_type == NEW_WINDOW))
	{
		SetColor(background);
		switch(App_State_Thumbdrive) 
		{
			case DEVICE_NOT_CONNECTED:
			{
				Bar(215, 205, 249, 239);															// blank the icon
				break;
			}
			case DEVICE_CONNECTED:
			{
				if(ExtImage(215, 205, THUMBDRIVE_ICON) == FALSE)									// thumbdrive icon
				{
					PutImage(215,205,(void*)&thumbdrive_stamp,IMAGE_NORMAL);
				}
				break;
			}
		}

		if(background != WHITE)
		{
			Color_Framed_Corners(215,205);															// cover framed bmp edges with background color
		}
	}

	last_thumbdrive_state = App_State_Thumbdrive;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Update_Audio_Icon(UINT8 update_type)
//*
//*	Description:
//* 	This routine dislays the appropriate icon for the chosen audio level.
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	index - represents the index within the audio control
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

#define keyboard_detached	0
#define keyboard_attached	1
#define keyboard_unknown	2

void Update_Keyboard_Icon(UINT8 update_type)
{
	static UINT8 last_keyboard_state = (UINT8) keyboard_unknown;
	UINT8 keyboard_state;

	if(App_State_Keyboard == DEVICE_NOT_CONNECTED)
		keyboard_state = (UINT8) keyboard_detached;
	else
		keyboard_state = (UINT8) keyboard_attached;

	if((last_keyboard_state != keyboard_state) || (update_type == NEW_WINDOW))
	{
		SetColor(background);
		switch(keyboard_state) 
		{
			case keyboard_detached:
			{
				Bar(215, 205, 249, 239);															// blank the icon
				break;
			}
			case keyboard_attached:
			{
				if(ExtImage(215, 205, KEYBOARD_ICON) == FALSE)										// keyboard icon
				{
					PutImage(215,205,(void*)&keyboard_stamp,IMAGE_NORMAL);
				}
				break;
			}
		}
		if(background != WHITE)
		{
			Color_Framed_Corners(215,205);															// cover framed bmp edges with background color
		}
	}

	last_keyboard_state = keyboard_state;

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Update_Range_Icon(UINT8 update_type)
//*
//*	Description:
//* 	This routine dislays the appropriate icon if a fixed range is selected.
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	index - represents the index within the audio control
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None
//*

void Update_Range_Icon(UINT8 update_type)
{
	static BYTE last_range = 255;

	if((last_range != lmi.calibration.controls.info.fixed_range) || (update_type == NEW_WINDOW))
	{
		switch(lmi.calibration.controls.info.fixed_range) 
		{
			case 1:
			{
				if(ExtImage(180, 205, RANGE_1_ICON) == FALSE)										// range 1 icon
				{
					PutImage(180,205,(void*)&range_1,IMAGE_NORMAL);
				}
				break;
			}
			case 2:
			{
				if(ExtImage(180, 205, RANGE_2_ICON) == FALSE)										// range 2 icon
				{
					PutImage(180,205,(void*)&range_2,IMAGE_NORMAL);
				}
				break;
			}
			case 3:
			{
				if(ExtImage(180, 205, RANGE_3_ICON) == FALSE)										// range 3 icon
				{
					PutImage(180,205,(void*)&range_3,IMAGE_NORMAL);
				}
				break;
			}
			case 4:
			{
				if(ExtImage(180, 205, RANGE_4_ICON) == FALSE)										// range 4 icon
				{
					PutImage(180,205,(void*)&range_4,IMAGE_NORMAL);
				}
				break;
			}
			case 5:
			{
				if(ExtImage(180, 205, RANGE_5_ICON) == FALSE)										// range 5 icon
				{
					PutImage(180,205,(void*)&range_5,IMAGE_NORMAL);
				}
				break;
			}
			default:
			{
				SetColor(background);
				Bar(180, 205, 214, 239);															// blank the icon
				break;
			}
		}

		if(background != WHITE)
		{
			Color_Framed_Corners(180,205);															// cover framed bmp edges with background color
		}

		last_range = lmi.calibration.controls.info.fixed_range;
	}

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//*	Function:
//* 	void Update_Correction_Factor_Icon(UINT8 update_type)
//*
//*	Description:
//* 	This routine dislays the appropriate icon if a correction factor is used.
//*
//*	Precondition:
//* 	None
//*
//*	Parameters:
//* 	index - represents the index within the correction factor group
//*
//*	Return Values:
//* 	None
//*
//*	Remarks:
//* 	None (9DP v1.02.02)
//*

void Update_Correction_Factor_Icon(UINT8 update_type)
{
	static BYTE last_correction_factor = 255;

	if((last_correction_factor != Correction_Factor_Mode) || (update_type == NEW_WINDOW))
	{
		switch(Correction_Factor_Mode) 
		{
			case 1:
			{
				if(ExtImage(145, 205, CF_1_ICON) == FALSE)
				{
					PutImage(145, 205, (void*)&cf_1_stamp, IMAGE_NORMAL);
				}
				break;
			}
			case 2:
			{
				if(ExtImage(145, 205, CF_2_ICON) == FALSE)
				{
					PutImage(145, 205, (void*)&cf_2_stamp, IMAGE_NORMAL);
				}
				break;
			}
			case 3:
			{
				if(ExtImage(145, 205, CF_3_ICON) == FALSE)
				{
					PutImage(145, 205, (void*)&cf_3_stamp, IMAGE_NORMAL);
				}
				break;
			}
			default:
			{
				SetColor(background);
				Bar(145, 205, 179, 239);
				break;
			}
		}

		if(background != WHITE)
		{
			Color_Framed_Corners(145,205);
		}

		last_correction_factor = Correction_Factor_Mode;
	}

	return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//* LUDLUM MEASUREMENTS INC. COPYRIGHT (C) 2010 
//**************************************************************************************************
//**************************************************************************************************

