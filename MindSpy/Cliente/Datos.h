#pragma once
#define MAX_BUFFER 4096
namespace MindSpy
{
	// Estructura para los datos del cliente
	enum CLNT_CMDS {	
		CLOSE,			// Cerrar la conexion
		NAME,			// Obtener nombre del servidor
		VERSION,		// Version del cliente
		SYSINFO,		// Información del sistema
		FILEINFO,		// Información de archivos
		REGINFO,		// Información del registro
		MENSAJE			// Mensaje desde el servidor
	};

	enum INTERNOS_CMDS {
		REQ_ALIAS,
		REQ_ENVIAR,
		REQ_SYSINFO,
		REQ_REGINFO,
		REQ_FILEINFO
	};

	// Estructura para los datos del sistema
	struct stSystemInfoResponse {
		UINT32 Build;					// Compilación del OS
		UINT16 VersionMayor;			// Versión mayor del OS
		UINT16 VersionMenor;			// Versión menor del OS
		UINT16 Arquitectura;			// Arquitectura del OS
		WCHAR NombreOS[64];				// Nombre del OS basado en la versión
		WCHAR MAC[18];					// Dirección física de la interface de red conectada al router
		WCHAR NombreUsuario[64];		// Nombre del usuario que usa el cliente
		bool EsWindowsServer;			// Detección para Windows server
	};

	struct stListaArchivos {
		UINT32 CantArchivos;
		WCHAR* Archivos;
		stListaArchivos() : CantArchivos(0), Archivos(NULL) {};
	};
}