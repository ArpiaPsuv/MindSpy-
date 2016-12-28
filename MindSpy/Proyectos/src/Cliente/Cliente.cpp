/**
* @file Cliente.cpp
* @author Carlos D. Alvarez
* @date 10/12/2016
* @brief Contiene el punto de entrada del programa
*/

#include "Cliente.h"
#include "Registro.h"

/**
* @fn main
* @brief Punto de entrada del programa
*/

DWORD GetRegValue(
	HKEY hRootKey,
	LPWSTR subKey,
	LPWSTR value,
	LPDWORD type,
	PVOID Buffer,
	DWORD sz)
{


#if _WIN64
	REGSAM  AccessRights = KEY_QUERY_VALUE | KEY_WOW64_64KEY;
#else
	REGSAM  AccessRights = KEY_QUERY_VALUE | KEY_WOW64_64KEY;;
#endif



	DWORD retVal = false;
	if (RegOpenKeyExW(hRootKey, subKey, 0, AccessRights, &hRootKey) == ERROR_SUCCESS)
	{
		if (RegQueryValueExW(hRootKey, value, NULL, type, (LPBYTE)Buffer, &sz) == ERROR_SUCCESS)retVal = true;
		RegCloseKey(hRootKey);
	}

	return  retVal;
}
//int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR    lpCmdLine, int nCmdShow)
int main()
{

	//HKEY_LOCAL_MACHINE\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion

	DWORD dsfdf= MAX_PATH;
	WCHAR InstallDateBuffer[5000];;
	DWORD type;
	HKEY hKey;


	DWORD STATUS = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", NULL, KEY_QUERY_VALUE , &hKey);
	  if (STATUS != ERROR_SUCCESS)RegCloseKey(hKey);
	  else if (STATUS == ERROR_SUCCESS)
	{
		STATUS = RegQueryValueExW(hKey, L"CurrentType", 0, &type, LPBYTE(&InstallDateBuffer), &dsfdf);
		if (STATUS != ERROR_SUCCESS )RegCloseKey(hKey);
		else RegCloseKey(hKey);
	}

	  //std::wcout << InstallDateBuffer;
	 
	/*
	time_t tm;
	tm = InstallDateBuffer;
	const struct tm *lpTm;
	lpTm = localtime((const time_t*)&tm);
	char tmp[128];
	strftime(tmp, sizeof(tmp), "%z %d-%m-%Y ", lpTm);
	MessageBoxW(NULL, (LPCWSTR)tmp, L"Install Time", MB_OK);
	*/

	/*
	RegData regData;
	WinReg winreg;
	DWORD  osInstallDate = 0;;
	regData = { HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", L"InstallDate" };

	// Prueba  obteniendo la fecha de instlacion del sistema operativo
	winreg.getRegDwordValue(&regData, &osInstallDate);
	time_t tm;
	tm = osInstallDate;
	const struct tm *lpTm;
	lpTm = localtime((const time_t*)&tm);
	char tmp[128];
	strftime(tmp, sizeof(tmp), "%z %d-%m-%Y ", lpTm);
	MessageBox(NULL, (LPCSTR)tmp, "Install Time", MB_OK);

	// Prueba  obteniendo La Raiz del sistema
	WCHAR systemRoot[MAX_PATH];
	regData = { HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", L"SystemRoot" };
	winreg.getRegStrinData(&regData, systemRoot, MAX_PATH);
	MessageBoxW(NULL, systemRoot, L"System Root", MB_OK);

	// Prueba  creando un nuevo valor de registro  Dword

	DWORD   Spyversion = 0;
	WCHAR   SpyversionW[30];
	regData = { HKEY_CURRENT_USER, L"SOFTWARE\\MinSpy", L"version"};
	winreg.setRegValueDword(&regData, 1);

	regData.rootKey = HKEY_CURRENT_USER;
	winreg.getRegDwordValue(&regData, &Spyversion);
	wsprintfW(SpyversionW, L"%i", Spyversion);
	MessageBoxW(NULL, SpyversionW, L"MinSpy version", MB_OK);

	//actualizamos la version
	regData.rootKey = HKEY_CURRENT_USER;
	winreg.setRegValueDword(&regData, 3);
	regData.rootKey = HKEY_CURRENT_USER;
	winreg.getRegDwordValue(&regData, &Spyversion);
	wsprintfW(SpyversionW, L"%i", Spyversion);
	MessageBoxW(NULL, SpyversionW, L"MinSpy version actualizada", MB_OK);


*/

	// Conectar...
	Conector cn;
	// Mientras la conexion siga vigente
	while (cn.Listo())
	{
	Sleep(1000);
	}
	return 0;
}