/* AUTOGENERATED FILE. DO NOT EDIT. */
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "unity.h"
#include "cmock.h"
#include "MockBmpDecoder.h"

typedef struct _CMOCK_BMP_bDecode_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  BYTE ReturnVal;
  IMG_FILE* Expected_pFile;

} CMOCK_BMP_bDecode_CALL_INSTANCE;

static struct MockBmpDecoderInstance
{
  CMOCK_MEM_INDEX_TYPE BMP_bDecode_CallInstance;
} Mock;

extern jmp_buf AbortFrame;

void MockBmpDecoder_Verify(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.BMP_bDecode_CallInstance, cmock_line, "Function 'BMP_bDecode' called less times than expected.");
}

void MockBmpDecoder_Init(void)
{
  MockBmpDecoder_Destroy();
}

void MockBmpDecoder_Destroy(void)
{
  CMock_Guts_MemFreeAll();
  memset(&Mock, 0, sizeof(Mock));
}

BYTE BMP_bDecode(IMG_FILE* pFile)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_BMP_bDecode_CALL_INSTANCE* cmock_call_instance = (CMOCK_BMP_bDecode_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.BMP_bDecode_CallInstance);
  Mock.BMP_bDecode_CallInstance = CMock_Guts_MemNext(Mock.BMP_bDecode_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'BMP_bDecode' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(cmock_call_instance->Expected_pFile), (void*)(pFile), sizeof(IMG_FILE), cmock_line, "Function 'BMP_bDecode' called with unexpected value for argument 'pFile'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_BMP_bDecode(CMOCK_BMP_bDecode_CALL_INSTANCE* cmock_call_instance, IMG_FILE* pFile)
{
  cmock_call_instance->Expected_pFile = pFile;
}

void BMP_bDecode_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, IMG_FILE* pFile, BYTE cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_BMP_bDecode_CALL_INSTANCE));
  CMOCK_BMP_bDecode_CALL_INSTANCE* cmock_call_instance = (CMOCK_BMP_bDecode_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.BMP_bDecode_CallInstance = CMock_Guts_MemChain(Mock.BMP_bDecode_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_BMP_bDecode(cmock_call_instance, pFile);
  memcpy(&cmock_call_instance->ReturnVal, &cmock_to_return, sizeof(BYTE));
}

