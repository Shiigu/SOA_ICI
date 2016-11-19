using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace SOA_ArduinoMock
{
    public static class Http
    {
        public static byte[] Post(string uri, NameValueCollection pairs)
        {
            using (var client = new WebClient())
            {
                return client.UploadValues(uri, pairs);
            }
        }

        public static AndroidSensorReading Get(string uri)
        {
            using (var client = new WebClient())
            {
                var json = client.DownloadString(uri);
                var deserializedData = (LatestDweet) JsonConvert.DeserializeObject(json, typeof(LatestDweet));
                if (deserializedData.With == null) return null;
                return new AndroidSensorReading
                {
                    SendingDateTime = DateTime.Parse(deserializedData.With.First().Created),
                    Type = deserializedData.With.First().Content.Type,
                    Value = deserializedData.With.First().Content.Value
                };
            }
        }
    }
}
