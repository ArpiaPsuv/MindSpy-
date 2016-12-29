#pragma once
#include <windows.h>
#include <vector> // esto no podria ser util, cuando tengamos nuestro propio nucleo de manejos

namespace MindSpy
{

#define MAX_VALUE_NAME 16383

	using namespace std;
	typedef vector<wstring> vstring;
	typedef vector<DWORD> VREG_DWORD;

	typedef struct RegistryQueryInfoW
	{
		WCHAR wcClassName[MAX_PATH];	// buffer that receives  class name
		DWORD szClassName;				// variable that specifies the size of the szClassName buffer
		//DWORD lpReserved;				// RegQueryInfoKey parameter  reserved (NULL)
		DWORD nMaxSubKeyLen;			// variable that receives the number of subkeys
		DWORD szMaxSubkey;				// variable that receives the number of subkeys 
		DWORD szMaxClassLen;			// variable that receives the size of the longest string 
		DWORD nValues;					// variable that receives the number of values that are associated with the key
		DWORD szMaxValueNameLen;		// variable that receives the size of the key's longest value name	
		DWORD szMaxValueLen;			// variable that receives the size of the key's longest value name
		DWORD szSecurityDescriptor;		// variable that receives the size of the key's security descriptor
		FILETIME ftLastWriteTime;		// last write time
	}RegistryQueryInfoW;

	typedef struct RegistrySubKeyValueInfo
	{
		vstring data;
		VREG_DWORD valueype;
	}RegistrySubKeyValueInfo;

	class WinReg
	{
	public:
		WinReg();
		~WinReg();

		DWORD GetRegValue(HKEY	hRootKey, LPWSTR subKey, LPWSTR	value, LPDWORD type, PVOID Buffer, DWORD sz);
		bool RegRetriveDwordValue(HKEY hRootKe, LPWSTR subKey, LPWSTR value, void* buffer);
		bool RegRetriveStringValue(HKEY	hRootKey, LPWSTR subKey, LPWSTR	value, LPWSTR buffer, DWORD bufferSz);

		bool RegQueryInfo(HKEY hRootKe, LPWSTR subKey, RegistryQueryInfoW *regInfo);

		vstring GetAllRegSubkeys(HKEY hRootKey, LPWSTR subKey);
		bool GetAllRegSubkeysValue(HKEY hRootKey, LPWSTR subKey);


	private:
		HKEY hKey; // global used in Almost all members...
		DWORD erroc;// global used in Almost all members...
		DWORD btypes;
	};


}
