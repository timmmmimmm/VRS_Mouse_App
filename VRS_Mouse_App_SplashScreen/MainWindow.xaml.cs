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
        private Storyboard retryStoryboard;
        private DoubleAnimation retryAnimation;

        public MainWindow()
        {
            InitializeComponent();
            retryStoryboard = new Storyboard
            {
                Duration = new Duration(TimeSpan.FromMilliseconds(300))
            };

            retryAnimation = new DoubleAnimation()
            {
                From = 0,
                To = 360,
                Duration = new Duration(TimeSpan.FromMilliseconds(100)),
                RepeatBehavior = new RepeatBehavior(3)
            };

            Storyboard.SetTarget(retryAnimation, RetryButton);
            Storyboard.SetTargetProperty(retryAnimation, new PropertyPath("(UIElement.RenderTransform).(RotateTransform.Angle)"));

            retryStoryboard.Children.Add(retryAnimation);

            Resources.Add("RetryStoryboard", retryStoryboard);
        }

        private void ExitButton_Click(object sender, RoutedEventArgs e)
        {
            System.Windows.Application.Current.Shutdown();
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
                RetryButton.Visibility = Visibility.Visible;   
            }));

            this.Dispatcher.Invoke(new Action(() => {
                ((Storyboard)Resources["RetryStoryboard"]).Begin(); 
            }));
        }

        private void findPort()
        {
           var portNames = SerialPort.GetPortNames();
            
            if(portNames.Count() == 0)
            {
                
            }
        }
    }
}
