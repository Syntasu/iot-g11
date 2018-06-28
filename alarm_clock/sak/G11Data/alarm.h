#ifndef ALARM_H
#define ALARM_H

#include "Arduino.h"

struct alarm
{
    int id = 0;
    date_time time;
    int state = -1; // 0 = Waiting,  1 = ringing, 2 = snooze, 3 = off

    int snoozeTime = 0;
    int snoozeCount = 0;

    alarm() 
    {
        id = -1;
        state = 0;
        time = date_time();
    }

    alarm(int aid, date_time t)
    {
        id = aid;
        time = t;
        state = 0;
    }

    //Get the current state of the alarm.
    int get_state()
    {
        return state;
    }

    //Set the current state of the alarm.
    void set_state(int s)
    {   
        state = s;
    }

    //Set for how long we want to snooze.
    void set_snooze(int t)
    {
        //Don't allow the user to spam the snooze button.
        //For extra evilness to make sure the user really has
        //an horrible way of starting his day.
        if(state == 2) return;

        snoozeTime = t;
        snoozeCount++;

        set_state(2);
    }

    //Update the state of the alarms.
    void update(date_time current_time)
    {
        long diff = this->time.difference(current_time);

        //If we have not yet ringed...
        if(state == 0)
        {
           //Check if we should ring the alarm 
           if(diff <= 0 && this->state != 1)
           {
               set_state(1);
           }
        }
        //If we are snoozing
        else if(state == 2)
        {
            //Check if we surpassed the snooze time.
            if(abs(diff) >= snoozeTime * snoozeCount)
            {
                set_state(1);
            }
        }
    }

    //We completely stopped the alarm.
    void stop()
    {
        set_state(3);
    }
};

#endif