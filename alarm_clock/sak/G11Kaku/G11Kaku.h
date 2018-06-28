#ifndef G11KAKU_H
#define G11KAKU_H

#include "Arduino.h"
#include <NewRemoteTransmitter.h> 

class G11Kaku
{
public:
    G11Kaku(unsigned long, byte, unsigned int, byte);
    void init();
    void set_kaku(int, bool);

    void toggle(bool);

private:
    int kaku_ids[5];
    bool kaku_states[5];
    NewRemoteTransmitter transmitter = NewRemoteTransmitter(0,0, 0, 0);


};

#endif