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



//int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR    lpCmdLine, int nCmdShow)
int main()
{


	//HKEY_LOCAL_MACHINE\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion
	WinReg wnreg;
	//Sistema system;

	//std::wcout << MachineGuidBuffer << std::endl;
	/*
	// Conectar...
	Conector cn;
	// Mientras la conexion siga vigente
	while (cn.Listo())
	{
	Sleep(1000);
	}*/
	HKEY hkey = HKEY_LOCAL_MACHINE;
	RegistrySubKeyValueInfo  buffer;
	wnreg.GetAllRegSubkeysValue(hkey,
		L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Google Chrome", &buffer);

	LPWSTR hi;
	DWORD i = 0;
	for (hi = buffer.name ; i < 3; i++)
	{
		hi += wcslen(buffer.name) + 1;
	
		wcout << hi << std::endl;
	
	}



	return 0;

}