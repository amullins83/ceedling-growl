#ifndef MODELS_H
#define MODELS_H

//**************************************************************************************************
//* Model Numbers
typedef enum _MODEL_NUMBERS
{
    NO_MODEL_NUMBER = 0,
    MODEL_9DP,
    MODEL_9DP_PLUS,
    MODEL_9DP_1,
    MODEL_9DP_1_PLUS,                                                                                   
    MODEL_9DLP,
    MODEL_9DLP_PLUS,
    MODEL_3D,
    MODEL_3D_PLUS,
    MODEL_276D,
    MODEL_V1,                                                                                       // model number for RTI (9DP v1.01.21)
    MODEL_9DP_STAR,                                                                                 // model 9DP* (9DP v1.01.23)
    MODEL_9DP_STAR_PLUS,                                                                            // model 9DP*+ (9DP v1.01.23)
    MODEL_9DP_2,                                                                                    // model 9DP-2 (9DP v1.01.23)
    MODEL_9DP_2_PLUS                                                                                // model 9DP-2+ (9DP v1.01.23)
} MODEL_NUMBERS;

//**************************************************************************************************
//* Model Number Types

typedef enum _MODEL_NUMBER_TYPES
{
    NO_MODEL_NUMBER_TYPE = 0,
    MODEL_9DP_LOW_PRESSURE,
    MODEL_9DP_HIGH_PRESSURE,
    MODEL_9DP_ULTRA_HIGH_PRESSURE,
    MODEL_9DP_LOW_GAIN_LOW_PRESSURE,
    MODEL_3D_NORMAL
} MODEL_NUMBER_TYPES;


MODEL_NUMBER_TYPES Check_Model_Number(MODEL_NUMBERS model_number);

#endif
