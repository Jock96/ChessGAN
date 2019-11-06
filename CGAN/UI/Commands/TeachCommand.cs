namespace UI.Commands
{
    using UI.ViewModels;
    using UI.Windows;

    /// <summary>
    /// Коанда обучения.
    /// </summary>
    public class TeachCommand : BaseTCommand<MainWindowVM>
    {
        /// <summary>
        /// Выполнить.
        /// </summary>
        /// <param name="parameter">Параметр.</param>
        protected override void Execute(MainWindowVM parameter)
        {
            new TeachWindow().Show();
        }
    }
}
