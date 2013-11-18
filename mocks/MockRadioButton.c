/* AUTOGENERATED FILE. DO NOT EDIT. */
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "unity.h"
#include "cmock.h"
#include "MockRadioButton.h"

typedef struct _CMOCK_RbSetText_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  RADIOBUTTON* Expected_pRb;
  XCHAR* Expected_pText;

} CMOCK_RbSetText_CALL_INSTANCE;

typedef struct _CMOCK_RbSetCheck_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  RADIOBUTTON* Expected_pRb;
  WORD Expected_ID;

} CMOCK_RbSetCheck_CALL_INSTANCE;

typedef struct _CMOCK_RbGetCheck_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  WORD ReturnVal;
  RADIOBUTTON* Expected_pRb;

} CMOCK_RbGetCheck_CALL_INSTANCE;

typedef struct _CMOCK_RbCreate_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  RADIOBUTTON* ReturnVal;
  WORD Expected_ID;
  SHORT Expected_left;
  SHORT Expected_top;
  SHORT Expected_right;
  SHORT Expected_bottom;
  WORD Expected_state;
  XCHAR* Expected_pText;
  GOL_SCHEME* Expected_pScheme;

} CMOCK_RbCreate_CALL_INSTANCE;

typedef struct _CMOCK_RbTranslateMsg_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  WORD ReturnVal;
  RADIOBUTTON* Expected_pRb;
  GOL_MSG* Expected_pMsg;

} CMOCK_RbTranslateMsg_CALL_INSTANCE;

typedef struct _CMOCK_RbMsgDefault_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  WORD Expected_translatedMsg;
  RADIOBUTTON* Expected_pRb;
  GOL_MSG* Expected_pMsg;

} CMOCK_RbMsgDefault_CALL_INSTANCE;

typedef struct _CMOCK_RbDraw_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  WORD ReturnVal;
  RADIOBUTTON* Expected_pRb;

} CMOCK_RbDraw_CALL_INSTANCE;

static struct MockRadioButtonInstance
{
  CMOCK_MEM_INDEX_TYPE RbSetText_CallInstance;
  CMOCK_MEM_INDEX_TYPE RbSetCheck_CallInstance;
  CMOCK_MEM_INDEX_TYPE RbGetCheck_CallInstance;
  CMOCK_MEM_INDEX_TYPE RbCreate_CallInstance;
  CMOCK_MEM_INDEX_TYPE RbTranslateMsg_CallInstance;
  CMOCK_MEM_INDEX_TYPE RbMsgDefault_CallInstance;
  CMOCK_MEM_INDEX_TYPE RbDraw_CallInstance;
} Mock;

extern jmp_buf AbortFrame;

void MockRadioButton_Verify(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.RbSetText_CallInstance, cmock_line, "Function 'RbSetText' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.RbSetCheck_CallInstance, cmock_line, "Function 'RbSetCheck' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.RbGetCheck_CallInstance, cmock_line, "Function 'RbGetCheck' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.RbCreate_CallInstance, cmock_line, "Function 'RbCreate' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.RbTranslateMsg_CallInstance, cmock_line, "Function 'RbTranslateMsg' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.RbMsgDefault_CallInstance, cmock_line, "Function 'RbMsgDefault' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.RbDraw_CallInstance, cmock_line, "Function 'RbDraw' called less times than expected.");
}

void MockRadioButton_Init(void)
{
  MockRadioButton_Destroy();
}

void MockRadioButton_Destroy(void)
{
  CMock_Guts_MemFreeAll();
  memset(&Mock, 0, sizeof(Mock));
}

void RbSetText(RADIOBUTTON* pRb, XCHAR* pText)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_RbSetText_CALL_INSTANCE* cmock_call_instance = (CMOCK_RbSetText_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.RbSetText_CallInstance);
  Mock.RbSetText_CallInstance = CMock_Guts_MemNext(Mock.RbSetText_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'RbSetText' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(cmock_call_instance->Expected_pRb), (void*)(pRb), sizeof(RADIOBUTTON), cmock_line, "Function 'RbSetText' called with unexpected value for argument 'pRb'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(cmock_call_instance->Expected_pText), (void*)(pText), sizeof(XCHAR), cmock_line, "Function 'RbSetText' called with unexpected value for argument 'pText'.");
  }
}

