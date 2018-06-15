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

//Compare the current time date (time date within this object) with the given time date.
bool G11Time::compare(unsigned int years, unsigned int months, unsigned int days, unsigned int hours, unsigned int minutes, unsigned int seconds)
{
    if(time.years != years || time.months != months ||
       time.days != days || time.hours != hours ||
       time.minutes != minutes || time.seconds != seconds)
    {
        return false;
    }
    else
    {
        return true;
    }
}

//Get the difference in time in seconds.
long int G11Time::difference(unsigned int years, unsigned int months, unsigned int days, unsigned int hours, unsigned int minutes, unsigned int seconds)
{
    long diff = time.seconds - seconds;
    diff += (time.minutes - minutes) * 60;
    diff += (time.hours - hours) * 3600;
    diff += (time.days - days) * 86400;
    diff += (time.months - months) * 2629743;
    diff += (time.years - years) * 31556926;

    return abs(diff);
}


time_date_data G11Time::get_time()
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

