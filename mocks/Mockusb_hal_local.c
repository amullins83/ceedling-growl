/* AUTOGENERATED FILE. DO NOT EDIT. */
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "unity.h"
#include "cmock.h"
#include "Mockusb_hal_local.h"

static struct Mockusb_hal_localInstance
{
  unsigned char placeHolder;
} Mock;

extern jmp_buf AbortFrame;

void Mockusb_hal_local_Verify(void)
{
}

void Mockusb_hal_local_Init(void)
{
  Mockusb_hal_local_Destroy();
}

void Mockusb_hal_local_Destroy(void)
{
  CMock_Guts_MemFreeAll();
  memset(&Mock, 0, sizeof(Mock));
}