void CMockExpectParameters_RbSetText(CMOCK_RbSetText_CALL_INSTANCE* cmock_call_instance, RADIOBUTTON* pRb, XCHAR* pText)
{
  cmock_call_instance->Expected_pRb = pRb;
  cmock_call_instance->Expected_pText = pText;
}

void RbSetText_CMockExpect(UNITY_LINE_TYPE cmock_line, RADIOBUTTON* pRb, XCHAR* pText)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_RbSetText_CALL_INSTANCE));
  CMOCK_RbSetText_CALL_INSTANCE* cmock_call_instance = (CMOCK_RbSetText_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.RbSetText_CallInstance = CMock_Guts_MemChain(Mock.RbSetText_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_RbSetText(cmock_call_instance, pRb, pText);
}

void RbSetCheck(RADIOBUTTON* pRb, WORD ID)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_RbSetCheck_CALL_INSTANCE* cmock_call_instance = (CMOCK_RbSetCheck_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.RbSetCheck_CallInstance);
  Mock.RbSetCheck_CallInstance = CMock_Guts_MemNext(Mock.RbSetCheck_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'RbSetCheck' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(cmock_call_instance->Expected_pRb), (void*)(pRb), sizeof(RADIOBUTTON), cmock_line, "Function 'RbSetCheck' called with unexpected value for argument 'pRb'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_ID), (void*)(&ID), sizeof(WORD), cmock_line, "Function 'RbSetCheck' called with unexpected value for argument 'ID'.");
  }
}

void CMockExpectParameters_RbSetCheck(CMOCK_RbSetCheck_CALL_INSTANCE* cmock_call_instance, RADIOBUTTON* pRb, WORD ID)
{
  cmock_call_instance->Expected_pRb = pRb;
  memcpy(&cmock_call_instance->Expected_ID, &ID, sizeof(WORD));
}

