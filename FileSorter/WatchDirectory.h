#include <iostream>
#include <windows.h>
#include <shlwapi.h>

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <fstream>

using std::cout;
using std::wcout;
using std::cin;
using std::wcin;
using std::string;
using std::wstring;
using std::vector;
using std::endl;


class WatchDirectory
{

	private:

		LPTSTR lpDir;

		DWORD dwWaitStatus;
		HANDLE dwChangeHandles[2];
		TCHAR lpDrive[4];
		TCHAR lpFile[_MAX_FNAME];
		TCHAR lpExt[_MAX_EXT];

		bool watching;

		void RefreshDirectory();
		void RefreshTree();
		vector<wstring>* getFilesNames();
		void moverParaPasta(wstring fileName, LPTSTR folderPath);
		bool isImageExt(LPTSTR extension);
		bool isCompressedExt(LPTSTR extension);

	public:

		WatchDirectory(LPTSTR lpdir);
		void watch();
		void stop();

};

