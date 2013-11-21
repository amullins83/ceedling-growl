#include "unity.h"
#include "GenericTypeDefs.h"
#include "verbose_assert_float.h"

#define i2cADDR_X_TEMP_0x90 0x90
#define TEMP_REG 1
#define TEMP_CONST_12BIT 0.0625
#define FLOAT_TOL 0.1

float Temperature_Reading = 0.0;

BYTE READ_WORD_CALL_COUNT = 0;
BYTE READ_WORD_EXPECTED_CALL_COUNT = 0;
WORD READ_WORD_RETURN_VALUE = 0;

WORD i2c2ReadWord(DWORD address, DWORD reg, BYTE bytes) {
    READ_WORD_CALL_COUNT++;
    return READ_WORD_RETURN_VALUE;
}

void i2c2ReadWord_ExpectAndReturn(DWORD address, DWORD reg, BYTE bytes, WORD retVal) {
    READ_WORD_EXPECTED_CALL_COUNT++;
    READ_WORD_RETURN_VALUE = retVal;
}

void Check_Temperature(void)
{
    INT16 tempRead = (INT16)i2c2ReadWord(i2cADDR_X_TEMP_0x90, TEMP_REG, 1); // read i2c temperature sensor temperature register
    tempRead = tempRead >> 4; // bit shift right 4 (fast divide by 16)
    Temperature_Reading = tempRead * TEMP_CONST_12BIT; // scale with temperature constant ( output will be in range -128 to 128 exclusive )
}


void setUp(void) {
    READ_WORD_CALL_COUNT = READ_WORD_EXPECTED_CALL_COUNT = READ_WORD_RETURN_VALUE = 0;
    Temperature_Reading = 0.0;
}

void tearDown(void) {

}

void test_check_temperature_calls_i2c2ReadWord(void) {
    i2c2ReadWord_ExpectAndReturn(i2cADDR_X_TEMP_0x90, TEMP_REG, 1, 0xaaa);

    Check_Temperature();

    TEST_ASSERT_EQUAL(READ_WORD_EXPECTED_CALL_COUNT, READ_WORD_CALL_COUNT);
}

void test_check_temperature_positive_random(void) {
    i2c2ReadWord_ExpectAndReturn(i2cADDR_X_TEMP_0x90, TEMP_REG, 1, 0xaaa);

    Check_Temperature();    

    verboseAssertFloatWithin(FLOAT_TOL, 10.664, Temperature_Reading);
}

void test_check_temperature_positive_max(void) {
    i2c2ReadWord_ExpectAndReturn(i2cADDR_X_TEMP_0x90, TEMP_REG, 1, 0x7fff);

    Check_Temperature();    

    verboseAssertFloatWithin(FLOAT_TOL, 127.99, Temperature_Reading);
}


void test_check_temperature_zero(void) {
    i2c2ReadWord_ExpectAndReturn(i2cADDR_X_TEMP_0x90, TEMP_REG, 1, 0x0);

    Check_Temperature();    

    verboseAssertFloatWithin(FLOAT_TOL, 0.0, Temperature_Reading);
}

void test_check_temperature_negative_random(void) {
    i2c2ReadWord_ExpectAndReturn(i2cADDR_X_TEMP_0x90, TEMP_REG, 1, 0xff80);

    Check_Temperature();    

    verboseAssertFloatWithin(FLOAT_TOL, -0.5, Temperature_Reading);
}

void test_check_temperature_negative_max(void) {
    i2c2ReadWord_ExpectAndReturn(i2cADDR_X_TEMP_0x90, TEMP_REG, 1, 0x8000);

    Check_Temperature();    

    verboseAssertFloatWithin(FLOAT_TOL, -127.99, Temperature_Reading);
}
