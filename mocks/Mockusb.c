/* AUTOGENERATED FILE. DO NOT EDIT. */
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "unity.h"
#include "cmock.h"
#include "Mockusb.h"

static struct MockusbInstance
{
  unsigned char placeHolder;
} Mock;

extern jmp_buf AbortFrame;

void Mockusb_Verify(void)
{
}

void Mockusb_Init(void)
{
  Mockusb_Destroy();
}

void Mockusb_Destroy(void)
{
  CMock_Guts_MemFreeAll();
  memset(&Mock, 0, sizeof(Mock));
}

