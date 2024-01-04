using System;
using System.IO.Ports;
using System.Threading;
using System.Windows;
using System.Windows.Threading;
using VRS_Mouse_App_SplashScreen.MouseAPIClient;

namespace VRS_Mouse_App_SplashScreen
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private readonly AnimationHolder _animationHolder;
        private readonly DispatcherTimer _timer;
        private readonly EventHandler _timerHandler;
        private readonly MouseService _mouseService;
        private Thread ServerFinderThread;
        private int ticks;
        private short countdown;
        private bool shutdown;

        public MainWindow()
        {
            InitializeComponent();
            _animationHolder = new AnimationHolder(this);

            _mouseService = new(App.GetMouseClient());

            _timer = new DispatcherTimer();
            _timerHandler = new EventHandler(OnTimerTick);
            ServerFinderThread = new(FindServerGetData);

            ticks = 0;
            countdown = 5;
            shutdown = false;

        }

        private void ExitButton_Click(object sender, RoutedEventArgs e)
        {
            shutdown = true;
            if (ServerFinderThread.IsAlive)
            {
                ServerFinderThread.Join();
            }

            System.Windows.Application.Current.Shutdown();
        }

        private void OnRetryClick(object sender, RoutedEventArgs e)
        {
            AnimateLoadingSpinner();
            countdown = 5;
            if (ServerFinderThread.IsAlive)
            {
                ServerFinderThread.Join();
            }
            ServerFinderThread = new Thread(FindServerGetData);
            ServerFinderThread.Start();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            InitializeTimer();
            ServerFinderThread.Start();
        }

        /// <summary>
        /// A method that searches all available ports to find a device that transits a specified verifier
        /// </summary>
        private async void FindServerGetData()
        {
            //Thread.Sleep(2000);

            var response = await _mouseService.GetMouseInfoAsync();

           

            Dispatcher.Invoke(new Action(() =>
            {
                var nextWindow = new MainMainWindow(_mouseService);
                App.Current.MainWindow = nextWindow;
                nextWindow.Show();
                this.Close();
            }));

        }

        private void InitializeTimer()
        {
            _timer.Tick += _timerHandler;
            _timer.Interval = new TimeSpan(0, 0, 0, 0, 1000);
            _timer.Start();
        }

        private void OnTimerTick(object? sender, EventArgs e)
        {
            ticks++;

            if (ticks > 5)
            {
                if (countdown > 0)
                {
                    InfoTextBlock.Text = (this.FindResource(ResourceNames.SplashScreen.InfoBoxStrings.DEFAULT) as string) +
                                         "\n ending in " + countdown.ToString() + "s";
                    countdown--;
                }
                else if (countdown == 0)
                {
                    AnimateRetryButton(ResourceNames.SplashScreen.InfoBoxStrings.DEVICE_NOT_FOUND);
                }
            }

        }



        private void AnimateRetryButton(string resourceName)
        {
            _animationHolder.HideLoadingSpinnerSpinRetryButtonThreeTimes();
            InfoTextBlock.Text = (this.FindResource(resourceName) as string);
            _timer.Stop();
            ticks = 0;
        }

        private void AnimateLoadingSpinner()
        {
            _animationHolder.HideRetryButtonFadeOutLoadnigSpinner();
            InfoTextBlock.Text = (this.FindResource(ResourceNames.SplashScreen.InfoBoxStrings.DEFAULT) as string);
            _timer.Start();
        }

    }
}
