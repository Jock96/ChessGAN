namespace UI.Commands
{
    using BL.Helpers;
    using BL.Extensions;

    using System;
    using System.Diagnostics;
    using System.Reflection;
    using System.Windows;
    using System.Linq;

    using UI.ViewModels;
    using System.Runtime.InteropServices;
    using System.IO;
    using BL.Enums;
    using BL.Constants;
    using System.Drawing;

    /// <summary>
    /// Команда инициализации игры.
    /// </summary>
    public class PlayCommand : BaseTCommand<MainWindowVM>
    {
        /// <summary>
        /// Выполнить.
        /// </summary>
        /// <param name="parameter">Параметр.</param>
        protected override void Execute(MainWindowVM parameter)
        {
            var debugPath = PathHelper.GetDebugPath();
            var modulePath = PathHelper.GetDebugModulePath();

            var process = new Process
            {
                StartInfo = new ProcessStartInfo
                {
                    FileName = modulePath,
                    UseShellExecute = true,
                    WorkingDirectory = debugPath
                }
            };

            process.Start();
            parameter.Module = process;

            var resources = PathHelper.GetResourcesPath();

            CreatePawns(parameter, resources);
            CreateRooks(parameter, resources);
            CreateKnights(parameter, resources);
            CreateBishops(parameter, resources);
            CreateKingsAndQueens(parameter, resources);

            parameter.ChessControlVM.IsGameStart = true;
        }

        #region Расположение фигур

        /// <summary>
        /// Создание каждого короля и ферзя.
        /// </summary>
        private static void CreateKingsAndQueens(MainWindowVM parameter, string resources)
        {
            var bitmap = new Bitmap($"{resources}{PiecesPathConstants.WHITE_QUEEN}");
            parameter.ChessControlVM.D1 = bitmap.ConvertToImageSource();

            bitmap = new Bitmap($"{resources}{PiecesPathConstants.WHITE_KING}");
            parameter.ChessControlVM.E1 = bitmap.ConvertToImageSource();

            bitmap = new Bitmap($"{resources}{PiecesPathConstants.BLACK_QUEEN}");
            parameter.ChessControlVM.D8 = bitmap.ConvertToImageSource();

            bitmap = new Bitmap($"{resources}{PiecesPathConstants.BLACK_KING}");
            parameter.ChessControlVM.E8 = bitmap.ConvertToImageSource();
        }

        /// <summary>
        /// Создание каждого слона.
        /// </summary>
        private static void CreateBishops(MainWindowVM parameter, string resources)
        {
            var bitmap = new Bitmap($"{resources}{PiecesPathConstants.WHITE_BISHOP}");
            parameter.ChessControlVM.C1 = bitmap.ConvertToImageSource();

            bitmap = new Bitmap($"{resources}{PiecesPathConstants.WHITE_BISHOP}");
            parameter.ChessControlVM.F1 = bitmap.ConvertToImageSource();

            bitmap = new Bitmap($"{resources}{PiecesPathConstants.BLACK_BISHOP}");
            parameter.ChessControlVM.C8 = bitmap.ConvertToImageSource();

            bitmap = new Bitmap($"{resources}{PiecesPathConstants.BLACK_BISHOP}");
            parameter.ChessControlVM.F8 = bitmap.ConvertToImageSource();
        }

        /// <summary>
        /// Создание каждого коня.
        /// </summary>
        private static void CreateKnights(MainWindowVM parameter, string resources)
        {
            var bitmap = new Bitmap($"{resources}{PiecesPathConstants.WHITE_KNIGHT}");
            parameter.ChessControlVM.B1 = bitmap.ConvertToImageSource();

            bitmap = new Bitmap($"{resources}{PiecesPathConstants.WHITE_KNIGHT}");
            parameter.ChessControlVM.G1 = bitmap.ConvertToImageSource();

            bitmap = new Bitmap($"{resources}{PiecesPathConstants.BLACK_KNIGHT}");
            parameter.ChessControlVM.B8 = bitmap.ConvertToImageSource();

            bitmap = new Bitmap($"{resources}{PiecesPathConstants.BLACK_KNIGHT}");
            parameter.ChessControlVM.G8 = bitmap.ConvertToImageSource();
        }

        /// <summary>
        /// Создание каждой ладьи.
        /// </summary>
        private static void CreateRooks(MainWindowVM parameter, string resources)
        {
            var bitmap = new Bitmap($"{resources}{PiecesPathConstants.WHITE_ROOK}");
            parameter.ChessControlVM.A1 = bitmap.ConvertToImageSource();

            bitmap = new Bitmap($"{resources}{PiecesPathConstants.WHITE_ROOK}");
            parameter.ChessControlVM.H1 = bitmap.ConvertToImageSource();

            bitmap = new Bitmap($"{resources}{PiecesPathConstants.BLACK_ROOK}");
            parameter.ChessControlVM.A8 = bitmap.ConvertToImageSource();

            bitmap = new Bitmap($"{resources}{PiecesPathConstants.BLACK_ROOK}");
            parameter.ChessControlVM.H8 = bitmap.ConvertToImageSource();
        }

        /// <summary>
        /// Создать пешки.
        /// </summary>
        private static void CreatePawns(MainWindowVM parameter, string resources)
        {
            var bitmap = new Bitmap($"{resources}{PiecesPathConstants.WHITE_PAWN}");
            parameter.ChessControlVM.A2 = bitmap.ConvertToImageSource();

            bitmap = new Bitmap($"{resources}{PiecesPathConstants.BLACK_PAWN}");
            parameter.ChessControlVM.A7 = bitmap.ConvertToImageSource();

            bitmap = new Bitmap($"{resources}{PiecesPathConstants.WHITE_PAWN}");
            parameter.ChessControlVM.B2 = bitmap.ConvertToImageSource();

            bitmap = new Bitmap($"{resources}{PiecesPathConstants.BLACK_PAWN}");
            parameter.ChessControlVM.B7 = bitmap.ConvertToImageSource();

            bitmap = new Bitmap($"{resources}{PiecesPathConstants.WHITE_PAWN}");
            parameter.ChessControlVM.C2 = bitmap.ConvertToImageSource();

            bitmap = new Bitmap($"{resources}{PiecesPathConstants.BLACK_PAWN}");
            parameter.ChessControlVM.C7 = bitmap.ConvertToImageSource();

            bitmap = new Bitmap($"{resources}{PiecesPathConstants.WHITE_PAWN}");
            parameter.ChessControlVM.D2 = bitmap.ConvertToImageSource();

            bitmap = new Bitmap($"{resources}{PiecesPathConstants.BLACK_PAWN}");
            parameter.ChessControlVM.D7 = bitmap.ConvertToImageSource();

            bitmap = new Bitmap($"{resources}{PiecesPathConstants.WHITE_PAWN}");
            parameter.ChessControlVM.E2 = bitmap.ConvertToImageSource();

            bitmap = new Bitmap($"{resources}{PiecesPathConstants.BLACK_PAWN}");
            parameter.ChessControlVM.E7 = bitmap.ConvertToImageSource();

            bitmap = new Bitmap($"{resources}{PiecesPathConstants.WHITE_PAWN}");
            parameter.ChessControlVM.F2 = bitmap.ConvertToImageSource();

            bitmap = new Bitmap($"{resources}{PiecesPathConstants.BLACK_PAWN}");
            parameter.ChessControlVM.F7 = bitmap.ConvertToImageSource();

            bitmap = new Bitmap($"{resources}{PiecesPathConstants.WHITE_PAWN}");
            parameter.ChessControlVM.G2 = bitmap.ConvertToImageSource();

            bitmap = new Bitmap($"{resources}{PiecesPathConstants.BLACK_PAWN}");
            parameter.ChessControlVM.G7 = bitmap.ConvertToImageSource();

            bitmap = new Bitmap($"{resources}{PiecesPathConstants.WHITE_PAWN}");
            parameter.ChessControlVM.H2 = bitmap.ConvertToImageSource();

            bitmap = new Bitmap($"{resources}{PiecesPathConstants.BLACK_PAWN}");
            parameter.ChessControlVM.H7 = bitmap.ConvertToImageSource();
        }

        #endregion

        #region Работа с потоком.

        [Flags]
        public enum ThreadAccess : int
        {
            TERMINATE = (0x0001),
            SUSPEND_RESUME = (0x0002),
            GET_CONTEXT = (0x0008),
            SET_CONTEXT = (0x0010),
            SET_INFORMATION = (0x0020),
            QUERY_INFORMATION = (0x0040),
            SET_THREAD_TOKEN = (0x0080),
            IMPERSONATE = (0x0100),
            DIRECT_IMPERSONATION = (0x0200)
        }

        [DllImport("kernel32.dll")]
        static extern IntPtr OpenThread(ThreadAccess dwDesiredAccess, bool bInheritHandle, uint dwThreadId);
        [DllImport("kernel32.dll")]
        static extern uint SuspendThread(IntPtr hThread);
        [DllImport("kernel32.dll")]
        static extern int ResumeThread(IntPtr hThread);
        [DllImport("kernel32", CharSet = CharSet.Auto, SetLastError = true)]
        static extern bool CloseHandle(IntPtr handle);


        private static void SuspendProcess(int pid)
        {
            var process = Process.GetProcessById(pid);

            if (process.ProcessName == string.Empty)
                return;

            foreach (ProcessThread pT in process.Threads)
            {
                IntPtr pOpenThread = OpenThread(ThreadAccess.SUSPEND_RESUME, false, (uint)pT.Id);

                if (pOpenThread == IntPtr.Zero)
                {
                    continue;
                }

                SuspendThread(pOpenThread);

                CloseHandle(pOpenThread);
            }
        }

        public static void ResumeProcess(int pid)
        {
            var process = Process.GetProcessById(pid);

            if (process.ProcessName == string.Empty)
                return;

            foreach (ProcessThread pT in process.Threads)
            {
                IntPtr pOpenThread = OpenThread(ThreadAccess.SUSPEND_RESUME, false, (uint)pT.Id);

                if (pOpenThread == IntPtr.Zero)
                {
                    continue;
                }

                var suspendCount = 0;
                do
                {
                    suspendCount = ResumeThread(pOpenThread);
                } while (suspendCount > 0);

                CloseHandle(pOpenThread);
            }
        }
    }

    #endregion

}
