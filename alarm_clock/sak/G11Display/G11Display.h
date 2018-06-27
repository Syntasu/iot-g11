#ifndef G11DISPLAY_H
#define G11DISPLAY_H

#include "Arduino.h"

class G11Display
{
public:
	void init(int latch_pin, int clock_pin, int data_pin);
		void update(int,int);

private:
	int LatchPin;
	int ClockPin;
	int DataPin;
	int dec[10] = { 126,72,61,109,75,103,119,76,127,79 };

	void display_number(int, int);
	void DoStuff(int, int);
};


#endif