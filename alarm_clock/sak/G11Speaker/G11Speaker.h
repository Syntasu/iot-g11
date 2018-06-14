#ifndef G11SPEAKER_H
#define G11SPEAKER_H

#include "Arduino.h"

class G11Speaker
{   
public:
    G11Speaker(int, bool);

    void play(int, int);
    void stop(int);

private:
    int speaker_pin = -1;
    bool debug = false;

    void print(String);
};

#endif