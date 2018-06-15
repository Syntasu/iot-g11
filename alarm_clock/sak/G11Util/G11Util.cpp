#include "Arduino.h"
#include "G11Util.h"

void G11Util::timed_delay(int duration)
{
    delay(duration);
    this->delayed += duration;
}

int G11Util::get_total_delay(bool reset)
{
    int value = this->delayed;
    if(reset) 
    {
        this->delayed = 0;
    }

    return value;
}