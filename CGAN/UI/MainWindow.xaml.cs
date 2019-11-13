namespace UI
{
    using System;
    using System.Diagnostics;
    using System.Linq;
    using System.Runtime.InteropServices;
    using System.Threading.Tasks;
    using System.Windows;

    using BL.Constants;

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
            CloseModule(new object(), new EventArgs());

            InitializeComponent();
            DataContext = new MainWindowVM();

            Closed += CloseModule;
            Process.GetCurrentProcess().Exited += CloseModule;
        }

        //[DllImport("kernel32.dll", SetLastError = true)]
        //[return: MarshalAs(UnmanagedType.Bool)]
        //private static extern bool AllocConsole();

        //[DllImport("kernel32.dll", SetLastError = true)]
        //[return: MarshalAs(UnmanagedType.Bool)]
        //private static extern bool FreeConsole();

        /// <summary>
        /// Закрывает открытые модули при выходе.
        /// </summary>
        /// <param name="sender">Отправитель.</param>
        /// <param name="e">Аргументы.</param>
        private void CloseModule(object sender, EventArgs e) =>
            Process.GetProcesses().ToList().FindAll(process =>
            string.Equals(process.ProcessName, PathConstants.MODULE_NAME))?.
            ForEach(process => process.Kill());
    }
}
