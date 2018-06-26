using Newtonsoft.Json;
using System.Net;

namespace IOT_app.Code.IO.Data
{
    public struct ConnectionData
    {
        public string IP;
        public int Port;

        public ConnectionData(string ip, int port)
        {
            IP = ip;
            Port = port;
        }

        public ConnectionData(string json)
        {
            ConnectionData data = JsonConvert.DeserializeObject<ConnectionData>(json);
            IP = data.IP;
            Port = data.Port;
        }

        public string Serialize()
        {
            return JsonConvert.SerializeObject(this);
        }

    }
}