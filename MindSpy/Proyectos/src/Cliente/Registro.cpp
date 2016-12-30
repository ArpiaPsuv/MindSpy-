#include "Registro.h"
#include <iostream>


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

	bool WinReg::RegQueryInfo(HKEY hRootKe,
		LPWSTR subKey,
		RegistryQueryInfoW* regInfo)
	{
		RtlSecureZeroMemory(regInfo, sizeof(RegistryQueryInfoW));

		erroc = RegOpenKeyExW(hRootKe, subKey, 0, KEY_READ, &hRootKe);

		if (erroc == ERROR_FILE_NOT_FOUND)return false;
		regInfo->szClassName = MAX_ZONE_PATH;
		RegQueryInfoKeyW(
			hRootKe, // key handle 
			regInfo->wcClassName, // buffer for class name 
			&regInfo->szClassName, // size of class string 
			0, // reserved 
			&regInfo->nMaxSubKeyLen, // number of subkeys 
			&regInfo->szMaxSubkey, // longest subkey size 
			&regInfo->szMaxClassLen, // longest class string 
			&regInfo->nValues, // number of values for this key 
			&regInfo->szMaxValueNameLen, // longest value name 
			&regInfo->szMaxValueLen, // longest value data 
			&regInfo->szSecurityDescriptor, // security descriptor 
			&regInfo->ftLastWriteTime); // last write time

		//https://msdn.microsoft.com/en-us/library/windows/desktop/ms724256(v=vs.85).aspx
		if (!regInfo->nMaxSubKeyLen)return false;

		//RegCloseKey(hKey);
	}

	vstring WinReg::GetAllRegSubkeys(
		HKEY hRootKey,
		LPWSTR subKey)
	{
		RegistryQueryInfoW *querys = (RegistryQueryInfoW*)malloc(sizeof(RegistryQueryInfoW));


		RegQueryInfo(hRootKey, subKey, querys);
		erroc = RegOpenKeyExW(hRootKey, subKey, 0, KEY_READ | KEY_WOW64_32KEY, &hRootKey);

		DWORD loop = 0;
		DWORD szBuffer = 0;
		LPWSTR  sbBuffer = new wchar_t[MAX_PATH];

		vector<wstring> data;

		data.resize(0);

		do
		{
			szBuffer = MAX_VALUE_NAME;

			LSTATUS res = RegEnumKeyExW(hRootKey, loop++, sbBuffer, &szBuffer,
				NULL,
				NULL,
				NULL,
				&querys->ftLastWriteTime);


			if (!res)
				data.push_back(wstring(subKey) + L"\\" + sbBuffer);

		} while (querys->nMaxSubKeyLen > loop);
		delete  sbBuffer;

		int szBuffTmp = 0;


		for (int i = 0; i < data.size(); i++)
			szBuffTmp += data[i].length() + 1;


		static LPWSTR tmp;
		LPWSTR offset;
		offset = tmp;
		ZeroMemory(tmp, szBuffTmp);

		for (int i = 0; i < data.size(); i++)
		{
			wcscpy(offset, data[i].c_str());
			offset += data[i].length() + 1;

		} delete tmp;

		return data;
	}

	bool WinReg::GetAllRegSubkeysValue(
		HKEY hRootKey,
		LPWSTR subKey,
		RegistrySubKeyValueInfo *buffer)
	{
		HKEY hkey;

		

		erroc = RegOpenKeyExW(hRootKey, subKey, 0, KEY_READ, &hkey);
		if (erroc != NO_ERROR)
			return false;

		int couter = 0;
		DWORD szValueBuffer = 0;
		LPWSTR  valueBuffer = new wchar_t[MAX_PATH];
			
		vector<wstring> data;
		data.resize(0);

		do
		{
			szValueBuffer = MAX_VALUE_NAME;

			erroc = RegEnumValueW(hkey, couter, valueBuffer, &szValueBuffer, NULL, NULL, NULL, NULL);
			data.push_back(wstring(valueBuffer));
			couter++;
		} while (erroc != ERROR_NO_MORE_ITEMS);

		delete valueBuffer;

		wchar_t*  Dest;
		unsigned int tamDatos = 0;
		for (wstring i: data)
		{
			tamDatos += i.length()+1;
		}

		Dest = new wchar_t[tamDatos];

		wchar_t*offset = Dest;
		for (wstring i : data)
		{
			wcsncpy(offset, i.c_str(), i.length());
			offset[i.length()] = 0;
			offset += i.length() + 1;
		}

		buffer->name = Dest; // Fix me Esto debe cambiarse
		buffer->size= data.size();

		for (size_t i = 0; i < 15; i++)
		{
			//std::wcout << &Dest[i] << endl;
		}
			
		return true;
	}
}
