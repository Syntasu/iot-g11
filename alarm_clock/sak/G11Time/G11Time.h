#ifndef G11TIME_H
#define G11TIME_H

#include "Arduino.h"

struct date_time
{
    unsigned int years;
    unsigned int months;
    unsigned int days;
    unsigned int hours;
    unsigned int minutes;
    unsigned int seconds;

    date_time() 
    {        
        this->years = 0;
        this->months = 0;
        this->days = 0;
        this->hours = 0;
        this->minutes = 0;
        this->seconds = 0;   
    }

    date_time(int years, int months, int days, int hours, 
                   int minutes, int seconds)
    {
        this->years = years;
        this->months = months;
        this->days = days;
        this->hours = hours;
        this->minutes = minutes;
        this->seconds = seconds;
    }

    // -1 = time was earlier than given time
    // 0 = time is equal to given time
    // 1 = given time is in the future.
    long int difference(date_time t)
    {
        long int diff = this->seconds - t.seconds;
        diff += (this->minutes - t.minutes) * 60;
        diff += (this->hours - t.hours) * 3600;
        diff += (this->days - t.days) * 86400;
        diff += (this->months - t.months) * 2629743;
        diff += (this->years - t.years) * 31556926;

        return diff;
    }
};

class G11Time
{   
public:
    G11Time() {};
    void setup(date_time);
    void simulate(unsigned int); //Simulate the time locally (very inaccurate)
    void sync_with_rtc();        //Syncronize the time with the realtime clock module.
    void sync_with_net();        //Syncronize the time from the net (NTP server).

    date_time get_time();
    String get_time_string();

private:
    int milli_bucket = 0;
    date_time time;
    String padValue(int);
};

#endif