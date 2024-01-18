using Avalonia.Controls;
using Avalonia.Interactivity;
using Avalonia.Dialogs;

namespace GUI;

public partial class MainWindow : Window
{
    public MainWindow()
    {
        InitializeComponent();
    }

    public void Button_Click(object sender, RoutedEventArgs e)
    {
        ShowMessageBox();
    }
    private async void ShowMessageBox()
    {
        var dialog = new DialogBuilder().SetTitle("MessageBox Title")
                                        .SetText("Hello, Avalonia!")
                                        .SetIcon(DialogIcon.None)
                                        .AddButton(DialogButton.Ok)
                                        .Build();

        await dialog.ShowDialog(this);
    }

}