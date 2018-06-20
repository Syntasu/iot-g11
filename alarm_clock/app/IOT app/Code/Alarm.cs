using System;

namespace IOT_app.Code
{
    public class Alarm
    {
        public string Name { get; set; }
        public DateTime Time { get; set; }

        public Alarm(string Name, DateTime Time)
        {
            this.Name = Name;
            this.Time = Time;
        }

    }
}