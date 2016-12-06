#pragma once
#include <string>
#include <Windows.h>
#include <Iphlpapi.h>
#include <VersionHelpers.h>
#include "Datos.h"
using namespace std;

namespace MindSpy
{
	class Sistema
	{
	private:
		stSystemInfoResponse info = {0};
		void ObtenerMAC();
		void ObtenerVersionWindows();

	public:
		Sistema();
		stSystemInfoResponse getInfo();
	};
}