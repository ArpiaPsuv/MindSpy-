#pragma once
#include <windows.h>
namespace MindSpy
{
	
	typedef struct RegData
	{
		/* @example
		 * 	RegData regData;
		 * 	regData = { HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", L"InstallDate" };
		 */

		HKEY rootKey;	 // Claves De reistro predefinidas [HKEY_CLASSES_ROOT, HKEY_CURRENT_USERHKEY_LOCAL_MACHINE, etc..]
		LPWSTR subKey;	// Subclave de la clave especificada por el parámetro rootKey.
		LPWSTR value;	//El nombre del valor de Registro.
	}RegData;


	class WinReg
	{
	public:
		WinReg();
		~WinReg();

		DWORD regReadValue(RegData* regData, LPDWORD typeBuff, PVOID Buffer, DWORD sz);
		
		DWORD getRegDwordValue(RegData* data, void* DataBuff);
		DWORD getRegStrinData(RegData* data, LPWSTR buffer, DWORD bufferSz);

		bool regSetValue(RegData* regData, DWORD valType, const void* buffer, DWORD bufferSize);
		bool setRegValueDword(RegData* rgdata, DWORD data);
		bool deleRegteKey(HKEY key, const LPWSTR subKey);


	private:
		DWORD errcode;
		DWORD type;
	};


}
