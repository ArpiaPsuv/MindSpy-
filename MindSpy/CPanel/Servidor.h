#pragma once
#include <windows.h>
#include <iostream>
#include <vector>
#include <cstdio>
#include "../Cliente/Datos.h"
using namespace std;
#define NO_PUERTO			9900
#define NO_SRV_CMDS			3
#define IP_NO_REGISTRADA	-1

namespace MindSpy 
{
	class Servidor {
	private:
		typedef struct stCon{
			int ID;
			char IP[32];
			char Alias[32];
			bool Activa;
			SOCKET c_socket;
			stSystemInfoResponse SistemaCliente;
		} CONEXION, *PCONEXION;
		WSADATA wsa;
		SOCKET s;
		vector<CONEXION> Conexiones;
		sockaddr_in server, client;
		int c, resultado;

		void HiloConexion();
		void HiloEscucha();

		static DWORD CALLBACK HiloConexionStatic(LPVOID lParam) {
			Servidor * c = reinterpret_cast<Servidor*> (lParam);
			c->HiloConexion();
			return TRUE;
		}

		static DWORD CALLBACK HiloEscuchaStatic(LPVOID lParam) {
			Servidor * c = reinterpret_cast<Servidor*> (lParam);
			c->HiloEscucha();
			return TRUE;
		}
	protected:
	public:
		Servidor();
		~Servidor();

		bool Listo();
		bool SetAlias(const char* IP, const char* Alias);
		bool EnviarMensaje(const char* IP, const char* Mensaje);
		void IniciarEscucha();
		int IpRegistrada(const char*IP);
	};
}