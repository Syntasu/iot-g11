using Android.App;
using Android.OS;
using Android.Widget;
using IOT_app.Code;
using IOT_app.Code.IO;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;

namespace IOT_app
{
    [Activity(Label = "AddEditAlarmActivity")]
    public class AddEditAlarmActivity : Activity
    {
        private Button btnSetButton;
        private EditText editTextAlarmName;
        private DatePicker dpAlarmDatePicker;
        private NumberPicker npAlarmHourNumberPicker;
        private NumberPicker npAlarmMinutesNumberPicker;

        private List<Alarm> alarms = new List<Alarm>();
        private Alarm alarmInQuestion = null;

        protected async override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            base.SetContentView(Resource.Layout.AddEditAlarm);

            //Load elements from view.
            btnSetButton = FindViewById<Button>(Resource.Id.btn_alarm_set);
            editTextAlarmName = FindViewById<EditText>(Resource.Id.etext_alarm_name);
            dpAlarmDatePicker = FindViewById<DatePicker>(Resource.Id.dt_alarm_date);
            npAlarmHourNumberPicker = FindViewById<NumberPicker>(Resource.Id.np_alarm_hour);
            npAlarmMinutesNumberPicker = FindViewById<NumberPicker>(Resource.Id.np_alarm_minute);

            //Set min max for hours.
            npAlarmHourNumberPicker.MinValue = 0;
            npAlarmHourNumberPicker.MaxValue = 23;

            //Set min max for minutes.
            npAlarmMinutesNumberPicker.MinValue = 0;
            npAlarmMinutesNumberPicker.MaxValue = 59;

            //Load the current alarms from disk.
            alarms = await IOWorker.ReadAlarmFile();

            string serializedAlarm = Intent.GetStringExtra("alarm");

            //Read in existing values or use a default values.
            if (!string.IsNullOrEmpty(serializedAlarm))
            {
                alarmInQuestion = JsonConvert.DeserializeObject<Alarm>(serializedAlarm);
                SetAlarmFields(alarmInQuestion.Name, alarmInQuestion.Time);
            }
            else
            {
                SetAlarmFields("", DateTime.Now);
            }

            //Bind elements to method(s).
            btnSetButton.Click += (o, s) => SaveAlarms();
        }

        /// <summary>
        ///     Set all the values in the user interface.
        /// </summary>
        /// <param name="name"> Name of the alarm. </param>
        /// <param name="time"> THe time of the alarm. </param>
        private void SetAlarmFields(string name, DateTime time)
        {
            //Populate the fields if we receive an existing alarm.
            editTextAlarmName.Text = name;
            dpAlarmDatePicker.DateTime = time;
            npAlarmHourNumberPicker.Value = time.Hour;
            npAlarmMinutesNumberPicker.Value = time.Minute;
        }

        /// <summary>
        ///     Save any new alarm that we added or modified.
        ///     Save this to disk.
        /// </summary>
        private async void SaveAlarms()
        {
            List<Alarm> tempAlarmList = new List<Alarm>();

            //Validate if the user input is valid.
            if(ValidAlarmInputs(out string name, out DateTime time))
            {
                //Construct a new alarm and add it to the temporary list.
                Alarm alarm = new Alarm(name, time);
                tempAlarmList.Add(alarm);

                //Copy over the alarms list, skip any modified alarms.
                foreach (Alarm a in alarms)
                {
                    if(a.Name != name)
                    {
                        tempAlarmList.Add(a);
                    }
                }

                //Save the new alarm array.
                alarms = tempAlarmList;
                await IOWorker.SaveAlarmFile(alarms);

                //Go back to the main alarm form.
                StartActivity(typeof(AlarmActivity));
            }

            //Else we do nothing, let the user have the chance to adjust the values and try again.
        }

        /// <summary>
        ///     Check if the user input is valid.
        /// </summary>
        /// <param name="alarmName">The alarm name we want to emit when the validation was successfull.</param>
        /// <param name="alarmTime">The alarm tume we want to emit when the validation was successfull.</param>
        /// <returns>Bool wether the validation was succesfull.</returns>
        private bool ValidAlarmInputs(out string alarmName, out DateTime alarmTime)
        {
            //Fetch all the user input.
            DateTime date = dpAlarmDatePicker.DateTime;
            int hours = npAlarmHourNumberPicker.Value;
            int minutes = npAlarmMinutesNumberPicker.Value;
            string name = editTextAlarmName.Text;

            //Check if the user supplied a name.
            if (string.IsNullOrEmpty(name) || name.Length > 20)
            {
                Toast.MakeText(this, Resource.String.toast_alarm_name_invalid, ToastLength.Long);
                alarmName = "";
                alarmTime = DateTime.Now;
                return false;
            }

            //Assign name and time.
            alarmName = name;
            alarmTime = new DateTime(
                date.Year, date.Month, date.Day,    //Take the part from the datepicker
                hours, minutes, 0                   //Take the part from the time picker.                 
            );

            TimeSpan difference = alarmTime - DateTime.Now;

            //Check if we don't put an alarm in the past.
            if(difference.TotalMinutes < 0)
            {
                alarmName = "";
                alarmTime = DateTime.Now;
                Toast.MakeText(this, Resource.String.toast_alarm_time_invalid, ToastLength.Long);
                return false;
            }

            return true;
        }
    }
}