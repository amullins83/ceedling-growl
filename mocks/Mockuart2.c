/* AUTOGENERATED FILE. DO NOT EDIT. */
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "unity.h"
#include "cmock.h"
#include "Mockuart2.h"

typedef struct _CMOCK_UART2GetBaudError_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  char ReturnVal;

} CMOCK_UART2GetBaudError_CALL_INSTANCE;

typedef struct _CMOCK_UART2GetChar_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  char ReturnVal;

} CMOCK_UART2GetChar_CALL_INSTANCE;

typedef struct _CMOCK_UART2PutChar_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  char Expected_ch;

} CMOCK_UART2PutChar_CALL_INSTANCE;

typedef struct _CMOCK_UART2Init_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;

} CMOCK_UART2Init_CALL_INSTANCE;

typedef struct _CMOCK_UART2IsPressed_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  char ReturnVal;

} CMOCK_UART2IsPressed_CALL_INSTANCE;

typedef struct _CMOCK_UART2PrintString_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  char* Expected_str;

} CMOCK_UART2PrintString_CALL_INSTANCE;

typedef struct _CMOCK_UART2PutDec_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  unsigned char Expected_dec;

} CMOCK_UART2PutDec_CALL_INSTANCE;

typedef struct _CMOCK_UART2PutHex_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  int Expected_toPrint;

} CMOCK_UART2PutHex_CALL_INSTANCE;

typedef struct _CMOCK_UART2PutHexWord_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  unsigned int Expected_toPrint;

} CMOCK_UART2PutHexWord_CALL_INSTANCE;

typedef struct _CMOCK_UART2PutHexDWord_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  unsigned long int Expected_toPrint;

} CMOCK_UART2PutHexDWord_CALL_INSTANCE;

typedef struct _CMOCK_UART2Char2Hex_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  char ReturnVal;
  char Expected_ch;

} CMOCK_UART2Char2Hex_CALL_INSTANCE;

typedef struct _CMOCK_UART2Hex2Char_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  char ReturnVal;
  char Expected_hex;

} CMOCK_UART2Hex2Char_CALL_INSTANCE;

typedef struct _CMOCK_UART2ClrError_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;

} CMOCK_UART2ClrError_CALL_INSTANCE;

static struct Mockuart2Instance
{
  CMOCK_MEM_INDEX_TYPE UART2GetBaudError_CallInstance;
  CMOCK_MEM_INDEX_TYPE UART2GetChar_CallInstance;
  CMOCK_MEM_INDEX_TYPE UART2PutChar_CallInstance;
  CMOCK_MEM_INDEX_TYPE UART2Init_CallInstance;
  CMOCK_MEM_INDEX_TYPE UART2IsPressed_CallInstance;
  CMOCK_MEM_INDEX_TYPE UART2PrintString_CallInstance;
  CMOCK_MEM_INDEX_TYPE UART2PutDec_CallInstance;
  CMOCK_MEM_INDEX_TYPE UART2PutHex_CallInstance;
  CMOCK_MEM_INDEX_TYPE UART2PutHexWord_CallInstance;
  CMOCK_MEM_INDEX_TYPE UART2PutHexDWord_CallInstance;
  CMOCK_MEM_INDEX_TYPE UART2Char2Hex_CallInstance;
  CMOCK_MEM_INDEX_TYPE UART2Hex2Char_CallInstance;
  CMOCK_MEM_INDEX_TYPE UART2ClrError_CallInstance;
} Mock;

extern jmp_buf AbortFrame;

void Mockuart2_Verify(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.UART2GetBaudError_CallInstance, cmock_line, "Function 'UART2GetBaudError' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.UART2GetChar_CallInstance, cmock_line, "Function 'UART2GetChar' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.UART2PutChar_CallInstance, cmock_line, "Function 'UART2PutChar' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.UART2Init_CallInstance, cmock_line, "Function 'UART2Init' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.UART2IsPressed_CallInstance, cmock_line, "Function 'UART2IsPressed' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.UART2PrintString_CallInstance, cmock_line, "Function 'UART2PrintString' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.UART2PutDec_CallInstance, cmock_line, "Function 'UART2PutDec' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.UART2PutHex_CallInstance, cmock_line, "Function 'UART2PutHex' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.UART2PutHexWord_CallInstance, cmock_line, "Function 'UART2PutHexWord' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.UART2PutHexDWord_CallInstance, cmock_line, "Function 'UART2PutHexDWord' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.UART2Char2Hex_CallInstance, cmock_line, "Function 'UART2Char2Hex' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.UART2Hex2Char_CallInstance, cmock_line, "Function 'UART2Hex2Char' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.UART2ClrError_CallInstance, cmock_line, "Function 'UART2ClrError' called less times than expected.");
}

