using Android.App;
using Android.OS;
using Android.Widget;
using IOT_app.Code;
using IOT_app.Code.IO;
using System.Threading.Tasks;

namespace IOT_app
{
    [Activity(Label = "KakuActivity")]
    public class KakuActivity : Activity
    {
        private Button btnBack;
        private CheckBox cbLightSocket1;
        private CheckBox cbLightSocket2;
        private CheckBox cbLightSocket3;
        private CheckBox cbLightSocket4;
        private CheckBox cbLightSocket5;
        private bool[] socketStates = new bool[5];

        protected async override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            base.SetContentView(Resource.Layout.Lights);

            //Find all the elements from the view.
            btnBack = FindViewById<Button>(Resource.Id.btn_lights_cancel);
            cbLightSocket1 = FindViewById<CheckBox>(Resource.Id.cb_light1);
            cbLightSocket2 = FindViewById<CheckBox>(Resource.Id.cb_light2);
            cbLightSocket3 = FindViewById<CheckBox>(Resource.Id.cb_light3);
            cbLightSocket4 = FindViewById<CheckBox>(Resource.Id.cb_light4);
            cbLightSocket5 = FindViewById<CheckBox>(Resource.Id.cb_light5);

            //Read current values from disk.
            bool[] lightSocketDataFromDisk = await IOWorker.ReadFile<bool[]>(AppFiles.LightSocket);

            //Assign loaded data to local variable.
            if(lightSocketDataFromDisk != null)
                socketStates = lightSocketDataFromDisk;

            //Set the values we fetched from disk or default values.
            cbLightSocket1.Checked = socketStates[0];
            cbLightSocket2.Checked = socketStates[1];
            cbLightSocket3.Checked = socketStates[2];
            cbLightSocket4.Checked = socketStates[3];
            cbLightSocket5.Checked = socketStates[4];

            //Register all the events for those elements.
            cbLightSocket1.CheckedChange += (s, e) => OnLightSocketChanged(1);
            cbLightSocket2.CheckedChange += (s, e) => OnLightSocketChanged(2);
            cbLightSocket3.CheckedChange += (s, e) => OnLightSocketChanged(3);
            cbLightSocket4.CheckedChange += (s, e) => OnLightSocketChanged(4);
            cbLightSocket5.CheckedChange += (s, e) => OnLightSocketChanged(5);
            btnBack.Click += (s, e) => StartActivity(typeof(MainActivity));
        }

        /// <summary>
        ///     Called when any of the 5 light sockets checkboxes change.
        ///     Required to sync up with the arduino.
        /// </summary>
        /// <param name="id">The id of the light socket we are changing.</param>
        private async void OnLightSocketChanged(int id)
        {
            bool state = false;

            //Check which kaku was edited, set the state accordingly
            switch (id)
            {
                case 1:
                    state = cbLightSocket1.Checked;
                    socketStates[0] = state;
                    break;
                case 2:
                    state = cbLightSocket2.Checked;
                    socketStates[1] = state;
                    break;
                case 3:
                    state = cbLightSocket3.Checked;
                    socketStates[2] = state;
                    break;
                case 4:
                    state = cbLightSocket4.Checked;
                    socketStates[3] = state;
                    break;
                case 5:
                    state = cbLightSocket5.Checked;
                    socketStates[4] = state;
                    break;
            }

            //Save the changes to disk.
            await IOWorker.SaveFile(AppFiles.LightSocket, AppFileExtension.JSON, socketStates);

            //Disable the checkboxes for 1/3 of a second.
            //This could lead to the disk and arduino being spammed
            //by the users, so we want to protect them from doing to.
            await Task.Run(async () =>
            {
                cbLightSocket1.Enabled = false;
                cbLightSocket2.Enabled = false;
                cbLightSocket3.Enabled = false;
                cbLightSocket4.Enabled = false;
                cbLightSocket5.Enabled = false;
                await Task.Delay(333);

            }).ContinueWith((task) => 
            {
                cbLightSocket1.Enabled = true;
                cbLightSocket2.Enabled = true;
                cbLightSocket3.Enabled = true;
                cbLightSocket4.Enabled = true;
                cbLightSocket5.Enabled = true;
            });

            //Syncronize arduino from app.
            string s = state ? "1" : "0";
            SocketWorker.Send(Commands.SyncKaku, id.ToString(), s);
        }
    }
}