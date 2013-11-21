#ifndef GLOBAL_VARS_29307_H
#define GLOBAL_VARS_29307_H

#include "instrument.h"

//**************************************************************************************************
//**************************************************************************************************
//* CONFIGURATION BITS
//**************************************************************************************************
//**************************************************************************************************
//*
//* Configuration Bit settings
//*
//*     Primary Oscillator:             HS
//*     Internal USB 3.3v Regulator:    Disabled
//*     IOLOCK:                         Set Once
//*     Primary Oscillator Output:      Digital I/O
//*     Clock Switching and Monitor:    Both disabled
//*     Oscillator:                     Primary with PLL
//*     USB 96MHz PLL Prescale:         Divide by 2
//*     Internal/External Switch Over:  Enabled
//*     WDT Postscaler:                 1:32768
//*     WDT Prescaler:                  1:128
//*     WDT Window:                     Non-window Mode
//*     Comm Channel:                   EMUC2/EMUD2
//*     Clip on Emulation Mode:         Reset into Operation Mode
//*     Write Protect:                  Disabled
//*     Code Protect:                   Disabled
//*     JTAG Port Enable:               Disabled
//*

#if defined( __PIC32MX__ )
    #pragma config UPLLEN   = ON                                                                    // USB PLL Enabled
    #pragma config FPLLMUL  = MUL_18                                                                // PLL Multiplier   ---- was MUL_15 and changed to MUL_18 (2/10/2010)
    #pragma config UPLLIDIV = DIV_2                                                                 // USB PLL Input Divider
    #pragma config FPLLIDIV = DIV_2                                                                 // PLL Input Divider
    #pragma config FPLLODIV = DIV_1                                                                 // PLL Output Divider
    #pragma config FPBDIV   = DIV_1                                                                 // Peripheral Clock divisor
    #pragma config FWDTEN   = OFF                                                                   // Watchdog Timer
    #pragma config WDTPS    = PS1                                                                   // Watchdog Timer Postscale
    #pragma config FCKSM    = CSDCMD                                                                // Clock Switching & Fail Safe Clock Monitor
    #pragma config OSCIOFNC = OFF                                                                   // CLKO Enable
    #pragma config POSCMOD  = HS                                                                    // Primary Oscillator
    #pragma config IESO     = OFF                                                                   // Internal/External Switch-over
    #pragma config FSOSCEN  = OFF                                                                   // Secondary Oscillator Enable (KLO was off)
    #pragma config FNOSC    = PRIPLL                                                                // Oscillator Selection
    #pragma config CP       = OFF                                                                   // Code Protect
    #pragma config BWP      = OFF                                                                   // Boot Flash Write Protect
    #pragma config PWP      = OFF                                                                   // Program Flash Write Protect
    #pragma config ICESEL   = ICS_PGx2                                                              // ICE/ICD Comm Channel Select
    #pragma config DEBUG    = ON                                                                    // Background Debugger Enable
#else
    #error Cannot define configuration bits.
#endif

//*
//**************************************************************************************************
//**************************************************************************************************
//* END OF CONFIGURATION BITS
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//* CONSTANTS
//**************************************************************************************************
//**************************************************************************************************
//* Timer

#define STOP_TIMER_IN_IDLE_MODE                 0x2000
#define TIMER_SOURCE_INTERNAL                   0x0000
#define TIMER_ON                                0x8000
#define GATED_TIME_DISABLED                     0x0000
#define TIMER_16BIT_MODE                        0x0000
#define TIMER_PRESCALER_1                       0x0000
#define TIMER_PRESCALER_8                       0x0010
#define TIMER_PRESCALER_64                      0x0020
#define TIMER_PRESCALER_256                     0x0030
#define TIMER_INTERRUPT_PRIORITY                0x0001

//* Fonts

#define Symbol_SansSerif_12_SIZE                91
#define Normal_Arial_Black_44_SIZE              8526
#define Secondary_Arial_Black_14_SIZE           9140
#define GOLFontDefault_SIZE                     4424
#define Arial_Rounded_12pt_32_90_SIZE           2956
#define Normal_Arial_Black_36_SIZE              6270
#define Normal_Arial_Black_8_SIZE               3489

//**************************************************************************************************
//* USB

#define MAX_NO_OF_IN_BYTES                      64
#define MAX_NO_OF_OUT_BYTES                     64 
#define PIC32MX_TIMER3_INTERRUPT                0x00001000

//*
//**************************************************************************************************
//**************************************************************************************************
//* END OF CONSTANTS
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//* GLOBAL VARIABLES
//**************************************************************************************************
//**************************************************************************************************
//* Bit Variables

