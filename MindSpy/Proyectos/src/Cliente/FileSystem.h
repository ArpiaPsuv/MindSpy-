/**
* @file FileSystem.h
* @author Carlos D. Alvarez
* @date 10/12/2016
* @brief Define la clase FileSystem
*/

#pragma once

#include <Windows.h>
#include <iostream>
#include <string>
#include <Shlobj.h>
#include "../Datos.h"
using namespace std;

namespace MindSpy
{

#define  DIR_ROOT_SYSTEM				0x01 // C:\WINDOWS
#define  DIR_ROOT_DESKTOP				0x02 
#define  DIR_ROOT_DOCUMENTS				0x04 
#define  DIR_ROOT_DOWNLOADS				0x08

	/**
	* @enum SystemFolder
	* @brief Contiene constantes para operar sobre directorios del sistema
	*/
	enum SystemFolder {
		//! Directorio de WINDOWS
		DIR_HOME_SYSTEM,
		//! Escritorio del usuario
		DIR_DESKTOP,
		//! Documentos del usuario
		DIR_DOCUMENTS,
		//! Descargas del usuario
		DIR_DOWNLOADS
	};

	/**
	* @enum ContentDir
	* @brief Tipo de consulta
	*/
	enum ContentDir
	{
		//! Todos los archivos y directorios
		ALL_FROM_PATH,
		//! Solo directorios
		ONLY_SUBDIR,
		//! Solo archivos
		ONLY_ARCHIVE
	};

	/**
	* @class FileSystem
	* @brief Contiene los métodos necesarios para operar sobre los directorios y archivos del sistema
	*/
	class FileSystem
	{
	public:
		/**
		* @brief Constructor por defecto, inicializa todos los datos en cero
		*/
		FileSystem();

		/**
		* @brief Destructor, libera la memoria reservada
		*/
		~FileSystem();

		/**
		* @brief Obtiene la ruta de un directorio del sistema
		* @param[in] Dir Constante de <a href="SystemFolder">SystemFolder</a> que indica qué directorio se devolverá
		* @return Un wstring con el directorio, o una cadena vacía si no se encontró
		*/
		wstring getSystemDir(SystemFolder Dir);

		/**
		* @brief Obtener el contenido de un directorio, bien sea en archivos o subdirectorios
		* @param[in]	path	Ruta a examinar
		* @param[in]	Filter	Filtro de exclusión, se permiten extensiones y comodines ? y *
		* @param[in]	type	Tipo de búsqueda
		* @param[in]	flags	Reservado, debe ser NULL
		* @return Un objeto de la estructura stListaArchivos con la lista que cumpla los criterios
		* @note: Si el directorio no existe, o no se tiene acceso a él, getDirContent devuelve 
		*	un objeto a una lista de cero elementos
		*/
		stListaArchivos getDirContent(wstring path, wstring Filter, ContentDir type, DWORD flags);

		/**
		* @brief Obtiene el directorio de Windows
		* @return Un string con el directorio de Windows	
		* @ warning -----------FUNCIÓN NO COMPATIBLE CON WINXP-----------
		*/
		wstring getWindowsPath();

		/**
		* @brief Obtiene el directorio de Windows
		* @return Un string con el directorio de Windows
		*/
		wstring getDesktopPath();

		/**
		* @brief Obtiene el directorio de documentos
		* @return Un string con el directorio de documentos
		*/
		wstring getDocumentsPath();

		/**
		* @brief Obtiene el directorio de descargas
		* @return Un string con el directorio de descargas
		*/
		wstring getDowloadsPath();

		/**
		* @brief Obtiene obtiene una lista de todos los archivos de un directorio que cumplan con un criterio
		* @param[in]	path	Ruta a examinar
		* @param[in]	Filter	Criterio
		* @return Un objeto stListaArchivos con la lista de los archivos encontrados
		*/
		stListaArchivos getAllFiles(wstring path, wstring Filter=L"*");

		/**
		* @brief Obtiene obtiene una lista de todos los archivos de un directorio que cumplan con un criterio
		* @param[in]	path	Ruta a examinar
		* @param[in]	Filter	Criterio
		* @return Un objeto stListaArchivos con la lista de los archivos encontrados
		*/
		stListaArchivos getAllSubDirs(wstring path, wstring Filter);

		/**
		* @brief Obtiene obtiene una lista de todos los archivos y directorios de un directorio que cumplan con un criterio
		* @param[in]	path	Ruta a examinar
		* @param[in]	Filter	Criterio
		* @return Un objeto stListaArchivos con la lista de los archivos y directorios encontrados
		*/
		stListaArchivos getAll(wstring path, wstring Filter=L"*");

		/**
		* @brief Obtiene obtiene una lista de todos los archivos de un directorio con la extensión especificada
		* @param[in]	path	Ruta a examinar
		* @param[in]	ext		extensión del archivo
		* @return Un objeto stListaArchivos con la lista de los archivos encontrados
		* @note El formado de la extensión debe ser ".extension"
		*/
		stListaArchivos getFileByExt(wstring path, wstring ext);
	private:
		//! Directorio temporal
		WCHAR dirPath[MAX_PATH];
		//! Puntero a un buffer de datos temporal
		PWCHAR BuffTemp;
		//! Tamaños de archivos de una lista temporal
		PLONGLONG TamañosTemp;
		//! Fechas de archivos de una lista temporal
		PFILETIME FechasCreacionTemp;
		//! Fechas de archivos de una lista temporal
		PFILETIME FechasModificacionTemp;
		//! Archivo o carpeta
		DWORD *AtributosTemp;
	};
}

