#include "Windows.h"
#include "winreg.h"


namespace MindSpy
{

	WinReg::WinReg()
	{


	}

	WinReg::~WinReg()
	{


	}


	/**
	 * \brief
	 * \param regData
	 * \param typeBuff
	 * \param Buffer
	 * \param sz
	 * \return
	 */
	DWORD WinReg::regReadValue(RegData* regData, LPDWORD typeBuff, PVOID Buffer, DWORD sz)
	{

		errcode = RegOpenKeyExW(regData->rootKey, regData->subKey, 0, KEY_QUERY_VALUE | KEY_WOW64_64KEY, &regData->rootKey);
		if (errcode == ERROR_SUCCESS)
		{
			errcode = RegQueryValueExW(regData->rootKey, regData->value, 0, typeBuff, LPBYTE(Buffer), &sz);
			if (errcode != ERROR_SUCCESS)return errcode;

		}
		return errcode;
	}

	bool WinReg::regSetValue(RegData* regData, DWORD valType, const void* buffer, DWORD bufferSize)
	{
		errcode = false;
		if (RegCreateKeyExW(regData->rootKey, regData->subKey, 0, NULL, 0, KEY_SET_VALUE, NULL, &regData->rootKey, NULL) == ERROR_SUCCESS)
		{
			if (RegSetValueExW(regData->rootKey, regData->value, 0, valType, (LPBYTE)buffer, bufferSize) == ERROR_SUCCESS)errcode = true;
			RegCloseKey(regData->rootKey);
		}
		return errcode;
	}




	DWORD WinReg::getRegDwordValue(RegData* data, void* DataBuff)
	{

		if (regReadValue(data, &type, DataBuff, sizeof(DWORD)) != ERROR_SUCCESS ||
			type != REG_DWORD) return false;
		return type == REG_DWORD;
	}

	DWORD WinReg::getRegStrinData(RegData* data, LPWSTR buffer, DWORD bufferSz)
	{

		DWORD sz = bufferSz * sizeof(WCHAR);
		errcode = regReadValue(data, &type, buffer, sz);

		if (sz > 2 && type == REG_EXPAND_SZ)
		{
			LPWSTR tmpBuf[200];
			wcscpy(LPWSTR(tmpBuf), buffer);
			if (tmpBuf == NULL || ExpandEnvironmentStringsW(LPWSTR(tmpBuf), buffer, bufferSz) == 0) return 1;
		}
		return  errcode;

	}

	bool WinReg::setRegValueDword(RegData* rgdata, DWORD data)
	{
		return regSetValue(rgdata, REG_DWORD, (LPBYTE)&data, sizeof(DWORD));

	}
}
