#ifndef G11ALARM_H
#define G11ALARM_H

#include "Arduino.h"
#include "datetime.h"

class G11Alarm
{
public:
    void schedule_alarm(date_time);
    int check_alarms(date_time);

    void snooze(int);
    void kill(date_time);

private:
    int alarm_count = 0;
    date_time alarms[8];

    //Time to snooze in seconds.
    int snooze_countdown = 0;
    int snooze_count = 0;

    bool snoozing = false;
    bool ringing = false;
    bool kill = false;
};

#endif