volatile BOOL Update_Arc_Flag                   = FALSE;
volatile BOOL Update_Count_Rate_Flag            = FALSE;
volatile BOOL Update_Display_Rate_Flag          = FALSE;
volatile BOOL Update_Log_File                   = FALSE;
volatile BOOL Update_Electrometer_Average       = FALSE;
volatile BOOL Reset_Level                       = FALSE;
volatile BOOL Reset_Pressed                     = FALSE;
volatile BOOL DisplayDetachOnce                 = FALSE;
volatile BOOL System_Error_Flag                 = FALSE;
volatile BOOL Transmit_Data                     = FALSE;
volatile BOOL Audio_Mute                        = FALSE;
volatile BOOL Audio_Enable                      = TRUE;
volatile BOOL Light_On                          = FALSE;
volatile BOOL Light_Temp_On                     = FALSE;
volatile BOOL Data_Logging_Header_Sent          = FALSE;
volatile BOOL Power_Up_Report_Integrate         = FALSE;
volatile BOOL Power_Down_Report_Integrate       = FALSE;
volatile BOOL Thirty_Second_Startup             = FALSE;
volatile BOOL Range_Lock                        = FALSE;
volatile BOOL Range_Lock_Valid                  = FALSE;
volatile BOOL Message_Event_Flag                = FALSE;
volatile BOOL Data_Logging_Enabled              = FALSE;
volatile BOOL Pulsed_Mode_Function              = FALSE;
volatile BOOL Start_Timed_Dose                  = FALSE;
volatile BOOL Stop_Timed_Dose                   = TRUE;
volatile BOOL Get_Temperature_Reading           = TRUE;
volatile BOOL Set_Temperature_Offset            = FALSE;
volatile BOOL Set_High_Temperature_Offset       = FALSE;
volatile BOOL Set_High_Temperature_Offset_10    = FALSE;                                            // (v1.02.07)
volatile BOOL Set_High_Temperature_Offset_15    = FALSE;                                            // (v1.02.07)
volatile BOOL Set_High_Temperature_Offset_20    = FALSE;                                            // (v1.02.07)
volatile BOOL Set_Low_Temperature_Offset        = FALSE;
volatile BOOL Set_Base_Temperature_Offset       = FALSE;
volatile BOOL Drained_Battery                   = FALSE;
volatile BOOL First_System_Initialization       = FALSE;
volatile BOOL System_Shutdown                   = FALSE;
volatile BOOL Integrate_Alert_Flag              = FALSE;                                            // renamed (v1.01.11)
volatile BOOL Integrate_Alert_Ack_Flag          = FALSE;                                            // renamed (v1.01.11)
volatile BOOL Integrate_Alarm_Flag              = FALSE;                                            // renamed (v1.01.11)
volatile BOOL Integrate_Alarm_Ack_Flag          = FALSE;                                            // renamed (v1.01.11)
volatile BOOL Radiation_Alert_Flag              = FALSE;                                            // (v1.01.11)
volatile BOOL Radiation_Alert_Ack_Flag          = FALSE;                                            // (v1.01.11)
volatile BOOL Radiation_Alarm_Flag              = FALSE;                                            // (v1.01.11)
volatile BOOL Radiation_Alarm_Ack_Flag          = FALSE;                                            // (v1.01.11)
volatile BOOL Restart_System                    = FALSE;                                            // (v1.01.12)
volatile BOOL Perform_Auto_Background           = FALSE;                                            // (v1.01.21)
volatile BOOL Report_Temperature_Offset         = FALSE;                                            // (v1.02.01)

//**************************************************************************************************
//* 8-bit Variables

volatile BYTE Save_Config_To_Thumbdrive         = 0;                                                // changed to BYTE value (v1.02.08)
volatile BYTE Current_Detector                  = 0;                                                // 
volatile BYTE Current_View                      = 0;                                                // initialized in EEPROM_Assign_Global_Variables()
volatile BYTE Current_Function_Index            = 0;                                                // initialized in EEPROM_Assign_Global_Variables()
volatile BYTE Range_Number                      = 1;                                                // start on Range 1
volatile BYTE New_Alarm                         = 0;                                                // Flag to determine current alarm condition -- default 0 - no alert/alarm
volatile BYTE Present_Level                     = 0;                                                // default 0 - Normal
volatile BYTE Audio_Div                         = 0;                                                // default 0 - divide by 10
volatile BYTE Audio_Mode                        = 2;                                                // default 2 - PIC32 driven audio
volatile BYTE Backlight_Mode                    = 0;                                                // initialized in EEPROM_Assign_Global_Variables()
volatile BYTE Audio_Operating_Mode              = 0;                                                // define audio icon variable to track which icon is displayed
volatile char DisplayMessage[29]                = {'\0'};                                           // global message buffer

