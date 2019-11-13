namespace BL.Constants
{
    /// <summary>
    /// Класс констант для работы с директориями.
    /// </summary>
    public static class PathConstants
    {
        /// <summary>
        /// Директория ресурсов.
        /// </summary>
        public const string RESOURCES_PATH = "\\Resources";

        /// <summary>
        /// Директори модуля.
        /// </summary>
        public const string MODULE_PATH = "\\Module";

        /// <summary>
        /// Путь модуля в бинарниках.
        /// </summary>
        public static readonly string MODULE_BIN_PATH = $"\\M1\\bin\\{MODULE_NAME}{EXTENSION}";

        /// <summary>
        /// Имя модуля.
        /// </summary>
        public const string MODULE_NAME = "GreKo-Windows-64";

        /// <summary>
        /// Имя дебаг модуля.
        /// </summary>
        public const string DEBUG_MODULE_NAME = "GreKo";

        /// <summary>
        /// Дебаг директория
        /// </summary>
        public const string DEBUG_PATH = "\\Debug\\";

        /// <summary>
        /// Ввод в модуль (не инициализированный).
        /// </summary>
        public const string MODULE_INPUT_NONE = "Input(None).txt";

        /// <summary>
        /// Вывод из модуля (не инициализированный).
        /// </summary>
        public const string MODULE_OUTPUT_NONE = "Output(None).txt";

        /// <summary>
        /// Ввод в модуль (из окна).
        /// </summary>
        public const string MODULE_INPUT_MAIN = "Input(Main).txt";

        /// <summary>
        /// Вывод из модуля (из окна).
        /// </summary>
        public const string MODULE_OUTPUT_MAIN = "Output(Main).txt";

        /// <summary>
        /// Ввод в модуль (из модуля).
        /// </summary>
        public const string MODULE_INPUT_MODULE = "Input(Module).txt";

        /// <summary>
        /// Вывод из модуля (из модуля).
        /// </summary>
        public const string MODULE_OUTPUT_MODULE = "Output(Module).txt";

        /// <summary>
        /// Расширение модуля.
        /// </summary>
        public const string EXTENSION = ".exe";
    }
}
