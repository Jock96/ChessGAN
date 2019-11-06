namespace UI.Windows
{
    using System.Windows;

    using UI.ViewModels;

    /// <summary>
    /// Логика взаимодействия для TeachWindow.xaml
    /// </summary>
    public partial class TeachWindow : Window
    {
        public TeachWindow()
        {
            InitializeComponent();
            DataContext = new TeachWindowVM(this);
        }
    }
}
