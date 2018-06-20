
using Android.Content;
using Android.Views;
using Android.Widget;
using System;
using System.Collections.Generic;

namespace IOT_app.Code
{
    public class AlarmAdapter : BaseAdapter<Alarm>
    {
        public List<Alarm> alarmCollection;
        private Context context;

        public AlarmAdapter(Context context, List<Alarm> alarmCollection)
        {
            this.alarmCollection = alarmCollection;
            this.context = context;
        }

        public override Alarm this[int position]
        {
            get
            {
                return alarmCollection[position];
            }
        }
        public override int Count
        {
            get
            {
                return alarmCollection.Count;
            }
        }

        public override long GetItemId(int position)
        {
            return position;
        }

        public override View GetView(int position, View convertView, ViewGroup parent)
        {
            View view = convertView;

            try
            {
                if (view == null)
                {
                    view = LayoutInflater.From(context).Inflate(Resource.Layout.listview_item_alarm, null, false);
                }

                Alarm alarm = this[position];
                view.FindViewById<TextView>(Resource.Id.alarm_name).Text = alarm.Name;
                view.FindViewById<TextView>(Resource.Id.alarm_time).Text = alarm.Time.ToString();
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex.Message);
            }

            return view;
        }
    }
}