void Mockuart2_Init(void)
{
  Mockuart2_Destroy();
}

void Mockuart2_Destroy(void)
{
  CMock_Guts_MemFreeAll();
  memset(&Mock, 0, sizeof(Mock));
}

char UART2GetBaudError(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_UART2GetBaudError_CALL_INSTANCE* cmock_call_instance = (CMOCK_UART2GetBaudError_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.UART2GetBaudError_CallInstance);
  Mock.UART2GetBaudError_CallInstance = CMock_Guts_MemNext(Mock.UART2GetBaudError_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'UART2GetBaudError' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  return cmock_call_instance->ReturnVal;
}

void UART2GetBaudError_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, char cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_UART2GetBaudError_CALL_INSTANCE));
  CMOCK_UART2GetBaudError_CALL_INSTANCE* cmock_call_instance = (CMOCK_UART2GetBaudError_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.UART2GetBaudError_CallInstance = CMock_Guts_MemChain(Mock.UART2GetBaudError_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
}

char UART2GetChar(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_UART2GetChar_CALL_INSTANCE* cmock_call_instance = (CMOCK_UART2GetChar_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.UART2GetChar_CallInstance);
  Mock.UART2GetChar_CallInstance = CMock_Guts_MemNext(Mock.UART2GetChar_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'UART2GetChar' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  return cmock_call_instance->ReturnVal;
}

void UART2GetChar_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, char cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_UART2GetChar_CALL_INSTANCE));
  CMOCK_UART2GetChar_CALL_INSTANCE* cmock_call_instance = (CMOCK_UART2GetChar_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.UART2GetChar_CallInstance = CMock_Guts_MemChain(Mock.UART2GetChar_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void UART2PutChar(char ch)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_UART2PutChar_CALL_INSTANCE* cmock_call_instance = (CMOCK_UART2PutChar_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.UART2PutChar_CallInstance);
  Mock.UART2PutChar_CallInstance = CMock_Guts_MemNext(Mock.UART2PutChar_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'UART2PutChar' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_INT8(cmock_call_instance->Expected_ch, ch, cmock_line, "Function 'UART2PutChar' called with unexpected value for argument 'ch'.");
  }
}

void CMockExpectParameters_UART2PutChar(CMOCK_UART2PutChar_CALL_INSTANCE* cmock_call_instance, char ch)
{
  cmock_call_instance->Expected_ch = ch;
}

void UART2PutChar_CMockExpect(UNITY_LINE_TYPE cmock_line, char ch)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_UART2PutChar_CALL_INSTANCE));
  CMOCK_UART2PutChar_CALL_INSTANCE* cmock_call_instance = (CMOCK_UART2PutChar_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.UART2PutChar_CallInstance = CMock_Guts_MemChain(Mock.UART2PutChar_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_UART2PutChar(cmock_call_instance, ch);
}

void UART2Init(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_UART2Init_CALL_INSTANCE* cmock_call_instance = (CMOCK_UART2Init_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.UART2Init_CallInstance);
  Mock.UART2Init_CallInstance = CMock_Guts_MemNext(Mock.UART2Init_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'UART2Init' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
}

void UART2Init_CMockExpect(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_UART2Init_CALL_INSTANCE));
  CMOCK_UART2Init_CALL_INSTANCE* cmock_call_instance = (CMOCK_UART2Init_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.UART2Init_CallInstance = CMock_Guts_MemChain(Mock.UART2Init_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
}

char UART2IsPressed(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_UART2IsPressed_CALL_INSTANCE* cmock_call_instance = (CMOCK_UART2IsPressed_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.UART2IsPressed_CallInstance);
  Mock.UART2IsPressed_CallInstance = CMock_Guts_MemNext(Mock.UART2IsPressed_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'UART2IsPressed' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  return cmock_call_instance->ReturnVal;
}

void UART2IsPressed_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, char cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_UART2IsPressed_CALL_INSTANCE));
  CMOCK_UART2IsPressed_CALL_INSTANCE* cmock_call_instance = (CMOCK_UART2IsPressed_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.UART2IsPressed_CallInstance = CMock_Guts_MemChain(Mock.UART2IsPressed_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void UART2PrintString(char* str)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_UART2PrintString_CALL_INSTANCE* cmock_call_instance = (CMOCK_UART2PrintString_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.UART2PrintString_CallInstance);
  Mock.UART2PrintString_CallInstance = CMock_Guts_MemNext(Mock.UART2PrintString_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'UART2PrintString' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_STRING(cmock_call_instance->Expected_str, str, cmock_line, "Function 'UART2PrintString' called with unexpected value for argument 'str'.");
  }
}

void CMockExpectParameters_UART2PrintString(CMOCK_UART2PrintString_CALL_INSTANCE* cmock_call_instance, char* str)
{
  cmock_call_instance->Expected_str = str;
}

void UART2PrintString_CMockExpect(UNITY_LINE_TYPE cmock_line, char* str)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_UART2PrintString_CALL_INSTANCE));
  CMOCK_UART2PrintString_CALL_INSTANCE* cmock_call_instance = (CMOCK_UART2PrintString_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.UART2PrintString_CallInstance = CMock_Guts_MemChain(Mock.UART2PrintString_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_UART2PrintString(cmock_call_instance, str);
}

void UART2PutDec(unsigned char dec)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_UART2PutDec_CALL_INSTANCE* cmock_call_instance = (CMOCK_UART2PutDec_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.UART2PutDec_CallInstance);
  Mock.UART2PutDec_CallInstance = CMock_Guts_MemNext(Mock.UART2PutDec_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'UART2PutDec' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_HEX8(cmock_call_instance->Expected_dec, dec, cmock_line, "Function 'UART2PutDec' called with unexpected value for argument 'dec'.");
  }
}

