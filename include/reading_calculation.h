#ifndef reading_calculation_H
#define reading_calculation_H

#include "GenericTypeDefs.h"
#include "instrument_constants.h"
#include "RunningAverage.h"
#include "time_constants.h"

UINT32 readADC(void);
void   setADC(UINT32 newADC);
float voltsFromADC(UINT32 reading);
float readingFromVolts(float volts, InstrumentConstants *instrument);
float readingAverage(float newReading, TimeConstants tc);
float getReadingAverage(void);
void  readingReset(void);
void  ReadingCalculation(void);

#endif
//reading_calculation_H
