#ifndef G11ALARM_H
#define G11ALARM_H

#include "Arduino.h"
#include "datetime.h"
#include "alarm.h"

#define MAX_ALARMS  16

class G11Alarm
{
public:
    bool add_alarm(alarm);
    bool edit_alarm(alarm);
    bool remove_alarm(alarm);

    int check_alarms(date_time);

    void snooze(int);
    void kill(date_time);

private:
    int alarm_count = 0;
    alarm alarms[MAX_ALARMS];

    //Time to snooze in seconds.
    int snooze_countdown = 0;
    int snooze_count = 0;

    bool snoozing = false;
    bool ringing = false;
    bool stop_alarms = false;
};

#endif