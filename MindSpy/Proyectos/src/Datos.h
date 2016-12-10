/**
* @file Datos.h
* @author Carlos D. Alvarez
* @date 10/12/2016
* @brief Contiene las definiciones de enumeradores, macros y estructuras necesarias para el servidor y el cliente
*/

#pragma once
#ifndef _DATOS_H
#define _DATOS_H
#include <string>
#include <windows.h>
/**
* @def MAX_BUFFER
* Tamaño del buffer máximo de entrada
*/
#define MAX_BUFFER 4096
/**
* @def getID(x)
* Obtiene el elemento x, de un arreglo de wchar, donde cada elemento tiene MAX_PATH caracteres
*/
#define getID(x) (MAX_PATH * x)
/**
* @def MAKELONGLONG(hi, lo)
* Convierte dos DWORD en un LONGLONG.
*/
#define MAKELONGLONG(hi, lo)    ((LONGLONG(DWORD(hi) & 0xffffffff) << 32 ) | LONGLONG(DWORD(lo) & 0xffffffff))
using namespace std;
namespace MindSpy
{
	/**
	* @enum CLNT_CMDS
	* Constantes de los comandos enviados desde y hacia el servidor
	*/
	enum CLNT_CMDS {	
		CLOSE,			// Cerrar la conexion
		NAME,			// Obtener nombre del servidor
		VERSION,		// Version del cliente
		SYSINFO,		// Información del sistema
		FILEINFO,		// Información de archivos
		REGINFO,		// Información del registro
		MENSAJE			// Mensaje desde el servidor
	};

	/**
	* @enum INTERNOS_CMDS
	* Constantes de los comandos del panel de control
	*/
	enum INTERNOS_CMDS {
		REQ_ALIAS,
		REQ_ENVIAR,
		REQ_SYSINFO,
		REQ_REGINFO,
		REQ_FILEINFO
	};

	/**
	* @enum FILEINFO_QUERY
	* Constantes para el tipo de consulta de archivos
	*/
	enum FILEINFO_QUERY
	{
		REQ_ALL_FROM_PATH,
		REQ_ONLY_SUBDIR,
		REQ_ONLY_ARCHIVE
	};

	/**
	* @struct stSystemInformation
	* Contiene los datos básicos del sistema operativo del cliente
	*/
	struct stSystemInfoResponse {
		UINT32 Build;					//! Compilación del OS
		UINT16 VersionMayor;			//! Versión mayor del OS
		UINT16 VersionMenor;			//! Versión menor del OS
		UINT16 Arquitectura;			//! Arquitectura del OS
		WCHAR NombreOS[64];				//! Nombre del OS basado en la versión
		WCHAR MAC[18];					//! Dirección física de la interface de red conectada al router
		WCHAR NombreUsuario[64];		//! Nombre del usuario que usa el cliente
		bool EsWindowsServer;			//! Detección para Windows server
	};

	/**
	* @struct stFileInfoRequest
	* Contiene los campos necesarios para realizar una consulta de archivos
	*/
	struct stFileInfoRequest {
		UINT8 Query;
		WCHAR Path[MAX_PATH];
		WCHAR Filtro[10];
	};

	/**
	* @struct stListaArchivos
	* Almacena los datos de una cantidad indeterminada de archivos o carpetas
	*/
	struct stListaArchivos {
		UINT32 CantArchivos;
		WCHAR* Archivos;
		PFILETIME FechasCreacion;
		PFILETIME FechasModificacion;
		PLONGLONG Tamaños;
		stListaArchivos() : 
			CantArchivos(0), 
			Archivos(NULL), 
			FechasCreacion(NULL), 
			FechasModificacion(NULL),
			Tamaños(NULL){};
	};

	/**
	* @struct stFile
	* Contiene los datos de un único archivo o carpeta
	*/
	struct stFile {
		FILETIME FechaCreacion;
		FILETIME FechaModificacion;
		wstring nombre;
		LONGLONG Tamaño;
	};
}

#endif // ifdef