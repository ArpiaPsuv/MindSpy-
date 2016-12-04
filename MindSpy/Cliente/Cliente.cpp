#include "Cliente.h"

DWORD WINAPI HiloProc(LPARAM l);

//int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR    lpCmdLine, int nCmdShow)
int main()
{
	Conector cn("127.0.0.1", 9900);
	while (cn.Listo())
	{
		Sleep(1000);
	}
	return 0;
}