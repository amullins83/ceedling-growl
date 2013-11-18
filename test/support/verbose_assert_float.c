#include "unity.h"
#include "verbose_assert_float.h"

void verboseAssertFloat(float expected, float actual) {
    verboseAssertFloatWithin(0.001, expected, actual);
}

void verboseAssertFloatWithin(float delta, float expected, float actual) {
    if( expected - actual > delta || actual - expected > delta )
        printf("%s: %f, %s: %f, %s: %f\n", "Expected", expected, "Actual", actual, "Delta", delta);
    TEST_ASSERT_FLOAT_WITHIN(delta, expected, actual);
}
