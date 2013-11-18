/* AUTOGENERATED FILE. DO NOT EDIT. */
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "unity.h"
#include "cmock.h"
#include "MockIcons.h"

typedef struct _CMOCK_Clear_Icon_Window_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;

} CMOCK_Clear_Icon_Window_CALL_INSTANCE;

typedef struct _CMOCK_New_Icon_Window_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;

} CMOCK_New_Icon_Window_CALL_INSTANCE;

typedef struct _CMOCK_Update_Icon_Window_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  UINT8 Expected_update_type;

} CMOCK_Update_Icon_Window_CALL_INSTANCE;

typedef struct _CMOCK_Update_Audio_Icon_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  UINT8 Expected_update_type;

} CMOCK_Update_Audio_Icon_CALL_INSTANCE;

typedef struct _CMOCK_Update_Backlight_Icon_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  UINT8 Expected_update_type;

} CMOCK_Update_Backlight_Icon_CALL_INSTANCE;

typedef struct _CMOCK_Update_Battery_Icon_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  UINT8 Expected_update_type;

} CMOCK_Update_Battery_Icon_CALL_INSTANCE;

typedef struct _CMOCK_Update_No_Attached_Icon_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  UINT8 Expected_update_type;

} CMOCK_Update_No_Attached_Icon_CALL_INSTANCE;

typedef struct _CMOCK_Update_PC_Icon_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  UINT8 Expected_update_type;

} CMOCK_Update_PC_Icon_CALL_INSTANCE;

typedef struct _CMOCK_Update_Keyboard_Icon_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  UINT8 Expected_update_type;

} CMOCK_Update_Keyboard_Icon_CALL_INSTANCE;

typedef struct _CMOCK_Update_Thumbdrive_Icon_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  UINT8 Expected_update_type;

} CMOCK_Update_Thumbdrive_Icon_CALL_INSTANCE;

typedef struct _CMOCK_Update_Range_Icon_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  UINT8 Expected_update_type;

} CMOCK_Update_Range_Icon_CALL_INSTANCE;

typedef struct _CMOCK_Update_Correction_Factor_Icon_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  UINT8 Expected_update_type;

} CMOCK_Update_Correction_Factor_Icon_CALL_INSTANCE;

typedef struct _CMOCK_Color_Battery_Edges_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  SHORT Expected_Xaxis;
  SHORT Expected_Yaxis;

} CMOCK_Color_Battery_Edges_CALL_INSTANCE;

typedef struct _CMOCK_Color_Framed_Corners_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  SHORT Expected_Xaxis;
  SHORT Expected_Yaxis;

} CMOCK_Color_Framed_Corners_CALL_INSTANCE;

static struct MockIconsInstance
{
  CMOCK_MEM_INDEX_TYPE Clear_Icon_Window_CallInstance;
  CMOCK_MEM_INDEX_TYPE New_Icon_Window_CallInstance;
  CMOCK_MEM_INDEX_TYPE Update_Icon_Window_CallInstance;
  CMOCK_MEM_INDEX_TYPE Update_Audio_Icon_CallInstance;
  CMOCK_MEM_INDEX_TYPE Update_Backlight_Icon_CallInstance;
  CMOCK_MEM_INDEX_TYPE Update_Battery_Icon_CallInstance;
  CMOCK_MEM_INDEX_TYPE Update_No_Attached_Icon_CallInstance;
  CMOCK_MEM_INDEX_TYPE Update_PC_Icon_CallInstance;
  CMOCK_MEM_INDEX_TYPE Update_Keyboard_Icon_CallInstance;
  CMOCK_MEM_INDEX_TYPE Update_Thumbdrive_Icon_CallInstance;
  CMOCK_MEM_INDEX_TYPE Update_Range_Icon_CallInstance;
  CMOCK_MEM_INDEX_TYPE Update_Correction_Factor_Icon_CallInstance;
  CMOCK_MEM_INDEX_TYPE Color_Battery_Edges_CallInstance;
  CMOCK_MEM_INDEX_TYPE Color_Framed_Corners_CallInstance;
} Mock;

extern jmp_buf AbortFrame;

