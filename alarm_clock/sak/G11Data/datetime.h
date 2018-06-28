#ifndef DATE_TIME_H
#define DATE_TIME_H

#include "Arduino.h"

struct date_time
{
    unsigned int years;
    unsigned int months;
    unsigned int days;
    unsigned int hours;
    unsigned int minutes;
    unsigned int seconds;

    //Default constructor, set everything to 0.
    date_time() 
    {        
        this->years = 0;
        this->months = 0;
        this->days = 0;
        this->hours = 0;
        this->minutes = 0;
        this->seconds = 0;   
    }

    //Constructor, set the given variables accordingly.
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

    //Find the difference between two date_times in seconds.
    long difference(date_time t)
    {
        //Convert everything to int, it doesn't like substracting unsigned integers to negative numbers :)
        long seconds = (int)this->seconds - (int)t.seconds;
        long minutes = ((int)this->minutes - (int)t.minutes) * 60;
        long hours = ((int)this->hours - (int)t.hours) * 3600;
        long days = ((int)this->days - (int)t.days) * 86400;
        long months = ((int)this->months - (int)t.months) * 2629743;
        long years = ((int)this->years - (int)t.years) * 31556926;
        
        return seconds + minutes + hours + days + months + years;
    }

    bool is_identity()
    {
        return this->years == 0 && this->months == 0 &&
               this->days == 0 && this->minutes == 0 &&
               this->seconds == 0;
    }
};

#endif