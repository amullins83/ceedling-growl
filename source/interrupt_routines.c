#include "instrument.h"

#define PIC32MX_TIMER3_INTERRUPT                0x00001000

// Globals needed
 // Timer-specific
extern DWORD tick;
extern BOOL Start_Reset_Timer;
extern BOOL Reset_Timer;
extern DWORD half_second_tick;
extern DWORD one_second_tick;
extern DWORD thirty_second_tick;
extern DWORD wait_state_tick;
extern DWORD system_error_tick;
extern DWORD message_event_tick;
extern DWORD log_file_write_tick;
extern DWORD temporary_backlight_tick;
extern DWORD integrate_update_tick;
extern DWORD timed_integrate_tick;
extern DWORD stream_report_tick;

 // ADC
extern UINT16 ADCReadingCounter;
extern UINT32 ADCReadingAverage;
extern UINT32 ADCReadingTotal;

 // Event flags
extern BOOL Update_Electrometer_Average;
extern BOOL Update_One_Second;
extern BOOL Update_Arc_Flag;
extern BOOL Update_Audio_Flag;
extern BOOL Update_Display_Rate_Flag;
extern BOOL Update_Count_Rate_Flag;
extern BOOL Get_Temperature_Reading;
extern BOOL Thirty_Second_Startup;
extern BOOL Wait_State_Event;
extern BOOL System_Error_Flag;
extern BOOL Message_Event_Flag;
extern BOOL Data_Logging_Enabled;
extern BOOL Update_Log_File;
extern BOOL Update_Data_Stream;
extern BOOL Light_Temp_On;
extern BOOL Rate_Accumulation;
extern BOOL Start_Timed_Dose;

 // The instrument model
extern TYPE_INSTRUMENT lmi;

 // The latest exposure reading in micro-Reontgens per hour
extern float new_rate_uR;

 // Application State
extern APP_STATE App_State_Keyboard;

//**************************************************************************************************
//**************************************************************************************************
//* INTERRUPT ROUTINES
//**************************************************************************************************
//**************************************************************************************************
//* Function:
//*     void __ISR(_TIMER_1_VECTOR, ipl6) Timer1Handler(void)
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
//*     Uses Timer 1
//*

