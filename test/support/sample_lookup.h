#ifndef SAMPLE_LOOKUP
#define SAMPLE_LOOKUP

#include "reading_calculation.h"

    typedef struct
    {
        float reading;
        float volts;
    } ReadingVoltsPair;

    typedef struct
    {
        InstrumentConstants *instrument;
        ReadingVoltsPair **range;
        int *numPairs;
        int numPairsCapacity;
    } SampleLookup;

    #define SAMPLE_LOOKUP_DEFAULT_CAPACITY 10
    #define NUM_RANGES 5

    SampleLookup *newSampleLookup(InstrumentConstants *instrument);
    void sampleLookupAddPair(SampleLookup *sl, int range, float correct_reading, float input_volts);
    void sampleLookupEditPair(SampleLookup *sl, int range, int pair, float correct_reading, float input_volts);
    void sampleLookupDeletePair(SampleLookup *sl, int range, int pair);
    void deleteSampleLookup(SampleLookup *sl);

#endif