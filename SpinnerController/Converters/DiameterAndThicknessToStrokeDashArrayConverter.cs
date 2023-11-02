using System;
using System.Globalization;
using System.Windows.Data;
using System.Windows.Media;

namespace SpinnerController.Converters
{
    internal class DiameterAndThicknessToStrokeDashArrayConverter : IMultiValueConverter
    {
        public object Convert(object[] values, Type targetType, object parameter, CultureInfo culture)
        {
            if (values.Length < 2 ||
                !double.TryParse(values[0].ToString(), out double diameter) ||
                !double.TryParse(values[1].ToString(), out double thickness))
                return 0;

            double circumference = Math.PI * diameter;

            double lineLength = circumference * 0.75;
            double gapLenght = circumference - lineLength;

            return new DoubleCollection(new[] { lineLength / thickness, gapLenght / thickness });
        }

        public object[] ConvertBack(object value, Type[] targetTypes, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
