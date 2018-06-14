#include "Arduino.h"
#include "G11Speaker.h"

G11Speaker::G11Speaker(int pin, bool debug)
{
    this->speaker_pin = pin;
    this->debug = debug;

    pinMode(speaker_pin, OUTPUT);
}

void G11Speaker::play(int pitch, int duration)
{
    if(speaker_pin <= 0) this->print("Speaker pin is not defined or configured,");

    tone(speaker_pin, pitch);
    delay(duration);
}

void G11Speaker::stop(int duration)
{
    if(speaker_pin <= 0) this->print("Speaker pin is not defined or configured.");

    noTone(speaker_pin);
    delay(duration);
}

void G11Speaker::print(String message)
{
    if(debug)
    {
        Serial.println(message);
    }
}