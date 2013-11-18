/* AUTOGENERATED FILE. DO NOT EDIT. */
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "unity.h"
#include "cmock.h"
#include "Mockusb_hal.h"

typedef struct _CMOCK_USBHALSetBusAddress_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  BYTE Expected_addr;

} CMOCK_USBHALSetBusAddress_CALL_INSTANCE;

typedef struct _CMOCK_USBHALControlUsbResistors_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  BYTE Expected_flags;

} CMOCK_USBHALControlUsbResistors_CALL_INSTANCE;

typedef struct _CMOCK_USBHALSessionIsValid_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  BOOL ReturnVal;

} CMOCK_USBHALSessionIsValid_CALL_INSTANCE;

typedef struct _CMOCK_USBHALControlBusPower_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  BOOL ReturnVal;
  BYTE Expected_cmd;

} CMOCK_USBHALControlBusPower_CALL_INSTANCE;

typedef struct _CMOCK_USBHALGetLastError_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  unsigned long ReturnVal;

} CMOCK_USBHALGetLastError_CALL_INSTANCE;

typedef struct _CMOCK_USBHALHandleBusEvent_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;

} CMOCK_USBHALHandleBusEvent_CALL_INSTANCE;

typedef struct _CMOCK_USBHALStallPipe_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  BOOL ReturnVal;
  TRANSFER_FLAGS Expected_pipe;

} CMOCK_USBHALStallPipe_CALL_INSTANCE;

typedef struct _CMOCK_USBHALUnstallPipe_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  BOOL ReturnVal;
  TRANSFER_FLAGS Expected_pipe;

} CMOCK_USBHALUnstallPipe_CALL_INSTANCE;

typedef struct _CMOCK_USBHALGetStalledEndpoints_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  UINT16 ReturnVal;

} CMOCK_USBHALGetStalledEndpoints_CALL_INSTANCE;

typedef struct _CMOCK_USBHALFlushPipe_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  BOOL ReturnVal;
  TRANSFER_FLAGS Expected_pipe;

} CMOCK_USBHALFlushPipe_CALL_INSTANCE;

typedef struct _CMOCK_USBHALTransferData_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  BOOL ReturnVal;
  TRANSFER_FLAGS Expected_flags;
  void* Expected_buffer;
  unsigned int Expected_size;

} CMOCK_USBHALTransferData_CALL_INSTANCE;

typedef struct _CMOCK_USBHALSetEpConfiguration_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  BOOL ReturnVal;
  BYTE Expected_ep_num;
  UINT16 Expected_max_pkt_size;
  UINT16 Expected_flags;

} CMOCK_USBHALSetEpConfiguration_CALL_INSTANCE;

typedef struct _CMOCK_USBHALInitialize_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  BOOL ReturnVal;
  unsigned long Expected_flags;

} CMOCK_USBHALInitialize_CALL_INSTANCE;

static struct Mockusb_halInstance
{
  CMOCK_MEM_INDEX_TYPE USBHALSetBusAddress_CallInstance;
  CMOCK_MEM_INDEX_TYPE USBHALControlUsbResistors_CallInstance;
  CMOCK_MEM_INDEX_TYPE USBHALSessionIsValid_CallInstance;
  CMOCK_MEM_INDEX_TYPE USBHALControlBusPower_CallInstance;
  CMOCK_MEM_INDEX_TYPE USBHALGetLastError_CallInstance;
  CMOCK_MEM_INDEX_TYPE USBHALHandleBusEvent_CallInstance;
  CMOCK_MEM_INDEX_TYPE USBHALStallPipe_CallInstance;
  CMOCK_MEM_INDEX_TYPE USBHALUnstallPipe_CallInstance;
  CMOCK_MEM_INDEX_TYPE USBHALGetStalledEndpoints_CallInstance;
  CMOCK_MEM_INDEX_TYPE USBHALFlushPipe_CallInstance;
  CMOCK_MEM_INDEX_TYPE USBHALTransferData_CallInstance;
  CMOCK_MEM_INDEX_TYPE USBHALSetEpConfiguration_CallInstance;
  CMOCK_MEM_INDEX_TYPE USBHALInitialize_CallInstance;
} Mock;

