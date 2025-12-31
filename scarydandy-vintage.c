' Add a Timer control to your form (Timer1)
' Set Timer1.Interval = 1 in the Properties window

Option Explicit

Private Declare Function GetSystemMetrics Lib "user32" (ByVal nIndex As Long) As Long
Private Declare Function SetCursorPos Lib "user32" (ByVal x As Long, ByVal y As Long) As Long
Private Declare Sub mouse_event Lib "user32" (ByVal dwFlags As Long, ByVal dx As Long, ByVal dy As Long, ByVal cButtons As Long, ByVal dwExtraInfo As Long)
Private Declare Sub keybd_event Lib "user32" (ByVal bVk As Byte, ByVal bScan As Byte, ByVal dwFlags As Long, ByVal dwExtraInfo As Long)

Private Const SM_CXSCREEN = 0
Private Const SM_CYSCREEN = 1
Private Const MOUSEEVENTF_LEFTDOWN = &H2
Private Const MOUSEEVENTF_LEFTUP = &H4
Private Const KEYEVENTF_KEYUP = &H2

Private Const VK_RETURN = &HD
Private Const VK_TAB = &H9
Private Const VK_ESCAPE = &H1B
Private Const VK_CONTROL = &H11
Private Const VK_MENU = &H12
Private Const VK_SHIFT = &H10
Private Const VK_LWIN = &H5B

Dim keys() As Variant

Private Sub Form_Load()
    ' Initialize the keys array
    keys = Array("ENTER", "TAB", "ESC", "CTRL", "ALT", "SHIFT", "WIN", "CTRL+C", "CTRL+V", "CTRL+A", "ALT+TAB", "ALT+F4")
    
    ' Enable the timer
    Timer1.Enabled = True
End Sub

Private Sub Timer1_Timer()
    Dim screenWidth As Long
    Dim screenHeight As Long
    Dim x As Long
    Dim y As Long
    Dim keyIndex As Integer
    
    ' Get screen dimensions
    screenWidth = GetSystemMetrics(SM_CXSCREEN)
    screenHeight = GetSystemMetrics(SM_CYSCREEN)
    
    ' Generate random coordinates
    Randomize
    x = Int(Rnd * screenWidth)
    y = Int(Rnd * screenHeight)
    
    ' Move mouse and click
    SetCursorPos x, y
    mouse_event MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0
    mouse_event MOUSEEVENTF_LEFTUP, 0, 0, 0, 0
    
    ' Send random key combination
    keyIndex = Int(Rnd * (UBound(keys) + 1))
    SendKey keys(keyIndex)
End Sub

Private Sub SendKey(keyCombo As String)
    Select Case keyCombo
        Case "ENTER"
            keybd_event VK_RETURN, 0, 0, 0
            keybd_event VK_RETURN, 0, KEYEVENTF_KEYUP, 0
        Case "TAB"
            keybd_event VK_TAB, 0, 0, 0
            keybd_event VK_TAB, 0, KEYEVENTF_KEYUP, 0
        Case "ESC"
            keybd_event VK_ESCAPE, 0, 0, 0
            keybd_event VK_ESCAPE, 0, KEYEVENTF_KEYUP, 0
        Case "CTRL"
            keybd_event VK_CONTROL, 0, 0, 0
            keybd_event VK_CONTROL, 0, KEYEVENTF_KEYUP, 0
        Case "ALT"
            keybd_event VK_MENU, 0, 0, 0
            keybd_event VK_MENU, 0, KEYEVENTF_KEYUP, 0
        Case "SHIFT"
            keybd_event VK_SHIFT, 0, 0, 0
            keybd_event VK_SHIFT, 0, KEYEVENTF_KEYUP, 0
        Case "WIN"
            keybd_event VK_LWIN, 0, 0, 0
            keybd_event VK_LWIN, 0, KEYEVENTF_KEYUP, 0
        Case "CTRL+C"
            keybd_event VK_CONTROL, 0, 0, 0
            keybd_event Asc("C"), 0, 0, 0
            keybd_event Asc("C"), 0, KEYEVENTF_KEYUP, 0
            keybd_event VK_CONTROL, 0, KEYEVENTF_KEYUP, 0
        Case "CTRL+V"
            keybd_event VK_CONTROL, 0, 0, 0
            keybd_event Asc("V"), 0, 0, 0
            keybd_event Asc("V"), 0, KEYEVENTF_KEYUP, 0
            keybd_event VK_CONTROL, 0, KEYEVENTF_KEYUP, 0
        Case "CTRL+A"
            keybd_event VK_CONTROL, 0, 0, 0
            keybd_event Asc("A"), 0, 0, 0
            keybd_event Asc("A"), 0, KEYEVENTF_KEYUP, 0
            keybd_event VK_CONTROL, 0, KEYEVENTF_KEYUP, 0
        Case "ALT+TAB"
            keybd_event VK_MENU, 0, 0, 0
            keybd_event VK_TAB, 0, 0, 0
            keybd_event VK_TAB, 0, KEYEVENTF_KEYUP, 0
            keybd_event VK_MENU, 0, KEYEVENTF_KEYUP, 0
        Case "ALT+F4"
            keybd_event VK_MENU, 0, 0, 0
            keybd_event &H73, 0, 0, 0  ' VK_F4
            keybd_event &H73, 0, KEYEVENTF_KEYUP, 0
            keybd_event VK_MENU, 0, KEYEVENTF_KEYUP, 0
    End Select
End Sub