void MockIcons_Verify(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.Clear_Icon_Window_CallInstance, cmock_line, "Function 'Clear_Icon_Window' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.New_Icon_Window_CallInstance, cmock_line, "Function 'New_Icon_Window' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.Update_Icon_Window_CallInstance, cmock_line, "Function 'Update_Icon_Window' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.Update_Audio_Icon_CallInstance, cmock_line, "Function 'Update_Audio_Icon' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.Update_Backlight_Icon_CallInstance, cmock_line, "Function 'Update_Backlight_Icon' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.Update_Battery_Icon_CallInstance, cmock_line, "Function 'Update_Battery_Icon' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.Update_No_Attached_Icon_CallInstance, cmock_line, "Function 'Update_No_Attached_Icon' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.Update_PC_Icon_CallInstance, cmock_line, "Function 'Update_PC_Icon' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.Update_Keyboard_Icon_CallInstance, cmock_line, "Function 'Update_Keyboard_Icon' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.Update_Thumbdrive_Icon_CallInstance, cmock_line, "Function 'Update_Thumbdrive_Icon' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.Update_Range_Icon_CallInstance, cmock_line, "Function 'Update_Range_Icon' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.Update_Correction_Factor_Icon_CallInstance, cmock_line, "Function 'Update_Correction_Factor_Icon' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.Color_Battery_Edges_CallInstance, cmock_line, "Function 'Color_Battery_Edges' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.Color_Framed_Corners_CallInstance, cmock_line, "Function 'Color_Framed_Corners' called less times than expected.");
}

void MockIcons_Init(void)
{
  MockIcons_Destroy();
}

void MockIcons_Destroy(void)
{
  CMock_Guts_MemFreeAll();
  memset(&Mock, 0, sizeof(Mock));
}

void Clear_Icon_Window(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_Clear_Icon_Window_CALL_INSTANCE* cmock_call_instance = (CMOCK_Clear_Icon_Window_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.Clear_Icon_Window_CallInstance);
  Mock.Clear_Icon_Window_CallInstance = CMock_Guts_MemNext(Mock.Clear_Icon_Window_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'Clear_Icon_Window' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
}

void Clear_Icon_Window_CMockExpect(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_Clear_Icon_Window_CALL_INSTANCE));
  CMOCK_Clear_Icon_Window_CALL_INSTANCE* cmock_call_instance = (CMOCK_Clear_Icon_Window_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.Clear_Icon_Window_CallInstance = CMock_Guts_MemChain(Mock.Clear_Icon_Window_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
}

void New_Icon_Window(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_New_Icon_Window_CALL_INSTANCE* cmock_call_instance = (CMOCK_New_Icon_Window_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.New_Icon_Window_CallInstance);
  Mock.New_Icon_Window_CallInstance = CMock_Guts_MemNext(Mock.New_Icon_Window_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'New_Icon_Window' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
}

void New_Icon_Window_CMockExpect(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_New_Icon_Window_CALL_INSTANCE));
  CMOCK_New_Icon_Window_CALL_INSTANCE* cmock_call_instance = (CMOCK_New_Icon_Window_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.New_Icon_Window_CallInstance = CMock_Guts_MemChain(Mock.New_Icon_Window_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
}

void Update_Icon_Window(UINT8 update_type)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_Update_Icon_Window_CALL_INSTANCE* cmock_call_instance = (CMOCK_Update_Icon_Window_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.Update_Icon_Window_CallInstance);
  Mock.Update_Icon_Window_CallInstance = CMock_Guts_MemNext(Mock.Update_Icon_Window_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'Update_Icon_Window' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_HEX8(cmock_call_instance->Expected_update_type, update_type, cmock_line, "Function 'Update_Icon_Window' called with unexpected value for argument 'update_type'.");
  }
}

void CMockExpectParameters_Update_Icon_Window(CMOCK_Update_Icon_Window_CALL_INSTANCE* cmock_call_instance, UINT8 update_type)
{
  cmock_call_instance->Expected_update_type = update_type;
}

void Update_Icon_Window_CMockExpect(UNITY_LINE_TYPE cmock_line, UINT8 update_type)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_Update_Icon_Window_CALL_INSTANCE));
  CMOCK_Update_Icon_Window_CALL_INSTANCE* cmock_call_instance = (CMOCK_Update_Icon_Window_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.Update_Icon_Window_CallInstance = CMock_Guts_MemChain(Mock.Update_Icon_Window_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_Update_Icon_Window(cmock_call_instance, update_type);
}

void Update_Audio_Icon(UINT8 update_type)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_Update_Audio_Icon_CALL_INSTANCE* cmock_call_instance = (CMOCK_Update_Audio_Icon_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.Update_Audio_Icon_CallInstance);
  Mock.Update_Audio_Icon_CallInstance = CMock_Guts_MemNext(Mock.Update_Audio_Icon_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'Update_Audio_Icon' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_HEX8(cmock_call_instance->Expected_update_type, update_type, cmock_line, "Function 'Update_Audio_Icon' called with unexpected value for argument 'update_type'.");
  }
}

