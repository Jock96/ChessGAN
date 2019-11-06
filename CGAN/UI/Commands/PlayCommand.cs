namespace UI.Commands
{
    using System.Windows;

    using UI.ViewModels;

    /// <summary>
    /// Команда инициализации игры.
    /// </summary>
    public class PlayCommand : BaseTCommand<MainWindowVM>
    {
        /// <summary>
        /// Выполнить.
        /// </summary>
        /// <param name="parameter">Параметр.</param>
        protected override void Execute(MainWindowVM parameter)
        {
            MessageBox.Show("Команда не реализована!",
                "Внимание", MessageBoxButton.OK, MessageBoxImage.Information);
        }
    }
}
