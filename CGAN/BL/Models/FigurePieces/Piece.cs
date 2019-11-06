namespace BL.Models.FigurePieces
{
    using BL.Enums;

    /// <summary>
    /// Модель фигуры.
    /// </summary>
    public abstract class Piece
    {
        /// <summary>
        /// Тип фигуры.
        /// </summary>
        public abstract PieceTypes PieceType { get; protected set; }

        /// <summary>
        /// Цвет фигуры.
        /// </summary>
        public PieceColorType PieceColor { get; private set; }

        /// <summary>
        /// Конструктор фигуры.
        /// </summary>
        /// <param name="color">Цвет фигуры.</param>
        public Piece(PieceColorType color)
        {
            PieceColor = color;
        }
    }
}
