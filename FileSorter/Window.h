#include <iostream>
#include <windows.h>
#include <WinUser.h>
#include <shlwapi.h>
#include <sys/stat.h>

#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <string>

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#include <commctrl.h>

#include <vector>
#include <fstream>

#define APPWM_ICONNOTIFY (WM_APP + 1)

#define SWM_MAIN	WM_APP + 2//	show the main window
#define SWM_ABOUT	WM_APP + 3//	about
#define SWM_EXIT	WM_APP + 4//	close the window
#define SWM_SETTING	WM_APP + 5//	show setting windows

#ifdef _UNICODE
#define scanf wscanf_s
#else
#define scanf scanf
#define printf printf
#endif

using std::cout;
using std::wcout;
using std::cin;
using std::wcin;
using std::string;
using std::wstring;
using std::vector;
using std::endl;


class Window
{

	private:

		NOTIFYICONDATA nid = {};

	public:

		int WINAPI initWindow(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow);
		static LRESULT CALLBACK WndProc(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam);

		Window();
		std::string GetLastErrorAsString();
		void ShowContextMenu(HWND hWnd);
		TCHAR* CharToTCHAR(const char* cha);

};

