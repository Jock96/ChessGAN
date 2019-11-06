namespace BL.Models.FigurePieces
{
    using BL.Enums;

    /// <summary>
    /// Модель ферзя.
    /// </summary>
    public class Queen : Piece
    {
        /// <summary>
        /// Модель ферзя.
        /// </summary>
        /// <param name="color">Цвет фигуры.</param>
        public Queen(PieceColorType color) : base(color)
        {
        }

        /// <summary>
        /// Тип фигуры.
        /// </summary>
        public override PieceTypes PieceType { get; protected set; } = PieceTypes.Queen;
    }
}
