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

    //Don't allow duplicate alarms to be added.
    for(int i = 0; i < alarm_count; i++)
    {
        if(this->alarms[i].id == new_alarm.id)
        {
            return false;
        }
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
bool G11Alarm::remove_alarm(alarm remove_alarm)
{
    this->alarm_count = 0;
    int writeIndex = 0;
    for(int i = 0; i < alarm_count; i++)
    {
        if(this->alarms[i].id != remove_alarm.id)
        {
            this->alarms[writeIndex++] = this->alarms[i];
            this->alarm_count++;
        }
    }

    return true;
}

//Fetch some infor about an alarm.
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

//Check if any of the alarms needs to be played.
//CURRENT_TIME: The current time of the arduino.
//RETURN: 0 = no event, 1 = ring, 2 = snooze, 3 = stop
//TODO: After killing, the alarms stay in snooze mode...
int G11Alarm::check_alarms(date_time current_time)
{
    // for(int i = 0; i < 8; i++)
    // {
    //     alarm alarm_time = alarms[i];

    //     //Don't read empty indexes.
    //     if(alarm_time.is_identity()) continue;

    //     //Get the time difference in seconds
    //     int diff = alarm_time.difference(current_time);

    //     //Tell the main code to stop sounding alarms.
    //     if(stop_alarms)
    //     {
    //         stop_alarms = false;
    //         return 3;
    //     }

    //     //If we are snoozing...
    //     if(snoozing)
    //     {
    //         //Check if we surpassed the snooze countdown.
    //         if(abs(diff) > (snooze_countdown * snooze_count))
    //         {
    //             snoozing = false;
    //             snooze_count++;
    //             return 1;
    //         }

    //         //Else return 2...
    //         return 2;
    //     }

    //     //If we are not snoozing....
    //     else
    //     {
    //         //If we haven't rung out alarms yets..
    //         if(!ringing)
    //         {
    //             //And the time surpassed the alarm, then we want to ring it.
    //             if(diff <= 0)
    //             {
    //                 ringing = true;
    //                 return 1;
    //             }
    //         }
    //     }

    //     return 0;
    // }

    return 0;
}

//Snooze the alarm by a given amount of seconds.
void G11Alarm::snooze(int snooze_sec)
{
    snooze_countdown = snooze_sec;
    snooze_count++;
    snoozing = true;
}

//Kill all alarms!
void G11Alarm::kill(date_time current_time)
{
    // stop_alarms = true;

    // for(int i = 0; i < this->alarm_count; i++)
    // {
    //     date_time alarm_time = alarms[i];

    //     //Don't read empty indices.
    //     if(alarm_time.is_identity()) continue;

    //     //Get the time difference in seconds
    //     int diff = alarm_time.difference(current_time);

    //     //Remove any alarms that have already rung.
    //     if(diff <= 0)
    //     {
    //         alarms[i] = date_time(); //Assign identity.
    //         alarm_count--;
    //     }
    // }

    // //Reset everything!
    // snooze_countdown = 0;
    // snooze_count = 0;
    // snoozing = false;
    // ringing = false;
}
