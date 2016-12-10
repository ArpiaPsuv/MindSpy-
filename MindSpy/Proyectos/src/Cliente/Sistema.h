#pragma once
#include <string>
#include <Windows.h>
#include <Iphlpapi.h>
#include <VersionHelpers.h>
#include "../Datos.h"
using namespace std;

namespace MindSpy
{
	// Clase que recopila información de sistema
	class Sistema
	{
	private:
		// Variable para guardar la información obtenida
		stSystemInfoResponse info ; 
		// Obtener la MAC de la interfaz de red conectada al router
		void ObtenerMAC();
		// Obtiene datos sobre la copia de Windows instalada
		void ObtenerVersionWindows();

	public:
		// Constructor, llama a los métodos de arriba y llena la estructura
		Sistema();
		// Devuelve la estructura
		stSystemInfoResponse getInfo(); 
	};
}