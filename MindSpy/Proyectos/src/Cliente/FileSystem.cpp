/**
* @file FileSystem.h
* @author Carlos D. Alvarez
* @date 10/12/2016
* @brief Define la clase FileSystem
*/

#include "FileSystem.h"

namespace MindSpy
{
	FileSystem::FileSystem()
	{
		BuffTemp = NULL;
		FechasCreacionTemp = NULL;
		FechasModificacionTemp = NULL;
		TamañosTemp = NULL;
		AtributosTemp = NULL;
	}

	FileSystem::~FileSystem()
	{
		if (BuffTemp)free(BuffTemp);
		if (FechasCreacionTemp)free(FechasCreacionTemp);
		if (FechasModificacionTemp)free(FechasModificacionTemp);
		if (TamañosTemp)free(TamañosTemp);
		if (AtributosTemp)free(AtributosTemp);
	}

	wstring FileSystem::getSystemDir(SystemFolder Dir)
	{
		//! Ruta temporal
		WCHAR path[MAX_PATH];
		//! Puntero a ruta temporal
		wchar_t *ppath;
		//! String unicode temporal para el retorno de la función		
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
		//! Lista de items actual
		int items = 0;
		//! Handle de FindFirstFileW
		HANDLE isFind;
		//! Datos del archivo
		WIN32_FIND_DATAW FindData;
		//! Ruta temporal
		wchar_t Ruta[MAX_PATH];

		if (BuffTemp)
			free(BuffTemp);
		if (FechasCreacionTemp)
			free(FechasCreacionTemp);
		if (FechasModificacionTemp)
			free(FechasModificacionTemp);
		if (TamañosTemp)
			free(TamañosTemp);
		if (AtributosTemp)
			free(AtributosTemp);

		wcscpy(Ruta, path.c_str());
		// En lugar de:
		//	PathAddBackslashW(Ruta);
		// nos evitamos incluir una lib entera solo para ella
		if (Ruta[wcslen(Ruta) - 1] != L'\\') {
			int r = wcslen(Ruta);
			Ruta[r] = L'\\';
			Ruta[r + 1] = 0;
		}
		wcscat(Ruta, Filter.c_str());
		isFind = FindFirstFileW(Ruta, &FindData);

		BuffTemp = (WCHAR*)malloc(sizeof(wchar_t)*MAX_PATH);
		TamañosTemp = (PLONGLONG)malloc(sizeof(LONGLONG));
		FechasCreacionTemp = (PFILETIME)malloc(sizeof(FILETIME));
		FechasModificacionTemp = (PFILETIME)malloc(sizeof(FILETIME));
		AtributosTemp = (DWORD*)malloc(sizeof(int));

		int Offset = 0;
		do
		{
			if (!wcscmp(FindData.cFileName, L".") || !wcscmp(FindData.cFileName, L".."))
				continue;
			if (type == ONLY_SUBDIR && !(FindData.dwFileAttributes  & FILE_ATTRIBUTE_DIRECTORY))
				continue;
			if (type == ONLY_ARCHIVE && !(FindData.dwFileAttributes  & FILE_ATTRIBUTE_ARCHIVE))
				continue;

			wcscpy((wchar_t*)((char*)BuffTemp + Offset), FindData.cFileName);
			Offset += wcslen(FindData.cFileName) * 2 + 2;
			FechasCreacionTemp[items] = FindData.ftCreationTime;
			FechasModificacionTemp[items] = FindData.ftLastWriteTime;
			TamañosTemp[items] = MAKELONGLONG(FindData.nFileSizeHigh, FindData.nFileSizeLow);
			AtributosTemp[items] = FindData.dwFileAttributes;
			items++;

			BuffTemp = (wchar_t*)realloc(BuffTemp, sizeof(wchar_t) * MAX_PATH * (items + 1));
			TamañosTemp = (long long*)realloc(TamañosTemp, sizeof(long long) * (items + 1));
			FechasCreacionTemp = (PFILETIME)realloc(FechasCreacionTemp, sizeof(FILETIME) * (items + 1));
			FechasModificacionTemp = (PFILETIME)realloc(FechasModificacionTemp, sizeof(FILETIME) * (items + 1));
			AtributosTemp = (DWORD*)realloc(AtributosTemp, sizeof(DWORD) * (items + 1));
		} while (FindNextFileW(isFind, &FindData));


		if (items) {
			stListaArchivos stla;
			stla.TamNombres = Offset;
			stla.CantArchivos = items;
			stla.Archivos = BuffTemp;
			stla.FechasCreacion = FechasCreacionTemp;
			stla.FechasModificacion = FechasModificacionTemp;
			stla.Tamaños = TamañosTemp;
			stla.Atributos = AtributosTemp;
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
		//! Filtro a pasarle a getAllFilesm basado en la extensión
		wstring filter = (wstring)L"*" + ext;
		return getAllFiles(path, filter);
	}
}
