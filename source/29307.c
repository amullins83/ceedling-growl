//**************************************************************************************************
//**************************************************************************************************
//*                                                                     
//* Filename:       29307.c                                         
//* Date:           05/19/10
//*                                                                     
//* Authors:        Clint Appling, James Comstock, Austin Mullins
//* Company:        Ludlum Measurements                                 
//* Address:        501 Oak Street                                      
//*                 Sweetwater, TX 79556                                
//*                                                                     
//**************************************************************************************************
//**************************************************************************************************
//*                                                                     
//*                                                                     
//**************************************************************************************************
//*                                                                     
//* Microcontroller:    PIC32MX460F512L-80I/PT
//* Oscillator:         8Mhz, external
//* Clock:              60MHz, internal (nS instruction cycle)
//* Code Memory:        512K bytes (256K word instructions), flash
//* RAM:                32K bytes
//* Internal EEPROM:    none
//* External EEPROM:    64K bytes, U7
//*                     64K bytes, U8
//* External Flash:     1M words (16Mbit x16)
//*
//**************************************************************************************************
//*
//* Disclosure:
//*
//* Some of the routines contained within this firmware may have been developed by Microchip and are
//* provided to developers for use in Microchip devices. The Microchip software license agreement is
//* listed below.
//*
//**************************************************************************************************
//*
//* Software License Agreement
//*
//* The software supplied herewith by Microchip Technology Incorporated (the "Company") for its
//* PICmicro® Microcontroller is intended and supplied to you, the Company’s customer, for use
//* solely and exclusively on Microchip PICmicro Microcontroller products.
//*
//* The software is owned by the Company and/or its supplier, and is protected under applicable
//* copyright laws. All rights are reserved. Any use in violation of the foregoing restrictions may
//* subject the user to criminal sanctions under applicable laws, as well as to civil liability for
//* the breach of the terms and conditions of this license.
//*
//* THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR
//* STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//* A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT, IN ANY CIRCUMSTANCES, BE
//* LIABLE FOR SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
//*
//**************************************************************************************************

#include "instrument.h"

//#define DEBUG_MODE

#include "globals_29307.h"


//**************************************************************************************************
//**************************************************************************************************
// MAIN ROUTINE
//**************************************************************************************************
//**************************************************************************************************
//* Function:
//*     int main(void)
//*
//* Description:
//*     None
//*
//* Precondition:
//*     None
//*
//* Parameters:
//*     None
//*
//* Return Values:
//*     None
//*
//* Remarks:
//*     None
//*

int main(void)
{
    TopLevelInitialize();

    while(1) {
        MainLoop();
    }

    return 0;
}

void TopLevelInitialize(void) {
    SystemInit();                                                                                   // initialize system (processor specific)
    I2CInit();                                                                                      // initialize I2C devices
    AudioInit();

    TimersInit();                                                                                   // initialize timers
    UARTInit();                                                                                     // initialize UART

    USBInitialize(0);                                                                               // initialize USB layers
    MemoryInit();                                                                                   // initialize external memory
    DisplayInit();                                                                                  // initialize display
    EEPROM_Initialize(FALSE);                                                                       // initialize EEPROMs
    HVBInit();                                                                                      // initialize electrometer
    SchemeInit();                                                                                   // initialize schemes
    VariableInit();                                                                                 // initialize variables
    RTCCInit();                                                                                     // initialize internal RTC
    CheckCalibrationDueDate();                                                                      // check calibration due date

    if(RCONbits.SWR == TRUE)
        RCONbits.SWR = FALSE;

    ic = newConstants(atoi((const char *)lmi.settings.product.info.serial_number));
}

void MainLoop(void) {
    Check_for_Restart();
    Check_Audio_Status();
    Check_USB_Port();
//  GOLDrawCallback();                                                                          // added in place of GOLDraw (v1.01.05) [moved above Check_Messages() (v1.01.08)]
    GOLDraw();                                                                                    // Changed back to GOLDraw, because circumventing it makes no sense.
    Check_Messages();

    // These functions need to be interruptable, so they are here instead of inside an ISR
    One_Second_Updates();
    Check_for_Temperature_Reading();
    // End interruptable ISR's

    // In contrast, UpdateReading will not be interrupted
    UpdateReading();
    Adjust_Audio();
    Adjust_Peak_Value();
}

void One_Second_Updates(void) {
    if(Update_One_Second == TRUE)              
    {
        if(Set_Temperature_Offset == TRUE)
            CalculateTemperatureOffset(TRUE);
        Calculate_Integrate_Value();
        Check_High_Voltage(1);
        RTCCRead();
        Update_One_Second = FALSE;
    }
}

void Check_for_Temperature_Reading(void) {
    if(Get_Temperature_Reading == TRUE)                                                         // once every 30 seconds
    {
        Check_Temperature();
        Update_Temperature_Correction();                                                        // (v1.02.07)
        Check_Pressure();                                                                       // (v1.02.03)
        HVBUpdateOffset(FALSE);
        Get_Temperature_Reading = FALSE;
    }
}

void UpdateReading(void) {
    INTEnable(INT_T1, 0);                                                                       // disable timer 1 interrupt
    if(Update_Electrometer_Average == TRUE)
    {
        ReadingCalculation();                                                                 // refactored (v1.04.00)
        Update_Electrometer_Average = FALSE;
    }
    else
    {            
        TakeADCReading();
    }
    INTEnable(INT_T1, 1);
}

void TakeADCReading(void) {
    ADCReading = GetADCValue(ADC_PORT0_SIGNAL1);                                            // read new electrometer value
    ADCReadingTotal += (UINT32) ADCReading;                                                 // add electrometer value to running total
    ADCReadingCounter++;                                                                    // increment total counter
    if(Range_Number == 5)                                                                   // if operating in the maximum range... (v1.01.05)
    {
        if(ADCReading > ADCReading_Peak)                                                    // ...if the new ADC reading is greater than the previous ADC reading... (v1.01.05)
        {
            ADCReading_Peak = ADCReading;                                                   //    ...store the new ADC reading (v1.01.05)
            Peak_Voltage = (ADCReading / 4095.0) * 2.5 - 0.200;;                                        //    ...calculate the peak voltage (v1.01.05)
        }
    }
}

void Check_for_Restart(void) {
    if((Restart_System == TRUE) && (Transmit_Data == FALSE))                                    // (v1.01.12)
    {
        DelayMs(100);
        Software_Reset();
    }
}

//*
//**************************************************************************************************
//**************************************************************************************************
//* END OF MAIN ROUTINE
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//* INITIALIZATION ROUTINES
//**************************************************************************************************
//**************************************************************************************************
//* Function:
//*     void AudioInit(void)
//*
//* Description:
//*     Initialize the audio. This function is dependent on the type of audio driver so it is
//*     instrument specific.
//*
//* Precondition:
//*     None
//*
//* Parameters:
//*     None
//*
//* Return Values:
//*     None
//*
//* Remarks:
//*     None
//*

