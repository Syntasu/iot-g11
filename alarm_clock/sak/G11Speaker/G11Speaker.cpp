#include "Arduino.h"
#include "G11Speaker.h"

//Constructor for the speaker object.
G11Speaker::G11Speaker(int pin)
{
    this->speaker_pin = pin;
    pinMode(speaker_pin, OUTPUT);
}

//Play a pitch for x amount of time.
//PITCH: Pitch that the speaker needs to produce
//DURATION: For how long it needs to keep that tone.
//RETURN: 0 = OK, 1 = Pin not connected.
int G11Speaker::play(int pitch, int duration)
{
    if(speaker_pin <= 0) 
    { 
        return 1;
    }

    tone(speaker_pin, pitch);
    delay(duration);

    return 0;
}

//Stop the speaker with what ever it was doing.
//DURATION: For how long it we want to stop playing
//RETURN: 0 = OK, 1 = Speaker class was not initialed
int G11Speaker::stop(int duration)
{
    if(speaker_pin <= 0)
    {
        return 1;
    }

    noTone(speaker_pin);
    delay(duration);
    return 0;
}