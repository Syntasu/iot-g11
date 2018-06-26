using Android.App;
using Android.Graphics;
using Android.OS;
using Android.Widget;
using IOT_app.Code;

namespace IOT_app
{
    [Activity(Icon = "@mipmap/ic_launcher_round", MainLauncher = true, Label = "SAK App", ScreenOrientation = Android.Content.PM.ScreenOrientation.Portrait)]
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

        protected async override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            base.SetContentView(Resource.Layout.main);

            //TODO: REMOVE;
            //await IOWorker.ClearFile(AppFiles.Alarm, AppFileExtension.JSON);
            //await IOWorker.ClearFile(AppFiles.LightSocket, AppFileExtension.JSON);

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

            buttonSnoozeAlarms.Click += (o, s) => SnoozeAlarms();
            buttonStopAlarms.Click += (o, s) => StopAlarms();

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

        private void SnoozeAlarms()
        {
            SocketWorker.Send(Commands.AlarmSnooze);
        }

        private void StopAlarms()
        {
            SocketWorker.Send(Commands.AlarmStop);
        }
    }
}