void CMockExpectParameters_UART2PutDec(CMOCK_UART2PutDec_CALL_INSTANCE* cmock_call_instance, unsigned char dec)
{
  cmock_call_instance->Expected_dec = dec;
}

void UART2PutDec_CMockExpect(UNITY_LINE_TYPE cmock_line, unsigned char dec)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_UART2PutDec_CALL_INSTANCE));
  CMOCK_UART2PutDec_CALL_INSTANCE* cmock_call_instance = (CMOCK_UART2PutDec_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.UART2PutDec_CallInstance = CMock_Guts_MemChain(Mock.UART2PutDec_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_UART2PutDec(cmock_call_instance, dec);
}

void UART2PutHex(int toPrint)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_UART2PutHex_CALL_INSTANCE* cmock_call_instance = (CMOCK_UART2PutHex_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.UART2PutHex_CallInstance);
  Mock.UART2PutHex_CallInstance = CMock_Guts_MemNext(Mock.UART2PutHex_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'UART2PutHex' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_INT(cmock_call_instance->Expected_toPrint, toPrint, cmock_line, "Function 'UART2PutHex' called with unexpected value for argument 'toPrint'.");
  }
}

void CMockExpectParameters_UART2PutHex(CMOCK_UART2PutHex_CALL_INSTANCE* cmock_call_instance, int toPrint)
{
  cmock_call_instance->Expected_toPrint = toPrint;
}

void UART2PutHex_CMockExpect(UNITY_LINE_TYPE cmock_line, int toPrint)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_UART2PutHex_CALL_INSTANCE));
  CMOCK_UART2PutHex_CALL_INSTANCE* cmock_call_instance = (CMOCK_UART2PutHex_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.UART2PutHex_CallInstance = CMock_Guts_MemChain(Mock.UART2PutHex_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_UART2PutHex(cmock_call_instance, toPrint);
}

void UART2PutHexWord(unsigned int toPrint)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_UART2PutHexWord_CALL_INSTANCE* cmock_call_instance = (CMOCK_UART2PutHexWord_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.UART2PutHexWord_CallInstance);
  Mock.UART2PutHexWord_CallInstance = CMock_Guts_MemNext(Mock.UART2PutHexWord_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'UART2PutHexWord' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_HEX32(cmock_call_instance->Expected_toPrint, toPrint, cmock_line, "Function 'UART2PutHexWord' called with unexpected value for argument 'toPrint'.");
  }
}

void CMockExpectParameters_UART2PutHexWord(CMOCK_UART2PutHexWord_CALL_INSTANCE* cmock_call_instance, unsigned int toPrint)
{
  cmock_call_instance->Expected_toPrint = toPrint;
}

