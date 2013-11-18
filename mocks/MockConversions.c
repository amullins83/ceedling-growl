/* AUTOGENERATED FILE. DO NOT EDIT. */
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "unity.h"
#include "cmock.h"
#include "MockConversions.h"

typedef struct _CMOCK_FloatValueConversion_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  float ReturnVal;
  float Expected_value;
  unsigned char Expected_multiplier;

} CMOCK_FloatValueConversion_CALL_INSTANCE;

typedef struct _CMOCK_ShortValueConversion_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  float ReturnVal;
  unsigned short Expected_value;
  unsigned char Expected_multiplier;

} CMOCK_ShortValueConversion_CALL_INSTANCE;

typedef struct _CMOCK_RelativeMultiplier_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  short ReturnVal;
  short Expected_measurement_multiplier;
  short Expected_event_multiplier;

} CMOCK_RelativeMultiplier_CALL_INSTANCE;

typedef struct _CMOCK_MultiplierConversion_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  float ReturnVal;
  unsigned char Expected_multiplier;

} CMOCK_MultiplierConversion_CALL_INSTANCE;

typedef struct _CMOCK_DateStringConversion_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  TYPE_DATE_FORMAT* Expected_datepointer;
  BYTE* Expected_datestringpointer;

} CMOCK_DateStringConversion_CALL_INSTANCE;

typedef struct _CMOCK_TimeStringConversion_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  TYPE_TIME_FORMAT* Expected_timepointer;
  BYTE* Expected_timestringpointer;

} CMOCK_TimeStringConversion_CALL_INSTANCE;

typedef struct _CMOCK_UnitConversions_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  float ReturnVal;
  float Expected_old_value;
  BYTE Expected_old_units;
  BYTE Expected_new_units;

} CMOCK_UnitConversions_CALL_INSTANCE;

typedef struct _CMOCK_RateToDoseConversion_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  BYTE ReturnVal;
  BYTE Expected_rate_units;

} CMOCK_RateToDoseConversion_CALL_INSTANCE;

static struct MockConversionsInstance
{
  CMOCK_MEM_INDEX_TYPE FloatValueConversion_CallInstance;
  CMOCK_MEM_INDEX_TYPE ShortValueConversion_CallInstance;
  CMOCK_MEM_INDEX_TYPE RelativeMultiplier_CallInstance;
  CMOCK_MEM_INDEX_TYPE MultiplierConversion_CallInstance;
  CMOCK_MEM_INDEX_TYPE DateStringConversion_CallInstance;
  CMOCK_MEM_INDEX_TYPE TimeStringConversion_CallInstance;
  CMOCK_MEM_INDEX_TYPE UnitConversions_CallInstance;
  CMOCK_MEM_INDEX_TYPE RateToDoseConversion_CallInstance;
} Mock;

extern jmp_buf AbortFrame;

void MockConversions_Verify(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.FloatValueConversion_CallInstance, cmock_line, "Function 'FloatValueConversion' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.ShortValueConversion_CallInstance, cmock_line, "Function 'ShortValueConversion' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.RelativeMultiplier_CallInstance, cmock_line, "Function 'RelativeMultiplier' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.MultiplierConversion_CallInstance, cmock_line, "Function 'MultiplierConversion' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.DateStringConversion_CallInstance, cmock_line, "Function 'DateStringConversion' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.TimeStringConversion_CallInstance, cmock_line, "Function 'TimeStringConversion' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.UnitConversions_CallInstance, cmock_line, "Function 'UnitConversions' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.RateToDoseConversion_CallInstance, cmock_line, "Function 'RateToDoseConversion' called less times than expected.");
}

void MockConversions_Init(void)
{
  MockConversions_Destroy();
}

void MockConversions_Destroy(void)
{
  CMock_Guts_MemFreeAll();
  memset(&Mock, 0, sizeof(Mock));
}

