using Android.App;
using Android.OS;

namespace IOT_app
{
    [Activity(MainLauncher = true, Label = "MainActivity", ScreenOrientation = Android.Content.PM.ScreenOrientation.Portrait)]
    public class MainActivity : Activity
    {
        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            base.SetContentView(Resource.Layout.main);

            StartActivity(typeof(AlarmActivity));
        }
    }
}

