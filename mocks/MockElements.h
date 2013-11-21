/* AUTOGENERATED FILE. DO NOT EDIT. */
#ifndef _MOCKELEMENTS_H
#define _MOCKELEMENTS_H

#include "Elements.h"

void MockElements_Init(void);
void MockElements_Destroy(void);
void MockElements_Verify(void);




#define Clear_Header_Window_Expect() Clear_Header_Window_CMockExpect(__LINE__)
void Clear_Header_Window_CMockExpect(UNITY_LINE_TYPE cmock_line);
#define Clear_Divider_Window_Expect() Clear_Divider_Window_CMockExpect(__LINE__)
void Clear_Divider_Window_CMockExpect(UNITY_LINE_TYPE cmock_line);
#define New_Divider_Window_Expect() New_Divider_Window_CMockExpect(__LINE__)
void New_Divider_Window_CMockExpect(UNITY_LINE_TYPE cmock_line);
#define Update_Divider_Expect(update_type) Update_Divider_CMockExpect(__LINE__, update_type)
void Update_Divider_CMockExpect(UNITY_LINE_TYPE cmock_line, UINT8 update_type);
#define Clear_Element_Window_Expect() Clear_Element_Window_CMockExpect(__LINE__)
void Clear_Element_Window_CMockExpect(UNITY_LINE_TYPE cmock_line);
#define New_Element_Window_Expect() New_Element_Window_CMockExpect(__LINE__)
void New_Element_Window_CMockExpect(UNITY_LINE_TYPE cmock_line);
#define Update_Element_Expect(update_type) Update_Element_CMockExpect(__LINE__, update_type)
void Update_Element_CMockExpect(UNITY_LINE_TYPE cmock_line, UINT8 update_type);
#define Create_New_Arc_Expect() Create_New_Arc_CMockExpect(__LINE__)
void Create_New_Arc_CMockExpect(UNITY_LINE_TYPE cmock_line);
#define Draw_Arc_Tick_Marks_Expect(cmock_arg1) Draw_Arc_Tick_Marks_CMockExpect(__LINE__, cmock_arg1)
void Draw_Arc_Tick_Marks_CMockExpect(UNITY_LINE_TYPE cmock_line, unsigned char cmock_arg1);
#define Setup_Arc_Units_Expect() Setup_Arc_Units_CMockExpect(__LINE__)
void Setup_Arc_Units_CMockExpect(UNITY_LINE_TYPE cmock_line);
#define Draw_Arc_Units_Expect(update_type) Draw_Arc_Units_CMockExpect(__LINE__, update_type)
void Draw_Arc_Units_CMockExpect(UNITY_LINE_TYPE cmock_line, UINT8 update_type);
#define Update_Arc_ExpectAndReturn(cmock_retval) Update_Arc_CMockExpectAndReturn(__LINE__, cmock_retval)
void Update_Arc_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, unsigned short int cmock_to_return);
#define Update_Range_Expect() Update_Range_CMockExpect(__LINE__)
void Update_Range_CMockExpect(UNITY_LINE_TYPE cmock_line);
#define GetRateResult_ExpectAndReturn(cmock_retval) GetRateResult_CMockExpectAndReturn(__LINE__, cmock_retval)
void GetRateResult_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, float cmock_to_return);

#endif