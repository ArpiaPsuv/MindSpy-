#include "Cliente.h"

//int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR    lpCmdLine, int nCmdShow)
int main()
{
	// Leer la IP desde la consola para conectarse a ella
	string ip;
	cout << "Ingrese la IP: "; cin >> ip;
	// Conectar...
	Conector cn(ip, 9900);
	// Mientras la conexión siga vigente
	while (cn.Listo())
	{
		Sleep(1000);
	}
	return 0;
} 