/* AUTOGENERATED FILE. DO NOT EDIT. */
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "unity.h"
#include "cmock.h"
#include "MockDelay.h"

typedef struct _CMOCK_Delay10us_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  DWORD Expected_dwCount;

} CMOCK_Delay10us_CALL_INSTANCE;

typedef struct _CMOCK_DelayMs_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  WORD Expected_ms;

} CMOCK_DelayMs_CALL_INSTANCE;

static struct MockDelayInstance
{
  CMOCK_MEM_INDEX_TYPE Delay10us_CallInstance;
  CMOCK_MEM_INDEX_TYPE DelayMs_CallInstance;
} Mock;

extern jmp_buf AbortFrame;

void MockDelay_Verify(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.Delay10us_CallInstance, cmock_line, "Function 'Delay10us' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.DelayMs_CallInstance, cmock_line, "Function 'DelayMs' called less times than expected.");
}

void MockDelay_Init(void)
{
  MockDelay_Destroy();
}

void MockDelay_Destroy(void)
{
  CMock_Guts_MemFreeAll();
  memset(&Mock, 0, sizeof(Mock));
}

void Delay10us(DWORD dwCount)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_Delay10us_CALL_INSTANCE* cmock_call_instance = (CMOCK_Delay10us_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.Delay10us_CallInstance);
  Mock.Delay10us_CallInstance = CMock_Guts_MemNext(Mock.Delay10us_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'Delay10us' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_dwCount), (void*)(&dwCount), sizeof(DWORD), cmock_line, "Function 'Delay10us' called with unexpected value for argument 'dwCount'.");
  }
}

void CMockExpectParameters_Delay10us(CMOCK_Delay10us_CALL_INSTANCE* cmock_call_instance, DWORD dwCount)
{
  memcpy(&cmock_call_instance->Expected_dwCount, &dwCount, sizeof(DWORD));
}

void Delay10us_CMockExpect(UNITY_LINE_TYPE cmock_line, DWORD dwCount)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_Delay10us_CALL_INSTANCE));
  CMOCK_Delay10us_CALL_INSTANCE* cmock_call_instance = (CMOCK_Delay10us_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.Delay10us_CallInstance = CMock_Guts_MemChain(Mock.Delay10us_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_Delay10us(cmock_call_instance, dwCount);
}

void DelayMs(WORD ms)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_DelayMs_CALL_INSTANCE* cmock_call_instance = (CMOCK_DelayMs_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.DelayMs_CallInstance);
  Mock.DelayMs_CallInstance = CMock_Guts_MemNext(Mock.DelayMs_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'DelayMs' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_ms), (void*)(&ms), sizeof(WORD), cmock_line, "Function 'DelayMs' called with unexpected value for argument 'ms'.");
  }
}

void CMockExpectParameters_DelayMs(CMOCK_DelayMs_CALL_INSTANCE* cmock_call_instance, WORD ms)
{
  memcpy(&cmock_call_instance->Expected_ms, &ms, sizeof(WORD));
}

void DelayMs_CMockExpect(UNITY_LINE_TYPE cmock_line, WORD ms)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_DelayMs_CALL_INSTANCE));
  CMOCK_DelayMs_CALL_INSTANCE* cmock_call_instance = (CMOCK_DelayMs_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.DelayMs_CallInstance = CMock_Guts_MemChain(Mock.DelayMs_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_DelayMs(cmock_call_instance, ms);
}
