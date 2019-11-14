namespace BL.Constants
{
    using System.Collections.Generic;

    /// <summary>
    /// Класс констант сообщений.
    /// </summary>
    public static class MessageConstants
    {
        /// <summary>
        /// Список ошибок.
        /// </summary>
        public static readonly List<string> ERROR_MESSAGES = new List<string>
        { PARSE_ERROR, WRONG_POSITION };

        /// <summary>
        /// Удача при выполнении.
        /// </summary>
        public const string OK = "OK";

        /// <summary>
        /// Неудача при преобразовании цифры позиции.
        /// </summary>
        public const string PARSE_ERROR = "Неудача при преобразовании цифры позиции";

        /// <summary>
        /// Неверная позиция.
        /// </summary>
        public const string WRONG_POSITION = "Неверная позиция";

        /// <summary>
        /// Таймаут.
        /// </summary>
        public const string TIMEOUT = "Timeout";

        /// <summary>
        /// Положительный ответ.
        /// </summary>
        public const string GOOD_RESPONCE = "OK";

        /// <summary>
        /// Отрицательный ответ.
        /// </summary>
        public const string BAD_RESPONCE = "Negative";

        /// <summary>
        /// Сообщение о неизвестной команде.
        /// </summary>
        public const string UNKNOWN = "Unknown command";

        /// <summary>
        /// Сообщение о поставленном мате.
        /// </summary>
        public const string MATES = "mates";

        /// <summary>
        /// Чёрные.
        /// </summary>
        public const string BLACK = "Black";

        /// <summary>
        /// Белые.
        /// </summary>
        public const string WHITE = "White";

        /// <summary>
        /// Поражение.
        /// </summary>
        public const string LOOSE = "Вы проиграли.";

        /// <summary>
        /// Победа.
        /// </summary>
        public const string WIN = "Вы победили.";
    }
}
