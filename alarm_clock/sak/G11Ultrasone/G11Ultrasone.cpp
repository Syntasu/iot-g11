#include "Arduino.h"
#include "G11Ultrasone.h"

void G11Ultrasone::Snooze()
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
	if (avg == 0)
	{
		count = 0;
	}
	if ((avg>0) && (avg<100))
	{
		Serial.println("HAND IS BOVEN DE WEKKER");
		count++;
		if (count>4)
		{
			Serial.println("SNOOZE");
			count = 0;
		}
	}
	delay(300);

}

