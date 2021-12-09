#include "Window.h"


Window::Window() {

}


int WINAPI Window::initWindow(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {

    WNDCLASSEX wcex;

    static TCHAR szWindowClass[] = _T("DesktopApp");
    static TCHAR szTitle[] = _T("Windows Desktop Guided Tour Application");


    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);


    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL, _T("Call to RegisterClassEx failed!"), _T("Windows Desktop Guided Tour"), NULL);

        return 1;
    }



    // The parameters to CreateWindowEx explained:
    // WS_EX_OVERLAPPEDWINDOW : An optional extended window style.
    // szWindowClass: the name of the application
    // szTitle: the text that appears in the title bar
    // WS_OVERLAPPEDWINDOW: the type of window to create
    // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
    // 500, 100: initial size (width, length)
    // NULL: the parent of this window
    // NULL: this application does not have a menu bar
    // hInstance: the first parameter from WinMain
    // NULL: not used in this application


    DWORD style = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);

    HWND PhWnd = CreateWindowEx(0, szWindowClass, szTitle, style, CW_USEDEFAULT, CW_USEDEFAULT, 500, 300, NULL, NULL, hInstance, NULL);



    if (!PhWnd)
    {
        MessageBox(NULL, _T("Call to CreateWindowEx failed!"), _T("Windows Desktop Guided Tour"), NULL);

        return 1;
    }


    ShowWindow(PhWnd, nCmdShow);

    UpdateWindow(PhWnd);


    //icon for notification area
    /// //////////////////////////////////////////////////////////////////////////////////////////////

    nid.uVersion = NOTIFYICON_VERSION_4;

    // Set the version
    Shell_NotifyIcon(NIM_SETVERSION, &nid);

    nid.cbSize = sizeof(nid);
    nid.hWnd = PhWnd;
    nid.uID = 1;
    nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
    nid.uCallbackMessage = APPWM_ICONNOTIFY;
    nid.cbSize = sizeof(nid);
    wcscpy_s(nid.szTip, L"FileSorter");
    nid.hIcon = (HICON)LoadImageA(NULL, "folderIcon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_SHARED | LR_DEFAULTSIZE);


    // Load the icon for high DPI.
    //LoadIconMetric(NULL, MAKEINTRESOURCE(L"C:\\Users\\Caio Peres\\source\\repos\\FileSorter\\FileSorter\\folderIcon.ico"), LIM_LARGE, &(nid.hIcon));

    // test for failed image operation
    if (nid.hIcon == NULL) {

        TCHAR* test = CharToTCHAR(GetLastErrorAsString().c_str());

        if (wcslen(test) == 0)
            OutputDebugString(L"no errors in \n");
        else
            OutputDebugString(test);

    }


    // Show the notification.
    Shell_NotifyIcon(NIM_ADD, &nid) ? S_OK : E_FAIL;

    /// //////////////////////////////////////////////////////////////////////////////////////////////

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    DestroyIcon(nid.hIcon);
    Shell_NotifyIcon(NIM_DELETE, &nid);

    return (int)msg.wParam;

}


LRESULT CALLBACK Window::WndProc(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam) {

    PAINTSTRUCT ps;
    HDC hdc;
    TCHAR greeting[] = _T("Hello, Windows desktop!");

    // weird windows stuff ( this retrieves the "this" pointer of the current instance, to be able to call non-static methods from a static method )
    Window* thisWindow = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

    /* // to test the message number
    TCHAR test[63];
    wsprintf(test, L"%d\n", message);
    OutputDebugString(test);
    */

    switch (message) {

        case APPWM_ICONNOTIFY:

            switch (lParam)
            {
                case WM_LBUTTONDBLCLK:
                    //ShowWindow(hWnd, SW_RESTORE);
                    // nada
                    break;
                case WM_RBUTTONDOWN:
                    OutputDebugString(L"RCLICK ");
                    thisWindow->ShowContextMenu(hWnd);
                    break;
            }

            break;

        case WM_COMMAND:

            switch (LOWORD(wParam))
            {
                case SWM_MAIN:
                    OutputDebugString(L"Main ");
                    break;
            }

            break;

        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);

            // Here your application is laid out.
            // For this introduction, we just print out "Hello, Windows desktop!"
            // in the top left corner.
            TextOut(hdc, 5, 5, greeting, _tcslen(greeting));
            // End application-specific layout section.

            EndPaint(hWnd, &ps);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
            break;

    }

    return 0;

}


TCHAR* Window::CharToTCHAR(const char* cha)
{
    int aa = strlen(cha);

    TCHAR* tmp = new TCHAR[aa + 1];

    for (int i = 0; i < aa + 1; i++)
    {
        tmp[i] = cha[i];
    }

    return tmp;
}


void Window::ShowContextMenu(HWND hWnd)
{

    HMENU hMenu = CreatePopupMenu();
    POINT pt;
    GetCursorPos(&pt);

    if (hMenu)
    {
        InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_MAIN, L"Main");
        InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_SETTING, L"Setting");
        InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_ABOUT, L"About");
        InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_EXIT, L"Exit");

        SetForegroundWindow(hWnd);

        // Display the menu
        TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);

        DestroyMenu(hMenu);

    }
}


std::string Window::GetLastErrorAsString()
{
    //Get the error message ID, if any.
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID == 0) {
        return std::string(); //No error message has been recorded
    }

    LPSTR messageBuffer = nullptr;

    //Ask Win32 to give us the string version of that message ID.
    //The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    //Copy the error message into a std::string.
    std::string message(messageBuffer, size);

    //Free the Win32's string's buffer.
    LocalFree(messageBuffer);

    return message;
}