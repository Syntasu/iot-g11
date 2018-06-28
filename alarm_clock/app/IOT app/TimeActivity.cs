using Android.App;
using Android.OS;
using Android.Widget;
using System;
using IOT_app.Code;

namespace IOT_app
{
    [Activity(Label = "TimeActivity")]
    public class TimeActivity : Activity
    {
        private Button btnCancel;
        private Button btnSetTime;
        private DatePicker dpDate;
        private NumberPicker npTimeHour;
        private NumberPicker npTimeMinute;

        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            base.SetContentView(Resource.Layout.Time);

            //Get the UI elements from the view.
            btnCancel = FindViewById<Button>(Resource.Id.btn_time_cancel);
            btnSetTime = FindViewById<Button>(Resource.Id.btn_set_time);
            dpDate = FindViewById<DatePicker>(Resource.Id.dt_time_picker);
            npTimeHour = FindViewById<NumberPicker>(Resource.Id.np_time_hour);
            npTimeMinute = FindViewById<NumberPicker>(Resource.Id.np_time_minute);

            //Set the min and max values for the numberpicker..
            npTimeHour.MinValue = 0;
            npTimeHour.MaxValue = 23;
            npTimeMinute.MinValue = 0;
            npTimeMinute.MaxValue = 59;

            //Get the time and date from the arduino.
            DateTime time = DateTime.Now;
            dpDate.DateTime = time;
            npTimeHour.Value = time.Hour;
            npTimeMinute.Value = time.Minute;

       
            btnSetTime.Click += (s, e) => SyncDateTime();
            btnCancel.Click += (s, e) => StartActivity(typeof(MainActivity));
        }

        /// <summary>
        ///     Tell the arduino to sync with the time we've set from the app.
        /// </summary>
        private void SyncDateTime()
        {
            DateTime time = new DateTime(
                dpDate.DateTime.Year, dpDate.DateTime.Month, dpDate.DateTime.Day,
                npTimeHour.Value, npTimeMinute.Value, 0
            );

            //Sync time with arduino.
            SocketWorker.Send(Commands.SyncTime, time.ToAgnosticString());
        }
    }
}