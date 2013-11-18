//**************************************************************************************************
//* File:   Detector.c
//* Author: Austin Mullins
//*
//* Created on September 19, 2013, 10:35 AM
//*
//* Company:        Ludlum Measurements
//* Address:        501 Oak Street
//*                 Sweetwater, TX 79556
//*
//**************************************************************************************************
//**************************************************************************************************
//*
//* Revision Notes:
//*
//*
//* ~~~~~~~~    09/19/13    Austin Mullins
//* - Original
//*       The purpose of this file is to define the functions declared in Detector.h.
//*       These functions essentially create a class "Detector" for which default detector
//*       objects can be constructed.
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

//**************************************************************************************************
//* File:   Detector.h
//* Author: amullins
//*
//* Created on September 19, 2013, 10:35 AM
//*
//* Company:        Ludlum Measurements
//* Address:        501 Oak Street
//*                 Sweetwater, TX 79556
//*
//**************************************************************************************************
//**************************************************************************************************
//*
//* Revision Notes:
//*
//*
//* ~~~~~~~~    09/19/13    Austin Mullins
//* - Original
//*       The purpose of this file is to define a detector type for instrument calibration.
//*       Each detector has a nominal conversion from CPS to uR/hr which will serve as the basis
//*       for calibrating the instrument. Detectors also have different high voltage and threshold
//*       defaults. This file merely sets up the base type for detectors. Detector models should be
//*       defined by a sub-class of this type.
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

#include "Detector.h"

Detector newDetector_byFields(DETECTOR_NUMBER model, int HV, int threshold, float uR_hr_CPS) {
    Detector newD;
    newD.model = model;
    newD.HV = HV;
    newD.threshold = threshold;
    newD.uR_hr_CPS = uR_hr_CPS;
    return newD;
}

void initializeDefaultDetector(Detector *Default_Detector) {
    Default_Detector[NO_DETECTOR]          =  newDetector_byFields(NO_DETECTOR, 0, 0, 1);
    Default_Detector[DETECTOR_9DP]         =  newDetector_byFields(DETECTOR_9DP, 0, 0, 1);
    Default_Detector[DETECTOR_9DP_1]       =  newDetector_byFields(DETECTOR_9DP_1, 0, 0, 1);
    Default_Detector[DETECTOR_9DLP]        =  newDetector_byFields(DETECTOR_9DLP, 0, 0, 1);
    Default_Detector[DETECTOR_9D]          =  newDetector_byFields(DETECTOR_9D, 0, 0, 1);
    Default_Detector[DETECTOR_UNDEFINED_1] =  newDetector_byFields(DETECTOR_UNDEFINED_1, 0, 0, 1);
    Default_Detector[DETECTOR_UNDEFINED_2] =  newDetector_byFields(DETECTOR_UNDEFINED_2, 0, 0, 1);
    Default_Detector[DETECTOR_UNDEFINED_3] =  newDetector_byFields(DETECTOR_UNDEFINED_3, 0, 0, 1);
    Default_Detector[DETECTOR_UNDEFINED_4] =  newDetector_byFields(DETECTOR_UNDEFINED_4, 0, 0, 1);
    Default_Detector[DETECTOR_UNDEFINED_5] =  newDetector_byFields(DETECTOR_UNDEFINED_5, 0, 0, 1);
    Default_Detector[DETECTOR_44_1]        =  newDetector_byFields(DETECTOR_44_1, 0, 0, 1);
    Default_Detector[DETECTOR_44_2]        =  newDetector_byFields(DETECTOR_44_2, 0, 0, 1);
    Default_Detector[DETECTOR_44_3]        =  newDetector_byFields(DETECTOR_44_3, 0, 0, 1);
    Default_Detector[DETECTOR_44_4]        =  newDetector_byFields(DETECTOR_44_4, 0, 0, 1);
    Default_Detector[DETECTOR_44_5]        =  newDetector_byFields(DETECTOR_44_5, 0, 0, 1);
    Default_Detector[DETECTOR_44_6]        =  newDetector_byFields(DETECTOR_44_6, 0, 0, 1);
    Default_Detector[DETECTOR_44_7]        =  newDetector_byFields(DETECTOR_44_7, 0, 0, 1);
    Default_Detector[DETECTOR_44_8]        =  newDetector_byFields(DETECTOR_44_8, 0, 0, 1);
    Default_Detector[DETECTOR_44_9]        =  newDetector_byFields(DETECTOR_44_9, 900, 60, 18.182);
    Default_Detector[DETECTOR_44_10]       =  newDetector_byFields(DETECTOR_44_10, 0, 0, 1);
    Default_Detector[DETECTOR_44_20]       =  newDetector_byFields(DETECTOR_44_20, 0, 0, 1);
    Default_Detector[DETECTOR_44_21]       =  newDetector_byFields(DETECTOR_44_21, 0, 0, 1);
    Default_Detector[DETECTOR_44_38]       =  newDetector_byFields(DETECTOR_44_38, 0, 0, 1);
    Default_Detector[DETECTOR_44_62]       =  newDetector_byFields(DETECTOR_44_62, 0, 0, 1);
    Default_Detector[DETECTOR_44_88]       =  newDetector_byFields(DETECTOR_44_88, 0, 0, 1);
    Default_Detector[DETECTOR_44_92]       =  newDetector_byFields(DETECTOR_44_92, 0, 0, 1);
    Default_Detector[DETECTOR_44_110]      =  newDetector_byFields(DETECTOR_44_110, 0, 0, 1);
    Default_Detector[DETECTOR_44_142]      =  newDetector_byFields(DETECTOR_44_142, 0, 0, 1);
    Default_Detector[DETECTOR_43_4]        =  newDetector_byFields(DETECTOR_43_4, 0, 0, 1);
    Default_Detector[DETECTOR_43_5]        =  newDetector_byFields(DETECTOR_43_5, 0, 0, 1);
}

Detector newDetector(DETECTOR_NUMBER model, Detector *Default_Detector) {
    Detector d = Default_Detector[model];
    return newDetector_byFields(model, d.HV, d.threshold, d.uR_hr_CPS);
}

