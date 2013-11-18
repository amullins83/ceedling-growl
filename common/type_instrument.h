#ifndef TYPE_INSTRUMENT_H
#define TYPE_INSTRUMENT_H

#ifdef THIS_IS_MODEL_9DP
    #include "M9DP_Config.h"
#endif

#include "Definitions.h"


typedef struct _INSTRUMENT
{
    TYPE_SETTINGS settings;
    TYPE_FEATURES features;
    TYPE_CONTROLS controls;
    struct
    {
        union
        {
            TYPE_DISPLAY_PROPERTIES info;
            unsigned char buffer[SIZE_DISPLAY_PROPERTIES];
        } properties;
        struct
        {
            union
            {
                TYPE_DETECTOR_PROPERTIES info;
                unsigned char buffer[SIZE_DETECTOR_PROPERTIES];
            } properties;
            union
            {
                TYPE_DETECTOR_CALIBRATION info;
                unsigned char buffer[SIZE_DETECTOR_CALIBRATION];
            } calibration;
            union
            {
                TYPE_DETECTOR_VIEW_SETUP info;
                unsigned char buffer[SIZE_DETECTOR_VIEW_SETUP];
            } view_setup;
            TYPE_VIEW view[NUMBER_OF_VIEWS];
        } detector[NUMBER_OF_DETECTORS];
    } display;
    TYPE_FUNCTION function[NUMBER_OF_FUNCTIONS];
    TYPE_ALERTS alerts;
    TYPE_ALARMS alarms;
    TYPE_CALIBRATION calibration;
} TYPE_INSTRUMENT;

#endif
