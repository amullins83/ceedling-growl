/* AUTOGENERATED FILE. DO NOT EDIT. */
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "unity.h"
#include "cmock.h"
#include "MockinstrumentStruct.h"

static struct MockinstrumentStructInstance
{
  unsigned char placeHolder;
} Mock;

extern jmp_buf AbortFrame;

void MockinstrumentStruct_Verify(void)
{
}

void MockinstrumentStruct_Init(void)
{
  MockinstrumentStruct_Destroy();
}

void MockinstrumentStruct_Destroy(void)
{
  CMock_Guts_MemFreeAll();
  memset(&Mock, 0, sizeof(Mock));
}

