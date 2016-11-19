using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SOA_ArduinoMock
{
    public class Content
    {
        public string Type { get; set; }
        public string Value { get; set; }
    }

    public class With
    {
        public string Thing { get; set; }
        public string Created { get; set; }
        public Content Content { get; set; }
    }

    public class LatestDweet
    {
        public string This { get; set; }
        public string By { get; set; }
        public string The { get; set; }
        public List<With> With { get; set; }
    }

}
