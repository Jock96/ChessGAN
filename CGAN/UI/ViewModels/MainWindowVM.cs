namespace UI.ViewModels
{
    using UI.Commands;

    /// <summary>
    /// Вью-модель окна.
    /// </summary>
    public class MainWindowVM : BaseVM
    {
        /// <summary>
        /// Вью-модель окна.
        /// </summary>
        public MainWindowVM()
        {
            TeachCommand = new TeachCommand();
            PlayCommand = new PlayCommand();
        }

        /// <summary>
        /// Команда обучения.
        /// </summary>
        public TeachCommand TeachCommand { get; set; }

        /// <summary>
        /// Команда инициализаци иигры.
        /// </summary>
        public PlayCommand PlayCommand { get; set; }
    }
}
