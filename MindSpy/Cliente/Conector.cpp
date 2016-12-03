#include "Conector.h"

namespace MindSpy 
{
	Conector::Conector() {
		WSAStartup(MAKEWORD(2, 2), &wsd);
		sckt = socket(AF_INET, SOCK_STREAM, 0);
	}

	Conector::~Conector() {
	}

	bool Conector::Conectar(string IP, UINT32 Puerto)
	{
		server.sin_addr.s_addr = inet_addr(IP.c_str());
		server.sin_family = AF_INET;
		server.sin_port = htons(Puerto);
		if (connect(sckt, (sockaddr*)&server, sizeof(server)) != SOCKET_ERROR)
		{
			return send(sckt, "1", 2, 0) != SOCKET_ERROR;
		}
		return false;
	}
	bool Conector::EnviarComando(Datos datos)
	{
		if (!datos.data)
			return false;
		
		if (datos.Tipo == MENSAJE)
			return send(sckt, datos.data, strlen(datos.data), 0) != SOCKET_ERROR;

		itoa(datos.Tipo, msg, 1000);
		send(sckt, msg, sizeof(DWORD), 0);

		switch(datos.Tipo)
		{
		case FICHEROS: 
			return !send(sckt, datos.data, sizeof(Fichero), 0);

		default:
			return false;
		}
	}
};