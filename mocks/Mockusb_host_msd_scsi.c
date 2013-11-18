/* AUTOGENERATED FILE. DO NOT EDIT. */
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "unity.h"
#include "cmock.h"
#include "Mockusb_host_msd_scsi.h"

typedef struct _CMOCK_USBHostMSDSCSIMediaDetect_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  BYTE ReturnVal;

} CMOCK_USBHostMSDSCSIMediaDetect_CALL_INSTANCE;

typedef struct _CMOCK_USBHostMSDSCSIMediaInitialize_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  MEDIA_INFORMATION* ReturnVal;

} CMOCK_USBHostMSDSCSIMediaInitialize_CALL_INSTANCE;

typedef struct _CMOCK_USBHostMSDSCSIMediaReset_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  BYTE ReturnVal;

} CMOCK_USBHostMSDSCSIMediaReset_CALL_INSTANCE;

typedef struct _CMOCK_USBHostMSDSCSISectorRead_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  BYTE ReturnVal;
  DWORD Expected_sectorAddress;
  BYTE* Expected_dataBuffer;

} CMOCK_USBHostMSDSCSISectorRead_CALL_INSTANCE;

typedef struct _CMOCK_USBHostMSDSCSISectorWrite_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  BYTE ReturnVal;
  DWORD Expected_sectorAddress;
  BYTE* Expected_dataBuffer;
  BYTE Expected_allowWriteToZero;

} CMOCK_USBHostMSDSCSISectorWrite_CALL_INSTANCE;

typedef struct _CMOCK_USBHostMSDSCSIWriteProtectState_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  BYTE ReturnVal;

} CMOCK_USBHostMSDSCSIWriteProtectState_CALL_INSTANCE;

typedef struct _CMOCK_USBHostMSDSCSIInitialize_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  BOOL ReturnVal;
  BYTE Expected_address;
  DWORD Expected_flags;
  BYTE Expected_clientDriverID;

} CMOCK_USBHostMSDSCSIInitialize_CALL_INSTANCE;

typedef struct _CMOCK_USBHostMSDSCSIEventHandler_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  BOOL ReturnVal;
  BYTE Expected_address;
  USB_EVENT Expected_event;
  void* Expected_data;
  DWORD Expected_size;

} CMOCK_USBHostMSDSCSIEventHandler_CALL_INSTANCE;

static struct Mockusb_host_msd_scsiInstance
{
  CMOCK_MEM_INDEX_TYPE USBHostMSDSCSIMediaDetect_CallInstance;
  CMOCK_MEM_INDEX_TYPE USBHostMSDSCSIMediaInitialize_CallInstance;
  CMOCK_MEM_INDEX_TYPE USBHostMSDSCSIMediaReset_CallInstance;
  CMOCK_MEM_INDEX_TYPE USBHostMSDSCSISectorRead_CallInstance;
  CMOCK_MEM_INDEX_TYPE USBHostMSDSCSISectorWrite_CallInstance;
  CMOCK_MEM_INDEX_TYPE USBHostMSDSCSIWriteProtectState_CallInstance;
  CMOCK_MEM_INDEX_TYPE USBHostMSDSCSIInitialize_CallInstance;
  CMOCK_MEM_INDEX_TYPE USBHostMSDSCSIEventHandler_CallInstance;
} Mock;

extern jmp_buf AbortFrame;

void Mockusb_host_msd_scsi_Verify(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.USBHostMSDSCSIMediaDetect_CallInstance, cmock_line, "Function 'USBHostMSDSCSIMediaDetect' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.USBHostMSDSCSIMediaInitialize_CallInstance, cmock_line, "Function 'USBHostMSDSCSIMediaInitialize' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.USBHostMSDSCSIMediaReset_CallInstance, cmock_line, "Function 'USBHostMSDSCSIMediaReset' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.USBHostMSDSCSISectorRead_CallInstance, cmock_line, "Function 'USBHostMSDSCSISectorRead' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.USBHostMSDSCSISectorWrite_CallInstance, cmock_line, "Function 'USBHostMSDSCSISectorWrite' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.USBHostMSDSCSIWriteProtectState_CallInstance, cmock_line, "Function 'USBHostMSDSCSIWriteProtectState' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.USBHostMSDSCSIInitialize_CallInstance, cmock_line, "Function 'USBHostMSDSCSIInitialize' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.USBHostMSDSCSIEventHandler_CallInstance, cmock_line, "Function 'USBHostMSDSCSIEventHandler' called less times than expected.");
}

