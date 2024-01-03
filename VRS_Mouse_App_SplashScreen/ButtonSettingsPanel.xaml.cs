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
    /// Interaction logic for ButtonSettingsPanel.xaml
    /// </summary>
    public partial class ButtonSettingsPanel : UserControl
    {
        private string btn1ModeSelected;
        public ButtonSettingsPanel()
        {
            InitializeComponent();
        }

        private void SyncButton_Click(object sender, RoutedEventArgs e)
        {

        }

        private void ComboBoxButton1_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
           
        }

        private void ComboBoxButton2_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

        }
    }
}
