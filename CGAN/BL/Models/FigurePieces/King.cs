namespace BL.Models.FigurePieces
{
    using BL.Enums;

    /// <summary>
    /// Модель короля.
    /// </summary>
    public class King : Piece
    {
        /// <summary>
        /// Модель короля.
        /// </summary>
        /// <param name="color">Цвет фигуры.</param>
        public King(PieceColorType color) : base(color)
        {
        }

        /// <summary>
        /// Тип фигуры.
        /// </summary>
        public override PieceTypes PieceType { get; protected set; } = PieceTypes.King;
    }
}
