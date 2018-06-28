#include "Arduino.h"
#include "G11Kaku.h"

//Create the remote transmitter object with the correct values.
G11Kaku::G11Kaku(unsigned long code, byte pin, unsigned int interval, byte retries)
{
    this->transmitter = NewRemoteTransmitter(code, pin, interval, retries);
}

void G11Kaku::init()
{
    for(int i = 0; i < 5; i++)
    {
        this->kaku_ids[i] = (i + 1);
        this->kaku_states[i] = false;
    }

}

//Enable/disable an kaku
void G11Kaku::set_kaku(int id, bool state)
{
    for(int i = 0; i < 5; i++)
    {
        Serial.print(this->kaku_ids[i]);
        Serial.print(" == ");
        Serial.print(id);

        if(this->kaku_ids[i] == id)
        {
            this->kaku_states[i] = state;
        }
        Serial.print(" -> ");
        Serial.println(this->kaku_states[i]);
    }
}


//Set the state of the kaku.
void G11Kaku::toggle(bool state)
{
    for(int i = 0; i < 5; i++)
    {
        Serial.println("toggle");
        Serial.println(this->kaku_states[i]);
        if(this->kaku_states[i])
        {        
            this->transmitter.sendUnit(i, state);
            delay(260);
        }
    }
}
