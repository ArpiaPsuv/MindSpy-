/**
* @file Datos.h
* @author Carlos D. Alvarez
* @date 10/12/2016
* @brief Contiene las definiciones de enumeradores, macros y estructuras necesarias para el servidor y el cliente
*/

#pragma once
#include <string>
#include <windows.h>

#define MAX_BUFFER 4096
#define getID(x) (MAX_PATH * x)
#define MAKELONGLONG(hi, lo)    ((LONGLONG(DWORD(hi) & 0xffffffff) << 32 ) | LONGLONG(DWORD(lo) & 0xffffffff))
#define CLIENTE_VERSION_MAYOR 0
#define CLIENTE_VERSION_MENOR 1
#define CLIENTE_BUILD 51

using namespace std;
namespace MindSpy
{
	/**
	* @enum CLNT_CMDS
	* Constantes de los comandos enviados desde y hacia el servidor
	*/
	enum CLNT_CMDS 
	{	
		//! Cerrar la conexion
		CLOSE,
		//! Obtener nombre del servidor
		NAME,	
		//! Version del cliente
		VERSION,	
		//! Información del sistema
		SYSINFO,	
		//! Información de archivos
		FILEINFO,
		//! Información del registro
		REGINFO,
		//! Mensaje desde el servidor
		MENSAJE			
	};

	/**
	* @enum INTERNOS_CMDS
	* Constantes de los comandos del panel de control
	*/
	enum INTERNOS_CMDS 
	{
		//! Solicitar cambio de alias al CPanel
		REQ_ALIAS,
		//! Enviar mensaje al cliente
		REQ_ENVIAR,	
		//! Solicitar información de sistema al cliente
		REQ_SYSINFO,
		//! Solicitar clave del registro al cliente
		REQ_REGINFO,	
		//! Solicitar archivos o directorios al cliente
		REQ_FILEINFO	
	};

	/**
	* @enum FILEINFO_QUERY
	* Constantes para el tipo de consulta de archivos
	*/
	enum FILEINFO_QUERY
	{
		//! Todos los archivos y directorios
		REQ_ALL_FROM_PATH,	
		//! Solo directorios
		REQ_ONLY_SUBDIR,
		//! Solo archivos
		REQ_ONLY_ARCHIVE	
	};

	/**
	* @struct stFileInfoRequest
	* Contiene los campos necesarios para realizar una consulta de archivos
	*/
	struct stFileInfoRequest 
	{
		//! Tipo de consulta
		UINT8 Query;		
		//! Ruta de la consulta
		WCHAR Path[MAX_PATH];	
		//! Criterio de búsqueda, se permiten comodines * y ?
		WCHAR Filtro[10];				
	};
	
	/**
	* @struct stInitialInfo
	* Contiene los datos a enviar al servidor en la conexión
	*/
	struct stInitialInfo 
	{
		UINT32 VersionMayor;
		UINT32 VersionMenor;
		UINT32 Build;
		char hwid[64];
	};

	/**
	* @struct stSystemInfoResponse
	* Contiene los datos básicos del sistema operativo del cliente
	*/
	struct stSystemInfoResponse 
	{
		//! Compilación del OS (x.x.<b>x</b>)
		UINT32 Build;	
		//! Versión mayor del OS (<b>x</b>.x.x)
		UINT16 VersionMayor;	
		//! Versión menor del OS (x.<b>x</b>.x)
		UINT16 VersionMenor;	
		//! Arquitectura del OS
		UINT16 Arquitectura;	
		//! Nombre del OS basado en la versión
		WCHAR NombreOS[64];		
		//! Dirección física de la interface de red conectada al router
		WCHAR MAC[18];			
		//! Nombre del usuario que usa el cliente
		WCHAR NombreUsuario[64];	
		//! Detección para Windows server
		bool EsWindowsServer;			
	};

	/**
	* @struct stListaArchivos
	* Almacena los datos de una cantidad indeterminada de archivos o carpetas
	* para enviarlos al servidor
	*/
	struct stListaArchivos 
	{
		//! Cantidad de archivos en la lista
		UINT32 CantArchivos;	
		//! Lista de archivos
		WCHAR* Archivos;		
		//! Fechas de creación de cada uno de los archivos
		PFILETIME FechasCreacion;		
		//! Fechas de modificación de cada uno de los archivos
		PFILETIME FechasModificacion;	
		//! Tamaños de los archivos
		PLONGLONG Tamaños;	
		//! Constructor de la estructura, inicializa todos los miembros a cero
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
	* para almacenarlos en local
	*/
	struct stFile {
		//! Fecha de creación del archivo
		FILETIME FechaCreacion;		
		//! Fecha de modificación del archivo
		FILETIME FechaModificacion;	
		//! Nombre del archivo	
		wstring nombre;	
		// ! Tamaño del archivo
		LONGLONG Tamaño;				
	};
}