volatile BYTE ExposureMultiplier                = 0;
volatile BYTE ExposureUnits                     = 0;
volatile BYTE ExposureRange                     = 0;

volatile char Total_Chars                       = 0;
volatile char Last_Total_Chars                  = -1;

volatile char BTNkeyboard_down                  = 1;                                                // enabled = 0, disabled = 1
volatile char BTNkeyboard_up                    = 1;                                                // enabled = 0, disabled = 1
volatile char BTNkeyboard_enter                 = 1;                                                // enabled = 0, disabled = 1
volatile char BTNkeyboard_Esc                   = 1;                                                // enabled = 0, disabled = 1

volatile BYTE Audio_IO_OutLAT                   = 0x00;                                             // keep trace of i2c I/O expander set bits

volatile BYTE USB_Port_Attachment               = NO_DEVICE_ATTACHED;                               // no device is attached to the USB port

volatile BYTE Default_Rate_Units                = R_HR;
volatile BYTE Default_Dose_Units                = R;

volatile BYTE menu_line[10];                                                                        // (v1.01.01)

volatile BYTE Correction_Factor_Mode            = 0;                                                // (v1.02.02)

//**************************************************************************************************
//* 16-bit Variables

//volatile SHORT Default_Rate_Multiplier            = MICRO;                                        // hidden (v1.01.12) converted to constant
//volatile SHORT Default_Dose_Multiplier            = MICRO;                                        // hidden (v1.01.12)

volatile WORD foreground;                                                                           // initialized in EEPROM_Assign_Global_Variables()
volatile WORD background;                                                                           // initialized in EEPROM_Assign_Global_Variables()
volatile WORD Full_Scale_Arc_Range[6][2];

volatile UINT16 ADCReading_Peak                 = 0;                                                // (v1.01.05)

//**************************************************************************************************
//* 32-bit Variables

volatile DWORD tick                             = 0;                                                // tick counter (1/2 second)
volatile DWORD half_second_tick                 = 4;                                                // (29307.01.00.05)
volatile DWORD one_second_tick                  = 8;                                                // 8 * 1/8 = 1 second
volatile DWORD thirty_second_tick               = 240;                                              // 240 * 1/8 = 30 seconds
volatile DWORD wait_state_tick                  = 0;                                                // future tick value
volatile DWORD system_error_tick                = 0;                                                // system error report tick
volatile DWORD message_event_tick               = 0;                                                // message event tick
volatile DWORD log_file_write_tick              = 480;                                              // log file write tick timer (60 seconds * 8 samples per second)
volatile DWORD integrate_update_tick            = 480;                                              // integrate update timer (60 seconds * 8 samples per second)
volatile DWORD temporary_backlight_tick         = 0;                                                // temporary backlight timer
volatile DWORD timed_integrate_tick             = 0;                                                //
volatile DWORD stream_report_tick               = 0;                                                //
volatile DWORD debugValue                       = 0;                                                // (v1.01.05)

//**************************************************************************************************
//* Float Variables

volatile float Radiation_Alert_Rate             = 0.0;                                              // initialized in EEPROM_Assign_Global_Variables()
volatile float Radiation_Alarm_Rate             = 0.0;                                              // initialized in EEPROM_Assign_Global_Variables()
volatile float Integrate_Alert_Rate             = 0.0;                                              // initialized in EEPROM_Assign_Global_Variables()
volatile float Integrate_Alarm_Rate             = 0.0;                                              // initialized in EEPROM_Assign_Global_Variables()
volatile float Rate_uR_hr                       = 0.0;
volatile float Rate_xR_hr                       = 0.0;
volatile float Peak_Rate_Reading                = 0.0;
volatile float Rate_Accumulation                = 0.0;
volatile float Battery_Reading                  = 0.0;
volatile float Temperature_Reading              = 0.0;
volatile float High_Voltage_Reading             = 0.0;
volatile float High_Voltage_Temperature_Reading = 0.0;
volatile float Signal_Voltage                   = 0.0;
volatile float Peak_Voltage                     = 0.0;
volatile float uR_filter                        = 0.0;                                              // lowest range uR filter
volatile float Temperature_Base                 = 0.0;
volatile float Temperature_Hot                  = 0.0;
volatile float Temperature_Cold                 = 0.0;
volatile float Correction_Factor_Value          = 1.000;                                            // (v1.02.02)
volatile float Chamber_Pressure_PSI             = 0.0;                                              // (v1.02.03)
volatile float Temperature_Average              = 0.0;                                              // (v1.03.02)
volatile float Reading_Average                  = 0.0;                                              // (v1.03.02)

