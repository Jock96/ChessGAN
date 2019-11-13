namespace BL.Helpers
{
    using BL.Enums;
    using System;
    using System.IO;

    /// <summary>
    /// Инструмент для работы с директориями.
    /// </summary>
    public static class PathHelper
    {
        /// <summary>
        /// Получает путь до папки ресурсов.
        /// </summary>
        /// <returns>Возвращает путь до папки ресурсов.</returns>
        public static string GetResourcesPath()
        {
            var rootPath = Directory.GetParent(Directory.GetParent(Directory.GetCurrentDirectory()).FullName).FullName;
            return rootPath + $"{Constants.PathConstants.RESOURCES_PATH}";
        }

        /// <summary>
        /// Получает путь до папки с модулем.
        /// </summary>
        /// <returns>Возвращает путь до папки с модулем.</returns>
        public static string GetModulePath()
        {
            var rootPath = Directory.GetParent(Directory.GetParent(Directory.GetCurrentDirectory()).FullName).FullName;
            return rootPath + $"{Constants.PathConstants.MODULE_PATH}{Constants.PathConstants.MODULE_BIN_PATH}";
        }

        /// <summary>
        /// Получает путь до дебаг модуля.
        /// </summary>
        /// <returns>Возвращает путь до дебаг модуля.</returns>
        public static string GetDebugModulePath()
        {
            var rootPath = Directory.GetParent(Directory.GetParent(
                Directory.GetParent(Directory.GetCurrentDirectory()).FullName).FullName).FullName;

            return rootPath + $"{Constants.PathConstants.DEBUG_PATH}" +
                $"{Constants.PathConstants.DEBUG_MODULE_NAME}{Constants.PathConstants.EXTENSION}";
        }

        /// <summary>
        /// Получает путь до дебаг.
        /// </summary>
        /// <returns>Возвращает путь до дебаг.</returns>
        public static string GetDebugPath()
        {
            var rootPath = Directory.GetParent(Directory.GetParent(
                Directory.GetParent(Directory.GetCurrentDirectory()).FullName).FullName).FullName;

            return rootPath + $"{Constants.PathConstants.DEBUG_PATH}";
        }

        /// <summary>
        /// Получает путь до ввода в модуль по типу.
        /// </summary>
        /// <returns>Возвращает путь до ввода в модуль.</returns>
        public static string GetDebugInputPathByType(IOTypes type)
        {
            var rootPath = Directory.GetParent(Directory.GetParent(
                Directory.GetParent(Directory.GetCurrentDirectory()).FullName).FullName).FullName;

            switch (type)
            {
                case IOTypes.Main:
                    return rootPath + $"{Constants.PathConstants.DEBUG_PATH}{Constants.PathConstants.MODULE_INPUT_MAIN}";

                case IOTypes.Module:
                    return rootPath + $"{Constants.PathConstants.DEBUG_PATH}{Constants.PathConstants.MODULE_INPUT_MODULE}";

                case IOTypes.None:
                    return rootPath + $"{Constants.PathConstants.DEBUG_PATH}{Constants.PathConstants.MODULE_INPUT_NONE}";
            }

            throw new Exception("Неизвестный тип ввода.");
        }

        /// <summary>
        /// Получает путь до вывода из модуля по типу.
        /// </summary>
        /// <returns>Возвращает путь до вывода из модуля.</returns>
        public static string GetDebugOutputPathByType(IOTypes type)
        {
            var rootPath = Directory.GetParent(Directory.GetParent(
                Directory.GetParent(Directory.GetCurrentDirectory()).FullName).FullName).FullName;

            switch (type)
            {
                case IOTypes.Main:
                    return rootPath + $"{Constants.PathConstants.DEBUG_PATH}{Constants.PathConstants.MODULE_OUTPUT_MAIN}";

                case IOTypes.Module:
                    return rootPath + $"{Constants.PathConstants.DEBUG_PATH}{Constants.PathConstants.MODULE_OUTPUT_MODULE}";

                case IOTypes.None:
                    return rootPath + $"{Constants.PathConstants.DEBUG_PATH}{Constants.PathConstants.MODULE_OUTPUT_NONE}";
            }

            throw new Exception("Неизвестный тип вывода.");
        }
    }
}
