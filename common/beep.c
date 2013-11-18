#include "beep.h"
#include "instrument.h"


extern unsigned char Audio_Mute;


void Mute()
{
    Audio_IO_OutLAT = Audio_IO_OutLAT & MUTE_AUDIO;
    WriteAudio();
    Audio_Mute = TRUE;
}

void Unmute()
{
    Audio_IO_OutLAT = Audio_IO_OutLAT | UNMUTE_AUDIO;
    WriteAudio();
    Audio_Mute = FALSE;
}

void WriteAudio()
{
    i2c2Write(i2cADDR_MAIN_IO_AUDIO, 0x0A, 1, Audio_IO_OutLAT, 1);                                  // write to the OLAT Register...... I/O expander
}


void BeepTwice(int millis) {
    BeepNTimes(2, millis);
}

void BeepNTimes(int times, int millis)
{
    int i;
    for(i = 0; i < times; i++)
        BeepOnce(millis);
}

void BeepOnce(int millis)
{
    int halfCycle = millis/2;

    Audio_IO_OutLAT = Audio_IO_OutLAT | UNMUTE_AUDIO | ENABLE_ALARM;
    WriteAudio();
    DelayMs(halfCycle);

    Audio_IO_OutLAT = Audio_IO_OutLAT & MUTE_AUDIO & DISABLE_ALARM;
    WriteAudio();
    DelayMs(halfCycle);
}

