#ifndef G11ULTRASONE_H
#define G11ULTRASONE_H

#include "Arduino.h"


class G11Ultrasone
{
public:
	int alarm_off();

private:
	long duration;
	int s;
	long avg;
	int sum;
	int SensVals[6];
	int count = 0;
	int trigPin = A2;
	int echoPin = A3;
};

#endif