void AudioInit(void)
{
    setVolume_i2c(i2cADDR_MAIN_DAC, VOLUME_EEPROM, 0);                                              // Set volume so it will powerup with the volume at 0.00% = 0
    Audio_IO_OutLAT = RESET_AUDIO;                                                                  // clear all bits
    WriteAudio();
    Audio_IO_OutLAT = (Audio_IO_OutLAT & MUTE_AUDIO) | AUDIO_MODE_2 | ENABLE_AUDIO;                 // mute audio, set audio to mode 2,  enable audio (9DP v1.01.13)
    WriteAudio();
    Audio_Mute = TRUE;                                                                              // audio is mute
    return;
}

// void Mute()
// {
//  Audio_IO_OutLAT = Audio_IO_OutLAT & MUTE_AUDIO;
//  WriteAudio();
//  Audio_Mute = TRUE;
// }

// void Unmute()
// {
//  Audio_IO_OutLAT = Audio_IO_OutLAT | UNMUTE_AUDIO;
//  WriteAudio();
//  Audio_Mute = FALSE;
// }


// void BeepNTimes(int times, int millis)
// {
//  int i;
//  for(i = 0; i < times; i++)
//      BeepOnce(millis);
// }

// void BeepOnce(int millis)
// {
//  int halfCycle = millis/2;

//  Audio_IO_OutLAT = Audio_IO_OutLAT | UNMUTE_AUDIO | ENABLE_ALARM;
//  WriteAudio();
//  DelayMs(halfCycle);

//  Audio_IO_OutLAT = Audio_IO_OutLAT & MUTE_AUDIO & DISABLE_ALARM;
//  WriteAudio();
//  DelayMs(halfCycle);
// }

//*
//**************************************************************************************************
//**************************************************************************************************
//* Function:
//*     void HVBInit( void )
//*
//* Description:
//*     Initialize the high voltage board. This function is dependent on the type of HVB so it is
//*     instrument specific.
//*
//* Precondition:
//*     None
//*
//* Parameters:
//*     None
//*
//* Return Values:
//*     None
//*
//* Remarks:
//*     None
//*


