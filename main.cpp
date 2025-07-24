#include "src\\calculator.h"

// Window procedure to handle messages sent to the main window
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HCURSOR g_hCustomCursor = NULL;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    
    // Define and register the window class
    WNDCLASS wc = {};
    wc.lpfnWndProc   = WindowProc;                          // Set the window procedure
    wc.hInstance     = hInstance;
    wc.lpszClassName = L"RenaissanceCalc";                  // Class name for the window
    wc.hIcon = (HICON)LoadImage(nullptr, L"media\\calculator_icon.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
    wc.hCursor = (HCURSOR)LoadImage(NULL, L"media\\normal.cur", IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE);
    wc.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(245, 222, 179))); // Light warm background color

    if (!RegisterClass(&wc)) return -1;

    // Create the application window
    HWND hwnd = CreateWindowEx(
        0,
        L"RenaissanceCalc",               // Class name
        L"Renaissance Calculator",        // Window title
        WS_SYSMENU | WS_MINIMIZEBOX,     // Window style (no resize, only close/minimize)
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 500,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd) return -1;

    // Show and update the window
    ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);

    // Message loop
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_COMMAND: {
            // Handle button click events
            ButtonID btnID = static_cast<ButtonID>(LOWORD(wParam));
            DisplayText(hwnd, btnID);
            return 0;
        }
        case WM_CREATE: {
            // Load custom cursor
            g_hCustomCursor = (HCURSOR)LoadImage(NULL, L"media\\normal.cur", IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE);
            if (!g_hCustomCursor) {
                MessageBox(hwnd, L"Failed to load cursor!", L"Error", MB_OK | MB_ICONERROR);
            }
            // Create buttons and apply visual styles
            AddButtons(hwnd);
            ApplyStyles(hwnd);
            return 0;
        }
        case WM_SETCURSOR: {
            // Set custom cursor if available
            if (g_hCustomCursor) {
                SetCursor(g_hCustomCursor);
                return TRUE; // Indicate we handled the cursor
            }
            break;
        }
        case WM_DESTROY:
            // Exit the application
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}
