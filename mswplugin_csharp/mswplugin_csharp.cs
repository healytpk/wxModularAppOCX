using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.Threading.Tasks;

namespace mswplugin_csharp
{
    public class Plugin
    {

        // P/Invoke to set the parent window
        [DllImport("user32.dll", SetLastError = true)]
        static extern IntPtr SetParent(IntPtr hWndChild, IntPtr hWndNewParent);

        public static int QueryPlugin(String pwzArgument)
        {
            return 666;
        }
        public static int PopulatePanelHWND(String pwzArgument)
        {
            IntPtr hwndParent = new IntPtr(0);

            if ( long.TryParse(pwzArgument, out long hwndLong) )
            {
                // Create an IntPtr from the long value
                hwndParent = new IntPtr(hwndLong);
            }

            // Create a new Form object
            Form form = new Form
            {
                Text = "Temperature Converter",
                FormBorderStyle = FormBorderStyle.None, // Make the form borderless
                TopLevel = false // Set it as a child window
            };

            // Set the parent window of the form to the HWND passed from C++
            SetParent(form.Handle, hwndParent);

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

            return 667;
        }
    }

}  // close namespace
