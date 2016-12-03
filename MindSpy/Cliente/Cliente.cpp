#include "Cliente.h"
int APIENTRY wWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPWSTR    lpCmdLine,
                     int       nCmdShow)
{
	Conector cn;
	bool r = cn.Conectar("127.0.0.1", 1024);

	if (!r) return 1;

	r = cn.EnviarComando(Datos(MENSAJE, "Hola server!"));
	int error;
	if (!r)
	{
		error = WSAGetLastError();
	}
	return 0;
}