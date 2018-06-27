using Android.App;
using Android.Graphics;
using Android.OS;
using Android.Widget;
using IOT_app.Code;
using IOT_app.Code.IO;
using IOT_app.Code.IO.Data;

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

            //TODO, REMOVE: Debug code.
            await IOWorker.ClearFile(AppFiles.Alarm, AppFileExtension.JSON);
            await IOWorker.ClearFile(AppFiles.LightSocket, AppFileExtension.JSON);
            //await IOWorker.ClearFile(AppFiles.Connection, AppFileExtension.JSON);

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

            SetUI(SocketWorker.IsConnected);

            //Check if we previously connected and try those connection details.
            ConnectionData data = await IOWorker.ReadFile<ConnectionData>(AppFiles.Connection);

            //Try to connect to connections details we used earlier.
            if (!data.Equals(default(ConnectionData)))
            {
                if (!SocketWorker.IsConnected)
                {
                    SockErr err = SocketWorker.Connect(data.IP, data.Port);

                    //If we do not have an error, notify the user we connected successfully
                    if (err == SockErr.None)
                    {
                        Toast.MakeText(this, Resource.String.toast_connected, ToastLength.Long).Show();
                        SetUI(SocketWorker.IsConnected);
                    }
                }
            }
        }

        /// <summary>
        ///     Set the state for all the buttons depending on wether if the app is connected or not.
        /// </summary>
        private void SetUI(bool connectionState)
        {
            if (connectionState)
            {
                textStatus.Text = "Status: " + GetString(Resource.String.info_connected);
                textStatus.SetTextColor(Color.Rgb(50, 150, 50));
            }
            else
            {
                textStatus.Text = "Status: " + GetString(Resource.String.info_disconnected);
                textStatus.SetTextColor(Color.Rgb(150, 50, 50));
            }
 
            buttonSnoozeAlarms.Enabled = connectionState;
            buttonStopAlarms.Enabled = connectionState;
            buttonAlarmActivity.Enabled = connectionState;
            buttonKakuActivity.Enabled = connectionState;
            buttonTimeActivity.Enabled = connectionState;
        }

        /// <summary>
        ///     Send the command to snooze all alarms on the arduino.
        /// </summary>
        private void SnoozeAlarms()
        {
            SocketWorker.Send(Commands.AlarmSnooze);
        }

        /// <summary>
        ///     Send the stop all command to the arduino.
        /// </summary>
        private void StopAlarms()
        {
            SocketWorker.Send(Commands.AlarmStop);
        }
    }
}

