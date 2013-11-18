/* AUTOGENERATED FILE. DO NOT EDIT. */
#ifndef _MOCKUSB_HAL_H
#define _MOCKUSB_HAL_H

#include "usb_hal.h"

void Mockusb_hal_Init(void);
void Mockusb_hal_Destroy(void);
void Mockusb_hal_Verify(void);




#define USBHALSetBusAddress_Expect(addr) USBHALSetBusAddress_CMockExpect(__LINE__, addr)
void USBHALSetBusAddress_CMockExpect(UNITY_LINE_TYPE cmock_line, BYTE addr);
#define USBHALControlUsbResistors_Expect(flags) USBHALControlUsbResistors_CMockExpect(__LINE__, flags)
void USBHALControlUsbResistors_CMockExpect(UNITY_LINE_TYPE cmock_line, BYTE flags);
#define USBHALSessionIsValid_ExpectAndReturn(cmock_retval) USBHALSessionIsValid_CMockExpectAndReturn(__LINE__, cmock_retval)
void USBHALSessionIsValid_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, BOOL cmock_to_return);
#define USBHALControlBusPower_ExpectAndReturn(cmd, cmock_retval) USBHALControlBusPower_CMockExpectAndReturn(__LINE__, cmd, cmock_retval)
void USBHALControlBusPower_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, BYTE cmd, BOOL cmock_to_return);
#define USBHALGetLastError_ExpectAndReturn(cmock_retval) USBHALGetLastError_CMockExpectAndReturn(__LINE__, cmock_retval)
void USBHALGetLastError_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, unsigned long cmock_to_return);
#define USBHALHandleBusEvent_Expect() USBHALHandleBusEvent_CMockExpect(__LINE__)
void USBHALHandleBusEvent_CMockExpect(UNITY_LINE_TYPE cmock_line);
#define USBHALStallPipe_ExpectAndReturn(pipe, cmock_retval) USBHALStallPipe_CMockExpectAndReturn(__LINE__, pipe, cmock_retval)
void USBHALStallPipe_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, TRANSFER_FLAGS pipe, BOOL cmock_to_return);
#define USBHALUnstallPipe_ExpectAndReturn(pipe, cmock_retval) USBHALUnstallPipe_CMockExpectAndReturn(__LINE__, pipe, cmock_retval)
void USBHALUnstallPipe_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, TRANSFER_FLAGS pipe, BOOL cmock_to_return);
#define USBHALGetStalledEndpoints_ExpectAndReturn(cmock_retval) USBHALGetStalledEndpoints_CMockExpectAndReturn(__LINE__, cmock_retval)
void USBHALGetStalledEndpoints_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, UINT16 cmock_to_return);
#define USBHALFlushPipe_ExpectAndReturn(pipe, cmock_retval) USBHALFlushPipe_CMockExpectAndReturn(__LINE__, pipe, cmock_retval)
void USBHALFlushPipe_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, TRANSFER_FLAGS pipe, BOOL cmock_to_return);
#define USBHALTransferData_ExpectAndReturn(flags, buffer, size, cmock_retval) USBHALTransferData_CMockExpectAndReturn(__LINE__, flags, buffer, size, cmock_retval)
void USBHALTransferData_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, TRANSFER_FLAGS flags, void* buffer, unsigned int size, BOOL cmock_to_return);
#define USBHALSetEpConfiguration_ExpectAndReturn(ep_num, max_pkt_size, flags, cmock_retval) USBHALSetEpConfiguration_CMockExpectAndReturn(__LINE__, ep_num, max_pkt_size, flags, cmock_retval)
void USBHALSetEpConfiguration_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, BYTE ep_num, UINT16 max_pkt_size, UINT16 flags, BOOL cmock_to_return);
#define USBHALInitialize_ExpectAndReturn(flags, cmock_retval) USBHALInitialize_CMockExpectAndReturn(__LINE__, flags, cmock_retval)
void USBHALInitialize_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, unsigned long flags, BOOL cmock_to_return);

#endif