void RbSetCheck_CMockExpect(UNITY_LINE_TYPE cmock_line, RADIOBUTTON* pRb, WORD ID)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_RbSetCheck_CALL_INSTANCE));
  CMOCK_RbSetCheck_CALL_INSTANCE* cmock_call_instance = (CMOCK_RbSetCheck_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.RbSetCheck_CallInstance = CMock_Guts_MemChain(Mock.RbSetCheck_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_RbSetCheck(cmock_call_instance, pRb, ID);
}

WORD RbGetCheck(RADIOBUTTON* pRb)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_RbGetCheck_CALL_INSTANCE* cmock_call_instance = (CMOCK_RbGetCheck_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.RbGetCheck_CallInstance);
  Mock.RbGetCheck_CallInstance = CMock_Guts_MemNext(Mock.RbGetCheck_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'RbGetCheck' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(cmock_call_instance->Expected_pRb), (void*)(pRb), sizeof(RADIOBUTTON), cmock_line, "Function 'RbGetCheck' called with unexpected value for argument 'pRb'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_RbGetCheck(CMOCK_RbGetCheck_CALL_INSTANCE* cmock_call_instance, RADIOBUTTON* pRb)
{
  cmock_call_instance->Expected_pRb = pRb;
}

void RbGetCheck_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, RADIOBUTTON* pRb, WORD cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_RbGetCheck_CALL_INSTANCE));
  CMOCK_RbGetCheck_CALL_INSTANCE* cmock_call_instance = (CMOCK_RbGetCheck_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.RbGetCheck_CallInstance = CMock_Guts_MemChain(Mock.RbGetCheck_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_RbGetCheck(cmock_call_instance, pRb);
  memcpy(&cmock_call_instance->ReturnVal, &cmock_to_return, sizeof(WORD));
}

RADIOBUTTON* RbCreate(WORD ID, SHORT left, SHORT top, SHORT right, SHORT bottom, WORD state, XCHAR* pText, GOL_SCHEME* pScheme)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_RbCreate_CALL_INSTANCE* cmock_call_instance = (CMOCK_RbCreate_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.RbCreate_CallInstance);
  Mock.RbCreate_CallInstance = CMock_Guts_MemNext(Mock.RbCreate_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'RbCreate' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_ID), (void*)(&ID), sizeof(WORD), cmock_line, "Function 'RbCreate' called with unexpected value for argument 'ID'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_left), (void*)(&left), sizeof(SHORT), cmock_line, "Function 'RbCreate' called with unexpected value for argument 'left'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_top), (void*)(&top), sizeof(SHORT), cmock_line, "Function 'RbCreate' called with unexpected value for argument 'top'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_right), (void*)(&right), sizeof(SHORT), cmock_line, "Function 'RbCreate' called with unexpected value for argument 'right'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_bottom), (void*)(&bottom), sizeof(SHORT), cmock_line, "Function 'RbCreate' called with unexpected value for argument 'bottom'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_state), (void*)(&state), sizeof(WORD), cmock_line, "Function 'RbCreate' called with unexpected value for argument 'state'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(cmock_call_instance->Expected_pText), (void*)(pText), sizeof(XCHAR), cmock_line, "Function 'RbCreate' called with unexpected value for argument 'pText'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(cmock_call_instance->Expected_pScheme), (void*)(pScheme), sizeof(GOL_SCHEME), cmock_line, "Function 'RbCreate' called with unexpected value for argument 'pScheme'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_RbCreate(CMOCK_RbCreate_CALL_INSTANCE* cmock_call_instance, WORD ID, SHORT left, SHORT top, SHORT right, SHORT bottom, WORD state, XCHAR* pText, GOL_SCHEME* pScheme)
{
  memcpy(&cmock_call_instance->Expected_ID, &ID, sizeof(WORD));
  memcpy(&cmock_call_instance->Expected_left, &left, sizeof(SHORT));
  memcpy(&cmock_call_instance->Expected_top, &top, sizeof(SHORT));
  memcpy(&cmock_call_instance->Expected_right, &right, sizeof(SHORT));
  memcpy(&cmock_call_instance->Expected_bottom, &bottom, sizeof(SHORT));
  memcpy(&cmock_call_instance->Expected_state, &state, sizeof(WORD));
  cmock_call_instance->Expected_pText = pText;
  cmock_call_instance->Expected_pScheme = pScheme;
}