extern jmp_buf AbortFrame;

void Mockusb_hal_Verify(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.USBHALSetBusAddress_CallInstance, cmock_line, "Function 'USBHALSetBusAddress' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.USBHALControlUsbResistors_CallInstance, cmock_line, "Function 'USBHALControlUsbResistors' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.USBHALSessionIsValid_CallInstance, cmock_line, "Function 'USBHALSessionIsValid' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.USBHALControlBusPower_CallInstance, cmock_line, "Function 'USBHALControlBusPower' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.USBHALGetLastError_CallInstance, cmock_line, "Function 'USBHALGetLastError' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.USBHALHandleBusEvent_CallInstance, cmock_line, "Function 'USBHALHandleBusEvent' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.USBHALStallPipe_CallInstance, cmock_line, "Function 'USBHALStallPipe' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.USBHALUnstallPipe_CallInstance, cmock_line, "Function 'USBHALUnstallPipe' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.USBHALGetStalledEndpoints_CallInstance, cmock_line, "Function 'USBHALGetStalledEndpoints' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.USBHALFlushPipe_CallInstance, cmock_line, "Function 'USBHALFlushPipe' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.USBHALTransferData_CallInstance, cmock_line, "Function 'USBHALTransferData' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.USBHALSetEpConfiguration_CallInstance, cmock_line, "Function 'USBHALSetEpConfiguration' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.USBHALInitialize_CallInstance, cmock_line, "Function 'USBHALInitialize' called less times than expected.");
}

void Mockusb_hal_Init(void)
{
  Mockusb_hal_Destroy();
}

void Mockusb_hal_Destroy(void)
{
  CMock_Guts_MemFreeAll();
  memset(&Mock, 0, sizeof(Mock));
}

void USBHALSetBusAddress(BYTE addr)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_USBHALSetBusAddress_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHALSetBusAddress_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.USBHALSetBusAddress_CallInstance);
  Mock.USBHALSetBusAddress_CallInstance = CMock_Guts_MemNext(Mock.USBHALSetBusAddress_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'USBHALSetBusAddress' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_addr), (void*)(&addr), sizeof(BYTE), cmock_line, "Function 'USBHALSetBusAddress' called with unexpected value for argument 'addr'.");
  }
}

void CMockExpectParameters_USBHALSetBusAddress(CMOCK_USBHALSetBusAddress_CALL_INSTANCE* cmock_call_instance, BYTE addr)
{
  memcpy(&cmock_call_instance->Expected_addr, &addr, sizeof(BYTE));
}

void USBHALSetBusAddress_CMockExpect(UNITY_LINE_TYPE cmock_line, BYTE addr)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_USBHALSetBusAddress_CALL_INSTANCE));
  CMOCK_USBHALSetBusAddress_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHALSetBusAddress_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.USBHALSetBusAddress_CallInstance = CMock_Guts_MemChain(Mock.USBHALSetBusAddress_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_USBHALSetBusAddress(cmock_call_instance, addr);
}

void USBHALControlUsbResistors(BYTE flags)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_USBHALControlUsbResistors_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHALControlUsbResistors_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.USBHALControlUsbResistors_CallInstance);
  Mock.USBHALControlUsbResistors_CallInstance = CMock_Guts_MemNext(Mock.USBHALControlUsbResistors_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'USBHALControlUsbResistors' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_flags), (void*)(&flags), sizeof(BYTE), cmock_line, "Function 'USBHALControlUsbResistors' called with unexpected value for argument 'flags'.");
  }
}

void CMockExpectParameters_USBHALControlUsbResistors(CMOCK_USBHALControlUsbResistors_CALL_INSTANCE* cmock_call_instance, BYTE flags)
{
  memcpy(&cmock_call_instance->Expected_flags, &flags, sizeof(BYTE));
}

void USBHALControlUsbResistors_CMockExpect(UNITY_LINE_TYPE cmock_line, BYTE flags)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_USBHALControlUsbResistors_CALL_INSTANCE));
  CMOCK_USBHALControlUsbResistors_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHALControlUsbResistors_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.USBHALControlUsbResistors_CallInstance = CMock_Guts_MemChain(Mock.USBHALControlUsbResistors_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_USBHALControlUsbResistors(cmock_call_instance, flags);
}

