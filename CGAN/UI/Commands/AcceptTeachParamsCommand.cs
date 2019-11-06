namespace UI.Commands
{
    using UI.ViewModels;

    using System.Windows;

    /// <summary>
    /// Команла принятия параметров.
    /// </summary>
    public class AcceptTeachParamsCommand : BaseTCommand<TeachWindowVM>
    {
        /// <summary>
        /// Выполнить.
        /// </summary>
        /// <param name="parameter">Параметр.</param>
        protected override void Execute(TeachWindowVM parameter)
        {
            var iterationCountString = parameter.IterationCount;

            if(!int.TryParse(iterationCountString, out var iterationCount))
            {
                MessageBox.Show("Задан неверный формат данных", "Ошибка",
                    MessageBoxButton.OK, MessageBoxImage.Error);

                parameter.IterationCount = string.Empty;
                return;
            }

            // TODO: убрать после реализации.
            var result = MessageBox.Show("Команда не реализована!",
                "Внимание", MessageBoxButton.OK, MessageBoxImage.Information);

            if (result.Equals(MessageBoxResult.OK))
                parameter.CloseOwnerWindow();
        }
    }
}
