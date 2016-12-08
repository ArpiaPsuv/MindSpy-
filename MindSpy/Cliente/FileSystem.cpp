#include "FileSystem.h"

namespace MindSpy
{
	FileSystem::FileSystem()
	{
		BuffTemp = NULL;
		FechasCreacionTemp = NULL;
		FechasModificacionTemp = NULL;
		TamañosTemp = NULL;
	}

	FileSystem::~FileSystem()
	{
		if (BuffTemp)free(BuffTemp);
		if (FechasCreacionTemp)free(FechasCreacionTemp);
		if (FechasModificacionTemp)free(FechasModificacionTemp);
		if (TamañosTemp)free(TamañosTemp);
	}

	/*
	*		-----------FUNCIÓN NO COMPATIBLE CON WINXP-----------
	*		USAR SHGetFolderPath EN CASO DE QUE EL SISTEMA SEA V5
	*/
	wstring FileSystem::getSystemDir(SystemFolder Dir)
	{
		WCHAR path[MAX_PATH];
		wchar_t *ppath;
		wstring retorno;

		switch (Dir) {
		case SystemFolder::DIR_HOME_SYSTEM:
			GetWindowsDirectoryW(path, MAX_PATH);
			return path;

		case SystemFolder::DIR_DESKTOP:
			SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &ppath);
			retorno = ppath;
			CoTaskMemFree(ppath);
			return retorno;

		case SystemFolder::DIR_DOCUMENTS:
			SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &ppath);
			retorno = ppath;
			CoTaskMemFree(ppath);
			return retorno;

		case SystemFolder::DIR_DOWNLOADS:
			SHGetKnownFolderPath(FOLDERID_Downloads, 0, NULL, &ppath);
			retorno = ppath;
			CoTaskMemFree(ppath);
			return retorno;

		default:
			return L"";
		}
	}

	stListaArchivos FileSystem::getDirContent(wstring path, wstring Filter, ContentDir type, DWORD flags)
	{
		int items = 0;
		int pos = 0;
		HANDLE isFind;
		WIN32_FIND_DATA FindData;
		wchar_t Ruta[MAX_PATH];

		if (BuffTemp)
			free(BuffTemp);
		if (FechasCreacionTemp)
			free(FechasCreacionTemp);
		if (FechasModificacionTemp)
			free(FechasModificacionTemp);
		if (TamañosTemp)
			free(TamañosTemp);

		wcscpy(Ruta, path.c_str());
		PathAddBackslashW(Ruta);
		wcscat(Ruta, Filter.c_str());
		isFind = FindFirstFileW(Ruta, &FindData);

		BuffTemp = (WCHAR*)malloc(sizeof(wchar_t)*MAX_PATH);
		TamañosTemp = (long long*)malloc(sizeof(long long));
		FechasCreacionTemp = (long long*)malloc(sizeof(long long));
		FechasModificacionTemp = (long long*)malloc(sizeof(long long));

		do
		{
			if (!wcscmp(FindData.cFileName, L".") || !wcscmp(FindData.cFileName, L".."))
				continue;
			if (type == ONLY_SUBDIR && !(FindData.dwFileAttributes  & FILE_ATTRIBUTE_DIRECTORY))
				continue;
			if (type == ONLY_ARCHIVE && !(FindData.dwFileAttributes  & FILE_ATTRIBUTE_ARCHIVE))
				continue;

			wcscpy(&BuffTemp[getID(items)], FindData.cFileName);
			TamañosTemp[items] = MAKELONGLONG(FindData.nFileSizeHigh, FindData.nFileSizeLow);
			FechasCreacionTemp[items] = MAKELONGLONG(FindData.ftCreationTime.dwHighDateTime, FindData.ftCreationTime.dwLowDateTime);
			FechasModificacionTemp[items] = MAKELONGLONG(FindData.ftLastWriteTime.dwHighDateTime, FindData.ftLastWriteTime.dwLowDateTime);

			items++;
			BuffTemp = (wchar_t*)realloc(BuffTemp, sizeof(wchar_t) * MAX_PATH * (items + 1));
			TamañosTemp = (long long*)realloc(TamañosTemp, sizeof(long long) * (items + 1));
			FechasCreacionTemp = (long long*)realloc(FechasCreacionTemp, sizeof(long long) * (items + 1));
			FechasModificacionTemp = (long long*)realloc(FechasModificacionTemp, sizeof(long long) * (items + 1));

		} while (FindNextFileW(isFind, &FindData));


		if (items) {
			stListaArchivos stla;
			stla.CantArchivos = items;
			stla.Archivos = BuffTemp;
			stla.FechasCreacion = FechasCreacionTemp;
			stla.FechasModificacion = FechasModificacionTemp;
			stla.Tamaños = TamañosTemp;
			return stla;
		}
		else
			return stListaArchivos();
	}

	wstring FileSystem::getWindowsPath()
	{
		return this->getSystemDir(SystemFolder::DIR_HOME_SYSTEM);
	}

	wstring FileSystem::getDesktopPath()
	{

		return getSystemDir(SystemFolder::DIR_DESKTOP);
	}

	wstring FileSystem::getDocumentsPath()
	{
		return getSystemDir(SystemFolder::DIR_DOCUMENTS);
	}

	wstring FileSystem::getDowloadsPath()
	{
		return getSystemDir(SystemFolder::DIR_DOWNLOADS);
	}

	stListaArchivos FileSystem::getAllFiles(wstring path, wstring Filter)
	{
		return getDirContent(path, Filter, ContentDir::ONLY_ARCHIVE, NULL);
	}

	stListaArchivos FileSystem::getAllSubDirs(wstring path, wstring Filter)
	{
		return getDirContent(path, Filter, ContentDir::ONLY_SUBDIR, NULL);
	}

	stListaArchivos FileSystem::getAll(wstring path, wstring Filter)
	{
		return getDirContent(path, Filter, ContentDir::ALL_FROM_PATH, NULL);
	}

	stListaArchivos FileSystem::getFileByExt(wstring path, wstring ext)
	{
		wstring filter = (wstring)L"*" + ext;
		return getAllFiles(path, filter);
	}
}