BOOL USBHALSessionIsValid(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_USBHALSessionIsValid_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHALSessionIsValid_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.USBHALSessionIsValid_CallInstance);
  Mock.USBHALSessionIsValid_CallInstance = CMock_Guts_MemNext(Mock.USBHALSessionIsValid_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'USBHALSessionIsValid' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  return cmock_call_instance->ReturnVal;
}

void USBHALSessionIsValid_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, BOOL cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_USBHALSessionIsValid_CALL_INSTANCE));
  CMOCK_USBHALSessionIsValid_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHALSessionIsValid_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.USBHALSessionIsValid_CallInstance = CMock_Guts_MemChain(Mock.USBHALSessionIsValid_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
}

BOOL USBHALControlBusPower(BYTE cmd)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_USBHALControlBusPower_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHALControlBusPower_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.USBHALControlBusPower_CallInstance);
  Mock.USBHALControlBusPower_CallInstance = CMock_Guts_MemNext(Mock.USBHALControlBusPower_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'USBHALControlBusPower' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_cmd), (void*)(&cmd), sizeof(BYTE), cmock_line, "Function 'USBHALControlBusPower' called with unexpected value for argument 'cmd'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_USBHALControlBusPower(CMOCK_USBHALControlBusPower_CALL_INSTANCE* cmock_call_instance, BYTE cmd)
{
  memcpy(&cmock_call_instance->Expected_cmd, &cmd, sizeof(BYTE));
}

void USBHALControlBusPower_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, BYTE cmd, BOOL cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_USBHALControlBusPower_CALL_INSTANCE));
  CMOCK_USBHALControlBusPower_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHALControlBusPower_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.USBHALControlBusPower_CallInstance = CMock_Guts_MemChain(Mock.USBHALControlBusPower_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_USBHALControlBusPower(cmock_call_instance, cmd);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

unsigned long USBHALGetLastError(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_USBHALGetLastError_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHALGetLastError_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.USBHALGetLastError_CallInstance);
  Mock.USBHALGetLastError_CallInstance = CMock_Guts_MemNext(Mock.USBHALGetLastError_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'USBHALGetLastError' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  return cmock_call_instance->ReturnVal;
}

void USBHALGetLastError_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, unsigned long cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_USBHALGetLastError_CALL_INSTANCE));
  CMOCK_USBHALGetLastError_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHALGetLastError_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.USBHALGetLastError_CallInstance = CMock_Guts_MemChain(Mock.USBHALGetLastError_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void USBHALHandleBusEvent(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_USBHALHandleBusEvent_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHALHandleBusEvent_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.USBHALHandleBusEvent_CallInstance);
  Mock.USBHALHandleBusEvent_CallInstance = CMock_Guts_MemNext(Mock.USBHALHandleBusEvent_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'USBHALHandleBusEvent' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
}

void USBHALHandleBusEvent_CMockExpect(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_USBHALHandleBusEvent_CALL_INSTANCE));
  CMOCK_USBHALHandleBusEvent_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHALHandleBusEvent_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.USBHALHandleBusEvent_CallInstance = CMock_Guts_MemChain(Mock.USBHALHandleBusEvent_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
}

BOOL USBHALStallPipe(TRANSFER_FLAGS pipe)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_USBHALStallPipe_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHALStallPipe_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.USBHALStallPipe_CallInstance);
  Mock.USBHALStallPipe_CallInstance = CMock_Guts_MemNext(Mock.USBHALStallPipe_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'USBHALStallPipe' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_pipe), (void*)(&pipe), sizeof(TRANSFER_FLAGS), cmock_line, "Function 'USBHALStallPipe' called with unexpected value for argument 'pipe'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_USBHALStallPipe(CMOCK_USBHALStallPipe_CALL_INSTANCE* cmock_call_instance, TRANSFER_FLAGS pipe)
{
  memcpy(&cmock_call_instance->Expected_pipe, &pipe, sizeof(TRANSFER_FLAGS));
}