void RbCreate_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, WORD ID, SHORT left, SHORT top, SHORT right, SHORT bottom, WORD state, XCHAR* pText, GOL_SCHEME* pScheme, RADIOBUTTON* cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_RbCreate_CALL_INSTANCE));
  CMOCK_RbCreate_CALL_INSTANCE* cmock_call_instance = (CMOCK_RbCreate_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.RbCreate_CallInstance = CMock_Guts_MemChain(Mock.RbCreate_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_RbCreate(cmock_call_instance, ID, left, top, right, bottom, state, pText, pScheme);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

WORD RbTranslateMsg(RADIOBUTTON* pRb, GOL_MSG* pMsg)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_RbTranslateMsg_CALL_INSTANCE* cmock_call_instance = (CMOCK_RbTranslateMsg_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.RbTranslateMsg_CallInstance);
  Mock.RbTranslateMsg_CallInstance = CMock_Guts_MemNext(Mock.RbTranslateMsg_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'RbTranslateMsg' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(cmock_call_instance->Expected_pRb), (void*)(pRb), sizeof(RADIOBUTTON), cmock_line, "Function 'RbTranslateMsg' called with unexpected value for argument 'pRb'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(cmock_call_instance->Expected_pMsg), (void*)(pMsg), sizeof(GOL_MSG), cmock_line, "Function 'RbTranslateMsg' called with unexpected value for argument 'pMsg'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_RbTranslateMsg(CMOCK_RbTranslateMsg_CALL_INSTANCE* cmock_call_instance, RADIOBUTTON* pRb, GOL_MSG* pMsg)
{
  cmock_call_instance->Expected_pRb = pRb;
  cmock_call_instance->Expected_pMsg = pMsg;
}

void RbTranslateMsg_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, RADIOBUTTON* pRb, GOL_MSG* pMsg, WORD cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_RbTranslateMsg_CALL_INSTANCE));
  CMOCK_RbTranslateMsg_CALL_INSTANCE* cmock_call_instance = (CMOCK_RbTranslateMsg_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.RbTranslateMsg_CallInstance = CMock_Guts_MemChain(Mock.RbTranslateMsg_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_RbTranslateMsg(cmock_call_instance, pRb, pMsg);
  memcpy(&cmock_call_instance->ReturnVal, &cmock_to_return, sizeof(WORD));
}

void RbMsgDefault(WORD translatedMsg, RADIOBUTTON* pRb, GOL_MSG* pMsg)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_RbMsgDefault_CALL_INSTANCE* cmock_call_instance = (CMOCK_RbMsgDefault_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.RbMsgDefault_CallInstance);
  Mock.RbMsgDefault_CallInstance = CMock_Guts_MemNext(Mock.RbMsgDefault_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'RbMsgDefault' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_translatedMsg), (void*)(&translatedMsg), sizeof(WORD), cmock_line, "Function 'RbMsgDefault' called with unexpected value for argument 'translatedMsg'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(cmock_call_instance->Expected_pRb), (void*)(pRb), sizeof(RADIOBUTTON), cmock_line, "Function 'RbMsgDefault' called with unexpected value for argument 'pRb'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(cmock_call_instance->Expected_pMsg), (void*)(pMsg), sizeof(GOL_MSG), cmock_line, "Function 'RbMsgDefault' called with unexpected value for argument 'pMsg'.");
  }
}

void CMockExpectParameters_RbMsgDefault(CMOCK_RbMsgDefault_CALL_INSTANCE* cmock_call_instance, WORD translatedMsg, RADIOBUTTON* pRb, GOL_MSG* pMsg)
{
  memcpy(&cmock_call_instance->Expected_translatedMsg, &translatedMsg, sizeof(WORD));
  cmock_call_instance->Expected_pRb = pRb;
  cmock_call_instance->Expected_pMsg = pMsg;
}

void RbMsgDefault_CMockExpect(UNITY_LINE_TYPE cmock_line, WORD translatedMsg, RADIOBUTTON* pRb, GOL_MSG* pMsg)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_RbMsgDefault_CALL_INSTANCE));
  CMOCK_RbMsgDefault_CALL_INSTANCE* cmock_call_instance = (CMOCK_RbMsgDefault_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.RbMsgDefault_CallInstance = CMock_Guts_MemChain(Mock.RbMsgDefault_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_RbMsgDefault(cmock_call_instance, translatedMsg, pRb, pMsg);
}

WORD RbDraw(RADIOBUTTON* pRb)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_RbDraw_CALL_INSTANCE* cmock_call_instance = (CMOCK_RbDraw_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.RbDraw_CallInstance);
  Mock.RbDraw_CallInstance = CMock_Guts_MemNext(Mock.RbDraw_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'RbDraw' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(cmock_call_instance->Expected_pRb), (void*)(pRb), sizeof(RADIOBUTTON), cmock_line, "Function 'RbDraw' called with unexpected value for argument 'pRb'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_RbDraw(CMOCK_RbDraw_CALL_INSTANCE* cmock_call_instance, RADIOBUTTON* pRb)
{
  cmock_call_instance->Expected_pRb = pRb;
}

void RbDraw_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, RADIOBUTTON* pRb, WORD cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_RbDraw_CALL_INSTANCE));
  CMOCK_RbDraw_CALL_INSTANCE* cmock_call_instance = (CMOCK_RbDraw_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.RbDraw_CallInstance = CMock_Guts_MemChain(Mock.RbDraw_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_RbDraw(cmock_call_instance, pRb);
  memcpy(&cmock_call_instance->ReturnVal, &cmock_to_return, sizeof(WORD));
}

