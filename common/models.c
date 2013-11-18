#include "models.h"

//*
//**************************************************************************************************
//**************************************************************************************************
//* Function:
//*     unsigned char Check_Model_Number(unsigned char model) 
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
//*     (9DP v1.01.23)
//*
    
MODEL_NUMBER_TYPES Check_Model_Number(MODEL_NUMBERS model_number)
{
    MODEL_NUMBER_TYPES model_type = (MODEL_NUMBER_TYPES)0;

    switch(model_number)
    {
        case MODEL_9DLP:
        case MODEL_9DLP_PLUS:
        {
            model_type = MODEL_9DP_LOW_PRESSURE;
            break;
        }
        case MODEL_9DP:
        case MODEL_9DP_PLUS:
        case MODEL_9DP_STAR:
        case MODEL_9DP_STAR_PLUS:
        case MODEL_V1:
        {
            model_type = MODEL_9DP_HIGH_PRESSURE;
            break;
        }
        case MODEL_9DP_2:
        case MODEL_9DP_2_PLUS:
        {               
            model_type = MODEL_9DP_ULTRA_HIGH_PRESSURE;
            break;                                  
        }                                           
        case MODEL_9DP_1:
        case MODEL_9DP_1_PLUS:
        {
            model_type = MODEL_9DP_LOW_GAIN_LOW_PRESSURE;
            break;
        }
        case MODEL_3D:
        case MODEL_3D_PLUS:
        {
            model_type = MODEL_3D_NORMAL;
            break;
        }
    }

    return model_type;
}