void USBHALStallPipe_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, TRANSFER_FLAGS pipe, BOOL cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_USBHALStallPipe_CALL_INSTANCE));
  CMOCK_USBHALStallPipe_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHALStallPipe_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.USBHALStallPipe_CallInstance = CMock_Guts_MemChain(Mock.USBHALStallPipe_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_USBHALStallPipe(cmock_call_instance, pipe);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

BOOL USBHALUnstallPipe(TRANSFER_FLAGS pipe)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_USBHALUnstallPipe_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHALUnstallPipe_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.USBHALUnstallPipe_CallInstance);
  Mock.USBHALUnstallPipe_CallInstance = CMock_Guts_MemNext(Mock.USBHALUnstallPipe_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'USBHALUnstallPipe' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_pipe), (void*)(&pipe), sizeof(TRANSFER_FLAGS), cmock_line, "Function 'USBHALUnstallPipe' called with unexpected value for argument 'pipe'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_USBHALUnstallPipe(CMOCK_USBHALUnstallPipe_CALL_INSTANCE* cmock_call_instance, TRANSFER_FLAGS pipe)
{
  memcpy(&cmock_call_instance->Expected_pipe, &pipe, sizeof(TRANSFER_FLAGS));
}

void USBHALUnstallPipe_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, TRANSFER_FLAGS pipe, BOOL cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_USBHALUnstallPipe_CALL_INSTANCE));
  CMOCK_USBHALUnstallPipe_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHALUnstallPipe_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.USBHALUnstallPipe_CallInstance = CMock_Guts_MemChain(Mock.USBHALUnstallPipe_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_USBHALUnstallPipe(cmock_call_instance, pipe);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

UINT16 USBHALGetStalledEndpoints(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_USBHALGetStalledEndpoints_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHALGetStalledEndpoints_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.USBHALGetStalledEndpoints_CallInstance);
  Mock.USBHALGetStalledEndpoints_CallInstance = CMock_Guts_MemNext(Mock.USBHALGetStalledEndpoints_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'USBHALGetStalledEndpoints' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  return cmock_call_instance->ReturnVal;
}

void USBHALGetStalledEndpoints_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, UINT16 cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_USBHALGetStalledEndpoints_CALL_INSTANCE));
  CMOCK_USBHALGetStalledEndpoints_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHALGetStalledEndpoints_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.USBHALGetStalledEndpoints_CallInstance = CMock_Guts_MemChain(Mock.USBHALGetStalledEndpoints_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
}

BOOL USBHALFlushPipe(TRANSFER_FLAGS pipe)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_USBHALFlushPipe_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHALFlushPipe_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.USBHALFlushPipe_CallInstance);
  Mock.USBHALFlushPipe_CallInstance = CMock_Guts_MemNext(Mock.USBHALFlushPipe_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'USBHALFlushPipe' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_pipe), (void*)(&pipe), sizeof(TRANSFER_FLAGS), cmock_line, "Function 'USBHALFlushPipe' called with unexpected value for argument 'pipe'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_USBHALFlushPipe(CMOCK_USBHALFlushPipe_CALL_INSTANCE* cmock_call_instance, TRANSFER_FLAGS pipe)
{
  memcpy(&cmock_call_instance->Expected_pipe, &pipe, sizeof(TRANSFER_FLAGS));
}

