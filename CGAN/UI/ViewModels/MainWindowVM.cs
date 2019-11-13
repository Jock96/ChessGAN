namespace UI.ViewModels
{
    using System.Diagnostics;

    using UI.Commands;
    using UI.Utils;

    using BL.Constants;

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
            SendMoveParametersCommand = new SendMoveParametersCommand();

            ChessControlVM = new ChessControlVM();
            ChessControlVM.ModuleCallback += ModuleTurnHandler;
        }

        /// <summary>
        /// Обработчик хода модуля.
        /// </summary>
        /// <param name="sender">Отправитель.</param>
        /// <param name="e">Аргумент.</param>
        private void ModuleTurnHandler(object sender, System.EventArgs e)
        {
            var moduleTurn = sender.ToString();

            if (moduleTurn.Contains(MessageConstants.UNKNOWN))
                return;

            var gamerTurn = ChessControlVM.Turn;
            ChessFieldWorker.SwapImages(gamerTurn, ChessControlVM);
            System.Threading.Thread.Sleep(500);
            ChessFieldWorker.SwapImages(moduleTurn, ChessControlVM);
        }

        /// <summary>
        /// Вью-модель контрола.
        /// </summary>
        public ChessControlVM ChessControlVM { get; set; }

        /// <summary>
        /// Процесс модуля.
        /// </summary>
        public Process Module { get; set; }

        /// <summary>
        /// Отправить параметры хода.
        /// </summary>
        public SendMoveParametersCommand SendMoveParametersCommand { get; set; }

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
