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
		cantidadConexiones = 0;
		sConexiones = (PCONEXION)malloc(sizeof(CONEXION));
		cout << "Servidor iniciado correctamente" << endl;
	}

	Servidor::~Servidor() {
		closesocket(s);
		WSACleanup();
		free(sConexiones);
	}

	bool Servidor::Listo() {
		return resultado ? true : false;
	}

	void Servidor::IniciarEscucha() {
		CreateThread(NULL, NULL, &HiloEscuchaStatic, this, NULL, NULL);
	}

	void Servidor::HiloEscucha() {
		listen(s, 3);
		while (true) {
			c = sizeof(struct sockaddr_in);
			sConexiones[cantidadConexiones].c_socket = accept(s, (sockaddr *)&client, &c);

			if (IpRegistrada(inet_ntoa(client.sin_addr)) != IP_NO_REGISTRADA) {
				const char *Mensaje = "Solo se permite un cliente por IP.";
				send(sConexiones[cantidadConexiones].c_socket, Mensaje, strlen(Mensaje), 0);
				cout << "Conexion entrante de " << inet_ntoa(client.sin_addr) << " rechazada, IP ya registrada." << endl;
				continue;
			}

			ZeroMemory(sConexiones[cantidadConexiones].Alias, sizeof(sConexiones[cantidadConexiones].Alias));
			strcpy(sConexiones[cantidadConexiones].IP, inet_ntoa(client.sin_addr));
			sConexiones[cantidadConexiones].ID = cantidadConexiones;
			sConexiones[cantidadConexiones].Activa = false;
			cout << "Conexion aceptada. IP: " << sConexiones[cantidadConexiones].IP << endl;
			sConexiones = (PCONEXION)realloc(sConexiones, sizeof(CONEXION)*(++cantidadConexiones + 1));
			CreateThread(NULL, NULL, &HiloConexionStatic, this, NULL, NULL);
			while (!sConexiones[cantidadConexiones - 1].Activa) Sleep(30);
			EnviarMensaje(sConexiones[cantidadConexiones - 1].IP, "SYSINFO");
		}
	}

	int Servidor::IpRegistrada(const char*IP) {
		for (int i = 0; i <= cantidadConexiones; i++) {
			if (!strcmp(IP, sConexiones[i].IP) && sConexiones[i].Activa) {
				return i;
			}
		}
		return IP_NO_REGISTRADA;
	}

	bool Servidor::EnviarMensaje(const char* IP, const char* Mensaje) {
		int r = IpRegistrada(IP);
		if (r != IP_NO_REGISTRADA) {
			send(sConexiones[r].c_socket, Mensaje, strlen(Mensaje), 0);
			return true;
		}
		return false;
	}

	void Servidor::HiloConexion() {
		int MyID = cantidadConexiones - 1;
		sConexiones[MyID].Activa = true;
		char szBuff[512];
		while (true) {
			int resp = recv(sConexiones[MyID].c_socket, szBuff, sizeof(szBuff), 0);
			if (resp > 0 && strlen(szBuff)) {

				if (szBuff[0] != '/') 
				{
					if (strlen(sConexiones[MyID].Alias)) {
						cout << "Mensaje de" 
							<< sConexiones[MyID].Alias 
							<< " (" << sConexiones[MyID].IP 
							<< "): " << szBuff << endl;
					}
					else 
					{
						cout << "Mensaje de " << sConexiones[MyID].IP 
							<< ": " << szBuff << endl;
					}
				} 
				else
				{
					if (!strcmp(szBuff, "/close")) {
						break;
					}
					else if (!strcmp(szBuff, "/name")) {
						sprintf(szBuff, "Carlos D. Alvarez (%d)", sConexiones[MyID].ID);
						cout << "Enviando nombre..." << endl;
						send(sConexiones[MyID].c_socket, szBuff, sizeof(szBuff), NULL);
					}
				}
			}
			else if (resp == SOCKET_ERROR) {
				break;
			}
			Sleep(30);
		}
		cout << "Cerrando conexion con " << sConexiones[MyID].IP << " (" << sConexiones[MyID].ID << ")..." << endl;
		sConexiones[MyID].Activa = false;
		closesocket(sConexiones[MyID].c_socket);
	}

	bool Servidor::SetAlias(const char* IP, const char* Alias) {
		int r = IpRegistrada(IP);
		if (r != IP_NO_REGISTRADA) {
			strcpy(sConexiones[r].Alias, Alias);
			return true;
		}
		return false;
	}
}