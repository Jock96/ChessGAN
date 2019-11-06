namespace BL.Models.FigurePieces
{
    using BL.Enums;

    /// <summary>
    /// Модель коня.
    /// </summary>
    public class Knight : Piece
    {
        /// <summary>
        /// Модель коня.
        /// </summary>
        /// <param name="color">Цвет фигуры.</param>
        public Knight(PieceColorType color) : base(color)
        {
        }

        /// <summary>
        /// Тип фигуры.
        /// </summary>
        public override PieceTypes PieceType { get; protected set; } = PieceTypes.Knight;
    }
}
