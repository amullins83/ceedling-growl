#ifndef __EEPROM_H
#define __EEPROM_H

//**************************************************************************************************
//**************************************************************************************************
//* EEPROM DEFINITIONS
//**************************************************************************************************
//**************************************************************************************************
//* CALIBRATION

#define EEPROM_CALIBRATION_LOCATION                         (0x0000)
#define EEPROM_CALIBRATION_HEADER_LOCATION                  (EEPROM_CALIBRATION_LOCATION)
#define EEPROM_CALIBRATION_CHECKOUT_LOCATION                (EEPROM_CALIBRATION_HEADER_LOCATION + TOTAL_CALIBRATION_HEADER)
#define EEPROM_CALIBRATION_CONSTANTS_LOCATION               (EEPROM_CALIBRATION_CHECKOUT_LOCATION + TOTAL_CALIBRATION_CHECKOUT)
#define EEPROM_CALIBRATION_OFFSETS_LOCATION                 (EEPROM_CALIBRATION_CONSTANTS_LOCATION + TOTAL_CALIBRATION_CONSTANTS)
#define EEPROM_CALIBRATION_HVB_LOCATION                     (EEPROM_CALIBRATION_OFFSETS_LOCATION + TOTAL_CALIBRATION_OFFSETS)
#define EEPROM_CALIBRATION_CONTROLS_LOCATION                (EEPROM_CALIBRATION_HVB_LOCATION + TOTAL_CALIBRATION_HVB)
#define EEPROM_CALIBRATION_CONFIG_LOCATION                  (EEPROM_CALIBRATION_CONTROLS_LOCATION + TOTAL_CALIBRATION_CONTROLS)

//**************************************************************************************************
//* SETTINGS

#define EEPROM_SETTINGS_LOCATION                            (0x1000)
#define EEPROM_PRODUCT_LOCATION                             (EEPROM_SETTINGS_LOCATION)
#define EEPROM_LANGUAGE_LOCATION                            (EEPROM_PRODUCT_LOCATION + TOTAL_PRODUCT)
#define EEPROM_CALENDAR_LOCATION                            (EEPROM_LANGUAGE_LOCATION + TOTAL_LANGUAGE)
#define EEPROM_CLOCK_LOCATION                               (EEPROM_CALENDAR_LOCATION + TOTAL_DATE)
#define EEPROM_PASSWORDS_LOCATION                           (EEPROM_CLOCK_LOCATION + TOTAL_TIME)
#define EEPROM_SETTINGS_MISC_LOCATION                       (EEPROM_PASSWORDS_LOCATION + TOTAL_PASSWORDS)
#define TOTAL_SETTINGS                                      (TOTAL_PRODUCT + TOTAL_LANGUAGE + TOTAL_DATE + TOTAL_TIME + TOTAL_PASSWORDS + TOTAL_SETTINGS_MISC)

//**************************************************************************************************
//* FEATURES

#define EEPROM_FEATURES_LOCATION                            (EEPROM_SETTINGS_LOCATION + TOTAL_SETTINGS)
#define EEPROM_SOURCE_CHECK_LOCATION                        (EEPROM_FEATURES_LOCATION)
#define EEPROM_BACKGROUND_CHECK_LOCATION                    (EEPROM_SOURCE_CHECK_LOCATION + TOTAL_SOURCE_CHECK)
#define EEPROM_INTEGRATE_LOCATION                           (EEPROM_BACKGROUND_CHECK_LOCATION + TOTAL_BACKGROUND_CHECK)
#define EEPROM_DATA_LOGGING_LOCATION                        (EEPROM_INTEGRATE_LOCATION + TOTAL_INTEGRATE)
#define TOTAL_FEATURES                                      (TOTAL_SOURCE_CHECK + TOTAL_BACKGROUND_CHECK + TOTAL_INTEGRATE + TOTAL_DATA_LOGGING)

//**************************************************************************************************
//* CONTROLS

#define EEPROM_CONTROLS_LOCATION                            (EEPROM_FEATURES_LOCATION + TOTAL_FEATURES)
#define EEPROM_BACKLIGHT_LOCATION                           (EEPROM_CONTROLS_LOCATION)
#define EEPROM_AUDIO_LOCATION                               (EEPROM_BACKLIGHT_LOCATION + TOTAL_BACKLIGHT)
#define EEPROM_GPS_LOCATION                                 (EEPROM_AUDIO_LOCATION + TOTAL_AUDIO)
#define EEPROM_BLUETOOTH_LOCATION                           (EEPROM_GPS_LOCATION + TOTAL_GPS)
#define TOTAL_CONTROLS                                      (TOTAL_BACKLIGHT + TOTAL_AUDIO + TOTAL_GPS + TOTAL_BLUETOOTH)

//**************************************************************************************************
//* DISPLAY

