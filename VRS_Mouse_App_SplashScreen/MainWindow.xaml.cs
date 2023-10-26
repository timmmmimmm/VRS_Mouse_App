using System;
using System.Collections.Generic;
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
        private bool _animationFinished = false;
        private  SerialPort? mousePort { get; set; }

        public MainWindow()
        {
            InitializeComponent();
            InitializeTimer();
            createRetryStoryboard();
            createLoadingSpinnerFadeStoryboard();
        }

        private void ExitButton_Click(object sender, RoutedEventArgs e)
        {
            System.Windows.Application.Current.Shutdown();
        }

        private void OnRetryClick(object sender, RoutedEventArgs e)
        {

        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {  
           new Thread(smthn).Start();
        }

        private void smthn()
        {
            Thread.Sleep(2000);
            
            this.Dispatcher.Invoke(new Action(() => 
            {
                LoadingSpinner.Opacity = 0;
            }));

            this.Dispatcher.Invoke(new Action(() => {
                ((Storyboard)Resources["SpinnerFadeStoryboard"]).Begin(); 
            }));
        }

        private void findPort()
        {
           var portNames = SerialPort.GetPortNames();
            
            if(portNames.Count() == 0)
            {
                
            }
        }

        private void InitializeTimer()
        {
            DispatcherTimer timer = new DispatcherTimer();
            timer.Tick += new EventHandler(onTimerTick);
            timer.Interval = new TimeSpan(0, 0, 0, 0, 500);
            timer.Start();
        }

        private void onTimerTick(object ?sender, EventArgs e)
        {

        }

        private void createRetryStoryboard()
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

        private void createLoadingSpinnerFadeStoryboard()
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
    }
}
