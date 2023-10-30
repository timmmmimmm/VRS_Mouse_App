using SpinnerController;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Animation;
using System.Windows;
using VRS_Mouse_App_SplashScreen.Properties;

namespace VRS_Mouse_App_SplashScreen
{
    internal class AnimationHolder
    {
        private const string retryStoryboardAnimationName = "RetryStoryboard";
        private const string spinnerFadeStoryboardAnimationName = "SpinnerFadeStoryboard";
        private readonly MainWindow ?SplashScreen;

        public AnimationHolder(MainWindow? splashScreen = null)
        {
            SplashScreen = splashScreen;

            CreateRetryStoryboard();
            CreateLoadingSpinnerFadeStoryboard();
        }



        private void CreateRetryStoryboard()
        {
            if(SplashScreen == null)
            {
                return;
            }

            var retryStoryboard = new Storyboard
            {
                Duration = new Duration(TimeSpan.FromMilliseconds(600))
            };

            var retryAnimation = new DoubleAnimation()
            {
                From = 0,
                To = 360,
                Duration = new Duration(TimeSpan.FromMilliseconds(200)),
                RepeatBehavior = new RepeatBehavior(3)
            };

            Storyboard.SetTarget(retryAnimation, SplashScreen.RetryButton);
            Storyboard.SetTargetProperty(retryAnimation, new PropertyPath("(UIElement.RenderTransform).(RotateTransform.Angle)"));

            retryStoryboard.Children.Add(retryAnimation);

            SplashScreen.Resources.Add (retryStoryboardAnimationName, retryStoryboard);
        }

        private void CreateLoadingSpinnerFadeStoryboard()
        {
            if(SplashScreen == null) 
            {
                return;
            }

            var spinnerFadeStoryboard = new Storyboard();

            var spinnerFadeAnimation = new DoubleAnimation()
            {
                From = 0,
                To = 1,
                Duration = new Duration(TimeSpan.FromMilliseconds(1000))
            };

            Storyboard.SetTarget(spinnerFadeAnimation, SplashScreen.LoadingSpinner);
            Storyboard.SetTargetProperty(spinnerFadeAnimation, new PropertyPath("Opacity"));

            spinnerFadeStoryboard.Children.Add(spinnerFadeAnimation);

           SplashScreen.Resources.Add(spinnerFadeStoryboardAnimationName, spinnerFadeStoryboard);
        }

        private void StartStoryboard(string storyboardName)
        {
            if(SplashScreen ==null)
            {
                return;
            }

            ((Storyboard)SplashScreen.Resources[storyboardName]).Begin();
        }

        public void HideRetryButtonFadeOutLoadnigSpinner()
        {
            if(SplashScreen == null)
            {
                return;
            }

            SplashScreen.RetryButton.Visibility = Visibility.Collapsed;
            SplashScreen.LoadingSpinner.Opacity = 0;
            SplashScreen.LoadingSpinner.Visibility = Visibility.Visible;
            StartStoryboard(spinnerFadeStoryboardAnimationName);
        }

        public void HideLoadingSpinnerSpinRetryButtonThreeTimes() {
        
            if(SplashScreen == null)
            {
                return;
            }

           SplashScreen.LoadingSpinner.Visibility = Visibility.Collapsed;
           SplashScreen.RetryButton.Visibility = Visibility.Visible;
           StartStoryboard("RetryStoryboard");
        }
    }
}
