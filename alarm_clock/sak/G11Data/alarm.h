#ifndef ALARM_H
#define ALARM_H

#include "Arduino.h"

struct alarm
{
    int id = 0;
    date_time time;
    int state = 0; // 0 = Waiting,  1 = ringing, 2 = snooze, 3 = off

    alarm() 
    {
        this->id = 0;
        this->state = 0;
        this->time = date_time();
    }

    alarm(int aid, date_time t)
    {
        this->id = aid;
        this->time = t;
        this->state = 0;
    }

    int get_state()
    {
        return state;
    }
};

#endif