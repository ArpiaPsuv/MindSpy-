#include "CPanel.h"
#define CANTIDAD_COMANDOS 2
char Comandos[CANTIDAD_COMANDOS][16] = { "ALIAS","ENVIAR" };
enum CMDS {
	ALIAS,
	ENVIAR
};

int TipoComando(const char*c) {
	for (int i = 0; i < CANTIDAD_COMANDOS; i++) {
		if (!strcmp(Comandos[i], c)) return i;
	}
	return -1;
}

//int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
int main()
{
	stringstream str;
	string Comando, Op1, Op2;
	char Mensaje[4096];
	Servidor c;
	if (c.Listo()) c.IniciarEscucha();
	while (true) {
		ZeroMemory(Mensaje, sizeof(Mensaje));
		cin.getline(Mensaje, 4096, '\n');
		str.clear();
		str << Mensaje;
		str >> Comando;
		str >> Op1;
		getline(str, Op2, '\n');

		for (unsigned int i = 0; i < Comando.length(); i++) {
			Comando[i] = toupper(Comando[i]);
		}

		switch (TipoComando(Comando.c_str())) {
		case ALIAS:
			if (c.SetAlias(Op1.c_str(), Op2.c_str())) {
				cout << "Alias cambiado." << endl;
			}
			else {
				cout << "Alias no cambiado." << endl;
			}
			break;

		case ENVIAR:
			if (c.EnviarMensaje(Op1.c_str(), Op2.c_str())) {
				cout << "Mensaje enviado." << endl;
			}
			else {
				cout << "Mensaje no enviado." << endl;
			}
			break;

		default:
			cout << "Comando \"" << Comando << "\" no existente" << endl;
		}
	}
}
