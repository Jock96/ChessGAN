namespace UI.ViewModels
{
    using System.Diagnostics;

    using UI.Commands;
    using UI.Utils;

    using BL.Constants;
    using System;
    using BL.Helpers;
    using System.IO;

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
        /// Сообщение.
        /// </summary>
        private string _message;

        /// <summary>
        /// Сообщение.
        /// </summary>
        public string Message
        {
            get => _message;
            set
            {
                _message = value;
                OnPropertyChanged();
            }
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

            if (OnMate())
                return;

            var gamerTurn = ChessControlVM.Turn;
            ChessFieldWorker.SwapImages(gamerTurn, ChessControlVM);

            System.Threading.Thread.Sleep(500);
            ChessFieldWorker.SwapImages(moduleTurn, ChessControlVM);
        }

        /// <summary>
        /// Проверить мат.
        /// </summary>
        private bool OnMate()
        {
            var debugPath = PathHelper.GetDebugPath();

            var blackWinFile = $"{debugPath}{PathConstants.BLACK_WIN}";

            if (File.Exists(blackWinFile))
            {
                KilAll(blackWinFile);
                Message = MessageConstants.LOOSE;

                return true;
            }

            var whiteWinFile = $"{debugPath}{PathConstants.WHITE_WIN}";

            if (File.Exists(whiteWinFile))
            {
                KilAll(whiteWinFile);
                Message = MessageConstants.WIN;

                return true;
            }

            return false;
        }

        /// <summary>
        /// Уничтожить всё.
        /// </summary>
        private void KilAll(string file)
        {
            Module.Kill();

            var input = PathHelper.GetDebugInputPathByType(BL.Enums.IOTypes.Module);
            var output = PathHelper.GetDebugOutputPathByType(BL.Enums.IOTypes.Module);

            File.Delete(file);
            File.Delete(input);
            File.Delete(output);
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
