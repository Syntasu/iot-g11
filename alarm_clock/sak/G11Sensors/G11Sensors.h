#ifndef G11SENSORS_H
#define G11SENSORS_H

#include "Arduino.h"
#include <DHT.h>



class G11Sensors
{
public:
	int give_temp();
	int give_hum();
	String isitdark();
	int give_ldrval();
	

private:
	float hum;
	float temp;
	DHT dht = DHT(A1, DHT11);
	int ldrval;
};

#endif