void Mockusb_host_msd_scsi_Init(void)
{
  Mockusb_host_msd_scsi_Destroy();
}

void Mockusb_host_msd_scsi_Destroy(void)
{
  CMock_Guts_MemFreeAll();
  memset(&Mock, 0, sizeof(Mock));
}

BYTE USBHostMSDSCSIMediaDetect(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_USBHostMSDSCSIMediaDetect_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHostMSDSCSIMediaDetect_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.USBHostMSDSCSIMediaDetect_CallInstance);
  Mock.USBHostMSDSCSIMediaDetect_CallInstance = CMock_Guts_MemNext(Mock.USBHostMSDSCSIMediaDetect_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'USBHostMSDSCSIMediaDetect' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  return cmock_call_instance->ReturnVal;
}

void USBHostMSDSCSIMediaDetect_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, BYTE cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_USBHostMSDSCSIMediaDetect_CALL_INSTANCE));
  CMOCK_USBHostMSDSCSIMediaDetect_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHostMSDSCSIMediaDetect_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.USBHostMSDSCSIMediaDetect_CallInstance = CMock_Guts_MemChain(Mock.USBHostMSDSCSIMediaDetect_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  memcpy(&cmock_call_instance->ReturnVal, &cmock_to_return, sizeof(BYTE));
}

MEDIA_INFORMATION* USBHostMSDSCSIMediaInitialize(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_USBHostMSDSCSIMediaInitialize_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHostMSDSCSIMediaInitialize_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.USBHostMSDSCSIMediaInitialize_CallInstance);
  Mock.USBHostMSDSCSIMediaInitialize_CallInstance = CMock_Guts_MemNext(Mock.USBHostMSDSCSIMediaInitialize_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'USBHostMSDSCSIMediaInitialize' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  return cmock_call_instance->ReturnVal;
}

void USBHostMSDSCSIMediaInitialize_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, MEDIA_INFORMATION* cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_USBHostMSDSCSIMediaInitialize_CALL_INSTANCE));
  CMOCK_USBHostMSDSCSIMediaInitialize_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHostMSDSCSIMediaInitialize_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.USBHostMSDSCSIMediaInitialize_CallInstance = CMock_Guts_MemChain(Mock.USBHostMSDSCSIMediaInitialize_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->ReturnVal = cmock_to_return;
}

BYTE USBHostMSDSCSIMediaReset(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_USBHostMSDSCSIMediaReset_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHostMSDSCSIMediaReset_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.USBHostMSDSCSIMediaReset_CallInstance);
  Mock.USBHostMSDSCSIMediaReset_CallInstance = CMock_Guts_MemNext(Mock.USBHostMSDSCSIMediaReset_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'USBHostMSDSCSIMediaReset' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  return cmock_call_instance->ReturnVal;
}

void USBHostMSDSCSIMediaReset_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, BYTE cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_USBHostMSDSCSIMediaReset_CALL_INSTANCE));
  CMOCK_USBHostMSDSCSIMediaReset_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHostMSDSCSIMediaReset_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.USBHostMSDSCSIMediaReset_CallInstance = CMock_Guts_MemChain(Mock.USBHostMSDSCSIMediaReset_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  memcpy(&cmock_call_instance->ReturnVal, &cmock_to_return, sizeof(BYTE));
}

BYTE USBHostMSDSCSISectorRead(DWORD sectorAddress, BYTE* dataBuffer)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_USBHostMSDSCSISectorRead_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHostMSDSCSISectorRead_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.USBHostMSDSCSISectorRead_CallInstance);
  Mock.USBHostMSDSCSISectorRead_CallInstance = CMock_Guts_MemNext(Mock.USBHostMSDSCSISectorRead_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'USBHostMSDSCSISectorRead' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_sectorAddress), (void*)(&sectorAddress), sizeof(DWORD), cmock_line, "Function 'USBHostMSDSCSISectorRead' called with unexpected value for argument 'sectorAddress'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(cmock_call_instance->Expected_dataBuffer), (void*)(dataBuffer), sizeof(BYTE), cmock_line, "Function 'USBHostMSDSCSISectorRead' called with unexpected value for argument 'dataBuffer'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_USBHostMSDSCSISectorRead(CMOCK_USBHostMSDSCSISectorRead_CALL_INSTANCE* cmock_call_instance, DWORD sectorAddress, BYTE* dataBuffer)
{
  memcpy(&cmock_call_instance->Expected_sectorAddress, &sectorAddress, sizeof(DWORD));
  cmock_call_instance->Expected_dataBuffer = dataBuffer;
}

