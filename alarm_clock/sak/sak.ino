#define ENABLE_DEBUG true;

// Pin configuration for the alarm clock.
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

#include <avr/pgmspace.h>
#include <G11Util.h>
#include <G11Time.h>
#include <G11Speaker.h>
#include <G11Alarm.h>

G11Util m_util;
G11Speaker m_speaker(PIN_SPEAKER);
G11Time m_time;
G11Alarm m_alarm;

void setup()
{
  Serial.begin(9600);
  delay(1000);
  
  //TODO: Fetch time from the correct source (RTC? NET? APP?)
  m_time.initialize(date_time(2018, 1, 1, 12, 0, 0));
  
  m_alarm.schedule_alarm(date_time(2018, 1, 1, 12, 0, 15)); 
  m_alarm.schedule_alarm(date_time(2018, 1, 1, 12, 0, 30)); 
  m_alarm.schedule_alarm(date_time(2018, 1, 1, 12, 0, 45));
  m_alarm.schedule_alarm(date_time(2018, 1, 1, 12, 1, 0));
  m_alarm.schedule_alarm(date_time(2018, 1, 1, 12, 2, 0));
}

void loop()
{
  m_util.timed_delay(500);

  date_time t = m_time.get_time();
  int timeDelay = m_util.get_total_delay(true);

  if(m_alarm.check_alarms(t))
  {
    //TODO: Stop playing when the alarm is snoozed or finished.
    m_speaker.play_pattern();
    m_util.virtual_delay(13152);
  }

  Serial.print("This frame took ");
  Serial.print(timeDelay);
  Serial.print(", the current time is ");
  Serial.println(m_time.get_time_string());
  
  m_time.simulate(timeDelay);
}

void enable_alarm()
{
  for (int i = 0; i <  3; i++)
  {
    m_speaker.play(4000, 33);
    m_speaker.stop(33);
  }

  m_speaker.stop(50);
  m_util.virtual_delay(116);
}


