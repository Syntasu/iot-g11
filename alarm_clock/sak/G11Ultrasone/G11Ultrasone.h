#ifndef G11ULTRASONE_H
#define G11ULTRASONE_H

#include "Arduino.h"


class G11Ultrasone
{
public:
	int get_state();

private:
	int count = 0;
	int return_code = 0;
	int trigPin = A2;
	int echoPin = A3;
};

#endif