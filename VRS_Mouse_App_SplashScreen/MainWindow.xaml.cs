using System;
using System.IO.Ports;
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
        private readonly DispatcherTimer timer;
        private readonly EventHandler timerHandler;
        private Thread ServerFinderThread;
        private int ticks;
        private short countdown;
        private bool shutdown;

        public MainWindow()
        {
            InitializeComponent();
            _animationHolder = new AnimationHolder(this);


            timer = new DispatcherTimer();
            timerHandler = new EventHandler(OnTimerTick);
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
        private void FindServerGetData()
        {
            Thread.Sleep(2000);
            //bool portFound = false;


            //while (!portFound)
            //{
            //    if(shutdown || countdown == 0)
            //    {
            //        return;
            //    }

            //    var portNames = SerialPort.GetPortNames();

            //    if (portNames.Length == 0)
            //    {
            //        this.Dispatcher.Invoke(new Action(() => {
            //            AnimateRetryButton(ResourceNames.SplashScreen.InfoBoxStrings.NO_DEVICES);
            //        }));
            //        return;
            //    }


            //    foreach (var port in portNames)
            //    {
            //        if (countdown == 0 || shutdown)
            //            return;

            //        try
            //        {
            //            MousePort = new(port)
            //            {
            //                ReadTimeout = 100,
            //                BaudRate = 115200

            //            };
            //            MousePort.Open();
            //            Thread.Sleep(200);

            //            if(MousePort.IsOpen)
            //            {
            //                string verificator = MousePort.ReadLine();
            //                if (verificator.Equals(DEVICE_VERIFIER))
            //                {
            //                    portFound = true;

            //                    Dispatcher.Invoke(new Action(() =>
            //                    {
            //                        timer.Stop();
            //                        InfoTextBlock.Text = FindResource(ResourceNames.SplashScreen.InfoBoxStrings.DEVICE_FOUND) as string;
            //                    }));

            //                    MousePort.ReadTimeout = -1;

            //                    for (short j = 0; j < 20; j++)
            //                    {
            //                        MousePort.WriteLine(APP_VERIFIER);
            //                    }
            //                    //TODO: Fetch current mouse state
            //                    break;
            //                }

            //                MousePort.Close();
            //            }

            //        }
            //        catch (IOException)
            //        {
            //            continue;
            //        }
            //        catch (UnauthorizedAccessException)
            //        {
            //            continue;
            //        }
            //        catch (ArgumentOutOfRangeException)
            //        {
            //            continue;
            //        }
            //        catch (TimeoutException)
            //        {
            //            continue;
            //        }
            //    }
            //}

            Dispatcher.Invoke(new Action(() =>
            {
                var nextWindow = new MainMainWindow();
                App.Current.MainWindow = nextWindow;
                nextWindow.Show();
                this.Close();
            }));

        }

        private void InitializeTimer()
        {
            timer.Tick += timerHandler;
            timer.Interval = new TimeSpan(0, 0, 0, 0, 1000);
            timer.Start();
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
            timer.Stop();
            ticks = 0;
        }

        private void AnimateLoadingSpinner()
        {
            _animationHolder.HideRetryButtonFadeOutLoadnigSpinner();
            InfoTextBlock.Text = (this.FindResource(ResourceNames.SplashScreen.InfoBoxStrings.DEFAULT) as string);
            timer.Start();
        }

    }
}
