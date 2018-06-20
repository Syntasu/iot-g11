using Android.App;
using Android.OS;

namespace IOT_app
{
    [Activity(Label = "KakuActivity")]
    public class KakuActivity : Activity
    {
        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            base.SetContentView(Resource.Layout.Lights);

        }
    }
}