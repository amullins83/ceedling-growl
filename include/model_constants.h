#ifndef MODEL_CONSTANTS_H
#define MODEL_CONSTANTS_H

    typedef struct
    {
        float chamber_output_low;
        float chamber_output_high;
        int   correction_factor;
        float over_range_voltage;
    } ModelConstants;

    ModelConstants *getModelConstants(void);

#endif
