using Android.App;
using Android.OS;
using Android.Widget;
using IOT_app.Code;
using IOT_app.Code.IO;
using IOT_app.Code.IO.Data;
using System;
using System.Text.RegularExpressions;

namespace IOT_app
{
    [Activity(Label = "ConnectionActivity")]
    public class ConnectionActivity : Activity
    {
        private TextView textConnectionStatus;
        private TextView textIpAddress;
        private TextView textPort;

        private EditText editTextIp;
        private EditText editTextPort;

        private Button buttonConnect;
        private Button buttonDisconect;
        private Button buttonBack;

        protected async override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            base.SetContentView(Resource.Layout.Connection);

            //Find all the elements from the view.
            textConnectionStatus = FindViewById<TextView>(Resource.Id.text_connection_status_value);
            textIpAddress = FindViewById<TextView>(Resource.Id.text_connection_ip_value);
            textPort = FindViewById<TextView>(Resource.Id.text_connection_port_value);
            editTextIp = FindViewById<EditText>(Resource.Id.etext_connection_ip);
            editTextPort = FindViewById<EditText>(Resource.Id.etext_connection_port);
            buttonConnect = FindViewById<Button>(Resource.Id.btn_connection_connect);
            buttonDisconect = FindViewById<Button>(Resource.Id.btn_connection_disconnect);
            buttonBack = FindViewById<Button>(Resource.Id.btn_connection_back);

            //Update the connection if a connection state changes.
            SocketWorker.OnSocketConnect += SetConnectionDetails;
            SocketWorker.OnSocketDisconnect += SetConnectionDetails;

            buttonConnect.Click += (o, s) => Connect();
            buttonDisconect.Click += (o, s) => Disconnect();
            buttonBack.Click += (o, s) => StartActivity(typeof(MainActivity));

            //Check if we previously connected and try those connection details.
            ConnectionData data = 
                await IOWorker.ReadFile<ConnectionData>(AppFiles.Connection);

            //Set the connection details from previous connection.
            if (!data.Equals(default(ConnectionData)))
            {
                textIpAddress.Text = data.IP;
                textPort.Text = data.Port.ToString();
            }

            SetConnectionDetails();

        }

        /// <summary>
        ///     Make a connection to the arduino.
        ///     Parse the user inputs and try to connect.
        /// </summary>
        private void Connect()
        {
            //Get user input
            string ip = editTextIp.Text;
            string port = editTextPort.Text;

            //Sanitize the user input.
            if(!IsValidIP(ip))
            {
                Toast.MakeText(this, Resource.String.error_invalid_ip, ToastLength.Long).Show();
                return;
            }

            if(!IsValidPort(port))
            {
                Toast.MakeText(this, Resource.String.error_invalid_port, ToastLength.Long).Show();
                return;
            }

            SockErr err = SocketWorker.Connect(ip, int.Parse(port));

            //Handle possible states after connecting.
            switch(err)
            {
                case SockErr.None:
                    SetConnectionDetails();
                    SaveConnectionDetails(ip);
                    break;
                case SockErr.ConnectionDuplicate:
                    Toast.MakeText(this, Resource.String.sockerr_duplicate, ToastLength.Long).Show();
                    break;
                case SockErr.ConnectionTimeout:
                    Toast.MakeText(this, Resource.String.sockerr_timeout, ToastLength.Long).Show();
                    break;
                default:
                    Toast.MakeText(this, Resource.String.sockerr_failed, ToastLength.Long).Show();
                    break;
            }
        }

        /// <summary>
        ///     Disconnect to whatever we are connected to.
        ///     Also forget the previous connection details.
        /// </summary>
        private async void Disconnect()
        {
            SocketWorker.Disconnect();
            await IOWorker.ClearFile(AppFiles.Connection, AppFileExtension.JSON);
            SetConnectionDetails();

            Toast.MakeText(this, Resource.String.toast_disconnected, ToastLength.Long).Show();
        }

        /// <summary>
        ///     Check if the given IP is valid or not.
        /// </summary>
        /// <param name="ip">The input IP.</param>
        /// <returns>True when valid.</returns>
        private bool IsValidIP(string ip)
        {
            if (!string.IsNullOrEmpty(ip))
            {
                Regex regex = new Regex("\\b((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)(\\.|$)){4}\\b");
                Match match = regex.Match(ip);
                return match.Success;
            }
            else
            {
                return false;
            }
        }

        /// <summary>
        ///     Checks if the given port is valid.
        /// </summary>
        /// <param name="port">The given port we want to check.</param>
        /// <returns>True if the port is valid.</returns>
        private bool IsValidPort(string port)
        {
            if (!string.IsNullOrEmpty(port))
            {
                Regex regex = new Regex("[0-9]+");
                Match match = regex.Match(port);

                if (match.Success)
                {
                    int portAsInteger = Int32.Parse(port);
                    return (portAsInteger >= 0 && portAsInteger <= 65535);
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }

        //Update the connection details in the UI.
        private void SetConnectionDetails()
        {
            if(SocketWorker.IsConnected)
            {
                textConnectionStatus.Text = GetString(Resource.String.info_connected);
                textIpAddress.Text = SocketWorker.ConnectedTo.ToString();
                textPort.Text = SocketWorker.ConnectedPort.ToString();
                buttonDisconect.Enabled = true;
            }
            else
            {
                textConnectionStatus.Text = GetString(Resource.String.info_disconnected);
                textIpAddress.Text = GetString(Resource.String.na);
                textPort.Text = GetString(Resource.String.na);
                buttonDisconect.Enabled = false;
            }
        }

        /// <summary>
        ///     Store the connection data on disk.
        /// </summary>
        private async void SaveConnectionDetails(string ip)
        {
            ConnectionData data = new ConnectionData(
                ip,
                SocketWorker.ConnectedPort
            );

            string json = data.Serialize();
            await IOWorker.SaveFile(AppFiles.Connection, AppFileExtension.JSON, data);
        }
    }
}