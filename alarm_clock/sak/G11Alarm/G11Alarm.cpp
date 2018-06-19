#include "Arduino.h"
#include "G11Alarm.h"

//Schedule a new alarm by a given date_time.
//ALARM_TIME: The time we want to sound the alarm.
void G11Alarm::schedule_alarm(date_time alarm_time)
{
    this->alarms[alarm_count] = alarm_time;
    alarm_count++;

    if(alarm_count == 8)
    {
        alarm_count = 0;
    }
}

//Check if any of the alarms needs to be played.
//CURRENT_TIME: The current time of the arduino.
//RETURN: 0 = no event, 1 = ring, 2 = snooze, 3 = stop
int G11Alarm::check_alarms(date_time current_time)
{
    for(int i = 0; i < 8; i++)
    {
        date_time alarm_time = alarms[i];

        //Don't read empty indexes.
        if(alarm_time.is_identity()) continue;

        //Get the time difference in seconds
        int diff = alarm_time.difference(current_time);

        if(kill)
        {
            kill = false;
            return 3;
        }

        //If we are snoozing...
        if(snoozing)
        {
            //Check if we surpassed the snooze countdown.
            if(abs(diff) > (snooze_countdown * snooze_count))
            {
                snoozing = false;
                snooze_count++;
                return 1;
            }

            //Else return 2...
            return 2;
        }

        //If we are not snoozing....
        else
        {
            //If we haven't rung out alarms yets..
            if(!ringing)
            {
                //And the time surpassed the alarm, then we want to ring it.
                if(diff <= 0)
                {
                    ringing = true;
                    return 1;
                }
            }
        }

        return 0;
    }
}

void G11Alarm::snooze(int snooze_sec)
{
    snooze_countdown = snooze_sec;
    snooze_count++;
    snoozing = true;
}

void G11Alarm::kill(date_time current_time)
{
    kill = true;

    for(int i = 0; i < this->alarm_count; i++)
    {
        date_time alarm_time = alarms[i];

        //Don't read empty indices.
        if(alarm_time.is_identity()) continue;

        //Get the time difference in seconds
        int diff = alarm_time.difference(current_time);

        //Remove any alarms that have already rung.
        if(diff <= 0)
        {
            alarms[i] = date_time(); //Assign identity.
            alarm_count--;
        }
    }

    snooze_countdown = 0;
    snooze_count = 0;
    snoozing = false;
    ringing = false;
}
