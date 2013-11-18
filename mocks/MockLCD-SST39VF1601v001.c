/* AUTOGENERATED FILE. DO NOT EDIT. */
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "unity.h"
#include "cmock.h"
#include "MockLCD-SST39VF1601v001.h"

typedef struct _CMOCK_SST39ChipErase_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;

} CMOCK_SST39ChipErase_CALL_INSTANCE;

typedef struct _CMOCK_SST39BlockErase_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  BYTE ReturnVal;
  DWORD Expected_address;

} CMOCK_SST39BlockErase_CALL_INSTANCE;

typedef struct _CMOCK_SST39WaitProgram_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;

} CMOCK_SST39WaitProgram_CALL_INSTANCE;

typedef struct _CMOCK_SST39SetAddress_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  DWORD Expected_address;

} CMOCK_SST39SetAddress_CALL_INSTANCE;

typedef struct _CMOCK_SST39WriteWord_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  BOOL ReturnVal;
  DWORD Expected_address;
  WORD Expected_data;

} CMOCK_SST39WriteWord_CALL_INSTANCE;

typedef struct _CMOCK_SST39WriteArray_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  BYTE ReturnVal;
  DWORD Expected_address;
  WORD* Expected_pData;
  WORD Expected_nCount;

} CMOCK_SST39WriteArray_CALL_INSTANCE;

typedef struct _CMOCK_SST39ReadWord_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  WORD ReturnVal;
  DWORD Expected_address;

} CMOCK_SST39ReadWord_CALL_INSTANCE;

typedef struct _CMOCK_SST39ReadFormedWord_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  WORD ReturnVal;
  DWORD Expected_address;

} CMOCK_SST39ReadFormedWord_CALL_INSTANCE;

typedef struct _CMOCK_SST39ReadArray_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  DWORD Expected_address;
  WORD* Expected_pData;
  WORD Expected_nCount;

} CMOCK_SST39ReadArray_CALL_INSTANCE;

typedef struct _CMOCK_SST39ReadWordArray_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  DWORD Expected_address;
  WORD* Expected_pData;
  WORD Expected_nCount;

} CMOCK_SST39ReadWordArray_CALL_INSTANCE;

typedef struct _CMOCK_Parse_Data_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  char* ReturnVal;
  char* Expected_data_string;

} CMOCK_Parse_Data_CALL_INSTANCE;

typedef struct _CMOCK_Image_Handler_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;

} CMOCK_Image_Handler_CALL_INSTANCE;

typedef struct _CMOCK_Terminate_Data_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  BYTE ReturnVal;
  char* Expected_message_string;

} CMOCK_Terminate_Data_CALL_INSTANCE;

typedef struct _CMOCK_ExtImage_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  BOOL ReturnVal;
  SHORT Expected_left;
  SHORT Expected_top;
  DWORD Expected_address;

} CMOCK_ExtImage_CALL_INSTANCE;

typedef struct _CMOCK_ExtImage8BPP_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  SHORT Expected_left;
  SHORT Expected_top;

} CMOCK_ExtImage8BPP_CALL_INSTANCE;

typedef struct _CMOCK_ExtImage16BPP_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  SHORT Expected_left;
  SHORT Expected_top;

} CMOCK_ExtImage16BPP_CALL_INSTANCE;

