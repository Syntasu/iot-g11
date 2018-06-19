#include "Arduino.h"
#include "G11Ultrasone.h"

String G11Ultrasone::alarm_state()
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
		return "nothing";
	}
	else if (count>4)
	{

		count = 0;
		return "snooze";
	}

	else if ((avg>0) && (avg<100))
	{

		count++;
		return "off";
	}


}

