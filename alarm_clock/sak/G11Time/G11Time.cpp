#include "Arduino.h"
#include "G11Time.h"

void G11Time::setup(date_time time)
{
    this->time = time;
}

void G11Time::simulate(unsigned int ms_elapsed)
{
    int inc = 0;
    this->milli_bucket += ms_elapsed;

    if(milli_bucket > 1000)
    {
        inc = ceil(this->milli_bucket / 1000);
        this->milli_bucket -= (inc * 1000);
    }

    int seconds = time.seconds + inc;
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

    time = date_time(years, months, days, hours, minutes, seconds);
}

void G11Time::sync_with_rtc()
{
    //TODO: Syncronize with the RTC module
}

void G11Time::sync_with_net()
{
    //TODO: Syncronize over net.
}

date_time G11Time::get_time()
{
    return time;
}

String G11Time::get_time_string()
{
    String timeStr = String("");

    timeStr += padValue(time.days);
    timeStr += "/";
    timeStr += padValue(time.months);
    timeStr += "/";
    timeStr += padValue(time.years);
    timeStr += " ";
    timeStr += padValue(time.hours);
    timeStr += ":";
    timeStr += padValue(time.minutes);
    timeStr += ".";
    timeStr += padValue(time.seconds);

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