void CMockExpectParameters_Update_Audio_Icon(CMOCK_Update_Audio_Icon_CALL_INSTANCE* cmock_call_instance, UINT8 update_type)
{
  cmock_call_instance->Expected_update_type = update_type;
}

void Update_Audio_Icon_CMockExpect(UNITY_LINE_TYPE cmock_line, UINT8 update_type)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_Update_Audio_Icon_CALL_INSTANCE));
  CMOCK_Update_Audio_Icon_CALL_INSTANCE* cmock_call_instance = (CMOCK_Update_Audio_Icon_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.Update_Audio_Icon_CallInstance = CMock_Guts_MemChain(Mock.Update_Audio_Icon_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_Update_Audio_Icon(cmock_call_instance, update_type);
}

void Update_Backlight_Icon(UINT8 update_type)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_Update_Backlight_Icon_CALL_INSTANCE* cmock_call_instance = (CMOCK_Update_Backlight_Icon_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.Update_Backlight_Icon_CallInstance);
  Mock.Update_Backlight_Icon_CallInstance = CMock_Guts_MemNext(Mock.Update_Backlight_Icon_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'Update_Backlight_Icon' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_HEX8(cmock_call_instance->Expected_update_type, update_type, cmock_line, "Function 'Update_Backlight_Icon' called with unexpected value for argument 'update_type'.");
  }
}

void CMockExpectParameters_Update_Backlight_Icon(CMOCK_Update_Backlight_Icon_CALL_INSTANCE* cmock_call_instance, UINT8 update_type)
{
  cmock_call_instance->Expected_update_type = update_type;
}

void Update_Backlight_Icon_CMockExpect(UNITY_LINE_TYPE cmock_line, UINT8 update_type)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_Update_Backlight_Icon_CALL_INSTANCE));
  CMOCK_Update_Backlight_Icon_CALL_INSTANCE* cmock_call_instance = (CMOCK_Update_Backlight_Icon_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.Update_Backlight_Icon_CallInstance = CMock_Guts_MemChain(Mock.Update_Backlight_Icon_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_Update_Backlight_Icon(cmock_call_instance, update_type);
}

void Update_Battery_Icon(UINT8 update_type)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_Update_Battery_Icon_CALL_INSTANCE* cmock_call_instance = (CMOCK_Update_Battery_Icon_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.Update_Battery_Icon_CallInstance);
  Mock.Update_Battery_Icon_CallInstance = CMock_Guts_MemNext(Mock.Update_Battery_Icon_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'Update_Battery_Icon' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_HEX8(cmock_call_instance->Expected_update_type, update_type, cmock_line, "Function 'Update_Battery_Icon' called with unexpected value for argument 'update_type'.");
  }
}

void CMockExpectParameters_Update_Battery_Icon(CMOCK_Update_Battery_Icon_CALL_INSTANCE* cmock_call_instance, UINT8 update_type)
{
  cmock_call_instance->Expected_update_type = update_type;
}

