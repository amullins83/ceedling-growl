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

#ifndef DETECTOR_H
#define	DETECTOR_H

//**************************************************************************************************
//* Detector Number

typedef enum _DETECTOR_NUMBER
{
	NO_DETECTOR = 0,																				// 0
	DETECTOR_9DP,
	DETECTOR_9DP_1,
	DETECTOR_9DLP,
	DETECTOR_9D,
	DETECTOR_UNDEFINED_1,
	DETECTOR_UNDEFINED_2,
	DETECTOR_UNDEFINED_3,
	DETECTOR_UNDEFINED_4,
	DETECTOR_UNDEFINED_5,
	DETECTOR_44_1,																					// 10
	DETECTOR_44_2,
	DETECTOR_44_3,
	DETECTOR_44_4,
	DETECTOR_44_5,
	DETECTOR_44_6,
	DETECTOR_44_7,
	DETECTOR_44_8,
	DETECTOR_44_9,
	DETECTOR_44_10,
	DETECTOR_44_20,																					// 20
	DETECTOR_44_21,
	DETECTOR_44_38,
	DETECTOR_44_62,
	DETECTOR_44_88,
	DETECTOR_44_92,
	DETECTOR_44_110,
	DETECTOR_44_142,
	DETECTOR_43_4,
	DETECTOR_43_5,
        NUM_DETECTOR_MODELS
} DETECTOR_NUMBER;


typedef struct _DETECTOR {
    DETECTOR_NUMBER model;
    int             HV;
    int             threshold;
    float           uR_hr_CPS;
} Detector;

void initializeDefaultDetector(Detector *Default_Detector);
Detector newDetector(DETECTOR_NUMBER model, Detector *Default_Detector);   // Creates a new detector with default values for the chosen model

#endif	/* DETECTOR_H */

