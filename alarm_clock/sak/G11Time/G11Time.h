#ifndef G11TIME_H
#define G11TIME_H

#include "Arduino.h"
#include "datetime.h"

class G11Time
{   
public:
    G11Time() {};
    void initialize(date_time);
    void simulate(float); //Simulate the time locally
    void set_datetime(date_time);

    date_time get_time();
    String get_time_string();

private:
    float milli_bucket = 0.0f;
    date_time current_time;
    String padValue(int);
};

#endif