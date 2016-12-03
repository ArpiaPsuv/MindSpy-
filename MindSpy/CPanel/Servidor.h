#pragma once
#include <windows.h>
#include <iostream>
using namespace std;
#define NO_PUERTO			9900
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
		} CONEXION, *PCONEXION;
		WSADATA wsa;
		SOCKET s;
		PCONEXION sConexiones;
		sockaddr_in server, client;
		int c, resultado, cantidadConexiones;

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