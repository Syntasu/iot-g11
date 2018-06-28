#ifndef G11ALARM_H
#define G11ALARM_H

#include "Arduino.h"
#include "datetime.h"
#include "alarm.h"

#define MAX_ALARMS 10

class G11Alarm
{
public:
    bool add_alarm(alarm);
    bool edit_alarm(alarm);
    void remove_alarm(int id);
    alarm get_alarm(int id);
    int get_alarm_count();
    int update(date_time);
    void snooze(int);
    void stop();

private:
    int alarm_count = 0;
    alarm alarms[MAX_ALARMS];
};

#endif