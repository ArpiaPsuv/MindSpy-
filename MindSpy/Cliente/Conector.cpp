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
	}

	Conector::~Conector() {
		closesocket(sckt);
	}

	bool Conector::Listo()
	{
		return Conectado;
	}

	bool Conector::EnviarComando(char* cmd)
	{
		return send(sckt, cmd, strlen(cmd)+1, 0) != SOCKET_ERROR;
	}
};