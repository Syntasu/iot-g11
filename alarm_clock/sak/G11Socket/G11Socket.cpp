#include "Arduino.h"
#include "G11Socket.h"
#include <SPI.h>
#include <Ethernet.h>

//Start the connection. (without DHCP)
void G11Socket::begin(IPAddress ip, byte mac[])
{
    Ethernet.begin(mac, ip);
    this->server.begin();
}

//Check for any imcoming messages.
void G11Socket::update(bool debug)
{
    EthernetClient client = server.available();

    //Reject client if a) client is null, b) we already serve a client.
    if (!client) return;

    //Variables for reading the incoming messages.
    String temp = "";
    String buff[4];
    int offset = 0;

    if(debug)
    {
        Serial.println("New command received...");
    }

    //Read ALL incoming data!
    while (client.available() > 0)
    {
        char recvChar = client.read();

        //If the character is a delimiter...
        //Store temp into results array.
        //Reset temp and increment offset.
        if (recvChar == ';')
        {
            if(debug)
            {
                Serial.print("IDX: ");
                Serial.print(offset);
                Serial.print(" DATA: ");
                Serial.print(temp);
                Serial.println("; ");
            }

            buff[offset++] = temp;
            temp = "";

        }
        else
        {
            temp += recvChar;
        }
    }

    //Store the last received part.
    buff[offset] = temp;

    //Invoke the bind. Can't touch this! ~ Rick James
    this->invoke_bind(buff[0], buff[1], buff[2], buff[3]);
}

//Bind a function to a command (command = first part of any incoming msg).
void G11Socket::bind(String command, net_cb callback)
{
    //Create a net bindable.
    net_bindable bind = net_bindable(command, callback);

    //Store it.
    this->binds[this->bindIndex++] = bind;
}

//Invoke a net_bindable based on the command that is given.
void G11Socket::invoke_bind(String command, String arg0, String arg1, String arg2)
{
    for(net_bindable bind : this->binds)
    {
        if(bind.command == command)
        {
            //Invoke!
            bind.callback(command, arg0, arg1, arg2);
        }
    }
}