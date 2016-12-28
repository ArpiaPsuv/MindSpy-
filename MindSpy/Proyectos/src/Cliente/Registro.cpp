#include "Registro.h"


namespace MindSpy
{

	WinReg::WinReg()
	{

	}

	WinReg::~WinReg()
	{

	}

	DWORD WinReg::GetRegValue(
		HKEY hRootKey,
		LPWSTR subKey,
		LPWSTR value,
		LPDWORD type,
		PVOID Buffer,
		DWORD sz)
	{

		DWORD retVal = false;
		if (RegOpenKeyExW(hRootKey, subKey, 0, KEY_QUERY_VALUE, &hRootKey) == ERROR_SUCCESS)
		{
			if (RegQueryValueExW(hRootKey, value, NULL, type, (LPBYTE)Buffer, &sz) == ERROR_SUCCESS)retVal = true;
			RegCloseKey(hRootKey);
		}

		return  retVal;
	}

	bool WinReg::RegRetriveDwordValue(HKEY hRootKe, LPWSTR subKey, LPWSTR value, void* buffer)
	{
		return false;
	}
}
