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


	class WinReg
	{
	public:
		WinReg();
		~WinReg();

		DWORD GetRegValue(HKEY	hRootKey, LPWSTR subKey, LPWSTR	value, LPDWORD type, PVOID Buffer, DWORD sz);
		bool RegRetriveDwordValue(HKEY hRootKe, LPWSTR subKey, LPWSTR value, void* buffer);


	private:
		HKEY hKey; // global used in Almost all members...
		REG_ERR_CODE erroc;// global used in Almost all members...
		DWORD btypes;
	};


}
