using Android.App;
using Android.OS;
using Android.Widget;

namespace IOT_app
{
    [Activity(MainLauncher = true, Label = "MainActivity", ScreenOrientation = Android.Content.PM.ScreenOrientation.Portrait)]
    public class MainActivity : Activity
    {
        private Button buttonSnoozeAlarms;
        private Button buttonStopAlarms;

        //Buttons to goto other activities.
        private Button buttonAlarmActivity;
        private Button buttonConnectionActivity;
        private Button buttonKakuActivity;
        private Button buttonTimeActivity;

        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            base.SetContentView(Resource.Layout.main);

            //Fetch the buttons.
            buttonAlarmActivity = FindViewById<Button>(Resource.Id.btn_alarm_management);
            buttonConnectionActivity = FindViewById<Button>(Resource.Id.btn_connection_management);
            buttonKakuActivity = FindViewById<Button>(Resource.Id.btn_light_management);
            buttonTimeActivity = FindViewById<Button>(Resource.Id.btn_time_management);

            //Tell each button to start a different activity.
            buttonAlarmActivity.Click += (o, s) => StartActivity(typeof(AlarmActivity));
            buttonConnectionActivity.Click += (o, s) => StartActivity(typeof(ConnectionActivity));
            buttonKakuActivity.Click += (o, s) => StartActivity(typeof(KakuActivity));
            buttonTimeActivity.Click += (o, s) => StartActivity(typeof(TimeActivity));
        }
    }
}

