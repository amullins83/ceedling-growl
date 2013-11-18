#ifndef MOCK_TYPE_INSTRUMENTS_COMPONENTS_H
#define MOCK_TYPE_INSTRUMENTS_COMPONENTS_H

#include "models.h"

#define NUMBER_OF_FUNCTIONS 5
#define NUMBER_OF_DETECTORS 1
#define NUMBER_OF_VIEWS     5

#define SIZE_DISPLAY_PROPERTIES 4
#define SIZE_DETECTOR_PROPERTIES 4
#define SIZE_DETECTOR_CALIBRATION 4
#define SIZE_DETECTOR_VIEW_SETUP 4

typedef struct {
    struct {
        struct
        {
            int fixed_range;
        } info;
    } controls;
} TYPE_CALIBRATION;

typedef int TYPE_VIEW;
typedef int TYPE_FUNCTION;
typedef int TYPE_ALERTS;
typedef int TYPE_ALARMS;

typedef struct {
  struct {
    struct {
        MODEL_NUMBERS model_number;
    } info;
  } product;
} TYPE_SETTINGS;

typedef int TYPE_FEATURES;
typedef int TYPE_CONTROLS;
typedef int TYPE_DISPLAY_PROPERTIES;

typedef int TYPE_DETECTOR_PROPERTIES;

typedef int TYPE_DETECTOR_CALIBRATION;

typedef int TYPE_DETECTOR_VIEW_SETUP;


#endif
