#ifndef G11TIME_H
#define G11TIME_H

#include "Arduino.h"

struct time_date_data
{
    unsigned int years;
    unsigned int months;
    unsigned int days;
    unsigned int hours;
    unsigned int minutes;
    unsigned int seconds;

    time_date_data() 
    {        
        this->years = 0;
        this->months = 0;
        this->days = 0;
        this->hours = 0;
        this->minutes = 0;
        this->seconds = 0;   
    }

    time_date_data(int years, int months, int days, int hours, 
                   int minutes, int seconds)
    {
        this->years = years;
        this->months = months;
        this->days = days;
        this->hours = hours;
        this->minutes = minutes;
        this->seconds = seconds;
    }
};

class G11Time
{   
public:
    G11Time() {};
    void setup(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
    void simulate(unsigned int); //Simulate the time locally (very inaccurate)
    void sync_with_rtc();        //Syncronize the time with the realtime clock module.
    void sync_with_net();        //Syncronize the time from the net (NTP server).

    time_date_data get_time();
    String get_time_string();

private:
    time_date_data time;
    String padValue(int);
};

#endif