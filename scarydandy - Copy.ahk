CoordMode "Mouse", "Screen"
left   := 0
top    := 0
right  := A_ScreenWidth  - 1
bottom := A_ScreenHeight - 1
interval := 1
keys := [
"{Enter}", "{Esc}",
"{Ctrl}", "{Alt}", "{Shift}",
"#",
"^c", "^v", "^a",
"bro", "{Space}", "{RButton}"
]
SetTimer DoStuff, interval
DoStuff() {
global left, top, right, bottom, keys
x := Random(left, right)
y := Random(top, bottom)
MouseMove x, y, 0
Click "Left"
idx := Random(1, keys.Length)
Send keys[idx]
}