/**
* @file Sistema.h
* @author Carlos D. Alvarez
* @date 10/12/2016
* @brief Define la clase Sistema
*/
#pragma once
#include <string>
#include <Windows.h>
#include <Iphlpapi.h>
#include <stdio.h>
#include "../Datos.h"
using namespace std;

namespace MindSpy
{
	/**
	* @class Sistema
	* @brief Clase que recopila información del sistema del cliente
	*/
	class Sistema
	{
	private:
		//! Variable para guardar la información obtenida
		stSystemInfoResponse info;

		/**
		* @brief Obtener la MAC de la interfaz de red conectada al router
		*/
		void ObtenerMAC();

		/**
		* @brief Cargar la version del OS
		*/
		void ObtenerVersionWindows();

		void ObtenerDatosEquipo();

	public:
		/**
		* @brief Obtener identificador único de la máquina
		*/
		const char* ObtenerHwid();

		/**
		* @brief Constructor, llama a los métodos informativos y llena la estructura
		*/
		Sistema();

		/**
		* @brief Devuelve la estructura con la información
		*/
		stSystemInfoResponse getInfo();
	};
}