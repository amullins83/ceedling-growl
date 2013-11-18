#include "sample_lookup_all_corners.h"

void sampleLookupAllCorners(SampleLookup **sl, int numInstruments, void (*callback)(SampleLookup **, int, int, int)) {
    int i, range, pair;
    for(i = 0; i < numInstruments; i++)
        for(range = 0; range < NUM_RANGES; range++)
            for(pair = 0; pair < sl[i]->numPairs[range]; pair++)
                callback(sl, i, range, pair);
}
