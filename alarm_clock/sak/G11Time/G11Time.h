#ifndef G11TIME_H
#define G11TIME_H

#include "Arduino.h"
#include "datetime.h"
#include <DS1302.h>

class G11Time
{   
public:
    G11Time() {};
    void initialize(date_time);
    void simulate(unsigned int); //Simulate the time locally (very inaccurate)
    void sync_with_rtc();        //Syncronize the time with the realtime clock module.
    void sync_with_net();        //Syncronize the time from the net (NTP server).

    date_time get_time();
    String get_time_string();

private:
    int milli_bucket = 0;
    date_time current_time;
    String padValue(int);
	Time t;

	 int kCePin = 0;  // Chip Enable / rst
	 int kIoPin = A4;  // DATA 
	 int kSclkPin = A5;  // Serial Clock		
	 DS1302 rtc = DS1302(kCePin, kIoPin, kSclkPin);  // Create a DS1302 object.
};

#endif