#ifndef NET_H
#define NET_H

#include "Arduino.h"

typedef void(*net_cb)(String, String, String, String);

struct net_bindable
{
    String command;
    net_cb callback;

    net_bindable()
    {
        this->command = "";
        this->callback = nullptr;
    }

    net_bindable(String command, net_cb callback)
    {
        this->command = command;
        this->callback = callback;
    }
};

#endif