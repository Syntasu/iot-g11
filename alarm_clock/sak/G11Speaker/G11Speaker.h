#ifndef G11SPEAKER_H
#define G11SPEAKER_H

#include "Arduino.h"


class G11Speaker
{   
public:
    G11Speaker(int);

    int play(int, int);
    int play_pattern();
    int stop(int);

private:
    int speaker_pin = -1;
    int pattern;
};


/*
    ***********************************
    ********Speaker patterns***********
    ***********************************

    Pattenr consists of 3 bytes:
    
    ------------------------------------
    |  operand  |  pitch  |  duration  |
    ------------------------------------

    Operand = either 0x0 or 0x1;
    0x0 = Stop playing.
    0x1 = Start playing.
    Pitch = Pitch used for the 0x1 operand. 0x0 will ignore pitch
    Duration = How long we want to play or stop.

    Operand, pitch and duration are represented in hexidecimal
    with the limit of 0 ≤ value ≤ 255.

    The pitch is multiplied by 20 and the duration by 10.
*/

const byte super_mario_pattern[234] PROGMEM =
{
    1, 132, 12, 1, 132, 12, 0, 0, 12, 1, 132, 12, 0, 0, 12, 1, 105, 12, 1, 132, 12,
    0, 0, 12, 1, 157, 12, 0, 0, 12, 0, 0, 12, 0, 0, 12, 1, 78, 12, 0, 0, 12, 0, 0, 
    12, 0, 0, 12, 1, 105, 12, 0, 0, 12, 0, 0, 12, 1, 78, 12,0, 0, 12, 0, 0, 12, 1,
    66, 12,  0, 0, 12, 0, 0, 12, 1, 88, 12, 0, 0, 12, 1, 99, 12, 0, 0, 12, 1, 93, 12,
    1, 88, 12, 0, 0, 12, 1, 78, 10, 1, 132, 10, 1, 157, 10, 1, 176, 12, 0, 0, 12, 1, 140,
    12, 1, 157, 12, 0, 0, 12, 1, 132, 12, 0, 0, 12, 1, 105, 12, 1, 116, 12, 1, 99, 12,
    0, 0, 12, 0, 0, 12, 1, 105, 12, 0, 0, 12, 0, 0, 12, 1, 78, 12, 0, 0, 12, 0, 0, 12,
    1, 67, 12, 0, 0, 12, 0, 0, 12, 1, 88, 12, 0, 0, 12, 1, 99, 12, 0, 0, 12, 1, 93, 12, 
    1, 88, 12,  0, 0, 12, 1, 78, 10, 1, 132, 10, 1, 157, 10, 1, 176, 12, 0, 0, 12, 1, 140, 
    12, 1, 157, 12, 0, 0, 12, 1, 132, 12, 0, 0, 12, 1, 157, 12, 1, 116, 12, 1, 99, 12, 
    0, 0, 12, 0, 0, 12,
};

#endif