void HVBInit(void)
{
    Range_Number = 1;                                                                               // set range number variable
    Electrometer_Switches = Range_1;                                                                // set Range 1

    setupi2c2_hv_dac_0xc0();
    HVBUpdateOffset(TRUE);

    setupi2c2_hv_io_0x48();                                                                         // configure ioexpander to control the electrometer board range switches
    i2c2Write(i2cADDR_HV_IO_0x48, 0x0A, 1, Electrometer_Switches, 1);                               // write to the OLAT Register
    DelayMs(50);

    setupi2c2_x_temp_0x90();
    Check_Temperature();

    return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//* Function:
//*     void AudioPulseInit(void)
//*
//* Description:
//*     Initialize the audio pulse. This function is dependent on the type of audio driver so it is
//*     instrument specific and should only be required on the Model 9DP series.
//*
//* Precondition:
//*     None
//*
//* Parameters:
//*     None
//*
//* Return Values:
//*     None
//*
//* Remarks:
//*     None
//*

void AudioPulseInit(void)
{
    OpenTimer23(T2_ON | T2_IDLE_STOP | T2_PS_1_256 | T2_32BIT_MODE_ON | T2_SOURCE_INT, Period + Pulse_Width);
    OpenOC1(OC_ON | OC_IDLE_STOP | OC_TIMER_MODE32 | OC_TIMER2_SRC | OC_CONTINUE_PULSE, Period + Pulse_Width, Period);
    return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//* Function:
//*     void StructureInit(void)
//*
//* Description:
//*     Initialize the instrument structure in the event that all of the elements are not initialized
//*     after reading the EEPROM
//*
//* Precondition:
//*     None
//*
//* Parameters:
//*     None
//*
//* Return Values:
//*     None
//*
//* Remarks:
//*     None
//*

void StructureInit(void)
{
    BYTE detector_index;
    BYTE index;

#ifndef EEPROM_DISPLAY_PROPERTIES_LOCATION
    lmi.display.properties.info.powerup_mode = DEFAULT_DETECTOR_POWERUP_MODE;
    lmi.display.properties.info.current_detector = DEFAULT_DETECTOR;
#endif

#ifndef EEPROM_DETECTOR_PROPERTIES_LOCATION
    for(detector_index = 0; detector_index < NUMBER_OF_DETECTORS; detector_index++)
    {
        if(detector_index == 0)
            lmi.display.detector[detector_index].properties.info.model = lmi.calibration.header.info.instrument_type;
        else if(detector_index == 1)
            lmi.display.detector[detector_index].properties.info.model = DEFAULT_DETECTOR_1;
        else if(detector_index == 2)
            lmi.display.detector[detector_index].properties.info.model = DEFAULT_DETECTOR_2;
        else if(detector_index == 3)
            lmi.display.detector[detector_index].properties.info.model = DEFAULT_DETECTOR_3;
        else if(detector_index == 4)
            lmi.display.detector[detector_index].properties.info.model = DEFAULT_DETECTOR_4;
        strncpy((BYTE *)lmi.display.detector[detector_index].properties.info.serial_number,DEFAULT_DETECTOR_SERIAL_NUMBER,LENGTH_SERIAL_NUMBER);
        strncpy((BYTE *)lmi.display.detector[detector_index].properties.info.user_id_number,DEFAULT_DETECTOR_USER_ID_NUMBER,LENGTH_USER_ID_NUMBER);
        strncpy((BYTE *)lmi.display.detector[detector_index].properties.info.user_serial_number,DEFAULT_DETECTOR_USER_SERIAL_NUMBER,LENGTH_USER_SERIAL_NUMBER);
        strncpy((BYTE *)lmi.display.detector[detector_index].properties.info.technician,(BYTE *)lmi.calibration.header.info.technician,LENGTH_CALIBRATION_TECHNICIAN);
    }
#endif

#ifndef EEPROM_DETECTOR_CALIBRATION_LOCATION
    for(detector_index = 0; detector_index < NUMBER_OF_DETECTORS; detector_index++)
    {
        for(index = 0; index < NUMBER_OF_RANGES; index++)
            lmi.display.detector[detector_index].calibration.info.u_arg[index] = lmi.calibration.constants.info.range[index];
        lmi.display.detector[detector_index].calibration.info.u_arg[6] = lmi.calibration.high_voltage_board.info.meter_offset_gain_1;
        lmi.display.detector[detector_index].calibration.info.u_arg[7] = lmi.calibration.high_voltage_board.info.meter_offset_gain_2;
        lmi.display.detector[detector_index].calibration.info.u_arg[8] = lmi.calibration.high_voltage_board.info.meter_offset_gain_3;
        lmi.display.detector[detector_index].calibration.info.u_arg[9] = lmi.calibration.high_voltage_board.info.dac_1_output_2;
        lmi.display.detector[detector_index].calibration.info.f_arg[0] = lmi.calibration.high_voltage_board.info.cal_temperature;
        lmi.display.detector[detector_index].calibration.info.f_arg[1] = lmi.calibration.high_voltage_board.info.temperature_offset_cold;
        lmi.display.detector[detector_index].calibration.info.f_arg[2] = lmi.calibration.high_voltage_board.info.temperature_offset_hot;
        lmi.display.detector[detector_index].calibration.info.f_arg[3] = lmi.calibration.high_voltage_board.info.temperature_offset_hot_10;                     // (v1.02.07)
        lmi.display.detector[detector_index].calibration.info.f_arg[4] = lmi.calibration.high_voltage_board.info.temperature_offset_hot_15;                     // (v1.02.07)
        lmi.display.detector[detector_index].calibration.info.f_arg[5] = lmi.calibration.high_voltage_board.info.temperature_offset_hot_20;                     // (v1.02.07)
    }
#endif

    return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//* END OF INITIALIZATION ROUTINES
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//* 9DP STRUCTURE ROUTINES
//**************************************************************************************************
//**************************************************************************************************
//* Function:
//*     void StructureUpdate(BYTE structure_index, BYTE detector_index)
//*
//* Description:
//*     None
//*
//* Precondition:
//*     None
//*
//* Parameters:
//*     None
//*
//* Return Values:
//*     None
//*
//* Remarks:
//*     Is required for 9DP only
//*

void StructureUpdate(BYTE structure_index, BYTE detector_index)
{
    BYTE index;

    switch(structure_index)
    {
        //* Display Properties
        case 1:
        {
            break;
        }
        //* Display - Detector Properties
        case 2:
        {
            lmi.calibration.header.info.instrument_type = lmi.display.detector[detector_index].properties.info.model;
            strncpy((BYTE *)lmi.calibration.header.info.technician, (BYTE *)lmi.display.detector[detector_index].properties.info.technician, LENGTH_CALIBRATION_TECHNICIAN);
            i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_CALIBRATION_HEADER_LOCATION, 2, (BYTE *)lmi.calibration.header.buffer, SIZE_CALIBRATION_HEADER);
            break;
        }
        //* Display - Detector Calibration Constants (v1.01.07)
        case 3:
        {
            if(detector_index == 0)
            {
                for(index = 0; index < NUMBER_OF_RANGES; index++)
                {
                    lmi.calibration.constants.info.range[index] = lmi.display.detector[detector_index].calibration.info.u_arg[index];
                }
                i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_CALIBRATION_CONSTANTS_LOCATION, 2, (BYTE *)lmi.calibration.constants.buffer, SIZE_CALIBRATION_CONSTANTS);
            }
            break;
        }
        //* Display - Detector High Voltage Board Calibration (Excludes Temperature Offset) (v1.01.07)
        case 4:
        {
            if(detector_index == 0)
            {
                lmi.calibration.high_voltage_board.info.meter_offset_gain_1 = lmi.display.detector[detector_index].calibration.info.u_arg[6];
                lmi.calibration.high_voltage_board.info.meter_offset_gain_2 = lmi.display.detector[detector_index].calibration.info.u_arg[7];
                lmi.calibration.high_voltage_board.info.meter_offset_gain_3 = lmi.display.detector[detector_index].calibration.info.u_arg[8];
                lmi.calibration.high_voltage_board.info.dac_1_output_2 = lmi.display.detector[detector_index].calibration.info.u_arg[9];
                i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_CALIBRATION_HVB_LOCATION, 2, (BYTE *)lmi.calibration.high_voltage_board.buffer, SIZE_CALIBRATION_HVB);
            }
            break;
        }
        //* Display - Detector High Voltage Board Calibration (Temperature Offset only)
        case 5:
        {
            lmi.calibration.high_voltage_board.info.cal_temperature = lmi.display.detector[detector_index].calibration.info.f_arg[0];
            lmi.calibration.high_voltage_board.info.temperature_offset_cold = lmi.display.detector[detector_index].calibration.info.f_arg[1];
            lmi.calibration.high_voltage_board.info.temperature_offset_hot = lmi.display.detector[detector_index].calibration.info.f_arg[2];
            lmi.calibration.high_voltage_board.info.temperature_offset_hot_10 = lmi.display.detector[detector_index].calibration.info.f_arg[3];                 // (v1.02.07)
            lmi.calibration.high_voltage_board.info.temperature_offset_hot_15 = lmi.display.detector[detector_index].calibration.info.f_arg[4];                 // (v1.02.07)
            lmi.calibration.high_voltage_board.info.temperature_offset_hot_20 = lmi.display.detector[detector_index].calibration.info.f_arg[5];                 // (v1.02.07)
            i2c2WritePage(i2cADDR_MAIN_E2P_PRI, EEPROM_CALIBRATION_HVB_LOCATION, 2, (BYTE *)lmi.calibration.high_voltage_board.buffer, SIZE_CALIBRATION_HVB);
            break;
        }
        //* Display - Detector High Voltage Board Calibration (Temperature Offset only w/no EEPROM write)
        case 6:
        {
            lmi.calibration.high_voltage_board.info.cal_temperature = lmi.display.detector[detector_index].calibration.info.f_arg[0];
            lmi.calibration.high_voltage_board.info.temperature_offset_cold = lmi.display.detector[detector_index].calibration.info.f_arg[1];
            lmi.calibration.high_voltage_board.info.temperature_offset_hot = lmi.display.detector[detector_index].calibration.info.f_arg[2];
            lmi.calibration.high_voltage_board.info.temperature_offset_hot_10 = lmi.display.detector[detector_index].calibration.info.f_arg[3];                 // (v1.02.07)
            lmi.calibration.high_voltage_board.info.temperature_offset_hot_15 = lmi.display.detector[detector_index].calibration.info.f_arg[4];                 // (v1.02.07)
            lmi.calibration.high_voltage_board.info.temperature_offset_hot_20 = lmi.display.detector[detector_index].calibration.info.f_arg[5];                 // (v1.02.07)
            break;
        }
    }

    return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//* END 9DP CONVERSION ROUTINES
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//* I/O ROUTINES
//**************************************************************************************************
//**************************************************************************************************
//* Function:
//*     void Check_Buttons(void)
//*
//* Description:
//*     Checks to see if a button has been pressed (excluding ON/OFF)
//*
//* Precondition:
//*     None
//*
//* Parameters:
//*     None
//*
//* Return Values:
//*     None
//*
//* Remarks:
//*     This is instrument dependent because keypad layout will change with instruments.
//*

void Check_Buttons(void)
{
    static unsigned char released = 0;
    unsigned char Buttons;
    unsigned char testbuttons;
    unsigned char tempview;
    UINT8 counter; 
    SHORT zero_width = 0;

    Buttons = i2c2ReadByte(i2cADDR_MAIN_IO_BUTTON, 0x09, 1);                                        // read i2c I/O expander GPIO register value

    if(Backlight_Mode == BK_LIGHT_TEMP_ON)
    {
        testbuttons = (Buttons & 0x07);
        if((Light_On == TRUE) && (lmi.controls.backlight.info.current_level == 0))
        {
            Set_Backlight_Level(lmi.controls.backlight.info.on_level);
        }
        else if((testbuttons != 0x07) && (lmi.controls.backlight.info.current_level == 0))          // if button pressed and backlight level is 0
        {
            Light_Temp_On = TRUE;                                                                   // initialize light flag
            temporary_backlight_tick = tick + ((DWORD) TICKS_PER_SEC * lmi.controls.backlight.info.temp_time);          // (v1.01.05)
            Set_Backlight_Level(lmi.controls.backlight.info.on_level);
        }
        else if((Light_Temp_On == FALSE) && (Light_On == FALSE) && (lmi.controls.backlight.info.current_level == lmi.controls.backlight.info.on_level))
        {
            Set_Backlight_Level(0);
        }
    }

    if(((Buttons & BTN_ACK_RESET) == 0) || (Reset_Pressed == TRUE))                                 // check for ACK/RESET Pressed
    {   
        if(Reset_Pressed == FALSE)
        {
            if(released == TRUE)                                                                    // PRIORITY:
            {
                if(Flag_Custom_Msg_Enabled == TRUE)                                                 // process message event
                {
                    Reset_Pressed = TRUE;
                }
                else if((Present_Level == ALARM) && (Alarm_Clear_Flag == 0))                        // clear alarm audio
                {
                    Alarm_Clear_Flag = 1;
                    Set_Audio_Tone();                                                               // update audio tone settings
                }
                else if((Present_Level == ALERT) && (Alert_Clear_Flag == 0))                        // clear alert audio
                {
                    Alert_Clear_Flag = 1;
                    Set_Audio_Tone();                                                               // update audio tone settings
                }
                else if((Alert_Clear_Flag == 1) || (Alarm_Clear_Flag == 1))                         // clear the alarm/alert text if displayed
                {
                    Alert_Clear_Flag = 0;
                    Alarm_Clear_Flag = 0;
                    Stop_Timed_Dose = TRUE;
                    if(Integrate_Alert_Flag == TRUE)
                    {
                        Integrate_Alert_Flag = FALSE;
                        Integrate_Alert_Ack_Flag = TRUE;
                    }
                    if(Integrate_Alarm_Flag == TRUE)
                    {
                        Integrate_Alarm_Flag = FALSE;
                        Integrate_Alarm_Ack_Flag = TRUE;
                    }
//                  if(Radiation_Alert_Flag == TRUE)                                                // (v1.01.11)
//                  {
//                      Radiation_Alert_Flag = FALSE;                                               // (v1.01.11)
//                      Radiation_Alert_Ack_Flag = TRUE;                                            // (v1.01.11)
//                  }
//                  if(Radiation_Alarm_Flag == TRUE)                                                // (v1.01.11)
//                  {
//                      Radiation_Alarm_Flag = FALSE;                                               // (v1.01.11)
//                      Radiation_Alarm_Ack_Flag = TRUE;                                            // (v1.01.11)
//                  }
                    if(Flag_Status_Text_Displayed == TRUE)
                    {
                        ClearTopRightText();
                    }
                    if((Backlight_Mode == BK_LIGHT_TEMP_ON) && (Light_On == TRUE))
                    {
                        Light_On = FALSE;                                                           // clear light flag
                    }
                    New_Alarm = CLEAR;
                    add_error_message_to_queue(NORMAL_MESSAGE);
                }
                else if((Present_Level == NORMAL) && (New_Alarm == CLEAR))                          // check for a reset
                {
                    Reset_Pressed = TRUE;                                                           // indicate that the RESET button is pressed
                    Alert_Clear_Flag = 0;                                                           // clear the alarm flag just in case
                    Alarm_Clear_Flag = 0;                                                           // clear the alert flag just in case
                    Reset_Timer = 0;                                                                // clear reset timer
                    Start_Reset_Timer = TRUE;                                                       // start reset timer
                }
            }
        }
        else
        {
            switch(screenState)                                                                     // every state may respond differently
            {
                case MONITOR_RADIATION:
                {
                    counter = (TICKS_PER_SEC * 3) - 1;                                              // (v1.01.05)
                    if(released == TRUE)
                    {
                        Start_Reset_Timer = FALSE;
                        if(Reset_Timer <= counter)                                                  // (v1.01.05)
                        {
                            Update_Element(RESET_WINDOW);
                        }
                        Reset_Pressed = FALSE;                                                      // clear Reset variable since reset has finished
                        Flag_Custom_Msg_Enabled = FALSE;                                            // clear message event variable just in case
                    }
                    else if((Reset_Timer > counter) && (Start_Reset_Timer == TRUE))
                    {
                        Start_Reset_Timer = FALSE;
                        Reset_Function(0.000);                                                      // resets function
                    }
                    break;
                }
                case DISPLAY_ALERTS:
                {
                    if(released == TRUE)
                    {
                        Reset_Pressed = FALSE;                                                      // resets button routine for next press
                        clear_error_message_off_queue();                                            // clears alert off queue
                        add_error_message_to_queue(NORMAL_MESSAGE);                                 // resets queue so message can repeat
                        Rate_uR_hr = 0.000;                                                         // clear rate and start fresh
                        DisplayDetachOnce = TRUE;                                                   // allows rate to be reset
                        screenState = CREATE_ELEMENT;                                               // set state back to create element (clears message)
                        Flag_Custom_Msg_Enabled = FALSE;                                            // end message event
                        Present_Level = NORMAL;                                                     // return audio level to normal
                        Alert_Clear_Flag = 0;                                                       // clear the alert flag (just in case)
                        Set_Audio_Tone();                                                           // call audio routine to return audio to normal
                    }
                    break;
                }
            }
        }
    }

    if(((Buttons & BTN_AUDIO) == 0) && (released == TRUE))                                          // check for Audio Pressed
    {
        lmi.controls.audio.info.operating_mode++;                                                   // update audio operating mode
        lmi.controls.audio.info.operating_mode %= END_LEVEL_AUDIO;                                  // roll over if end of audio list
        Set_Audio_Tone();
    }

    if(((Buttons & BTN_FUNCTION) == 0) && (released == TRUE))                                       // check for FUNCTION Pressed
    {   
        New_Function_Window();
    }

    if(((Buttons & BTN_HANDLE_1) == 0) && (released == TRUE))                                       // check for views (v1.01.13)
    {   
//      if(screenState == MONITOR_RADIATION)
//      {
//          tempview = Current_View;
//          do
//          {
//              tempview = (tempview + 1) % NUMBER_OF_VIEWS;
//          } while(lmi.display.detector[Current_Detector].view[tempview].element_header.info.style == 0);
//          Current_View = tempview;
//          UpdateFunctionVariables();
//          screenState = CREATE_ELEMENT;
//          DisplayDetachOnce = TRUE;
//      }
    }

    if(((Buttons & BTN_HANDLE_2) == 0) && (released == TRUE))                                       // check for start (v1.01.13)
    {   
//      if(screenState == MONITOR_RADIATION)
//      {
//          Reset_Function(0.000);
//      }
    }

    if(((Buttons & BTN_HANDLE_3) == 0) && (released == TRUE))                                       // check for log (v1.01.13)
    {   
//      if(screenState == MONITOR_RADIATION)
//      {
//          Update_Log_File = TRUE;
//      }
    }

    if((Buttons == 0x7F) | (Buttons == 0xFF))                                                       // Verify that buttons are released
    {
        released = 1;
    }
    else
    {
        released = 0;
    }
    return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//* END I/O ROUTINES
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//  CALIBRATION ROUTINES
//**************************************************************************************************
//**************************************************************************************************
//* Function:
//*     void CalculateMeterOffset(UINT8 gain)
//*
//* Description:
//*     
//*
//* Precondition:
//*     None
//*
//* Parameters:
//*     None
//*
//* Return Values:
//*     None
//*
//* Remarks:
//*     Is required for 9DP only
//*

UINT16 CalculateMeterOffset(UINT8 gain)
{
    UINT8 new_range;
    UINT16 meteroffset = 0;

    switch(gain)
    {
        case 1:                                                                                         // (v1.01.01)
        case 2:
        case 3:
        {
            new_range = gain + 2;
            break;
        }
        default:
        {
            lmi.calibration.controls.info.fixed_range = 0;
            Update_Element(RESET_WINDOW);
            return 0;
        }
    }

    lmi.calibration.controls.info.fixed_range = new_range;
    Change_Range(new_range);
    Update_Element(RESET_WINDOW);
    i2c2MultiWrite(i2cADDR_HV_DAC_0xC0, (meteroffset + 0x9000), (electrometeroffset + 0x9000), (0x9000), (0x9000));

    // worse case scenarios: meteroffset == 0 or meteroffset == 4100
    for(meteroffset = 0; meteroffset < MAXDACVALUE; meteroffset += 10)                                  
    {
        i2c2MultiWrite(i2cADDR_HV_DAC_0xC0, (meteroffset + 0x9000), (electrometeroffset + 0x9000), (0x9000), (0x9000));
        DelayMs(5);
        ADCReading = GetADCValue((BYTE) ADC_PORT0_SIGNAL1);
        if(ADCReading <= 327)                                                                           // v1.04: max is now 2.5V: 0.200V / (2.5V / 4095) = 327.6
            break;
    }

    // skipped if previous worse case scenarios exist
    for(; (meteroffset > 0) && (meteroffset < MAXDACVALUE); meteroffset--)
    {
        i2c2MultiWrite(i2cADDR_HV_DAC_0xC0, (meteroffset + 0x9000), (electrometeroffset + 0x9000), (0x9000), (0x9000));
        DelayMs(5);
        ADCReading = GetADCValue((BYTE) ADC_PORT0_SIGNAL1);
        if(ADCReading >= 327)                                                                           // v1.04: max is now 2.5V: 0.200V / (2.5V / 4095) = 327.6
            break;
    }

    meteroffset = meteroffset + lmi.calibration.high_voltage_board.info.dac_1_offset;                   // (v1.0.73)

    if(meteroffset > MAXDACVALUE)
    {
        return MAXDACVALUE;
    }
    else if(meteroffset > 0)
    {
        switch(gain)
        {
            case 1:
            {
                lmi.calibration.high_voltage_board.info.meter_offset_gain_1 = meteroffset;
                lmi.display.detector[Current_Detector].calibration.info.u_arg[6] = lmi.calibration.high_voltage_board.info.meter_offset_gain_1;
                break;
            }
            case 2:
            {
                lmi.calibration.high_voltage_board.info.meter_offset_gain_2 = meteroffset;
                lmi.display.detector[Current_Detector].calibration.info.u_arg[7] = lmi.calibration.high_voltage_board.info.meter_offset_gain_2;
                break;
            }
            case 3:
            {
                lmi.calibration.high_voltage_board.info.meter_offset_gain_3 = meteroffset;
                lmi.display.detector[Current_Detector].calibration.info.u_arg[8] = lmi.calibration.high_voltage_board.info.meter_offset_gain_3;
                break;
            }
        }
        HVBUpdateOffset(FALSE);
        return meteroffset;
    }

    return 0;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//* Function:
//*     void CalculateTemperatureOffset(BOOL test_running)
//*
//* Description:
//*     
//*
//* Precondition:
//*     None
//*
//* Parameters:
//*     None
//*
//* Return Values:
//*     None
//*
//* Remarks:
//*     Is required for 9DP only
//*

void CalculateTemperatureOffset(BOOL test_running)
{
    static float reading_sum = 0.0;
    static float temperature_sum = 0.0;
    static float reading_average_backup = 0.0;                                                                                              // (v1.03.02)
    static float temperature_average_backup = 0.0;                                                                                          // (v1.03.02)
    static float reading_base_backup = 0.0;                                                                                                 // (v1.03.02)
    static UINT16 counter = 0;

    float reading_difference = 0.0;
    float temp_difference = 0.0;
    float temperature_correction = 0.0;                                                                                                     // (v1.03.02)

    if(test_running == FALSE)                                                                                                               // called fisr time through to initialize variables
    {
        Reading_Average = 0.0;                                                                                                              // (v1.03.02)
        Temperature_Average = 0.0;                                                                                                          // (v1.03.02)

        Temperature_Base = 0.0;
        Temperature_Hot = 0.0;
        Temperature_Cold = 0.0;

        lmi.display.detector[Current_Detector].calibration.info.f_arg[0] = Temperature_Base;                                                // update the structure (v1.03.02)
        lmi.display.detector[Current_Detector].calibration.info.f_arg[1] = Temperature_Cold;                                                // update the structure (v1.03.02)
        lmi.display.detector[Current_Detector].calibration.info.f_arg[2] = Temperature_Hot;                                                 // update the structure (v1.03.02)
        lmi.display.detector[Current_Detector].calibration.info.f_arg[3] = Temperature_Hot;                                                 // update the structure (v1.03.02)
        lmi.display.detector[Current_Detector].calibration.info.f_arg[4] = Temperature_Hot;                                                 // update the structure (v1.03.02)
        lmi.display.detector[Current_Detector].calibration.info.f_arg[5] = Temperature_Hot;                                                 // update the structure (v1.03.02)
        StructureUpdate(5, Current_Detector);                                                                                               // update the structure (v1.01.03)

        reading_sum = 0.0;
        temperature_sum = 0.0;
        reading_average_backup = 0.0;                                                                                                       // (v1.03.02)
        temperature_average_backup = 0.0;                                                                                                   // (v1.03.02)
        reading_base_backup = 0.0;                                                                                                          // (v1.03.02)
        counter = 0;

        Set_Base_Temperature_Offset = TRUE;
        Set_Low_Temperature_Offset = TRUE;
        Set_High_Temperature_Offset = TRUE;
        Set_High_Temperature_Offset_10 = TRUE;                                                                                              // (v1.02.07)
        Set_High_Temperature_Offset_15 = TRUE;                                                                                              // (v1.02.07)
        Set_High_Temperature_Offset_20 = TRUE;                                                                                              // (v1.02.07)
        Report_Temperature_Offset = TRUE;                                                                                                   // (v1.03.02)

        return;
    }

    if((Set_High_Temperature_Offset_20 == FALSE) && (Set_Low_Temperature_Offset == FALSE) && (Set_Base_Temperature_Offset == FALSE))        // called when routine is complete or reset (v1.02.07)
    {
        StructureUpdate(5, Current_Detector);                                                                                               // update the structure (v1.01.03)

        reading_sum = 0.0;                                                                                                                  // (v1.01.03)
        temperature_sum = 0.0;
        reading_average_backup = 0.0;                                                                                                       // (v1.03.02)
        temperature_average_backup = 0.0;                                                                                                   // (v1.03.02)
        reading_base_backup = 0.0;                                                                                                          // (v1.03.02)
        counter = 0;                                                                                                                        // (v1.03.02)

        Set_Temperature_Offset = FALSE;

        return;                                                                                                                             // (v1.03.02)
    }

    temp_difference = Temperature_Reading - temperature_average_backup;                                                                     // (v1.03.02)

    if(Set_Base_Temperature_Offset == TRUE)
    {
        counter++;
        if(counter > 3600)                                                                                                                  // added one hour (v1.02.01)
        {
            reading_sum += Rate_uR_hr;
            temperature_sum += Temperature_Reading;
            if(counter == 3780)                                                                                                             // corrected (v1.03.02)
            {
                Reading_Average = reading_sum / 180.0;                                                                                      // (v1.03.02)
                Temperature_Average = temperature_sum / 180.0;                                                                              // (v1.03.02)

                Temperature_Base = Temperature_Average;                                                                                     // (v1.03.02)
                lmi.display.detector[Current_Detector].calibration.info.f_arg[0] = Temperature_Base;                                        // update the variable (v1.01.03)
                StructureUpdate(6, Current_Detector);                                                                                       // update the structure (v1.01.14)

                reading_sum = 0.0;
                temperature_sum = 0.0;
                reading_average_backup = Reading_Average;                                                                                   // (v1.03.02)
                temperature_average_backup = Temperature_Average;                                                                           // (v1.03.02)
                reading_base_backup = Reading_Average;                                                                                      // (v1.03.02)
                counter = 0;

                Set_Base_Temperature_Offset = FALSE;
                Report_Temperature_Offset = TRUE;                                                                                           // (v1.03.02)
            }
        }                                                                                                                                   //
    }   
    else if((temp_difference > 4.5) && (Set_High_Temperature_Offset_15 == TRUE))                                                            // (v1.03.01)
    {                                                                                                                                       // (v1.02.07)
        counter++;                                                                                                                          // (v1.02.07)
        reading_sum += Rate_uR_hr;                                                                                                          // (v1.02.07)
        temperature_sum += Temperature_Reading;                                                                                             // (v1.02.07)
        if(counter == 180)                                                                                                                  // (v1.02.07)
        {                                                                                                                                   // (v1.02.07)
            Reading_Average = reading_sum / 180.0;                                                                                          // (v1.03.02)
            reading_difference = Reading_Average - reading_average_backup;                                                                  // (v1.03.02)
            Temperature_Average = temperature_sum / 180.0;                                                                                  // (v1.03.02)
            temp_difference = Temperature_Average - temperature_average_backup;                                                             // (v1.03.02)
    
            temperature_correction = fabs(reading_difference / temp_difference);                                                            // (v1.03.02)
            if(Set_High_Temperature_Offset == TRUE)                                                                                         // (v1.02.07)
            {                                                                                                                               // (v1.02.07)
                lmi.display.detector[Current_Detector].calibration.info.f_arg[2] = temperature_correction;                                  // (v1.03.02)
                Set_High_Temperature_Offset = FALSE;                                                                                        // (v1.02.07)
            }                                                                                                                               // (v1.02.07)
            else if(Set_High_Temperature_Offset_10 == TRUE)                                                                                 // (v1.02.07)
            {                                                                                                                               // (v1.02.07)
                lmi.display.detector[Current_Detector].calibration.info.f_arg[3] = temperature_correction;                                  // (v1.03.02)
                Set_High_Temperature_Offset_10 = FALSE;                                                                                     // (v1.02.07)
            }                                                                                                                               // (v1.02.07)
            else if(Set_High_Temperature_Offset_15 == TRUE)                                                                                 // (v1.02.07)
            {                                                                                                                               // (v1.02.07)
                lmi.display.detector[Current_Detector].calibration.info.f_arg[4] = temperature_correction;                                  // (v1.03.02)
                Set_High_Temperature_Offset_15 = FALSE;                                                                                     // (v1.02.07)
            }                                                                                                                               // (v1.02.07)
//          StructureUpdate(6, Current_Detector);                                                                                           // hidden (v1.03.03)

            reading_sum = 0.0;                                                                                                              // (v1.02.07)
            temperature_sum = 0.0;                                                                                                          // (v1.02.07)
            reading_average_backup = Reading_Average;                                                                                       // (v1.03.02)
            temperature_average_backup = Temperature_Average;                                                                               // (v1.03.02)
            counter = 0;                                                                                                                    // (v1.02.07)

            Report_Temperature_Offset = TRUE;                                                                                               // (v1.03.02)
        }       
    }
    else if((temp_difference > 4.8) && (Set_High_Temperature_Offset_20 == TRUE))                                                            // (v1.03.01)
    {                                                                                                                                       // (v1.03.01)
        counter++;                                                                                                                          // (v1.03.01)
        reading_sum += Rate_uR_hr;                                                                                                          // (v1.03.01)
        temperature_sum += Temperature_Reading;                                                                                             // (v1.03.01)
        if(counter == 180)                                                                                                                  // (v1.03.01)
        {                                                                                                                                   // (v1.03.01)
            Reading_Average = reading_sum / 180.0;                                                                                          // (v1.03.02)
            reading_difference = Reading_Average - reading_average_backup;                                                                  // (v1.03.02)
            Temperature_Average = temperature_sum / 180.0;                                                                                  // (v1.03.02)
            temp_difference = Temperature_Average - temperature_average_backup;                                                             // (v1.03.02)

            Temperature_Hot = Temperature_Average;                                                                                          // (v1.03.02)
            temperature_correction = fabs(reading_difference / temp_difference);                                                            // (v1.03.02)
            lmi.display.detector[Current_Detector].calibration.info.f_arg[5] = temperature_correction;                                      // (v1.03.02)
            StructureUpdate(6, Current_Detector);                                                                                           // (v1.03.01)

            reading_sum = 0.0;                                                                                                              // (v1.03.01)
            temperature_sum = 0.0;                                                                                                          // (v1.03.01)
            reading_average_backup = reading_base_backup;                                                                                   // (v1.03.02)
            temperature_average_backup = Temperature_Base;                                                                                  // (v1.03.02)
            counter = 0;                                                                                                                    // (v1.03.01)

            Set_High_Temperature_Offset_20 = FALSE;                                                                                         // (v1.03.01)
            Report_Temperature_Offset = TRUE;                                                                                               // (v1.03.02)
        }                                                                                                                                   // (v1.03.01)
    }                                                                                                                                       // (v1.03.01)
    else if((temp_difference < -25.0) && (Set_Low_Temperature_Offset == TRUE))
    {
        counter++;
        reading_sum += Rate_uR_hr;
        temperature_sum += Temperature_Reading;
        if(counter == 180)
        {
            Reading_Average = reading_sum / 180.0;                                                                                          // (v1.03.02)
            reading_difference = Reading_Average - reading_average_backup;                                                                  // (v1.03.02)
            Temperature_Average = temperature_sum / 180.0;                                                                                  // (v1.03.02)
            temp_difference = Temperature_Average - temperature_average_backup;                                                             // (v1.03.02)

            Temperature_Cold = Temperature_Average;                                                                                         // (v1.03.02)
            temperature_correction = fabs(reading_difference / temp_difference);                                                            // (v1.03.02)
            lmi.display.detector[Current_Detector].calibration.info.f_arg[1] = temperature_correction;                                      // (v1.03.02)
            StructureUpdate(6, Current_Detector);                                                                                           // (v1.03.01)

            reading_sum = 0.0;                                                                                                              // (v1.03.02)
            temperature_sum = 0.0;                                                                                                          // (v1.03.02)
            reading_average_backup = reading_base_backup;                                                                                   // (v1.03.02)
            temperature_average_backup = Temperature_Base;                                                                                  // (v1.03.02)
            counter = 0;                                                                                                                    // (v1.03.02)

            Set_Low_Temperature_Offset = FALSE;
            Report_Temperature_Offset = TRUE;                                                                                               // (v1.03.02)
        }
    }

    return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//* Function:
//*     void ADCOffset(void)
//*
//* Description:
//*     
//*
//* Precondition:
//*     None
//*
//* Parameters:
//*     None
//*
//* Return Values:
//*     None
//*
//* Remarks:
//*     Hidden (v1.01.12)
//*

//UINT16 ADCOffset(void)
//{
//  return GetADCValue((BYTE) ADC_PORT5_EXTRA1);
//}

//*
//**************************************************************************************************
//**************************************************************************************************
// END OF CALIBRATION ROUTINES
//**************************************************************************************************
//**************************************************************************************************



//*
//**************************************************************************************************
//**************************************************************************************************
//* Function:
//*     void HVBUpdateOffset(BOOL eeprom)
//*
//* Description:
//*     Adjust the value of the electrometer offset based on the current/calibration temperature
//*     difference. Change the meter offset based on the current range.
//*
//* Precondition:
//*     None
//*
//* Parameters:
//*     None
//*
//* Return Values:
//*     None
//*
//* Remarks:
//*     Required for all programs
//*

void HVBUpdateOffset(BOOL eeprom)
{
    unsigned char model_type;                                                                                                           // (v1.01.24)
    UINT16 meteroffset;
    INT16 INT16_offset_adjust = 0;
    float float_offset_adjust = 0.0;
//  float temp_difference = 0.0;

//  temp_difference = Temperature_Reading - lmi.calibration.high_voltage_board.info.cal_temperature;

    model_type = Check_Model_Number(lmi.settings.product.info.model_number);                                                            // (v1.01.24)
    if((model_type == MODEL_9DP_HIGH_PRESSURE) || (model_type == MODEL_9DP_ULTRA_HIGH_PRESSURE))                                        // (v1.01.24)
    {                                                                                                                                   // (v1.01.24)
//      if(Temperature_Reading >= lmi.calibration.high_voltage_board.info.cal_temperature)                                              // (v1.01.24)
//          float_offset_adjust = (temp_difference * lmi.calibration.high_voltage_board.info.temperature_offset_hot) / 1.68;            // (v1.01.24)
//      else                                                                                                                            // (v1.01.24)
//          float_offset_adjust = (temp_difference * lmi.calibration.high_voltage_board.info.temperature_offset_cold) / 1.68;           // (v1.01.24)
        if(Temperature_Reading >= lmi.calibration.high_voltage_board.info.cal_temperature)                                              // (v1.02.07)
            float_offset_adjust = temp_difference_adjust / 1.68;                                                                        // (v1.02.07)
        else                                                                                                                            // (v1.02.07)
            float_offset_adjust = temp_difference_adjust / 1.68;                                                                        // (v1.02.07)
    }                                                                                                                                   // (v1.01.24)
    else                                                                                                                                // (v1.01.24)
    {                                                                                                                                   // (v1.01.24)
//      if(Temperature_Reading >= lmi.calibration.high_voltage_board.info.cal_temperature)                                              // (v1.01.24)
//          float_offset_adjust = (temp_difference * lmi.calibration.high_voltage_board.info.temperature_offset_hot) / (1.68 * 40.2);   // (v1.01.24)
//      else                                                                                                                            // (v1.01.24)
//          float_offset_adjust = (temp_difference * lmi.calibration.high_voltage_board.info.temperature_offset_cold) / (1.68 * 40.2);  // (v1.01.24)
        if(Temperature_Reading >= lmi.calibration.high_voltage_board.info.cal_temperature)                                              // (v1.02.07)
            float_offset_adjust = temp_difference_adjust / (1.68 * 40.2);                                                               // (v1.02.07)
        else                                                                                                                            // (v1.02.07)
            float_offset_adjust = temp_difference_adjust / (1.68 * 40.2);                                                               // (v1.02.07)
    }                                                                                                                                   // (v1.01.24)

    INT16_offset_adjust = (INT16) float_offset_adjust;

    switch(Range_Number)
    {
        case 1:
        {
            lmi.calibration.high_voltage_board.info.dac_1_output_1 = lmi.calibration.high_voltage_board.info.meter_offset_gain_1;
            electrometeroffset = lmi.calibration.high_voltage_board.info.dac_1_output_2 - lmi.calibration.high_voltage_board.info.dac_1_offset + INT16_offset_adjust;
            break;
        }
        case 2:
        {
            lmi.calibration.high_voltage_board.info.dac_1_output_1 = lmi.calibration.high_voltage_board.info.meter_offset_gain_2;
            electrometeroffset = lmi.calibration.high_voltage_board.info.dac_1_output_2 - lmi.calibration.high_voltage_board.info.dac_1_offset + INT16_offset_adjust;
            break;
        }
        case 3:
        {
            lmi.calibration.high_voltage_board.info.dac_1_output_1 = lmi.calibration.high_voltage_board.info.meter_offset_gain_1;
            electrometeroffset = lmi.calibration.high_voltage_board.info.dac_1_output_2 - lmi.calibration.high_voltage_board.info.dac_1_offset;
            break;
        }
        case 4:
        {
            lmi.calibration.high_voltage_board.info.dac_1_output_1 = lmi.calibration.high_voltage_board.info.meter_offset_gain_2;
            electrometeroffset = lmi.calibration.high_voltage_board.info.dac_1_output_2 - lmi.calibration.high_voltage_board.info.dac_1_offset;
            break;
        }
        case 5:
        {
            lmi.calibration.high_voltage_board.info.dac_1_output_1 = lmi.calibration.high_voltage_board.info.meter_offset_gain_3;
            electrometeroffset = lmi.calibration.high_voltage_board.info.dac_1_output_2 - lmi.calibration.high_voltage_board.info.dac_1_offset;
            break;
        }
    }

    meteroffset = lmi.calibration.high_voltage_board.info.dac_1_output_1 - lmi.calibration.high_voltage_board.info.dac_1_offset;

    if(eeprom == FALSE)
    {
        i2c2MultiWrite(i2cADDR_HV_DAC_0xC0, (meteroffset + 0x9000), (electrometeroffset + 0x9000), (0x9000), (0x9000));
    }
    else
    {
        i2c2Write(i2cADDR_HV_DAC_0xC0, (0x40 | DAC_Channel_A), 1, (0x9000 + meteroffset), 2);
        i2c2Write(i2cADDR_HV_DAC_0xC0, (DAC_SingleWrite | DAC_Channel_A), 1, (0x9000 + meteroffset), 2);
        DelayMs(50);
    
        i2c2Write(i2cADDR_HV_DAC_0xC0, (0x40 | DAC_Channel_B), 1, (0x9000 + electrometeroffset), 2);
        i2c2Write(i2cADDR_HV_DAC_0xC0, (DAC_SingleWrite | DAC_Channel_B), 1, (0x9000 + electrometeroffset), 2);
        DelayMs(50);
    
        i2c2Write(i2cADDR_HV_DAC_0xC0, (0x40 | DAC_Channel_C), 1, (0x9000), 2);
        i2c2Write(i2cADDR_HV_DAC_0xC0, (DAC_SingleWrite | DAC_Channel_C), 1, (0x9000), 2);
        DelayMs(50);
    
        i2c2Write(i2cADDR_HV_DAC_0xC0, (0x40 | DAC_Channel_D), 1, (0x9000), 2);
        i2c2Write(i2cADDR_HV_DAC_0xC0, (DAC_SingleWrite | DAC_Channel_D), 1, (0x9000), 2);
        DelayMs(50);
    }

    return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//* Function:
//*     void Update_Temperature_Correction(void) (v1.02.07)
//*
//* Description:
//*     Adjust the value of the electrometer temperature correction based on the temperature
//*     difference.
//*
//* Precondition:
//*     None
//*
//* Parameters:
//*     None
//*
//* Return Values:
//*     None
//*
//* Remarks:
//*     Required for 9DP
//*

void Update_Temperature_Correction(void)
{
    float temp_difference = 0.0;

    temp_difference = Temperature_Reading - lmi.calibration.high_voltage_board.info.cal_temperature;

    if(Temperature_Reading >= lmi.calibration.high_voltage_board.info.cal_temperature)
    {
        if(temp_difference < 5.0)
        {
            temp_difference_adjust = temp_difference * lmi.calibration.high_voltage_board.info.temperature_offset_hot;
        }
        else if(temp_difference < 10.0)
        {
            temp_difference_adjust = (temp_difference - 5.0) * lmi.calibration.high_voltage_board.info.temperature_offset_hot_10;
            temp_difference_adjust += 5.0 * lmi.calibration.high_voltage_board.info.temperature_offset_hot;
        }
        else if(temp_difference < 15.0)
        {
            temp_difference_adjust = (temp_difference - 10.0) * lmi.calibration.high_voltage_board.info.temperature_offset_hot_15;
            temp_difference_adjust += 5.0 * lmi.calibration.high_voltage_board.info.temperature_offset_hot_10;
            temp_difference_adjust += 5.0 * lmi.calibration.high_voltage_board.info.temperature_offset_hot;
        }
        else
        {
            temp_difference_adjust = (temp_difference - 15.0) * lmi.calibration.high_voltage_board.info.temperature_offset_hot_20;
            temp_difference_adjust += 5.0 * lmi.calibration.high_voltage_board.info.temperature_offset_hot_15;
            temp_difference_adjust += 5.0 * lmi.calibration.high_voltage_board.info.temperature_offset_hot_10;
            temp_difference_adjust += 5.0 * lmi.calibration.high_voltage_board.info.temperature_offset_hot;
        }
    }
    else
        temp_difference_adjust = temp_difference * lmi.calibration.high_voltage_board.info.temperature_offset_cold;

    return;
}

void Adjust_Audio(void) {

    UINT32 timerReading;

    if(Update_Audio_Flag)
    {
        if((Period < 4000) || (Period > 500E3))
            Period = 4000;

        if(Period > 483608)
            Audio_Enable = FALSE;
        else
            Audio_Enable = TRUE;


        WritePeriod23(Period + Pulse_Width);
        SetPulseOC1(Period, (Period + Pulse_Width));
        timerReading = ReadTimer23();
        if(timerReading >= (Period + Pulse_Width))
            WriteTimer23(Period + Pulse_Width - 1000);
        Update_Audio_Flag = FALSE;
    }
}

void Adjust_Peak_Value(void) {
    if((Thirty_Second_Startup == TRUE) && (Rate_uR_hr > Peak_Rate_Reading))
    {
        Peak_Rate_Reading = Rate_uR_hr;
        if(Range_Lock == TRUE)
            Range_Lock_Valid = TRUE;
    }
}




//*
//**************************************************************************************************
//**************************************************************************************************
//* END OF ELECTROMETER ROUTINES
//**************************************************************************************************
//**************************************************************************************************



//*
//**************************************************************************************************
//**************************************************************************************************
//* LUDLUM MEASUREMENTS INC. COPYRIGHT (C) 2012-2013
//**************************************************************************************************
//**************************************************************************************************
