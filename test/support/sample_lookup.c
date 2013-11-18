#include "sample_lookup.h"
#include <stdlib.h>

ReadingVoltsPair **createNewRange(int pairsCapacity) {
    int current_range;
    ReadingVoltsPair **range = (ReadingVoltsPair **)malloc(sizeof(ReadingVoltsPair *)*NUM_RANGES);
    for(current_range = 0; current_range < NUM_RANGES; current_range++)
        range[current_range] = (ReadingVoltsPair *)malloc(sizeof(ReadingVoltsPair)*pairsCapacity);
    return range;
}

SampleLookup *newSampleLookup(InstrumentConstants *instrument) {
    int current_range;
    SampleLookup *newSL = (SampleLookup *)malloc(sizeof(SampleLookup));
    newSL->numPairs = (int *)malloc(sizeof(int)*NUM_RANGES);
    for(current_range = 0; current_range < NUM_RANGES; current_range++)
        newSL->numPairs[current_range] = 0;
    newSL->numPairsCapacity = SAMPLE_LOOKUP_DEFAULT_CAPACITY;
    newSL->range = createNewRange(newSL->numPairsCapacity);
    newSL->instrument = instrument;
    
    return newSL;
}

void sampleLookupCopyRangePairsTo(SampleLookup *sl, int range, ReadingVoltsPair *newPairs) {
    int i;
    for(i = 0; i < sl->numPairs[range]; i++)
        newPairs[i] = sl->range[range][i];
}

void sampleLookupCopyRangeTo(SampleLookup *sl, ReadingVoltsPair **newRange) {
    int current_range;
    for(current_range = 0; current_range < NUM_RANGES; current_range++)
        sampleLookupCopyRangePairsTo(sl, current_range, newRange[current_range]);
}

void sampleLookupDoubleCapacity(SampleLookup *sl) {
    ReadingVoltsPair **newRange = createNewRange(sl->numPairsCapacity*2);
    sampleLookupCopyRangeTo(sl, newRange);
    free(sl->range);
    sl->range = newRange;
    sl->numPairsCapacity *= 2;
}

void sampleLookupAddPair(SampleLookup *sl, int range, float correct_reading, float input_volts) {
    if(sl->numPairs[range] == sl->numPairsCapacity) // double capacity when cap reached
        sampleLookupDoubleCapacity(sl);

    ReadingVoltsPair *currentPair = &sl->range[range][sl->numPairs[range]];
    currentPair->reading = correct_reading;
    currentPair->volts = input_volts;
    sl->numPairs[range]++;
}

void sampleLookupEditPair(SampleLookup *sl, int range, int pair, float correct_reading, float input_volts) {
    sl->range[range][pair] = (ReadingVoltsPair){ correct_reading, input_volts};
}

void sampleLookupEditInstrument(SampleLookup *sl, InstrumentConstants *instrument) {
    sl->instrument = instrument;
}

void sampleLookupDeletePair(SampleLookup *sl, int range, int pair) {
    int i = pair + 1;
    for(; i < sl->numPairs[range]; i++)
        sl->range[range][i - 1] = sl->range[range][i];
    sl->numPairs[range]--;
}

void deleteSampleLookup(SampleLookup *sl) {
    free(sl->range);
    free(sl->numPairs);
    free(sl);
}
