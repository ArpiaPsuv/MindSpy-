#include "Cliente.h"
//int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR    lpCmdLine, int nCmdShow)
int main()
{
	Conector cn;
	char buffer[1024];
	bool r = cn.Conectar("127.0.0.1", 9900);

	if (!r) return 1;

	while (true)
	{
		cin.getline(buffer, 1024, '\n');
		r = cn.EnviarComando(buffer);
	}
	
	return 0;
}