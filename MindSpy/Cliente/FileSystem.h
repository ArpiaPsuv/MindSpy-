#pragma once
#ifndef FILESYSTEM_H
#define FILESYSTEM_H


namespace MindSpy
{

#define  DIR_ROOT_SYSTEM				0x01 // C:\WINDOWS
#define  DIR_ROOT_DESKTOP				0x02 
#define  DIR_ROOT_DOCUMENTS				0x04 
#define  DIR_ROOT_DOWNLOADS				0x08 




	/**
	* \brief  Simple Enumeracion los Tipos de Directorio
	*/
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

	enum ContentDir
	{
		ALL_FROM_PATH = 0x38ACC70A,
		ONLY_SUBDIR,


	};


	class FileSystem
	{
	public:
		FileSystem();
		~FileSystem();

		bool getSytemDir(LPWSTR buffer, DWORD flags);
		bool getDirContent(LPWSTR path, LPWSTR buffer, DWORD type, DWORD flags);

		LPWSTR getWindowsPath();
		LPWSTR getDesktopPath();
		LPWSTR getDocumentsPath();
		LPWSTR getDowloadsPath();

		WCHAR* getAllFiles();


	private:
		WCHAR* path;
		WCHAR** rm;
		WCHAR dirPath[MAX_PATH];
	};


}

#endif // FILESYSTEM_H

