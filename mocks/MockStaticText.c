/* AUTOGENERATED FILE. DO NOT EDIT. */
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "unity.h"
#include "cmock.h"
#include "MockStaticText.h"

typedef struct _CMOCK_StSetText_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  STATICTEXT* Expected_pSt;
  XCHAR* Expected_pText;

} CMOCK_StSetText_CALL_INSTANCE;

typedef struct _CMOCK_StCreate_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  STATICTEXT* ReturnVal;
  WORD Expected_ID;
  SHORT Expected_left;
  SHORT Expected_top;
  SHORT Expected_right;
  SHORT Expected_bottom;
  WORD Expected_state;
  XCHAR* Expected_pText;
  GOL_SCHEME* Expected_pScheme;

} CMOCK_StCreate_CALL_INSTANCE;

typedef struct _CMOCK_StTranslateMsg_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  WORD ReturnVal;
  STATICTEXT* Expected_pSt;
  GOL_MSG* Expected_pMsg;

} CMOCK_StTranslateMsg_CALL_INSTANCE;

typedef struct _CMOCK_StDraw_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  WORD ReturnVal;
  STATICTEXT* Expected_pSt;

} CMOCK_StDraw_CALL_INSTANCE;

static struct MockStaticTextInstance
{
  CMOCK_MEM_INDEX_TYPE StSetText_CallInstance;
  CMOCK_MEM_INDEX_TYPE StCreate_CallInstance;
  CMOCK_MEM_INDEX_TYPE StTranslateMsg_CallInstance;
  CMOCK_MEM_INDEX_TYPE StDraw_CallInstance;
} Mock;

extern jmp_buf AbortFrame;

void MockStaticText_Verify(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.StSetText_CallInstance, cmock_line, "Function 'StSetText' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.StCreate_CallInstance, cmock_line, "Function 'StCreate' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.StTranslateMsg_CallInstance, cmock_line, "Function 'StTranslateMsg' called less times than expected.");
  UNITY_TEST_ASSERT(CMOCK_GUTS_NONE == Mock.StDraw_CallInstance, cmock_line, "Function 'StDraw' called less times than expected.");
}

void MockStaticText_Init(void)
{
  MockStaticText_Destroy();
}

void MockStaticText_Destroy(void)
{
  CMock_Guts_MemFreeAll();
  memset(&Mock, 0, sizeof(Mock));
}

void StSetText(STATICTEXT* pSt, XCHAR* pText)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_StSetText_CALL_INSTANCE* cmock_call_instance = (CMOCK_StSetText_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.StSetText_CallInstance);
  Mock.StSetText_CallInstance = CMock_Guts_MemNext(Mock.StSetText_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'StSetText' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(cmock_call_instance->Expected_pSt), (void*)(pSt), sizeof(STATICTEXT), cmock_line, "Function 'StSetText' called with unexpected value for argument 'pSt'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(cmock_call_instance->Expected_pText), (void*)(pText), sizeof(XCHAR), cmock_line, "Function 'StSetText' called with unexpected value for argument 'pText'.");
  }
}

void CMockExpectParameters_StSetText(CMOCK_StSetText_CALL_INSTANCE* cmock_call_instance, STATICTEXT* pSt, XCHAR* pText)
{
  cmock_call_instance->Expected_pSt = pSt;
  cmock_call_instance->Expected_pText = pText;
}

void StSetText_CMockExpect(UNITY_LINE_TYPE cmock_line, STATICTEXT* pSt, XCHAR* pText)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_StSetText_CALL_INSTANCE));
  CMOCK_StSetText_CALL_INSTANCE* cmock_call_instance = (CMOCK_StSetText_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.StSetText_CallInstance = CMock_Guts_MemChain(Mock.StSetText_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_StSetText(cmock_call_instance, pSt, pText);
}

STATICTEXT* StCreate(WORD ID, SHORT left, SHORT top, SHORT right, SHORT bottom, WORD state, XCHAR* pText, GOL_SCHEME* pScheme)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_StCreate_CALL_INSTANCE* cmock_call_instance = (CMOCK_StCreate_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.StCreate_CallInstance);
  Mock.StCreate_CallInstance = CMock_Guts_MemNext(Mock.StCreate_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'StCreate' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_ID), (void*)(&ID), sizeof(WORD), cmock_line, "Function 'StCreate' called with unexpected value for argument 'ID'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_left), (void*)(&left), sizeof(SHORT), cmock_line, "Function 'StCreate' called with unexpected value for argument 'left'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_top), (void*)(&top), sizeof(SHORT), cmock_line, "Function 'StCreate' called with unexpected value for argument 'top'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_right), (void*)(&right), sizeof(SHORT), cmock_line, "Function 'StCreate' called with unexpected value for argument 'right'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_bottom), (void*)(&bottom), sizeof(SHORT), cmock_line, "Function 'StCreate' called with unexpected value for argument 'bottom'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(&cmock_call_instance->Expected_state), (void*)(&state), sizeof(WORD), cmock_line, "Function 'StCreate' called with unexpected value for argument 'state'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(cmock_call_instance->Expected_pText), (void*)(pText), sizeof(XCHAR), cmock_line, "Function 'StCreate' called with unexpected value for argument 'pText'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(cmock_call_instance->Expected_pScheme), (void*)(pScheme), sizeof(GOL_SCHEME), cmock_line, "Function 'StCreate' called with unexpected value for argument 'pScheme'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_StCreate(CMOCK_StCreate_CALL_INSTANCE* cmock_call_instance, WORD ID, SHORT left, SHORT top, SHORT right, SHORT bottom, WORD state, XCHAR* pText, GOL_SCHEME* pScheme)
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

