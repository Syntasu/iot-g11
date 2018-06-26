using Android.App;
using Android.Content;
using Android.OS;
using Android.Widget;
using IOT_app.Code;
using IOT_app.Code.IO;
using System.Collections.Generic;

namespace IOT_app
{
    [Activity(Label = "AlarmActivity")]
    public class AlarmActivity : Activity
    {
        private Button btnAlarmCreate;
        private Button btnAlarmCancel;

        private ListView lv_alarms;
        private List<Alarm> alarms = new List<Alarm>();

        protected override async void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            base.SetContentView(Resource.Layout.Alarm);

            //Get elements from view.
            lv_alarms = FindViewById<ListView>(Resource.Id.lv_alarms);
            btnAlarmCreate = FindViewById<Button>(Resource.Id.btn_alarm_create);
            btnAlarmCancel = FindViewById<Button>(Resource.Id.btnAlarmCancel);
            
            //Grab the alarms file from disk and add it to the list we want to display.
            List<Alarm> a = await IOWorker.ReadFile<List<Alarm>>(AppFiles.Alarm);

            if (a != null)
            {
                alarms = a;
            }

            //Add event listeners for various events.
            btnAlarmCreate.Click += (o, s) => GotoAddEditAlarmsActivity(null);  //Goto add alarm.
            btnAlarmCancel.Click += (o, s) => StartActivity(typeof(MainActivity));   //Go back to the main activity.
            lv_alarms.ItemClick += OnClickAlarm;                                //Edit an alarm.


            //Set the adapter for the alarms list.
            AlarmAdapter alarmAdapter = new AlarmAdapter(this, alarms);
            lv_alarms.Adapter = alarmAdapter;
        }

        /// <summary>
        ///     When we click an existing alarm in the listview, we want to edit it.
        /// </summary>
        private void OnClickAlarm(object sender, AdapterView.ItemClickEventArgs e)
        {
            //Sanity check to prevent accesing out of bounds (should theoretically not be possible if the list view is maintained correctly).
            if(e.Position < 0 || e.Position >= alarms.Count)
            {
                Toast.MakeText(this, $"Index {e.Position} is out of bounds...", ToastLength.Long);
                return;
            }

            //Get the alarm, start the edit activity.
            Alarm alarm = alarms[e.Position];
            GotoAddEditAlarmsActivity(alarm);
        }

        /// <summary>
        ///     Add or edit an alarm.
        /// </summary>
        /// <param name="alarm">The alarm we want to edit, alarm is null when a new alarm is added.</param>
        private async void GotoAddEditAlarmsActivity(Alarm alarm)
        {
            //Save the current alarms.
            await IOWorker.SaveFile(AppFiles.Alarm, AppFileExtension.JSON, alarms);

            //If the given alarm is not null, we are editing an existing one.
            //There for we pass the alarm in question via the Intent.
            if(alarm != null)
            {
                Intent intent = new Intent(this, typeof(AddEditAlarmActivity));
                intent.PutExtra("alarm", alarm.Serialize());
                StartActivity(intent);
            }
            //If we do not have given an alarm, we are creating a new one.
            //Thus we do not pass in any json that could be deserialized.
            //Code in the other activity should handle this.
            else
            {
                Intent intent = new Intent(this, typeof(AddEditAlarmActivity));
                intent.PutExtra("alarm", "");
                StartActivity(intent);
            }
        }
    }
}