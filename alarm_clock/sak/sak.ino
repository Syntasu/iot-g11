#include <G11Util.h>
#include <G11Time.h>
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

G11Util m_util;
G11Speaker m_speaker(PIN_SPEAKER, ENABLE_DEBUG);
G11Time m_time;

void setup() 
{
  Serial.begin(9600);
  m_time.setup(2018, 6, 14, 14, 55, 0);
}

void loop() 
{
  int timeDelay = m_util.get_total_delay(true);
  
  Serial.print("This frame took ");
  Serial.print(timeDelay);
  Serial.print(", the current time is ");
  Serial.println(m_time.get_time_string());
  
  m_time.simulate(timeDelay);
  m_util.timed_delay(1000);
}

void enable_alarm()
{
  for(int i = 0; i <  3; i++)
  {
    m_speaker.play(4000, 33);
    m_speaker.stop(33);
  }
  m_speaker.stop(500);
}


