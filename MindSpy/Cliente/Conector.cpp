#include "Conector.h"

namespace MindSpy 
{
	Conector::Conector() {
		WSAStartup(MAKEWORD(2, 2), &wsd);
		sckt = socket(AF_INET, SOCK_STREAM, 0);
	}

	Conector::~Conector() {
		closesocket(sckt);
	}

	bool Conector::Conectar(string IP, UINT32 Puerto)
	{
		server.sin_addr.s_addr = inet_addr(IP.c_str());
		server.sin_family = AF_INET;
		server.sin_port = htons(Puerto);
		return connect(sckt, (sockaddr*)&server, sizeof(server)) != SOCKET_ERROR;
		
	}
	bool Conector::EnviarComando(char* cmd)
	{
		return send(sckt, cmd, strlen(cmd)+1, 0) != SOCKET_ERROR;
	}
};