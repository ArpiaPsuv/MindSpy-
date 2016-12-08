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

		/*
		*	Nombre: EnviarComando
		*	Par�metros:
		-USHORT SizeofData: Tama�o de la data a enviar
		-USHORT comando: N�mero del comando a enviar
		-BYTE* Data: Puntero a la data que se enviar�. Este puntero puede ser a cualquier estructura
		de datos compatible.
		*	Funci�n: Envia un comando, junto con su data, al cliente
		*	Retorno: bool. Devuelve true si el mensaje se envi�. False en caso contrario.
		*/
		bool EnviarComando(char* IP, UINT32 SizeofData, UINT32 comando, BYTE* Data);
		void IniciarEscucha();
		int IpRegistrada(const char*IP);
	};
}