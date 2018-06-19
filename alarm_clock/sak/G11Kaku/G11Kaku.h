#ifndef G11KAKU_H
#define G11KAKU_H

#include "Arduino.h"
#include <NewRemoteTransmitter.h> 

class G11Kaku
{
public:
    G11Kaku(unsigned long, byte, unsigned int, byte);
    void init_kaku(int);
    void set_kaku(int, bool);

private:
    NewRemoteTransmitter transmitter = NewRemoteTransmitter(0,0, 0, 0);


};

#endif