namespace BL.Models.FigurePieces
{
    using BL.Enums;

    /// <summary>
    /// Модель пешки.
    /// </summary>
    public class Pawn : Piece
    {
        /// <summary>
        /// Модель пешки.
        /// </summary>
        /// <param name="color">Цвет фигуры.</param>
        public Pawn(PieceColorType color) : base(color)
        {
        }

        /// <summary>
        /// Тип фигуры.
        /// </summary>
        public override PieceTypes PieceType { get; protected set; } = PieceTypes.Pawn;
    }
}
