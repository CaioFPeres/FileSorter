#include "WatchDirectory.h"




WatchDirectory::WatchDirectory(LPTSTR lpdir) {
    
    this->lpDir = lpdir;

    _tsplitpath_s(lpDir, lpDrive, 4, NULL, 0, lpFile, _MAX_FNAME, lpExt, _MAX_EXT);

    lpDrive[2] = (TCHAR)'\\';
    lpDrive[3] = (TCHAR)'\0';

    dwChangeHandles[0] = NULL;
    dwChangeHandles[1] = NULL;
    dwWaitStatus = NULL;

    watching = true;
}


void WatchDirectory::watch()
{

    // Watch the directory for file creation and deletion.

    dwChangeHandles[0] = FindFirstChangeNotification(
        lpDir,                         // directory to watch 
        TRUE,                         // do not watch subtree 
        FILE_NOTIFY_CHANGE_FILE_NAME); // watch file name changes

    if (dwChangeHandles[0] == INVALID_HANDLE_VALUE)
    {
        printf("\n ERROR: FindFirstChangeNotification function failed.\n");
        ExitProcess(GetLastError());
    }


    // Watch the subtree for directory creation and deletion. 
    /*
    dwChangeHandles[1] = FindFirstChangeNotification(
        lpDrive,                       // directory to watch
        TRUE,                          // watch the subtree
        FILE_NOTIFY_CHANGE_DIR_NAME);  // watch dir name changes

    if (dwChangeHandles[1] == INVALID_HANDLE_VALUE)
    {
        printf("\n ERROR: FindFirstChangeNotification function failed.\n");
        ExitProcess(GetLastError());
    }
    */


    while (watching)
    {
        // Wait for notification.

        dwWaitStatus = WaitForMultipleObjects(1, dwChangeHandles, FALSE, INFINITE);

        switch (dwWaitStatus)
        {
            case WAIT_OBJECT_0:

                // A file was created, renamed, or deleted in the directory.
                // Refresh this directory and restart the notification.

                RefreshDirectory();

                if (FindNextChangeNotification(dwChangeHandles[0]) == FALSE)
                {
                    printf("\n ERROR: FindNextChangeNotification function failed.\n");
                    ExitProcess(GetLastError());
                }
                break;

            case WAIT_OBJECT_0 + 1:

                // A directory was created, renamed, or deleted.
                // Refresh the tree and restart the notification.

                //RefreshTree();

                if (FindNextChangeNotification(dwChangeHandles[1]) == FALSE)
                {
                    printf("\n ERROR: FindNextChangeNotification function failed.\n");
                    ExitProcess(GetLastError());
                }
                break;

            default:
                printf("\n ERROR: Unhandled dwWaitStatus.\n");
                ExitProcess(GetLastError());
                break;
        }
    }

}

void WatchDirectory::stop() {
    this->watching = false;
}

void WatchDirectory::RefreshDirectory()
{
    vector<wstring>* fileNames = getFilesNames();
    int i = 0;

    while (i < (*fileNames).size()) {
        moverParaPasta((*fileNames)[i++], lpDir);
    }
}

void WatchDirectory::RefreshTree()
{
    // This is where you might place code to refresh your
    // directory listing, including the subtree.

    _tprintf(TEXT("Directory tree (%s) changed.\n"), lpDrive);
}


