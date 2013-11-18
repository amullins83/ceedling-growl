/* AUTOGENERATED FILE. DO NOT EDIT. */
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "unity.h"
#include "cmock.h"
#include "Mockusb_host_hid_parser.h"

typedef struct _CMOCK_USBHostHID_HasUsage_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  BOOL ReturnVal;
  HID_REPORTITEM* Expected_reportItem;
  WORD Expected_usagePage;
  WORD Expected_usage;
  WORD* Expected_pindex;
  BYTE* Expected_count;

} CMOCK_USBHostHID_HasUsage_CALL_INSTANCE;

static struct Mockusb_host_hid_parserInstance
{
  CMOCK_MEM_INDEX_TYPE USBHostHID_HasUsage_CallInstance;
} Mock;

extern jmp_buf AbortFrame;

void Mockusb_host_hid_parser_Verify(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.USBHostHID_HasUsage_CallInstance, cmock_line, "Function 'USBHostHID_HasUsage' called less times than expected.");
}

void Mockusb_host_hid_parser_Init(void)
{
  Mockusb_host_hid_parser_Destroy();
}

void Mockusb_host_hid_parser_Destroy(void)
{
  CMock_Guts_MemFreeAll();
  memset(&Mock, 0, sizeof(Mock));
}

BOOL USBHostHID_HasUsage(HID_REPORTITEM* reportItem, WORD usagePage, WORD usage, WORD* pindex, BYTE* count)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_USBHostHID_HasUsage_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHostHID_HasUsage_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.USBHostHID_HasUsage_CallInstance);
  Mock.USBHostHID_HasUsage_CallInstance = CMock_Guts_MemNext(Mock.USBHostHID_HasUsage_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'USBHostHID_HasUsage' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(cmock_call_instance->Expected_reportItem), (void*)(reportItem), sizeof(HID_REPORTITEM), cmock_line, "Function 'USBHostHID_HasUsage' called with unexpected value for argument 'reportItem'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_usagePage), (void*)(&usagePage), sizeof(WORD), cmock_line, "Function 'USBHostHID_HasUsage' called with unexpected value for argument 'usagePage'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_usage), (void*)(&usage), sizeof(WORD), cmock_line, "Function 'USBHostHID_HasUsage' called with unexpected value for argument 'usage'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(cmock_call_instance->Expected_pindex), (void*)(pindex), sizeof(WORD), cmock_line, "Function 'USBHostHID_HasUsage' called with unexpected value for argument 'pindex'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(cmock_call_instance->Expected_count), (void*)(count), sizeof(BYTE), cmock_line, "Function 'USBHostHID_HasUsage' called with unexpected value for argument 'count'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_USBHostHID_HasUsage(CMOCK_USBHostHID_HasUsage_CALL_INSTANCE* cmock_call_instance, HID_REPORTITEM* reportItem, WORD usagePage, WORD usage, WORD* pindex, BYTE* count)
{
  cmock_call_instance->Expected_reportItem = reportItem;
  memcpy(&cmock_call_instance->Expected_usagePage, &usagePage, sizeof(WORD));
  memcpy(&cmock_call_instance->Expected_usage, &usage, sizeof(WORD));
  cmock_call_instance->Expected_pindex = pindex;
  cmock_call_instance->Expected_count = count;
}

void USBHostHID_HasUsage_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, HID_REPORTITEM* reportItem, WORD usagePage, WORD usage, WORD* pindex, BYTE* count, BOOL cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_USBHostHID_HasUsage_CALL_INSTANCE));
  CMOCK_USBHostHID_HasUsage_CALL_INSTANCE* cmock_call_instance = (CMOCK_USBHostHID_HasUsage_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.USBHostHID_HasUsage_CallInstance = CMock_Guts_MemChain(Mock.USBHostHID_HasUsage_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_USBHostHID_HasUsage(cmock_call_instance, reportItem, usagePage, usage, pindex, count);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

