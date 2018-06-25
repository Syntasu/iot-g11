using Newtonsoft.Json;
using PCLStorage;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace IOT_app.Code.IO
{
    public class IOWorker
    {
        private static IFolder Root => FileSystem.Current.LocalStorage;

        /// <summary>
        ///     Read the alarms stored on the disk.
        /// </summary>
        /// <returns>
        ///     A list of alarms that was read from the disk.
        ///     If there is nothing on the disk, return an empty list.
        /// </returns>
        public static async Task<List<Alarm>> ReadAlarmFile()
        {
            //Check if alarm file exists.
            bool alarmFileExists = await DoesFileExist("alarm.json");

            if(!alarmFileExists)
            {
                await CreateFile("alarm.json");
            }

            //Fetch  the data from the file. (JSON).
            string data = await ReadFromFile("alarm.json");

            if (string.IsNullOrEmpty(data))
            {
                //We don't have any stored data, return this.
                return new List<Alarm>();
            }
            else
            {
                //Convert from json of what ever was on the disk.
                return JsonConvert.DeserializeObject<List<Alarm>>(data);
            }
        }

        /// <summary>
        ///     Save the alarms list to disk in JSON format.
        /// </summary>
        /// <param name="alarms"> The list of alarms we want to serialize to disk.</param>
        /// <returns>Void, but async always needs to return Task.</returns>
        public static async Task SaveAlarmFile(List<Alarm> alarms)
        {
            //Overwrite the existing file.
            await CreateFile("alarm.json");

            //Serialize the contents and write it out the disk.
            string content = JsonConvert.SerializeObject(alarms);
            await WriteToFile("alarm.json", content);
        }

        /// <summary>
        ///     Remove the alarms file, clearing out any previous alarms.
        /// </summary>
        /// <returns>Bool wether we actually cleared the alarms file or not.</returns>
        public static async Task<bool> ClearAlarmFile()
        {
            bool alarmFileExists = await DoesFileExist("alarm.json");

            if (alarmFileExists)
            {
                await DeleteFile("alarm.json");
                return true;
            }

            return false;
        }

        #region Helper functions

        /// <summary>
        ///     Check if a given file exists in the root folder of the application.
        /// </summary>
        /// <param name="fileName">The file name we want to check against.</param>
        /// <returns>Bool wether the file exists or not.</returns>
        private async static Task<bool> DoesFileExist(string fileName)
        {
            ExistenceCheckResult fileExists = await Root.CheckExistsAsync(fileName);
            return fileExists == ExistenceCheckResult.FileExists;
        }

        /// <summary>
        ///     Check if a given folder exists in the root folder of the application.
        /// </summary>
        /// <param name="folderName">The folder name we want to check against.</param>
        /// <returns>A bool, wether if the folder exists or not.</returns>
        private async static Task<bool> DoesFolderExist(string folderName)
        {
            ExistenceCheckResult folderExists = await Root.CheckExistsAsync(folderName);
            return folderExists == ExistenceCheckResult.FolderExists;
        }

        /// <summary>
        ///     Create a new folder in the root directory of the application.
        /// </summary>
        /// <param name="folderName">The name of the folder we want to create.</param>
        /// <returns>A reference to the newly created folder.</returns>
        private async static Task<IFolder> CreateFolder(string folderName)
        {
            return await Root.CreateFolderAsync(folderName, CreationCollisionOption.ReplaceExisting);
        }

        /// <summary>
        ///     Create a new file in the root directory of the application.
        /// </summary>
        /// <param name="fileName"> The file name we want to create.</param>
        /// <returns>A reference back the newly created file.</returns>
        private async static Task<IFile> CreateFile(string fileName)
        {
            return await Root.CreateFileAsync(fileName, CreationCollisionOption.ReplaceExisting);
        }

        /// <summary>
        ///     Write a new file to disk.
        /// </summary>
        /// <param name="fileName">The name of the file</param>
        /// <param name="content">The contents of the file that should be written.</param>
        /// <returns>Void, task stuff.</returns>
        private async static Task WriteToFile(string fileName, string content = "")
        {
            IFile file = await CreateFile(fileName);
            await file.WriteAllTextAsync(content);
        }

        /// <summary>
        ///     Read a file from disk.
        /// </summary>
        /// <param name="fileName">The file we want to read from disk.</param>
        /// <returns>The contents of the file.</returns>
        private async static Task<string> ReadFromFile(string fileName)
        {
            string content = "";
            bool exists = await DoesFileExist(fileName);

            if (exists)
            {
                IFile file = await Root.GetFileAsync(fileName);
                content = await file.ReadAllTextAsync();
            }

            return content;
        }

        /// <summary>
        ///     Delete a file from disk
        /// </summary>
        /// <param name="fileName">The file we want to delete</param>
        /// <returns>Bool wether it succeeded or not.</returns>
        private async static Task<bool> DeleteFile(string fileName)
        {
            bool exists = await DoesFileExist(fileName);

            if (exists) 
            {
                IFile file = await Root.GetFileAsync(fileName);
                await file.DeleteAsync();

                return true;
            }

            return false;
        }

        #endregion
    }
}