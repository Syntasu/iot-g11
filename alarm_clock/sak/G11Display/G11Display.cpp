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
	
	//This will display the numbers in the Time Array.
	for (int i = 0; i < 4; i++) 
	{
		DoStuff(i, 9);
		//display_number(Timearray[i], 10);
	
	}
}

void G11Display::display_number(int digit, int time_limit) 
{

}

void G11Display::DoStuff(int segment, int digit)
{
	int offset = segment * 4;
	int d = this->dec[digit];

	shiftOut(this->DataPin, this->ClockPin, MSBFIRST, d);
	digitalWrite(this->LatchPin, HIGH);

	digitalWrite(offset, LOW);
	digitalWrite(offset, HIGH);
	digitalWrite(offset, LOW);

	digitalWrite(this->LatchPin, LOW);
	shiftOut(this->DataPin, this->ClockPin, MSBFIRST, 0);
	digitalWrite(this->LatchPin, HIGH);
	digitalWrite(offset, HIGH);
}