static struct MockLCD_SST39VF1601v001Instance
{
  CMOCK_MEM_INDEX_TYPE SST39ChipErase_CallInstance;
  CMOCK_MEM_INDEX_TYPE SST39BlockErase_CallInstance;
  CMOCK_MEM_INDEX_TYPE SST39WaitProgram_CallInstance;
  CMOCK_MEM_INDEX_TYPE SST39SetAddress_CallInstance;
  CMOCK_MEM_INDEX_TYPE SST39WriteWord_CallInstance;
  CMOCK_MEM_INDEX_TYPE SST39WriteArray_CallInstance;
  CMOCK_MEM_INDEX_TYPE SST39ReadWord_CallInstance;
  CMOCK_MEM_INDEX_TYPE SST39ReadFormedWord_CallInstance;
  CMOCK_MEM_INDEX_TYPE SST39ReadArray_CallInstance;
  CMOCK_MEM_INDEX_TYPE SST39ReadWordArray_CallInstance;
  CMOCK_MEM_INDEX_TYPE Parse_Data_CallInstance;
  CMOCK_MEM_INDEX_TYPE Image_Handler_CallInstance;
  CMOCK_MEM_INDEX_TYPE Terminate_Data_CallInstance;
  CMOCK_MEM_INDEX_TYPE ExtImage_CallInstance;
  CMOCK_MEM_INDEX_TYPE ExtImage8BPP_CallInstance;
  CMOCK_MEM_INDEX_TYPE ExtImage16BPP_CallInstance;
} Mock;

extern jmp_buf AbortFrame;

void MockLCD_SST39VF1601v001_Verify(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.SST39ChipErase_CallInstance, cmock_line, "Function 'SST39ChipErase' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.SST39BlockErase_CallInstance, cmock_line, "Function 'SST39BlockErase' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.SST39WaitProgram_CallInstance, cmock_line, "Function 'SST39WaitProgram' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.SST39SetAddress_CallInstance, cmock_line, "Function 'SST39SetAddress' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.SST39WriteWord_CallInstance, cmock_line, "Function 'SST39WriteWord' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.SST39WriteArray_CallInstance, cmock_line, "Function 'SST39WriteArray' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.SST39ReadWord_CallInstance, cmock_line, "Function 'SST39ReadWord' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.SST39ReadFormedWord_CallInstance, cmock_line, "Function 'SST39ReadFormedWord' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.SST39ReadArray_CallInstance, cmock_line, "Function 'SST39ReadArray' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.SST39ReadWordArray_CallInstance, cmock_line, "Function 'SST39ReadWordArray' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.Parse_Data_CallInstance, cmock_line, "Function 'Parse_Data' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.Image_Handler_CallInstance, cmock_line, "Function 'Image_Handler' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.Terminate_Data_CallInstance, cmock_line, "Function 'Terminate_Data' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.ExtImage_CallInstance, cmock_line, "Function 'ExtImage' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.ExtImage8BPP_CallInstance, cmock_line, "Function 'ExtImage8BPP' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.ExtImage16BPP_CallInstance, cmock_line, "Function 'ExtImage16BPP' called less times than expected.");
}

void MockLCD_SST39VF1601v001_Init(void)
{
  MockLCD_SST39VF1601v001_Destroy();
}

void MockLCD_SST39VF1601v001_Destroy(void)
{
  CMock_Guts_MemFreeAll();
  memset(&Mock, 0, sizeof(Mock));
}

