#include "Arduino.h"
#include "G11Time.h"

void G11Time::setup(unsigned int year, unsigned int month, unsigned int day, unsigned int hour, unsigned int minutes, unsigned int seconds)
{
    this->time = time_date_data(year, month, day, hour, minutes, seconds);
}

void G11Time::simulate(unsigned int seconds_elapsed)
{
    int seconds = time.seconds + seconds_elapsed;
    int minutes = time.minutes;
    int hours = time.hours;
    int days = time.days;
    int months = time.months;
    int years = time.years;

    if(seconds >= 60)
    {
        seconds = 0;
        minutes++;
    }

    if(minutes >= 60)
    {
        minutes = 0;
        hours++;
    }

    if(hours >= 24)
    {
        hours = 0;
        days++;
    }

    //Not accurate! :)
    if(days >= 31)
    {
        days = 0;
        months++;
    }

    if(months > 12)
    {
        months = 0;
        years++;
    }

    time = time_date_data(years, months, days, hours, minutes, seconds);
}

void G11Time::sync_with_rtc()
{
    //TODO: Syncronize with the RTC module
}

void G11Time::sync_with_net()
{
    //TODO: Syncronize over net.
}

time_date_data G11Time::get_time()
{
    return time;
}

String G11Time::get_time_string()
{
    String timeStr = String("");

    timeStr += this->padValue(time.days);
    timeStr += "/";
    timeStr += this->padValue(time.months);
    timeStr += "/";
    timeStr += this->padValue(time.years);
    timeStr += " ";
    timeStr += this->padValue(time.hours);
    timeStr += ":";
    timeStr += this->padValue(time.minutes);
    timeStr += ".";
    timeStr += this->padValue(time.seconds);

    return timeStr;
}

String G11Time::padValue(int input)
{
    String outVal = "";

    if(input < 10)
    {
        outVal += "0";
    }

    outVal += input;
    return outVal;
}

