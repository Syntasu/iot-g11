#include "Arduino.h"
#include "G11Alarm.h"

//Schedule a new alarm by a given date_time.
//ALARM_TIME: The time we want to sound the alarm.
void G11Alarm::schedule_alarm(date_time alarm_time)
{
    this->alarms[alarm_count] = alarm_time;
    alarm_count++;
}

//Check if any of the alarms needs to be played.
//CURRENT_TIME: The current time of the arduino.
bool G11Alarm::check_alarms(date_time current_time)
{
    for(int i = 0; i < alarm_count; i++)
    {
        date_time alarm_time = alarms[i];

        //Get the time difference in seconds
        long int diff = alarm_time.difference(current_time);

        //If it's either negative (surpassed the time) or equal (exactly that time)
        //Then we want to sound the alarm.
        if(diff <= 0)
        {
            return true;
        }
    }

    //We did not find any alarm that needs to ring.
    return false;
}
