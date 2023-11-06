using Microsoft.VisualBasic;
using System;
using System.Diagnostics;
using System.IO.Ports;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media.Animation;

namespace VRS_Mouse_App_SplashScreen
{
    /// <summary>
    /// Interaction logic for MainMainWindow.xaml
    /// </summary>
    public partial class MainMainWindow : Window
    {
        private readonly SerialPort? MousePort;
        private readonly Duration AnimationDuration;

        public MainMainWindow()
        {
            AnimationDuration = new Duration(TimeSpan.FromMilliseconds(300));
            InitializeComponent();
            ChangeContent(new MouseInfoPanel());
        }

        public MainMainWindow(SerialPort port) : this()
        {
            MousePort = port;
        }

        DoubleAnimation CreateAnimation(double from, double to, EventHandler? completedEventaHandler)
        {
            DoubleAnimation animation = new(from,to,AnimationDuration);
            if(completedEventaHandler != null)
            {
                animation.Completed += completedEventaHandler;
            }
            return animation;
        }

        void SlideAnimation(UIElement newContent, UIElement oldContent, EventHandler? completedEventHndler)
        {
            double start = Canvas.GetBottom(oldContent);
            Canvas.SetBottom(newContent, start - oldContent.RenderSize.Height);
            ScreenContainer.Children.Add(newContent);

            if(double.IsNaN(start))
            {
                start = oldContent.RenderSize.Height;
            }

            DoubleAnimation outAnimation = CreateAnimation(start, start - oldContent.RenderSize.Height,null);
            DoubleAnimation inAnimation = CreateAnimation(start + oldContent.RenderSize.Height, start, completedEventHndler);
            oldContent.BeginAnimation(Canvas.BottomProperty, outAnimation);
            newContent.BeginAnimation(Canvas.BottomProperty, inAnimation);
        }

        public void ChangeContent(UIElement newContent)
        {
            ScreenContainer.IsHitTestVisible = false;
            var oldContent = ScreenContainer.Children[0];
            EventHandler onAnimationCompleted = delegate (object? sender, EventArgs e)
            {
                ScreenContainer.IsHitTestVisible = true;
                if (oldContent != null)
                {
                    ScreenContainer.Children.Remove(oldContent);
                }

                oldContent = null;
            };

            SlideAnimation(newContent,oldContent, onAnimationCompleted);
        }

        private void MinimizeButton_Click(object sender, RoutedEventArgs e)
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
                DragMove();
            }
        }


        private void NavPanel_PreviewMouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            Trace.WriteLine(NavPanel.SelectedIndex.ToString());

            var currentIndex = NavPanel.SelectedIndex;
            switch (currentIndex)
            {
                case (int)NavPanelNames.MouseInfo:

                    break;
                case (int)NavPanelNames.MouseSettings: 
                    break;
                case (int)NavPanelNames.ButtonSettings: 
                    break;
            }
            var currentItem = NavPanel.Items.GetItemAt(currentIndex);
            if (currentIndex != -1)
            {
                NavPanel.Items.RemoveAt(currentIndex);
                NavPanel.Items.Insert(currentIndex, currentItem);
            }
            NavPanel.UpdateLayout();
            NavPanel.SelectedIndex = currentIndex;
        }

        private void NavPanel_Loaded(object sender, RoutedEventArgs e)
        {
            NavPanel.SelectedIndex = 0;
        }

      
    }
}
