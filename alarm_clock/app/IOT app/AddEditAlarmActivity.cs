using Android.App;
using Android.OS;
using Android.Widget;

namespace IOT_app
{
    [Activity(Label = "AddEditAlarmActivity")]
    public class AddEditAlarmActivity : Activity
    {
        private Button btnSetButton;

        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            base.SetContentView(Resource.Layout.AddEditAlarm);

            btnSetButton = FindViewById<Button>(Resource.Id.btn_alarm_set);
            btnSetButton.Click += (o, s) => StoreAlarm();
        }

        private void StoreAlarm()
        {
           
        }
    }
}