#include "Sistema.h"

namespace MindSpy
{
	void Sistema::ObtenerMAC()
	{
		PIP_ADAPTER_INFO AdapterInfo;
		DWORD dwBufLen = sizeof(AdapterInfo);
		static char mac_addr[18];
		AdapterInfo = (IP_ADAPTER_INFO *)malloc(dwBufLen);

		if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == ERROR_BUFFER_OVERFLOW) {
			free(AdapterInfo);
			AdapterInfo = (IP_ADAPTER_INFO*)malloc(dwBufLen);
		}

		if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == NO_ERROR) {
			do {
				if (strcmp(AdapterInfo->IpAddressList.IpAddress.String, "0.0.0.0"))
				{
					sprintf(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X",
						AdapterInfo->Address[0], AdapterInfo->Address[1],
						AdapterInfo->Address[2], AdapterInfo->Address[3],
						AdapterInfo->Address[4], AdapterInfo->Address[5]);

					strcpy(info.MAC, mac_addr);
					break;
				}
				AdapterInfo = AdapterInfo->Next;
			} while (AdapterInfo);
		}
	}

	void Sistema::ObtenerVersionWindows()
	{
		HKEY Clave;
		int r = RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
			L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 
			NULL, 
			KEY_QUERY_VALUE, 
			&Clave);

		if (r != ERROR_SUCCESS)
			return;

		DWORD TipoDst;
		DWORD Guardados=8;
		char szBuild[8];
		r = RegGetValueA(Clave, "", "CurrentBuild", RRF_RT_ANY, &TipoDst, &szBuild, &Guardados);
		r = RegGetValueA(Clave, "", "ProductName", RRF_RT_ANY, &TipoDst, &info.NombreOS, &Guardados);
		r = RegGetValueA(Clave, "", "EditionID", RRF_RT_ANY, &TipoDst, &info.TipoLicencia, &Guardados);
		r = RegGetValueA(Clave, "", "RegisteredOwner", RRF_RT_ANY, &TipoDst, &info.NombreInstalador, &Guardados);
		RegGetValue(Clave, L"", L"CurrentMajorVersionNumber", RRF_RT_REG_DWORD, &TipoDst, &info.VersionMayor, &Guardados);
		RegGetValue(Clave, L"", L"CurrentMinorVersionNumber", RRF_RT_REG_DWORD, &TipoDst, &info.VersionMenor, &Guardados);
		RegCloseKey(Clave);
		info.Build = atoi(szBuild);

		// TO-DO;  TERMINAR LA LECTURA DEL NOMBRE Y EL REGISTRADOR
	}

	Sistema::Sistema()
	{
		DWORD bufflen = 64;
		GetUserNameA(info.NombreUsuario, &bufflen);
		ObtenerMAC();
		ObtenerVersionWindows();
		if (IsWindowsServer())
			info.EsWindowsServer = true;
		else
			info.EsWindowsServer = false;
	}
}