void Update_Battery_Icon_CMockExpect(UNITY_LINE_TYPE cmock_line, UINT8 update_type)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_Update_Battery_Icon_CALL_INSTANCE));
  CMOCK_Update_Battery_Icon_CALL_INSTANCE* cmock_call_instance = (CMOCK_Update_Battery_Icon_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.Update_Battery_Icon_CallInstance = CMock_Guts_MemChain(Mock.Update_Battery_Icon_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_Update_Battery_Icon(cmock_call_instance, update_type);
}

void Update_No_Attached_Icon(UINT8 update_type)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_Update_No_Attached_Icon_CALL_INSTANCE* cmock_call_instance = (CMOCK_Update_No_Attached_Icon_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.Update_No_Attached_Icon_CallInstance);
  Mock.Update_No_Attached_Icon_CallInstance = CMock_Guts_MemNext(Mock.Update_No_Attached_Icon_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'Update_No_Attached_Icon' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_HEX8(cmock_call_instance->Expected_update_type, update_type, cmock_line, "Function 'Update_No_Attached_Icon' called with unexpected value for argument 'update_type'.");
  }
}

void CMockExpectParameters_Update_No_Attached_Icon(CMOCK_Update_No_Attached_Icon_CALL_INSTANCE* cmock_call_instance, UINT8 update_type)
{
  cmock_call_instance->Expected_update_type = update_type;
}

void Update_No_Attached_Icon_CMockExpect(UNITY_LINE_TYPE cmock_line, UINT8 update_type)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_Update_No_Attached_Icon_CALL_INSTANCE));
  CMOCK_Update_No_Attached_Icon_CALL_INSTANCE* cmock_call_instance = (CMOCK_Update_No_Attached_Icon_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.Update_No_Attached_Icon_CallInstance = CMock_Guts_MemChain(Mock.Update_No_Attached_Icon_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_Update_No_Attached_Icon(cmock_call_instance, update_type);
}

void Update_PC_Icon(UINT8 update_type)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_Update_PC_Icon_CALL_INSTANCE* cmock_call_instance = (CMOCK_Update_PC_Icon_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.Update_PC_Icon_CallInstance);
  Mock.Update_PC_Icon_CallInstance = CMock_Guts_MemNext(Mock.Update_PC_Icon_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'Update_PC_Icon' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_HEX8(cmock_call_instance->Expected_update_type, update_type, cmock_line, "Function 'Update_PC_Icon' called with unexpected value for argument 'update_type'.");
  }
}

void CMockExpectParameters_Update_PC_Icon(CMOCK_Update_PC_Icon_CALL_INSTANCE* cmock_call_instance, UINT8 update_type)
{
  cmock_call_instance->Expected_update_type = update_type;
}

void Update_PC_Icon_CMockExpect(UNITY_LINE_TYPE cmock_line, UINT8 update_type)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_Update_PC_Icon_CALL_INSTANCE));
  CMOCK_Update_PC_Icon_CALL_INSTANCE* cmock_call_instance = (CMOCK_Update_PC_Icon_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.Update_PC_Icon_CallInstance = CMock_Guts_MemChain(Mock.Update_PC_Icon_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_Update_PC_Icon(cmock_call_instance, update_type);
}

void Update_Keyboard_Icon(UINT8 update_type)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_Update_Keyboard_Icon_CALL_INSTANCE* cmock_call_instance = (CMOCK_Update_Keyboard_Icon_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.Update_Keyboard_Icon_CallInstance);
  Mock.Update_Keyboard_Icon_CallInstance = CMock_Guts_MemNext(Mock.Update_Keyboard_Icon_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'Update_Keyboard_Icon' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_HEX8(cmock_call_instance->Expected_update_type, update_type, cmock_line, "Function 'Update_Keyboard_Icon' called with unexpected value for argument 'update_type'.");
  }
}

void CMockExpectParameters_Update_Keyboard_Icon(CMOCK_Update_Keyboard_Icon_CALL_INSTANCE* cmock_call_instance, UINT8 update_type)
{
  cmock_call_instance->Expected_update_type = update_type;
}

