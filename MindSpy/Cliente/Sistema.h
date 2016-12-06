#pragma once
#include <string>
#include <Windows.h>
#include <Iphlpapi.h>
#include <VersionHelpers.h>
using namespace std;

namespace MindSpy
{
	struct stSystemInfo {
		UINT32 Build;
		UINT16 VersionMayor;
		UINT16 VersionMenor;
		UINT16 Arquitectura;
		CHAR NombreOS[64];
		CHAR MAC[18];
		CHAR NombreUsuario[64];
		bool EsWindowsServer;
	};

	class Sistema
	{
	private:
		stSystemInfo info ;
		void ObtenerMAC();
		void ObtenerVersionWindows();

	public:
		Sistema();
		stSystemInfo getInfo();
	};
}