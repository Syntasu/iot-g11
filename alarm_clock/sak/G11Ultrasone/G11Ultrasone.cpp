#include "Arduino.h"
#include "G11Ultrasone.h"

//Returns 2 if we should stop all alarms from ringing.
//Returns 1 if we should snooze the alarms.
//Returns 0 if nothing significant happend.
int G11Ultrasone::get_state()
{
	int avg = 1;
	
	for (int x = 0; x < 4; x++)
	{
		pinMode(echoPin, INPUT);
		pinMode(trigPin, OUTPUT);
		digitalWrite(trigPin, LOW);
		delayMicroseconds(2);
		digitalWrite(trigPin, HIGH);
		delayMicroseconds(10);
		digitalWrite(trigPin, LOW);
		long duration = pulseIn(echoPin, HIGH, 5882);
		avg += duration * 0.034 / 2;
		
	}

	avg /= 4;

	//We read something from the sensor
	if (avg > 0)
	{
		//Don't keep counting if we rached max.
		if(count >= 10) return -1;

		count++;

		if(count == 3)
		{
			return_code = 2;
			return 0; // we should beep.
		}

		if(count == 6)
		{
			return_code = 3;
			return 1; // we should beep.
		}

		return -1;
	}
	else
	{
		//We don\t want to start if we didn't meassure anything.
		if(count == 0) return -1;

		//Clamp the count value.
		if(count > 10) count = 10;

		count--; 

		if(count == 0 && return_code > 0)
		{
			int c = return_code;
			return_code = 0;
			count = 0;
			return c;
		}

		return -1;
	}
}

