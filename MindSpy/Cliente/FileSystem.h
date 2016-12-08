#pragma once
#ifndef FILESYSTEM_H
#define FILESYSTEM_H  // <<<< Eso es redundante. Para evitar eso es el #pragma once o.o

#include <Shlobj.h>
#include <Shlwapi.h>
#include <Windows.h>
#include <iostream>
#include <string>
#include "Datos.h"
using namespace std;
#define getID(x) (MAX_PATH * x)

namespace MindSpy
{

#define  DIR_ROOT_SYSTEM				0x01 // C:\WINDOWS
#define  DIR_ROOT_DESKTOP				0x02 
#define  DIR_ROOT_DOCUMENTS				0x04 
#define  DIR_ROOT_DOWNLOADS				0x08 




	/**
	* \brief  Simple Enumeracion los Tipos de Directorio
	* \note La dejaré solo por compatibilidad de versiones. No considero que haya que complicarse tanto

	enum SystemFolder
	{

		WIN_FOLDERS = 0x00,
		DIR_HOME_SYSTEM = WIN_FOLDERS | DIR_ROOT_SYSTEM, // C:\\WINDOWS
		DIR_DESKTOP = WIN_FOLDERS | DIR_ROOT_DESKTOP,
		DIR_DOCUMENTS = WIN_FOLDERS | DIR_ROOT_DOCUMENTS,
		DIR_DOWNLOADS = WIN_FOLDERS | DIR_ROOT_DOWNLOADS,

		WIN_CUSTOM_FOLDERS = 0x33BD9B76,
		DIR_FIREFOX = WIN_CUSTOM_FOLDERS | 0xFA0EACB1,
		DIR_TOTAL = 0x08
	};	

	*/

	enum SystemFolder {
		DIR_HOME_SYSTEM,
		DIR_DESKTOP,
		DIR_DOCUMENTS,
		DIR_DOWNLOADS
	};

	enum ContentDir
	{
		ALL_FROM_PATH,
		ONLY_SUBDIR,
		ONLY_ARCHIVE
	};

	class FileSystem
	{
	public:
		FileSystem();
		~FileSystem();

		wstring getSystemDir(SystemFolder Dir);
		stListaArchivos getDirContent(wstring path, wstring Filter, ContentDir type, DWORD flags);

		wstring getWindowsPath();
		wstring getDesktopPath();
		wstring getDocumentsPath();
		wstring getDowloadsPath();
		stListaArchivos getAllFiles(wstring path, wstring Filter);
		stListaArchivos getAllSubDirs(wstring path, wstring Filter);
		stListaArchivos getAll(wstring path, wstring Filter);
		stListaArchivos getFileByExt(wstring path, wstring ext);

	private:
		WCHAR dirPath[MAX_PATH];
		WCHAR *BuffTemp;
		long long * TamañosTemp;
		long long * FechasCreacionTemp;
		long long * FechasModificacionTemp;
	};


}

#endif// FILESYSTEM_H

