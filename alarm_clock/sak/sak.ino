#include <G11Speaker.h>

// Pin configuration for the alarm clock.
#define ENABLE_DEBUG true
#define PIN_DISPLAY_DATA    1
#define PIN_DISPLAY_CLK     2
#define PIN_DISPLAY_CS      3
#define PIN_RF_TRANSMITTER  4
#define PIN_SNOOZE_BUTTON   5
#define PIN_DHT11_SENSOR    6
#define PIN_SPEAKER         9
#define PIN_LDR             A0
#define PIN_CLOCK_SDA       A4
#define PIN_CLOCK_SCL       A5

G11Speaker speaker(PIN_SPEAKER, ENABLE_DEBUG);

void setup() 
{
  Serial.begin(9600);
}

void loop() 
{  
  enable_alarm();
}

void enable_alarm()
{
  for(int i = 0; i <  3; i++)
  {
    speaker.play(4000, 33);
    speaker.stop(33);
  }

  speaker.stop(500);
}


