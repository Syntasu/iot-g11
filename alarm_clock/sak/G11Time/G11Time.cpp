#include "Arduino.h"
#include "G11Time.h"

//Set the time from where the timer should start.
//INTIAL_TIME: The time where we want to start from.
void G11Time::initialize(date_time initial_time)
{
    this->current_time = initial_time;
}

// Simulate the time based on a given elasped time (very inaccurate, debug purposes).
// MS_ELAPSED: The amout of millisecond that have elapsed this frame.
void G11Time::simulate(float ms_elapsed)
{
    //Convert milliseconds to seconds, keep the remainder in the millisecond bucket.
    int inc = 0;
    this->milli_bucket += ms_elapsed;

    if(this->milli_bucket > 1000.0f)
    {
        inc = (int)(this->milli_bucket / 1000.0f);
        this->milli_bucket -= (inc * 1000.0f);
    }

    //Increment time.
    int seconds = this->current_time.seconds + inc;
    int minutes = this->current_time.minutes;
    int hours = this->current_time.hours;
    int days = this->current_time.days;
    int months = this->current_time.months;
    int years = this->current_time.years;

    //Check if each of the time units needs to wrap around.
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

    //Update the struct (we can't just say this->time.seconds = x, because it's a copy). #justcppthings.
    this->current_time = date_time(years, months, days, hours, minutes, seconds);
}

//Adjust the time and date of the clock
void G11Time::set_datetime(date_time dt)
{
    this->current_time = dt;
}

//Boilerplate to return the current time.
date_time G11Time::get_time()
{
    return this->current_time;
}

//Return the time as a string.
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

//Convert 0 to a padded string (i.e. 5 becomes 05).
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