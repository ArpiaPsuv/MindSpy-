/**
* @file Cliente.cpp
* @author Carlos D. Alvarez
* @date 10/12/2016
* @brief Contiene el punto de entrada del programa
*/

#include "Cliente.h"
/**
* @fn main
* @brief Punto de entrada del programa
*/
//int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR    lpCmdLine, int nCmdShow)
int main()
{
	// Conectar...
	Conector cn;
	// Mientras la conexión siga vigente
	while (cn.Listo())
	{
		Sleep(1000);
	}
	return 0;
}