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


		REGSAM samDesired = KEY_QUERY_VALUE | KEY_WOW64_64KEY;
		DWORD erroc = false;
		if (erroc = RegOpenKeyExW(hRootKey, subKey, 0, samDesired, &hRootKey) == ERROR_SUCCESS)
		{
			if (RegQueryValueExW(hRootKey, value, NULL, type, (LPBYTE)Buffer, &sz) == ERROR_SUCCESS)erroc = true;
			RegCloseKey(hRootKey);
		}

		return  erroc;
	}

	bool WinReg::RegRetriveDwordValue(
		HKEY hRootKe,
		LPWSTR subKey,
		LPWSTR value,
		void* buffer)
	{
		DWORD b;
		DWORD sz = sizeof(DWORD);
		erroc = GetRegValue(hRootKe, subKey, value, &b, (LPBYTE)buffer, sizeof(DWORD));
		if (erroc != ERROR_SUCCESS || b != REG_DWORD)
			return false;

		return true;
	}

	bool WinReg::RegRetriveStringValue(
		HKEY hRootKey,
		LPWSTR subKey,
		LPWSTR value,
		LPWSTR buffer,
		DWORD bufferSz)
	{

		DWORD size = bufferSz * sizeof(WCHAR);
		ZeroMemory(buffer, size);
		erroc = GetRegValue(hRootKey, subKey, value, &btypes, buffer, size);


		if (erroc != true) return false;

		if (size % sizeof(WCHAR) == 0 && (btypes == REG_SZ || btypes == REG_EXPAND_SZ))
		{
			/* fix me
			* https://msdn.microsoft.com/en-us/library/windows/desktop/ms724884(v=vs.85).aspx
			*/

			if (btypes == REG_EXPAND_SZ)
			{
				LPWSTR tmpBuf = (LPWSTR)malloc(size * sizeof(WCHAR));
				ZeroMemory(&tmpBuf, sizeof(WCHAR));
				if (tmpBuf != NULL)memcpy(tmpBuf, buffer, size);
				if (tmpBuf == NULL || ExpandEnvironmentStringsW(tmpBuf, buffer, bufferSz) == 0)return  false;
				free(tmpBuf);

			}
		}

		return  true;
	}
}
