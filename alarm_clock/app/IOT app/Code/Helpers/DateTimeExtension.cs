
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
        public static string ToAgnosticString(this DateTime time)
        {
            return $"{time.Year}/{time.Month}/{time.Day}/{time.Hour}/{time.Minute}/{time.Second}";
        }

    }
}