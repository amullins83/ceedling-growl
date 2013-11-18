/* AUTOGENERATED FILE. DO NOT EDIT. */
#ifndef _MOCKGRID_H
#define _MOCKGRID_H

#include "Grid.h"

void MockGrid_Init(void);
void MockGrid_Destroy(void);
void MockGrid_Verify(void);




#define GridClearCellState_Expect(pGrid, column, row, state) GridClearCellState_CMockExpect(__LINE__, pGrid, column, row, state)
void GridClearCellState_CMockExpect(UNITY_LINE_TYPE cmock_line, GRID* pGrid, SHORT column, SHORT row, WORD state);
#define GridCreate_ExpectAndReturn(ID, left, top, right, bottom, state, numColumns, numRows, cellWidth, cellHeight, pScheme, cmock_retval) GridCreate_CMockExpectAndReturn(__LINE__, ID, left, top, right, bottom, state, numColumns, numRows, cellWidth, cellHeight, pScheme, cmock_retval)
void GridCreate_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, WORD ID, SHORT left, SHORT top, SHORT right, SHORT bottom, WORD state, SHORT numColumns, SHORT numRows, SHORT cellWidth, SHORT cellHeight, GOL_SCHEME* pScheme, GRID* cmock_to_return);
#define GridDraw_ExpectAndReturn(pGrid, cmock_retval) GridDraw_CMockExpectAndReturn(__LINE__, pGrid, cmock_retval)
void GridDraw_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, GRID* pGrid, WORD cmock_to_return);
#define GridFreeItems_Expect(pGrid) GridFreeItems_CMockExpect(__LINE__, pGrid)
void GridFreeItems_CMockExpect(UNITY_LINE_TYPE cmock_line, GRID* pGrid);
#define GridGetCell_ExpectAndReturn(pGrid, column, row, cellType, cmock_retval) GridGetCell_CMockExpectAndReturn(__LINE__, pGrid, column, row, cellType, cmock_retval)
void GridGetCell_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, GRID* pGrid, SHORT column, SHORT row, WORD* cellType, void* cmock_to_return);
#define GridMsgDefault_Expect(translatedMsg, pGrid, pMsg) GridMsgDefault_CMockExpect(__LINE__, translatedMsg, pGrid, pMsg)
void GridMsgDefault_CMockExpect(UNITY_LINE_TYPE cmock_line, WORD translatedMsg, GRID* pGrid, GOL_MSG* pMsg);
#define GridSetCell_ExpectAndReturn(pGrid, column, row, state, data, cmock_retval) GridSetCell_CMockExpectAndReturn(__LINE__, pGrid, column, row, state, data, cmock_retval)
void GridSetCell_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, GRID* pGrid, SHORT column, SHORT row, WORD state, void* data, WORD cmock_to_return);
#define GridSetCellState_Expect(pGrid, column, row, state) GridSetCellState_CMockExpect(__LINE__, pGrid, column, row, state)
void GridSetCellState_CMockExpect(UNITY_LINE_TYPE cmock_line, GRID* pGrid, SHORT column, SHORT row, WORD state);
#define GridSetFocus_Expect(pGrid, column, row) GridSetFocus_CMockExpect(__LINE__, pGrid, column, row)
void GridSetFocus_CMockExpect(UNITY_LINE_TYPE cmock_line, GRID* pGrid, SHORT column, SHORT row);
#define GridTranslateMsg_ExpectAndReturn(pCb, pMsg, cmock_retval) GridTranslateMsg_CMockExpectAndReturn(__LINE__, pCb, pMsg, cmock_retval)
void GridTranslateMsg_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, GRID* pCb, GOL_MSG* pMsg, WORD cmock_to_return);

#endif