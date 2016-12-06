#include "Windows.h"
#include "FileSystem.h"
#include "Shlobj.h"

#include <iostream> //remove  later
#include <minwinbase.h>

namespace MindSpy
{
	FileSystem::FileSystem()
	{
	}

	FileSystem::~FileSystem()
	{
	}

	bool FileSystem::getSytemDir(LPWSTR buffer, DWORD flags)
	{
		if ((flags & WIN_FOLDERS) == WIN_FOLDERS)
		{
			//flags &= ~WIN_DEFAULT_FOLDERS;

			if (flags & DIR_ROOT_SYSTEM)
			{
				printf("%s\n", "Nesecito La ruta Del sistema operativo Windows");
				path = (WCHAR *)malloc(sizeof(WCHAR*));
				GetWindowsDirectoryW(path, MAX_PATH);

				memcpy(buffer, path, MAX_PATH);
				//wcscpy(buffer, path);
				return (wcslen(buffer) > 0) ? true : false;
			}

	
			if (flags & DIR_ROOT_DESKTOP)
			{
				//Esto No va Funcionar Para versiones de Windows Menor A  Windows Vista
				printf("%s\n", "Nesecito La ruta Del escritorio");

				WCHAR* path = (WCHAR *)malloc(sizeof(WCHAR*));
				SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &path);
				wcscpy(buffer, path);
				CoTaskMemFree(path);
				return (wcslen(buffer) > 0) ? true : false;
			}


			if (flags & DIR_ROOT_DOCUMENTS)
			{
				//Esto No va Funcionar Para versiones de Windows Menor A  Windows Vista
				printf("%s\n", "Nesecito La ruta Del directorio de Mis documentos");

				WCHAR* path = (WCHAR *)malloc(sizeof(WCHAR*));
				SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &path);
				wcscpy(buffer, path);
				CoTaskMemFree(path);
				return (wcslen(buffer) > 0) ? true : false;
			}

			if (flags & DIR_ROOT_DOWNLOADS)
			{
				//Esto No va Funcionar Para versiones de Windows Menor A  Windows Vista
				printf("%s\n", "Nesecito La ruta Del directorio de Descargas");

				WCHAR* path = (WCHAR *)malloc(sizeof(WCHAR*));
				SHGetKnownFolderPath(FOLDERID_Downloads, 0, NULL, &path);
				wcscpy(buffer, path);
				CoTaskMemFree(path);
				return (wcslen(buffer) > 0) ? true : false;
			}
		}

		return false;
	}

	LPWSTR FileSystem::getWindowsPath()
	{
		
		ZeroMemory(&dirPath, sizeof(dirPath));
		if (getSytemDir(dirPath, DIR_HOME_SYSTEM)) return dirPath;
		return  false;
	}

	LPWSTR FileSystem::getDesktopPath()
	{
		
		ZeroMemory(&dirPath, sizeof(dirPath));
		if (getSytemDir(dirPath, DIR_DESKTOP)) return dirPath;
		return  false;
	}

	LPWSTR FileSystem::getDocumentsPath()
	{
		ZeroMemory(&dirPath, sizeof(dirPath));
		if (getSytemDir(dirPath, DIR_DOCUMENTS)) return dirPath;
		return  false;
	}

	LPWSTR FileSystem::getDowloadsPath()
	{
		ZeroMemory(&dirPath, sizeof(dirPath));
		if (getSytemDir(dirPath, DIR_DOWNLOADS)) return dirPath;
		return  false;
	}
}
