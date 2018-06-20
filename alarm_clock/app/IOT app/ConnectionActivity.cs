using Android.App;
using Android.OS;

namespace IOT_app
{
    [Activity(Label = "ConnectionActivity")]
    public class ConnectionActivity : Activity
    {
        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            base.SetContentView(Resource.Layout.Connection);
        }
    }
}