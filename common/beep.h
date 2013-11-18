/* 
 * File:   beep.h
 * Author: amullins
 *
 * Created on September 19, 2013, 4:43 PM
 */

#ifndef BEEP_H
#define	BEEP_H

#ifdef	__cplusplus
extern "C" {
#endif

void BeepNTimes(int times, int millis);         // n04 Causes the audio to beep _times_ times, at a rate of one beep per _millis_ milliseconds
void BeepOnce(int millis);                      // n04 Causes audio to beep once with a total cycle time of _millis_ milliseconds
void BeepTwice(int millis);                     // n04 Causes audio to beep twice at a rate of one beep per _millis_ milliseconds
void WriteAudio();                              // n04 Helper function for audio writes.
void Mute();                                    // n04 Helper function to mute audio
void Unmute();                                  // n04 "      "        "  unmute "

extern unsigned char Audio_Mute;
extern unsigned char Audio_IO_OutLAT;

#ifdef	__cplusplus
}
#endif

#endif	/* BEEP_H */