void SST39ChipErase(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_SST39ChipErase_CALL_INSTANCE* cmock_call_instance = (CMOCK_SST39ChipErase_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.SST39ChipErase_CallInstance);
  Mock.SST39ChipErase_CallInstance = CMock_Guts_MemNext(Mock.SST39ChipErase_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'SST39ChipErase' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
}

void SST39ChipErase_CMockExpect(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_SST39ChipErase_CALL_INSTANCE));
  CMOCK_SST39ChipErase_CALL_INSTANCE* cmock_call_instance = (CMOCK_SST39ChipErase_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.SST39ChipErase_CallInstance = CMock_Guts_MemChain(Mock.SST39ChipErase_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
}

BYTE SST39BlockErase(DWORD address)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_SST39BlockErase_CALL_INSTANCE* cmock_call_instance = (CMOCK_SST39BlockErase_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.SST39BlockErase_CallInstance);
  Mock.SST39BlockErase_CallInstance = CMock_Guts_MemNext(Mock.SST39BlockErase_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'SST39BlockErase' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_address), (void*)(&address), sizeof(DWORD), cmock_line, "Function 'SST39BlockErase' called with unexpected value for argument 'address'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_SST39BlockErase(CMOCK_SST39BlockErase_CALL_INSTANCE* cmock_call_instance, DWORD address)
{
  memcpy(&cmock_call_instance->Expected_address, &address, sizeof(DWORD));
}

void SST39BlockErase_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, DWORD address, BYTE cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_SST39BlockErase_CALL_INSTANCE));
  CMOCK_SST39BlockErase_CALL_INSTANCE* cmock_call_instance = (CMOCK_SST39BlockErase_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.SST39BlockErase_CallInstance = CMock_Guts_MemChain(Mock.SST39BlockErase_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_SST39BlockErase(cmock_call_instance, address);
  memcpy(&cmock_call_instance->ReturnVal, &cmock_to_return, sizeof(BYTE));
}

void SST39WaitProgram(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_SST39WaitProgram_CALL_INSTANCE* cmock_call_instance = (CMOCK_SST39WaitProgram_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.SST39WaitProgram_CallInstance);
  Mock.SST39WaitProgram_CallInstance = CMock_Guts_MemNext(Mock.SST39WaitProgram_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'SST39WaitProgram' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
}

void SST39WaitProgram_CMockExpect(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_SST39WaitProgram_CALL_INSTANCE));
  CMOCK_SST39WaitProgram_CALL_INSTANCE* cmock_call_instance = (CMOCK_SST39WaitProgram_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.SST39WaitProgram_CallInstance = CMock_Guts_MemChain(Mock.SST39WaitProgram_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
}

void SST39SetAddress(DWORD address)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_SST39SetAddress_CALL_INSTANCE* cmock_call_instance = (CMOCK_SST39SetAddress_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.SST39SetAddress_CallInstance);
  Mock.SST39SetAddress_CallInstance = CMock_Guts_MemNext(Mock.SST39SetAddress_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'SST39SetAddress' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_address), (void*)(&address), sizeof(DWORD), cmock_line, "Function 'SST39SetAddress' called with unexpected value for argument 'address'.");
  }
}

void CMockExpectParameters_SST39SetAddress(CMOCK_SST39SetAddress_CALL_INSTANCE* cmock_call_instance, DWORD address)
{
  memcpy(&cmock_call_instance->Expected_address, &address, sizeof(DWORD));
}

