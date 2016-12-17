/**
* @file Sistema.cpp
* @author Carlos D. Alvarez
* @date 10/12/2016
* @brief Implementa la clase Sistema
*/
#include "Sistema.h"

namespace MindSpy
{
	const char* Sistema::ObtenerHwid() 
	{
		HKEY opened;
		DWORD copiados;
		static char Buffer[64];
		DWORD type = REG_SZ;
		int r = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Cryptography", 0, KEY_QUERY_VALUE, &opened);	
		if (r == NO_ERROR) {
			RegQueryValueExA(opened, "MachineGuid", 0, &type, (LPBYTE)Buffer, &copiados);
			RegCloseKey(opened);
			return Buffer;
		}
		return NULL;
	}

	void Sistema::ObtenerMAC()
	{
		//! Estrcutrura que va a alojar la información de los adaptadores. Es una lista enlazada
		PIP_ADAPTER_INFO AdapterInfo;
		//! Tamaño de la estructura
		DWORD dwBufLen = sizeof(AdapterInfo);
		//! Variable para guardar la MAC
		static char mac_addr[18];
		// Reservar memoria preliminar para la estructura
		AdapterInfo = (IP_ADAPTER_INFO *)malloc(dwBufLen);
		// Inicializar la estructura. Si falla, es poque el tamaño es incorrecto
		if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == ERROR_BUFFER_OVERFLOW)
		{
			// Si el tamaño es incorrecto, la función devuelve el tamaño correcto en dwBufLen.
			// Solo liberamos y reservamos de nuevo
			free(AdapterInfo);
			AdapterInfo = (IP_ADAPTER_INFO*)malloc(dwBufLen);
		}
		//! Se crea una copia del puntero para liberarla al final, ya que el puntero cambia
		PIP_ADAPTER_INFO ParaLiberar = AdapterInfo;
		// Una vez con el tamaño correcto, se obtiene la info
		if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == ERROR_SUCCESS)
		{
			do
			{
				// Se valida si la IP de la interfaz y su puerta de enlace predeterminada, son distintas a cero
				if (strcmp(AdapterInfo->IpAddressList.IpAddress.String, "0.0.0.0") &&
					strcmp(AdapterInfo->GatewayList.IpAddress.String, "0.0.0.0"))
				{
					// De serlo, se le da formato a la MAC y se copia a la variable
					sprintf(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X",
						AdapterInfo->Address[0], AdapterInfo->Address[1],
						AdapterInfo->Address[2], AdapterInfo->Address[3],
						AdapterInfo->Address[4], AdapterInfo->Address[5]);

					// Se copian esos cambios a la estructura y se termina el ciclo
					strncpy(info.MAC, mac_addr, 18);
					break;
				}
				// Sino, se mueve el puntero a la estructura siguiente
				AdapterInfo = AdapterInfo->Next;
			}
			while (AdapterInfo);
			// Liberamos la memoria
			free(ParaLiberar);
		}
		return;
	}

	void Sistema::ObtenerVersionWindows()
	{
		/**
		* RtlGetVersion es una función de la API del WDK. 
		* @details En teoría, funciona solo en modo kernel bajo un IRQL >5
		* Su equivalente en el Windows SDK está en desuso, así que se usará esta.
		* Se crea un puntero a esa función y se obtiene su dirección válida con GetProcAddress.
		*/
		void (__stdcall * RtlGetVersionW)(POSVERSIONINFOEXW);
		RtlGetVersionW = (void(__stdcall*)(POSVERSIONINFOEXW)) GetProcAddress(LoadLibraryW(L"ntdll.dll"), "RtlGetVersion");
		// Si GetProcAddress falla (algo poco probable), el constructor termina
		if (!RtlGetVersionW)
			return;

		//! Se declara la estructura para alojar la info, y se asigna su tamaño
		OSVERSIONINFOEXW oviex;
		oviex.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXW);
		// Se obtiene la info
		RtlGetVersionW(&oviex);
		// Liberamos la biblioteca
		FreeLibrary(LoadLibraryW(L"ntdll.dll"));
		// Se obtiene la información de la estructura y dependiendo de la versión, se asigna el nombre del OS
		info.Build = oviex.dwBuildNumber;
		info.VersionMayor = (UINT16)oviex.dwMajorVersion;
		info.VersionMenor = (UINT16)oviex.dwMinorVersion;
		
		//! Valdrá TRUE si el OS es de 64 bits
		BOOL wow;

		//! Contiene el Handle del proceso abierto
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId());
		IsWow64Process(hProcess, &wow);
		CloseHandle(hProcess);
		info.Arquitectura = wow ? 64 : 32;
		info.EsWindowsServer = false;
		switch (info.VersionMayor)
		{
		case 10:
			strncpy(info.NombreOS, "Windows 10", 64);
			break;
		case 6:
			switch (info.VersionMenor)
			{
			case 3:
				strncpy(info.NombreOS, "Windows 8.1", 64);
				break;

			case 2:
				if (oviex.wProductType == VER_NT_WORKSTATION)
					strncpy(info.NombreOS, "Windows 8", 64);
				else {
					strncpy(info.NombreOS, "Windows Server 2012", 64);
					info.EsWindowsServer = true;
				}
				break;
			case 1:
				if (oviex.wProductType == VER_NT_WORKSTATION)
					strncpy(info.NombreOS, "Windows 7", 64);
				else {
					strncpy(info.NombreOS, "Windows Server 2008 R2", 64);
					info.EsWindowsServer = true;
				}
				break;

			case 0:
				if (oviex.wProductType == VER_NT_WORKSTATION)
					strncpy(info.NombreOS, "Windows Vista", 64);
				else {
					strncpy(info.NombreOS, "Windows Server 2008", 64);
					info.EsWindowsServer = true;
				}
				break;
			}

			break;
		case 5:
			switch (info.VersionMenor)
			{
			case 2:
				if (oviex.wSuiteMask == VER_SUITE_WH_SERVER) {
					strncpy(info.NombreOS, "Windows Home Server", 64);
					info.EsWindowsServer = true;
				} else if (oviex.wProductType == VER_NT_WORKSTATION) {
					strncpy(info.NombreOS, "Windows XP Professional, x64 Edition", 64);
				} else {
					strncpy(info.NombreOS, "Windows Server 2003", 64);
					info.EsWindowsServer = true;
				}
				break;

			case 1:
				switch (oviex.wServicePackMajor)
				{
				case 3:
					strncpy(info.NombreOS, "Windows XP SP3", 64);
					break;

				case 2:
					strncpy(info.NombreOS, "Windows XP SP2", 64);
					break;

				case 1:
					strncpy(info.NombreOS, "Windows XP SP1", 64);
					break;
				}
				break;
			}
		}
	}

	void Sistema::ObtenerDatosEquipo() 
	{
		DWORD tam = 64;
		GetComputerNameA(info.NombreEquipo, &tam);

		HKEY opened;
		DWORD copiados;
		static char Buffer[64];

		DWORD type = REG_SZ;
		int r = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\SystemInformation", 0, KEY_QUERY_VALUE, &opened);	
		if (r == NO_ERROR) {
			RegQueryValueExA(opened, "SystemManufacturer", 0, &type, (LPBYTE)info.FabricanteEquipo, &copiados);
			RegQueryValueExA(opened, "SystemProductName", 0, &type, (LPBYTE)info.ModeloEquipo, &copiados);
			RegCloseKey(opened);
		}
		unsigned long long memoria;
		GetPhysicallyInstalledSystemMemory(&memoria);
		info.MemoriaFisica = (memoria / 1024);

	}

	Sistema::Sistema()
	{
		// Se inicializan todos los datos de la estructura
		//! Almacenará el nombre del usuario actual
		DWORD bufflen = 64;
		GetUserNameA(info.NombreUsuario, &bufflen);
		ObtenerMAC();
		ObtenerVersionWindows();
	}

	stSystemInfoResponse Sistema::getInfo()
	{
		// Se retorna la estructura
		return info;
	}
} 
