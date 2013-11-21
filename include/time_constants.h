#ifndef TIME_CONSTANTS_H
#define TIME_CONSTANTS_H

#include "GenericTypeDefs.h"

typedef struct
{
    int sampleRate;
    float minTime;
    float maxTime;
    BOOL  fixedTime;
    float fixedTimeConstant;
    int   minTimeThreshold;
    int   maxTimeThreshold;
} TimeConstants;


TimeConstants *getDefaultTimeConstants(void);
TimeConstants getCurrentTimeConstants(void);

#endif
