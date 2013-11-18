/* AUTOGENERATED FILE. DO NOT EDIT. */
#ifndef _MOCKUSB_HOST_LOCAL_H
#define _MOCKUSB_HOST_LOCAL_H

#include "usb_host_local.h"

void Mockusb_host_local_Init(void);
void Mockusb_host_local_Destroy(void);
void Mockusb_host_local_Verify(void);




#define _USB_CheckCommandAndEnumerationAttempts_Expect() _USB_CheckCommandAndEnumerationAttempts_CMockExpect(__LINE__)
void _USB_CheckCommandAndEnumerationAttempts_CMockExpect(UNITY_LINE_TYPE cmock_line);
#define _USB_FindClassDriver_ExpectAndReturn(bClass, bSubClass, bProtocol, pbClientDrv, cmock_retval) _USB_FindClassDriver_CMockExpectAndReturn(__LINE__, bClass, bSubClass, bProtocol, pbClientDrv, cmock_retval)
void _USB_FindClassDriver_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, BYTE bClass, BYTE bSubClass, BYTE bProtocol, BYTE* pbClientDrv, BOOL cmock_to_return);
#define _USB_FindDeviceLevelClientDriver_ExpectAndReturn(cmock_retval) _USB_FindDeviceLevelClientDriver_CMockExpectAndReturn(__LINE__, cmock_retval)
void _USB_FindDeviceLevelClientDriver_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, BOOL cmock_to_return);
#define _USB_FindEndpoint_ExpectAndReturn(endpoint, cmock_retval) _USB_FindEndpoint_CMockExpectAndReturn(__LINE__, endpoint, cmock_retval)
void _USB_FindEndpoint_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, BYTE endpoint, USB_ENDPOINT_INFO* cmock_to_return);
#define _USB_FindInterface_ExpectAndReturn(bInterface, bAltSetting, cmock_retval) _USB_FindInterface_CMockExpectAndReturn(__LINE__, bInterface, bAltSetting, cmock_retval)
void _USB_FindInterface_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, BYTE bInterface, BYTE bAltSetting, USB_INTERFACE_INFO* cmock_to_return);
#define _USB_FindNextToken_Expect() _USB_FindNextToken_CMockExpect(__LINE__)
void _USB_FindNextToken_CMockExpect(UNITY_LINE_TYPE cmock_line);
#define _USB_FindServiceEndpoint_ExpectAndReturn(transferType, cmock_retval) _USB_FindServiceEndpoint_CMockExpectAndReturn(__LINE__, transferType, cmock_retval)
void _USB_FindServiceEndpoint_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, BYTE transferType, BOOL cmock_to_return);
#define _USB_FreeConfigMemory_Expect() _USB_FreeConfigMemory_CMockExpect(__LINE__)
void _USB_FreeConfigMemory_CMockExpect(UNITY_LINE_TYPE cmock_line);
#define _USB_FreeMemory_Expect() _USB_FreeMemory_CMockExpect(__LINE__)
void _USB_FreeMemory_CMockExpect(UNITY_LINE_TYPE cmock_line);
#define _USB_InitControlRead_Expect(pEndpoint, pControlData, controlSize, pData, size) _USB_InitControlRead_CMockExpect(__LINE__, pEndpoint, pControlData, controlSize, pData, size)
void _USB_InitControlRead_CMockExpect(UNITY_LINE_TYPE cmock_line, USB_ENDPOINT_INFO* pEndpoint, BYTE* pControlData, WORD controlSize, BYTE* pData, WORD size);
#define _USB_InitControlWrite_Expect(pEndpoint, pControlData, controlSize, pData, size) _USB_InitControlWrite_CMockExpect(__LINE__, pEndpoint, pControlData, controlSize, pData, size)
void _USB_InitControlWrite_CMockExpect(UNITY_LINE_TYPE cmock_line, USB_ENDPOINT_INFO* pEndpoint, BYTE* pControlData, WORD controlSize, BYTE* pData, WORD size);
#define _USB_InitRead_Expect(pEndpoint, pData, size) _USB_InitRead_CMockExpect(__LINE__, pEndpoint, pData, size)
void _USB_InitRead_CMockExpect(UNITY_LINE_TYPE cmock_line, USB_ENDPOINT_INFO* pEndpoint, BYTE* pData, WORD size);
#define _USB_InitWrite_Expect(pEndpoint, pData, size) _USB_InitWrite_CMockExpect(__LINE__, pEndpoint, pData, size)
void _USB_InitWrite_CMockExpect(UNITY_LINE_TYPE cmock_line, USB_ENDPOINT_INFO* pEndpoint, BYTE* pData, WORD size);
#define _USB_NotifyClients_Expect(DevAddress, event, data, size) _USB_NotifyClients_CMockExpect(__LINE__, DevAddress, event, data, size)
void _USB_NotifyClients_CMockExpect(UNITY_LINE_TYPE cmock_line, BYTE DevAddress, USB_EVENT event, void* data, unsigned int size);
#define _USB_ParseConfigurationDescriptor_ExpectAndReturn(cmock_retval) _USB_ParseConfigurationDescriptor_CMockExpectAndReturn(__LINE__, cmock_retval)
void _USB_ParseConfigurationDescriptor_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, BOOL cmock_to_return);
#define _USB_ResetDATA0_Expect(endpoint) _USB_ResetDATA0_CMockExpect(__LINE__, endpoint)
void _USB_ResetDATA0_CMockExpect(UNITY_LINE_TYPE cmock_line, BYTE endpoint);
#define _USB_SendToken_Expect(endpoint, tokenType) _USB_SendToken_CMockExpect(__LINE__, endpoint, tokenType)
void _USB_SendToken_CMockExpect(UNITY_LINE_TYPE cmock_line, BYTE endpoint, BYTE tokenType);
#define _USB_SetBDT_Expect(direction) _USB_SetBDT_CMockExpect(__LINE__, direction)
void _USB_SetBDT_CMockExpect(UNITY_LINE_TYPE cmock_line, BYTE direction);
#define _USB_TransferInProgress_ExpectAndReturn(cmock_retval) _USB_TransferInProgress_CMockExpectAndReturn(__LINE__, cmock_retval)
void _USB_TransferInProgress_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, BOOL cmock_to_return);

#endif
