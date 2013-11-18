/* AUTOGENERATED FILE. DO NOT EDIT. */
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "unity.h"
#include "cmock.h"
#include "Mocki2cConfig.h"

typedef struct _CMOCK_setupi2c2_peripherals_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;

} CMOCK_setupi2c2_peripherals_CALL_INSTANCE;

typedef struct _CMOCK_setupi2c2_main_adc_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;

} CMOCK_setupi2c2_main_adc_CALL_INSTANCE;

typedef struct _CMOCK_setupi2c2_main_dac_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;

} CMOCK_setupi2c2_main_dac_CALL_INSTANCE;

typedef struct _CMOCK_setupi2c2_main_io_audio_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;

} CMOCK_setupi2c2_main_io_audio_CALL_INSTANCE;

typedef struct _CMOCK_setupi2c2_main_io_buttons_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;

} CMOCK_setupi2c2_main_io_buttons_CALL_INSTANCE;

typedef struct _CMOCK_setupi2c2_hv_io_0x48_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;

} CMOCK_setupi2c2_hv_io_0x48_CALL_INSTANCE;

typedef struct _CMOCK_setupi2c2_hv_dac_0xc0_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;

} CMOCK_setupi2c2_hv_dac_0xc0_CALL_INSTANCE;

typedef struct _CMOCK_setupi2c2_x_temp_0x90_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;

} CMOCK_setupi2c2_x_temp_0x90_CALL_INSTANCE;

typedef struct _CMOCK_setupi2c2_x_pres_0x28_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;

} CMOCK_setupi2c2_x_pres_0x28_CALL_INSTANCE;

typedef struct _CMOCK_i2cRead_PIC18_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;

} CMOCK_i2cRead_PIC18_CALL_INSTANCE;

typedef struct _CMOCK_getVolume_i2c_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  UINT16 ReturnVal;
  UINT8 Expected_DeviceAddress;
  UINT8 Expected_DeviceRegister;

} CMOCK_getVolume_i2c_CALL_INSTANCE;

typedef struct _CMOCK_setVolume_i2c_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  UINT8 Expected_DeviceAddress;
  UINT8 Expected_DeviceRegister;
  UINT16 Expected_volume;

} CMOCK_setVolume_i2c_CALL_INSTANCE;

static struct Mocki2cConfigInstance
{
  CMOCK_MEM_INDEX_TYPE setupi2c2_peripherals_CallInstance;
  CMOCK_MEM_INDEX_TYPE setupi2c2_main_adc_CallInstance;
  CMOCK_MEM_INDEX_TYPE setupi2c2_main_dac_CallInstance;
  CMOCK_MEM_INDEX_TYPE setupi2c2_main_io_audio_CallInstance;
  CMOCK_MEM_INDEX_TYPE setupi2c2_main_io_buttons_CallInstance;
  CMOCK_MEM_INDEX_TYPE setupi2c2_hv_io_0x48_CallInstance;
  CMOCK_MEM_INDEX_TYPE setupi2c2_hv_dac_0xc0_CallInstance;
  CMOCK_MEM_INDEX_TYPE setupi2c2_x_temp_0x90_CallInstance;
  CMOCK_MEM_INDEX_TYPE setupi2c2_x_pres_0x28_CallInstance;
  CMOCK_MEM_INDEX_TYPE i2cRead_PIC18_CallInstance;
  CMOCK_MEM_INDEX_TYPE getVolume_i2c_CallInstance;
  CMOCK_MEM_INDEX_TYPE setVolume_i2c_CallInstance;
} Mock;

extern jmp_buf AbortFrame;

void Mocki2cConfig_Verify(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.setupi2c2_peripherals_CallInstance, cmock_line, "Function 'setupi2c2_peripherals' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.setupi2c2_main_adc_CallInstance, cmock_line, "Function 'setupi2c2_main_adc' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.setupi2c2_main_dac_CallInstance, cmock_line, "Function 'setupi2c2_main_dac' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.setupi2c2_main_io_audio_CallInstance, cmock_line, "Function 'setupi2c2_main_io_audio' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.setupi2c2_main_io_buttons_CallInstance, cmock_line, "Function 'setupi2c2_main_io_buttons' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.setupi2c2_hv_io_0x48_CallInstance, cmock_line, "Function 'setupi2c2_hv_io_0x48' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.setupi2c2_hv_dac_0xc0_CallInstance, cmock_line, "Function 'setupi2c2_hv_dac_0xc0' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.setupi2c2_x_temp_0x90_CallInstance, cmock_line, "Function 'setupi2c2_x_temp_0x90' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.setupi2c2_x_pres_0x28_CallInstance, cmock_line, "Function 'setupi2c2_x_pres_0x28' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.i2cRead_PIC18_CallInstance, cmock_line, "Function 'i2cRead_PIC18' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.getVolume_i2c_CallInstance, cmock_line, "Function 'getVolume_i2c' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.setVolume_i2c_CallInstance, cmock_line, "Function 'setVolume_i2c' called less times than expected.");
}

