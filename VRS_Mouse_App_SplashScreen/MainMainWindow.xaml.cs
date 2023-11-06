using Microsoft.VisualBasic;
using System;
using System.Diagnostics;
using System.IO.Ports;
using System.Threading;
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
        private bool animationFinished = false;
        private int lastIndex = 0;

        public MainMainWindow()
        {
            AnimationDuration = new Duration(TimeSpan.FromMilliseconds(300));
            InitializeComponent();
            ChangeContent(new MouseInfoPanel(), ScrollDirection.Up);
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

        void SlideAnimation(UIElement newContent, UIElement oldContent, EventHandler? completedEventHndler, ScrollDirection scrollDirection)
        {
            double start = Canvas.GetBottom(oldContent);
            Canvas.SetBottom(newContent, scrollDirection == ScrollDirection.Up? -Height : Height);
            ScreenContainer.Children.Add(newContent);

            if(double.IsNaN(start))
            {
                start = 0;
            }

            DoubleAnimation outAnimation = CreateAnimation(start, scrollDirection == ScrollDirection.Up ? start + Height : start - Height, null);
            DoubleAnimation inAnimation = CreateAnimation(scrollDirection == ScrollDirection.Up ? start - Height: start + Height, start, completedEventHndler);
            oldContent.BeginAnimation(Canvas.BottomProperty, outAnimation);
            newContent.BeginAnimation(Canvas.BottomProperty, inAnimation);
        }

        public void ChangeContent(UIElement newContent, ScrollDirection scrollDirection)
        {
            if(ScreenContainer.Children.Count == 0)
            {
                ScreenContainer.Children.Add(newContent);
                return;
            }

            if(ScreenContainer.Children.Count == 1)
            {
                animationFinished = false;
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
                    animationFinished = true;
                };

                SlideAnimation(newContent, oldContent, onAnimationCompleted, scrollDirection);
            }

           
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
            var currentIndex = NavPanel.SelectedIndex;
            
            if(currentIndex != lastIndex)
            {
               var diff = currentIndex - lastIndex;
               var currentScrollDir = diff > 0 ? ScrollDirection.Down : ScrollDirection.Up;
                switch (currentIndex)
                {
                    case (int)NavPanelNames.MouseInfo:
                        if(Math.Abs(diff) > 1)
                        {
       
                            ChangeContent(new MouseSettingsPanel(),currentScrollDir);
                           
                            new Thread(() => {
                                while (!animationFinished) ;
                                Dispatcher.Invoke(() =>
                                {
    
                                    ChangeContent(new MouseInfoPanel(), currentScrollDir);
                                });
                            }).Start();
                            break;
                        }

                        ChangeContent(new MouseInfoPanel(), currentScrollDir);
                        break;
                    case (int)NavPanelNames.MouseSettings:
                        
                        new Thread(() => {
                            while (!animationFinished) ;
                            Dispatcher.Invoke(() =>
                            {
                                ChangeContent(new MouseSettingsPanel(), currentScrollDir);
                            });
                        }).Start();
                        break;
                    case (int)NavPanelNames.ButtonSettings:
                        if(Math.Abs(diff) > 1)
                        {
                            ChangeContent(new MouseSettingsPanel(), currentScrollDir);
    
                            new Thread(() => {
                                while (!animationFinished) ;
                                Dispatcher.Invoke(() =>
                            {
                                ChangeContent(new ButtonSettingsPanel(), currentScrollDir);
                            });
                            }).Start();
                            
                            break;
                        }
                        ChangeContent(new ButtonSettingsPanel(), currentScrollDir);
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
            
            lastIndex = currentIndex;
        }

        private void NavPanel_Loaded(object sender, RoutedEventArgs e)
        {
            NavPanel.SelectedIndex = 0;
        }

      
    }
}