void USBHostMSDSCSISectorRead_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, DWORD sectorAddress, BYTE* dataBuffer, BYTE cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_USBHostMSDSCSISectorRead_CALL_INSTANCE));
  CMOCK_USBHostMSDSCSISectorRead_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHostMSDSCSISectorRead_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.USBHostMSDSCSISectorRead_CallInstance = CMock_Guts_MemChain(Mock.USBHostMSDSCSISectorRead_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_USBHostMSDSCSISectorRead(cmock_call_instance, sectorAddress, dataBuffer);
  memcpy(&cmock_call_instance->ReturnVal, &cmock_to_return, sizeof(BYTE));
}

BYTE USBHostMSDSCSISectorWrite(DWORD sectorAddress, BYTE* dataBuffer, BYTE allowWriteToZero)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_USBHostMSDSCSISectorWrite_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHostMSDSCSISectorWrite_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.USBHostMSDSCSISectorWrite_CallInstance);
  Mock.USBHostMSDSCSISectorWrite_CallInstance = CMock_Guts_MemNext(Mock.USBHostMSDSCSISectorWrite_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'USBHostMSDSCSISectorWrite' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_sectorAddress), (void*)(&sectorAddress), sizeof(DWORD), cmock_line, "Function 'USBHostMSDSCSISectorWrite' called with unexpected value for argument 'sectorAddress'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(cmock_call_instance->Expected_dataBuffer), (void*)(dataBuffer), sizeof(BYTE), cmock_line, "Function 'USBHostMSDSCSISectorWrite' called with unexpected value for argument 'dataBuffer'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_allowWriteToZero), (void*)(&allowWriteToZero), sizeof(BYTE), cmock_line, "Function 'USBHostMSDSCSISectorWrite' called with unexpected value for argument 'allowWriteToZero'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_USBHostMSDSCSISectorWrite(CMOCK_USBHostMSDSCSISectorWrite_CALL_INSTANCE* cmock_call_instance, DWORD sectorAddress, BYTE* dataBuffer, BYTE allowWriteToZero)
{
  memcpy(&cmock_call_instance->Expected_sectorAddress, &sectorAddress, sizeof(DWORD));
  cmock_call_instance->Expected_dataBuffer = dataBuffer;
  memcpy(&cmock_call_instance->Expected_allowWriteToZero, &allowWriteToZero, sizeof(BYTE));
}

void USBHostMSDSCSISectorWrite_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, DWORD sectorAddress, BYTE* dataBuffer, BYTE allowWriteToZero, BYTE cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_USBHostMSDSCSISectorWrite_CALL_INSTANCE));
  CMOCK_USBHostMSDSCSISectorWrite_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHostMSDSCSISectorWrite_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.USBHostMSDSCSISectorWrite_CallInstance = CMock_Guts_MemChain(Mock.USBHostMSDSCSISectorWrite_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_USBHostMSDSCSISectorWrite(cmock_call_instance, sectorAddress, dataBuffer, allowWriteToZero);
  memcpy(&cmock_call_instance->ReturnVal, &cmock_to_return, sizeof(BYTE));
}

BYTE USBHostMSDSCSIWriteProtectState(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_USBHostMSDSCSIWriteProtectState_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHostMSDSCSIWriteProtectState_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.USBHostMSDSCSIWriteProtectState_CallInstance);
  Mock.USBHostMSDSCSIWriteProtectState_CallInstance = CMock_Guts_MemNext(Mock.USBHostMSDSCSIWriteProtectState_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'USBHostMSDSCSIWriteProtectState' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  return cmock_call_instance->ReturnVal;
}

void USBHostMSDSCSIWriteProtectState_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, BYTE cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_USBHostMSDSCSIWriteProtectState_CALL_INSTANCE));
  CMOCK_USBHostMSDSCSIWriteProtectState_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHostMSDSCSIWriteProtectState_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.USBHostMSDSCSIWriteProtectState_CallInstance = CMock_Guts_MemChain(Mock.USBHostMSDSCSIWriteProtectState_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  memcpy(&cmock_call_instance->ReturnVal, &cmock_to_return, sizeof(BYTE));
}

