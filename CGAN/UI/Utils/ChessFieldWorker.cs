namespace UI.Utils
{
    using System.Windows.Media;

    using UI.ViewModels;

    /// <summary>
    /// Класс по работе с игровым полем.
    /// </summary>
    public static class ChessFieldWorker
    {
        /// <summary>
        /// Переместить фигуры.
        /// </summary>
        /// <param name="turn">Ход.</param>
        /// <param name="control">Вью-модель.</param>
        public static void SwapImages(string turn, ChessControlVM control)
        {
            var normilizeTurn = turn.ToUpper();

            var firstPosition = normilizeTurn.Remove(2);
            var secondPosition = normilizeTurn.Replace(firstPosition, string.Empty);

            var firstFieldProperty = control.GetType().GetProperty(firstPosition);
            var firstFieldValue = (ImageSource)firstFieldProperty.GetValue(control);

            ImageSource newValue;

            if (firstFieldValue != null)
                newValue = firstFieldValue.CloneCurrentValue();
            else
                newValue = null;

            control.GetType().GetProperty(secondPosition).SetValue(control, newValue);
            firstFieldProperty.SetValue(control, null);
        }
    }
}
