#pragma once
#include <string>
#include <Windows.h>
#include <Iphlpapi.h>

using namespace std;

namespace MindSpy
{
	class Sistema
	{
	public:
		string ObtenerMAC();
	};
}