#include "Graphics.h"

extern const char L26162[];
//BITMAP NAME CAN BE CHANGED HERE.
volatile const BITMAP_FLASH compass_framed_icon = {0,L26162};
const char L26162[] = {
0x00,0x08,0x22,0x00,0x22,0x00,0x28,0x52,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x08,0x40,0x08,0x02,0x10,0x02,0x08,0x42,0x10,0x42,0x08,0x82,0x10,0x82,0x10,0x04,0x08,0x44,0x10,0x44,0x17,0x44,
0x08,0x84,0x10,0x84,0x17,0x84,0x08,0xBC,0x10,0xBC,0x10,0xFC,0xF7,0x85,0xE8,0xBD,0xF0,0xBD,0xF7,0xBD,0xFF,0xBD,0xF0,0xFD,0xF7,0xFD,0xFF,0xBF,0xF0,0xFF,0xF7,0xFF,0xFF,
0xFF,0xE3,0x18,0xE3,0x18,0xE3,0x18,0xE3,0x18,0xE3,0x18,0xE3,0x18,0xE3,0x18,0xE3,0x18,0xE3,0x00,0xE3,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0xE3,0x00,0xE3,0x08,0xA2,0x08,0x61,0x08,0x61,0x08,0x61,0x08,0x61,0x08,0x61,0x08,0xA2,0x10,0xE3,0x00,0xA3,0x10,0xE3,0x18,0xA2,0x08,0x61,0x08,0x61,0x08,0x61,
0x08,0xA2,0x10,0xE3,0x10,0xE2,0x00,0x63,0x10,0xE3,0x18,0x61,0x08,0x20,0x00,0x20,0x00,0x20,0x08,0x61,0x10,0xE3,0x18,0xE1,0x00,0x23,0x18,0xE3,0x18,0x61,0x00,0x20,0x00,
0x00,0x00,0x20,0x00,0x61,0x10,0xE3,0x18,0xE0,0x00,0xA1,0x18,0xE3,0x18,0x62,0x00,0x00,0x00,0xA2,0x08,0x00,0x00,0xA1,0x10,0xE3,0x18,0x62,0x00,0xA1,0x18,0xE3,0x18,0x22,
0x00,0xA2,0x10,0x61,0x08,0xA2,0x00,0xA0,0x18,0xE3,0x18,0x62,0x00,0xA1,0x18,0xE3,0x18,0x20,0x10,0x21,0x00,0x41,0x00,0x60,0x10,0x20,0x10,0xE3,0x18,0x62,0x00,0xA1,0x18,
0xE3,0x10,0xA0,0x08,0x40,0x00,0x20,0x00,0x21,0x00,0x22,0x00,0xE3,0x18,0x62,0x00,0xA1,0x18,0xA3,0x00,0x62,0x00,0x60,0x00,0x60,0x10,0x43,0x00,0xA1,0x00,0xE2,0x18,0x62,
0x00,0xA1,0x18,0x63,0x00,0x01,0x08,0x20,0x00,0x81,0x10,0xC3,0x10,0x60,0x10,0xE1,0x18,0x62,0x00,0xA1,0x18,0x22,0x08,0x20,0x00,0x21,0x10,0xA1,0x10,0xC3,0x18,0x00,0x08,
0xA0,0x18,0x62,0x00,0xA1,0x18,0x21,0x08,0x00,0x08,0x62,0x10,0x81,0x10,0xC2,0x18,0x01,0x00,0x60,0x18,0x62,0x00,0xA1,0x18,0x61,0x08,0x20,0x08,0xA2,0x10,0x21,0x00,0xE2,
0x18,0x23,0x00,0x61,0x18,0x62,0x00,0xA1,0x18,0xA1,0x08,0x81,0x10,0x82,0x00,0x01,0x00,0xA1,0x18,0x63,0x00,0x62,0x10,0x62,0x00,0xA1,0x18,0x61,0x00,0xA2,0x10,0x81,0x10,
0x20,0x08,0x40,0x10,0xA2,0x00,0x61,0x10,0x62,0x00,0xA1,0x18,0x61,0x00,0xC2,0x10,0x60,0x08,0x20,0x10,0x00,0x10,0x82,0x00,0x61,0x10,0x62,0x00,0xA1,0x18,0x61,0x00,0xC3,
0x18,0x20,0x08,0x20,0x08,0x00,0x08,0x82,0x08,0x61,0x10,0x62,0x00,0xA1,0x18,0x61,0x00,0xE3,0x18,0x82,0x10,0x82,0x08,0x40,0x08,0x82,0x00,0x61,0x10,0x62,0x00,0xA1,0x18,
0x61,0x00,0xE3,0x18,0x82,0x10,0x81,0x00,0x60,0x08,0x62,0x00,0x61,0x18,0x62,0x00,0xA1,0x18,0xA1,0x00,0xE1,0x18,0x82,0x00,0x81,0x08,0x61,0x08,0x62,0x00,0x62,0x18,0x62,
0x00,0xA1,0x18,0xA2,0x00,0xC0,0x18,0xC3,0x10,0x81,0x08,0xA1,0x08,0x22,0x00,0xA2,0x18,0x62,0x00,0xA1,0x18,0xA2,0x08,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,
0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x16,0x05,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x05,0x00,0x00,0x05,0x16,0x1D,0x1D,0x1D,0x1D,0x0E,0x16,0x16,0x16,0x16,0x16,
0x16,0x16,0x16,0x16,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x16,0x16,0x16,0x0E,0x0E,0x0E,0x16,0x16,0x16,0x0E,0x1D,0x1D,0x1D,0x16,0x16,0x16,0x1D,0x1D,0x1D,
0x1D,0x1D,0x1D,0x1D,0x16,0x16,0x16,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x16,0x16,0x16,0x1D,0x1D,0x0E,0x0E,0x0E,0x16,0x16,0x16,0x16,0x16,0x1D,0x1D,0x0E,0x16,0x1D,0x1D,0x1D,
0x1D,0x1D,0x1D,0x16,0x16,0x1D,0x16,0x05,0x05,0x00,0x00,0x00,0x00,0x05,0x05,0x16,0x1D,0x16,0x16,0x0E,0x05,0x16,0x0E,0x16,0x16,0x16,0x0E,0x1D,0x1D,0x05,0x1D,0x1D,0x1D,
0x1D,0x1D,0x1D,0x16,0x16,0x16,0x05,0x00,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x00,0x05,0x16,0x16,0x05,0x16,0x1D,0x0E,0x0E,0x0E,0x1D,0x05,0x1D,0x0E,0x16,0x1D,0x1D,
0x1D,0x1D,0x1D,0x16,0x1D,0x0E,0x00,0x05,0x05,0x03,0x05,0x06,0x0E,0x0E,0x06,0x05,0x03,0x05,0x05,0x00,0x0E,0x16,0x0E,0x0E,0x05,0x16,0x0E,0x1D,0x16,0x0E,0x0E,0x16,0x1D,
0x1D,0x1D,0x1D,0x16,0x16,0x0E,0x00,0x05,0x01,0x06,0x16,0x17,0x1A,0x1D,0x1A,0x1A,0x13,0x13,0x05,0x01,0x05,0x00,0x0E,0x16,0x05,0x16,0x16,0x1D,0x1D,0x16,0x0E,0x0E,0x16,
0x1D,0x1D,0x1D,0x16,0x16,0x16,0x05,0x05,0x06,0x0E,0x17,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1A,0x16,0x11,0x05,0x05,0x05,0x05,0x16,0x16,0x16,0x1D,0x1D,0x1D,0x16,0x0E,0x0E,
0x16,0x1D,0x1D,0x1D,0x16,0x16,0x05,0x05,0x07,0x16,0x17,0x1D,0x1D,0x1D,0x1A,0x1D,0x1A,0x16,0x13,0x15,0x15,0x15,0x0E,0x00,0x05,0x05,0x16,0x16,0x1D,0x1D,0x1D,0x16,0x0E,
0x0E,0x16,0x1D,0x1D,0x16,0x16,0x0E,0x05,0x00,0x11,0x1C,0x1D,0x1D,0x1D,0x16,0x1A,0x13,0x0E,0x0E,0x0F,0x06,0x0E,0x10,0x15,0x0D,0x00,0x05,0x0E,0x16,0x16,0x1D,0x1D,0x16,
0x0E,0x0E,0x16,0x1D,0x1D,0x16,0x16,0x05,0x05,0x05,0x19,0x19,0x1D,0x1C,0x19,0x11,0x0F,0x0C,0x11,0x0E,0x0C,0x0E,0x16,0x0F,0x0D,0x10,0x05,0x05,0x05,0x16,0x16,0x1D,0x1D,
0x16,0x0E,0x0E,0x16,0x1D,0x1D,0x0E,0x16,0x05,0x01,0x0E,0x1C,0x1C,0x19,0x16,0x11,0x07,0x07,0x0E,0x12,0x0B,0x0B,0x0B,0x0E,0x0B,0x0D,0x10,0x0D,0x00,0x05,0x16,0x0E,0x1D,
0x1D,0x16,0x0E,0x0E,0x16,0x1D,0x1D,0x0E,0x0E,0x05,0x01,0x18,0x1C,0x1C,0x19,0x11,0x10,0x11,0x0B,0x0E,0x12,0x06,0x06,0x06,0x0C,0x0B,0x11,0x15,0x0D,0x00,0x05,0x0E,0x0E,
0x1D,0x1D,0x16,0x0E,0x0E,0x16,0x1D,0x16,0x0E,0x0E,0x05,0x05,0x19,0x1C,0x19,0x15,0x10,0x10,0x11,0x13,0x11,0x11,0x0F,0x04,0x04,0x0B,0x0B,0x14,0x15,0x10,0x00,0x05,0x0E,
0x0E,0x16,0x1D,0x16,0x0E,0x0E,0x16,0x1D,0x16,0x0E,0x0E,0x05,0x0E,0x19,0x19,0x19,0x11,0x07,0x05,0x0B,0x13,0x05,0x05,0x0E,0x04,0x04,0x03,0x0E,0x10,0x14,0x10,0x0E,0x05,
0x0E,0x0E,0x16,0x1D,0x16,0x0E,0x0E,0x16,0x1D,0x16,0x0E,0x0E,0x05,0x05,0x11,0x19,0x15,0x07,0x07,0x0D,0x0B,0x0F,0x05,0x05,0x13,0x04,0x03,0x03,0x15,0x15,0x15,0x11,0x00,
0x05,0x0E,0x0E,0x16,0x1D,0x16,0x0E,0x0E,0x16,0x1D,0x16,0x0E,0x0E,0x05,0x03,0x11,0x15,0x11,0x0D,0x0D,0x10,0x10,0x0E,0x05,0x05,0x0E,0x03,0x03,0x0B,0x16,0x18,0x15,0x15,
0x01,0x05,0x0E,0x0E,0x16,0x1D,0x16,0x0E,0x0E,0x16,0x1D,0x16,0x0E,0x16,0x0E,0x03,0x08,0x11,0x10,0x10,0x10,0x10,0x07,0x05,0x0E,0x03,0x03,0x05,0x0B,0x15,0x1B,0x1C,0x1C,
0x0E,0x03,0x0E,0x16,0x0E,0x16,0x1D,0x16,0x0E,0x0E,0x16,0x1D,0x1D,0x0E,0x0E,0x0E,0x03,0x06,0x07,0x0D,0x11,0x16,0x16,0x10,0x05,0x0E,0x06,0x06,0x0B,0x16,0x1D,0x1C,0x1C,
0x1B,0x06,0x03,0x0E,0x0E,0x0E,0x1D,0x1D,0x16,0x0E,0x0E,0x16,0x1D,0x1D,0x0E,0x05,0x0E,0x05,0x04,0x02,0x0D,0x15,0x16,0x0E,0x10,0x0D,0x0E,0x11,0x15,0x18,0x15,0x19,0x1D,
0x1C,0x0E,0x03,0x05,0x0E,0x05,0x0E,0x1D,0x1D,0x16,0x0E,0x0E,0x16,0x1D,0x1D,0x16,0x05,0x0E,0x0E,0x03,0x04,0x07,0x16,0x0E,0x07,0x11,0x0E,0x0D,0x15,0x15,0x19,0x19,0x18,
0x1D,0x15,0x03,0x03,0x0E,0x0E,0x05,0x16,0x1D,0x1D,0x16,0x0E,0x0E,0x16,0x1D,0x1D,0x1D,0x0E,0x05,0x16,0x0E,0x03,0x0E,0x07,0x07,0x07,0x07,0x0E,0x0D,0x11,0x15,0x19,0x1C,
0x1B,0x15,0x0E,0x03,0x0E,0x16,0x05,0x0E,0x1D,0x1D,0x1D,0x16,0x0E,0x0E,0x16,0x1D,0x1D,0x1D,0x16,0x05,0x05,0x16,0x0E,0x01,0x04,0x05,0x0E,0x07,0x07,0x07,0x0E,0x11,0x15,
0x19,0x0A,0x03,0x01,0x0E,0x16,0x05,0x05,0x16,0x1D,0x1D,0x1D,0x16,0x0E,0x0E,0x16,0x1D,0x1D,0x1D,0x1D,0x16,0x05,0x05,0x16,0x0E,0x05,0x04,0x0B,0x05,0x05,0x05,0x07,0x05,
0x05,0x0B,0x04,0x05,0x0E,0x16,0x05,0x05,0x16,0x1D,0x1D,0x1D,0x1D,0x16,0x0E,0x0E,0x16,0x1D,0x1D,0x1D,0x1D,0x1D,0x16,0x05,0x05,0x16,0x16,0x0E,0x05,0x03,0x04,0x09,0x0B,
0x04,0x03,0x05,0x0E,0x16,0x16,0x05,0x05,0x16,0x1D,0x1D,0x1D,0x1D,0x1D,0x16,0x0E,0x0E,0x16,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x16,0x05,0x00,0x05,0x16,0x16,0x16,0x0E,0x0E,
0x0E,0x0E,0x16,0x16,0x16,0x05,0x00,0x05,0x16,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x16,0x0E,0x0E,0x16,0x1D,0x1D,0x1D,0x1D,0x1D,0x16,0x16,0x0E,0x05,0x00,0x00,0x00,0x05,0x0E,
0x16,0x16,0x0E,0x05,0x00,0x00,0x00,0x05,0x0E,0x16,0x16,0x1D,0x1D,0x1D,0x1D,0x1D,0x16,0x0E,0x1D,0x05,0x1D,0x1D,0x1D,0x1D,0x1D,0x16,0x0E,0x0E,0x05,0x05,0x05,0x05,0x00,
0x00,0x00,0x00,0x00,0x00,0x05,0x05,0x05,0x05,0x0E,0x0E,0x16,0x1D,0x1D,0x1D,0x1D,0x1D,0x05,0x1D,0x1D,0x0E,0x16,0x1D,0x1D,0x1D,0x1D,0x16,0x0E,0x0E,0x0E,0x0E,0x05,0x05,
0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x0E,0x0E,0x0E,0x0E,0x16,0x1D,0x1D,0x1D,0x1D,0x16,0x0E,0x1D,0x1D,0x16,0x16,0x16,0x1D,0x1D,0x1D,0x16,0x16,0x16,0x0E,0x0E,0x0E,
0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x16,0x16,0x16,0x1D,0x1D,0x1D,0x16,0x16,0x16,0x1D,0x1D,0x1D,0x0E,0x16,0x16,0x16,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,
0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x16,0x16,0x16,0x0E,0x1D,0x1D,0x1D,0x1D,0x16,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x16,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,
0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,};
