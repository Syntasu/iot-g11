//Bool to tell the system we are in debug mode, so we want to print all the Serial.print(ln) messages.
#define ENABLE_DEBUG false

// Pin configuration for the alarm clock.
#define PIN_DISPLAY_DATA    3
#define PIN_DISPLAY_CLK     2
#define PIN_DISPLAY_CS      1
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

#define SNOOZE_TIME 30

bool alarm_playing = false;

void setup()
{
  //Setup serial communication.
  if (ENABLE_DEBUG)
  {
    Serial.begin(9600);
    logln("Running in debug mode...");
  }

  //Setup each of the modules.
  net_setup();
  time_setup();
  alarm_setup();
  kaku_setup();
  display_setup();
}

void display_setup()
{
  m_display.init (PIN_DISPLAY_CS, PIN_DISPLAY_CLK, PIN_DISPLAY_DATA); 
}

//Make a connection to the interwebs.
void net_setup()
{
  IPAddress ip = IPAddress(192, 168, 1, 17);
  byte mac[] = { 0x84, 0xAD, 0xE2, 0xA9, 0xEB, 0x9A};

  m_net.begin(ip, mac);

  //Setting up the binds for the commands.
  m_net.bind("alarm_add", cmd_alarm_add);
  m_net.bind("alarm_edit", cmd_alarm_edit);
  m_net.bind("alarm_remove", cmd_alarm_remove);
  m_net.bind("alarm_snooze", cmd_alarm_snooze);
  m_net.bind("alarm_stop", cmd_alarm_stop);
  m_net.bind("time_sync", cmd_time_sync);
  
  log("The SAK is connected on: ");
  logln(String(Ethernet.localIP()));
}

void time_setup()
{
  //Set the starting time by a given date_time struct.
  //Format is as following: yyyy/mm/dd hh:mm:ss
  date_time start_time = date_time(2018, 1, 1, 12, 0, 0);
  m_time.initialize(start_time);
}

void alarm_setup()
{
  date_time alarm_time = date_time(2018, 1, 1, 12, 0, 10);
  alarm a = alarm(0, alarm_time);
  m_alarm.add_alarm(a);
}

void kaku_setup()
{
  //Ready unit 0 and 1 kakus.
  m_kaku.init_kaku(0);
  m_kaku.init_kaku(1);
}

int loopCount = 0;
void loop()
{
  //Get how much time this frame took (incl. delay).
  //True means we want to reset the counter internally.
  int delta = m_util.get_total_delay(true);

  alarm_update();
  speaker_update(delta);
  net_update();
  display_update();
  
  if(loopCount > 50 || alarm_playing)
  {
    ultrasone_update();
    loopCount = 0;
  }
  
  time_update(delta);
  loopCount++;
}

void display_update()
{
  date_time t = m_time.get_time();
  m_display.update(t.hours, t.minutes);
}

void time_update(int timeDelay)
{
  //Delay for 3 milliseconds, keep track of the time we delayed.
  //The total time delayed will be used to increment the simulated time.
  m_util.timed_delay(1);
  
  //Grab the current time and return it as date_time.
  date_time t = m_time.get_time();


#if 0
  log("The current time is ");
  log(m_time.get_time_string());
  log(" the delay is ");
  logln(String(timeDelay * (4.275f / timeDelay)));
#endif

  //Simulate the time based on the delay of one frame/iteration.
  m_time.simulate(25);
}

void alarm_update()
{
  //Grab the current time.
  date_time t = m_time.get_time();

  //Check if any alarm needs to me sounded. (after the current time exceeds the alarm time).
  bool alarm_state = m_alarm.update(t);
  
  int alarms = m_alarm.get_alarm_count();
  
  if(alarm_state && !alarm_playing)
  {
    m_speaker.play(0, true);
    //m_kaku.set_kaku(0, true);
    //m_kaku.set_kaku(1, true);
    logln("ALARM");
    alarm_playing = true;
  }
  else if(!alarm_state && alarm_playing)
  {
    m_speaker.stop();
    //m_kaku.set_kaku(0, false);
    //m_kaku.set_kaku(1, false);
    logln("STOP ALARM");
    alarm_playing = false;
  }
}

void net_update()
{
  //Receive any incoming message
  m_net.update(ENABLE_DEBUG);
}

void speaker_update(int timeDelay)
{
  //Update the speaker, check how much time the speaker has consumed.
  int consumed_time = m_speaker.update(timeDelay);
  m_util.virtual_delay(consumed_time + 1);
}

void ultrasone_update()
{
  int state = m_ultrasone.get_state();
  logln(String(state));
  
  //Beep once to notify the user.
  if(state == 0)
  {
    tone(PIN_SPEAKER, 2500);
    delay(200);
  }

  //Beep twice
  if(state == 1)
  {
    tone(PIN_SPEAKER, 4000);
    delay(200);
  }

  //We need to snooze!
  if(state == 2)
  {
    m_alarm.snooze(SNOOZE_TIME);
  }

  if(state == 3)
  {
    m_alarm.stop();
  }
}

//Command handler for adding a new alarm.
void cmd_alarm_add(String command, String a0, String a1, String a2)
{
  int id = a0.toInt();
  date_time alarm_time = m_util.str_to_datetime(a1);

  logln(a1);
  
  alarm a = alarm(id, alarm_time);
  bool result = m_alarm.add_alarm(a);

  if(!result)
  {
    logln("Failed to add alarm.");
  }

  log("Alarm count is now ");
  logln(String(m_alarm.get_alarm_count()));
}

//Command handler for editing alarms.
void cmd_alarm_edit(String command, String a0, String a1, String a2)
{
  int id = a0.toInt();
  date_time alarm_time = m_util.str_to_datetime(a1);
  alarm a = alarm(id, alarm_time);

  bool result = m_alarm.edit_alarm(a);

  if(!result)
  {
    logln("Failed to edit alarm");
  }

  log("Alarm count is now ");
  logln(String(m_alarm.get_alarm_count()));
}

void cmd_alarm_remove(String command, String a0, String a1, String a2)
{
  int id = a0.toInt();
  m_alarm.remove_alarm(id);

  log("Alarm count is now ");
  logln(String(m_alarm.get_alarm_count()));
}

void cmd_alarm_snooze(String command, String a0, String a1, String a2)
{
  if(ENABLE_DEBUG)
  {
    m_alarm.snooze(10);
  }
  else
  {
    m_alarm.snooze(SNOOZE_TIME);
  }
}

void cmd_alarm_stop(String command, String a0, String a1, String a2)
{
  m_alarm.stop();
}

void cmd_time_sync(String command, String a0, String a1, String a2)
{
  date_time t = m_util.str_to_datetime(a0);
  m_time.set_datetime(t);
}

void logln(String message)
{
  if(ENABLE_DEBUG)
  {
    Serial.println(message);
  }
}

void log(String message)
{
  if(ENABLE_DEBUG)
  {
    Serial.print(message);
  }
}


