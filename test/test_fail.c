#include "unity.h"

#define TRUE  1
#define FALSE 0

void setUp(void) {

}

void tearDown(void) {

}

void test_fail(void) {
    TEST_ASSERT_UNLESS(FALSE);
}
