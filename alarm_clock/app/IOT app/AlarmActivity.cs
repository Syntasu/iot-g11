using Android.App;
using Android.Content;
using Android.OS;
using Android.Widget;
using IOT_app.Code;
using IOT_app.Code.IO;
using System;
using System.Collections.Generic;

namespace IOT_app
{
    [Activity(Label = "AlarmActivity")]
    public class AlarmActivity : Activity
    {
        private Button btnAlarmCreate;

        private ListView lv_alarms;
        private List<Alarm> alarms = new List<Alarm>();

        protected override async void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            base.SetContentView(Resource.Layout.Alarm);

            List<Alarm> a = await IOWorker.ReadAlarmFile();
            alarms = a;

            btnAlarmCreate = FindViewById<Button>(Resource.Id.btn_alarm_create);
            btnAlarmCreate.Click += (o, s) => GotoAddEditAlarmsActivity(null);

            lv_alarms = FindViewById<ListView>(Resource.Id.lv_alarms);
            lv_alarms.ItemClick += OnClickAlarm;

            AlarmAdapter alarmAdapter = new AlarmAdapter(this, alarms);
            lv_alarms.Adapter = alarmAdapter;
        }

        private void OnClickAlarm(object sender, AdapterView.ItemClickEventArgs e)
        {
            if(e.Position < 0 || e.Position >= alarms.Count)
            {
                Toast.MakeText(this, $"Index {e.Position} is out of bounds...", ToastLength.Long);
                return;
            }

            Alarm alarm = alarms[e.Position];
            GotoAddEditAlarmsActivity(alarm);
        }

        private async void GotoAddEditAlarmsActivity(Alarm alarm)
        {
            //Save the current alarms.
            await IOWorker.SaveAlarmFile(alarms);

            //If the given alarm is not null, we are editing an existing one.
            //There for we pass the alarm in question via the Intent.
            if(alarm != null)
            {
                Intent intent = new Intent(this, typeof(AddEditAlarmActivity));
                intent.PutExtra("alarm", alarm.Serialize());
                StartActivity(intent);
            }
            //If we do not have given an alarm, we are creating a new one.
            else
            {
                Intent intent = new Intent(this, typeof(AddEditAlarmActivity));
                intent.PutExtra("alarm", "");
                StartActivity(intent);
            }
        }
    }
}