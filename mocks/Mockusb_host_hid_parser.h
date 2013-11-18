/* AUTOGENERATED FILE. DO NOT EDIT. */
#ifndef _MOCKUSB_HOST_HID_PARSER_H
#define _MOCKUSB_HOST_HID_PARSER_H

#include "usb_host_hid_parser.h"

void Mockusb_host_hid_parser_Init(void);
void Mockusb_host_hid_parser_Destroy(void);
void Mockusb_host_hid_parser_Verify(void);




#define USBHostHID_HasUsage_ExpectAndReturn(reportItem, usagePage, usage, pindex, count, cmock_retval) USBHostHID_HasUsage_CMockExpectAndReturn(__LINE__, reportItem, usagePage, usage, pindex, count, cmock_retval)
void USBHostHID_HasUsage_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, HID_REPORTITEM* reportItem, WORD usagePage, WORD usage, WORD* pindex, BYTE* count, BOOL cmock_to_return);

#endif