//**************************************************************************************************
//* Structures

volatile TYPE_INSTRUMENT lmi;
volatile TYPE_FLOAT_READING_FORMAT old_reading;
volatile TYPE_FLOAT_READING_FORMAT new_reading;
volatile TYPE_MESSAGE_BOX pmsgScreen;

//**************************************************************************************************
//* Global States Used

volatile APP_STATE App_State_Keyboard           = DEVICE_NOT_CONNECTED;
volatile APP_STATE App_State_FTDI               = DEVICE_NOT_CONNECTED;
volatile APP_STATE App_State_Thumbdrive         = DEVICE_NOT_CONNECTED;
volatile COMPUTER_STATE PC_State                = PC_NOT_CONNECTED;                                 // pc is disconnected until FTDI device is attached
volatile IMAGE_STATE imageState                 = IMAGE_START;

//*
//**************************************************************************************************
//**************************************************************************************************
//* END OF GLOBAL VARIABLES
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//* EXTERNAL GLOBAL VARIABLES
//**************************************************************************************************
//**************************************************************************************************
//* Bit Variables

extern BOOL Flag_Status_Text_Displayed;
extern BOOL Flag_Note_Text_Displayed;
extern BOOL Flag_Custom_Msg_Enabled;
extern BOOL Custom_Error_Pending;
extern BOOL editScreen;
extern BOOL editMode;
extern BOOL Update_Data_Stream;

//**************************************************************************************************
//* 8-bit Variables

extern BYTE ViewMenu;
extern BYTE DetectorMenu;
extern BYTE USB_RXMessage[MESSAGE_SIZE];
extern BYTE USB_TXMessage[MESSAGE_SIZE];
extern BYTE File_TXMessage[MESSAGE_SIZE];                                                           // (v1.02.01)
extern BYTE TXMessageIndex;
extern UINT8 Error_Recent;
extern UINT8 Error_Last;

extern char CMD_Detector;
extern char CMD_Range;
extern char CMD_View;
extern char CMD_Function;

//**************************************************************************************************
//* 16-bit Variables

extern signed short MenuLine;
extern signed short SubMenuLine;

//**************************************************************************************************
// External Image Storage

extern DWORD MemoryAddress;
extern DWORD FileErrorCounter;

//**************************************************************************************************
// External Graphic Schemes

extern GOL_SCHEME *altScheme;
extern GOL_SCHEME *btnScheme;

//**************************************************************************************************
//* Bitmap Images

extern BITMAP_FLASH LMI_Icon;
extern BITMAP_FLASH LB_Down_Arrow;
extern BITMAP_FLASH LB_Up_Arrow;

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

//**************************************************************************************************
//* Fonts

extern const FONT_FLASH Symbol_SansSerif_12;
extern const FONT_FLASH Normal_Arial_Black_44;
extern const FONT_FLASH Secondary_Arial_Black_14;
extern const FONT_FLASH GOLFontDefault;
extern const FONT_FLASH Arial_Rounded_12pt_32_90;
extern const FONT_FLASH Normal_Arial_Black_36;
extern const FONT_FLASH Normal_Arial_Black_8;

//**************************************************************************************************
// Additional Variables

extern HID_DATA_DETAILS         Appl_LED_Indicator;
extern HID_DATA_DETAILS         Appl_ModifierKeysDetails;
extern HID_DATA_DETAILS         Appl_NormalKeysDetails;
extern HID_USER_DATA_SIZE       Appl_BufferModifierKeys[8];
extern HID_USER_DATA_SIZE       Appl_BufferNormalKeys[6];
extern HID_USER_DATA_SIZE       Appl_ShadowBuffer1[6];
extern HID_REPORT_BUFFER        Appl_raw_report_buffer;
extern HID_LED_REPORT_BUFFER    Appl_led_report_buffer;

//*
//**************************************************************************************************
//**************************************************************************************************
//* END OF GLOBAL VARIABLES
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//* LOCAL VARIABLES
//**************************************************************************************************
//**************************************************************************************************
//* Bit Variables

BOOL Start_Reset_Timer                          = FALSE;
BOOL Alarm_Clear_Flag                           = 0;
BOOL Alert_Clear_Flag                           = 0;
BOOL Wait_State_Event                           = 0;
BOOL ReportBufferUpdated;
BOOL Update_Audio_Flag                          = FALSE;
BOOL Update_One_Second                          = FALSE;

