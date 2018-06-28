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

//Convert a given string to date_time struct
date_time G11Util::str_to_datetime(String input)
{
    String temp = "";
    String buff[6];
    int buffIndex = 0;

    for(int i = 0; i < input.length(); i++)
    {
        char c = input[i];

        if(c == '/')
        {
            buff[buffIndex++] = temp;
            temp = "";
        }
        else
        {
            temp += c;
        }
    }

    buff[buffIndex] = temp;

    int year = buff[0].toInt();
    int month = (int)buff[1].toInt();
    int day = (int)buff[2].toInt();
    int hour = (int)buff[3].toInt();
    int minute = (int)buff[4].toInt();
    int second = (int)buff[5].toInt();
    
    return date_time(year, month, day, hour, minute, second);
}

// -> y/m/d 