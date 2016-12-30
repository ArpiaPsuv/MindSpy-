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
	vector<RegistrySubKeyValueInfo>  buffer = wnreg.GetAllRegSubkeysValue(hkey,
		L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\EasyBCD");

	for (RegistrySubKeyValueInfo i : buffer) {
		wstring tipo = L"";
		switch (i.valuetype)
		{
		case REG_NONE:   
			tipo = L"No value type"; break;
		case REG_SZ:    
			tipo = L"Unicode nul terminated string"; break;
		case REG_EXPAND_SZ:   
			tipo = L"Unicode nul terminated string"; break;
		case REG_BINARY:       
			tipo = L"Free form binary"; break;
		case REG_DWORD:        
			tipo = L"32-bit number"; break;
		case REG_DWORD_BIG_ENDIAN:    
			tipo = L"32-bit number"; break;
		case REG_LINK:                 
			tipo = L"Symbolic Link (unicode)"; break;
		case REG_MULTI_SZ:               
			tipo = L"Multiple Unicode strings"; break;
		case REG_RESOURCE_LIST:         
			tipo = L"Resource list in the resource map"; break;
		case REG_FULL_RESOURCE_DESCRIPTOR: 
			tipo = L"Resource list in the hardware description"; break;
		case REG_QWORD:                   
			tipo = L"64-bit number"; break;
		}
		wcout << L"nombre: " << i.name << std::endl << L"Tipo: " << tipo << std::endl << std::endl;

	}

	return 0;

}