#include "Cliente.h"

//int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR    lpCmdLine, int nCmdShow)
int main()
{
	// Conectar...
	Conector cn;
	// Mientras la conexi�n siga vigente
	while (cn.Listo())
	{
		Sleep(1000);
	}
	return 0;
} 