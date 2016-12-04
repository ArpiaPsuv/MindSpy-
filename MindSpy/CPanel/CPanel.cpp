#include "CPanel.h"
#define CANTIDAD_COMANDOS 4
char Comandos[CANTIDAD_COMANDOS][16] = {"ALIAS","ENVIAR", "SYSINFO", "VERSION"};
enum CLNT_CMDS {
	ALIAS,
	ENVIAR,
	SYSINFO,
	VERSION
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
		case CLNT_CMDS::ALIAS:
			if (c.SetAlias(Op1.c_str(), Op2.c_str())) {
				cout << "Alias cambiado." << endl;
			}
			else {
				cout << "Alias no cambiado." << endl;
			}
			break;

		case CLNT_CMDS::ENVIAR:
			if (c.EnviarMensaje(Op1.c_str(), Op2.c_str())) {
				cout << "Mensaje enviado." << endl;
			}
			else {
				cout << "Mensaje no enviado." << endl;
			}
			break;

		case CLNT_CMDS::SYSINFO:
			if (c.EnviarMensaje(Op1.c_str(), Comando.c_str()))
			{
				cout << "Solicitando información de sistema..." << endl;
			} 
			else
			{
				cout << "Solicitud fallida" << endl;
			}
		

		default:
			cout << "Comando \"" << Comando << "\" no existente" << endl;
		}
	}
}
