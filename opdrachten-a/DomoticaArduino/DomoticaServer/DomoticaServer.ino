#define UNIT_CODE 28620750 
#define PORT 3300
#define USE_DHCP false

#include <SPI.h> 
#include <Ethernet.h>
#include <NewRemoteTransmitter.h> 

//Set up the socket server.
byte mac[] = { 0x40, 0x6c, 0x8f, 0x36, 0x84, 0x8a };
IPAddress ipAddress = IPAddress(192, 168, 1, 16);
EthernetServer server(PORT);

//Define pin stuff.
#define RF_PIN       7  // output, pin to control the RF-sender (and Click-On Click-Off-device)
#define lowPin       5  // output, always LOW
#define highPin      6  // output, always HIGH
#define switchPin    7  // input, connected to some kind of inputswitch
#define ledPin       8  // output, led used for "connect state": blinking = searching; continuously = connected
#define infoPin      9  // output, more information
#define analogPin    A0  // sensor value

NewRemoteTransmitter kakuTransmitter(UNIT_CODE, RF_PIN, 260, 3);

bool pinState = false;                   // Variable to store actual pin state
bool pinChange = false;                  // Variable to store actual pin change
int  sensorValue = 0;                    // Variable to store actual sensor value
int currentPowerOutlet = 0;
bool p1state, p2state, p3state;          // Variables to store the states for the power outlet transmitter.

void setup()
{
   Serial.begin(9600);
   Serial.println("Domotica project, Arduino Domotica Server\n");
   
   //Init I/O-pins
   pinMode(switchPin, INPUT);
   pinMode(lowPin, OUTPUT);
   pinMode(highPin, OUTPUT);
   pinMode(RF_PIN, OUTPUT);
   pinMode(ledPin, OUTPUT);
   pinMode(infoPin, OUTPUT);
   
   //Default states
   digitalWrite(switchPin, HIGH);
   digitalWrite(lowPin, LOW);
   digitalWrite(highPin, HIGH);
   digitalWrite(RF_PIN, LOW);
   digitalWrite(ledPin, LOW);
   digitalWrite(infoPin, LOW);

#if USE_DHCP
   //Try to get an IP address from the DHCP server.
   if (Ethernet.begin(mac) == 0)
   {
      Serial.println("Could not obtain IP-address from DHCP -> do nothing");
      while (true){     // no point in carrying on, so do nothing forevermore; check your router
      }
   }
#else
  Ethernet.begin(mac, ipAddress);
#endif
   
   Serial.print("LED (for connect-state and pin-state) on pin "); 
   Serial.println(ledPin);
   
   Serial.print("Input switch on pin "); 
   Serial.println(switchPin);
   
   Serial.println("Ethernetboard connected (pins 10, 11, 12, 13 and SPI)");
   Serial.println("Connect to DHCP source in local network (blinking led -> waiting for connection)");
   
   //Start the ethernet server.
   server.begin();

   // Print IP-address and led indication of server state
   Serial.print("Listening address: ");
   Serial.print(Ethernet.localIP());
   
   // for hardware debug: LED indication of server state: blinking = waiting for connection
   int IPnr = getIPComputerNumber(Ethernet.localIP());   // Get computernumber in local network 192.168.1.3 -> 3)
   Serial.print(" ["); 
   Serial.print(IPnr); 
   Serial.print("] "); 
   Serial.print("  [Testcase: telnet "); 
   Serial.print(Ethernet.localIP()); 
   Serial.print(" "); 
   Serial.print(PORT); 
   Serial.println("]");
   
   signalNumber(ledPin, IPnr);
}

void loop()
{
   // Listen for incomming connection (app)
   EthernetClient ethernetClient = server.available();
   if (!ethernetClient) {
      blink(ledPin);
      return; // wait for connection and blink LED
   }

   Serial.println("Application connected");
   digitalWrite(ledPin, LOW);

   // Do what needs to be done while the socket is connected.
   while (ethernetClient.connected()) 
   {
      checkEvent(switchPin, pinState);          // update pin state
      sensorValue = readSensor(A0, 100);         // update sensor value
        
      // Activate pin based op pinState
      if (pinChange) {
         if (pinState) { digitalWrite(ledPin, HIGH); }
         pinChange = false;
      }
   
      // Execute when byte is received.
      while (ethernetClient.available())
      {
         char inByte = ethernetClient.read();   // Get byte from the client.
         executeCommand(inByte);                // Wait for command to execute
         inByte = NULL;                         // Reset the read byte.
      } 
   }
   Serial.println("Application disonnected");
}

// Choose and switch your Kaku device, state is true/false (HIGH/LOW)
bool toggleKakuDevice(int unit)
{
   //Some interference from other project groups seem to trigger our RF outlet lol.   
   Serial.println("woooooh i'm being spoopy!");
   
   // Set state acorrdingly.
   switch(unit)
   {
      case 1:
        p1state = !p1state;
        kakuTransmitter.sendUnit(0, p1state);
        return p1state;
      case 2:
        p2state = !p2state;
        kakuTransmitter.sendUnit(1, p2state);
        return p2state;
      case 3: 
        p3state = !p3state;
        kakuTransmitter.sendUnit(2, p3state);
        return p3state;
      default:
        Serial.println("Unknown power outlet");
        return false;
   }

   delay(100);
}

