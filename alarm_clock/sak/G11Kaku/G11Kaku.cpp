#include "Arduino.h"
#include "G11Kaku.h"

G11Kaku::G11Kaku(unsigned long code, byte pin, unsigned int interval, byte retries)
{
    this->transmitter = NewRemoteTransmitter(code, pin, interval, retries);
}

void G11Kaku::init_kaku(int id)
{
    this->transmitter.sendUnit(id, false);
    delay(260);
}

void G11Kaku::set_kaku(int id, bool state)
{
    this->transmitter.sendUnit(id, state);
}

bool G11Kaku::get_kaku_state(int id)
{
    return this->kaku_states[id];
}