void StCreate_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, WORD ID, SHORT left, SHORT top, SHORT right, SHORT bottom, WORD state, XCHAR* pText, GOL_SCHEME* pScheme, STATICTEXT* cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_StCreate_CALL_INSTANCE));
  CMOCK_StCreate_CALL_INSTANCE* cmock_call_instance = (CMOCK_StCreate_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.StCreate_CallInstance = CMock_Guts_MemChain(Mock.StCreate_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_StCreate(cmock_call_instance, ID, left, top, right, bottom, state, pText, pScheme);
  cmock_call_instance->ReturnVal = cmock_to_return;
}

WORD StTranslateMsg(STATICTEXT* pSt, GOL_MSG* pMsg)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_StTranslateMsg_CALL_INSTANCE* cmock_call_instance = (CMOCK_StTranslateMsg_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.StTranslateMsg_CallInstance);
  Mock.StTranslateMsg_CallInstance = CMock_Guts_MemNext(Mock.StTranslateMsg_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'StTranslateMsg' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(cmock_call_instance->Expected_pSt), (void*)(pSt), sizeof(STATICTEXT), cmock_line, "Function 'StTranslateMsg' called with unexpected value for argument 'pSt'.");
  }
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(cmock_call_instance->Expected_pMsg), (void*)(pMsg), sizeof(GOL_MSG), cmock_line, "Function 'StTranslateMsg' called with unexpected value for argument 'pMsg'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_StTranslateMsg(CMOCK_StTranslateMsg_CALL_INSTANCE* cmock_call_instance, STATICTEXT* pSt, GOL_MSG* pMsg)
{
  cmock_call_instance->Expected_pSt = pSt;
  cmock_call_instance->Expected_pMsg = pMsg;
}

void StTranslateMsg_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, STATICTEXT* pSt, GOL_MSG* pMsg, WORD cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_StTranslateMsg_CALL_INSTANCE));
  CMOCK_StTranslateMsg_CALL_INSTANCE* cmock_call_instance = (CMOCK_StTranslateMsg_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.StTranslateMsg_CallInstance = CMock_Guts_MemChain(Mock.StTranslateMsg_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_StTranslateMsg(cmock_call_instance, pSt, pMsg);
  memcpy(&cmock_call_instance->ReturnVal, &cmock_to_return, sizeof(WORD));
}

WORD StDraw(STATICTEXT* pSt)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_StDraw_CALL_INSTANCE* cmock_call_instance = (CMOCK_StDraw_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.StDraw_CallInstance);
  Mock.StDraw_CallInstance = CMock_Guts_MemNext(Mock.StDraw_CallInstance);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "Function 'StDraw' called more times than expected.");
  cmock_line = cmock_call_instance->LineNumber;
  {
    UNITY_TEST_ASSERT_EQUAL_MEMORY((void*)(cmock_call_instance->Expected_pSt), (void*)(pSt), sizeof(STATICTEXT), cmock_line, "Function 'StDraw' called with unexpected value for argument 'pSt'.");
  }
  return cmock_call_instance->ReturnVal;
}

void CMockExpectParameters_StDraw(CMOCK_StDraw_CALL_INSTANCE* cmock_call_instance, STATICTEXT* pSt)
{
  cmock_call_instance->Expected_pSt = pSt;
}

void StDraw_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, STATICTEXT* pSt, WORD cmock_to_return)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_StDraw_CALL_INSTANCE));
  CMOCK_StDraw_CALL_INSTANCE* cmock_call_instance = (CMOCK_StDraw_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, "CMock has run out of memory. Please allocate more.");
  Mock.StDraw_CallInstance = CMock_Guts_MemChain(Mock.StDraw_CallInstance, cmock_guts_index);
  cmock_call_instance->LineNumber = cmock_line;
  CMockExpectParameters_StDraw(cmock_call_instance, pSt);
  memcpy(&cmock_call_instance->ReturnVal, &cmock_to_return, sizeof(WORD));
}

