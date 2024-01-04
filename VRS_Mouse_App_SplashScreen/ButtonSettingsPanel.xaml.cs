﻿using System;
using System.Collections.Generic;
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
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace VRS_Mouse_App_SplashScreen
{
    /// <summary>
    /// Interaction logic for ButtonSettingsPanel.xaml
    /// </summary>
    public partial class ButtonSettingsPanel : UserControl
    {
        private int btn1ModeSelected, btn2ModeSelected;
        private readonly MainMainWindow ?mainWindow;

        public ButtonSettingsPanel()
        {
            InitializeComponent();
        }

        public ButtonSettingsPanel(MainMainWindow mainMainWindow) : this()
        {
            mainWindow = mainMainWindow;

            ComboBoxButton1.SelectedIndex = mainWindow.Btn1Mode;
            ComboBoxButton2.SelectedIndex = mainWindow.Btn2Mode;
        }

       

        private void ComboBoxButton1_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
           switch (ComboBoxButton1.SelectedIndex)
            {
                case (int) ButtonActions.None:
                    btn1ModeSelected = (int)ButtonActions.None;
                    break;

                case (int) ButtonActions.Home: 
                    btn1ModeSelected = (int)ButtonActions.Home;
                    break;
            }
        }

        private void ComboBoxButton2_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            switch (ComboBoxButton2.SelectedIndex)
            {
                case (int)ButtonActions.None:
                    btn2ModeSelected = (int)ButtonActions.None;
                    break;

                case (int)ButtonActions.Home:
                    btn2ModeSelected = (int)ButtonActions.Home;
                    break;
            }
        }

        private void SyncButton_Click(object sender, RoutedEventArgs e)
        {
            mainWindow?.SendButtonModes(btn1ModeSelected, btn2ModeSelected);
            new Thread(SendData).Start();
        }

        private void SendData()
        {
            Dispatcher.Invoke(new Action(() =>
            {
                SyncButton.Visibility = Visibility.Collapsed;
                LoadingSpinner.Visibility = Visibility.Visible;
                statusText.Text = "Syncing with mouse";
            }));

            Thread.Sleep(1000);
            //TODO: Server comms

            Dispatcher.Invoke(new Action(() =>
            {
                LoadingSpinner.Visibility = Visibility.Collapsed;
                SyncButton.Visibility = Visibility.Visible;
                statusText.Text = "Sync";
            }));
        }
    }
}
