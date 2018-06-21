using Android.App;
using Android.Graphics;
using Android.OS;
using Android.Widget;
using IOT_app.Code;

namespace IOT_app
{
    [Activity(MainLauncher = true, Label = "MainActivity", ScreenOrientation = Android.Content.PM.ScreenOrientation.Portrait)]
    public class MainActivity : Activity
    {
        private Button buttonSnoozeAlarms;
        private Button buttonStopAlarms;
        private TextView textStatus;

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
            buttonSnoozeAlarms = FindViewById<Button>(Resource.Id.btn_quick_snooze);
            buttonStopAlarms = FindViewById<Button>(Resource.Id.btn_quick_stop);
            buttonAlarmActivity = FindViewById<Button>(Resource.Id.btn_alarm_management);
            buttonConnectionActivity = FindViewById<Button>(Resource.Id.btn_connection_management);
            buttonKakuActivity = FindViewById<Button>(Resource.Id.btn_light_management);
            buttonTimeActivity = FindViewById<Button>(Resource.Id.btn_time_management);

            textStatus = FindViewById<TextView>(Resource.Id.text_status);

            //Tell each button to start a different activity.
            buttonAlarmActivity.Click += (o, s) => StartActivity(typeof(AlarmActivity));
            buttonConnectionActivity.Click += (o, s) => StartActivity(typeof(ConnectionActivity));
            buttonKakuActivity.Click += (o, s) => StartActivity(typeof(KakuActivity));
            buttonTimeActivity.Click += (o, s) => StartActivity(typeof(TimeActivity));

            //Check if we have connection. Enable the quick menu when we do.
            if(SocketWorker.IsConnected)
            {
                textStatus.Text = "Status: " + GetString(Resource.String.info_connected);
                textStatus.SetTextColor(Color.Rgb(50, 150, 50));
                buttonSnoozeAlarms.Enabled = true;
                buttonStopAlarms.Enabled = true;

            }
            else
            {
                textStatus.Text = "Status: " + GetString(Resource.String.info_disconnected);
                textStatus.SetTextColor(Color.Rgb(150, 50, 50));
                buttonSnoozeAlarms.Enabled = false;
                buttonStopAlarms.Enabled = false;
            }
        }
    }
}