vector<wstring>* WatchDirectory::getFilesNames() {

    vector<wstring>* fileNames = new vector<wstring>;
    int dirNameSize = 0;

    WIN32_FIND_DATA ffd;
    LARGE_INTEGER filesize;
    TCHAR szDir[MAX_PATH];
    size_t length_of_arg;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    DWORD dwError = 0;

    // Prepare string for use with FindFile functions. First, copy the
    // string to a buffer, then append '\*' to the directory name.

    wcscpy_s(szDir, lpDir);

    for (; szDir[dirNameSize] != '\0'; dirNameSize++);

    szDir[dirNameSize++] = '\\';
    szDir[dirNameSize++] = '*';
    szDir[dirNameSize] = '\0';


    // Find the first file in the directory.

    hFind = FindFirstFile(szDir, &ffd);

    if (INVALID_HANDLE_VALUE == hFind)
    {
        wprintf_s(L"\nERROR: %lu", dwError);
        return fileNames;
    }

    // List all the files in the directory with some info about them.
    do
    {
        if (ffd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
        {
            // Encontrou diret�rios, N�o fazer nada;
            //wprintf_s(TEXT("  %s   <DIR>\n"), ffd.cFileName);
        }
        else
        {
            filesize.LowPart = ffd.nFileSizeLow;
            filesize.HighPart = ffd.nFileSizeHigh;
            //wprintf_s(TEXT("  %s   %ld bytes\n"), ffd.cFileName, (int)filesize.QuadPart);  // printa nomes e tamanhos dos arquivos;
            (*fileNames).push_back(ffd.cFileName);
        }

    } while (FindNextFile(hFind, &ffd) != 0);

    dwError = GetLastError();
    if (dwError != ERROR_NO_MORE_FILES)
    {
        wprintf_s(L"\nERROR: %lu", dwError);
    }

    // printing with C++ iterator
    /*
    for (std::vector<wstring>::iterator it = (*fileNames).begin(); it != (*fileNames).end(); ++it) {
        wcout << *it << endl;
    }
    */


    FindClose(hFind);

    return fileNames;
}


void WatchDirectory::moverParaPasta(wstring fileName, LPTSTR folderPath) {

    wstring newFolderPath;
    wstring filePath = folderPath + wstring(L"\\") + fileName;
    LPTSTR extension = PathFindExtension(fileName.c_str());



    if (isImageExt(extension)) {

        newFolderPath = folderPath + wstring(L"\\") + L"Image" + wstring(L"\\");

        // cria diretorio se n�o existe
        if (GetFileAttributes(newFolderPath.c_str()) != FILE_ATTRIBUTE_DIRECTORY) {
            CreateDirectory(newFolderPath.c_str(), NULL);
        }

        MoveFile(filePath.c_str(), wstring(newFolderPath + fileName).c_str());
    }
    else if (isCompressedExt(extension)) {

        newFolderPath = folderPath + wstring(L"\\") + L"Rar" + wstring(L"\\");

        // cria diretorio se n�o existe
        if (GetFileAttributes(newFolderPath.c_str()) != FILE_ATTRIBUTE_DIRECTORY) {
            CreateDirectory(newFolderPath.c_str(), NULL);
        }

        MoveFile(filePath.c_str(), wstring(newFolderPath + fileName).c_str());
    }
    else {

        newFolderPath = folderPath + wstring(L"\\") + L"Documents" + wstring(L"\\");

        // cria diretorio se n�o existe
        if (GetFileAttributes(newFolderPath.c_str()) != FILE_ATTRIBUTE_DIRECTORY) {
            CreateDirectory(newFolderPath.c_str(), NULL);
        }

        MoveFile(filePath.c_str(), wstring(newFolderPath + fileName).c_str());
    }

}


bool WatchDirectory::isImageExt(LPTSTR extension) {

    if (wcscmp(extension, L".img") == 0 || wcscmp(extension, L".bmp") == 0 || wcscmp(extension, L".gif") == 0 || wcscmp(extension, L".png") == 0 ||
        wcscmp(extension, L".jpg") == 0 || wcscmp(extension, L".jpeg") == 0) {
        return true;
    }
    else {
        return false;
    }

}

bool WatchDirectory::isCompressedExt(LPTSTR extension) {

    if (wcscmp(extension, L".rar") == 0 || wcscmp(extension, L".zip") == 0 || wcscmp(extension, L".tar") == 0 || wcscmp(extension, L".gz") == 0 ||
        wcscmp(extension, L".7z") == 0) {
        return true;
    }
    else {
        return false;
    }

}