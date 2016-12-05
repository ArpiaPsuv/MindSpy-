#pragma once
#include <string>
#include <Windows.h>
#include <Iphlpapi.h>
#include <VersionHelpers.h>
using namespace std;

namespace MindSpy
{
	struct stSystemInfo {
		USHORT VersionMayor;
		USHORT VersionMenor;
		UINT32 Build;
		char NombreOS[64];
		char TipoLicencia[64];
		char MAC[18];
		char NombreUsuario[64];
		char NombreInstalador[64];
		bool EsWindowsServer;
	};

	class Sistema
	{
	private:
		stSystemInfo info = {0};
		void ObtenerMAC();
		void ObtenerVersionWindows();

	public:
		Sistema();
	};
}