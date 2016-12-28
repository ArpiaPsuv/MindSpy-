#pragma once
#include <windows.h>
namespace MindSpy
{
	
	enum class REG_ERR_CODE
	{
		REG_SUCCESS = ERROR_SUCCESS,
		REG_NOT_FOUND = ERROR_FILE_NOT_FOUND,
		REG_MORE_DATA = ERROR_MORE_DATA
	};

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

	class WinReg
	{
	public:
		WinReg();
		~WinReg();

		DWORD GetRegValue(HKEY	hRootKey, LPWSTR subKey, LPWSTR	value, LPDWORD type, PVOID Buffer, DWORD sz);
		bool RegRetriveDwordValue(HKEY hRootKe, LPWSTR subKey, LPWSTR value, void* buffer);
		bool RegRetriveStringValue(HKEY	hRootKey, LPWSTR subKey, LPWSTR	value, LPWSTR buffer, DWORD bufferSz);


	private:
		HKEY hKey; // global used in Almost all members...
		DWORD erroc;// global used in Almost all members...
		DWORD btypes;
	};


}
