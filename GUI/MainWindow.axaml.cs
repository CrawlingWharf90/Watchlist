using Avalonia;
using Avalonia.Controls;
using Avalonia.Interactivity;
using Avalonia.Dialogs;
using Avalonia.VisualTree;

namespace GUI
{
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        public void PopUpWindow(object sender, RoutedEventArgs args)
        {
            //Write Hello World
            TextBlock textBlock = new TextBlock();
            textBlock.Text = "Hello World!";
            textBlock.FontSize = 20;
            textBlock.HorizontalAlignment = Avalonia.Layout.HorizontalAlignment.Center;

        }
    }
}
