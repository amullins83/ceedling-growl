#include "unity.h"
#include "expected_conversion.h"
#include "sample_lookup.h"
#include "instrument_constants.h"
#include "reading_calculation.h"
#include "verbose_assert_float.h"
#include <math.h>

float deltaForRange(int r) {
    return 26*pow(10, r);
}

void expectedConversion(SampleLookup **sl, int instrument, int range, int pair) {
    static int current_instrument = 0;
    if(instrument != current_instrument) {
        printf("%s: %d\n", "ID", sl[instrument]->instrument->id);
        current_instrument = instrument;
    }
    float expected, actual, delta;
    instrumentConstantsSetCurrentRange(sl[instrument]->instrument, range + 1);
    expected = sl[instrument]->range[range][pair].reading;
    actual = readingFromVolts(sl[instrument]->range[range][pair].volts, sl[instrument]->instrument);
    delta = deltaForRange(range);
    
    verboseAssertFloatWithin(delta, expected, actual);
}