void SST39SetAddress_CMockExpect(UNITY_LINE_TYPE cmock_line, DWORD address)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_SST39SetAddress_CALL_INSTANCE));
  CMOCK_SST39SetAddress_CALL_INSTANCE* cmock_call_instance = (CMOCK_SST39SetAddress_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.SST39SetAddress_CallInstance = CMock_Guts_MemChain(Mock.SST39SetAddress_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_SST39SetAddress(cmock_call_instance, address);
}

BOOL SST39WriteWord(DWORD address, WORD data)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_SST39WriteWord_CALL_INSTANCE* cmock_call_instance = (CMOCK_SST39WriteWord_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.SST39WriteWord_CallInstance);
  Mock.SST39WriteWord_CallInstance = CMock_Guts_MemNext(Mock.SST39WriteWord_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'SST39WriteWord' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_address), (void*)(&address), sizeof(DWORD), cmock_line, "Function 'SST39WriteWord' called with unexpected value for argument 'address'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_data), (void*)(&data), sizeof(WORD), cmock_line, "Function 'SST39WriteWord' called with unexpected value for argument 'data'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_SST39WriteWord(CMOCK_SST39WriteWord_CALL_INSTANCE* cmock_call_instance, DWORD address, WORD data)
{
  memcpy(&cmock_call_instance->Expected_address, &address, sizeof(DWORD));
  memcpy(&cmock_call_instance->Expected_data, &data, sizeof(WORD));
}

void SST39WriteWord_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, DWORD address, WORD data, BOOL cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_SST39WriteWord_CALL_INSTANCE));
  CMOCK_SST39WriteWord_CALL_INSTANCE* cmock_call_instance = (CMOCK_SST39WriteWord_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.SST39WriteWord_CallInstance = CMock_Guts_MemChain(Mock.SST39WriteWord_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_SST39WriteWord(cmock_call_instance, address, data);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

BYTE SST39WriteArray(DWORD address, WORD* pData, WORD nCount)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_SST39WriteArray_CALL_INSTANCE* cmock_call_instance = (CMOCK_SST39WriteArray_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.SST39WriteArray_CallInstance);
  Mock.SST39WriteArray_CallInstance = CMock_Guts_MemNext(Mock.SST39WriteArray_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'SST39WriteArray' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_address), (void*)(&address), sizeof(DWORD), cmock_line, "Function 'SST39WriteArray' called with unexpected value for argument 'address'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(cmock_call_instance->Expected_pData), (void*)(pData), sizeof(WORD), cmock_line, "Function 'SST39WriteArray' called with unexpected value for argument 'pData'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_nCount), (void*)(&nCount), sizeof(WORD), cmock_line, "Function 'SST39WriteArray' called with unexpected value for argument 'nCount'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_SST39WriteArray(CMOCK_SST39WriteArray_CALL_INSTANCE* cmock_call_instance, DWORD address, WORD* pData, WORD nCount)
{
  memcpy(&cmock_call_instance->Expected_address, &address, sizeof(DWORD));
  cmock_call_instance->Expected_pData = pData;
  memcpy(&cmock_call_instance->Expected_nCount, &nCount, sizeof(WORD));
}

void SST39WriteArray_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, DWORD address, WORD* pData, WORD nCount, BYTE cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_SST39WriteArray_CALL_INSTANCE));
  CMOCK_SST39WriteArray_CALL_INSTANCE* cmock_call_instance = (CMOCK_SST39WriteArray_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.SST39WriteArray_CallInstance = CMock_Guts_MemChain(Mock.SST39WriteArray_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_SST39WriteArray(cmock_call_instance, address, pData, nCount);
  memcpy(&cmock_call_instance->ReturnVal, &cmock_to_return, sizeof(BYTE));
}

WORD SST39ReadWord(DWORD address)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_SST39ReadWord_CALL_INSTANCE* cmock_call_instance = (CMOCK_SST39ReadWord_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.SST39ReadWord_CallInstance);
  Mock.SST39ReadWord_CallInstance = CMock_Guts_MemNext(Mock.SST39ReadWord_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'SST39ReadWord' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_address), (void*)(&address), sizeof(DWORD), cmock_line, "Function 'SST39ReadWord' called with unexpected value for argument 'address'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_SST39ReadWord(CMOCK_SST39ReadWord_CALL_INSTANCE* cmock_call_instance, DWORD address)
{
  memcpy(&cmock_call_instance->Expected_address, &address, sizeof(DWORD));
}

void SST39ReadWord_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, DWORD address, WORD cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_SST39ReadWord_CALL_INSTANCE));
  CMOCK_SST39ReadWord_CALL_INSTANCE* cmock_call_instance = (CMOCK_SST39ReadWord_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.SST39ReadWord_CallInstance = CMock_Guts_MemChain(Mock.SST39ReadWord_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_SST39ReadWord(cmock_call_instance, address);
  memcpy(&cmock_call_instance->ReturnVal, &cmock_to_return, sizeof(WORD));
}

WORD SST39ReadFormedWord(DWORD address)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_SST39ReadFormedWord_CALL_INSTANCE* cmock_call_instance = (CMOCK_SST39ReadFormedWord_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.SST39ReadFormedWord_CallInstance);
  Mock.SST39ReadFormedWord_CallInstance = CMock_Guts_MemNext(Mock.SST39ReadFormedWord_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'SST39ReadFormedWord' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_address), (void*)(&address), sizeof(DWORD), cmock_line, "Function 'SST39ReadFormedWord' called with unexpected value for argument 'address'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_SST39ReadFormedWord(CMOCK_SST39ReadFormedWord_CALL_INSTANCE* cmock_call_instance, DWORD address)
{
  memcpy(&cmock_call_instance->Expected_address, &address, sizeof(DWORD));
}

void SST39ReadFormedWord_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, DWORD address, WORD cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_SST39ReadFormedWord_CALL_INSTANCE));
  CMOCK_SST39ReadFormedWord_CALL_INSTANCE* cmock_call_instance = (CMOCK_SST39ReadFormedWord_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.SST39ReadFormedWord_CallInstance = CMock_Guts_MemChain(Mock.SST39ReadFormedWord_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_SST39ReadFormedWord(cmock_call_instance, address);
  memcpy(&cmock_call_instance->ReturnVal, &cmock_to_return, sizeof(WORD));
}

void SST39ReadArray(DWORD address, WORD* pData, WORD nCount)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_SST39ReadArray_CALL_INSTANCE* cmock_call_instance = (CMOCK_SST39ReadArray_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.SST39ReadArray_CallInstance);
  Mock.SST39ReadArray_CallInstance = CMock_Guts_MemNext(Mock.SST39ReadArray_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'SST39ReadArray' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_address), (void*)(&address), sizeof(DWORD), cmock_line, "Function 'SST39ReadArray' called with unexpected value for argument 'address'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(cmock_call_instance->Expected_pData), (void*)(pData), sizeof(WORD), cmock_line, "Function 'SST39ReadArray' called with unexpected value for argument 'pData'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_nCount), (void*)(&nCount), sizeof(WORD), cmock_line, "Function 'SST39ReadArray' called with unexpected value for argument 'nCount'.");
  }
}