#define EEPROM_DISPLAY_LOCATION                             (EEPROM_CONTROLS_LOCATION + TOTAL_CONTROLS)

//#define EEPROM_DISPLAY_PROPERTIES_LOCATION                (EEPROM_DISPLAY_LOCATION)
#ifdef EEPROM_DISPLAY_PROPERTIES_LOCATION
    #define TOTAL_DISPLAY_PROPERTIES_EEPROM                 (TOTAL_DISPLAY_PROPERTIES)
#else
    #define TOTAL_DISPLAY_PROPERTIES_EEPROM                 0
#endif

//**************************************************************************************************
//* DISPLAY - DETECTOR

#define EEPROM_DETECTOR_LOCATION                            (EEPROM_DISPLAY_LOCATION + TOTAL_DISPLAY_PROPERTIES_EEPROM)

//#define EEPROM_DETECTOR_PROPERTIES_LOCATION               (EEPROM_DETECTOR_LOCATION)
#ifdef EEPROM_DETECTOR_PROPERTIES_LOCATION
    #define TOTAL_DETECTOR_PROPERTIES_EEPROM                (TOTAL_DETECTOR_PROPERTIES)
#else
    #define TOTAL_DETECTOR_PROPERTIES_EEPROM                0
#endif

//#define EEPROM_DETECTOR_CALIBRATION_LOCATION              (EEPROM_DETECTOR_LOCATION + TOTAL_DETECTOR_PROPERTIES_EEPROM)
#ifdef EEPROM_DETECTOR_CALIBRATION_LOCATION
    #define TOTAL_DETECTOR_CALIBRATION_EEPROM               (TOTAL_DETECTOR_CALIBRATION)
#else
    #define TOTAL_DETECTOR_CALIBRATION_EEPROM               0
#endif

#define EEPROM_DETECTOR_VIEW_SETUP_LOCATION                 (EEPROM_DETECTOR_LOCATION + TOTAL_DETECTOR_PROPERTIES_EEPROM + TOTAL_DETECTOR_CALIBRATION_EEPROM)
#ifdef EEPROM_DETECTOR_VIEW_SETUP_LOCATION
    #define TOTAL_DETECTOR_VIEW_SETUP_EEPROM                (TOTAL_DETECTOR_VIEW_SETUP)
#else
    #define TOTAL_DETECTOR_VIEW_SETUP_EEPROM                0
#endif

//**************************************************************************************************
//* DISPLAY - VIEW

#define EEPROM_VIEW_LOCATION                                (EEPROM_DETECTOR_LOCATION + TOTAL_DETECTOR_PROPERTIES_EEPROM + TOTAL_DETECTOR_CALIBRATION_EEPROM + TOTAL_DETECTOR_VIEW_SETUP_EEPROM)
#define EEPROM_APPEARANCE_LOCATION                          (EEPROM_VIEW_LOCATION)
#define EEPROM_ELEMENT_HEADER_LOCATION                      (EEPROM_APPEARANCE_LOCATION + TOTAL_APPEARANCE)
#define EEPROM_ELEMENT_LOCATION                             (EEPROM_ELEMENT_HEADER_LOCATION + TOTAL_ELEMENT_HEADER)
#define EEPROM_VIEW_FUNCTIONS_LOCATION                      (EEPROM_ELEMENT_LOCATION + TOTAL_ELEMENT)
#define EEPROM_ALERTS_RADIATION_LOCATION                    (EEPROM_VIEW_FUNCTIONS_LOCATION + TOTAL_VIEW_FUNCTIONS)
#define EEPROM_ALERTS_INTEGRATE_LOCATION                    (EEPROM_ALERTS_RADIATION_LOCATION + TOTAL_ALERTS_RADIATION)
#define EEPROM_ALERTS_FUNCTION_LOCATION                     (EEPROM_ALERTS_INTEGRATE_LOCATION + TOTAL_ALERTS_INTEGRATE)
#define EEPROM_ALARMS_RADIATION_LOCATION                    (EEPROM_ALERTS_FUNCTION_LOCATION + TOTAL_ALERTS_FUNCTION)
#define EEPROM_ALARMS_INTEGRATE_LOCATION                    (EEPROM_ALARMS_RADIATION_LOCATION + TOTAL_ALARMS_RADIATION)
#define EEPROM_ALARMS_FUNCTION_LOCATION                     (EEPROM_ALARMS_INTEGRATE_LOCATION + TOTAL_ALARMS_INTEGRATE)
//#define EEPROM_VIEW_CALIBRATION_LOCATION                  (EEPROM_ALARMS_FUNCTION_LOCATION + TOTAL_ALARMS_FUNCTION)
#ifdef EEPROM_VIEW_CALIBRATION_LOCATION
    #define TOTAL_VIEW                                      (TOTAL_APPEARANCE + TOTAL_ELEMENT_HEADER + TOTAL_ELEMENT + TOTAL_VIEW_FUNCTIONS + (6 * TOTAL_ALERTS_RADIATION) + TOTAL_VIEW_CALIBRATION)
