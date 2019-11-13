namespace BL.Behaviors
{
    using BL.Constants;
    using BL.Models.FigurePieces;

    using System.Linq;
    using System.Collections.Generic;

    /// <summary>
    /// Класс поведения для фигур.
    /// </summary>
    public static class PieceBehavior
    {
        public static string KingMoveVariants(string currentPosition, Piece[,] currentPiecePositions)
        {
            // TODO: Попробоват другой вариант.

            return string.Empty;
            if (!TryGetPositionAndNumbere(currentPosition, out var symbol, out var number, out var message))
                return message;

            if (!TryGetNumberBySymbol(symbol, out var numberBySymbol, out var errorMessage))
                return errorMessage;

            var kingStep = 1;

            var up = $"{symbol}{number + kingStep}";
            var down = $"{symbol}{number - kingStep}";

            var leftByNumber = numberBySymbol - kingStep;
            var rightByNumber = numberBySymbol + kingStep;

            var moveVariants = new List<string>();
        }

        /// <summary>
        /// Получить цифровое представление символа.
        /// </summary>
        /// <param name="symbol">Символ.</param>
        /// <param name="number">Цифра.</param>
        /// <param name="message">Сообщение.</param>
        /// <returns>Возвращает удачность выполнения.</returns>
        private static bool TryGetNumberBySymbol(string symbol, out int number, out string message)
        {
            number = -1;
            message = MessageConstants.OK;

            var numberBySymbol = FieldModelConstants.SYMBOL_TO_NUMBER_DICTIONARY.Where(pair => 
            string.Equals(pair.Key, symbol, System.StringComparison.InvariantCultureIgnoreCase));

            if (!numberBySymbol.Any())
            {
                message = MessageConstants.WRONG_POSITION;
                return false;
            }

            number = numberBySymbol.First().Value;
            return true;
        }

        /// <summary>
        /// Попытка получить разделённую позицию (символ/номер).
        /// </summary>
        /// <param name="currentPosition">Текущая позиция.</param>
        /// <param name="symbol">Символ.</param>
        /// <param name="number">Номер.</param>
        /// <param name="message">Сообщение выполнения.</param>
        /// <returns>Возвращает удачность выполнения.</returns>
        private static bool TryGetPositionAndNumbere(string currentPosition, out string symbol, out int number, out string message)
        {
            symbol = currentPosition.Remove(1);
            message = MessageConstants.OK;
            number = -1;

            if (!int.TryParse(currentPosition.Replace(symbol, string.Empty), out var parseNumber))
            {
                message = MessageConstants.PARSE_ERROR;
                return false;
            }

            if (!FieldModelConstants.VALID_NUMBERS.Contains(parseNumber))
            {
                message = MessageConstants.WRONG_POSITION;
                return false;
            }

            number = parseNumber;
            return true;
        }
    }
}
