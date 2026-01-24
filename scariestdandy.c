
/*
    Random Mouse & Keyboard Input Generator
    Compile with Open Watcom: wcl386 -bt=nt scariestdandy.c -l=nt_win
*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define INTERVAL_MS 1
#define TIMER_ID 1

/* Global screen bounds */
static int g_left   = 0;
static int g_top    = 0;
static int g_right  = 0;
static int g_bottom = 0;

/* Random number between min and max inclusive */
static int random_range(int min_val, int max_val)
{
    return min_val + (rand() % (max_val - min_val + 1));
}

/* Move mouse to absolute screen position */
static void move_mouse(int x, int y)
{
    SetCursorPos(x, y);
}

/* Simulate mouse left click using mouse_event (XP compatible) */
static void click_left(void)
{
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

/* Simulate key press using keybd_event (XP compatible) */
static void press_key(BYTE vk)
{
    keybd_event(vk, 0, 0, 0);
    keybd_event(vk, 0, KEYEVENTF_KEYUP, 0);
}

/* Simulate key press with modifiers */
static void send_key(BYTE vk, int use_ctrl, int use_alt, int use_shift, int use_win)
{
    /* Press modifiers */
    if (use_ctrl) {
        keybd_event(VK_CONTROL, 0, 0, 0);
    }
    if (use_alt) {
        keybd_event(VK_MENU, 0, 0, 0);
    }
    if (use_shift) {
        keybd_event(VK_SHIFT, 0, 0, 0);
    }
    if (use_win) {
        keybd_event(VK_LWIN, 0, 0, 0);
    }
    
    /* Press and release main key */
    keybd_event(vk, 0, 0, 0);
    keybd_event(vk, 0, KEYEVENTF_KEYUP, 0);
    
    /* Release modifiers in reverse order */
    if (use_win) {
        keybd_event(VK_LWIN, 0, KEYEVENTF_KEYUP, 0);
    }
    if (use_shift) {
        keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
    }
    if (use_alt) {
        keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);
    }
    if (use_ctrl) {
        keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
    }
}

/* Key definitions matching the AHK script */
typedef struct {
    BYTE vk;
    int ctrl;
    int alt;
    int shift;
    int win;
} KeyCombo;

static KeyCombo g_keys[] = {
    { VK_RETURN,  0, 0, 0, 0 },  /* {Enter} */
    { VK_TAB,     0, 0, 0, 0 },  /* {Tab} */
    { VK_ESCAPE,  0, 0, 0, 0 },  /* {Esc} */
    { VK_CONTROL, 0, 0, 0, 0 },  /* {Ctrl} */
    { VK_MENU,    0, 0, 0, 0 },  /* {Alt} */
    { VK_SHIFT,   0, 0, 0, 0 },  /* {Shift} */
    { VK_LWIN,    0, 0, 0, 0 },  /* {LWin} */
    { 'C',        1, 0, 0, 0 },  /* ^c */
    { 'V',        1, 0, 0, 0 },  /* ^v */
    { 'A',        1, 0, 0, 0 },  /* ^a */
    { VK_TAB,     0, 1, 0, 0 },  /* !{Tab} (Alt+Tab) */
    { '4',        0, 0, 0, 0 },  /* "4" key */
    { VK_SPACE,   0, 0, 0, 0 },  /* {Space} */
    { VK_RBUTTON, 0, 0, 0, 0 },  /* {RButton} (right mouse button) */
    /* "toco" — send as sequence of characters 't','o','c','o' */
};

#define NUM_KEYS (sizeof(g_keys) / sizeof(g_keys[0]))

/* Timer callback - does the random stuff */
static void CALLBACK timer_proc(HWND hwnd, UINT msg, UINT_PTR id, DWORD tick)
{
    int x, y, idx;
    
    (void)hwnd;
    (void)msg;
    (void)id;
    (void)tick;
    
    /* Random mouse position and move */
    x = random_range(g_left, g_right);
    y = random_range(g_top, g_bottom);
    move_mouse(x, y);
    
    /* Click left button */
    click_left();
    
    /* Send random key combo */
    idx = random_range(0, (int)NUM_KEYS - 1);
    send_key(g_keys[idx].vk,
             g_keys[idx].ctrl,
             g_keys[idx].alt,
             g_keys[idx].shift,
             g_keys[idx].win);
}

/* Window procedure */
static LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg) {
    case WM_DESTROY:
        KillTimer(hwnd, TIMER_ID);
        PostQuitMessage(0);
        return 0;
        
    case WM_KEYDOWN:
        /* Press Escape to quit */
        if (wp == VK_ESCAPE) {
            DestroyWindow(hwnd);
            return 0;
        }
        break;
    }
    
    return DefWindowProc(hwnd, msg, wp, lp);
}

int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmdline, int show)
{
    WNDCLASS wc;
    HWND hwnd;
    MSG msg;
    
    (void)prev;
    (void)cmdline;
    (void)show;
    
    /* Seed random number generator */
    srand((unsigned int)time(NULL));
    
    /* Get screen dimensions */
    g_right  = GetSystemMetrics(SM_CXSCREEN) - 1;
    g_bottom = GetSystemMetrics(SM_CYSCREEN) - 1;
    
    /* Register window class */
    memset(&wc, 0, sizeof(wc));
    wc.lpfnWndProc   = wnd_proc;
    wc.hInstance     = inst;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = "ChaosClass";
    
    if (!RegisterClass(&wc)) {
        return 1;
    }
    
    /* Create hidden window for message loop */
    hwnd = CreateWindow(
        "ChaosClass",
        "Chaos",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        200, 100,
        NULL, NULL, inst, NULL
    );
    
    if (!hwnd) {
        return 1;
    }
    
    /* Optionally show window - uncomment to see it */
    /* ShowWindow(hwnd, SW_SHOW); */
    
    /* Start timer */
    SetTimer(hwnd, TIMER_ID, INTERVAL_MS, timer_proc);
    
    /* Message loop */
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return (int)msg.wParam;
}