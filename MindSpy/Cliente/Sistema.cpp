#include "Sistema.h"

namespace MindSpy
{
	void Sistema::ObtenerMAC()
	{
		PIP_ADAPTER_INFO AdapterInfo;
		DWORD dwBufLen = sizeof(AdapterInfo);
		static char mac_addr[18];
		AdapterInfo = (IP_ADAPTER_INFO *)malloc(dwBufLen);

		if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == ERROR_BUFFER_OVERFLOW)
		{
			free(AdapterInfo);
			AdapterInfo = (IP_ADAPTER_INFO*)malloc(dwBufLen);
		}

		if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == NO_ERROR)
		{
			do
			{
				if (strcmp(AdapterInfo->IpAddressList.IpAddress.String, "0.0.0.0") &&
					strcmp(AdapterInfo->GatewayList.IpAddress.String, "0.0.0.0"))
				{
					sprintf(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X",
					        AdapterInfo->Address[0], AdapterInfo->Address[1],
					        AdapterInfo->Address[2], AdapterInfo->Address[3],
					        AdapterInfo->Address[4], AdapterInfo->Address[5]);

					strcpy(info.MAC, mac_addr);
					break;
				}
				AdapterInfo = AdapterInfo->Next;
			}
			while (AdapterInfo);
		}
	}

	void Sistema::ObtenerVersionWindows()
	{
		void (__stdcall * RtlGetVersionW)(POSVERSIONINFOEXW);
		RtlGetVersionW = (void(__stdcall*)(POSVERSIONINFOEXW)) GetProcAddress(LoadLibraryW(L"ntdll.dll"), "RtlGetVersion");
		if (!RtlGetVersionW)
			return;

		OSVERSIONINFOEXW oviex;
		oviex.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXW);
		RtlGetVersionW(&oviex);
		FreeLibrary(LoadLibraryW(L"ntdll.dll"));
		info.Build = oviex.dwBuildNumber;
		info.VersionMayor = oviex.dwMajorVersion;
		info.VersionMenor = oviex.dwMinorVersion;
		BOOL wow;

		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId());
		IsWow64Process(hProcess, &wow);
		CloseHandle(hProcess);
		info.Arquitectura = wow ? 64 : 32;

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
				else
					strncpy(info.NombreOS, "Windows Server 2012", 64);
				break;
			case 1:
				if (oviex.wProductType == VER_NT_WORKSTATION)
					strncpy(info.NombreOS, "Windows 7", 64);
				else
					strncpy(info.NombreOS, "Windows Server 2008 R2", 64);
				break;

			case 0:
				if (oviex.wProductType == VER_NT_WORKSTATION)
					strncpy(info.NombreOS, "Windows Vista", 64);
				else
					strncpy(info.NombreOS, "Windows Server 2008", 64);
				break;
			}

			break;
		case 5:
			switch (info.VersionMenor)
			{
			case 2:
				if (oviex.wSuiteMask == VER_SUITE_WH_SERVER)
					strncpy(info.NombreOS, "Windows Home Server", 64);
				else if (oviex.wProductType = VER_NT_WORKSTATION)
					strncpy(info.NombreOS, "Windows XP Professional, x64 Edition", 64);
				else
					strncpy(info.NombreOS, "Windows Server 2003", 64);
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

		/*
		*			MÉTODO POR EL REGISTRO DE WINDOWS
		*
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
		*
		*
		*/
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

	stSystemInfo Sistema::getInfo()
	{
		return info;
	}
}
