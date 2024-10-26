using System;
using System.Runtime.InteropServices;
using System.Windows.Forms;

public class Plugin {

    // P/Invoke to set the parent window
    [DllImport("user32.dll", SetLastError = true)]
    static extern IntPtr SetParent(IntPtr hWndChild, IntPtr hWndNewParent);

    // Method to create and attach WinForms controls to the provided HWND
    [DllExport("PopulatePanelHWND", CallingConvention = CallingConvention.StdCall)]
    public static bool PopulatePanelHWND(IntPtr hWndParent)
    {
        // Create a new Form object
        Form form = new Form {
            Text = "Temperature Converter",
            FormBorderStyle = FormBorderStyle.None, // Make the form borderless
            TopLevel = false // Set it as a child window
        };

        // Set the parent window of the form to the HWND passed from C++
        SetParent(form.Handle, hWndParent);

        // Create controls
        Label fahrenheitLabel = new Label { Text = "Temperature in Fahrenheit:", Dock = DockStyle.Top };
        TextBox fahrenheitTextBox = new TextBox { Dock = DockStyle.Top };
        Button convertButton = new Button { Text = "Convert to Celsius", Dock = DockStyle.Top };
        Label celsiusLabel = new Label { Dock = DockStyle.Top };

        // Add event handler for button click
        convertButton.Click += (sender, e) =>
        {
            if (double.TryParse(fahrenheitTextBox.Text, out double fahrenheit))
            {
                double celsius = (fahrenheit - 32) * 5 / 9;
                celsiusLabel.Text = $"Temperature in Celsius: {celsius:F2}"; // Display the result
            }
        };

        // Add the controls to the form
        form.Controls.Add(celsiusLabel);
        form.Controls.Add(convertButton);
        form.Controls.Add(fahrenheitTextBox);
        form.Controls.Add(fahrenheitLabel);

        // Show the form inside the passed window
        form.Show();

        return true;
    }
}
