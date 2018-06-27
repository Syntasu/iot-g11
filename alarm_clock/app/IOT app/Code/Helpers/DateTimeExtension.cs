
using System;

namespace IOT_app.Code
{
    public static class DateTimeExtension
    {
        /// <summary>
        ///     Convert C# DateTime into something the arduino can read.
        /// </summary>
        /// <param name="time">The time we want to convert to an agnostic string.</param>
        /// <returns>A semi colon seperated string with year, months, day, hour, minute and seconds.</returns>
        public static string[] ToAgnosticString(this DateTime time)
        {
            string[] output = new string[2];
            output[0] = $"{time.Year};{time.Month};{time.Day}";
            output[1] = $"{time.Hour};{time.Minute};{time.Second}";

            return output;
        }

    }
}