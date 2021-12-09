#include "Window.h"
#include "WatchDirectory.h"


DWORD WINAPI threadFunction(LPVOID lParam) {

    WatchDirectory* wd = (WatchDirectory*)lParam;

    wd->watch();

    return 0;
}


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {

    // doesnt even need to be dynamic
    Window wind;

    FILE* arq = NULL;
    wchar_t path[MAX_PATH];


    //reading path

    if (_wfopen_s(&arq, L"monitorDir.txt", L"r") != 0) {
        printf("\nSomething went wrong.");
        return 0;
    }
    else if (arq == NULL) {
        printf("\nSomething went wrong.");
        return 0;
    }

    // gets all characters, including spaces and breaklines ( ws = versão unicode)
    fgetws(path, MAX_PATH, arq);

    WatchDirectory* wd = new WatchDirectory(path);

    DWORD threadID;

    HANDLE myHandle = CreateThread(NULL, 0, threadFunction, wd, 0, &threadID);

    wind.initWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

    wd->stop();
    
	return 0;
}