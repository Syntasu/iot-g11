#ifndef G11SOCKET_H
#define G11SOCKET_H

#include "Arduino.h"
#include "net.h"
#include <SPI.h>
#include <Ethernet.h>

class G11Socket
{
public:
    G11Socket() {};

    void begin(IPAddress, byte[]);
    void update(bool);

    void bind(String, net_cb);
    void invoke_bind(String, String, String, String);

private:
    EthernetServer server = EthernetServer(3333);

    int bindIndex = 0;
    net_bindable binds[8];
};


#endif