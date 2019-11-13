namespace UI.Commands
{
    using System;
    using System.Windows.Controls;

    /// <summary>
    /// Проверка хода.
    /// </summary>
    public class CheckTurnCommand : BaseCommand
    {
        /// <summary>
        /// Событие изменения хода.
        /// </summary>
        public event EventHandler TurnChanged;

        /// <summary>
        /// Выполнить.
        /// </summary>
        /// <param name="parameter">Параметр.</param>
        public override void Execute(object parameter)
        {
            if (!(parameter is Image))
                return;

            var image = (Image)parameter;
            TurnChanged.Invoke(image.Name, null);
        }
    }
}
