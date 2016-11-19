using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Linq;
using System.Text;
using System.Threading;
using Android.App;
using Android.Content;
using Android.Locations;
using Android.OS;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using SOA_Android.Activities;
using SOA_Android.Support_Classes;

namespace SOA_Android.Services
{
    [Service]
    public class HourService : Service
    {
        private Thread _thread;

        public override StartCommandResult OnStartCommand(Intent intent, StartCommandFlags flags, int startId)
        {
            BeginWriting();
            return StartCommandResult.Sticky;
        }

        private void BeginWriting()
        {
            var dweetResponse = Http.Post(Constants.DweetPostHttp, new NameValueCollection
            {
                { "type", "Hour" }
            });
            _thread = new Thread(() =>
            {
                while (_thread.IsAlive)
                {
                    var i = new Intent();
                    i.SetAction("AndroidReading");

                    SendBroadcast(i);

                    Thread.Sleep(1000);
                }
                return;
            });
            _thread.Start();
        }

        public override IBinder OnBind(Intent intent)
        {
            return null;
        }
    }
}