void CMockExpectParameters_SST39ReadArray(CMOCK_SST39ReadArray_CALL_INSTANCE* cmock_call_instance, DWORD address, WORD* pData, WORD nCount)
{
  memcpy(&cmock_call_instance->Expected_address, &address, sizeof(DWORD));
  cmock_call_instance->Expected_pData = pData;
  memcpy(&cmock_call_instance->Expected_nCount, &nCount, sizeof(WORD));
}

void SST39ReadArray_CMockExpect(UNITY_LINE_TYPE cmock_line, DWORD address, WORD* pData, WORD nCount)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_SST39ReadArray_CALL_INSTANCE));
  CMOCK_SST39ReadArray_CALL_INSTANCE* cmock_call_instance = (CMOCK_SST39ReadArray_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.SST39ReadArray_CallInstance = CMock_Guts_MemChain(Mock.SST39ReadArray_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_SST39ReadArray(cmock_call_instance, address, pData, nCount);
}

void SST39ReadWordArray(DWORD address, WORD* pData, WORD nCount)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_SST39ReadWordArray_CALL_INSTANCE* cmock_call_instance = (CMOCK_SST39ReadWordArray_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.SST39ReadWordArray_CallInstance);
  Mock.SST39ReadWordArray_CallInstance = CMock_Guts_MemNext(Mock.SST39ReadWordArray_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'SST39ReadWordArray' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_address), (void*)(&address), sizeof(DWORD), cmock_line, "Function 'SST39ReadWordArray' called with unexpected value for argument 'address'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(cmock_call_instance->Expected_pData), (void*)(pData), sizeof(WORD), cmock_line, "Function 'SST39ReadWordArray' called with unexpected value for argument 'pData'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_nCount), (void*)(&nCount), sizeof(WORD), cmock_line, "Function 'SST39ReadWordArray' called with unexpected value for argument 'nCount'.");
  }
}

void CMockExpectParameters_SST39ReadWordArray(CMOCK_SST39ReadWordArray_CALL_INSTANCE* cmock_call_instance, DWORD address, WORD* pData, WORD nCount)
{
  memcpy(&cmock_call_instance->Expected_address, &address, sizeof(DWORD));
  cmock_call_instance->Expected_pData = pData;
  memcpy(&cmock_call_instance->Expected_nCount, &nCount, sizeof(WORD));
}

