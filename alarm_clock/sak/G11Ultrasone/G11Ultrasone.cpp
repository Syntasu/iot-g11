#include "Arduino.h"
#include "G11Ultrasone.h"
//returns 2 when the ultrasone sensor detects an object within 100cm, returns 0 when it doesn't. 
//returns 1 when it detects said object for 1.6 seconds or more (=snooze)
int G11Ultrasone::alarm_off()
{
	sum = 0; s = 0; avg = 0;
	
	for (int x = 0; x<6; x++)
	{
		pinMode(echoPin, INPUT);
		pinMode(trigPin, OUTPUT);
		digitalWrite(trigPin, LOW);
		delayMicroseconds(2);
		digitalWrite(trigPin, HIGH);
		delayMicroseconds(10);
		digitalWrite(trigPin, LOW);
		duration = pulseIn(echoPin, HIGH, 5882);
		s = duration * 0.034 / 2;
		SensVals[x] = s;
		sum += SensVals[x];
	}
	avg = sum / 6;

	delay(300);
	if (avg == 0)
	{
		count = 0;
		return 0;
	}
	else if (count>4)
	{

		count = 0;
		return 1;
	}

	else if ((avg>0) && (avg<100))
	{

		count++;
		return 2;
	}


}

