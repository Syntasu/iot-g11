#include "Arduino.h"
#include "G11Display.h"

void G11Display::init(int LatchPin, int ClockPin, int DataPin) {
	this->LatchPin = LatchPin;
	this->ClockPin = ClockPin;
	this->DataPin = DataPin;

	// setting the pins to output.
	pinMode(LatchPin, OUTPUT);
	pinMode(ClockPin, OUTPUT);
	pinMode(DataPin, OUTPUT);
	pinMode(4, OUTPUT);
	pinMode(5, OUTPUT);
	pinMode(6, OUTPUT);
	pinMode(7, OUTPUT);
}

void G11Display::update(int hour, int min) 
{
	int digit01 = min % 10;
	int digit02 = (int)floor(min / 10);
	int digit03 = hour % 10;
	int digit04 = (int)floor(hour / 10);

	//The array what will be shown on the display. 
	int Timearray[][4] = {
		{ digit01,digit02,digit03,digit04 }
	};

	display_number(Timearray[0], 1);
}

void G11Display::display_number(int Timearray[], int time_limit) 
{
	int count;
	int dp;

	//This entire outer loop is kind of a delay loop, it will loop the four digits displayed over and over until time_limit is reached.
	for (int t = 0; t < time_limit; t++) {
		//The loop below will loop through the 4 digits being displayed once.
		for (int j = 0; j<4; j++) {
			count = Timearray[j];
			int i = j + 4;
			//Latch pin is low so the display will not flikker
			digitalWrite(this->LatchPin, LOW);

			//We found the number we want to display      
			dp = this->dec[count];

			//This function shifts the 8-bit information into the shift register
			shiftOut(this->DataPin, this->ClockPin, MSBFIRST, dp);
			digitalWrite(this->LatchPin, HIGH);

			digitalWrite(i, LOW);
			digitalWrite(i, HIGH);
			digitalWrite(i, LOW);

			digitalWrite(this->LatchPin, LOW);
			shiftOut(this->DataPin, this->ClockPin, MSBFIRST, 0);
			digitalWrite(this->LatchPin, HIGH);
			digitalWrite(i, HIGH);

		}
	}
}
