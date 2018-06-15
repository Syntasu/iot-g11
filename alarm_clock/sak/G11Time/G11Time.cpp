#include "Arduino.h"
#include "G11Time.h"

void G11Time::initialize(date_time initial_time)
{
    this->current_time = initial_time;
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

    int seconds = this->current_time.seconds + inc;
    int minutes = this->current_time.minutes;
    int hours = this->current_time.hours;
    int days = this->current_time.days;
    int months = this->current_time.months;
    int years = this->current_time.years;

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

    this->current_time = date_time(years, months, days, hours, minutes, seconds);
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
    return this->current_time;
}

String G11Time::get_time_string()
{
    String timeStr = String("");

    timeStr += padValue(this->current_time.days);
    timeStr += "/";
    timeStr += padValue(this->current_time.months);
    timeStr += "/";
    timeStr += padValue(this->current_time.years);
    timeStr += " ";
    timeStr += padValue(this->current_time.hours);
    timeStr += ":";
    timeStr += padValue(this->current_time.minutes);
    timeStr += ".";
    timeStr += padValue(this->current_time.seconds);

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

