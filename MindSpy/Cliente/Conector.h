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
		char msg[MAX_BUFFER];
	public:
		Conector();
		~Conector();
		bool Conectar(string IP, UINT32 Puerto);
		bool EnviarComando(char* cmd);
	};
}
