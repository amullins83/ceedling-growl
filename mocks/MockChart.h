/* AUTOGENERATED FILE. DO NOT EDIT. */
#ifndef _MOCKCHART_H
#define _MOCKCHART_H

#include "Chart.h"

void MockChart_Init(void);
void MockChart_Destroy(void);
void MockChart_Verify(void);




#define ChSetDataSeries_ExpectAndReturn(pCh, seriesNum, status, cmock_retval) ChSetDataSeries_CMockExpectAndReturn(__LINE__, pCh, seriesNum, status, cmock_retval)
void ChSetDataSeries_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, CHART* pCh, WORD seriesNum, BYTE status, SHORT cmock_to_return);
#define ChCreate_ExpectAndReturn(ID, left, top, right, bottom, state, pData, pParam, pScheme, cmock_retval) ChCreate_CMockExpectAndReturn(__LINE__, ID, left, top, right, bottom, state, pData, pParam, pScheme, cmock_retval)
void ChCreate_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, WORD ID, SHORT left, SHORT top, SHORT right, SHORT bottom, WORD state, DATASERIES* pData, CHARTPARAM* pParam, GOL_SCHEME* pScheme, CHART* cmock_to_return);
#define ChAddDataSeries_ExpectAndReturn(pCh, nSamples, pData, pName, cmock_retval) ChAddDataSeries_CMockExpectAndReturn(__LINE__, pCh, nSamples, pData, pName, cmock_retval)
void ChAddDataSeries_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, CHART* pCh, WORD nSamples, WORD* pData, XCHAR* pName, DATASERIES* cmock_to_return);
#define ChRemoveDataSeries_Expect(pCh, number) ChRemoveDataSeries_CMockExpect(__LINE__, pCh, number)
void ChRemoveDataSeries_CMockExpect(UNITY_LINE_TYPE cmock_line, CHART* pCh, WORD number);
#define ChSetValueRange_Expect(pCh, min, max) ChSetValueRange_CMockExpect(__LINE__, pCh, min, max)
void ChSetValueRange_CMockExpect(UNITY_LINE_TYPE cmock_line, CHART* pCh, WORD min, WORD max);
#define ChSetPercentRange_Expect(pCh, min, max) ChSetPercentRange_CMockExpect(__LINE__, pCh, min, max)
void ChSetPercentRange_CMockExpect(UNITY_LINE_TYPE cmock_line, CHART* pCh, WORD min, WORD max);
#define ChSetSampleRange_Expect(pCh, start, end) ChSetSampleRange_CMockExpect(__LINE__, pCh, start, end)
void ChSetSampleRange_CMockExpect(UNITY_LINE_TYPE cmock_line, CHART* pCh, WORD start, WORD end);
#define ChTranslateMsg_ExpectAndReturn(pCh, pMsg, cmock_retval) ChTranslateMsg_CMockExpectAndReturn(__LINE__, pCh, pMsg, cmock_retval)
void ChTranslateMsg_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, CHART* pCh, GOL_MSG* pMsg, WORD cmock_to_return);
#define ChDraw_ExpectAndReturn(pCh, cmock_retval) ChDraw_CMockExpectAndReturn(__LINE__, pCh, cmock_retval)
void ChDraw_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, CHART* pCh, WORD cmock_to_return);

#endif