#else
    #define TOTAL_VIEW                                      (TOTAL_APPEARANCE + TOTAL_ELEMENT_HEADER + TOTAL_ELEMENT + TOTAL_VIEW_FUNCTIONS + (6 * TOTAL_ALERTS_RADIATION))
#endif
#define TOTAL_VIEW_ARRAY                                    (NUMBER_OF_VIEWS * TOTAL_VIEW)

#define TOTAL_DETECTOR                                      (TOTAL_DETECTOR_PROPERTIES_EEPROM + TOTAL_DETECTOR_CALIBRATION_EEPROM + TOTAL_DETECTOR_VIEW_SETUP_EEPROM + TOTAL_VIEW_ARRAY)
#define TOTAL_DETECTOR_ARRAY                                (NUMBER_OF_DETECTORS * TOTAL_DETECTOR)

//**************************************************************************************************
//* FUNCTIONS

#define EEPROM_FUNCTIONS_LOCATION                           (EEPROM_DETECTOR_LOCATION + TOTAL_DETECTOR_ARRAY)
#define TOTAL_FUNCTIONS                                     (NUMBER_OF_FUNCTIONS * TOTAL_FUNCTION)

//**************************************************************************************************
//* ALERTS

#define EEPROM_ALERTS_LOCATION                              (EEPROM_FUNCTIONS_LOCATION + TOTAL_FUNCTIONS)
#define EEPROM_ALERTS_LOW_BATTERY_LOCATION                  (EEPROM_ALERTS_LOCATION)
#define EEPROM_ALERTS_LOW_MEMORY_LOCATION                   (EEPROM_ALERTS_LOW_BATTERY_LOCATION + TOTAL_ALERTS_LOW_BATTERY)
#define EEPROM_ALERTS_CALIBRATION_DUE_LOCATION              (EEPROM_ALERTS_LOW_MEMORY_LOCATION + TOTAL_ALERTS_LOW_MEMORY)
#define EEPROM_ALERTS_SOURCE_CHECK_LOCATION                 (EEPROM_ALERTS_CALIBRATION_DUE_LOCATION + TOTAL_ALERTS_CALIBRATION_DUE)
#define EEPROM_ALERTS_BACKGROUND_CHECK_LOCATION             (EEPROM_ALERTS_SOURCE_CHECK_LOCATION + TOTAL_ALERTS_SOURCE_CHECK)
#define NUMBER_OF_ALERTS                                    5
#define TOTAL_ALERTS                                        (NUMBER_OF_ALERTS * ALERTS_SIZE)

//**************************************************************************************************
//* ALARMS

#define EEPROM_ALARMS_LOCATION                              (EEPROM_ALERTS_LOCATION + TOTAL_ALERTS)
#define EEPROM_ALARMS_LOW_BATTERY_LOCATION                  (EEPROM_ALARMS_LOCATION)
#define EEPROM_ALARMS_LOW_MEMORY_LOCATION                   (EEPROM_ALARMS_LOW_BATTERY_LOCATION + TOTAL_ALARMS_LOW_BATTERY)
#define EEPROM_ALARMS_CALIBRATION_DUE_LOCATION              (EEPROM_ALARMS_LOW_MEMORY_LOCATION + TOTAL_ALARMS_LOW_BATTERY)
#define EEPROM_ALARMS_SOURCE_CHECK_LOCATION                 (EEPROM_ALARMS_CALIBRATION_DUE_LOCATION + TOTAL_ALARMS_CALIBRATION_DUE)
#define EEPROM_ALARMS_BACKGROUND_CHECK_LOCATION             (EEPROM_ALARMS_SOURCE_CHECK_LOCATION + TOTAL_ALARMS_SOURCE_CHECK)
#define EEPROM_ALARMS_HV_LOW_LOCATION                       (EEPROM_ALARMS_BACKGROUND_CHECK_LOCATION + TOTAL_ALARMS_BACKGROUND_CHECK)
#define EEPROM_ALARMS_HV_HIGH_LOCATION                      (EEPROM_ALARMS_HV_LOW_LOCATION + TOTAL_ALARMS_HV_LOW)
#define EEPROM_ALARMS_LOW_READING_LOCATION                  (EEPROM_ALARMS_HV_HIGH_LOCATION + TOTAL_ALARMS_HV_HIGH)
#define NUMBER_OF_ALARMS                                    8
#define TOTAL_ALARMS                                        (NUMBER_OF_ALARMS * ALARMS_SIZE)

//*
//**************************************************************************************************
//**************************************************************************************************
//* END EEPROM DEFINITIONS
//**************************************************************************************************
//**************************************************************************************************

#endif
