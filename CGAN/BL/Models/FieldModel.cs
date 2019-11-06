namespace BL.Models
{
    using BL.Models.FigurePieces;
    using BL.Enums;
    using BL.Constants;

    using System;

    /// <summary>
    /// Модель поля.
    /// </summary>
    public class FieldModel
    {
        /// <summary>
        /// Текущая ситуация на поле.
        /// </summary>
        public Piece[,] CurrentPositions { get; private set; }
            = new Piece[8, 8] { 
                {new Rook(PieceColorType.Black),
                 new Knight(PieceColorType.Black),
                 new Bishop(PieceColorType.Black),
                 new Queen(PieceColorType.Black),
                 new King(PieceColorType.Black),
                 new Bishop(PieceColorType.Black),
                 new Knight(PieceColorType.Black),
                 new Rook(PieceColorType.Black) },
                {null,null,null,null,null,null,null,null },
                {null,null,null,null,null,null,null,null },
                {null,null,null,null,null,null,null,null },
                {null,null,null,null,null,null,null,null },
                {null,null,null,null,null,null,null,null },
                {null,null,null,null,null,null,null,null },
                {new Rook(PieceColorType.White),
                 new Knight(PieceColorType.White),
                 new Bishop(PieceColorType.White),
                 new Queen(PieceColorType.White),
                 new King(PieceColorType.White),
                 new Bishop(PieceColorType.White),
                 new Knight(PieceColorType.White),
                 new Rook(PieceColorType.White) } };

        /// <summary>
        /// Сделать ход.
        /// </summary>
        /// <param name="moveParameter">Параметр хода.</param>
        /// <returns>Возвращает флаг произошёл ли ход.</returns>
        public bool Move(string moveParameter)
        {
            var separatorIndex = moveParameter.IndexOf(FieldModelConstants.SEPARATOR);
            var currentPosition = moveParameter.Remove(separatorIndex);

            var futurePosition = moveParameter.Replace(
                $"{currentPosition}{FieldModelConstants.SEPARATOR}", string.Empty);

            Piece piece;

            try
            {
                piece = TryGetElement(currentPosition);
            }
            catch
            {
                return false;
                throw;
            }

            if (piece == null)
                return false;

            // Получение возможных вариантов хода. 
            switch (piece.PieceType)
            {
                case PieceTypes.King:
                    break;

                case PieceTypes.Queen:
                    break;

                case PieceTypes.Bishop:
                    break;

                case PieceTypes.Rook:
                    break;

                case PieceTypes.Knight:
                    break;

                case PieceTypes.Pawn:
                    break;
            }
        }

        /// <summary>
        /// Получить элемент по переданной позиции.
        /// </summary>
        /// <param name="currentPosition">Переданная позиция.</param>
        /// <returns>Возвращает фигуру.</returns>
        private Piece TryGetElement(string currentPosition)
        {
            if (!FieldModelConstants.TryGetIndexFromMatrix(currentPosition, out var x, out var y))
                throw new Exception("Не удалось найти позицию элемента!");

            if (!x.HasValue || !y.HasValue)
                throw new Exception("Одна из позиций оказалась Null.");

            return CurrentPositions[(int)x, (int)y];
        }
    }
}
