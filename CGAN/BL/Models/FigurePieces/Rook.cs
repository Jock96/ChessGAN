namespace BL.Models.FigurePieces
{
    using BL.Enums;

    /// <summary>
    /// Модель ладьи.
    /// </summary>
    public class Rook : Piece
    {
        /// <summary>
        /// Модель ладьи.
        /// </summary>
        /// <param name="color">Цвет фигуры.</param>
        public Rook(PieceColorType color) : base(color)
        {
        }

        /// <summary>
        /// Тип фигуры.
        /// </summary>
        public override PieceTypes PieceType { get; protected set; } = PieceTypes.Rook;
    }
}
