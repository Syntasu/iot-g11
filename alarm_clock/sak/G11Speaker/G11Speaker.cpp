#include "Arduino.h"
#include "G11Speaker.h"

//Constructor for the speaker object.
//PIN: The pin where the speaker is connected to.
G11Speaker::G11Speaker(int pin)
{
    this->speaker_pin = pin;
    pinMode(this->speaker_pin, OUTPUT);
}

//Update the speakers.
//RETURN: How much time we have consumed.
int G11Speaker::update(int milliseconds)
{
    //We currently don't have a pin, no need to update.s
    if(this->speaker_pin <= 0)
    {
        return 0;
    }

    //We don't have anything to play, skip.
    if(this->state == 0)
    {
        //Make sure the speakers doesn't make sound.
        noTone(this->speaker_pin);
        delay(1);

        return 0;
    }
    //Play the next step of the pattern.
    else if(this->state == 1)
    {
        this->elapsed = milliseconds;
        return do_step();
    }

    return 0;
}

//Play a pattern (currently super mario theme song).
void G11Speaker::play(int pattern, bool repeat)
{
    if(pattern == 0)
    {
        this->max = 234;
        this->offset = 0;
        this->state = 1;
        this->repeat = repeat;
    }
}

//Stop playing the speaker, reset all the variables.
void G11Speaker::stop()
{
    this->max = 0;
    this->offset = 0;
    this->state = 0;
    this->repeat = false;
}

int G11Speaker::do_step()
{
    //Read the operand byte from the program memory.
    int operand = pgm_read_byte(&p0[this->offset]);

    //Read the pitch and multiply it by 20.
    int pitch = pgm_read_byte(&p0[this->offset + 1]) * 20;

    //Read the duration from program memory, see how many times per second.
    int duration = 1000 / pgm_read_byte(&p0[this->offset + 2]);

    //Check what operand we are dealing with.
    // 0x0 = Stop playing for x amount of time.
    // 0x1 = Play with pitch x and duration of y.
    if(operand == 0x0)
    {
        noTone(this->speaker_pin);
        delay(duration);
    }
    else if(operand == 0x1)
    {
        tone(this->speaker_pin, pitch);
        delay(duration);
    }

    //Check if we reached the end of the pattern.
    this->offset += 3;
    //Serial.println(this->offset);

    if(this->offset == this->max)
    {
        //Check if we want to repeat it.
        if(this->repeat)
        {
            this->offset = 0;
        }
        else
        {
            this->stop();
        }
    }

    noTone(this->speaker_pin);
    delay(1);

    return duration;
}