using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SOA_ArduinoMock
{
    public class AndroidSensorReading
    {
        public string Type { get; set; }
        public string Value { get; set; }
        public DateTime SendingDateTime { get; set; }
    }
}
