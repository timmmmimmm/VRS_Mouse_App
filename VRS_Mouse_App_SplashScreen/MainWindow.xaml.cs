using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Threading;
using static System.Net.Mime.MediaTypeNames;

namespace VRS_Mouse_App_SplashScreen
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window 
    {
        private const string DEVICE_VERIFIER = "FFFF", APP_VERIFIER = "GGGG", DEVICE_ACK = "NNNN";
        private SerialPort? MousePort { get; set; }
        private readonly DispatcherTimer timer;
        private readonly EventHandler timerHandler;
        private readonly Thread PortFinderThread;
        private int ticks;
        private static short countdown;
        private static bool _shutdown;

        public MainWindow()
        {
            InitializeComponent();
            timer = new DispatcherTimer();
            timerHandler = new EventHandler(OnTimerTick);
            PortFinderThread = new(FindPort);
  
            ticks = 0;
            countdown = 5;
            _shutdown = false;
            CreateRetryStoryboard();
            CreateLoadingSpinnerFadeStoryboard();
        }

        private void ExitButton_Click(object sender, RoutedEventArgs e)
        {
            _shutdown = true;
            if(PortFinderThread.IsAlive)
            {
                PortFinderThread.Join();
            }
          ;
            System.Windows.Application.Current.Shutdown();
        }

        private void OnRetryClick(object sender, RoutedEventArgs e)
        {
            AnimateLoadingSpinner();
            countdown = 5;
            new Thread(FindPort).Start();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {  
            InitializeTimer();
            PortFinderThread.Start();
        }


        private void FindPort()
        {
            Thread.Sleep(1000);
            var portNames = SerialPort.GetPortNames();
            bool portFound = false;

            if (portNames.Length == 0)
            {
                this.Dispatcher.Invoke(new Action(() => { 
                    AnimateRetryButton("InfoTextPromptNoDevices"); }));
                return;
            }

            while (!portFound && !_shutdown && countdown!=0)
            {

                foreach (var port in portNames)
                {
                    if (countdown == 0 || _shutdown)
                        return;

                    try
                    {
                        MousePort = new(port)
                        {
                            ReadTimeout = 100,
                            BaudRate = 115200

                        };
                        MousePort.Open();
                        if(MousePort.IsOpen)
                        {
                            string verificator = MousePort.ReadLine();
                            if (verificator.Equals(DEVICE_VERIFIER))
                            {
                                portFound = true;

                                Dispatcher.Invoke(new Action(() =>
                                {
                                    timer.Stop();
                                    InfoTextBlock.Text = FindResource("InfoTextDeviceFound") as string;
                                }));

                                MousePort.ReadTimeout = -1;

                                for (short j = 0; j < 20; j++)
                                {
                                    MousePort.WriteLine(APP_VERIFIER);
                                }
                                //TODO: Fetch current mouse state
                                break;
                            }

                            MousePort.Close();
                        }
                        
                    }
                    catch (IOException)
                    {
                        continue;
                    }
                    catch (UnauthorizedAccessException)
                    {
                        continue;
                    }
                    catch (ArgumentOutOfRangeException)
                    {
                        continue;
                    }
                    catch (TimeoutException)
                    {
                        continue;
                    }
                }
            }

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

        private void OnTimerTick(object ?sender, EventArgs e)
        {
            ticks++;

           if(ticks > 5) 
            {
                if(countdown > 0)
                {
                    InfoTextBlock.Text = (this.FindResource("DefaultInfoTextPrompt") as string) + "\n ending in " + countdown.ToString() + "s";
                    countdown--;
                }
                else if (countdown == 0 )
                {
                    AnimateRetryButton("InfoTextPromptDeviceNotFound");
                }
            }

        }

        private void CreateRetryStoryboard()
        {
            var retryStoryboard = new Storyboard
            {
                Duration = new Duration(TimeSpan.FromMilliseconds(600))
            };

            var retryAnimation = new DoubleAnimation()
            {
                From = 0,
                To = 360,
                Duration = new Duration(TimeSpan.FromMilliseconds(200)),
                RepeatBehavior = new RepeatBehavior(3)
            };

            Storyboard.SetTarget(retryAnimation, RetryButton);
            Storyboard.SetTargetProperty(retryAnimation, new PropertyPath("(UIElement.RenderTransform).(RotateTransform.Angle)"));

            retryStoryboard.Children.Add(retryAnimation);

            Resources.Add("RetryStoryboard", retryStoryboard);
        }

        private void CreateLoadingSpinnerFadeStoryboard()
        {
            var spinnerFadeStoryboard = new Storyboard();

            var spinnerFadeAnimation = new DoubleAnimation()
            {
                From = 0,
                To = 1,
                Duration = new Duration(TimeSpan.FromMilliseconds(1000))
            };

            Storyboard.SetTarget(spinnerFadeAnimation, LoadingSpinner);
            Storyboard.SetTargetProperty(spinnerFadeAnimation, new PropertyPath("Opacity"));

            spinnerFadeStoryboard.Children.Add(spinnerFadeAnimation);

            Resources.Add("SpinnerFadeStoryboard", spinnerFadeStoryboard);
        }

        private void AnimateRetryButton(string resourceName)
        {
            LoadingSpinner.Visibility = Visibility.Collapsed;
            RetryButton.Visibility = Visibility.Visible;
            StartStoryboard("RetryStoryboard");
            InfoTextBlock.Text = (this.FindResource(resourceName) as string);
            timer.Stop();
            ticks = 0;
        }

        private void AnimateLoadingSpinner()
        {
            RetryButton.Visibility = Visibility.Collapsed;
            LoadingSpinner.Opacity = 0;
            LoadingSpinner.Visibility = Visibility.Visible;
            StartStoryboard("SpinnerFadeStoryboard");
            InfoTextBlock.Text = (this.FindResource("DefaultInfoTextPrompt") as string);
            timer.Start();
        }

        private void StartStoryboard(string storyboardName)
        {
            ((Storyboard)Resources[storyboardName]).Begin();
        }
    }
}
