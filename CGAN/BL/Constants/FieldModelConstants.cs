namespace BL.Constants
{
    using System;
    using System.Collections.Generic;

    /// <summary>
    /// Константы модели поля.
    /// </summary>
    public static class FieldModelConstants
    {
        /// <summary>
        /// Строковое обозначение ячеек поля.
        /// </summary>
        public static readonly string[,] FIELD = 
            new string[MATRIX_SIZE, MATRIX_SIZE] 
            { { "A8", "B8", "C8", "D8", "E8", "F8", "G8", "H8" },
              { "A7", "B7", "C7", "D7", "E7", "F7", "G7", "H7" },
              { "A6", "B6", "C6", "D6", "E6", "F6", "G6", "H6" },
              { "A5", "B5", "C5", "D5", "E5", "F5", "G5", "H5" },
              { "A4", "B4", "C4", "D4", "E4", "F4", "G4", "H4" },
              { "A3", "B3", "C3", "D3", "E3", "F3", "G3", "H3" },
              { "A2", "B2", "C2", "D2", "E2", "F2", "G2", "H2" },
              { "A1", "B1", "C1", "D1", "E1", "F1", "G1", "H1" }};

        /// <summary>
        /// Парсинг цифр в нумерацию.
        /// </summary>
        public static readonly Dictionary<string, int> SYMBOL_TO_NUMBER_DICTIONARY = new Dictionary<string, int>
            { { "A", 1}, { "B", 2}, { "C", 3}, { "D", 4}, { "E", 5}, { "F", 6}, { "G", 7}, { "H", 8} };

        /// <summary>
        /// Валидные номера позиций.
        /// </summary>
        public static readonly List<int> VALID_NUMBERS = new List<int> { 1, 2, 3, 4, 5, 6, 7, 8 };

        /// <summary>
        /// Размер матрицы поля.
        /// </summary>
        public const int MATRIX_SIZE = 8;

        /// <summary>
        /// Разделитель параметра хода.
        /// </summary>
        public const string SEPARATOR = "|";

        /// <summary>
        /// Попытка получить позицию элемента в матрице.
        /// </summary>
        /// <param name="element">Элемента.</param>
        /// <param name="x">Позиция по X.</param>
        /// <param name="y">Позиция по Y.</param>
        /// <returns>Возвращает флаг удачности выполнения.</returns>
        public static bool TryGetIndexFromMatrix(string element, out int? x, out int? y)
        {
            x = null;
            y = null;

            for (var xIndex = 0; xIndex < MATRIX_SIZE; ++xIndex)
                for (var yIndex = 0; yIndex < MATRIX_SIZE; ++yIndex)
                {
                    if (!string.Equals(element, FIELD[xIndex, yIndex],
                        StringComparison.InvariantCultureIgnoreCase))
                        continue;

                    x = xIndex;
                    y = yIndex;

                    return true;
                }

            return false;
        }
    }
}
