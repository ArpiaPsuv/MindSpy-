#pragma once
#include <windows.h>
#include <string>
#include "Datos.h"
using namespace std;

namespace MindSpy
{

	class Conector {
	private:
		WSADATA wsd;
		SOCKET sckt;
		sockaddr_in server;
		bool Conectado;
		char msg[MAX_BUFFER];
	public:
		Conector(string IP, UINT32 Puerto);
		~Conector();
		bool Listo();
		bool EnviarComando(char* cmd);
	};
}