BOOL USBHostMSDSCSIInitialize(BYTE address, DWORD flags, BYTE clientDriverID)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_USBHostMSDSCSIInitialize_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHostMSDSCSIInitialize_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.USBHostMSDSCSIInitialize_CallInstance);
  Mock.USBHostMSDSCSIInitialize_CallInstance = CMock_Guts_MemNext(Mock.USBHostMSDSCSIInitialize_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'USBHostMSDSCSIInitialize' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_address), (void*)(&address), sizeof(BYTE), cmock_line, "Function 'USBHostMSDSCSIInitialize' called with unexpected value for argument 'address'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_flags), (void*)(&flags), sizeof(DWORD), cmock_line, "Function 'USBHostMSDSCSIInitialize' called with unexpected value for argument 'flags'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_clientDriverID), (void*)(&clientDriverID), sizeof(BYTE), cmock_line, "Function 'USBHostMSDSCSIInitialize' called with unexpected value for argument 'clientDriverID'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_USBHostMSDSCSIInitialize(CMOCK_USBHostMSDSCSIInitialize_CALL_INSTANCE* cmock_call_instance, BYTE address, DWORD flags, BYTE clientDriverID)
{
  memcpy(&cmock_call_instance->Expected_address, &address, sizeof(BYTE));
  memcpy(&cmock_call_instance->Expected_flags, &flags, sizeof(DWORD));
  memcpy(&cmock_call_instance->Expected_clientDriverID, &clientDriverID, sizeof(BYTE));
}

void USBHostMSDSCSIInitialize_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, BYTE address, DWORD flags, BYTE clientDriverID, BOOL cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_USBHostMSDSCSIInitialize_CALL_INSTANCE));
  CMOCK_USBHostMSDSCSIInitialize_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHostMSDSCSIInitialize_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.USBHostMSDSCSIInitialize_CallInstance = CMock_Guts_MemChain(Mock.USBHostMSDSCSIInitialize_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_USBHostMSDSCSIInitialize(cmock_call_instance, address, flags, clientDriverID);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

BOOL USBHostMSDSCSIEventHandler(BYTE address, USB_EVENT event, void* data, DWORD size)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_USBHostMSDSCSIEventHandler_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHostMSDSCSIEventHandler_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.USBHostMSDSCSIEventHandler_CallInstance);
  Mock.USBHostMSDSCSIEventHandler_CallInstance = CMock_Guts_MemNext(Mock.USBHostMSDSCSIEventHandler_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'USBHostMSDSCSIEventHandler' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_address), (void*)(&address), sizeof(BYTE), cmock_line, "Function 'USBHostMSDSCSIEventHandler' called with unexpected value for argument 'address'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_event), (void*)(&event), sizeof(USB_EVENT), cmock_line, "Function 'USBHostMSDSCSIEventHandler' called with unexpected value for argument 'event'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_PTR(cmock_call_instance->Expected_data, data, cmock_line, "Function 'USBHostMSDSCSIEventHandler' called with unexpected value for argument 'data'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_size), (void*)(&size), sizeof(DWORD), cmock_line, "Function 'USBHostMSDSCSIEventHandler' called with unexpected value for argument 'size'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_USBHostMSDSCSIEventHandler(CMOCK_USBHostMSDSCSIEventHandler_CALL_INSTANCE* cmock_call_instance, BYTE address, USB_EVENT event, void* data, DWORD size)
{
  memcpy(&cmock_call_instance->Expected_address, &address, sizeof(BYTE));
  memcpy(&cmock_call_instance->Expected_event, &event, sizeof(USB_EVENT));
  cmock_call_instance->Expected_data = data;
  memcpy(&cmock_call_instance->Expected_size, &size, sizeof(DWORD));
}

void USBHostMSDSCSIEventHandler_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, BYTE address, USB_EVENT event, void* data, DWORD size, BOOL cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_USBHostMSDSCSIEventHandler_CALL_INSTANCE));
  CMOCK_USBHostMSDSCSIEventHandler_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHostMSDSCSIEventHandler_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.USBHostMSDSCSIEventHandler_CallInstance = CMock_Guts_MemChain(Mock.USBHostMSDSCSIEventHandler_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_USBHostMSDSCSIEventHandler(cmock_call_instance, address, event, data, size);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

