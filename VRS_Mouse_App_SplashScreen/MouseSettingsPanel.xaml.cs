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
        private double sliderVal;
        private readonly MainMainWindow? mainWindow;

        public MouseSettingsPanel()
        {
            sliderVal = 0.1;
            currentSliderValueStr = sliderVal.ToString();
            InitializeComponent();
            currentSliderValue.Text = currentSliderValueStr;
            //LoadingSpinner.Visibility = Visibility.Collapsed;
        }

        public MouseSettingsPanel(MainMainWindow mainMainWindow) : this()
        {
            mainWindow = mainMainWindow;
            sliderVal = Math.Round(mainMainWindow.GetMouseSensitivityValue(), 3);
            SensitivitySlider.Value = sliderVal * 10;
            currentSliderValueStr = (sliderVal).ToString();
            currentSliderValue.Text = currentSliderValueStr;
        }

        private void SensitivitySlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (currentSliderValue != null)
            {
                sliderVal = Math.Round(SensitivitySlider.Value / 10.0, 3);
                currentSliderValueStr = sliderVal.ToString();
                currentSliderValue.Text = currentSliderValueStr;
            }

        }

        private void SyncButton_Click(object sender, RoutedEventArgs e)
        {
            
            mainWindow?.SendSensitivity(sliderVal);
            new Thread(aa).Start();

           
        }

        private void aa()
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
                LoadingSpinner.Visibility= Visibility.Collapsed;
                SyncButton.Visibility = Visibility.Visible;
                statusText.Text = "Sync";
            }));
        }

    }
}
