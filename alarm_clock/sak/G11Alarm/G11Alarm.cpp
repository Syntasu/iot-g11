#include "Arduino.h"
#include "G11Alarm.h"

void G11Alarm::schedule_alarm(date_time time)
{
    alarms[alarm_count] = time;
    alarm_count++;
}

bool G11Alarm::check_alarms(date_time current_time)
{
    for(int i = 0; i < alarm_count; i++)
    {
        date_time alarm_time = alarms[i];
        long int diff = alarm_time.difference(current_time);

        Serial.println(diff);
    }

    return false;
}
