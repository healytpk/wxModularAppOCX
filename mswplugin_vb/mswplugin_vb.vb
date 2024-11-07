Imports System
Imports System.Runtime.InteropServices
Imports System.Windows.Forms
Public Class Plugin

    <DllImport("user32.dll", SetLastError:=True)>
    Private Shared Function SetParent(hWndChild As IntPtr, hWndNewParent As IntPtr) As IntPtr
    End Function

    Public Shared Function QueryPlugin(pwzArgument As String) As Integer
        Return 666
    End Function

    Public Shared Function PopulatePanelNativeHandle(pwzArgument As String) As Integer
        Dim hwndParent As IntPtr = IntPtr.Zero

        If ULong.TryParse(pwzArgument, hwndParent) Then
            Dim form As New Form With {
                .Text = "Torque Converter",
                .FormBorderStyle = FormBorderStyle.None,
                .TopLevel = False
            }

            SetParent(form.Handle, hwndParent)

            Dim footPoundsLabel As New Label With {
                .Text = "Torque in Foot Pounds:",
                .Dock = DockStyle.Top
            }
            Dim footPoundsTextBox As New TextBox With {
                .Dock = DockStyle.Top
            }
            Dim convertButton As New Button With {
                .Text = "Convert to Newton Metres",
                .Dock = DockStyle.Top
            }
            Dim newtonMetresLabel As New Label With {
                .Dock = DockStyle.Top
            }

            AddHandler convertButton.Click,
                Sub(sender, e)
                    Dim footPounds As Double
                    If Double.TryParse(footPoundsTextBox.Text, footPounds) Then
                        Dim newtonMetres As Double = footPounds * 1.35582
                        newtonMetresLabel.Text = $"Torque in Newton Metres: {newtonMetres:F2}"
                    End If
                End Sub

            form.Controls.Add(newtonMetresLabel)
            form.Controls.Add(convertButton)
            form.Controls.Add(footPoundsTextBox)
            form.Controls.Add(footPoundsLabel)

            form.Show()

            Return 667
        End If

        Return 0
    End Function
End Class
