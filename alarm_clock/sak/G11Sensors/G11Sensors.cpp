#include "Arduino.h"
#include "G11Sensors.h"

int G11Sensors::give_temp()
{
	temp = dht.readTemperature();
	delay(1000);
	return temp;
}

int G11Sensors::give_hum()
{
	hum = dht.readHumidity();
	delay(1000);
	return hum;
}
bool G11Sensors::isitdark()
{
	pinMode(A0, INPUT);
	 ldrval = analogRead(A0);
	 
	if (ldrval < 100) { return true; }
	else { return false; }
}
int G11Sensors::give_ldrval()
{
	pinMode(A0, INPUT);
	ldrval = analogRead(A0);
	
	return ldrval;
}


