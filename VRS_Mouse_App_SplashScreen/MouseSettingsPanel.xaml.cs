using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
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
        private string ?currentSliderValueStr;
        public MouseSettingsPanel()
        {
            currentSliderValueStr = (0.1).ToString(); 
            InitializeComponent();
            currentSliderValue.Text = currentSliderValueStr;
        }

        private void SensitivitySlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if(currentSliderValue != null)
            {
                currentSliderValueStr = Math.Round(SensitivitySlider.Value / 10.0, 3).ToString();
                currentSliderValue.Text = currentSliderValueStr;
            }
            
        }
    }
}
