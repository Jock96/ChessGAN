namespace UI.ViewModels
{
    using BL.Utils;
    using System;
    using System.Windows;
    using System.Windows.Media;
    using UI.Commands;

    /// <summary>
    /// Вью-модель контролла шахмат.
    /// </summary>
    public class ChessControlVM : BaseVM
    {
        /// <summary>
        /// Событие изменения хода.
        /// </summary>
        public event EventHandler ModuleCallback;

        /// <summary>
        /// Вью-модель контролла шахмат.
        /// </summary>
        public ChessControlVM()
        {
            CheckTurnCommand = new CheckTurnCommand();
            CheckTurnCommand.TurnChanged += OnTurnChanged;
        }

        /// <summary>
        /// Игра началась?
        /// </summary>
        private bool _isGameStart;

        /// <summary>
        /// Игра началась?
        /// </summary>
        public bool IsGameStart
        {
            get => _isGameStart;
            set
            {
                _isGameStart = value;
                OnPropertyChanged();
            }
        }

        /// <summary>
        /// Обработка изменения хода.
        /// </summary>
        /// <param name="sender">Отправитель.</param>
        /// <param name="e">Аргументы.</param>
        private void OnTurnChanged(object sender, System.EventArgs e)
        {
            if (!_isGameStart)
                return;

            var value = sender.ToString();

            if (string.Equals(value, Turn, 
                System.StringComparison.InvariantCultureIgnoreCase))
            {
                Turn = string.Empty;
                return;
            }

            if (Turn.Length < 4)
            {
                Turn += value;

                if (Turn.Length.Equals(4))
                {
                    if(!GameManager.SendTurnMessage(Turn, out var callback, out var responce))
                    {
                        // TODO: Обработать повторную отправку.
                        MessageBox.Show($"{responce}", "Ошибка", MessageBoxButton.OK, MessageBoxImage.Error);
                        return;
                    }

                    ModuleCallback.Invoke(callback, null);
                }
            }
            else
            {
                Turn = value;
            }
        }

        /// <summary>
        /// Проверить ход.
        /// </summary>
        public CheckTurnCommand CheckTurnCommand { get; set; }

        /// <summary>
        /// Ход.
        /// </summary>
        public string Turn { get; set; } = string.Empty;

        #region Изображения на доске

        #region A(1-8)

        private ImageSource _a1;
        private ImageSource _a2;
        private ImageSource _a3;
        private ImageSource _a4;
        private ImageSource _a5;
        private ImageSource _a6;
        private ImageSource _a7;
        private ImageSource _a8;

        public ImageSource A1
        {
            get => _a1;
            set
            {
                _a1 = value;
                OnPropertyChanged();
            }
        }
        public ImageSource A2
        {
            get => _a2;
            set
            {
                _a2 = value;
                OnPropertyChanged();
            }
        }
        public ImageSource A3
        {
            get => _a3;
            set
            {
                _a3 = value;
                OnPropertyChanged();
            }
        }
        public ImageSource A4
        {
            get => _a4;
            set
            {
                _a4 = value;
                OnPropertyChanged();
            }
        }
        public ImageSource A5
        {
            get => _a5;
            set
            {
                _a5 = value;
                OnPropertyChanged();
            }
        }
        public ImageSource A6
        {
            get => _a6;
            set
            {
                _a6 = value;
                OnPropertyChanged();
            }
        }
        public ImageSource A7
        {
            get => _a7;
            set
            {
                _a7 = value;
                OnPropertyChanged();
            }
        }
        public ImageSource A8
        {
            get => _a8;
            set
            {
                _a8 = value;
                OnPropertyChanged();
            }
        }
        #endregion

        #region B(1-8)

        private ImageSource _b1;
        private ImageSource _b2;
        private ImageSource _b3;
        private ImageSource _b4;
        private ImageSource _b5;
        private ImageSource _b6;
        private ImageSource _b7;
        private ImageSource _b8;

        public ImageSource B1
        {
            get => _b1;
            set
            {
                _b1 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource B2
        {
            get => _b2;
            set
            {
                _b2 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource B3
        {
            get => _b3;
            set
            {
                _b3 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource B4
        {
            get => _b4;
            set
            {
                _b4 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource B5
        {
            get => _b5;
            set
            {
                _b5 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource B6
        {
            get => _b6;
            set
            {
                _b6 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource B7
        {
            get => _b7;
            set
            {
                _b7 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource B8
        {
            get => _b8;
            set
            {
                _b8 = value;
                OnPropertyChanged();
            }
        }

        #endregion

        #region C(1-8)

        private ImageSource _c1;
        private ImageSource _c2;
        private ImageSource _c3;
        private ImageSource _c4;
        private ImageSource _c5;
        private ImageSource _c6;
        private ImageSource _c7;
        private ImageSource _c8;

        public ImageSource C1
        {
            get => _c1;
            set
            {
                _c1 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource C2
        {
            get => _c2;
            set
            {
                _c2 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource C3
        {
            get => _c3;
            set
            {
                _c3 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource C4
        {
            get => _c4;
            set
            {
                _c4 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource C5
        {
            get => _c5;
            set
            {
                _c5 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource C6
        {
            get => _c6;
            set
            {
                _c6 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource C7
        {
            get => _c7;
            set
            {
                _c7 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource C8
        {
            get => _c8;
            set
            {
                _c8 = value;
                OnPropertyChanged();
            }
        }

        #endregion

        #region D(1-8)

        private ImageSource _d1;
        private ImageSource _d2;
        private ImageSource _d3;
        private ImageSource _d4;
        private ImageSource _d5;
        private ImageSource _d6;
        private ImageSource _d7;
        private ImageSource _d8;

        public ImageSource D1
        {
            get => _d1;
            set
            {
                _d1 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource D2
        {
            get => _d2;
            set
            {
                _d2 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource D3
        {
            get => _d3;
            set
            {
                _d3 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource D4
        {
            get => _d4;
            set
            {
                _d4 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource D5
        {
            get => _d5;
            set
            {
                _d5 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource D6
        {
            get => _d6;
            set
            {
                _d6 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource D7
        {
            get => _d7;
            set
            {
                _d7 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource D8
        {
            get => _d8;
            set
            {
                _d8 = value;
                OnPropertyChanged();
            }
        }

        #endregion

        #region E(1-8)

        private ImageSource _e1;
        private ImageSource _e2;
        private ImageSource _e3;
        private ImageSource _e4;
        private ImageSource _e5;
        private ImageSource _e6;
        private ImageSource _e7;
        private ImageSource _e8;

        public ImageSource E1
        {
            get => _e1;
            set
            {
                _e1 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource E2
        {
            get => _e2;
            set
            {
                _e2 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource E3
        {
            get => _e3;
            set
            {
                _e3 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource E4
        {
            get => _e4;
            set
            {
                _e4 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource E5
        {
            get => _e5;
            set
            {
                _e5 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource E6
        {
            get => _e6;
            set
            {
                _e6 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource E7
        {
            get => _e7;
            set
            {
                _e7 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource E8
        {
            get => _e8;
            set
            {
                _e8 = value;
                OnPropertyChanged();
            }
        }

        #endregion

        #region F(1-8)

        private ImageSource _f1;
        private ImageSource _f2;
        private ImageSource _f3;
        private ImageSource _f4;
        private ImageSource _f5;
        private ImageSource _f6;
        private ImageSource _f7;
        private ImageSource _f8;

        public ImageSource F1
        {
            get => _f1;
            set
            {
                _f1 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource F2
        {
            get => _f2;
            set
            {
                _f2 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource F3
        {
            get => _f3;
            set
            {
                _f3 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource F4
        {
            get => _f4;
            set
            {
                _f4 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource F5
        {
            get => _f5;
            set
            {
                _f5 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource F6
        {
            get => _f6;
            set
            {
                _f6 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource F7
        {
            get => _f7;
            set
            {
                _f7 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource F8
        {
            get => _f8;
            set
            {
                _f8 = value;
                OnPropertyChanged();
            }
        }

        #endregion

        #region G(1-8)

        private ImageSource _g1;
        private ImageSource _g2;
        private ImageSource _g3;
        private ImageSource _g4;
        private ImageSource _g5;
        private ImageSource _g6;
        private ImageSource _g7;
        private ImageSource _g8;

        public ImageSource G1
        {
            get => _g1;
            set
            {
                _g1 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource G2
        {
            get => _g2;
            set
            {
                _g2 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource G3
        {
            get => _g3;
            set
            {
                _g3 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource G4
        {
            get => _g4;
            set
            {
                _g4 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource G5
        {
            get => _g5;
            set
            {
                _g5 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource G6
        {
            get => _g6;
            set
            {
                _g6 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource G7
        {
            get => _g7;
            set
            {
                _g7 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource G8
        {
            get => _g8;
            set
            {
                _g8 = value;
                OnPropertyChanged();
            }
        }

        #endregion

        #region H(1-8)

        private ImageSource _h1;
        private ImageSource _h2;
        private ImageSource _h3;
        private ImageSource _h4;
        private ImageSource _h5;
        private ImageSource _h6;
        private ImageSource _h7;
        private ImageSource _h8;

        public ImageSource H1
        {
            get => _h1;
            set
            {
                _h1 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource H2
        {
            get => _h2;
            set
            {
                _h2 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource H3
        {
            get => _h3;
            set
            {
                _h3 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource H4
        {
            get => _h4;
            set
            {
                _h4 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource H5
        {
            get => _h5;
            set
            {
                _h5 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource H6
        {
            get => _h6;
            set
            {
                _h6 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource H7
        {
            get => _h7;
            set
            {
                _h7 = value;
                OnPropertyChanged();
            }
        }

        public ImageSource H8
        {
            get => _h8;
            set
            {
                _h8 = value;
                OnPropertyChanged();
            }
        }

        #endregion

        #endregion
    }
}
