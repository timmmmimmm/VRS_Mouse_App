using System;
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
    /// Interaction logic for MouseSettingsPanel.xaml
    /// </summary>
    public partial class MouseSettingsPanel : UserControl
    {
        private string? currentSliderValueStr;
        private int sliderVal;
        private readonly MainMainWindow? mainWindow;

        public MouseSettingsPanel()
        {
            sliderVal = 1;
            currentSliderValueStr = sliderVal.ToString();
            InitializeComponent();
            currentSliderValue.Text = currentSliderValueStr;
            //LoadingSpinner.Visibility = Visibility.Collapsed;
        }

        public MouseSettingsPanel(MainMainWindow mainMainWindow) : this()
        {
            mainWindow = mainMainWindow;
            sliderVal = mainMainWindow.MouseSensitivityValue;
            SensitivitySlider.Value = sliderVal;
            currentSliderValueStr = (sliderVal).ToString();
            currentSliderValue.Text = currentSliderValueStr;
        }

        private void SensitivitySlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (currentSliderValue != null)
            {
                sliderVal = (int) Math.Round(SensitivitySlider.Value);
                currentSliderValueStr = sliderVal.ToString();
                currentSliderValue.Text = currentSliderValueStr;
            }

        }

        private void SyncButton_Click(object sender, RoutedEventArgs e)
        {
            new Thread(SendData).Start();  
        }

        private void SendData()
        {
            Dispatcher.Invoke(new Action(() =>
            {
                SyncButton.Visibility = Visibility.Collapsed;
                LoadingSpinner.Visibility = Visibility.Visible;
                statusText.Text = "Syncing with mouse";
                
                if(mainWindow != null)
                {
                    mainWindow.ExitButton.IsEnabled = false;
                    mainWindow.NavPanel.IsHitTestVisible = false;
                }
            }));

            mainWindow?.SendSensitivity(sliderVal);
            Thread.Sleep(5000);
            Dispatcher.Invoke(new Action(() =>
            {
                LoadingSpinner.Visibility= Visibility.Collapsed;
                SyncButton.Visibility = Visibility.Visible;
                statusText.Text = "Sync";
                if (mainWindow != null)
                {
                    mainWindow.ExitButton.IsEnabled = true;
                    mainWindow.NavPanel.IsHitTestVisible = true;
                }
            }));
        }

    }
}