float FloatValueConversion(float value, unsigned char multiplier)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_FloatValueConversion_CALL_INSTANCE* cmock_call_instance = (CMOCK_FloatValueConversion_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.FloatValueConversion_CallInstance);
  Mock.FloatValueConversion_CallInstance = CMock_Guts_MemNext(Mock.FloatValueConversion_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'FloatValueConversion' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_FLOAT(cmock_call_instance->Expected_value, value, cmock_line, "Function 'FloatValueConversion' called with unexpected value for argument 'value'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_HEX8(cmock_call_instance->Expected_multiplier, multiplier, cmock_line, "Function 'FloatValueConversion' called with unexpected value for argument 'multiplier'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_FloatValueConversion(CMOCK_FloatValueConversion_CALL_INSTANCE* cmock_call_instance, float value, unsigned char multiplier)
{
  cmock_call_instance->Expected_value = value;
  cmock_call_instance->Expected_multiplier = multiplier;
}

void FloatValueConversion_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, float value, unsigned char multiplier, float cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_FloatValueConversion_CALL_INSTANCE));
  CMOCK_FloatValueConversion_CALL_INSTANCE* cmock_call_instance = (CMOCK_FloatValueConversion_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.FloatValueConversion_CallInstance = CMock_Guts_MemChain(Mock.FloatValueConversion_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_FloatValueConversion(cmock_call_instance, value, multiplier);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

float ShortValueConversion(unsigned short value, unsigned char multiplier)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_ShortValueConversion_CALL_INSTANCE* cmock_call_instance = (CMOCK_ShortValueConversion_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.ShortValueConversion_CallInstance);
  Mock.ShortValueConversion_CallInstance = CMock_Guts_MemNext(Mock.ShortValueConversion_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'ShortValueConversion' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_HEX16(cmock_call_instance->Expected_value, value, cmock_line, "Function 'ShortValueConversion' called with unexpected value for argument 'value'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_HEX8(cmock_call_instance->Expected_multiplier, multiplier, cmock_line, "Function 'ShortValueConversion' called with unexpected value for argument 'multiplier'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_ShortValueConversion(CMOCK_ShortValueConversion_CALL_INSTANCE* cmock_call_instance, unsigned short value, unsigned char multiplier)
{
  cmock_call_instance->Expected_value = value;
  cmock_call_instance->Expected_multiplier = multiplier;
}

void ShortValueConversion_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, unsigned short value, unsigned char multiplier, float cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_ShortValueConversion_CALL_INSTANCE));
  CMOCK_ShortValueConversion_CALL_INSTANCE* cmock_call_instance = (CMOCK_ShortValueConversion_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.ShortValueConversion_CallInstance = CMock_Guts_MemChain(Mock.ShortValueConversion_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_ShortValueConversion(cmock_call_instance, value, multiplier);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

short RelativeMultiplier(short measurement_multiplier, short event_multiplier)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_RelativeMultiplier_CALL_INSTANCE* cmock_call_instance = (CMOCK_RelativeMultiplier_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.RelativeMultiplier_CallInstance);
  Mock.RelativeMultiplier_CallInstance = CMock_Guts_MemNext(Mock.RelativeMultiplier_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'RelativeMultiplier' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_INT16(cmock_call_instance->Expected_measurement_multiplier, measurement_multiplier, cmock_line, "Function 'RelativeMultiplier' called with unexpected value for argument 'measurement_multiplier'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_INT16(cmock_call_instance->Expected_event_multiplier, event_multiplier, cmock_line, "Function 'RelativeMultiplier' called with unexpected value for argument 'event_multiplier'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_RelativeMultiplier(CMOCK_RelativeMultiplier_CALL_INSTANCE* cmock_call_instance, short measurement_multiplier, short event_multiplier)
{
  cmock_call_instance->Expected_measurement_multiplier = measurement_multiplier;
  cmock_call_instance->Expected_event_multiplier = event_multiplier;
}

void RelativeMultiplier_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, short measurement_multiplier, short event_multiplier, short cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_RelativeMultiplier_CALL_INSTANCE));
  CMOCK_RelativeMultiplier_CALL_INSTANCE* cmock_call_instance = (CMOCK_RelativeMultiplier_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.RelativeMultiplier_CallInstance = CMock_Guts_MemChain(Mock.RelativeMultiplier_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_RelativeMultiplier(cmock_call_instance, measurement_multiplier, event_multiplier);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

float MultiplierConversion(unsigned char multiplier)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_MultiplierConversion_CALL_INSTANCE* cmock_call_instance = (CMOCK_MultiplierConversion_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.MultiplierConversion_CallInstance);
  Mock.MultiplierConversion_CallInstance = CMock_Guts_MemNext(Mock.MultiplierConversion_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'MultiplierConversion' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_HEX8(cmock_call_instance->Expected_multiplier, multiplier, cmock_line, "Function 'MultiplierConversion' called with unexpected value for argument 'multiplier'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_MultiplierConversion(CMOCK_MultiplierConversion_CALL_INSTANCE* cmock_call_instance, unsigned char multiplier)
{
  cmock_call_instance->Expected_multiplier = multiplier;
}

void MultiplierConversion_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, unsigned char multiplier, float cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_MultiplierConversion_CALL_INSTANCE));
  CMOCK_MultiplierConversion_CALL_INSTANCE* cmock_call_instance = (CMOCK_MultiplierConversion_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.MultiplierConversion_CallInstance = CMock_Guts_MemChain(Mock.MultiplierConversion_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_MultiplierConversion(cmock_call_instance, multiplier);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void DateStringConversion(TYPE_DATE_FORMAT* datepointer, BYTE* datestringpointer)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_DateStringConversion_CALL_INSTANCE* cmock_call_instance = (CMOCK_DateStringConversion_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.DateStringConversion_CallInstance);
  Mock.DateStringConversion_CallInstance = CMock_Guts_MemNext(Mock.DateStringConversion_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'DateStringConversion' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(cmock_call_instance->Expected_datepointer), (void*)(datepointer), sizeof(TYPE_DATE_FORMAT), cmock_line, "Function 'DateStringConversion' called with unexpected value for argument 'datepointer'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(cmock_call_instance->Expected_datestringpointer), (void*)(datestringpointer), sizeof(BYTE), cmock_line, "Function 'DateStringConversion' called with unexpected value for argument 'datestringpointer'.");
  }
}

void CMockExpectParameters_DateStringConversion(CMOCK_DateStringConversion_CALL_INSTANCE* cmock_call_instance, TYPE_DATE_FORMAT* datepointer, BYTE* datestringpointer)
{
  cmock_call_instance->Expected_datepointer = datepointer;
  cmock_call_instance->Expected_datestringpointer = datestringpointer;
}

void DateStringConversion_CMockExpect(UNITY_LINE_TYPE cmock_line, TYPE_DATE_FORMAT* datepointer, BYTE* datestringpointer)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_DateStringConversion_CALL_INSTANCE));
  CMOCK_DateStringConversion_CALL_INSTANCE* cmock_call_instance = (CMOCK_DateStringConversion_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.DateStringConversion_CallInstance = CMock_Guts_MemChain(Mock.DateStringConversion_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_DateStringConversion(cmock_call_instance, datepointer, datestringpointer);
}

void TimeStringConversion(TYPE_TIME_FORMAT* timepointer, BYTE* timestringpointer)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_TimeStringConversion_CALL_INSTANCE* cmock_call_instance = (CMOCK_TimeStringConversion_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.TimeStringConversion_CallInstance);
  Mock.TimeStringConversion_CallInstance = CMock_Guts_MemNext(Mock.TimeStringConversion_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'TimeStringConversion' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(cmock_call_instance->Expected_timepointer), (void*)(timepointer), sizeof(TYPE_TIME_FORMAT), cmock_line, "Function 'TimeStringConversion' called with unexpected value for argument 'timepointer'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(cmock_call_instance->Expected_timestringpointer), (void*)(timestringpointer), sizeof(BYTE), cmock_line, "Function 'TimeStringConversion' called with unexpected value for argument 'timestringpointer'.");
  }
}

void CMockExpectParameters_TimeStringConversion(CMOCK_TimeStringConversion_CALL_INSTANCE* cmock_call_instance, TYPE_TIME_FORMAT* timepointer, BYTE* timestringpointer)
{
  cmock_call_instance->Expected_timepointer = timepointer;
  cmock_call_instance->Expected_timestringpointer = timestringpointer;
}

void TimeStringConversion_CMockExpect(UNITY_LINE_TYPE cmock_line, TYPE_TIME_FORMAT* timepointer, BYTE* timestringpointer)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_TimeStringConversion_CALL_INSTANCE));
  CMOCK_TimeStringConversion_CALL_INSTANCE* cmock_call_instance = (CMOCK_TimeStringConversion_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.TimeStringConversion_CallInstance = CMock_Guts_MemChain(Mock.TimeStringConversion_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_TimeStringConversion(cmock_call_instance, timepointer, timestringpointer);
}

float UnitConversions(float old_value, BYTE old_units, BYTE new_units)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_UnitConversions_CALL_INSTANCE* cmock_call_instance = (CMOCK_UnitConversions_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.UnitConversions_CallInstance);
  Mock.UnitConversions_CallInstance = CMock_Guts_MemNext(Mock.UnitConversions_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'UnitConversions' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_FLOAT(cmock_call_instance->Expected_old_value, old_value, cmock_line, "Function 'UnitConversions' called with unexpected value for argument 'old_value'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_old_units), (void*)(&old_units), sizeof(BYTE), cmock_line, "Function 'UnitConversions' called with unexpected value for argument 'old_units'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_new_units), (void*)(&new_units), sizeof(BYTE), cmock_line, "Function 'UnitConversions' called with unexpected value for argument 'new_units'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_UnitConversions(CMOCK_UnitConversions_CALL_INSTANCE* cmock_call_instance, float old_value, BYTE old_units, BYTE new_units)
{
  cmock_call_instance->Expected_old_value = old_value;
  memcpy(&cmock_call_instance->Expected_old_units, &old_units, sizeof(BYTE));
  memcpy(&cmock_call_instance->Expected_new_units, &new_units, sizeof(BYTE));
}

void UnitConversions_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, float old_value, BYTE old_units, BYTE new_units, float cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_UnitConversions_CALL_INSTANCE));
  CMOCK_UnitConversions_CALL_INSTANCE* cmock_call_instance = (CMOCK_UnitConversions_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.UnitConversions_CallInstance = CMock_Guts_MemChain(Mock.UnitConversions_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_UnitConversions(cmock_call_instance, old_value, old_units, new_units);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

BYTE RateToDoseConversion(BYTE rate_units)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_RateToDoseConversion_CALL_INSTANCE* cmock_call_instance = (CMOCK_RateToDoseConversion_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.RateToDoseConversion_CallInstance);
  Mock.RateToDoseConversion_CallInstance = CMock_Guts_MemNext(Mock.RateToDoseConversion_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'RateToDoseConversion' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_rate_units), (void*)(&rate_units), sizeof(BYTE), cmock_line, "Function 'RateToDoseConversion' called with unexpected value for argument 'rate_units'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_RateToDoseConversion(CMOCK_RateToDoseConversion_CALL_INSTANCE* cmock_call_instance, BYTE rate_units)
{
  memcpy(&cmock_call_instance->Expected_rate_units, &rate_units, sizeof(BYTE));
}

void RateToDoseConversion_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, BYTE rate_units, BYTE cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_RateToDoseConversion_CALL_INSTANCE));
  CMOCK_RateToDoseConversion_CALL_INSTANCE* cmock_call_instance = (CMOCK_RateToDoseConversion_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.RateToDoseConversion_CallInstance = CMock_Guts_MemChain(Mock.RateToDoseConversion_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_RateToDoseConversion(cmock_call_instance, rate_units);
  memcpy(&cmock_call_instance->ReturnVal, &cmock_to_return, sizeof(BYTE));
}
