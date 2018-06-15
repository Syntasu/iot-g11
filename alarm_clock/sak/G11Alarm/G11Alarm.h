#ifndef G11ALARM_H
#define G11ALARM_H

#include "Arduino.h"
#include "datetime.h"

class G11Alarm
{
public:
    void schedule_alarm(date_time);
    bool check_alarms(date_time);

private:
    int alarm_count = 0;
    date_time alarms[24];
};

#endif