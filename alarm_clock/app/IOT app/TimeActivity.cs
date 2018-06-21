using Android.App;
using Android.OS;

namespace IOT_app
{
    [Activity(Label = "TimeActivity")]
    public class TimeActivity : Activity
    {
        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            base.SetContentView(Resource.Layout.Time);
        }
    }
}