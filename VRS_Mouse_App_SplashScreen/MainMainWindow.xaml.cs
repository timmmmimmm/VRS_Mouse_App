using Microsoft.VisualBasic;
using System;
using System.Diagnostics;
using System.IO.Ports;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace VRS_Mouse_App_SplashScreen
{
    /// <summary>
    /// Interaction logic for MainMainWindow.xaml
    /// </summary>
    public partial class MainMainWindow : Window
    {
        private readonly SerialPort? MousePort;

        public MainMainWindow()
        {
            InitializeComponent();
        }

        public MainMainWindow(SerialPort port) : this()
        {
            MousePort = port;

        }

        private void ExitButton_Copy_Click(object sender, RoutedEventArgs e)
        {
            WindowState = WindowState.Minimized;
        }

        private void ExitButton_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {

        }

        private void DragRectangle_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Pressed)
            {
                this.DragMove();
            }
        }

        private void MouseInfoButton_Click(object sender, RoutedEventArgs e)
        {
            Trace.WriteLine(NavPanel.Items.Count.ToString());
         
        }

        private void MouseOptions_Click(object sender, RoutedEventArgs e)
        {
            Trace.WriteLine(NavPanel.SelectedIndex.ToString());
        }

        private void NavPanel_PreviewMouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            Trace.WriteLine(NavPanel.SelectedIndex.ToString());
            if(!(NavPanel.SelectedIndex == 0))
            {
                var currentItem = NavPanel.SelectedItem;
                if (currentItem != null)
                {
                    NavPanel.Items.Remove(currentItem);
                    NavPanel.Items.Insert(0, currentItem);
                }
                NavPanel.UpdateLayout();
            }
        }
    }
}
