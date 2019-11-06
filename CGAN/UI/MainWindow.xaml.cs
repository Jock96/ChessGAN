namespace UI
{
    using System.Windows;

    using UI.ViewModels;

    /// <summary>
    /// Код-бехайнд главного окна.
    /// </summary>
    public partial class MainWindow : Window
    {
        /// <summary>
        /// Точка входа.
        /// </summary>
        public MainWindow()
        {
            InitializeComponent();
            DataContext = new MainWindowVM();
        }
    }
}
