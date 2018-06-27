#ifndef ALARM_H
#define ALARM_H

#include "Arduino.h"

struct alarm
{
    String name;

    alarm() 
    {
        this->name = "Jane doe"; 
    }
};

#endif