// Implementation of (simple) protocol between app and Arduino
// Request (from app) is single char ('a', 's', 't', 'i' etc.)
// Response (to app) is 4 chars  (not all commands demand a response)
void executeCommand(char cmd)
{     
         char buf[4] = {'\0', '\0', '\0', '\0'};

         // Command protocol
         Serial.print("["); Serial.print(cmd); Serial.print("] -> ");
         
         switch (cmd) {
         case 'a': // Report sensor value to the app  
            intToCharBuf(sensorValue, buf, 4);                // convert to charbuffer
            server.write(buf, 4);                             // response is always 4 chars (\n included)
            Serial.print("Sensor: "); Serial.println(buf);
            break;
         case 's': // Report switch state to the app
            if (pinState) { server.write("ON\n"); Serial.println("Pin state is ON"); }  // always send 4 chars
            else { server.write("OFF\n"); Serial.println("Pin state is OFF"); }
            break;
         case 't': // Toggle state; If state is already ON then turn it OFF
            if (pinState) { pinState = false; Serial.println("Set pin state to \"OFF\""); }
            else { pinState = true; Serial.println("Set pin state to \"ON\""); }  
            pinChange = true; 
            break;
         case 'i':    
            digitalWrite(infoPin, HIGH);
            break;
         case '1':
            Serial.println("Set outlet to 1");
            currentPowerOutlet = 1;
            intToCharBuf(p1state, buf, 4);
            server.write(buf, 4);
            break;
         case '2':
            Serial.println("Set outlet to 2");
            currentPowerOutlet = 2;
            intToCharBuf(p2state, buf, 4);
            server.write(buf, 4);
            break;
         case '3':
            Serial.println("Set outlet to 3");
            currentPowerOutlet = 3;
            intToCharBuf(p3state, buf, 4);
            server.write(buf, 4);
            break;
            
          case 'T':
            if(currentPowerOutlet == 0)
            {
              break;
            }
            else
            {
              bool state = toggleKakuDevice(currentPowerOutlet);
              intToCharBuf(state, buf, 4);
              server.write(buf, 4);
              break;
            }
            
         default:
            digitalWrite(infoPin, LOW);
         }
}

// read value from pin pn, return value is mapped between 0 and mx-1
int readSensor(int pn, int mx)
{
  return map(analogRead(pn), 0, 1023, 0, mx-1);    
}

// Convert int <val> char buffer with length <len>
void intToCharBuf(int val, char buf[], int len)
{
   String s;
   s = String(val);                        // convert tot string
   if (s.length() == 1) s = "0" + s;       // prefix redundant "0" 
   if (s.length() == 2) s = "0" + s;  
   s = s + "\n";                           // add newline
   s.toCharArray(buf, len);                // convert string to char-buffer
}

// Check switch level and determine if an event has happend
// event: low -> high or high -> low
void checkEvent(int p, bool &state)
{
   static bool swLevel = false;       // Variable to store the switch level (Low or High)
   static bool prevswLevel = false;   // Variable to store the previous switch level

   swLevel = digitalRead(p);
   if (swLevel)
      if (prevswLevel) delay(1);
      else {               
         prevswLevel = true;   // Low -> High transition
         state = true;
         pinChange = true;
      } 
   else // swLevel == Low
      if (!prevswLevel) delay(1);
      else {
         prevswLevel = false;  // High -> Low transition
         state = false;
         pinChange = true;
      }
}

// blink led on pin <pn>
void blink(int pn)
{
  digitalWrite(pn, HIGH); 
  delay(100); 
  digitalWrite(pn, LOW); 
  delay(100);
}

// Visual feedback on pin, based on IP number, used for debug only
// Blink ledpin for a short burst, then blink N times, where N is (related to) IP-number
void signalNumber(int pin, int n)
{
   int i;
   for (i = 0; i < 30; i++)
       { digitalWrite(pin, HIGH); delay(20); digitalWrite(pin, LOW); delay(20); }
   delay(1000);
   for (i = 0; i < n; i++)
       { digitalWrite(pin, HIGH); delay(300); digitalWrite(pin, LOW); delay(300); }
    delay(1000);
}

// Convert IPAddress tot String (e.g. "192.168.1.105")
String IPAddressToString(IPAddress address)
{
    return String(address[0]) + "." + 
           String(address[1]) + "." + 
           String(address[2]) + "." + 
           String(address[3]);
}

// Returns B-class network-id: 192.168.1.3 -> 1)
int getIPClassB(IPAddress address)
{
    return address[2];
}

// Returns computernumber in local network: 192.168.1.3 -> 3)
int getIPComputerNumber(IPAddress address)
{
    return address[3];
}

// Returns computernumber in local network: 192.168.1.105 -> 5)
int getIPComputerNumberOffset(IPAddress address, int offset)
{
    return getIPComputerNumber(address) - offset;
}

