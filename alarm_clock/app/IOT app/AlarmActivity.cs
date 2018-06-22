using Android.App;
using Android.Content;
using Android.OS;
using Android.Widget;
using IOT_app.Code;
using IOT_app.Code.IO;
using System;
using System.Collections.Generic;
using System.Windows;

namespace IOT_app
{
    [Activity(Label = "AlarmActivity")]
    public class AlarmActivity : Activity
    {
        private Button btnAlarmCreate;

        private ListView lv_alarms;
        private List<Alarm> alarms = new List<Alarm>
        {
            new Alarm("Hel", DateTime.Now),
            new Alarm("World Peace", DateTime.Now),
            new Alarm("Google calendar", DateTime.Now),
        };

        protected override async void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            base.SetContentView(Resource.Layout.Alarm);

            //await IOWorker.WriteAlarmFile(alarms);
            List<Alarm> a = await IOWorker.ReadAlarmFile();
            alarms = a;

            //IOWorker.SaveAlarms(alarms);


            btnAlarmCreate = FindViewById<Button>(Resource.Id.btn_alarm_create);
            btnAlarmCreate.Click += (o, s) => GotoAddEditAlarmsActivity(null);

            lv_alarms = FindViewById<ListView>(Resource.Id.lv_alarms);

            AlarmAdapter alarmAdapter = new AlarmAdapter(this, alarms);
            lv_alarms.Adapter = alarmAdapter;
        }

        private void GotoAddEditAlarmsActivity(Alarm alarm)
        {
            if(alarm != null)
            {
                Intent intent = new Intent(this, typeof(AddEditAlarmActivity));
                intent.PutExtra("alarm", alarm.Serialize());
                StartActivity(intent);
            }
            else
            {
                StartActivity(typeof(AddEditAlarmActivity));
            }
        }
    }
}