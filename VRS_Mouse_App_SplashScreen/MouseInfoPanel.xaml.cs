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
    /// Interaction logic for MouseInfoPanel.xaml
    /// </summary>
    public partial class MouseInfoPanel : UserControl
    {
        public MouseInfoPanel()
        {
            InitializeComponent();
        }

        public MouseInfoPanel(MainMainWindow mainWindow) : this()
        {
            TextBlockSensitivity.Text = mainWindow.MouseSensitivityValue.ToString();


            switch (mainWindow.Btn1Mode)
            {
                case (int)ButtonActions.None:
                    TextBoxButton1Mode.Text = ButtonActions.None.ToString();
                    break;

                case (int)ButtonActions.Home:
                    TextBoxButton1Mode.Text = ButtonActions.Home.ToString();
                    break;
            }

            switch (mainWindow.Btn2Mode)
            {
                case (int)ButtonActions.None:
                    TextBoxButton2Mode.Text = ButtonActions.None.ToString();
                    break;

                case (int)ButtonActions.Home:
                    TextBoxButton2Mode.Text = ButtonActions.Home.ToString();
                    break;
            }

        }
    }
}
