#include "Arduino.h"
#include "G11Alarm.h"

//Schedule a new alarm by a given date_time.
//ALARM_TIME: The time we want to sound the alarm.
bool G11Alarm::add_alarm(alarm new_alarm)
{
    if(alarm_count >= MAX_ALARMS)
    {
        return false;
    }

    this->alarms[alarm_count] = new_alarm;
    alarm_count++;

    return true;
}

//Edit an existing alarm.
bool G11Alarm::edit_alarm(alarm edit_alarm)
{
  for(int i = 0; i < alarm_count; i++)
    {
        if(this->alarms[i].id == edit_alarm.id)
        {
            //Overwrite index with a new value.
            this->alarms[i] = edit_alarm;
            return true;
        }
    }

    return false;
}

//Remove an alarm from the pool
void G11Alarm::remove_alarm(int id)
{
    //Loop over all the alarms and see which need to be added
    //  (all the alarms except the one that needs to be removed)
    //  We simply write over the old array for memory consumption purposes.
    int writeIndex = 0;
    for(int i = 0; i < alarm_count; i++)
    {
        alarm a = this->alarms[i];

        if(a.id != id)
        {
            this->alarms[writeIndex++] = a;
        }
    }

    //Store the new count.
    this->alarm_count = writeIndex;
}

//Fetch some information  about an alarm.
alarm G11Alarm::get_alarm(int id)
{
    for(int i = 0; i < alarm_count; i++)
    {
        if(this->alarms[i].id == id)
        {
            return this->alarms[i];
        }
    }

    return alarm();
}

//Fetch the count of alarms.
int G11Alarm::get_alarm_count()
{
    return this->alarm_count;
}

//Update all the alarms
//CURRENT_TIME: The current time of the alarm.
//RETURN: Wether it should ring or not.
int G11Alarm::update(date_time current_time)
{
    int state = 0;

    for(int i = 0; i < alarm_count; i++)
    {
        alarm a = this->alarms[i];

        if(a.id >= 0)
        {
            a.update(current_time);
            int alarm_state = a.get_state();

            //We turned off the alarm....
            if(alarm_state == 3)
            {
                state = 2;
                this->remove_alarm(a.id);
            }
            //A alarm indicated we should ring the alarm.
            else if(alarm_state == 1)
            {
                state = 1;
            }

            //Copy back the alarm, we mutated some data in a copy of the alarm struct
            //There for we need to copy back into the array to make sure the data
            //is being persisted in the array.
            this->alarms[i] = a;
        }
    }

    return state;
}

//Snooze the alarm by a given amount of seconds.
void G11Alarm::snooze(int snooze_time)
{
    for(int i = 0; i < alarm_count; i++)
    {
        alarm a = this->alarms[i];

        if(a.id >= 0)
        {
            Serial.println(a.id);
            a.set_snooze(snooze_time);

            //Copy back the alarm, we mutated some data in a copy of the alarm struct
            //There for we need to copy back into the array to make sure the data
            //is being persisted in the array.
            this->alarms[i] = a;
        }
    }
}

//Stop all the alarms. 
void G11Alarm::stop()
{
    for(int i = 0; i < alarm_count; i++)
    {
        alarm a = this->alarms[i];

        if(a.id >= 0)
        {
            int alarm_state = a.get_state();

            if(alarm_state == 1 || alarm_state == 2)
            {
                a.stop();

                //Copy back the alarm, we mutated some data in a copy of the alarm struct
                //There for we need to copy back into the array to make sure the data
                //is being persisted in the array.
                this->alarms[i] = a;
            }
        }
    }
}