void UART2PutHexWord_CMockExpect(UNITY_LINE_TYPE cmock_line, unsigned int toPrint)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_UART2PutHexWord_CALL_INSTANCE));
  CMOCK_UART2PutHexWord_CALL_INSTANCE* cmock_call_instance = (CMOCK_UART2PutHexWord_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.UART2PutHexWord_CallInstance = CMock_Guts_MemChain(Mock.UART2PutHexWord_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_UART2PutHexWord(cmock_call_instance, toPrint);
}

void UART2PutHexDWord(unsigned long int toPrint)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_UART2PutHexDWord_CALL_INSTANCE* cmock_call_instance = (CMOCK_UART2PutHexDWord_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.UART2PutHexDWord_CallInstance);
  Mock.UART2PutHexDWord_CallInstance = CMock_Guts_MemNext(Mock.UART2PutHexDWord_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'UART2PutHexDWord' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_toPrint), (void*)(&toPrint), sizeof(unsigned long int), cmock_line, "Function 'UART2PutHexDWord' called with unexpected value for argument 'toPrint'.");
  }
}

void CMockExpectParameters_UART2PutHexDWord(CMOCK_UART2PutHexDWord_CALL_INSTANCE* cmock_call_instance, unsigned long int toPrint)
{
  memcpy(&cmock_call_instance->Expected_toPrint, &toPrint, sizeof(unsigned long int));
}

void UART2PutHexDWord_CMockExpect(UNITY_LINE_TYPE cmock_line, unsigned long int toPrint)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_UART2PutHexDWord_CALL_INSTANCE));
  CMOCK_UART2PutHexDWord_CALL_INSTANCE* cmock_call_instance = (CMOCK_UART2PutHexDWord_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.UART2PutHexDWord_CallInstance = CMock_Guts_MemChain(Mock.UART2PutHexDWord_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_UART2PutHexDWord(cmock_call_instance, toPrint);
}

char UART2Char2Hex(char ch)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_UART2Char2Hex_CALL_INSTANCE* cmock_call_instance = (CMOCK_UART2Char2Hex_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.UART2Char2Hex_CallInstance);
  Mock.UART2Char2Hex_CallInstance = CMock_Guts_MemNext(Mock.UART2Char2Hex_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'UART2Char2Hex' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_INT8(cmock_call_instance->Expected_ch, ch, cmock_line, "Function 'UART2Char2Hex' called with unexpected value for argument 'ch'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_UART2Char2Hex(CMOCK_UART2Char2Hex_CALL_INSTANCE* cmock_call_instance, char ch)
{
  cmock_call_instance->Expected_ch = ch;
}

void UART2Char2Hex_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, char ch, char cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_UART2Char2Hex_CALL_INSTANCE));
  CMOCK_UART2Char2Hex_CALL_INSTANCE* cmock_call_instance = (CMOCK_UART2Char2Hex_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.UART2Char2Hex_CallInstance = CMock_Guts_MemChain(Mock.UART2Char2Hex_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_UART2Char2Hex(cmock_call_instance, ch);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

char UART2Hex2Char(char hex)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_UART2Hex2Char_CALL_INSTANCE* cmock_call_instance = (CMOCK_UART2Hex2Char_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.UART2Hex2Char_CallInstance);
  Mock.UART2Hex2Char_CallInstance = CMock_Guts_MemNext(Mock.UART2Hex2Char_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'UART2Hex2Char' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_INT8(cmock_call_instance->Expected_hex, hex, cmock_line, "Function 'UART2Hex2Char' called with unexpected value for argument 'hex'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_UART2Hex2Char(CMOCK_UART2Hex2Char_CALL_INSTANCE* cmock_call_instance, char hex)
{
  cmock_call_instance->Expected_hex = hex;
}

void UART2Hex2Char_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, char hex, char cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_UART2Hex2Char_CALL_INSTANCE));
  CMOCK_UART2Hex2Char_CALL_INSTANCE* cmock_call_instance = (CMOCK_UART2Hex2Char_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.UART2Hex2Char_CallInstance = CMock_Guts_MemChain(Mock.UART2Hex2Char_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_UART2Hex2Char(cmock_call_instance, hex);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void UART2ClrError(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_UART2ClrError_CALL_INSTANCE* cmock_call_instance = (CMOCK_UART2ClrError_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.UART2ClrError_CallInstance);
  Mock.UART2ClrError_CallInstance = CMock_Guts_MemNext(Mock.UART2ClrError_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'UART2ClrError' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
}

void UART2ClrError_CMockExpect(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_UART2ClrError_CALL_INSTANCE));
  CMOCK_UART2ClrError_CALL_INSTANCE* cmock_call_instance = (CMOCK_UART2ClrError_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.UART2ClrError_CallInstance = CMock_Guts_MemChain(Mock.UART2ClrError_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
}
