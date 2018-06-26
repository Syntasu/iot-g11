using Newtonsoft.Json;
using System;

namespace IOT_app.Code
{
    public class Alarm
    {
        public byte Id { get; set; }
        public string Name { get; set; }
        public DateTime Time { get; set; }

        //Empty constructor, to make the serializer shut up about
        //not having a default constructor...
        public Alarm() { }

        //Construct alarm from serialized string.
        public Alarm(string serializedString)
        {
            Alarm a = JsonConvert.DeserializeObject<Alarm>(serializedString);
            Id = a.Id;
            Name = a.Name;
            Time = a.Time;
        }

        public Alarm(byte Id, string Name, DateTime Time)
        {
            this.Id = Id;
            this.Name = Name;
            this.Time = Time;
        }

        //Conver the alarm into an JSON string.
        public string Serialize()
        {
            return JsonConvert.SerializeObject(this);
        }
    }
}