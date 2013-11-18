#include "unity.h"
#include "expected_conversion.h"
#include "sample_lookup.h"
#include "instrument_constants.h"
#include "reading_calculation.h"
#include <math.h>

float deltaForRange(int r) {
    return 2.6*pow(10, r + 1);
}

void expectedConversion(SampleLookup **sl, int instrument, int range, int pair) {
    float expected, actual, delta;
    instrumentConstantsSetCurrentRange(sl[instrument]->instrument, range + 1);
    expected = sl[instrument]->range[range][pair].reading;
    actual = readingFromVolts(sl[instrument]->range[range][pair].volts, sl[instrument]->instrument);
    delta = deltaForRange(range);
    if((expected - actual) > delta || (actual - expected) > delta)
        printf("%s: %d, %s: %f, %s: %f, %s: %f\n", "ID", sl[instrument]->instrument->id, "Delta", delta, "Expected", expected, "Actual", actual);
    TEST_ASSERT_FLOAT_WITHIN(delta, expected, actual);
}
