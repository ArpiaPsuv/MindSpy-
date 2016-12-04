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
		return resultado==1;
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
			EnviarMensaje(Conexiones[id].IP, "SYSINFO");
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

	bool Servidor::EnviarMensaje(const char* IP, const char* Mensaje) {
		int r = IpRegistrada(IP);
		if (r != IP_NO_REGISTRADA) {
			send(Conexiones[r].c_socket, Mensaje, strlen(Mensaje), 0);
			return true;
		}
		return false;
	}

	void Servidor::HiloConexion() {
		int MyID = Conexiones.size() - 1;
		Conexiones[MyID].Activa = true;
		char szBuff[512];
		while (true) {
			int resp = recv(Conexiones[MyID].c_socket, szBuff, sizeof(szBuff), 0);
			if (resp > 0 && strlen(szBuff)) {

				if (szBuff[0] != '/') 
				{
					if (strlen(Conexiones[MyID].Alias)) {
						cout << "Mensaje de" 
							<< Conexiones[MyID].Alias 
							<< " (" << Conexiones[MyID].IP 
							<< "): " << szBuff << endl;
					}
					else 
					{
						cout << "Mensaje de " << Conexiones[MyID].IP 
							<< ": " << szBuff << endl;
					}
				} 
				else
				{
					if (!strcmp(szBuff, "/close")) {
						break;
					}
					else if (!strcmp(szBuff, "/name")) {
						sprintf(szBuff, "Carlos D. Alvarez (%d)", Conexiones[MyID].ID);
						cout << "Enviando nombre..." << endl;
						send(Conexiones[MyID].c_socket, szBuff, sizeof(szBuff), NULL);
					}
				}
			}
			else if (resp == SOCKET_ERROR) {
				break;
			}
			Sleep(30);
		}
		cout << "Cerrando conexion con " << Conexiones[MyID].IP << " (" << Conexiones[MyID].ID << ")..." << endl;
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