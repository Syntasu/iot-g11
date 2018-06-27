//Bool to tell the system we are in debug mode, so we want to print all the Serial.print(ln) messages.
#define ENABLE_DEBUG true

// Pin configuration for the alarm clock.
#define PIN_DISPLAY_DATA    2
#define PIN_DISPLAY_CLK     3
#define PIN_DISPLAY_CS      4
#define PIN_RF_TRANSMITTER  5
#define PIN_SNOOZE_BUTTON   6
#define PIN_DHT11_SENSOR    7
#define PIN_SPEAKER         9
#define PIN_LDR             A0
#define PIN_CLOCK_SDA       A4
#define PIN_CLOCK_SCL       A5

//Include pgmspace, spi and ethernet, so we can write to the program memory
#include <avr/pgmspace.h>
#include <SPI.h>
#include <Ethernet.h>

//Include our own headers.
#include <G11Util.h>
#include <G11Socket.h>
#include <G11Kaku.h>
#include <G11Time.h>
#include <G11Speaker.h>
#include <G11Alarm.h>
#include <G11Sensors.h>
#include <G11Ultrasone.h>
#include <G11Display.h>

//Create an instance of all our classes (defined in the header files).
G11Util m_util;
G11Socket m_net;
G11Kaku m_kaku(28620750, PIN_RF_TRANSMITTER, 260, 3);
G11Speaker m_speaker(PIN_SPEAKER);
G11Time m_time;
G11Alarm m_alarm;
G11Ultrasone m_ultrasone;
G11Sensors m_sensors;
G11Display m_display;

bool alarm_playing = false;

void setup()
{
  //Setup serial communication.
  if (ENABLE_DEBUG)
  {
    Serial.begin(9600);
  }

  //Setup each of the modules.
  net_setup();
  time_setup();
  //alarm_setup();
  //kaku_setup();
  display_setup();
}

void display_setup()
{
  m_display.init (PIN_DISPLAY_CS,PIN_DISPLAY_CLK ,PIN_DISPLAY_DATA); 
}
//Make a connection to the interwebs.
void net_setup()
{
  IPAddress ip = IPAddress(192, 168, 1, 15);
  byte mac[] = { 0x84, 0xAD, 0xE2, 0xA9, 0xEB, 0x9A};

  m_net.begin(ip, mac);

  //Setting up the binds for the commands.
  m_net.bind("alarm_add", cmd_alarm_add);
  //m_net.bind("alarm_add", cmd_alarm_add);
  //m_net.bind("alarm_add", cmd_alarm_add);
  
  if (ENABLE_DEBUG)
  {
    Serial.print("The SAK is connected on: ");
    Serial.println(Ethernet.localIP());
  }
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
  date_time alarm_time = date_time(2018, 1, 1, 12, 0, 10);
  m_alarm.schedule_alarm(alarm_time);
}

void kaku_setup()
{
  //Ready unit 0 and 1 kakus.
  m_kaku.init_kaku(0);
  m_kaku.init_kaku(1);
}

void loop()
{
  //Get how much time this frame took (incl. delay).
  //True means we want to reset the counter internally.
  int delta = m_util.get_total_delay(true);

  time_update(delta);
  //alarm_update();
  //speaker_update(delta);
  net_update();
  display_update();
}

void display_update()
{
  date_time time = m_time.get_time();
  int hours = (int)time.hours;
  int minutes = (int)time.minutes;
  m_display.update(hours, minutes);
}

void time_update(int timeDelay)
{
  //Delay for 1 milliseconds, keep track of the time we delayed.
  //The total time delayed will be used to increment the simulated time.
  m_util.timed_delay(1);

  //Grab the current time and return it as date_time.
  date_time t = m_time.get_time();

  //TEST, REMOVE: Print the current time.
  if (ENABLE_DEBUG)
  {
    Serial.print("This frame took ");
    Serial.print(timeDelay);
    Serial.print(", the current time is ");
    Serial.println(m_time.get_time_string());
  }

  //Simulate the time based on the delay of one frame/iteration.
  m_time.sync_with_rtc();
}

void alarm_update()
{
  //Grab the current time.
  date_time t = m_time.get_time();

  //Check if any alarm needs to me sounded. (after the current time exceeds the alarm time).
  int alarm_state = m_alarm.check_alarms(t);
  
  if(alarm_state == 1)
  {
    if(!alarm_playing)
    {
      m_speaker.play(0, true);
      m_kaku.set_kaku(0, true);
      m_kaku.set_kaku(1, true);
      alarm_playing = true;
    }
  }

  if(alarm_state == 2 || alarm_state == 3)
  {
    if(alarm_playing)
    {
      m_speaker.stop();
      m_kaku.set_kaku(0, false);
      m_kaku.set_kaku(1, false);
      alarm_playing = false;
    }
  }
}

void net_update()
{
  //Receive any incoming message
  m_net.update();
}

void speaker_update(int timeDelay)
{
  //Update the speaker, check how much time the speaker has consumed.
  int consumed_time = m_speaker.update(timeDelay);
  m_util.virtual_delay(consumed_time + 1);
}

void cmd_alarm_add(String command, String a0, String a1, String a2)
{
  Serial.println(command);
  Serial.println(a0);
  Serial.println(a1);
  Serial.println(a2);
}

void cmd_alarm_snooze(String command, String a0, String a1, String a2)
{
  Serial.println("Snooze!");
  //Snooze for 10 seconds.
  m_alarm.snooze(10);
}

void cmd_alarm_stop(String command, String a0, String a1, String a2)
{
  Serial.println("Stop!");
  m_alarm.kill(m_time.get_time());
}

void ultrasone_update()
{
   int state = m_ultrasone.alarm_off();
 
   if(state == 1)
   {
      m_alarm.snooze(10); //10 voor testing anders 600 
      Serial.println("Snooze!");
   }
   else if(state == 2)
   {
      m_alarm.kill(m_time.get_time());
      Serial.println("Stop!");
   }  
}

