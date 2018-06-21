using Android.App;
using Android.OS;
using Android.Widget;
using IOT_app.Code;

namespace IOT_app
{
    [Activity(Label = "ConnectionActivity")]
    public class ConnectionActivity : Activity
    {
        private TextView textConnectionStatus;
        private TextView textIpAddress;
        private TextView textPort;

        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            base.SetContentView(Resource.Layout.Connection);

            textConnectionStatus = FindViewById<TextView>(Resource.Id.text_connection_status_value);
            textIpAddress = FindViewById<TextView>(Resource.Id.text_connection_ip_value);
            textPort = FindViewById<TextView>(Resource.Id.text_connection_port_value);

            SocketWorker.OnSocketConnect += OnConnect;
            SocketWorker.OnSocketDisconnect += OnDisconnect;

            SetConnectionDetails();
        }

        private void OnDisconnect()
        {
            SetConnectionDetails();
        }

        private void OnConnect()
        {
            SetConnectionDetails();
        }

        private void SetConnectionDetails()
        {
            if(SocketWorker.IsConnected)
            {
                textConnectionStatus.Text = "connected";
                textIpAddress.Text = SocketWorker.ConnectedTo.ToString();
                textPort.Text = SocketWorker.ConnectedPort.ToString();
            }
            else
            {
                textConnectionStatus.Text = "disconnected";
                textIpAddress.Text = "n.a.";
                textPort.Text = "n.a.";
            }
        }
    }
}