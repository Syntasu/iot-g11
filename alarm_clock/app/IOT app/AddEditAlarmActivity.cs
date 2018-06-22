using Android.App;
using Android.OS;

namespace IOT_app
{
    [Activity(Label = "AddEditAlarmActivity")]
    public class AddEditAlarmActivity : Activity
    {
        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            base.SetContentView(Resource.Layout.AddEditAlarm);
        }
    }
}