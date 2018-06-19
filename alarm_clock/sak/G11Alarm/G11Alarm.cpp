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
//RETURN: 0 = no event, 1 = ring, 2 = stop
int G11Alarm::check_alarms(date_time current_time)
{
    for(int i = 0; i < this->alarm_count; i++)
    {
        date_time alarm_time = alarms[i];

        //Get the time difference in seconds
        int diff = alarm_time.difference(current_time);

        if(snoozing)
        {
            if(abs(diff) > (snooze_countdown * snooze_count))
            {
                snoozing = false;
                snooze_count++;
                return 1;
            }

            return 2;
        }
        else
        {
            if(!ringing)
            {
                if(diff < 0)
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