void Update_Keyboard_Icon_CMockExpect(UNITY_LINE_TYPE cmock_line, UINT8 update_type)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_Update_Keyboard_Icon_CALL_INSTANCE));
  CMOCK_Update_Keyboard_Icon_CALL_INSTANCE* cmock_call_instance = (CMOCK_Update_Keyboard_Icon_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.Update_Keyboard_Icon_CallInstance = CMock_Guts_MemChain(Mock.Update_Keyboard_Icon_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_Update_Keyboard_Icon(cmock_call_instance, update_type);
}

void Update_Thumbdrive_Icon(UINT8 update_type)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_Update_Thumbdrive_Icon_CALL_INSTANCE* cmock_call_instance = (CMOCK_Update_Thumbdrive_Icon_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.Update_Thumbdrive_Icon_CallInstance);
  Mock.Update_Thumbdrive_Icon_CallInstance = CMock_Guts_MemNext(Mock.Update_Thumbdrive_Icon_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'Update_Thumbdrive_Icon' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_HEX8(cmock_call_instance->Expected_update_type, update_type, cmock_line, "Function 'Update_Thumbdrive_Icon' called with unexpected value for argument 'update_type'.");
  }
}

void CMockExpectParameters_Update_Thumbdrive_Icon(CMOCK_Update_Thumbdrive_Icon_CALL_INSTANCE* cmock_call_instance, UINT8 update_type)
{
  cmock_call_instance->Expected_update_type = update_type;
}

void Update_Thumbdrive_Icon_CMockExpect(UNITY_LINE_TYPE cmock_line, UINT8 update_type)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_Update_Thumbdrive_Icon_CALL_INSTANCE));
  CMOCK_Update_Thumbdrive_Icon_CALL_INSTANCE* cmock_call_instance = (CMOCK_Update_Thumbdrive_Icon_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.Update_Thumbdrive_Icon_CallInstance = CMock_Guts_MemChain(Mock.Update_Thumbdrive_Icon_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_Update_Thumbdrive_Icon(cmock_call_instance, update_type);
}

void Update_Range_Icon(UINT8 update_type)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_Update_Range_Icon_CALL_INSTANCE* cmock_call_instance = (CMOCK_Update_Range_Icon_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.Update_Range_Icon_CallInstance);
  Mock.Update_Range_Icon_CallInstance = CMock_Guts_MemNext(Mock.Update_Range_Icon_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'Update_Range_Icon' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_HEX8(cmock_call_instance->Expected_update_type, update_type, cmock_line, "Function 'Update_Range_Icon' called with unexpected value for argument 'update_type'.");
  }
}

void CMockExpectParameters_Update_Range_Icon(CMOCK_Update_Range_Icon_CALL_INSTANCE* cmock_call_instance, UINT8 update_type)
{
  cmock_call_instance->Expected_update_type = update_type;
}

void Update_Range_Icon_CMockExpect(UNITY_LINE_TYPE cmock_line, UINT8 update_type)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_Update_Range_Icon_CALL_INSTANCE));
  CMOCK_Update_Range_Icon_CALL_INSTANCE* cmock_call_instance = (CMOCK_Update_Range_Icon_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.Update_Range_Icon_CallInstance = CMock_Guts_MemChain(Mock.Update_Range_Icon_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_Update_Range_Icon(cmock_call_instance, update_type);
}

void Update_Correction_Factor_Icon(UINT8 update_type)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_Update_Correction_Factor_Icon_CALL_INSTANCE* cmock_call_instance = (CMOCK_Update_Correction_Factor_Icon_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.Update_Correction_Factor_Icon_CallInstance);
  Mock.Update_Correction_Factor_Icon_CallInstance = CMock_Guts_MemNext(Mock.Update_Correction_Factor_Icon_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'Update_Correction_Factor_Icon' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_HEX8(cmock_call_instance->Expected_update_type, update_type, cmock_line, "Function 'Update_Correction_Factor_Icon' called with unexpected value for argument 'update_type'.");
  }
}

void CMockExpectParameters_Update_Correction_Factor_Icon(CMOCK_Update_Correction_Factor_Icon_CALL_INSTANCE* cmock_call_instance, UINT8 update_type)
{
  cmock_call_instance->Expected_update_type = update_type;
}

