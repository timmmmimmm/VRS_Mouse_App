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

        public MainWindow()
        {
            InitializeComponent();
        
        }

        private void ExitButton_Click(object sender, RoutedEventArgs e)
        {
            System.Windows.Application.Current.Shutdown();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            ThreadPool.QueueUserWorkItem(_ =>
            {
                Dispatcher.BeginInvoke(new Action(() =>
                {
                    animateTextBlock(InfoTextBlock);
                    _animationFinished = true;
                }));
            });

            ThreadPool.QueueUserWorkItem(_ =>
            {
                Dispatcher.BeginInvoke(() =>
                {
                    while (!_animationFinished) { }
                    animateErrorState("sdadas");
                });
            });
        }

        private void animateTextBlock(TextBlock textBlock)
        {
            textBlock.Opacity = 0;
            textBlock.Text = "Looking for devices";
            textBlock.Visibility = Visibility.Visible;

            DoubleAnimation animation = new(1, TimeSpan.FromMilliseconds(200))
            {
                BeginTime = TimeSpan.FromMilliseconds(100)
            };
            textBlock.BeginAnimation(TextBlock.OpacityProperty, animation);
        }

        private void animateErrorState(string prompt)
        {
            InfoTextBlock.Text = prompt;
            LoadingSpinner.Visibility = Visibility.Collapsed;
            

            
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
