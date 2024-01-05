using System.Net.Http;
using System.Windows;
using System.Windows.Interop;
using System.Windows.Media;


namespace VRS_Mouse_App_SplashScreen
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        private static readonly HttpClient MouseClient = new(new SocketsHttpHandler());

        protected override void OnStartup(StartupEventArgs e)
        {
            base.OnStartup(e);
            RenderOptions.ProcessRenderMode = RenderMode.SoftwareOnly;
        }

        public static HttpClient GetMouseClient()
        {
            return MouseClient;
        }
    }
}
