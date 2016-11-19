using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Globalization;
using System.Linq;
using System.Text;

using Android.App;
using Android.Content;
using Android.Content.PM;
using Android.Graphics;
using Android.OS;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using SOA_Android.Services;
using SOA_Android.Support_Classes;

namespace SOA_Android.Activities
{
    [Activity(ScreenOrientation = ScreenOrientation.Portrait)]
    public class WriteActivity : Activity
    {
        private readonly ColorConfiguration colorConfig = ColorConversion.GetColorSetupFromXML();
        private string sensorType = "";

        private TextView txtSensorTitle, txtSensorValue, txtTitle;
        private Button btnSensorValue;
        private Intent currentSensorService = null;

        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);

            RequestWindowFeature(WindowFeatures.NoTitle);
            SetContentView(Resource.Layout.Write);

            var dweetResponse = Http.Post(Constants.DweetPostColorsHttp, new NameValueCollection
            {
                { "type", "colors" },
                { "value", ColorConversion.GetXMLString() }
            });

            sensorType = Intent.GetStringExtra("SensorType") ?? "?????";

            txtSensorTitle = (TextView) FindViewById(Resource.Id.txtSensorTitle);
            txtSensorTitle.Text = "Sensando " + sensorType;

            txtSensorValue = (TextView)FindViewById(Resource.Id.txtSensorValue);
            txtSensorValue.Text = "";

            btnSensorValue = (Button)FindViewById(Resource.Id.btnSensorValue);
            btnSensorValue.SetBackgroundColor(Color.Transparent);

            var filter = new IntentFilter("AndroidReading");
            RegisterReceiver(new MyBroadcastReceiver(), filter);

            switch (sensorType)
            {
                case "Proximidad":
                    currentSensorService = new Intent(this, typeof(ProximitySensorService));
                    btnSensorValue.LayoutParameters = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MatchParent, LinearLayout.LayoutParams.MatchParent);
                    btnSensorValue.Visibility = ViewStates.Visible;
                    break;
                case "GPS":
                    currentSensorService = new Intent(this, typeof(GPSService));
                    txtSensorValue.LayoutParameters = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MatchParent, LinearLayout.LayoutParams.MatchParent);
                    btnSensorValue.Visibility = ViewStates.Gone;
                    break;
                case "Hora":
                    currentSensorService = new Intent(this, typeof(HourService));
                    txtTitle = (TextView)FindViewById(Resource.Id.txtTitle);
                    txtTitle.Text = "Hora actual";
                    btnSensorValue.LayoutParameters = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MatchParent, LinearLayout.LayoutParams.MatchParent);
                    txtSensorTitle.Visibility = ViewStates.Gone;
                    btnSensorValue.Visibility = ViewStates.Visible;
                    break;
            }

            if(currentSensorService != null)
                StartService(currentSensorService);
        }

        protected override void OnPause()
        {
            if (currentSensorService != null)
                StopService(currentSensorService);
            var dweetResponse = Http.Post(Constants.DweetPostHttp, new NameValueCollection
            {
                { "type", "HT" }
            });
            base.OnPause();
        }

        protected override void OnResume()
        {
            if (currentSensorService != null)
                StartService(currentSensorService);
            base.OnResume();
        }

        public override void OnBackPressed()
        {
            if (currentSensorService != null)
                StopService(currentSensorService);
            var dweetResponse = Http.Post(Constants.DweetPostHttp, new NameValueCollection
            {
                { "type", "HT" }
            });
            base.OnBackPressed();
        }

        protected override void OnDestroy()
        {
            if (currentSensorService != null)
                StopService(currentSensorService);
            var dweetResponse = Http.Post(Constants.DweetPostHttp, new NameValueCollection
            {
                { "type", "HT" }
            });
            base.OnDestroy();
        }
        [IntentFilter(new[] { "AndroidReading" })]
        class MyBroadcastReceiver : BroadcastReceiver
        {
            public override void OnReceive(Context context, Intent intent)
            {
                if (!intent.Action.Equals("AndroidReading")) return;

                var reading = intent.GetStringExtra("Value") ?? "";

                var activity = ((WriteActivity)context);

                if (activity.sensorType != "Hora" && reading.Equals("NOTHING"))
                {
                    activity.StartActivity(typeof(MainMenuActivity));
                    activity.StopService(activity.currentSensorService);
                }

                Color color;

                switch (activity.sensorType)
                {
                    case "Proximidad":
                        double value = 0;
                        if (!double.TryParse(reading, out value)) return;
                        color =
                                ColorConversion.GetGradientColor(
                                    double.Parse(reading, NumberStyles.Any, CultureInfo.InvariantCulture),
                                    activity.colorConfig.ProximityColor.LowColor,
                                    activity.colorConfig.ProximityColor.HighColor, double.Parse(ColorConversion.DefaultColors.ProximityColor.HighColor.Threshold)).ToAndroidColor();
                        activity.btnSensorValue.SetBackgroundColor(color);
                        break;
                    case "GPS":
                        if (reading.IndexOf('|') == -1) return;
                        var splits = reading.Split('|');
                        if (splits.Count() < 2 || string.IsNullOrEmpty(splits[1])) return;
                        reading = $"Latitud: {splits[0].Substring(0,6)}\nLongitud: {splits[1].Substring(0, 6)}";
                        break;
                    case "Hora":
                        var secondsInDay = (DateTime.Now - DateTime.Today).TotalSeconds;
                        color =
                                ColorConversion.GetGradientColor(
                                    secondsInDay,
                                    activity.colorConfig.HourColor.LowColor,
                                    activity.colorConfig.HourColor.HighColor, double.Parse(ColorConversion.DefaultColors.HourColor.HighColor.Threshold)).ToAndroidColor();
                        activity.btnSensorValue.SetBackgroundColor(color);
                        reading = DateTime.Now.ToString("HH:mm:ss");
                        break;
                }

                activity.txtSensorValue.Text = reading;
            }
        }
    }
}