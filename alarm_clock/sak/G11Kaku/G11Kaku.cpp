#include "Arduino.h"
#include "G11Kaku.h"

//Create the remote transmitter object with the correct values.
G11Kaku::G11Kaku(unsigned long code, byte pin, unsigned int interval, byte retries)
{
    this->transmitter = NewRemoteTransmitter(code, pin, interval, retries);
}

//Initialize; Turn off the kaku's by default.
void G11Kaku::init_kaku(int id)
{
    this->transmitter.sendUnit(id, false);
    delay(260);
}

//Set the state of the kaku.
void G11Kaku::set_kaku(int id, bool state)
{
    this->transmitter.sendUnit(id, state);
}
