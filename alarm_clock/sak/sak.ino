//Bool to tell the system we are in debug mode, so we want to print all the Serial.print(ln) messages.
#define ENABLE_DEBUG false

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

//Include pgmspace, so we can write to the program memory
#include <avr/pgmspace.h>

//Include our own headers.
#include <G11Util.h>
#include <G11Time.h>
#include <G11Speaker.h>
#include <G11Alarm.h>

//Create an instance of all our classes (defined in the header files).
G11Util m_util;
G11Speaker m_speaker(PIN_SPEAKER);
G11Time m_time;
G11Alarm m_alarm;

void setup()
{
  if(ENABLE_DEBUG)
  {
    Serial.begin(9600);
  }

  time_setup();
  alarm_setup();

  m_speaker.play(0, true);
}

void time_setup()
{  
  //TODO: Fetch time from the correct source (RTC? NET? APP?)
  //Set the starting time by a given date_time struct.
  //Format is as following: yyyy/mm/dd hh:mm:ss
  date_time start_time = date_time(2018, 1, 1, 12, 0, 0);
  m_time.initialize(start_time);
}

void alarm_setup()
{
  date_time alarm_time = date_time(2018, 1, 1, 12, 0, 15); 
  m_alarm.schedule_alarm(alarm_time); 
}

int c = 0;
void loop()
{
  //Get how much time this frame took (incl. delay).
  //True means we want to reset the counter internally.
  int delta = m_util.get_total_delay(true);
  
  time_update(delta);
  alarm_update();
  speaker_update(delta);

  c++;

  if(c > 100)
  {
    m_speaker.stop();
  }
}

void time_update(int timeDelay)
{
  //Delay for 1 milliseconds, keep track of the time we delayed.
  //The total time delayed will be used to increment the simulated time.
  m_util.timed_delay(1);

  //Grab the current time and return it as date_time.
  date_time t = m_time.get_time();

  //TEST, REMOVE: Print the current time.
  if(ENABLE_DEBUG)
  {
    Serial.print("This frame took ");
    Serial.print(timeDelay);
    Serial.print(", the current time is ");
    Serial.println(m_time.get_time_string());
  }

  //Simulate the time based on the delay of one frame/iteration.
  //TODO: Replace this with RTC or time via internet.
  m_time.simulate(timeDelay);
}

void alarm_update()
{
  //Grab the current time.
  date_time t = m_time.get_time();
  
  //Check if any alarm needs to me sounded. (after the current time exceeds the alarm time).
  //TODO: More information about the alarm that is playing. Things like last time it rang, is it snoozed, which tune etc...
  if(m_alarm.check_alarms(t))
  {
    //Play the speaker with pattern 0.
    m_speaker.play(0, true);
  }
}

void speaker_update(int timeDelay)
{
  int consumed_time = m_speaker.update(timeDelay);
  m_util.virtual_delay(consumed_time + 1);
}



