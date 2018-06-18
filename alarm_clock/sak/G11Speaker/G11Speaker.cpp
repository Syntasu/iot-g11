#include "Arduino.h"
#include "G11Speaker.h"

//Constructor for the speaker object.
//PIN: The pin where the speaker is connected to.
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

//Play a pattern (currently super mario theme song).
int G11Speaker::play_pattern()
{
    for(int i = 0; i < 234; i += 3)
    {
        //Read the operand byte from the program memory.
        int operand = pgm_read_byte(&super_mario_pattern[i]);

        //Read the pitch and multiply it by 20.
        int pitch = pgm_read_byte(&super_mario_pattern[i+1]) * 20;

        //Read the duration from program memory, see how many times per second.
        int duration = 1000 / pgm_read_byte(&super_mario_pattern[i+2]);

        //Check what operand we are dealing with.
        // 0x0 = Stop playing for x amount of time.
        // 0x1 = Play with pitch x and duration of y.
        if(operand == 0x0)
        {
            this->stop(duration);
        }
        else if(operand == 0x1)
        {
            this->play(pitch, duration);
        }
    }

    //Immediately terminate any sound.
    stop(1);
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