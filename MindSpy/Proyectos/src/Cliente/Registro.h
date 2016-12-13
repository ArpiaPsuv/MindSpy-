#pragma once
#include <windows.h>
namespace MindSpy
{
	/**
	* definicion
	*/
	typedef struct RegData
	{
		HKEY rootKey;
		LPWSTR subKey;
		LPWSTR value;
	}RegData;

/**
* una
*/
	class WinReg
	{
	public:
		WinReg();
		~WinReg();

		DWORD regReadValue(RegData* regData, LPDWORD typeBuff, PVOID Buffer, DWORD sz);

		bool regSetValue(RegData* regData, DWORD valType, const void* buffer, DWORD bufferSize);
		DWORD getRegDwordValue(RegData* data, void* DataBuff);
		DWORD getRegStrinData(RegData* data, LPWSTR buffer, DWORD bufferSz);

		bool setRegValueDword(RegData* rgdata, DWORD data);


	private:
		DWORD errcode;
		DWORD type;
	};


}
