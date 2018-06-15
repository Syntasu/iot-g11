#include "Arduino.h"
#include "G11Alarm.h"

void G11Alarm::schedule_alarm(date_time alarm_time)
{
    this->alarms[alarm_count] = alarm_time;
    alarm_count++;
}

bool G11Alarm::check_alarms(date_time current_time)
{
    for(int i = 0; i < alarm_count; i++)
    {
        date_time alarm_time = alarms[i];
        long int diff = alarm_time.difference(current_time);

        if(diff <= 0)
        {
            return true;
        }
    }

    return false;
}
