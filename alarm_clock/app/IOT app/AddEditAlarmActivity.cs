using Android.App;
using Android.OS;
using Android.Widget;
using IOT_app.Code;
using IOT_app.Code.IO;
using Newtonsoft.Json;
using System.Collections.Generic;

namespace IOT_app
{
    [Activity(Label = "AddEditAlarmActivity")]
    public class AddEditAlarmActivity : Activity
    {
        private Button btnSetButton;

        private List<Alarm> alarms = new List<Alarm>();
        private Alarm alarmInQuestion = null;

        protected async override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            base.SetContentView(Resource.Layout.AddEditAlarm);

            //Load the current alarms from disk.
            alarms = await IOWorker.ReadAlarmFile();

            string serializedAlarm = Intent.GetStringExtra("alarm");

            if (string.IsNullOrEmpty(serializedAlarm))
            {
                alarmInQuestion = JsonConvert.DeserializeObject<Alarm>(serializedAlarm);
            }

            btnSetButton = FindViewById<Button>(Resource.Id.btn_alarm_set);
            btnSetButton.Click += (o, s) => StoreAlarm();
        }

        private void StoreAlarm()
        {
           
        }
    }
}