namespace BL.Models.FigurePieces
{
    using BL.Enums;

    /// <summary>
    /// Модель слона.
    /// </summary>
    public class Bishop : Piece
    {
        /// <summary>
        /// Модель слона.
        /// </summary>
        /// <param name="color">Цвет фигуры.</param>
        public Bishop(PieceColorType color) : base(color)
        {
        }

        /// <summary>
        /// Тип фигуры.
        /// </summary>
        public override PieceTypes PieceType { get; protected set; } = PieceTypes.Bishop;
    }
}
