#include "Cliente.h"

//int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR    lpCmdLine, int nCmdShow)
int main()
{
	string ip;
	cout << "Ingrese la IP: "; cin >> ip;
	Conector cn(ip, 9900);
	while (cn.Listo())
	{
		Sleep(1000);
	}
	return 0;
}