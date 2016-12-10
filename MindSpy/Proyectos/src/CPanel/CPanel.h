/**
* @file CPanel.h
* @author Carlos D. Alvarez
* @date 10/12/2016
* @brief Define la clase CPanel
*/
#pragma once

#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>

#include "Servidor.h"
#include "../Datos.h"

using namespace std;

namespace MindSpy
{
	/**
	* Interactúa con el servidor
	*	
	* Provee una interfaz de comunicación con el servidor.<br>
	* Permite líneas de comandos para enviar parámetros.
	*/
	class CPanel {
	private:
		/**
		* Obtiene el ID del comando pasado como parámetro
		* @param[in] c Nombre del comando
		* @return ID del comando o -1, de no existir coincidencias del texto 
		*		  con alguno de los comandos existentes.
		* @note La función no es case-sensitive
		*/
		int TipoComando(const char*c);
	public:
		/**
		* Inicia el CPanel
		*/
		void Run();
	};
}
