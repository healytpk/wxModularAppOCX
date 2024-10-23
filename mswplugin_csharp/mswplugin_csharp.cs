using System;
using System.Runtime.InteropServices;
using System.Windows.Forms;

public class Plugin
{
    // P/Invoke to set the parent window
    [DllImport("user32.dll", SetLastError = true)]
    static extern IntPtr SetParent(IntPtr hWndChild, IntPtr hWndNewParent);

    // Method to create and attach WinForms controls to the provided HWND
    [DllExport("PopulatePanelHWND", CallingConvention = CallingConvention.StdCall)]
    public static bool PopulatePanelHWND(IntPtr hWndParent)
    {
        // Create a new Form object
        Form form = new Form {
            Text = "Embedded Plugin",
            FormBorderStyle = FormBorderStyle.None, // Make the form borderless
            TopLevel = false // Set it as a child window
        };

        // Set the parent window of the form to the HWND passed from C++
        SetParent(form.Handle, hWndParent);

        // Create some widgets like text boxes and buttons
        TextBox textBox = new TextBox { Text = "Monkey!", Dock = DockStyle.Top };
        Button button = new Button { Text = "Click Me", Dock = DockStyle.Bottom };

        // Add the controls to the form
        form.Controls.Add(textBox);
        form.Controls.Add(button);

        // Show the form inside the passed window
        form.Show();

        MessageBox.Show("I like monkeys!");

        return true;
    }
}