//**************************************************************************************************
//* 8-bit Variables

BYTE USBDeviceAddress                           = 0;                                                // Address of the device on the USB
BYTE usbErrorCode;                                                                                  // USB error                                        
BYTE usbHIDStatus;                                                                                  // HID device status
BYTE usbMSDStatus;                                                                                  // MSD device status

BYTE currCharPos;
BYTE FirstKeyPressed ;

BYTE Reset_Timer                                = 0;
BYTE GetNewSamples                              = 0;                                                // indicate gathering new sample information
BYTE Electrometer_Switches                      = 0x00;                                             // keep track of i2c I/O expander HV board switch settings

//**************************************************************************************************
//* 16-bit Variables

const short x_orgin                             = 160;                                              // variable to hold the ARC X-axis orgin which calculations are based on
const short y_orgin                             = 170;                                              // variable to hold the ARC Y-axis orgin which calcualtions are based on                                    
const short edge_spacing                        = 20;                                               // variable used with calculations when drawing the arc and tick marks (spacing from edge of screen)

UINT16 ADCReading                               = 0;
UINT16 ADCReadingCounter                        = 0;
UINT16 electrometeroffset                       = 0;

//**************************************************************************************************
//* 32-bit Variables

UINT32 ADCReadingTotal                          = 0;
UINT32 ADCReadingAverage                        = 0;
UINT32 Period                                   = 2E05;
UINT32 Pulse_Width                              = 1E03;

//**************************************************************************************************
//* Float Variables

float new_rate_uR                               = 0.0;
float exposure                                  = 0.0;
float ADCReadingAccumulator                     = 0.0;
float temp_peak_voltage                         = 0.0;
float temp_difference_adjust                    = 0.0;                                              // (v1.02.07)

//**************************************************************************************************
//* Strings Used

const XCHAR multipliers[]                       = {0,'z','a','f','p','n',0x007F,'m',0,'k','M','G','T','P','E','Z'};

//**************************************************************************************************
//* Screen States

SCREEN_STATES screenState                       = CREATE_SPLASH;                                    // current state of main demo state mashine 
SCREEN_STATES nextscreenState                   = NOT_DISPLAYED;                                    // next screen state
SCREEN_STATES PreviousScreenState               = NOT_DISPLAYED;

//**************************************************************************************************
//* File System

FSFILE * BMPFile                                = NULL;

//**************************************************************************************************
//* Special Instrument Properties

InstrumentConstants *ic;
RunningAverage *GlobalReadingAverage;

//*
//**************************************************************************************************
//**************************************************************************************************
//* END OF LOCAL VARIABLES
//**************************************************************************************************
//**************************************************************************************************

//**************************************************************************************************
//**************************************************************************************************
//* FUNCTION DECLARATIONS
//**************************************************************************************************
//**************************************************************************************************
//* Initialization Routines

void AudioInit(void);
void AudioPulseInit(void);
void HVBInit(void);
void StructureInit(void);
// void BeepNTimes(int times, int millis);
// void BeepOnce(int millis);
void WriteAudio();
// void Mute();
// void Unmute();

//**************************************************************************************************
//* 9DP Conversion Routines

void StructureUpdate(BYTE structure_index, BYTE detector_index);

//**************************************************************************************************
//* I/O Routines

void Check_Buttons(void);

//**************************************************************************************************
//* Calibration Routines

INT16 Calculate_DAC_Meter_Offset(UINT16 threshold);
UINT16 CalculateMeterOffset(UINT8 gain);
void CalculateTemperatureOffset(BOOL test_running);
void Update_Temperature_Correction(void);                                                           // (v1.02.07)

//**************************************************************************************************
//* Electrometer Routines

void Update_Range(void);                   // n1 Extracted to facilitate range updates outside redraw cycle
void Change_Range(BYTE new_range);
void HVBUpdateOffset(BOOL eeprom);

//**************************************************************************************************
//* Top-level

void TopLevelInitialize(void);
void MainLoop(void);
void One_Second_Updates(void);
void Check_for_Temperature_Reading(void);
void UpdateReading(void);
void TakeADCReading(void);
void Check_for_Restart(void);

// Called Inside Main Loop
void Adjust_Audio(void);        // (v1.04.00)
void Adjust_Peak_Value(void);   // (v1.04.00)


//*
//**************************************************************************************************
//**************************************************************************************************
//* END OF FUNCTION DECLARATIONS
//**************************************************************************************************
//**************************************************************************************************

#endif
