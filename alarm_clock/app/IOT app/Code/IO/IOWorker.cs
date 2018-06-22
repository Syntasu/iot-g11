using Newtonsoft.Json;
using PCLStorage;
using System.Collections.Generic;
using System.IO;
using System.Threading.Tasks;

namespace IOT_app.Code.IO
{
    public class IOWorker
    {
        private static IFolder Root => FileSystem.Current.LocalStorage;

        public static async Task<List<Alarm>> ReadAlarmFile()
        {
            bool alarmFileExists = await DoesFileExist("alarm.json");

            if(!alarmFileExists)
            {
                await CreateFile("alarm.json");
                await WriteToFile("alarm.json", "{}");
            }

            string data = await ReadFromFile("alarm.json");
            return JsonConvert.DeserializeObject<List<Alarm>>(data);
        }

        public static async Task WriteAlarmFile(List<Alarm> alarms)
        {
            bool alarmFileExists = await DoesFileExist("alarm.json");

            if (alarmFileExists)
            {
                await DeleteFile("alarm.json");
                await CreateFile("alarm.json");
            }

            string content = JsonConvert.SerializeObject(alarms);
            await WriteToFile("alarm.json", content);
        }

        private async static Task<bool> DoesFileExist(string fileName)
        {
            ExistenceCheckResult fileExists = await Root.CheckExistsAsync(fileName);
            return fileExists == ExistenceCheckResult.FileExists;
        }

        private async static Task<bool> DoesFolderExist(string folderName)
        {
            ExistenceCheckResult folderExists = await Root.CheckExistsAsync(folderName);
            return folderExists == ExistenceCheckResult.FolderExists;
        }

        private async static Task<IFolder> CreateFolder(string folderName)
        {
            return await Root.CreateFolderAsync(folderName, CreationCollisionOption.ReplaceExisting);
        }

        private async static Task<IFile> CreateFile(string fileName)
        {
            return await Root.CreateFileAsync(fileName, CreationCollisionOption.ReplaceExisting);
        }

        private async static Task<bool> WriteToFile(string fileName, string content = "")
        {
            IFile file = await CreateFile(fileName);
            await file.WriteAllTextAsync(content);
            return true;
        }

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
    }
}