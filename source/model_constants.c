#include "model_constants.h"

ModelConstants *getModelConstants()
{
    static ModelConstants constants[] = {
        (ModelConstants){
            //No model number
            0.0,
            0.0,
            0,
            2.5
        },                                                                                                         // (v1.01.24)
        // //case MODEL_9DP_LOW_PRESSURE:                                                                                // (v1.01.24)
        (ModelConstants){                                                                                                           // (v1.01.24)
            0.030, //chamber_output_low                                                                             // (v1.01.24)
            0.000307, //chamber_output_high                                                                         // (v1.01.24)
            1, //correction_factor                                                                                  // (v1.01.24)
            2.0 //over_range_voltage                                                                               // (v1.01.24)                                                                                                  // (v1.01.24)
        },                                                                                                          // (v1.01.24)
        //case MODEL_9DP_HIGH_PRESSURE:                                                                               // (v1.01.24)
        (ModelConstants){                                                                                                           // (v1.01.24)
            0.060, //chamber_output_low                                                                             // (v1.01.24)
            0.000613, //chamber_output_high                                                                         // (v1.01.24)
            1, //correction_factor                                                                                  // (v1.01.24)
            2.0 //over_range_voltage                                                                               // (v1.01.24)                                                                                                  // (v1.01.24)
        },                                                                                                          // (v1.01.24)
        //case MODEL_9DP_ULTRA_HIGH_PRESSURE:                                                                         // (v1.01.24)
        (ModelConstants){                                                                                                           // (v1.01.24)
            0.075, //chamber_output_low                                                                             // (v1.01.24)
            0.000728, //chamber_output_high                                                                         // (v1.01.24)
            1, //correction_factor                                                                                  // (v1.01.24)
            2.2 //over_range_voltage                                                                               // (v1.01.24)                                                                                                  // (v1.01.24)
        },                                                                                                          // (v1.01.24)
        //case MODEL_9DP_LOW_GAIN_LOW_PRESSURE:                                                                       // (v1.01.24)
        (ModelConstants){                                                                                                           // (v1.01.24)
            0.00154, //chamber_output_low                                                                           // (v1.01.24)
            0.0000166, //chamber_output_high                                                                        // (v1.01.24)
            2, //correction_factor                                                                                  // (v1.01.24)
            2.0 //over_range_voltage                                                                               // (v1.01.24)                                                                                                  // (v1.01.24)
        },
        (ModelConstants){
            // MODEL_3D_NORMAL
            0.0,
            0.0,
            0,
            2.5
        }
    };

    return constants;
}