void Mocki2cConfig_Init(void)
{
  Mocki2cConfig_Destroy();
}

void Mocki2cConfig_Destroy(void)
{
  CMock_Guts_MemFreeAll();
  memset(&Mock, 0, sizeof(Mock));
}

void setupi2c2_peripherals(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_setupi2c2_peripherals_CALL_INSTANCE* cmock_call_instance = (CMOCK_setupi2c2_peripherals_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.setupi2c2_peripherals_CallInstance);
  Mock.setupi2c2_peripherals_CallInstance = CMock_Guts_MemNext(Mock.setupi2c2_peripherals_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'setupi2c2_peripherals' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
}

void setupi2c2_peripherals_CMockExpect(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_setupi2c2_peripherals_CALL_INSTANCE));
  CMOCK_setupi2c2_peripherals_CALL_INSTANCE* cmock_call_instance = (CMOCK_setupi2c2_peripherals_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.setupi2c2_peripherals_CallInstance = CMock_Guts_MemChain(Mock.setupi2c2_peripherals_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
}

void setupi2c2_main_adc(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_setupi2c2_main_adc_CALL_INSTANCE* cmock_call_instance = (CMOCK_setupi2c2_main_adc_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.setupi2c2_main_adc_CallInstance);
  Mock.setupi2c2_main_adc_CallInstance = CMock_Guts_MemNext(Mock.setupi2c2_main_adc_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'setupi2c2_main_adc' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
}

void setupi2c2_main_adc_CMockExpect(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_setupi2c2_main_adc_CALL_INSTANCE));
  CMOCK_setupi2c2_main_adc_CALL_INSTANCE* cmock_call_instance = (CMOCK_setupi2c2_main_adc_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.setupi2c2_main_adc_CallInstance = CMock_Guts_MemChain(Mock.setupi2c2_main_adc_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
}

void setupi2c2_main_dac(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_setupi2c2_main_dac_CALL_INSTANCE* cmock_call_instance = (CMOCK_setupi2c2_main_dac_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.setupi2c2_main_dac_CallInstance);
  Mock.setupi2c2_main_dac_CallInstance = CMock_Guts_MemNext(Mock.setupi2c2_main_dac_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'setupi2c2_main_dac' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
}

void setupi2c2_main_dac_CMockExpect(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_setupi2c2_main_dac_CALL_INSTANCE));
  CMOCK_setupi2c2_main_dac_CALL_INSTANCE* cmock_call_instance = (CMOCK_setupi2c2_main_dac_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.setupi2c2_main_dac_CallInstance = CMock_Guts_MemChain(Mock.setupi2c2_main_dac_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
}

void setupi2c2_main_io_audio(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_setupi2c2_main_io_audio_CALL_INSTANCE* cmock_call_instance = (CMOCK_setupi2c2_main_io_audio_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.setupi2c2_main_io_audio_CallInstance);
  Mock.setupi2c2_main_io_audio_CallInstance = CMock_Guts_MemNext(Mock.setupi2c2_main_io_audio_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'setupi2c2_main_io_audio' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
}

void setupi2c2_main_io_audio_CMockExpect(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_setupi2c2_main_io_audio_CALL_INSTANCE));
  CMOCK_setupi2c2_main_io_audio_CALL_INSTANCE* cmock_call_instance = (CMOCK_setupi2c2_main_io_audio_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.setupi2c2_main_io_audio_CallInstance = CMock_Guts_MemChain(Mock.setupi2c2_main_io_audio_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
}

void setupi2c2_main_io_buttons(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_setupi2c2_main_io_buttons_CALL_INSTANCE* cmock_call_instance = (CMOCK_setupi2c2_main_io_buttons_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.setupi2c2_main_io_buttons_CallInstance);
  Mock.setupi2c2_main_io_buttons_CallInstance = CMock_Guts_MemNext(Mock.setupi2c2_main_io_buttons_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'setupi2c2_main_io_buttons' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
}

void setupi2c2_main_io_buttons_CMockExpect(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_setupi2c2_main_io_buttons_CALL_INSTANCE));
  CMOCK_setupi2c2_main_io_buttons_CALL_INSTANCE* cmock_call_instance = (CMOCK_setupi2c2_main_io_buttons_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.setupi2c2_main_io_buttons_CallInstance = CMock_Guts_MemChain(Mock.setupi2c2_main_io_buttons_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
}

void setupi2c2_hv_io_0x48(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_setupi2c2_hv_io_0x48_CALL_INSTANCE* cmock_call_instance = (CMOCK_setupi2c2_hv_io_0x48_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.setupi2c2_hv_io_0x48_CallInstance);
  Mock.setupi2c2_hv_io_0x48_CallInstance = CMock_Guts_MemNext(Mock.setupi2c2_hv_io_0x48_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'setupi2c2_hv_io_0x48' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
}

void setupi2c2_hv_io_0x48_CMockExpect(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_setupi2c2_hv_io_0x48_CALL_INSTANCE));
  CMOCK_setupi2c2_hv_io_0x48_CALL_INSTANCE* cmock_call_instance = (CMOCK_setupi2c2_hv_io_0x48_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.setupi2c2_hv_io_0x48_CallInstance = CMock_Guts_MemChain(Mock.setupi2c2_hv_io_0x48_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
}

void setupi2c2_hv_dac_0xc0(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_setupi2c2_hv_dac_0xc0_CALL_INSTANCE* cmock_call_instance = (CMOCK_setupi2c2_hv_dac_0xc0_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.setupi2c2_hv_dac_0xc0_CallInstance);
  Mock.setupi2c2_hv_dac_0xc0_CallInstance = CMock_Guts_MemNext(Mock.setupi2c2_hv_dac_0xc0_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'setupi2c2_hv_dac_0xc0' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
}

void setupi2c2_hv_dac_0xc0_CMockExpect(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_setupi2c2_hv_dac_0xc0_CALL_INSTANCE));
  CMOCK_setupi2c2_hv_dac_0xc0_CALL_INSTANCE* cmock_call_instance = (CMOCK_setupi2c2_hv_dac_0xc0_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.setupi2c2_hv_dac_0xc0_CallInstance = CMock_Guts_MemChain(Mock.setupi2c2_hv_dac_0xc0_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
}

void setupi2c2_x_temp_0x90(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_setupi2c2_x_temp_0x90_CALL_INSTANCE* cmock_call_instance = (CMOCK_setupi2c2_x_temp_0x90_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.setupi2c2_x_temp_0x90_CallInstance);
  Mock.setupi2c2_x_temp_0x90_CallInstance = CMock_Guts_MemNext(Mock.setupi2c2_x_temp_0x90_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'setupi2c2_x_temp_0x90' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
}

void setupi2c2_x_temp_0x90_CMockExpect(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_setupi2c2_x_temp_0x90_CALL_INSTANCE));
  CMOCK_setupi2c2_x_temp_0x90_CALL_INSTANCE* cmock_call_instance = (CMOCK_setupi2c2_x_temp_0x90_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.setupi2c2_x_temp_0x90_CallInstance = CMock_Guts_MemChain(Mock.setupi2c2_x_temp_0x90_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
}

void setupi2c2_x_pres_0x28(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_setupi2c2_x_pres_0x28_CALL_INSTANCE* cmock_call_instance = (CMOCK_setupi2c2_x_pres_0x28_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.setupi2c2_x_pres_0x28_CallInstance);
  Mock.setupi2c2_x_pres_0x28_CallInstance = CMock_Guts_MemNext(Mock.setupi2c2_x_pres_0x28_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'setupi2c2_x_pres_0x28' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
}

void setupi2c2_x_pres_0x28_CMockExpect(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_setupi2c2_x_pres_0x28_CALL_INSTANCE));
  CMOCK_setupi2c2_x_pres_0x28_CALL_INSTANCE* cmock_call_instance = (CMOCK_setupi2c2_x_pres_0x28_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.setupi2c2_x_pres_0x28_CallInstance = CMock_Guts_MemChain(Mock.setupi2c2_x_pres_0x28_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
}

void i2cRead_PIC18(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_i2cRead_PIC18_CALL_INSTANCE* cmock_call_instance = (CMOCK_i2cRead_PIC18_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.i2cRead_PIC18_CallInstance);
  Mock.i2cRead_PIC18_CallInstance = CMock_Guts_MemNext(Mock.i2cRead_PIC18_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'i2cRead_PIC18' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
}

void i2cRead_PIC18_CMockExpect(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_i2cRead_PIC18_CALL_INSTANCE));
  CMOCK_i2cRead_PIC18_CALL_INSTANCE* cmock_call_instance = (CMOCK_i2cRead_PIC18_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.i2cRead_PIC18_CallInstance = CMock_Guts_MemChain(Mock.i2cRead_PIC18_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
}

UINT16 getVolume_i2c(UINT8 DeviceAddress, UINT8 DeviceRegister)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_getVolume_i2c_CALL_INSTANCE* cmock_call_instance = (CMOCK_getVolume_i2c_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.getVolume_i2c_CallInstance);
  Mock.getVolume_i2c_CallInstance = CMock_Guts_MemNext(Mock.getVolume_i2c_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'getVolume_i2c' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_HEX8(cmock_call_instance->Expected_DeviceAddress, DeviceAddress, cmock_line, "Function 'getVolume_i2c' called with unexpected value for argument 'DeviceAddress'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_HEX8(cmock_call_instance->Expected_DeviceRegister, DeviceRegister, cmock_line, "Function 'getVolume_i2c' called with unexpected value for argument 'DeviceRegister'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_getVolume_i2c(CMOCK_getVolume_i2c_CALL_INSTANCE* cmock_call_instance, UINT8 DeviceAddress, UINT8 DeviceRegister)
{
  cmock_call_instance->Expected_DeviceAddress = DeviceAddress;
  cmock_call_instance->Expected_DeviceRegister = DeviceRegister;
}

void getVolume_i2c_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, UINT8 DeviceAddress, UINT8 DeviceRegister, UINT16 cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_getVolume_i2c_CALL_INSTANCE));
  CMOCK_getVolume_i2c_CALL_INSTANCE* cmock_call_instance = (CMOCK_getVolume_i2c_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.getVolume_i2c_CallInstance = CMock_Guts_MemChain(Mock.getVolume_i2c_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_getVolume_i2c(cmock_call_instance, DeviceAddress, DeviceRegister);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

void setVolume_i2c(UINT8 DeviceAddress, UINT8 DeviceRegister, UINT16 volume)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_setVolume_i2c_CALL_INSTANCE* cmock_call_instance = (CMOCK_setVolume_i2c_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.setVolume_i2c_CallInstance);
  Mock.setVolume_i2c_CallInstance = CMock_Guts_MemNext(Mock.setVolume_i2c_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'setVolume_i2c' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_HEX8(cmock_call_instance->Expected_DeviceAddress, DeviceAddress, cmock_line, "Function 'setVolume_i2c' called with unexpected value for argument 'DeviceAddress'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_HEX8(cmock_call_instance->Expected_DeviceRegister, DeviceRegister, cmock_line, "Function 'setVolume_i2c' called with unexpected value for argument 'DeviceRegister'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_HEX16(cmock_call_instance->Expected_volume, volume, cmock_line, "Function 'setVolume_i2c' called with unexpected value for argument 'volume'.");
  }
}

void CMockExpectParameters_setVolume_i2c(CMOCK_setVolume_i2c_CALL_INSTANCE* cmock_call_instance, UINT8 DeviceAddress, UINT8 DeviceRegister, UINT16 volume)
{
  cmock_call_instance->Expected_DeviceAddress = DeviceAddress;
  cmock_call_instance->Expected_DeviceRegister = DeviceRegister;
  cmock_call_instance->Expected_volume = volume;
}

void setVolume_i2c_CMockExpect(UNITY_LINE_TYPE cmock_line, UINT8 DeviceAddress, UINT8 DeviceRegister, UINT16 volume)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_setVolume_i2c_CALL_INSTANCE));
  CMOCK_setVolume_i2c_CALL_INSTANCE* cmock_call_instance = (CMOCK_setVolume_i2c_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.setVolume_i2c_CallInstance = CMock_Guts_MemChain(Mock.setVolume_i2c_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_setVolume_i2c(cmock_call_instance, DeviceAddress, DeviceRegister, volume);
}
