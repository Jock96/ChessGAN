namespace UI.ViewModels
{
    using UI.Commands;
    using UI.Windows;

    /// <summary>
    /// Вью-модель окна обучения.
    /// </summary>
    public class TeachWindowVM : BaseVM
    {
        /// <summary>
        /// Вью-модель окна обучения.
        /// </summary>
        /// <param name="teachWindow">Визуальное представление окна.</param>
        public TeachWindowVM(TeachWindow teachWindow)
        {
            _ownerWindow = teachWindow;
            AcceptTeachParamsCommand = new AcceptTeachParamsCommand();
        }

        public AcceptTeachParamsCommand AcceptTeachParamsCommand { get; set; }

        /// <summary>
        /// Визуальное представление окна.
        /// </summary>
        private TeachWindow _ownerWindow;

        /// <summary>
        /// Количество итераций.
        /// </summary>
        private string _iterationCount = string.Empty;

        /// <summary>
        /// Количество итераций.
        /// </summary>
        public string IterationCount
        {
            get => _iterationCount;
            set
            {
                _iterationCount = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Закрывает окно, которое владеет вью-моделью.
        /// </summary>
        public void CloseOwnerWindow() => _ownerWindow.Close();
    }
}
