/* AUTOGENERATED FILE. DO NOT EDIT. */
#ifndef _MOCKUSB_HOST_MSD_H
#define _MOCKUSB_HOST_MSD_H

#include "usb_host_msd.h"

void Mockusb_host_msd_Init(void);
void Mockusb_host_msd_Destroy(void);
void Mockusb_host_msd_Verify(void);




#define USBHostMSDDeviceStatus_ExpectAndReturn(deviceAddress, cmock_retval) USBHostMSDDeviceStatus_CMockExpectAndReturn(__LINE__, deviceAddress, cmock_retval)
void USBHostMSDDeviceStatus_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, BYTE deviceAddress, BYTE cmock_to_return);
#define USBHostMSDResetDevice_ExpectAndReturn(deviceAddress, cmock_retval) USBHostMSDResetDevice_CMockExpectAndReturn(__LINE__, deviceAddress, cmock_retval)
void USBHostMSDResetDevice_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, BYTE deviceAddress, BYTE cmock_to_return);
#define USBHostMSDTasks_Expect() USBHostMSDTasks_CMockExpect(__LINE__)
void USBHostMSDTasks_CMockExpect(UNITY_LINE_TYPE cmock_line);
#define USBHostMSDTerminateTransfer_Expect(deviceAddress) USBHostMSDTerminateTransfer_CMockExpect(__LINE__, deviceAddress)
void USBHostMSDTerminateTransfer_CMockExpect(UNITY_LINE_TYPE cmock_line, BYTE deviceAddress);
#define USBHostMSDTransfer_ExpectAndReturn(deviceAddress, deviceLUN, direction, commandBlock, commandBlockLength, data, dataLength, cmock_retval) USBHostMSDTransfer_CMockExpectAndReturn(__LINE__, deviceAddress, deviceLUN, direction, commandBlock, commandBlockLength, data, dataLength, cmock_retval)
void USBHostMSDTransfer_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, BYTE deviceAddress, BYTE deviceLUN, BYTE direction, BYTE* commandBlock, BYTE commandBlockLength, BYTE* data, DWORD dataLength, BYTE cmock_to_return);
#define USBHostMSDTransferIsComplete_ExpectAndReturn(deviceAddress, errorCode, byteCount, cmock_retval) USBHostMSDTransferIsComplete_CMockExpectAndReturn(__LINE__, deviceAddress, errorCode, byteCount, cmock_retval)
void USBHostMSDTransferIsComplete_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, BYTE deviceAddress, BYTE* errorCode, DWORD* byteCount, BOOL cmock_to_return);
#define USBHostMSDInitialize_ExpectAndReturn(address, flags, clientDriverID, cmock_retval) USBHostMSDInitialize_CMockExpectAndReturn(__LINE__, address, flags, clientDriverID, cmock_retval)
void USBHostMSDInitialize_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, BYTE address, DWORD flags, BYTE clientDriverID, BOOL cmock_to_return);
#define USBHostMSDEventHandler_ExpectAndReturn(address, event, data, size, cmock_retval) USBHostMSDEventHandler_CMockExpectAndReturn(__LINE__, address, event, data, size, cmock_retval)
void USBHostMSDEventHandler_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, BYTE address, USB_EVENT event, void* data, DWORD size, BOOL cmock_to_return);

#endif
