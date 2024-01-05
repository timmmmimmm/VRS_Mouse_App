using System;
using System.Net.Http;
using System.Threading;
using System.Windows;
using System.Windows.Threading;

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
        private readonly HttpClient _mouseClient;
        private const string MOUSE_CLIENT_URI = "http://localhost:12345/ma/api/all";
        private Thread ServerFinderThread;
        private int ticks;
        private short countdown;

        public MainWindow()
        {
            InitializeComponent();
            _animationHolder = new AnimationHolder(this);

            _mouseClient = App.GetMouseClient();

            _timer = new DispatcherTimer();
            _timerHandler = new EventHandler(OnTimerTick);
            ServerFinderThread = new(FindServerGetData);

            ticks = 0;
            countdown = 5;

        }

        private void ExitButton_Click(object sender, RoutedEventArgs e)
        {
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
                _mouseClient.CancelPendingRequests();
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
        private  async void FindServerGetData()
        {
            int sensitivity = 1, btn1Mode = 0, btn2Mode = 0;
            try
            {
                
                var response = await _mouseClient.GetStringAsync(MOUSE_CLIENT_URI);

                var roughValues = response.Split(',');

                var roughDPI = roughValues[0].Split(":");
                var roughBtn1 = roughValues[1].Split(":");
                var roughBtn2 = roughValues[2].Split(":");

                roughBtn2 = roughBtn2[1].Split("}");

                sensitivity = int.Parse(roughDPI[1]);
                btn1Mode = int.Parse(roughBtn1[1]);
                btn2Mode = int.Parse(roughBtn2[0]);
            }
            catch(HttpRequestException)
            {
                return;
            }

            Dispatcher.Invoke(new Action(() =>
            {
                var nextWindow = new MainMainWindow(_mouseClient, sensitivity, btn1Mode, btn2Mode);
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
