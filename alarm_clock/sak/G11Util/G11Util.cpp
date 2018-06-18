#include "Arduino.h"
#include "G11Util.h"

//Delay for x amount of time and keep track of the total delay.
//DURATION: Time we want to delay for.
void G11Util::timed_delay(int duration)
{
    delay(duration);
    this->delayed += duration;
}

//Keep track of the delayed time but do not actually delay.
//Handy for when you know you've delayed the code and just want to take it into account.
//DURATION: The time we have delayed for.
void G11Util::virtual_delay(int duration)
{
    this->delayed += duration;
}

//Gets the total time we have delayed for (in this frame).
//RESET: True will force the delay counter back to zero.
int G11Util::get_total_delay(bool reset)
{
    int value = this->delayed;
    
    if(reset) 
    {
        this->delayed = 0;
    }

    return value;
}