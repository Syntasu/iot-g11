#ifndef G11SPEAKER_H
#define G11SPEAKER_H

#include "Arduino.h"

class G11Speaker
{   
public:
    G11Speaker(int);

    int play(int, int);
    int stop(int);

private:
    int speaker_pin = -1;
};

#endif