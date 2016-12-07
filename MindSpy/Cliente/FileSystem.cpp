#include "FileSystem.h"

namespace MindSpy
{
	FileSystem::FileSystem()
	{
		BuffTemp = NULL;
	}

	FileSystem::~FileSystem()
	{
		if (BuffTemp)
			free(BuffTemp);
	}

	bool FileSystem::getSytemDir(LPWSTR buffer, DWORD flags)
	{
		if ((flags & WIN_FOLDERS) == WIN_FOLDERS)
		{
			path = (WCHAR*)malloc(sizeof(wchar_t)*MAX_PATH);

			if (flags & DIR_ROOT_SYSTEM)
			{
				printf("%s\n", "Nesecito La ruta Del sistema operativo Windows");

				GetWindowsDirectoryW(path, MAX_PATH);
				memcpy(buffer, path, MAX_PATH);
				return (wcslen(buffer) > 0) ? true : false;
			}


			if (flags & DIR_ROOT_DESKTOP)
			{
				//Esto No va Funcionar Para versiones de Windows Menor A  Windows Vista
				printf("%s\n", "Nesecito La ruta Del escritorio");


				SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &path);
				wcscpy(buffer, path);
				CoTaskMemFree(path);
				return (wcslen(buffer) > 0) ? true : false;
			}


			if (flags & DIR_ROOT_DOCUMENTS)
			{
				//Esto No va Funcionar Para versiones de Windows Menor A  Windows Vista
				printf("%s\n", "Nesecito La ruta Del directorio de Mis documentos");

				SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &path);
				wcscpy(buffer, path);
				CoTaskMemFree(path);
				return (wcslen(buffer) > 0) ? true : false;
			}

			if (flags & DIR_ROOT_DOWNLOADS)
			{
				//Esto No va Funcionar Para versiones de Windows Menor A  Windows Vista
				printf("%s\n", "Nesecito La ruta Del directorio de Descargas");

				SHGetKnownFolderPath(FOLDERID_Downloads, 0, NULL, &path);
				wcscpy(buffer, path);
				CoTaskMemFree(path);
				return (wcslen(buffer) > 0) ? true : false;
			}
		}

		return false;
	}

	stListaArchivos FileSystem::getDirContent(LPWSTR path, DWORD type, DWORD flags)
	{
		int items = 0;
		int pos = 0;
		HANDLE isFind;
		WIN32_FIND_DATA FindData;
		WCHAR *ext = L"*";
		if (BuffTemp)
			free(BuffTemp);

		PathAddBackslashW(path);
		if (type == ALL_FROM_PATH || ONLY_SUBDIR)
			wcscat(path, ext);

		isFind = FindFirstFileW(path, &FindData);
		BuffTemp = (wchar_t*)malloc(sizeof(wchar_t)*MAX_PATH);
		//rm = (WCHAR **)malloc(sizeof(WCHAR*));

		do
		{
			if (type == ONLY_SUBDIR && !(FindData.dwFileAttributes  & FILE_ATTRIBUTE_DIRECTORY))
				continue;

			wcscpy(&BuffTemp[getID(items++)], FindData.cFileName);
			BuffTemp = (wchar_t*)realloc(BuffTemp, sizeof(wchar_t) * MAX_PATH * (items + 1));
		} while (FindNextFileW(isFind, &FindData));
		if (items) {
			stListaArchivos stla;
			stla.CantArchivos = items;
			stla.Archivos = BuffTemp;
			return stla;
		}
		else
			return stListaArchivos();
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

	stListaArchivos FileSystem::getAllFiles()
	{

		WCHAR rtPath[MAX_PATH];
		RtlSecureZeroMemory(&path, sizeof(WCHAR));
		RtlSecureZeroMemory(&rtPath, sizeof(WCHAR));

		path = getDowloadsPath();
		stListaArchivos stla = getDirContent(path, ALL_FROM_PATH, NULL);

		for (int i = 0; i < stla.CantArchivos; i++)
			std::wcout << &stla.Archivos[getID(i)] << std::endl;
		return 0;
	}
}
