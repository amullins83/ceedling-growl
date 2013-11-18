#ifndef ALL_CORNERS_H
#define ALL_CORNERS_H

#include "sample_lookup.h"

void sampleLookupAllCorners(SampleLookup **sl, int numInstruments, void (*callback)(SampleLookup **, int, int, int));

#endif