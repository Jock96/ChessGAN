namespace BL.Utils
{
    using BL.Constants;
    using BL.Enums;
    using BL.Helpers;

    using System.IO;

    /// <summary>
    /// Инструмент управления игрой.
    /// </summary>
    public static class GameManager
    {
        /// <summary>
        /// Счётчик.
        /// </summary>
        private static int _counter = 10;

        /// <summary>
        /// Отправить сообщение хода модулю.
        /// </summary>
        /// <param name="message">Сообщение хода.</param>
        /// <param name="callback">Ответный ход.</param>
        /// <param name="responce">Ответ модуля.</param>
        /// <returns>Возвращает корректность выполнения.</returns>
        public static bool SendTurnMessage(string message, out string callback, out string responce)
        {
            callback = string.Empty;
            responce = MessageConstants.TIMEOUT;

            var mainInputFilePath = PathHelper.GetDebugInputPathByType(IOTypes.Main);
            var mainOutputFilePath = PathHelper.GetDebugOutputPathByType(IOTypes.Main);

            var moduleInputFilePath = PathHelper.GetDebugInputPathByType(IOTypes.Module);
            var moduleOutputFilePath = PathHelper.GetDebugOutputPathByType(IOTypes.Module);

            using (var writer = new StreamWriter(mainOutputFilePath))
            {
                writer.Write(string.Empty);
            }

            using (var writer = new StreamWriter(mainInputFilePath))
            {
                writer.Write(message.ToLower());
            }

            var innerCounter = 0;

            while(innerCounter < _counter)
            {
                if(File.Exists(moduleOutputFilePath))
                {
                    responce = MessageConstants.GOOD_RESPONCE;

                    //using (var reader = new StreamReader(moduleOutputFilePath))
                    //{
                    //    callback = reader.ReadLine();
                    //}

                    try
                    {
                        using (var reader = new StreamReader(moduleOutputFilePath))
                        {
                            callback = reader.ReadLine();
                        }
                    }
                    catch
                    {
                        System.Threading.Thread.Sleep(500);
                        ++innerCounter;
                        continue;
                    }

                    break;
                }

                System.Threading.Thread.Sleep(500);
                ++innerCounter;
            }

            if (responce.Equals(MessageConstants.GOOD_RESPONCE))
            {
                File.Delete(moduleOutputFilePath);
                File.Delete(moduleInputFilePath);

                return true;
            }
            else
            {
                return false;
            }
        }
    }
}