void __ISR(_TIMER_1_VECTOR, ipl6) Timer1Handler(void)                                               // was ipl2 (29307.01.00.05)
{
    tick++;

    if(ADCReadingCounter != 0)
    {
        ADCReadingAverage = (ADCReadingTotal / ADCReadingCounter);
        ADCReadingTotal = 0;
        ADCReadingCounter = 0;
        Update_Electrometer_Average = TRUE;
    }        


    if(Start_Reset_Timer == TRUE)                                                                   // if reset button is pressed, start reset timer
    {
        Reset_Timer++;                                                                              // increment the reset timer every 1/2 second
    }

    if(tick == half_second_tick)
    {
        Update_Arc_Flag = TRUE;                                                                     //
        half_second_tick += (DWORD) TICKS_PER_SEC / 2;                                              // set new half second tick counter (v1.01.05)
    }

    if(tick == one_second_tick)
    {
        Update_One_Second = TRUE;                                                                   // update real time clock
        Update_Audio_Flag = TRUE;                                                                   // update audio
        Update_Display_Rate_Flag = TRUE;                                                            // update display rate
        Update_Count_Rate_Flag = TRUE;                                                              // update count rate
        one_second_tick += (DWORD) TICKS_PER_SEC;                                                   // set new one second tick counter (v1.01.05)
    }

    if(tick == thirty_second_tick)
    {
        Get_Temperature_Reading = TRUE;
        if(Thirty_Second_Startup == FALSE)
            Thirty_Second_Startup = TRUE;
        thirty_second_tick = tick + ((DWORD) TICKS_PER_SEC * 30);                                   // 30 seconds * samples per second (v1.01.05)
    }

    if((Wait_State_Event == FALSE) && (tick == wait_state_tick))                                    // compare tick to wait state tick
    {
        Wait_State_Event = TRUE;                                                                    // trigger wait state if appropriate time has passed
    }

    if((System_Error_Flag == TRUE) && (tick == system_error_tick))                                  // compare tick to system error display timer
    {
        System_Error_Flag = FALSE;                                                                  // clear displayed error if appropriate time has passed
    }

    if((Message_Event_Flag == FALSE) && (tick == message_event_tick))
    {
        Message_Event_Flag = TRUE;
    }

    if((Data_Logging_Enabled == TRUE) && (tick == log_file_write_tick))                             // compare tick to log file timer if logging is enabled
    {
        log_file_write_tick += (lmi.features.data_logging.info.sample_period * (DWORD) TICKS_PER_SEC);  // calculate next log file event (v1.01.05)
        Update_Log_File = TRUE;                                                                     // enable log file update flag
    }

    if((Light_Temp_On == TRUE) && (tick == temporary_backlight_tick))                               // compare tick to temporary backlight timer
    {
        Light_Temp_On = FALSE;                                                                      // turn light out if timer expired
    }

    if((lmi.features.integrate.info.operating_mode == 1) && (tick == integrate_update_tick))        // update integrate value if enabled
    {
        Rate_Accumulation += (new_rate_uR / (3600.0 * (float) TICKS_PER_SEC));                      // new rate / (60 minutes * 60 seconds * samples per second) (v1.01.05)
        new_rate_uR = 0.0;                                                                          // clear new reading (v1.01.02)
        integrate_update_tick++;                                                                    // update the integrate tick timer
    }

    if((Start_Timed_Dose == TRUE) && (tick == timed_integrate_tick))
    {
        Start_Timed_Dose = FALSE;
    }

    if((lmi.calibration.controls.info.stream_data_reading == 1) && (tick == stream_report_tick))    // compare tick to stream report time
    {
        Update_Data_Stream = TRUE;                                                                  // update data stream
        stream_report_tick = tick + lmi.calibration.controls.info.stream_report_time;               // set new stream report tick counter
    }

    mT1ClearIntFlag();                                                                              // clear the interrupt flag

    return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//* Function:
//*     void __ISR(_OUTPUT_COMPARE_1_VECTOR, ipl6) OC1_IntHandler(void)
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
//*     Uses Timer 2 and Timer 3
//*

void  __ISR(_OUTPUT_COMPARE_1_VECTOR, ipl6) OC1_IntHandler(void)                                     // was ipl4 (29307.01.00.05)
{
    IFS0CLR = 0x0040;                                                                               // Clear the interrupt flag
    return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//* Function:
//*     void __ISR(_TIMER_3_VECTOR, ipl6) _T3Interrupt(void)
//*
//* Description:
//*     Timer ISR, used to update application state. If no transfers are pending new input request
//*     is scheduled.
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
//*     Timer 3 (used by output compare for audio)
//*

void __ISR(_TIMER_3_VECTOR, ipl6) _T3Interrupt(void)                                                // was ipl2 (29307.01.00.05)
{
    if (IFS0 & PIC32MX_TIMER3_INTERRUPT)
    {
        mT3ClearIntFlag();                                                                          // Clear the interrupt flag
    }
}

//*
//**************************************************************************************************
//**************************************************************************************************
//* Function:
//*     void __ISR(_TIMER_4_VECTOR, ipl5) _T4Interrupt(void)
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
//*     Timer 4 (used by keyboard)
//*

void __ISR(_TIMER_4_VECTOR, ipl5) _T4Interrupt(void)                                                // was ipl4 (29307.01.00.05)
{
    if (IFS0bits.T4IF)
    {
        IFS0bits.T4IF   = 0;
        if(App_State_Keyboard == READY_TO_TX_RX_REPORT)
        {
            App_State_Keyboard = GET_INPUT_REPORT;                                                  // If no report is pending schedule new request
        }
    }
    return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//* Function:
//*     void __ISR(_TIMER_5_VECTOR, ipl7) Timer5Handler(void)
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
//*     Timer 5 (used by electrometer sampling)
//*

void __ISR(_TIMER_5_VECTOR, ipl6) Timer5Handler(void)
{
    ClrWdt();                                                                                       // clear watchdog timer
    mT5ClearIntFlag();                                                                              // clear the interrupt flag
    return;
}

//*
//**************************************************************************************************
//**************************************************************************************************
//* END INTERRUPT ROUTINES
//**************************************************************************************************
//**************************************************************************************************
