namespace UI.Commands
{
    using UI.ViewModels;

    /// <summary>
    /// Отправить параметры хода.
    /// </summary>
    public class SendMoveParametersCommand : BaseTCommand<MainWindowVM>
    {
        /// <summary>
        /// Выполнить.
        /// </summary>
        /// <param name="parameter">Параметр.</param>
        protected override void Execute(MainWindowVM parameter)
        {
            var input = parameter.Module.StandardInput;
            input.Write('a');
        }
    }
}