void SST39ReadWordArray_CMockExpect(UNITY_LINE_TYPE cmock_line, DWORD address, WORD* pData, WORD nCount)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_SST39ReadWordArray_CALL_INSTANCE));
  CMOCK_SST39ReadWordArray_CALL_INSTANCE* cmock_call_instance = (CMOCK_SST39ReadWordArray_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.SST39ReadWordArray_CallInstance = CMock_Guts_MemChain(Mock.SST39ReadWordArray_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_SST39ReadWordArray(cmock_call_instance, address, pData, nCount);
}

char* Parse_Data(char* data_string)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_Parse_Data_CALL_INSTANCE* cmock_call_instance = (CMOCK_Parse_Data_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.Parse_Data_CallInstance);
  Mock.Parse_Data_CallInstance = CMock_Guts_MemNext(Mock.Parse_Data_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'Parse_Data' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_STRING(cmock_call_instance->Expected_data_string, data_string, cmock_line, "Function 'Parse_Data' called with unexpected value for argument 'data_string'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_Parse_Data(CMOCK_Parse_Data_CALL_INSTANCE* cmock_call_instance, char* data_string)
{
  cmock_call_instance->Expected_data_string = data_string;
}

void Parse_Data_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, char* data_string, char* cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_Parse_Data_CALL_INSTANCE));
  CMOCK_Parse_Data_CALL_INSTANCE* cmock_call_instance = (CMOCK_Parse_Data_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.Parse_Data_CallInstance = CMock_Guts_MemChain(Mock.Parse_Data_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_Parse_Data(cmock_call_instance, data_string);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void Image_Handler(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_Image_Handler_CALL_INSTANCE* cmock_call_instance = (CMOCK_Image_Handler_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.Image_Handler_CallInstance);
  Mock.Image_Handler_CallInstance = CMock_Guts_MemNext(Mock.Image_Handler_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'Image_Handler' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
}

void Image_Handler_CMockExpect(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_Image_Handler_CALL_INSTANCE));
  CMOCK_Image_Handler_CALL_INSTANCE* cmock_call_instance = (CMOCK_Image_Handler_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.Image_Handler_CallInstance = CMock_Guts_MemChain(Mock.Image_Handler_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
}

BYTE Terminate_Data(char* message_string)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_Terminate_Data_CALL_INSTANCE* cmock_call_instance = (CMOCK_Terminate_Data_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.Terminate_Data_CallInstance);
  Mock.Terminate_Data_CallInstance = CMock_Guts_MemNext(Mock.Terminate_Data_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'Terminate_Data' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_STRING(cmock_call_instance->Expected_message_string, message_string, cmock_line, "Function 'Terminate_Data' called with unexpected value for argument 'message_string'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_Terminate_Data(CMOCK_Terminate_Data_CALL_INSTANCE* cmock_call_instance, char* message_string)
{
  cmock_call_instance->Expected_message_string = message_string;
}

void Terminate_Data_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, char* message_string, BYTE cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_Terminate_Data_CALL_INSTANCE));
  CMOCK_Terminate_Data_CALL_INSTANCE* cmock_call_instance = (CMOCK_Terminate_Data_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.Terminate_Data_CallInstance = CMock_Guts_MemChain(Mock.Terminate_Data_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_Terminate_Data(cmock_call_instance, message_string);
  memcpy(&cmock_call_instance->ReturnVal, &cmock_to_return, sizeof(BYTE));
}

BOOL ExtImage(SHORT left, SHORT top, DWORD address)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_ExtImage_CALL_INSTANCE* cmock_call_instance = (CMOCK_ExtImage_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.ExtImage_CallInstance);
  Mock.ExtImage_CallInstance = CMock_Guts_MemNext(Mock.ExtImage_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'ExtImage' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_left), (void*)(&left), sizeof(SHORT), cmock_line, "Function 'ExtImage' called with unexpected value for argument 'left'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_top), (void*)(&top), sizeof(SHORT), cmock_line, "Function 'ExtImage' called with unexpected value for argument 'top'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_address), (void*)(&address), sizeof(DWORD), cmock_line, "Function 'ExtImage' called with unexpected value for argument 'address'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_ExtImage(CMOCK_ExtImage_CALL_INSTANCE* cmock_call_instance, SHORT left, SHORT top, DWORD address)
{
  memcpy(&cmock_call_instance->Expected_left, &left, sizeof(SHORT));
  memcpy(&cmock_call_instance->Expected_top, &top, sizeof(SHORT));
  memcpy(&cmock_call_instance->Expected_address, &address, sizeof(DWORD));
}

void ExtImage_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, SHORT left, SHORT top, DWORD address, BOOL cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_ExtImage_CALL_INSTANCE));
  CMOCK_ExtImage_CALL_INSTANCE* cmock_call_instance = (CMOCK_ExtImage_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.ExtImage_CallInstance = CMock_Guts_MemChain(Mock.ExtImage_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_ExtImage(cmock_call_instance, left, top, address);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void ExtImage8BPP(SHORT left, SHORT top)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_ExtImage8BPP_CALL_INSTANCE* cmock_call_instance = (CMOCK_ExtImage8BPP_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.ExtImage8BPP_CallInstance);
  Mock.ExtImage8BPP_CallInstance = CMock_Guts_MemNext(Mock.ExtImage8BPP_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'ExtImage8BPP' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_left), (void*)(&left), sizeof(SHORT), cmock_line, "Function 'ExtImage8BPP' called with unexpected value for argument 'left'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_top), (void*)(&top), sizeof(SHORT), cmock_line, "Function 'ExtImage8BPP' called with unexpected value for argument 'top'.");
  }
}

void CMockExpectParameters_ExtImage8BPP(CMOCK_ExtImage8BPP_CALL_INSTANCE* cmock_call_instance, SHORT left, SHORT top)
{
  memcpy(&cmock_call_instance->Expected_left, &left, sizeof(SHORT));
  memcpy(&cmock_call_instance->Expected_top, &top, sizeof(SHORT));
}

void ExtImage8BPP_CMockExpect(UNITY_LINE_TYPE cmock_line, SHORT left, SHORT top)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_ExtImage8BPP_CALL_INSTANCE));
  CMOCK_ExtImage8BPP_CALL_INSTANCE* cmock_call_instance = (CMOCK_ExtImage8BPP_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.ExtImage8BPP_CallInstance = CMock_Guts_MemChain(Mock.ExtImage8BPP_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_ExtImage8BPP(cmock_call_instance, left, top);
}

void ExtImage16BPP(SHORT left, SHORT top)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_ExtImage16BPP_CALL_INSTANCE* cmock_call_instance = (CMOCK_ExtImage16BPP_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.ExtImage16BPP_CallInstance);
  Mock.ExtImage16BPP_CallInstance = CMock_Guts_MemNext(Mock.ExtImage16BPP_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'ExtImage16BPP' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_left), (void*)(&left), sizeof(SHORT), cmock_line, "Function 'ExtImage16BPP' called with unexpected value for argument 'left'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_top), (void*)(&top), sizeof(SHORT), cmock_line, "Function 'ExtImage16BPP' called with unexpected value for argument 'top'.");
  }
}

void CMockExpectParameters_ExtImage16BPP(CMOCK_ExtImage16BPP_CALL_INSTANCE* cmock_call_instance, SHORT left, SHORT top)
{
  memcpy(&cmock_call_instance->Expected_left, &left, sizeof(SHORT));
  memcpy(&cmock_call_instance->Expected_top, &top, sizeof(SHORT));
}

void ExtImage16BPP_CMockExpect(UNITY_LINE_TYPE cmock_line, SHORT left, SHORT top)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_ExtImage16BPP_CALL_INSTANCE));
  CMOCK_ExtImage16BPP_CALL_INSTANCE* cmock_call_instance = (CMOCK_ExtImage16BPP_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.ExtImage16BPP_CallInstance = CMock_Guts_MemChain(Mock.ExtImage16BPP_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_ExtImage16BPP(cmock_call_instance, left, top);
}