void USBHALFlushPipe_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, TRANSFER_FLAGS pipe, BOOL cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_USBHALFlushPipe_CALL_INSTANCE));
  CMOCK_USBHALFlushPipe_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHALFlushPipe_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.USBHALFlushPipe_CallInstance = CMock_Guts_MemChain(Mock.USBHALFlushPipe_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_USBHALFlushPipe(cmock_call_instance, pipe);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

BOOL USBHALTransferData(TRANSFER_FLAGS flags, void* buffer, unsigned int size)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_USBHALTransferData_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHALTransferData_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.USBHALTransferData_CallInstance);
  Mock.USBHALTransferData_CallInstance = CMock_Guts_MemNext(Mock.USBHALTransferData_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'USBHALTransferData' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_flags), (void*)(&flags), sizeof(TRANSFER_FLAGS), cmock_line, "Function 'USBHALTransferData' called with unexpected value for argument 'flags'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_PTR(cmock_call_instance->Expected_buffer, buffer, cmock_line, "Function 'USBHALTransferData' called with unexpected value for argument 'buffer'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_HEX32(cmock_call_instance->Expected_size, size, cmock_line, "Function 'USBHALTransferData' called with unexpected value for argument 'size'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_USBHALTransferData(CMOCK_USBHALTransferData_CALL_INSTANCE* cmock_call_instance, TRANSFER_FLAGS flags, void* buffer, unsigned int size)
{
  memcpy(&cmock_call_instance->Expected_flags, &flags, sizeof(TRANSFER_FLAGS));
  cmock_call_instance->Expected_buffer = buffer;
  cmock_call_instance->Expected_size = size;
}

void USBHALTransferData_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, TRANSFER_FLAGS flags, void* buffer, unsigned int size, BOOL cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_USBHALTransferData_CALL_INSTANCE));
  CMOCK_USBHALTransferData_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHALTransferData_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.USBHALTransferData_CallInstance = CMock_Guts_MemChain(Mock.USBHALTransferData_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_USBHALTransferData(cmock_call_instance, flags, buffer, size);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

BOOL USBHALSetEpConfiguration(BYTE ep_num, UINT16 max_pkt_size, UINT16 flags)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_USBHALSetEpConfiguration_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHALSetEpConfiguration_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.USBHALSetEpConfiguration_CallInstance);
  Mock.USBHALSetEpConfiguration_CallInstance = CMock_Guts_MemNext(Mock.USBHALSetEpConfiguration_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'USBHALSetEpConfiguration' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_ep_num), (void*)(&ep_num), sizeof(BYTE), cmock_line, "Function 'USBHALSetEpConfiguration' called with unexpected value for argument 'ep_num'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_HEX16(cmock_call_instance->Expected_max_pkt_size, max_pkt_size, cmock_line, "Function 'USBHALSetEpConfiguration' called with unexpected value for argument 'max_pkt_size'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_HEX16(cmock_call_instance->Expected_flags, flags, cmock_line, "Function 'USBHALSetEpConfiguration' called with unexpected value for argument 'flags'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_USBHALSetEpConfiguration(CMOCK_USBHALSetEpConfiguration_CALL_INSTANCE* cmock_call_instance, BYTE ep_num, UINT16 max_pkt_size, UINT16 flags)
{
  memcpy(&cmock_call_instance->Expected_ep_num, &ep_num, sizeof(BYTE));
  cmock_call_instance->Expected_max_pkt_size = max_pkt_size;
  cmock_call_instance->Expected_flags = flags;
}

void USBHALSetEpConfiguration_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, BYTE ep_num, UINT16 max_pkt_size, UINT16 flags, BOOL cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_USBHALSetEpConfiguration_CALL_INSTANCE));
  CMOCK_USBHALSetEpConfiguration_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHALSetEpConfiguration_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.USBHALSetEpConfiguration_CallInstance = CMock_Guts_MemChain(Mock.USBHALSetEpConfiguration_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_USBHALSetEpConfiguration(cmock_call_instance, ep_num, max_pkt_size, flags);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

BOOL USBHALInitialize(unsigned long flags)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_USBHALInitialize_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHALInitialize_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.USBHALInitialize_CallInstance);
  Mock.USBHALInitialize_CallInstance = CMock_Guts_MemNext(Mock.USBHALInitialize_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'USBHALInitialize' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_HEX32(cmock_call_instance->Expected_flags, flags, cmock_line, "Function 'USBHALInitialize' called with unexpected value for argument 'flags'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_USBHALInitialize(CMOCK_USBHALInitialize_CALL_INSTANCE* cmock_call_instance, unsigned long flags)
{
  cmock_call_instance->Expected_flags = flags;
}

void USBHALInitialize_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, unsigned long flags, BOOL cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_USBHALInitialize_CALL_INSTANCE));
  CMOCK_USBHALInitialize_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHALInitialize_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.USBHALInitialize_CallInstance = CMock_Guts_MemChain(Mock.USBHALInitialize_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_USBHALInitialize(cmock_call_instance, flags);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

