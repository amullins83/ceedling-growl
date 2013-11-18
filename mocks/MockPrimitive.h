/* AUTOGENERATED FILE. DO NOT EDIT. */
#ifndef _MOCKPRIMITIVE_H
#define _MOCKPRIMITIVE_H

#include "Primitive.h"

void MockPrimitive_Init(void);
void MockPrimitive_Destroy(void);
void MockPrimitive_Verify(void);




#define Arc_ExpectAndReturn(xL, yT, xR, yB, r1, r2, octant, cmock_retval) Arc_CMockExpectAndReturn(__LINE__, xL, yT, xR, yB, r1, r2, octant, cmock_retval)
void Arc_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, SHORT xL, SHORT yT, SHORT xR, SHORT yB, SHORT r1, SHORT r2, BYTE octant, WORD cmock_to_return);
#define InitGraph_Expect() InitGraph_CMockExpect(__LINE__)
void InitGraph_CMockExpect(UNITY_LINE_TYPE cmock_line);
#define OutChar_ExpectAndReturn(ch, cmock_retval) OutChar_CMockExpectAndReturn(__LINE__, ch, cmock_retval)
void OutChar_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, XCHAR ch, WORD cmock_to_return);
#define OutText_ExpectAndReturn(textString, cmock_retval) OutText_CMockExpectAndReturn(__LINE__, textString, cmock_retval)
void OutText_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, XCHAR* textString, WORD cmock_to_return);
#define OutTextXY_ExpectAndReturn(x, y, textString, cmock_retval) OutTextXY_CMockExpectAndReturn(__LINE__, x, y, textString, cmock_retval)
void OutTextXY_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, SHORT x, SHORT y, XCHAR* textString, WORD cmock_to_return);
#define GetTextHeight_ExpectAndReturn(font, cmock_retval) GetTextHeight_CMockExpectAndReturn(__LINE__, font, cmock_retval)
void GetTextHeight_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, void* font, SHORT cmock_to_return);
#define GetTextWidth_ExpectAndReturn(textString, font, cmock_retval) GetTextWidth_CMockExpectAndReturn(__LINE__, textString, font, cmock_retval)
void GetTextWidth_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, XCHAR* textString, void* font, SHORT cmock_to_return);
#define SetFont_Expect(font) SetFont_CMockExpect(__LINE__, font)
void SetFont_CMockExpect(UNITY_LINE_TYPE cmock_line, void* font);
#define Line_ExpectAndReturn(x1, y1, x2, y2, cmock_retval) Line_CMockExpectAndReturn(__LINE__, x1, y1, x2, y2, cmock_retval)
void Line_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, SHORT x1, SHORT y1, SHORT x2, SHORT y2, WORD cmock_to_return);
#define Circle_ExpectAndReturn(x, y, radius, cmock_retval) Circle_CMockExpectAndReturn(__LINE__, x, y, radius, cmock_retval)
void Circle_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, SHORT x, SHORT y, SHORT radius, WORD cmock_to_return);
#define Bevel_ExpectAndReturn(x1, y1, x2, y2, rad, cmock_retval) Bevel_CMockExpectAndReturn(__LINE__, x1, y1, x2, y2, rad, cmock_retval)
void Bevel_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, SHORT x1, SHORT y1, SHORT x2, SHORT y2, SHORT rad, WORD cmock_to_return);
#define FillBevel_ExpectAndReturn(x1, y1, x2, y2, rad, cmock_retval) FillBevel_CMockExpectAndReturn(__LINE__, x1, y1, x2, y2, rad, cmock_retval)
void FillBevel_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, SHORT x1, SHORT y1, SHORT x2, SHORT y2, SHORT rad, WORD cmock_to_return);
#define DrawPoly_ExpectAndReturn(numPoints, polyPoints, cmock_retval) DrawPoly_CMockExpectAndReturn(__LINE__, numPoints, polyPoints, cmock_retval)
void DrawPoly_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, SHORT numPoints, SHORT* polyPoints, WORD cmock_to_return);
#define Bar_ExpectAndReturn(left, top, right, bottom, cmock_retval) Bar_CMockExpectAndReturn(__LINE__, left, top, right, bottom, cmock_retval)
void Bar_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, SHORT left, SHORT top, SHORT right, SHORT bottom, WORD cmock_to_return);
#define ClearDevice_Expect() ClearDevice_CMockExpect(__LINE__)
void ClearDevice_CMockExpect(UNITY_LINE_TYPE cmock_line);
#define PutImage_ExpectAndReturn(left, top, bitmap, stretch, cmock_retval) PutImage_CMockExpectAndReturn(__LINE__, left, top, bitmap, stretch, cmock_retval)
void PutImage_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, SHORT left, SHORT top, void* bitmap, BYTE stretch, WORD cmock_to_return);
#define GetImageWidth_ExpectAndReturn(bitmap, cmock_retval) GetImageWidth_CMockExpectAndReturn(__LINE__, bitmap, cmock_retval)
void GetImageWidth_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, void* bitmap, SHORT cmock_to_return);
#define GetImageHeight_ExpectAndReturn(bitmap, cmock_retval) GetImageHeight_CMockExpectAndReturn(__LINE__, bitmap, cmock_retval)
void GetImageHeight_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, void* bitmap, SHORT cmock_to_return);
#define ExternalMemoryCallback_ExpectAndReturn(memory, offset, nCount, buffer, cmock_retval) ExternalMemoryCallback_CMockExpectAndReturn(__LINE__, memory, offset, nCount, buffer, cmock_retval)
void ExternalMemoryCallback_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, EXTDATA* memory, LONG offset, WORD nCount, void* buffer, WORD cmock_to_return);

#endif