void Update_Correction_Factor_Icon_CMockExpect(UNITY_LINE_TYPE cmock_line, UINT8 update_type)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_Update_Correction_Factor_Icon_CALL_INSTANCE));
  CMOCK_Update_Correction_Factor_Icon_CALL_INSTANCE* cmock_call_instance = (CMOCK_Update_Correction_Factor_Icon_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.Update_Correction_Factor_Icon_CallInstance = CMock_Guts_MemChain(Mock.Update_Correction_Factor_Icon_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_Update_Correction_Factor_Icon(cmock_call_instance, update_type);
}

void Color_Battery_Edges(SHORT Xaxis, SHORT Yaxis)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_Color_Battery_Edges_CALL_INSTANCE* cmock_call_instance = (CMOCK_Color_Battery_Edges_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.Color_Battery_Edges_CallInstance);
  Mock.Color_Battery_Edges_CallInstance = CMock_Guts_MemNext(Mock.Color_Battery_Edges_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'Color_Battery_Edges' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_Xaxis), (void*)(&Xaxis), sizeof(SHORT), cmock_line, "Function 'Color_Battery_Edges' called with unexpected value for argument 'Xaxis'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_Yaxis), (void*)(&Yaxis), sizeof(SHORT), cmock_line, "Function 'Color_Battery_Edges' called with unexpected value for argument 'Yaxis'.");
  }
}

void CMockExpectParameters_Color_Battery_Edges(CMOCK_Color_Battery_Edges_CALL_INSTANCE* cmock_call_instance, SHORT Xaxis, SHORT Yaxis)
{
  memcpy(&cmock_call_instance->Expected_Xaxis, &Xaxis, sizeof(SHORT));
  memcpy(&cmock_call_instance->Expected_Yaxis, &Yaxis, sizeof(SHORT));
}

void Color_Battery_Edges_CMockExpect(UNITY_LINE_TYPE cmock_line, SHORT Xaxis, SHORT Yaxis)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_Color_Battery_Edges_CALL_INSTANCE));
  CMOCK_Color_Battery_Edges_CALL_INSTANCE* cmock_call_instance = (CMOCK_Color_Battery_Edges_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.Color_Battery_Edges_CallInstance = CMock_Guts_MemChain(Mock.Color_Battery_Edges_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_Color_Battery_Edges(cmock_call_instance, Xaxis, Yaxis);
}

void Color_Framed_Corners(SHORT Xaxis, SHORT Yaxis)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_Color_Framed_Corners_CALL_INSTANCE* cmock_call_instance = (CMOCK_Color_Framed_Corners_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.Color_Framed_Corners_CallInstance);
  Mock.Color_Framed_Corners_CallInstance = CMock_Guts_MemNext(Mock.Color_Framed_Corners_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'Color_Framed_Corners' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_Xaxis), (void*)(&Xaxis), sizeof(SHORT), cmock_line, "Function 'Color_Framed_Corners' called with unexpected value for argument 'Xaxis'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_Yaxis), (void*)(&Yaxis), sizeof(SHORT), cmock_line, "Function 'Color_Framed_Corners' called with unexpected value for argument 'Yaxis'.");
  }
}

void CMockExpectParameters_Color_Framed_Corners(CMOCK_Color_Framed_Corners_CALL_INSTANCE* cmock_call_instance, SHORT Xaxis, SHORT Yaxis)
{
  memcpy(&cmock_call_instance->Expected_Xaxis, &Xaxis, sizeof(SHORT));
  memcpy(&cmock_call_instance->Expected_Yaxis, &Yaxis, sizeof(SHORT));
}

void Color_Framed_Corners_CMockExpect(UNITY_LINE_TYPE cmock_line, SHORT Xaxis, SHORT Yaxis)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_Color_Framed_Corners_CALL_INSTANCE));
  CMOCK_Color_Framed_Corners_CALL_INSTANCE* cmock_call_instance = (CMOCK_Color_Framed_Corners_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.Color_Framed_Corners_CallInstance = CMock_Guts_MemChain(Mock.Color_Framed_Corners_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_Color_Framed_Corners(cmock_call_instance, Xaxis, Yaxis);
}

