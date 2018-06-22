using Android.App;
using Android.Content;
using Android.OS;
using Android.Widget;
using IOT_app.Code;
using System;
using System.Collections.Generic;

namespace IOT_app
{
    [Activity(Label = "AlarmActivity")]
    public class AlarmActivity : Activity
    {
        private Button btnAlarmCreate;

        private ListView lv_alarms;
        private List<Alarm> alarms = new List<Alarm>
        {
            new Alarm("Hello world", DateTime.Now),
            new Alarm("World Peace", DateTime.Now),
            new Alarm("Google calendar", DateTime.Now),
        };

        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            base.SetContentView(Resource.Layout.Alarm);

            btnAlarmCreate = FindViewById<Button>(Resource.Id.btn_alarm_create);
            btnAlarmCreate.Click += (o, s) => GotoAddEditAlarmsActivity(null);

            lv_alarms = FindViewById<ListView>(Resource.Id.lv_alarms);

            AlarmAdapter alarmAdapter = new AlarmAdapter(this, alarms);
            lv_alarms.Adapter = alarmAdapter;
        }

        private void GotoAddEditAlarmsActivity(Alarm alarm)
        {
            if(alarm == null)
            {
                //StartActivity();
            }
            else
            {

            }
        }
    }
}