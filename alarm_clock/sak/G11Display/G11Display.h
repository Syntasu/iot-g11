#ifndef G11DISPLAY_H
#define G11DISPLAY_H

#include "Arduino.h"

class G11Display
{
public:
	void init(int latch_pin, int clock_pin, int data_pin);
		void update(int,int);

private:
	int dec[10] = { 126,72,61,109,75,103,119,76,127,79 };
	int LatchPin;
	int ClockPin;
	int DataPin;
	void display_number(int Timearray[], int time_limit);
};


#endif